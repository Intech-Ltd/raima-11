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
**    UNIQUE KEY
**    ----------
**    This document describes the process to create a simple database containing a
**    unique key, insert records, and read data through the key.
**
**    For a full description of RDM keys, see Database Design in the Users Guide.
**
**    This example contains a single record type with a single field which is designated
**    as a unique key. This means that every record of type info that is successfully
**    inserted into the database will have a unique value. If an insertion is attempted
**    with a key field value that already exists in the database, the insertion is rejected
**    and an error code is returned from the d_fillnew function.
*/

#include <stdio.h>
#include "rdm.h"
#include "unique_key.h"

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "unique_key_dbd.h"
#endif

static const char *const szData[] = {
    "John",
    "James",
    "Duncan",
    "Bill",
    "John",     /* Inserting this should fail, referential integrity rules violated. */
    "Paul",
    "David",
    NULL
};

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static int32_t EXTERNAL_FCN unique_keyTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int i;
    int iStatus;
    struct info sInfo;      /* Defined in unique_key.h, memory placholder for records */
    DB_TASK *task;

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

    /* Open the database */
#if defined(NEED_SETUP_DATABASE) /* Embed database setup into program */
    iStatus = d_open_ptr("unique_key", "o", &unique_key_dbd, sizeof(unique_key_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("unique_key","o",task);
#endif

    if (iStatus == S_OKAY) {

        /* Create the data records */
        for(i = 0; szData[i] && iStatus == S_OKAY; i++) {
            strcpy(sInfo.myChar,szData[i]);
            iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);

            if (iStatus == S_DUPLICATE) {
                printf("%s is a duplicate record.\n",sInfo.myChar);
                iStatus = S_OKAY;   /* This was expected, re-set error code. */
            }
        }

        if (iStatus == S_OKAY) {
            /* Do a key match lookup. */
            iStatus = d_keyfind(INFO_MYCHAR,szData[3],task,CURR_DB);
            if (iStatus == S_OKAY) {

                /* Read the full content of the current record */
                iStatus = d_recread(&sInfo,task,CURR_DB);
                if (iStatus == S_OKAY) {
                    printf("Record found doing a key lookup: %s\n",sInfo.myChar);
                } else {
                    fprintf(stderr,"Sorry, I can't read the first info record.");
                }

                /* Do a closest key match lookup, and return the rest in key order. */
                iStatus = d_keyfind(INFO_MYCHAR,"Jim",task,CURR_DB);
                if (iStatus == S_NOTFOUND) {
                    iStatus = d_keynext(INFO_MYCHAR,task,CURR_DB);
                }
                while (iStatus == S_OKAY) {
                    /* Read the full content of the current record */
                    iStatus = d_recread(&sInfo,task,CURR_DB);
                    if (iStatus == S_OKAY) {
                        printf("Record found doing a key scan lookup: %s\n", sInfo.myChar);
                    } else {
                        fprintf(stderr,"Sorry, I can't read the first info record.");
                    }

                    /* Go to the next record in key order. */
                    iStatus = d_keynext(INFO_MYCHAR,task,CURR_DB);
                }
            }
        }

        /* Close the database */
        d_close(task);
        d_closetask(task);
    } else {
        fprintf(stderr,"Sorry, I can't open the unique_key database.");
    }

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

PSP_STARTUP_EXAMPLE_CR(unique_keyTutorial)

