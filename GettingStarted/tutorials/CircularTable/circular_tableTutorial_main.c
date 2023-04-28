/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2012 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

/*
**    CIRCULAR TABLE
**    ------------
**    This document describes the process to create a circular record in a database, 
**    insert a circular record, and read data from this database.
*/

#include <stdio.h>
#include "rdm.h"            /* The RDM API.                 */
#include "circular_table.h" /* The one_to_many database definitions. */

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "circular_table_dbd.h"
#endif

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static const struct album albums[] = {
    { "Eric Dolphy" },
    { "Miles Davis" },
    { "Charles Mingus" },
    { "Chico Hamilton" },
    { "John Coltrane" }
};

static int32_t EXTERNAL_FCN circular_tableTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t      iStatus;
    uint32_t     ii;
    struct album sAlbum;
    DB_TASK     *task;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    /* Open a task to store context */
    if ((iStatus = d_opentask(&task)) != S_OKAY)
    {
        printf("Failed to open a new task, error = %d\n", iStatus);
        return (1);
    }

    /* Set a database error handler */
    if ((iStatus = d_set_dberr_ex(tutorial_dberr, NULL, task)) != S_OKAY)
    {
        printf("Failed set dberror handler, error = %d\n", iStatus);
        d_closetask(task);
        return (1);
    }

    /* Open the database. */
#if defined(NEED_SETUP_DATABASE) /* Embed database setup into program */
    iStatus = d_open_ptr("circular_table", "o", &circular_table_dbd, sizeof(circular_table_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("circular_table","o",task);
#endif

    if (iStatus == S_OKAY) {

        /* Create the data records */
        for (ii = 0;
             ii < sizeof(albums)/sizeof(struct album) && iStatus == S_OKAY;
             ii++)
        {
            iStatus = d_fillnew(ALBUM,&albums[ii],task,CURR_DB);

            /* Read and dump all album records. */
            if (iStatus == S_OKAY) {
                printf("\nDUMPING ALL RECORDS\n");
                for (iStatus = d_recfrst(ALBUM,task,CURR_DB);
                        iStatus == S_OKAY; iStatus = d_recnext(task,CURR_DB))
                {
                    iStatus = d_recread(&sAlbum,task,CURR_DB);
                    if (iStatus != S_OKAY)
                        break;

                    printf("   %s\n", sAlbum.artist);
                }

                /* Reset the status, the d_recnext will end with a S_NOTFOUND
                 * when there are no more records. */
                if (iStatus == S_NOTFOUND) /*lint !e850 */
                    iStatus = S_OKAY;
            }
        }

        /* Close the database */
        d_close(task);
        d_closetask(task);
    }
    else 
        fprintf(stderr,"Sorry, I can't open the circular_table database.");

    return 0;
}

static void EXTERNAL_FCN tutorial_dberr(
    int32_t     errnum, 
    const char *errmsg,
    void       *data,
    DB_TASK    *task)
{ /*lint !e403 */
    UNREF_PARM(data)
    UNREF_PARM(task)

    DisplayErrorNow(FALSE, FALSE, "\n%s (errnum = %d)\n", errmsg, errnum);
} /*lint !e818 */
PSP_STARTUP_EXAMPLE_CR(circular_tableTutorial)
