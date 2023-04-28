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
**    HYBRID IN-MEMORY
**    ----------------
**    This document describes the process to create a simple database that stores data both
**    in in-memory tables and on-disk tables. We'll insert a record in each table, and read 
**    the data back from both.
*/

#include <stdio.h>
#include "rdm.h"             /* The RDM API.                      */
#include "hybrid_in_memory.h"/* The hybrid_in_memory database definitions. */

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "hybrid_in_memory_dbd.h"
#endif

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static int32_t EXTERNAL_FCN hybrid_in_memoryTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int              iStatus;
    struct on_disk   sOnDisk;   /* Defined in hybrid_in_memory.h, memory placeholder for records */
    struct in_memory sInMemory; /* Defined in hybrid_in_memory.h, memory placeholder for records */
    DB_TASK         *task;

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
    iStatus = d_open_ptr("hybrid_in_memory", "o", &hybrid_in_memory_dbd, sizeof(hybrid_in_memory_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("hybrid_in_memory","o",task);
#endif

    if(iStatus == S_OKAY) {
        
        /* Create a On Disk record */
        strcpy(sOnDisk.myChar,"I'm on-disk!");
        iStatus = d_fillnew(ON_DISK,&sOnDisk,task,CURR_DB);
        if(iStatus == S_OKAY) {
            
            /* Move to the first on-disk record in the info table */
            iStatus = d_recfrst(ON_DISK,task,CURR_DB);
            if(iStatus == S_OKAY) {
                
                /* Read the full content of the current record */
                iStatus = d_recread(&sOnDisk,task,CURR_DB);
                if(iStatus == S_OKAY) {
                    printf("%s\n",sOnDisk.myChar);
                    fprintf(stderr,"Sorry, I can't read the first on-disk record.");
                }
            } else {
                fprintf(stderr,"Sorry, I can't navigate to the first on-disk record.");
            }
        } else {
            fprintf(stderr,"Sorry, I can't create a new on-disk record.");
        }
        
        if(iStatus == S_OKAY) {
            
            /* Create a In Memory record */
            strcpy(sInMemory.myChar,"I'm in-memory!");
            iStatus = d_fillnew(IN_MEMORY,&sInMemory,task,CURR_DB);
            if(iStatus == S_OKAY) {
                
                /* Move to the first in-memory record in the info table */
                iStatus = d_recfrst(IN_MEMORY,task,CURR_DB);
                if(iStatus == S_OKAY) {

                    /* Read the full content of the current record */
                    iStatus = d_recread(&sInMemory,task,CURR_DB);
                    if(iStatus == S_OKAY) {
                        printf("%s\n\n",sInMemory.myChar);
                    } else {
                        fprintf(stderr,"Sorry, I can't read the first in-memory record.");
                    }
                } else {
                    fprintf(stderr,"Sorry, I can't navigate to the first in-memory record.");
                }
            } else {
                fprintf(stderr,"Sorry, I can't create a new in-memory record.");
            }
        }
        
        /* Close the database */
        d_close(task);
        d_closetask(task);
    } else {
        fprintf(stderr,"Sorry, I can't open the hybrid_in_memory database.");
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

PSP_STARTUP_EXAMPLE_CR(hybrid_in_memoryTutorial)

