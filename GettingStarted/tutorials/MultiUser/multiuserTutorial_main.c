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
**    MULTI-USER
**    ----------
**    This document describes the process to create a simple database that uses multiple 
**    threads to access and update the database. Note that the database is opened in shared 
**    mode because it requires locking from each task that will be using it.
*/

#include <stdio.h>
#include "rdm.h"                /* The RDM API.                   */
#include "psp.h"                /* The RDM Platform Support API.           */
#include "multiuser.h"          /* The multiuser database definitions.     */

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "multiuser_dbd.h"
#endif

/* adjust the size of a transaction with this value */
#define TXSIZE 80

/* The global information shared among all threads */
static PSP_SEM psLock;     /*lint !e956 */
static int iThreads;       /*lint !e956 */

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static void exitThread(DB_TASK *task)
{
    /* Decrease the global thread count. */
    psp_syncEnterExcl(psLock);
    iThreads--;                 /* Exclusive access to this variable while updating */
    psp_syncExitExcl(psLock);

    d_close(task);
    d_closetask(task);
    psp_threadEnd();
}

static void beginTrans(const char *szTID, DB_TASK *task)
{
    int iStatus;

    /* Start a transaction */
    iStatus = d_trbegin(szTID,task);
    if (iStatus != S_OKAY)
        exitThread(task);

    /* Make a write lock request, this will block if another   */
    /* thread has the lock. Note that d_trend frees all locks. */
    iStatus = d_reclock(INFO,"w",task,CURR_DB);
    if (iStatus != S_OKAY)
        exitThread(task);
}

static void thread(void *pData)
{
    DB_TASK    *task;
    struct info sInfo;  /* Defined in multiuser.h */
    char        szTID[32];
    int         iStatus;
    int         iCnt;

    sprintf(szTID, "0x%p", &pData);

    /* Open the task to store context */
    iStatus = d_opentask(&task);
    if (iStatus != S_OKAY)
        exitThread(task);

    /* Set an error display handler */
    iStatus = d_set_dberr_ex(tutorial_dberr, NULL, task);
    if (iStatus != S_OKAY)
        exitThread(task);

    /* Open the database in shared mode. */
    iStatus = d_open("multiuser","s",task);
    if (iStatus != S_OKAY)
        exitThread(task);

    beginTrans(szTID, task);
    for (iCnt = 0; iCnt < 1000 && iStatus == S_OKAY; iCnt++) {

        /* Create a info record */
        sprintf(sInfo.myStr,"Inserted %d by Thread %s",iCnt,szTID);
        iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);
        if (iStatus  != S_OKAY) {
            fprintf(stderr,"Sorry, I can't create a new info record.");
            exitThread(task);
        }

        /* allow other tasks to proceed */
        if (iCnt && iCnt%TXSIZE == 0) {
            d_trend(task);
            beginTrans(szTID, task);
        }
    }
    /* Commit the transaction. */
    d_trend(task);
    exitThread(task);
}

static int32_t EXTERNAL_FCN multiuserTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    DB_TASK   *task;
    int32_t    ii;
    int32_t    iCnt = 0;
    int32_t    iStatus;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

#if defined(NEED_SETUP_DATABASE) /* Embed database setup into program */
    /* Open a task to store context */
    if ((iStatus = d_opentask(&task)) != S_OKAY)
    {
        DisplayErrorNow(0, 0, "Failed to open a new task, error = %d\n", iStatus);
        return (iStatus);
    }

    /* Set a database error handler */
    if ((iStatus = d_set_dberr_ex(tutorial_dberr, NULL, task)) != S_OKAY)
    {
        DisplayErrorNow(0, 0, "Failed set dberror handler, error = %d\n", iStatus);
        d_closetask(task);
        return (iStatus);
    }

    iStatus = d_open_ptr("multiuser", "x", &multiuser_dbd, sizeof(multiuser_dbd), task);
    if (iStatus == S_OKAY) 
    {
        d_initialize(task, CURR_DB);
        d_close(task);
        d_closetask(task);
    }
    else 
    {
        d_closetask(task);
        DisplayErrorNow(0, 0, "Sorry! Could not create multiuser database. (ErrCode = %d)\n", iStatus);
        return 1;
    }
#endif

    /* Setup the thread data structure. */
    if ( (psLock = psp_syncCreate(PSP_MUTEX_SEM)) == NO_PSP_SEM)
    {
        fprintf(stderr, "Failed to create synchronization mutex\n");
        return 1;
    }

    iThreads = 4;
    /* Startup 4 insert threads. */
    for (ii = 0; ii < iThreads; ii++)
    {
        if (psp_threadBegin(thread, 0, NULL) == PSP_FAILED)
        {
            fprintf(stderr, "Failed to start thread %d\n", ii);
            psp_syncEnterExcl(psLock);
            iThreads--;     /* reduce thread cnt of launched threads */
            psp_syncExitExcl(psLock);
        }
    }

    /* Need to wait until all threads have completed. */
    while (iThreads > 0)
        psp_sleep(1000);

    /* Open the task to store context */
    iStatus = d_opentask(&task);
    if (iStatus == S_OKAY)
    {
        /* use an error handler to display db error message */
        iStatus = d_set_dberr_ex(tutorial_dberr, NULL, task);
        if (iStatus == S_OKAY)
        {
            /* Open the database in shared mode. */
            iStatus = d_open("multiuser","s",task);
            if (iStatus == S_OKAY)
            {
                /* Make a read lock. */
                iStatus = d_reclock(INFO,"r",task,CURR_DB);
                if (iStatus == S_OKAY)
                {
                    /* Count all the INFO records */
                    for (iStatus = d_recfrst(INFO,task,CURR_DB); iStatus == S_OKAY;
                    iStatus = d_recnext(task,CURR_DB))
                        iCnt++;

                    /* Free the read lock */
                    d_recfree(INFO,task,CURR_DB);
                    printf("There are %d info records in the multiuser database.\n",iCnt);
                }
                d_close(task);
            }
            else
            {
                DisplayErrorNow(0, 0, "Error setting error handler\n");
            }
        }
        else
        {
            DisplayErrorNow(0, 0, "Error opening database\n");
        }
        d_closetask(task);
    }

    psp_syncDelete(psLock);         /* cleanup mutex semaphore */

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

PSP_STARTUP_EXAMPLE_CR(multiuserTutorial)

