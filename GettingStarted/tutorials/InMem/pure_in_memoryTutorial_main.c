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
**    IN-MEMORY
**    ---------
**    This document describes the process to create a simple pure in-memory database, 
**    insert a record, and read data from the database. This application requires no 
**    persistent storage to run and can act as a simple example for disk less 
**    applications.
*/

#include <stdio.h>
#include "rdm.h"                /* The RDM API.                   */
#include "pure_in_memory.h"     /* The pure_in_memory database definitions.*/
#include "pure_in_memory_dbd.h" /* The pure_in_memory database dictionary. */

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static int32_t EXTERNAL_FCN pure_in_memoryTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t     iStatus;
    struct info sInfo; /* Defined in hello_world.h, memory placeholder for records*/
    DB_TASK    *task;

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

    /* Open the database.  */
    iStatus = d_open_ptr("pure_in_memory","o",&pure_in_memory_dbd, 
                       sizeof(pure_in_memory_dbd),task);
    if(iStatus == S_OKAY) {

        /* Initialize the pure_in_memory database. */
        iStatus = d_initialize(task,CURR_DB);
        if(iStatus == S_OKAY) {

            /* Create a in-memory record */
            strcpy(sInfo.myChar,"I'm running in pure memory!");
            iStatus = d_fillnew(INFO,&sInfo,task,CURR_DB);
            
            if(iStatus == S_OKAY) {
                
                /* Move to the first record in the info table */
                iStatus = d_recfrst(INFO,task,CURR_DB);
                if(iStatus == S_OKAY) {
                    
                    /* Read the full content of the current record */
                    iStatus = d_recread(&sInfo,task,CURR_DB);
                    if(iStatus == S_OKAY) {
                        printf("%s\n\n",sInfo.myChar);
                    } else {
                        fprintf(stderr,"Sorry, I can't read the first info record.");
                    }
                } else {
                    fprintf(stderr,"Sorry, I can't navigate to the first info record.");
                }
            } else {
                fprintf(stderr,"Sorry, I can't create a new info record.");
            }
        } else {
            fprintf(stderr,"Sorry, I can't initialize the in-memory database.");
        }
        
        /* Close the database */
        d_close(task);
        d_closetask(task);
    } else {
        fprintf(stderr,"Sorry, I can't open the pure_in_memory database.");
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

PSP_STARTUP_EXAMPLE_CR(pure_in_memoryTutorial)

