/*
**    HELLO WORLD
**    -----------
**    This document describes the process to create a simple database, insert a record 
**    containing a text field, read the text field from database and print it out.
*/

#include <stdio.h>
#include "rdm.h"          /* The RDM API.                 */
#include "hello_world.h"  /* The hello_world database definitions. */

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "hello_world_dbd.h"
#endif

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static int32_t EXTERNAL_FCN hello_worldTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int      iStatus;
    DB_TASK *task;
    TFS_HANDLE hTFS;
    struct   info sInfo;  /* Defined in hello_world.h, memory placholder for records */

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    /* Open a task for all context info */
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
    iStatus = d_open_ptr("hello_world", "o", &hello_world_dbd, sizeof(hello_world_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("hello_world","o",task);
#endif
    if (iStatus == S_OKAY) {
        /* Create a Hello World record */
        strcpy(sInfo.myChar,"Hello World! - using TFST");
        iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);
        if (iStatus == S_OKAY) {
            /* Move to the first record in the info table */
            iStatus = d_recfrst(INFO,task,CURR_DB);
            if (iStatus == S_OKAY) {
                /* Read the full content of the current record */
                iStatus = d_recread(&sInfo,task,CURR_DB);
                if (iStatus == S_OKAY)
                    printf("%s\n",sInfo.myChar);
                else
                    fprintf(stderr,"Sorry, I can't read the first info record.\n");
            }
            else
                fprintf(stderr,"Sorry, I can't navigate to the first info record.\n");
        }
        else
            fprintf(stderr,"Sorry, I can't create a new info record.\n");

        /* Close the database */d_close(task);
        d_closetask(task);
    }
    d_tfsinitEx(NULL, NULL, &hTFS, TFS_TYPE_STANDALONE);
    d_opentask(&task);
    
    /* Open the database */
#if defined(NEED_SETUP_DATABASE) /* Embed database setup into program */
    iStatus = d_open_ptr("hello_world", "o", &hello_world_dbd, sizeof(hello_world_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("hello_world","o",task);
#endif
    if (iStatus == S_OKAY) {
        /* Create a Hello World record */
        strcpy(sInfo.myChar,"Hello World! - using TFSS");
        iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);
        if (iStatus == S_OKAY) {
            /* Move to the first record in the info table */
            iStatus = d_recfrst(INFO,task,CURR_DB);
            if (iStatus == S_OKAY) {
                /* Read the full content of the current record */
                iStatus = d_recread(&sInfo,task,CURR_DB);
                if (iStatus == S_OKAY)
                    printf("%s\n",sInfo.myChar);
                else
                    fprintf(stderr,"Sorry, I can't read the first info record.\n");
            }
            else
                fprintf(stderr,"Sorry, I can't navigate to the first info record.\n");
        }
        else
            fprintf(stderr,"Sorry, I can't create a new info record.\n");
        
        /* Close the database */d_close(task);
        d_closetask(task);
        d_tfsterm(hTFS);
    }
    d_tfsinitEx(NULL, NULL, &hTFS, TFS_TYPE_RPC);
    d_opentask(&task);
    
    /* Open the database */
#if defined(NEED_SETUP_DATABASE) /* Embed database setup into program */
    iStatus = d_open_ptr("hello_world", "o", &hello_world_dbd, sizeof(hello_world_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("hello_world","o",task);
#endif
    if (iStatus == S_OKAY) {
        /* Create a Hello World record */
        strcpy(sInfo.myChar,"Hello World! - using TFSR");
        iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);
        if (iStatus == S_OKAY) {
            /* Move to the first record in the info table */
            iStatus = d_recfrst(INFO,task,CURR_DB);
            if (iStatus == S_OKAY) {
                /* Read the full content of the current record */
                iStatus = d_recread(&sInfo,task,CURR_DB);
                if (iStatus == S_OKAY)
                    printf("%s\n",sInfo.myChar);
                else
                    fprintf(stderr,"Sorry, I can't read the first info record.\n");
            }
            else
                fprintf(stderr,"Sorry, I can't navigate to the first info record.\n");
        }
        else
            fprintf(stderr,"Sorry, I can't create a new info record.\n");
        
        /* Close the database */d_close(task);
        d_closetask(task);
        d_tfsterm(hTFS);
    }
    else {
        fprintf(stderr,"\nSorry, I can't open the hello_world database in TFSR mode. Err=%d\n", iStatus);
        if (iStatus == S_TX_CONNECT) {
            fprintf(stderr, "*** Please make sure a separate tfserver is running, then restart this tutorial\n");
        }
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

PSP_STARTUP_EXAMPLE_CR(hello_worldTutorial)

