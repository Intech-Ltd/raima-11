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
**    ONE-TO-MANY
**    -----------
**    This document describes the process to create a simple one-to-many relationship 
**    between two records in a RDM database. We'll insert records and read 
**    them back from the database.
*/

#include <stdio.h>
#include "rdm.h"         /* The RDM API.                 */
#include "one_to_many.h" /* The one_to_many database definitions. */

#if defined(NEED_SETUP_DATABASE) /* Alternative method to embed database setup */
#include "one_to_many_dbd.h"
#endif

static void EXTERNAL_FCN tutorial_dberr(int32_t, const char *, void *, DB_TASK *);

static const struct one ones[]     = { { "John" }, { "James" }, { "Duncan" } };

static const struct many johns[]   = { { "John's First" }, { "John's Second" }, { "John's Third" } };
static const struct many james[]   = { { "James's First" }, { "James's Second" } };
static const struct many duncans[] = { { "Duncan's First" }, { "Duncan's Second" } };

static int32_t EXTERNAL_FCN one_to_manyTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int                iStatus; /*lint -esym(850,iStatus) */
    struct one         sOne;
    struct many        sMany;
    const struct many *psMany = NULL;
    uint32_t           iO;
    int                iM;
    int                iN = 0;
    DB_ADDR            dba;
    DB_TASK           *task;

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
    iStatus = d_open_ptr("one_to_many", "o", &one_to_many_dbd, sizeof(one_to_many_dbd), task);
    if (iStatus == S_OKAY) {
        d_initialize(task, CURR_DB);
    }
#else /* Or, database setup done externally using ddlp and initdb utilities */
    iStatus = d_open("one_to_many","o",task);
#endif

    if (iStatus == S_OKAY) {
        /* Create the data records */
        for (iO = 0; iO < sizeof(ones)/sizeof(struct one) && iStatus == S_OKAY; iO++) {
            iStatus = d_fillnew(ONE,&ones[iO],task,CURR_DB);
            if (iStatus == S_OKAY) {

                /* Set current record to be the owner of the relationship. */
                iStatus = d_setor(RELATIONSHIP,task,CURR_DB);
                if (iStatus == S_OKAY) {

                    if (strcmp(ones[iO].myChar,"John") == 0) {
                        psMany = johns;
                        iN = sizeof(johns)/sizeof(struct many);
                    }
                    else if (strcmp(ones[iO].myChar,"James") == 0) {
                        psMany = james;
                        iN = sizeof(james)/sizeof(struct many);
                    }
                    else if (strcmp(ones[iO].myChar,"Duncan") == 0) {
                        psMany = duncans;
                        iN = sizeof(duncans)/sizeof(struct many);
                    }
                    else
                        break;

                    /* Create member records and connect them to the owner. */
                    for (iM = 0; iM < iN && iStatus == S_OKAY; iM++) {
                        iStatus = d_fillnew(MANY,psMany,task,CURR_DB);
                        if (iStatus == S_OKAY) {
                            psMany++;
                            iStatus = d_connect(RELATIONSHIP,task,CURR_DB);
                            if (iStatus != S_OKAY) { 
                                fprintf(stderr,"Sorry, I can't connect one with many.");  
                            } 
                        }
                        else
                            fprintf(stderr,"Sorry, I can't create a new many record.");
                    }
                }
                else
                    fprintf(stderr,"Sorry, I can't set the new one record as owner.");
            }
            else
                fprintf(stderr,"Sorry, I can't create a new one record.");
        }

        if (iStatus == S_OKAY) {
            /* Read the one-to-may records and navigate the relationship. */
            for (iStatus = d_recfrst(ONE,task,CURR_DB);
                 iStatus == S_OKAY;
                 iStatus = d_recnext(task,CURR_DB)) {

                d_crget(&dba,task,CURR_DB); /* Keep track of the current record. */

                /* Read the full content of the current record */
                iStatus = d_recread(&sOne,task,CURR_DB);
                if (iStatus == S_OKAY) { 
                    printf("%s\n",sOne.myChar);

                    /* Set the owner of the relationship. */ 
                    iStatus = d_setor(RELATIONSHIP,task,CURR_DB);
                    if (iStatus == S_OKAY) {

                        /* Loop all the relationship members. */
                        for (iStatus = d_findfm(RELATIONSHIP,task,CURR_DB);
                                iStatus == S_OKAY;
                                iStatus = d_findnm(RELATIONSHIP,task,CURR_DB)) { /*lint !e445 */

                            /* Read a single field from the current member. */
                            iStatus = d_csmread(RELATIONSHIP,MANY_MYCHAR, sMany.myChar, task, CURR_DB); 
                            if (iStatus == S_OKAY) {
                                printf("   %s\n",sMany.myChar);
                            }
                            else
                                fprintf(stderr,"Sorry, I can't read the members myChar field.");
                        }
                    }
                    else
                        fprintf(stderr,"Sorry, I can't set the one record as relationship owner.");
                }
                else
                    fprintf(stderr,"Sorry, I can't read the one record.");
                d_crset(&dba,task,CURR_DB); /* Reset the current record. */
            } 
        }

        /* Close the database */
        d_close(task);
        d_closetask(task); 
    }
    else
        fprintf(stderr,"Sorry, I can't open the one_to_many database.");

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

PSP_STARTUP_EXAMPLE_CR(one_to_manyTutorial)

