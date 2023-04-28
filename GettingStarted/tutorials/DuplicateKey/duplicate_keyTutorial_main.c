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
**    DUPLICATE KEY
**    ----------
**    This document describes the process to create a simple database containing a
**    key, insert records, and read data through the key.
**
**    For a full description of RDM keys, see Database Design in the Users Guide.
**
**    This example contains a single record type with a single field which is designated
**    as a key.  An insertion with a key field value that already exists in the database
**    will not be rejected as an error from the d_fillnew function. Multiple records with
**    the same key value can be stored in the database when the key is not designated as 
**    unique.
*/

#include <stdio.h>
#include "rdm.h"
#include "duplicate_key.h"

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "duplicate_key_dbd.h"
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

static int32_t EXTERNAL_FCN duplicate_keyTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int i;
    int iStatus;
    struct info sInfo;      /* Defined in unique_key.h, memory placholder for records */
    DB_TASK *task;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    /* create a new task to contain context */
    d_opentask(&task);

    /* Open the database */
#if defined(NEED_SETUP_DATABASE) /* Embed database setup into program */
    iStatus = d_open_ptr("duplicate_key", "o", &duplicate_key_dbd, sizeof(duplicate_key_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("duplicate_key","o",task);
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
        fprintf(stderr,"Sorry, I can't open the duplicate_key database.");
    }

    return 0;
}

PSP_STARTUP_EXAMPLE_CR(duplicate_keyTutorial)

