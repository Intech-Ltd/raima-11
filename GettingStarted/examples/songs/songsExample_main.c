/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#include <rdm.h>
#include <internal.h>

#include "songs_fk.h"
#include "songs_nk_dbd.h"
#include "songs_pk_dbd.h"
#include "songs_fk_dbd.h"

#define NUM_GENRES  200
#define NUM_ARTISTS 2000
#define MAX_ALBUMS  4
#define NUM_TRACKS  10

typedef struct {
    const char *desc;
    const char *name;
    const void *dbd;
    size_t      dbd_size;
    int32_t     indexed;
    int32_t     pages;
    int32_t     initialized;
} DB_INFO;

static DB_INFO db_info[] = /*lint !e956 */
{
    { "Full key",    "songs_fk", &songs_fk_dbd, sizeof(songs_fk_dbd), 1, 1500, 0 },
    { "Partial key", "songs_pk", &songs_pk_dbd, sizeof(songs_pk_dbd), 1, 1200, 0 },
    { "No key",      "songs_nk", &songs_nk_dbd, sizeof(songs_nk_dbd), 0, 300, 0 }
};

static int32_t EXTERNAL_FCN songs_main(int32_t, const char *const *);

/* safe gets() */
static char *getstring(
    char   *s,
    int32_t size)
{
    size_t len;

    if (fgets(s, size, stdin) == NULL)
        return NULL;

    len = strlen(s);
    if (s[len-1] == '\n')
        s[len-1] = '\0';

    return s;
}

static void get_random_name(
    char  *name,
    size_t size)
{
    size_t ii = 0;
    size_t len;
    size_t mask = size;

    static int32_t string_num = 0; /*lint !e956 */

    while (mask)
    {
        ii++;
        mask >>= 1;
    }

    mask = (((size_t) 1) << (ii - 1)) - 1;
    len = ((uint32_t) rand() & mask) + (size - mask) - 1;

    for (ii = 0; ii < len; ii++)
    {
        /* use readable ascii characters */
        name[ii] = 59 + (rand() & 63);
    }

    name[len] = '\0';

    if (strlen(name) == 0 && size > 7)
        sprintf(name, "%07d", ++string_num);
}

static int32_t cycle_trans(
    int32_t *itemCount,
    DB_TASK *task)
{
    int32_t status = S_OKAY;

    if (++(*itemCount) % 1000 == 0)
    {
        if ((status = d_trend(task)) != S_OKAY)
            return status;

        status = d_trbegin("populate", task);
    }

    return status;
}

static void my_dberr(
    int32_t errnum,
    char   *errmsg)
{
    /* We may get -905 errors if the user tries to read
       the database after removing it. We don't want these errors to be seen
       by the user.
    */
    if(errnum != S_NOFILE)
    {
        DisplayErrorNow(FALSE, FALSE, "\n%s (errnum = %d)\n", errmsg, errnum);
    }
}

static int32_t create_genres(
    DB_ADDR *genre_dbas,
    int32_t *itemCount,
    DB_TASK *task)
{
    int32_t      status;
    uint32_t     ii;
    struct genre genre_rec;

    memset(&genre_rec, 0, sizeof(struct genre));

    /* create genre records */
    for (ii = 0; ii < NUM_GENRES; ii++)
    {
        get_random_name(genre_rec.genre_name, SIZEOF_GENRE_GENRE_NAME);
        if ((status = d_fillnew(GENRE, &genre_rec, task, CURR_DB)) != S_OKAY)
            return status;

        /* get database address of newly created record, to connect tracks to
         * it later */
        if ((status = d_crget(&genre_dbas[ii], task, CURR_DB)) != S_OKAY)
            return status;

        /* Check to see if we need to end the transaction and start a new one */
        if ((status = cycle_trans(itemCount, task)) != S_OKAY)
            return status;
    }

    return S_OKAY;
}

static int32_t connect_to_set(
    int32_t        set,
    int32_t        num,
    const DB_ADDR *dbas,
    DB_TASK       *task)
{
    int32_t item;
    int32_t status;

    /* Pick a random item from the array */
    item = rand() % num;

    /* Set the picked DB_ADDR as the owner of the specified set */
    if ((status = d_csoset(set, &dbas[item], task, CURR_DB)) != S_OKAY)
        return status;

    /* Connect the current record to that owner */
    return d_connect(set, task, CURR_DB);
}

static int32_t create_tracks(
    const DB_ADDR *genre_dbas,
    int32_t       *itemCount,
    DB_TASK       *task)
{
    int32_t        status;
    uint32_t       ii;
    struct track   track_rec;
    static int32_t track_id = 0; /*lint !e956 */

    memset(&track_rec, 0, sizeof(struct track));

    /* Create the tracks for an artist */
    for (ii = 0; ii < NUM_TRACKS; ii++)
    {
        get_random_name(track_rec.track_name, SIZEOF_TRACK_TRACK_NAME);
        track_rec.track_year = 1960 + (rand() & 0x3f);

        /* Use a non-random track id in this test, as rand() does not
           generate a unique number */
        sprintf(track_rec.track_id, "%d", track_id++);

        if ((status = d_fillnew(TRACK, &track_rec, task, CURR_DB)) != S_OKAY)
            return status;

        /* Connect track to artist */
        if ((status = d_connect(ARTIST_TRACKS, task, CURR_DB)) != S_OKAY)
            return status;

        /* Connect track to album */
        if ((status = d_connect(ALBUM_TRACKS, task, CURR_DB)) != S_OKAY)
            return status;

        /* Connect track to genre */
        status = connect_to_set(GENRE_TRACKS, NUM_GENRES, genre_dbas, task);
        if (status != S_OKAY)
            return status;

        /* Check to see if we need to end the transaction and start a new one */
        if ((status = cycle_trans(itemCount, task)) != S_OKAY)
            return status;
    }

    return S_OKAY;
}

static int32_t create_albums(
    int32_t        num_albums,
    const DB_ADDR *genre_dbas,
    int32_t       *itemCount,
    DB_TASK       *task)
{
    int32_t      status;
    int32_t      ii;
    struct album album_rec;

    memset(&album_rec, 0, sizeof(struct album));

    /* Create albums */
    for (ii = 0; ii < num_albums; ii++)
    {
        get_random_name(album_rec.album_name, SIZEOF_ALBUM_ALBUM_NAME);
        if ((status = d_fillnew(ALBUM, &album_rec, task, CURR_DB)) != S_OKAY)
            return status;

        /* Get database address of newly created record, to connect tracks to
         * it later */
        if ((status = d_setor(ALBUM_TRACKS, task, CURR_DB)) != S_OKAY)
            return status;

        /* Connect this album to the artist */
        if ((status = d_connect(ARTIST_ALBUMS, task, CURR_DB)) != S_OKAY)
            return status;

        /* Create the tracks for this artist */
        if ((status = create_tracks(genre_dbas, itemCount, task)) != S_OKAY)
            return status;

        /* Check to see if we need to end the transaction and start a new one */
        if ((status = cycle_trans(itemCount, task)) != S_OKAY)
            return status;
    }

    return S_OKAY;
}

static int32_t create_artists(
    const DB_ADDR *genre_dbas,
    int32_t       *itemCount,
    DB_TASK       *task)
{
    int32_t       status;
    int32_t       num_albums;
    uint32_t      ii;
    struct artist artist_rec;

    memset(&artist_rec, 0, sizeof(struct artist));

    /* Create artists, albums and tracks */
    for (ii = 0; ii < NUM_ARTISTS; ii++)
    {
        /* Create an artist record */
        get_random_name(artist_rec.artist_name, SIZEOF_ARTIST_ARTIST_NAME);
        if ((status = d_fillnew(ARTIST, &artist_rec, task, CURR_DB)) != S_OKAY)
            return status;

        /* Make this artist current owner of sets ARTIST_TRACKS and
         * ARTIST_ALBUMS */
        if ((status = d_setor(ARTIST_TRACKS, task, CURR_DB)) != S_OKAY)
            return status;

        if ((status = d_setor(ARTIST_ALBUMS, task, CURR_DB)) != S_OKAY)
            return status;

        /* Check to see if we need to end the transaction and start a new one */
        if ((status = cycle_trans(itemCount, task)) != S_OKAY)
            return status;

        /* Generate random number of albums for this artist */
        if ((num_albums = rand() % (MAX_ALBUMS + 1)) == 0)
            continue;

        /* Create the albums for this artist */
        status = create_albums(num_albums, genre_dbas, itemCount, task);
        if (status != S_OKAY)
            return status;
    }

    return S_OKAY;
}

static int32_t create_database_contents(
    DB_TASK *task)
{
    int32_t status;
    int32_t itemCount = 0;
    DB_ADDR genre_dbas[NUM_GENRES];

    /* Erase any existing data in the database */
    if ((status = d_initialize(task, CURR_DB)) != S_OKAY)
        return status;

    /* Start a new transaction */
    if ((status = d_trbegin("populate", task)) != S_OKAY)
        return status;

    /* Create the genres, getting a list of DB_ADDRs for later */
    if ((status = create_genres(genre_dbas, &itemCount, task)) != S_OKAY)
    {
        d_trabort(task);
        return status;
    }

    /* Create the artists (and albums and tracks) */
    if ((status = create_artists(genre_dbas, &itemCount, task)) != S_OKAY)
    {
        d_trabort(task);
        return status;
    }

    /* End the transaction */
    d_trend(task);

    return S_OKAY;
}

/* Create an im-memory database and populate it with data */
static int32_t populate_database(
    DB_INFO *dbi)
{
    DB_TASK *task;
    int32_t  status;

    srand((uint32_t) time(NULL));

    /* Open a new task */
    if ((status = d_opentask(&task)) != S_OKAY)
        return status;

    /* Setup initial information for the new task */
    if ((status = d_setpages(dbi->pages, 0, task)) != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    if ((status = d_set_dberr((ERRORPROC) my_dberr, task)) != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    /* Open the database */
    status = d_open_ptr(dbi->name, "o", dbi->dbd, (int32_t) dbi->dbd_size, task);
    if (status != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    /* Create all the database contents (genres, artists, albums, and tracks) */
    status = create_database_contents(task);

    /* Close the database and task */
    d_close(task);
    d_closetask(task);
    dbi->initialized = 1;
    return status;
}

/* Read one track record and its owner through specified set (artist or genre
   record). The track record must be the current record in the database.
*/
static int32_t read_track_and_owner(
    int32_t  set,
    DB_TASK *task)
{
    int32_t       status;
    struct track  trec;
    struct artist arec;
    struct genre  grec;

    if ((status = d_recread(&trec, task, CURR_DB)) != S_OKAY)
        return status;

    if ((status = d_findco(set, task, CURR_DB)) != S_OKAY)
        return status;

    if (set == ARTIST_TRACKS)
    {
        if ((status = d_recread(&arec, task, CURR_DB)) != S_OKAY)
            return status;

        /*
            printf("%-10.10s%-38.38s%-28.28s\n", trec.track_id, trec.track_name,
                    arec.artist_name);
        */
    }
    else if (set == GENRE_TRACKS)
    {
        if ((status = d_recread(&grec, task, CURR_DB)) != S_OKAY)
            return status;

        /*
            printf("%-10.10s%-38.38s%-28.28s\n", trec.track_id, trec.track_name,
                    grec.genre_name);
        */
    }

    return S_OKAY;
}

static int32_t read_tracks_sequentially(
    uint32_t *count,
    DB_TASK  *task)
{
    int32_t status;

    *count = 0;

    for (status = d_recfrst(TRACK, task, CURR_DB);
             status == S_OKAY;
             status = d_recnext(task, CURR_DB)) {

        if ((status = read_track_and_owner(ARTIST_TRACKS, task)) != S_OKAY)
            return status;

        (*count)++;
    }

    if (status == S_NOTFOUND) /*lint !e850 */
        status = S_OKAY;

    return status;
}

/* Read all track records in database, in physical order (i.e. creation order), and
   read associated artist record
*/
static int32_t read_database_in_record_order(
    const DB_INFO *dbi,
    uint32_t      *count)
{
    DB_TASK *task;
    int32_t  status;

    /* create a database task and open the database */
    if ((status = d_opentask(&task)) != S_OKAY)
        return status;

    if ((d_set_dberr((ERRORPROC)my_dberr, task)) != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = d_open_ptr(dbi->name, "o", dbi->dbd, (int32_t) dbi->dbd_size, task);
    if (status != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = read_tracks_sequentially(count, task);

    /* close the database and task */
    d_close(task);
    d_closetask(task);

    return status;
}

static int32_t read_tracks_by_index(
    int32_t   key,
    uint32_t *count,
    DB_TASK  *task)
{
    int32_t status;

    *count = 0;

    for (status = d_keyfrst(key, task, CURR_DB);
             status == S_OKAY;
             status = d_keynext(key, task, CURR_DB))
    {
        if ((status = read_track_and_owner(ARTIST_TRACKS, task)) != S_OKAY)
            return status;

        (*count)++;
    }

    if (status == S_NOTFOUND) /*lint !e850 */
        status = S_OKAY;

    return status;
}

/* Read all track records in database, in key order, and
   read associated artist record
*/
static int32_t read_database_in_key_order(
    const DB_INFO *dbi,
    int32_t        key,
    uint32_t      *count)
{
    DB_TASK *task;
    int32_t  status;

    /* create a database task and open the database */
    if ((status = d_opentask(&task)) != S_OKAY)
        return status;

    if ((status = d_set_dberr((ERRORPROC)my_dberr, task)) != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = d_open_ptr(dbi->name, "o", dbi->dbd, (int32_t) dbi->dbd_size, task);
    if (status != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = read_tracks_by_index(key, count, task);

    /* close the database and task */
    d_close(task);
    d_closetask(task);

    return status;
}

static int32_t read_tracks_by_set(
    int32_t   key,
    int32_t   set,
    uint32_t *count,
    DB_TASK  *task)
{
    int32_t status;

    *count = 0;

    /* go through all key values (artist or genre) */
    for (status = d_keyfrst(key, task, CURR_DB);
            status == S_OKAY;
            status = d_keynext(key, task, CURR_DB))
    {
        if ((status = d_setor(set, task, CURR_DB)) != S_OKAY)
            return status;

        /* go through all tracks connected to this artist or genre */
        for (status = d_findfm(set, task, CURR_DB);
                status == S_OKAY;
                status = d_findnm(set, task, CURR_DB)) /*lint !e445 */
        {
            if ((status = read_track_and_owner(set, task)) != S_OKAY)
                return status;

            (*count)++;
        }

        if (status != S_EOS) /*lint !e850 */
            return status;
    }

    if (status == S_NOTFOUND) /*lint !e850 */
        status = S_OKAY;

    return status;
}

/* Read all track records in database, in key and set order, and
   read associated artist record. Use the specified key to get
   owner record, then use the specified set to get track records.
*/
static int32_t read_database_in_set_order(
    const DB_INFO *dbi,
    int32_t        key,
    int32_t        set,
    uint32_t      *count)
{
    DB_TASK *task;
    int32_t  status;

    /* create a database task and open the database */
    if ((status = d_opentask(&task)) != S_OKAY)
        return status;

    if ((d_set_dberr((ERRORPROC)my_dberr, task)) != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = d_open_ptr(dbi->name, "o", dbi->dbd, (int32_t) dbi->dbd_size, task);
    if (status != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = read_tracks_by_set(key, set, count, task);

    /* close the database and task */
    d_close(task);
    d_closetask(task);

    return status;
}

static int32_t remove_database(
    DB_INFO *dbi)
{
    DB_TASK *task;
    int32_t  status;

    /* create a database task and open the database */
    if ((status = d_opentask(&task)) != S_OKAY)
        return status;

    if ((d_set_dberr((ERRORPROC)my_dberr, task)) != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = d_open_ptr(dbi->name, "o", dbi->dbd, (int32_t) dbi->dbd_size, task);
    if (status != S_OKAY)
    {
        d_closetask(task);
        return status;
    }

    status = d_destroy(dbi->name, task);    /* delete all database files */

    /* close the database and task */
    d_close(task);
    d_closetask(task);

    return status;
} /*lint !e818 */

static void print_menu(
    const void *dbd)
{
    printf ("\nSONGS commands:\n");
    printf ("   F - Use Full Keys %s\n", dbd == &songs_fk_dbd ? "(default)" : "");
    printf ("   P - Use Partial Keys %s\n", dbd == &songs_pk_dbd ? "(default)" : "");
    printf ("   N - Use No Keys %s\n", dbd == &songs_nk_dbd ? "(default)" : "");
    printf ("   p - Populate Database\n");
    printf ("   t - Read All Tracks\n");
    printf ("   i - Read All Tracks by ID\n");
    printf ("   n - Read All Tracks by Name\n");
    printf ("   a - Read All Tracks by Artist\n");
    printf ("   g - Read All Tracks by Genre\n");
/* removed option
    printf ("   r - Remove Database from RAM\n");
*/
    printf ("   q - Quit\n");
    printf ("enter command: ");
    fflush (stdout);
}

static int32_t dbnotinitialized( void )
{
    printf("This database has not been populated\n");
    return -1;
}

static int32_t read_db_record_order(
    DB_INFO *dbi)
{
    int32_t  status;
    uint32_t record_count;

    if (!dbi->initialized)
        return dbnotinitialized();
    status = read_database_in_record_order(dbi, &record_count);
    printf("%u records\n", record_count);

    return status;
} /*lint !e818 */

static int32_t read_db_track_id_order(
    DB_INFO *dbi)
{
    int32_t  status;
    uint32_t record_count;

    if (!dbi->initialized)
        return dbnotinitialized();
    status = read_database_in_key_order(dbi, TRACK_TRACK_ID, &record_count);
    printf("%u records\n", record_count);

    return status;
} /*lint !e818 */

static int32_t read_db_track_name_order(
    DB_INFO *dbi)
{
    int32_t  status;
    uint32_t record_count;

    if (!dbi->initialized)
        return dbnotinitialized();
    status = read_database_in_key_order(dbi, TRACK_TRACK_NAME, &record_count);
    printf("%u records\n", record_count);

    return status;
} /*lint !e818 */

static int32_t read_db_artist_name_order(
    DB_INFO *dbi)
{
    int32_t  status;
    uint32_t record_count;

    if (!dbi->initialized)
        return dbnotinitialized();
    status = read_database_in_set_order(dbi, ARTIST_ARTIST_NAME, ARTIST_TRACKS, &record_count);
    printf("%u records\n", record_count);

    return status;
} /*lint !e818 */

static int32_t read_db_genre_order(
    DB_INFO *dbi)
{
    int32_t  status;
    uint32_t record_count;

    if (!dbi->initialized)
        return dbnotinitialized();
    status = read_database_in_set_order(dbi, GENRE_GENRE_NAME, GENRE_TRACKS, &record_count);
    printf("%u records\n", record_count);

    return status;
} /*lint !e818 */

typedef int32_t (*ACTION_FCN)(DB_INFO *);

typedef struct {
    const char *name;
    ACTION_FCN  fcn;
    int16_t     need_index;
    int16_t     db_ndx;
} ACTION_INFO;

static const ACTION_INFO action_info[] =
{
    { NULL,                  NULL,                      0, 0 },
    { NULL,                  NULL,                      0, 1 },
    { NULL,                  NULL,                      0, 2 },
    { "populate",            populate_database,         0, -1 },
    { "remove",              remove_database,           0, -1 },
    { "read",                read_db_record_order,      0, -1 },
    { "read by track ID",    read_db_track_id_order,    1, -1 },
    { "read by track name",  read_db_track_name_order,  1, -1 },
    { "read by artist name", read_db_artist_name_order, 1, -1 },
    { "read by genre",       read_db_genre_order,       1, -1 },
};

static const ACTION_INFO *getAction(
    const DB_INFO *dbi)
{
    uint32_t ndx;
    char     cmd[20];

    for (;;)
    {
        print_menu(dbi->dbd);
        if (getstring(cmd, sizeof(cmd)) == NULL)
            return NULL;

        ndx = (uint32_t) -1;

        switch (cmd[0])
        {
            case 'F': ndx = 0; break;
            case 'P': ndx = 1; break;
            case 'N': ndx = 2; break;
            case 'p': ndx = 3; break;
            /* removed option
            case 'r': ndx = 4; break;
            */
            case 't': ndx = 5; break;
            case 'i': ndx = 6; break;
            case 'n': ndx = 7; break;
            case 'a': ndx = 8; break;
            case 'g': ndx = 9; break;
            case 'q':
            case 'Q': return NULL;
            default : printf("*** incorrect entry -- re-enter\n"); break;
        }

        if (ndx != (uint32_t) -1)
            break;
    }

    return &action_info[ndx];
}

static int32_t EXTERNAL_FCN songs_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t            status;
    uint32_t           start_time;
    uint32_t           end_time;
    DB_INFO           *dbi = &db_info[0];
    const ACTION_INFO *action;
    TFS_HANDLE         hTFS;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    d_tfsinit(NULL, NULL, &hTFS);

    for (;;)
    {
        if ((action = getAction(dbi)) == NULL)
            break;

        if (action->fcn == NULL)
        {
            dbi = &db_info[action->db_ndx];
            continue;
        }

        if (action->need_index && !dbi->indexed)
        {
            printf("*** database has no indexes!\n");
            continue;
        }

        start_time = psp_timeMilliSecs();

        status = (action->fcn)(dbi);

        end_time = psp_timeMilliSecs();

        if (status == S_OKAY)
        {
            printf("\n# %s database %s succeeded,\n# time (milliseconds): %u\n",
                    dbi->desc, action->name, end_time - start_time);
        }
        else
            printf("%s database %s failed, error %d\n", dbi->desc,
                   action->name, status);
    }
    d_tfsterm(hTFS);
    return 0;
}

PSP_STARTUP_EXAMPLE(songs)
