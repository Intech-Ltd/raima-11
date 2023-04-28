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

#include "rdm.h"
#include "blarc.h"
#include "blarc_dbd.h"

static const LOCK_REQUEST file_info_r[] = { {FILE_INFO, 'r'} };
static const LOCK_REQUEST file_info_w[] = { {FILE_INFO, 'w'} };

typedef struct _blarc_ctx
{
    const char     *docroot;
    TFS_PARAMS      tparams;
    cmdline_status  cmd_status;
    TFS_HANDLE      hTFS;
    DB_TASK        *task;
} BLARC_CTX;

typedef enum { CMD_ADD, CMD_LIST, CMD_DELETE, CMD_EXTRACT } COMMANDS;

#define BUF_SIZE 4096

/* ===========================================================================
 *  Store the data in the database.  For this function a transaction is
 *  already active and the appropriate locks are held.
 */
static int32_t store_a_file(
    const char *category,
    const char *fn,
    FILE       *fp,
    BLARC_CTX  *ctx)
{
    int32_t                   rdm_status;
    uint32_t                  sz;
    char                      buf[BUF_SIZE];
    struct file_info_cat_file cf;
    struct file_info          fi;

    memset(&fi, 0, sizeof(fi));
    strcpy(fi.category, category);
    strcpy(fi.base_name, fn);

    memset(&cf, 0, sizeof(cf));
    strcpy(cf.category, category);
    strcpy(cf.base_name, fn);

    if (d_keyfind(FILE_INFO_CAT_FILE, &cf, ctx->task, CURR_DB) == S_OKAY)
    {
        DisplayMessageNow(FALSE, FALSE, "Replacing %s %s\n", cf.category, cf.base_name);
    }
    else
    {
        DisplayMessageNow(FALSE, FALSE, "Creating %s %s\n", cf.category, cf.base_name);
        if ((rdm_status = d_fillnew(FILE_INFO, &fi, ctx->task, CURR_DB)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error adding FILE_INFO record (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            return rdm_status;
        }
    }

    while ((sz = (uint32_t) fread(buf, 1, BUF_SIZE, fp)) > 0)
    {
        if ((rdm_status = d_blobwrite(FILE_INFO_FILE_CONTENTS, buf, sz, ctx->task, CURR_DB)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error writing to BLOB (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            return rdm_status;
        }
    }

    return S_OKAY;
}

/* ===========================================================================
 *  Store the data in the database.  The function creates the transaction and
 *  applies the appropriate locks and then calls store_a_file() to do the work.
 */
static int32_t store_file(
    const char *category,
    const char *fn,
    FILE       *fp,
    BLARC_CTX  *ctx)
{
    int32_t rdm_status;

    if ((rdm_status = d_trbegin("newfile", ctx->task)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error starting a transaction (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    if ((rdm_status = d_lock(1, file_info_w, ctx->task, CURR_DB)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Failed to acquire locks (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        d_trabort(ctx->task); /*lint !e534 */
        return rdm_status;
    }

    if ((rdm_status = store_a_file(category, fn, fp, ctx)) != S_OKAY)
    {
        d_trabort(ctx->task); /*lint !e534 */
        return rdm_status;
    }

    if ((rdm_status = d_trend(ctx->task)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error ending transaction (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }
    return rdm_status;
}

/* ===========================================================================
 *  Delete just the one specified file for the category.  For this function a
 *  transaction is already active and the appropriate locks are held.
 */
static int32_t delete_one_file(
    const char *category,
    const char *fn,
    BLARC_CTX  *ctx)
{
    int32_t                   rdm_status;
    struct file_info_cat_file cf;

    memset(&cf, 0, sizeof(cf));
    strcpy(cf.category, category);
    strcpy(cf.base_name, fn);

    if ((rdm_status = d_keyfind(FILE_INFO_CAT_FILE, &cf, ctx->task, CURR_DB)) != S_OKAY)
    {
        if(rdm_status == S_NOTFOUND)
        {
            DisplayMessageNow(FALSE, FALSE, "Cannot find %s %s\n", cf.category, cf.base_name);
        }
        else
        {
            DisplayErrorNow(FALSE, FALSE, "Error performing key lookup (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        }
        return rdm_status;
    }

    DisplayMessageNow(FALSE, FALSE, "Deleting %s %s\n", cf.category, cf.base_name);
    if ((rdm_status = d_delete(ctx->task, CURR_DB)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error deleting record (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
    }

    return S_OKAY;
}

/* ===========================================================================
 *  Delete all files for a category.  For this function a transaction is
 *  already active and the appropriate locks are held.
 */
static int32_t delete_all_files(
    const char *category,
    BLARC_CTX  *ctx)
{
    int32_t                   rdm_status;
    struct file_info_cat_file cf;

    memset(&cf, 0, sizeof(cf));
    strcpy(cf.category, category);

    /* This d_keyfind should fail with S_NOTFOUND as we have only specified
     * part of the key, but it positions the key so the d_keynext will give
     * us any keys that match the category. */
    rdm_status = d_keyfind(FILE_INFO_CAT_FILE, &cf, ctx->task, CURR_DB);
    if (rdm_status != S_OKAY && rdm_status != S_NOTFOUND)
    {
        DisplayErrorNow(FALSE, FALSE, "Error looking up category (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    /* Loop on the key until we reach the end of the keys or a new category,
     * deleting each record. */
    while ((rdm_status = d_keynext(FILE_INFO_CAT_FILE, ctx->task, CURR_DB)) == S_OKAY) /*lint !e838 */
    {
        if ((rdm_status = d_keyread(&cf, ctx->task)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error reading key value (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            return rdm_status;
        }

        /* Check here to make sure we are still in the correct category */
        if (strcmp(cf.category, category) != 0)
            break;

        DisplayMessageNow(FALSE, FALSE, "Deleting %s %s\n", cf.category, cf.base_name);
        if ((rdm_status = d_delete(ctx->task, CURR_DB)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error deleting record (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            break;
        }
    }

    /* We expect to get an S_NOTFOUND status if we have reached the end of the
     * keys.  Anything other than S_NOTFOUND or S_OKAY indicates an error */
    if (rdm_status != S_NOTFOUND && rdm_status != S_OKAY)
    {
        return rdm_status;
    }

    return S_OKAY;
}

/* ===========================================================================
 *  Delete files.  This function setups up a transaction and acquires the
 *  appropriate locks.  It then calls either delete_one_file or
 *  delete_all_files.
 */
static int32_t delete_file(
    const char *category,
    const char *fn,
    BLARC_CTX  *ctx)
{
    int32_t rdm_status;

    if ((rdm_status = d_trbegin("newfile", ctx->task)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error starting transaction (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    if ((rdm_status = d_lock(1, file_info_w, ctx->task, CURR_DB)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to acquire locks (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        d_trabort(ctx->task); /*lint !e534 */
        return rdm_status;
    }

    if (fn)
    {
        rdm_status = delete_one_file(category, fn, ctx);
    }
    else
    {
        rdm_status = delete_all_files(category, ctx);
    }

    if (rdm_status != S_OKAY)
    {
        d_trabort(ctx->task); /*lint !e534 */
        return rdm_status;
    }

    if ((rdm_status = d_trend(ctx->task)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error committing transaction (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    return S_OKAY;
}

/* ===========================================================================
 *  Extract a file from the database.  The file contents are written to the
 *  file specified in 'fn'.  This function is called with the  appropriate
 *  locks already in place.
 */
static int32_t extract_a_file(
    const char *category,
    const char *fn,
    BLARC_CTX  *ctx)
{
    uint32_t                   sz;
    int32_t                    rdm_status;
    FILE                      *fp;
    char                       buf[BUF_SIZE];
    struct file_info_cat_file  cf;

    if ((fp = fopen(fn, "wb")) == NULL)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to open file %s\n", fn);
        return S_NOFILE;
    }

    memset(&cf, 0, sizeof(cf));
    strcpy(cf.category, category);
    strcpy(cf.base_name, fn);

    if ((rdm_status = d_keyfind(FILE_INFO_CAT_FILE, &cf, ctx->task, CURR_DB)) == S_OKAY)
    {
        rdm_status = d_blobsize(FILE_INFO_FILE_CONTENTS, &sz, ctx->task, CURR_DB);
        if(rdm_status != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error getting BLOB size (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        }
        DisplayMessageNow(FALSE, FALSE, "Extracting %s %s (%d bytes)\n", cf.category, cf.base_name, sz);
        for ( ; ; )
        {
            rdm_status = d_blobread(FILE_INFO_FILE_CONTENTS, buf, BUF_SIZE, &sz, ctx->task, CURR_DB);
            if (rdm_status != S_OKAY)
            {
                /* Will return S_NOTFOUND when we have read all of the blob data */
                if(rdm_status == S_NOTFOUND || sz == 0)
                {
                    break;
                }
                
                DisplayErrorNow(FALSE, FALSE, "Error reading BLOB data (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
                break;
            }
               
            if (fwrite(buf, 1, sz, fp) != sz)
            {
                DisplayErrorNow(FALSE, FALSE, "Failed to write to the output file");
                rdm_status = S_BADWRITE;
                break;
            }
        }
    }
    else
    {
        DisplayMessageNow(FALSE, FALSE, "Cannot find file %s in category %s\n", fn, category);
    }

    fclose(fp);

    return rdm_status;
}

/* ===========================================================================
 *  Extract a file from the database.  This function acquires the appropriate
 *  locks and then calls extract_a_file.
 */
static int32_t extract_file(
    const char *category,
    const char *fn,
    BLARC_CTX  *ctx)
{
    int32_t rdm_status;

    if ((rdm_status = d_lock(1, file_info_r, ctx->task, CURR_DB)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to acquire locks (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    rdm_status = extract_a_file(category, fn, ctx);

    d_freeall(ctx->task); /*lint !e534 */

    return rdm_status;
}

/* ===========================================================================
 *  Extract all files from the database for a particular category.  This
 *  function acquires the appropriate locks and does all the work.
 */
static int32_t extract_cat(
    const char *category,
    BLARC_CTX  *ctx)
{
    int32_t                   rdm_status;
    struct file_info_cat_file cf;

    if ((rdm_status = d_lock(1, file_info_r, ctx->task, CURR_DB)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to acquire locks (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    DisplayMessageNow(FALSE, FALSE, "Extracting category: %s\n", category);

    memset(&cf, 0, sizeof(cf));
    strcpy(cf.category, category);

    /* This d_keyfind is expected to fail with S_NOTFOUND as we have only
     * specified part of the key, but a subsequent d_keynext will return the
     * first (if any) key for this category */
    rdm_status = d_keyfind(FILE_INFO_CAT_FILE, &cf, ctx->task, CURR_DB);
    if (rdm_status != S_OKAY && rdm_status != S_NOTFOUND)
    {
        DisplayErrorNow(FALSE, FALSE, "Error looking up category (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        d_freeall(ctx->task); /*lint !e534 */
        return rdm_status;
    }

    /* Loop through all the keys as long as the category continues to match */
    while ((rdm_status = d_keynext(FILE_INFO_CAT_FILE, ctx->task, CURR_DB)) == S_OKAY) /*lint !e838 */
    {
        if ((rdm_status = d_keyread(&cf, ctx->task)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error reading key data (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            break;
        }

        if (strcmp(cf.category, category) != 0)
        {
            break;
        }

        if ((rdm_status = extract_a_file(category, cf.base_name, ctx)) != S_OKAY) /*lint !e838 */
        {
            break;
        }
    }

    d_freeall(ctx->task); /*lint !e534 */

    /* We expect to get an S_NOTFOUND status if we have reached the end of the
     * keys.  Anything other than S_NOTFOUND or S_OKAY indicates an error */
    if (rdm_status != S_NOTFOUND && rdm_status != S_OKAY)
    {
        return rdm_status;
    }

    return S_OKAY;
}

/* ===========================================================================
 *  List all the files in the databases that match the category specified.
 *  This function is called with locks already acquired.
 */
static int32_t list_files_in_category(
    const char *category,
    BLARC_CTX  *ctx)
{
    int32_t                   rdm_status;
    struct file_info_cat_file cf;
    int32_t                   have_files = 0;
    uint32_t                  blob_size;

    DisplayMessageNow(FALSE, FALSE, "Category: %s\n", category);

    memset(&cf, 0, sizeof(cf));
    strcpy(cf.category, category);

    /* This d_keyfind is expected to fail with S_NOTFOUND as we have only
     * specified part of the key, but a subsequent d_keynext will return the
     * first (if any) key for this category */
    rdm_status = d_keyfind(FILE_INFO_CAT_FILE, &cf, ctx->task, CURR_DB);
    if (rdm_status != S_OKAY && rdm_status != S_NOTFOUND)
    {
        DisplayErrorNow(FALSE, FALSE, "Error looking up category (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    /* Loop through the keys until reaching the end of the keys or the
     * category changes */
    while ((rdm_status = d_keynext(FILE_INFO_CAT_FILE, ctx->task, CURR_DB)) == S_OKAY) /*lint !e838 */
    {
        if ((rdm_status = d_keyread(&cf, ctx->task)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error reading key data (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            break;
        }

        if (strcmp(cf.category, category) != 0)
        {
            break;
        }

        /* Get the size of the BLOB data */
        rdm_status = d_blobsize(FILE_INFO_FILE_CONTENTS, &blob_size, ctx->task, CURR_DB);
        if(rdm_status != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error getting BLOB size (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        }

        DisplayMessage(FALSE, FALSE, "    %s (%d bytes)\n", cf.base_name, blob_size);
        have_files = 1;
    }
    if(have_files)
    {
        DisplayMessageNow(FALSE, FALSE, "\n");
    }
    else
    {
        DisplayMessageNow(FALSE, FALSE, "    There are no files stored under category %s\n", category);
    }

    /* We expect to get an S_NOTFOUND status if we have reached the end of the
     * keys.  Anything other than S_NOTFOUND or S_OKAY indicates an error */
    if (rdm_status != S_NOTFOUND && rdm_status != S_OKAY)
    {
        return rdm_status;
    }

    return S_OKAY;
}

/* ===========================================================================
 *  List all the files in the databases along with which category they are in.
 *  This function is called with locks already acquired.
 */
static int32_t list_all_files(
    BLARC_CTX *ctx)
{
    int32_t          rdm_status;
    char             last_cat[SIZEOF_FILE_INFO_CATEGORY] = "";
    struct file_info fi;
    int32_t          have_files = 0;
    uint32_t         blob_size;

    /* Loop through all the keys.  Whenever we find a new category print it.
     * Print each file we find */
    for (rdm_status = d_keyfrst(FILE_INFO_CAT_FILE, ctx->task, CURR_DB);
            rdm_status == S_OKAY;
            rdm_status = d_keynext(FILE_INFO_CAT_FILE, ctx->task, CURR_DB))
    {        
        if ((rdm_status = d_recread(&fi, ctx->task, CURR_DB)) != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error reading file data (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            return rdm_status;
        }

        if (strcmp(fi.category, last_cat) != 0)
        {
            DisplayMessage(FALSE, FALSE, "Category: %s\n", fi.category);
            strcpy(last_cat, fi.category);
        }

        rdm_status = d_blobsize(FILE_INFO_FILE_CONTENTS, &blob_size, ctx->task, CURR_DB);
        if(rdm_status != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error getting BLOB size (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        }

        DisplayMessage(FALSE, FALSE, "    %s (%d bytes)\n", fi.base_name, blob_size);
        have_files = 1;
    }
    if(have_files)
    {
        DisplayMessageNow(FALSE, FALSE, "\n");
    }
    else
    {
        DisplayMessageNow(FALSE, FALSE, "There are no files currently stored in the BLARC database\n");
    }

    return S_OKAY; /*lint !e850 */
}

/* ===========================================================================
 *  List files.  This function acquires the appropriate locks and then calls
 *  either list_files_in_category or list_all_files.
 */
static int32_t list_files(
    const char *category,
    BLARC_CTX  *ctx)
{
    int32_t rdm_status;

    if ((rdm_status = d_lock(1, file_info_r, ctx->task, CURR_DB)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to acquire locks (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    if (category)
    {
        rdm_status = list_files_in_category(category, ctx);
    }
    else
    {
        rdm_status = list_all_files(ctx);
    }

    d_freeall(ctx->task);

    return rdm_status;
}

/* ===========================================================================
 * Parse through the options.
 * trying to access a record, we initialize the database.  The initialize
 * function requires exclusive locks, so apply them up front.
 */
static int32_t get_opts(
    int32_t             argc,
    const char *const  *argv,
    COMMANDS           *cmd,
    const char        **category,
    const char *const **filelist,
    uint32_t           *numfiles,
    BLARC_CTX          *ctx)
{
    const char        *opt;
    int32_t            status;
    int32_t            finalopt;
    int                _argc = 0;
    const char *const *_argv = NULL;
    const char        *optarg;
    /*lint -save -e785 -e956 */
    static cmdline_opt opts[] = {
        { ARG_OPT,         ":docroot",                  "s=path",     "PATH location of TFS document root (absolute or relative to CWD)" },
        { ARG_OPT,         ":v",                        NULL,         "Verbose output" },
        { ARG_OPT,         ":c",                        "s=category", "Category under which to file this BLOB" },
        { ARG_OPT,         ":l",                        NULL,         "list files" },
        { ARG_OPT,         ":x",                        NULL,         "extract files"},
        { ARG_OPT,         ":d",                        NULL,         "delete files from archive"},
        { ARG_FINAL_MULTI, ":filename1 [filename2...]", NULL,         "file name list"},
        { ARG_END,         NULL,                        NULL,         NULL }
    };
    /*lint -restore */


    if ((status = b_cmd_expand(argc, argv, &_argc, &_argv)) != B_CMD_OKAY)
    {
        return status;
    }

    status = b_cmd_initopt(RDM_EXAMPLE_DESC("BLOB Archive"), _argc, _argv, opts, NULL, &ctx->cmd_status);
    if (status != B_CMD_OKAY)
    {
        return status;
    }

    *cmd = CMD_ADD;
    *category = NULL;
    *filelist = NULL;
    *numfiles = 0;

    /* check for batch mode operations */
    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'c': 
                *category = optarg;
                break;

            case 'd':
                if(stricmp(opt, "docroot") == 0)
                {
                    ctx->docroot = psp_strdup(optarg, TAG0);
                }
                else
                {
                    *cmd = CMD_DELETE;
                }
                break;

            case 'l':
                *cmd = CMD_LIST;
                break;

            case 'v':
                ctx->tparams.verbose = 1;
                break;

            case 'x':
                *cmd = CMD_EXTRACT;
                break;
        } /*lint !e744 */
    }

    if ((finalopt = b_cmd_finaloptpos(opts)) != 0)
    {
        *filelist = &_argv[finalopt];
        *numfiles = (uint32_t) (_argc - finalopt);
    }

    if (*cmd != CMD_LIST && !*category)
    {
        *category = "NONE";
    }

    return B_CMD_OKAY;
}

/* ===========================================================================
 * Make sure that the data/key files exist.  If we get an S_NOFILE error
 * trying to access a record, we initialize the database.  The initialize
 * function requires exclusive locks, so apply them up front.
 */
static int32_t make_sure_db_exists(
    BLARC_CTX *ctx)
{
    int32_t rdm_status;

    if ((rdm_status = d_trbegin("blarc", ctx->task)) != S_OKAY)
    {
        return rdm_status;
    }

    if ((rdm_status = d_reclock(FILE_INFO, "w", ctx->task, CURR_DB)) != S_OKAY)
    {
        if (rdm_status == S_UNAVAIL)
        {
            return S_OKAY;
        }
    }

    if ((rdm_status = d_recfrst(FILE_INFO, ctx->task, CURR_DB)) != S_OKAY)
    {
        rdm_status = d_initialize(ctx->task, CURR_DB);
    }

    d_trend(ctx->task);

    return rdm_status;
}

/* ===========================================================================
 * Initialize the application
 */
static int32_t blarc_initialize(
    BLARC_CTX *ctx)
{
    int32_t rdm_status;

    /* Initialize the TFS */
    rdm_status = d_tfsinitEx(ctx->docroot, &ctx->tparams, &ctx->hTFS, ctx->cmd_status.tfs_type);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error initializing TFS (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    if ((rdm_status = d_opentask(&ctx->task)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error opening a RDM task (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    rdm_status = d_open_ptr("blarc", "s", &blarc_dbd, sizeof(blarc_dbd), ctx->task);
    if (rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error opening the blarc database (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    if ((rdm_status = make_sure_db_exists(ctx)) != S_OKAY)
    {
        d_close(ctx->task);
        d_closetask(ctx->task);
    }

    return rdm_status;
}

/* ===========================================================================
 * Initialize the application
 */
static void blarc_cleanup(
    BLARC_CTX *ctx)
{
    if(ctx->task)
    {
        d_close(ctx->task); /*lint !e534 */
        d_closetask(ctx->task); /*lint !e534 */
        ctx->task = NULL;
    }

    d_tfsterm(ctx->hTFS); /*lint !e534 */
        
    if(ctx->cmd_status.stdout_opened)
    {
        psp_stdout_close();
    }
        
    if(ctx->cmd_status.stderr_opened)
    {
        psp_stderr_close();
    }

    return;
}


/* ===========================================================================
 * Process the user-specified command
 */
static int32_t process_commands(
    COMMANDS           cmd,
    const char        *category,
    const char *const *filelist,
    uint32_t           numfiles,
    BLARC_CTX         *ctx)
{
    int32_t  status = S_OKAY;
    uint32_t ii;
    FILE    *fp;

    switch (cmd)
    {
        case CMD_LIST:
            status = list_files(category, ctx);
            break;

        case CMD_DELETE:
            if (numfiles)
            {
                for (ii = 0; ii < numfiles && status == S_OKAY; ii++)
                {
                    status = delete_file(category, filelist[ii], ctx);
                }
            }
            else
            {
                status = delete_file(category, NULL, ctx);
            }

            break;

        case CMD_EXTRACT:
            if (numfiles)
            {
                for (ii = 0; ii < numfiles && status == S_OKAY; ii++)
                {
                    status = extract_file(category, filelist[ii], ctx);
                }
            }
            else
            {
                status = extract_cat(category, ctx);
            }

            break;

        case CMD_ADD:
            for (ii = 0; ii < numfiles; ii++)
            {
                if ((fp = fopen(filelist[ii], "rb")) == NULL)
                {
                    DisplayErrorNow(FALSE, FALSE, "Unable to open %s\n", filelist[ii]);
                    continue;
                }

                status = store_file(category, filelist[ii], fp, ctx);

                fclose(fp);

                if (status != S_OKAY)
                {
                    DisplayErrorNow(FALSE, FALSE, "Failed to store file %s", filelist[ii]);
                }
            }
            break;
    }

    return status;
}

int32_t EXTERNAL_FCN blarcExample_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t            status;
    uint32_t           numfiles;
    COMMANDS           cmd;
    BLARC_CTX          ctx;
    const char        *category = NULL;
    const char *const *filelist;

    memset(&ctx, 0, sizeof(ctx));
    if (get_opts(argc, argv, &cmd, &category, &filelist, &numfiles, &ctx) != B_CMD_OKAY)
    {
        blarc_cleanup(&ctx);
        return 1;
    }

    if (blarc_initialize(&ctx) != S_OKAY)
    {
        blarc_cleanup(&ctx);
        return 1;
    }

    status = process_commands(cmd, category, filelist, numfiles, &ctx);

    blarc_cleanup(&ctx);

    return (status == S_OKAY ? 0 : 1);
}

PSP_STARTUP_EXAMPLE(blarcExample)
