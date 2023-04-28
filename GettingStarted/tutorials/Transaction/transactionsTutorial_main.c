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
**    TRANSACTIONS
**    ----------
**    This document describes the process to create a simple database that stores data using
**    transactions. We'll insert a hundred records in the table, and verify by counting the 
**    number of records stored in the database.
**
*/

#include <stdio.h>
#include "rdm.h"                /* The RDM API.                   */
#include "transactions.h"       /* The transaction database definitions.   */

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "transactions_dbd.h"
#endif

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static int32_t EXTERNAL_FCN transactionsTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    struct   info sInfo;
    int      iStatus; /*lint -esym(850,iStatus) */
    int      iTrans;
    int      iCnt;
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
    iStatus = d_open_ptr("transactions", "o", &transactions_dbd, sizeof(transactions_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("transactions", "x", task);
#endif

    if (iStatus == S_OKAY) {

        /* Loop to group 10 records into 10 different transactions. */
        for(iTrans = 0; iTrans < 10 && iStatus == S_OKAY; iTrans++) {

            /* Start a transaction. */
            iStatus = d_trbegin("tr",task);
            if (iStatus == S_OKAY) {
                for(iCnt = 0; iCnt < 10 && iStatus == S_OKAY; iCnt++) {

                    /* Create a new record */
                    sprintf(sInfo.myStr,"Inserted %d Transaction %d",iCnt,iTrans);
                    iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);
                    if (iStatus != S_OKAY) {
                        fprintf(stderr,"Sorry, I can't create a new info record.");
                    }
                }

                if (iStatus == S_OKAY) {
                    /* Commit the transaction */
                    iStatus = d_trend(task);
                }
                else {
                    /* Something is wrong, abort the transaction */
                    d_trabort(task);
                }
            }
        }

        iCnt = 0;

        /* Loop through all the records in the INFO table, and count them */
        for (iStatus = d_recfrst(INFO,task,CURR_DB);
             iStatus == S_OKAY;
             iStatus = d_recnext(task,CURR_DB)) {

            /* Read and print the INFO record. */
            iStatus = d_recread(&sInfo,task,CURR_DB);
            if (iStatus == S_OKAY) {
                printf("%s\n",sInfo.myStr);
            }

            iCnt++;
        }
        printf("\n");
        printf("I found %d INFO records in the database\n",iCnt);

        /* Close the database */
        d_close(task);
        d_closetask(task);
    }
    else
        fprintf(stderr,"Sorry, I can't open the transaction database.");

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

PSP_STARTUP_EXAMPLE_CR(transactionsTutorial)
