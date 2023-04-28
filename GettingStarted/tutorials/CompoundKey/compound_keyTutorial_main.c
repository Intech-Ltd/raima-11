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
**    COMPOUND KEY
**    ------------
**    This document describes the process to create a simple database containing a key,
**    insert records, and read data through the key.
**
**    For a full description of RDM keys, see Database Design in the Users Guide.
**
**    This example contains a single record type with two fields which are referenced in 
**    a non-unique compound key.
*/

#include <stdio.h>
#include "rdm.h"
#include "compound_key.h"

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "compound_key_dbd.h"
#endif

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static const struct info infos[] = {
  { "John", 30 },
  { "James", 33 },
  { "Duncan", 37 },
  { "Bill", 44 },
  { "John", 27 },
  { "Paul", 21 },
  { "David", 27 },
  { "David", 27 }
};

static int32_t EXTERNAL_FCN compound_keyTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t     iStatus;
    uint32_t    i;
    struct info sInfo;
    DB_TASK    *task;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    /* create the task variable to store context */
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
    iStatus = d_open_ptr("compound_key", "o", &compound_key_dbd, sizeof(compound_key_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("compound_key","o",task);
#endif

    if (iStatus == S_OKAY) {

        /* Create the data records */
        for(i = 0; i < sizeof(infos)/sizeof(struct info) && iStatus == S_OKAY; i++)
            iStatus = d_fillnew(INFO,&infos[i],task,CURR_DB);

        if (iStatus == S_OKAY) {
            /* Do a key match lookup on all 'John' records. */
            memset(&sInfo,0,sizeof(sInfo));
            strcpy(sInfo.myChar,"John");
            iStatus = d_keyfind(INFO_MYKEY,&sInfo,task,CURR_DB);
            if (iStatus == S_NOTFOUND)
                iStatus =  d_keynext(INFO_MYKEY,task,CURR_DB);

            while (iStatus == S_OKAY) {
                /* Read the full content of the current record */
                iStatus = d_recread(&sInfo,task,CURR_DB);
                if (iStatus == S_OKAY && strcmp(sInfo.myChar,"John") == 0) {
                    printf("Record found doing a key scan lookup: %s, %d\n",
                    sInfo.myChar,sInfo.myAge);
                } 
                else
                    break;

                /* Go to the next record in key order. */
                iStatus = d_keynext(INFO_MYKEY,task,CURR_DB);
            }
        }

        /* Close the database */
        d_close(task);
        d_closetask(task);
    }
    else
        fprintf(stderr,"Sorry, I can't open the compound_key database.");

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

PSP_STARTUP_EXAMPLE_CR(compound_keyTutorial)

