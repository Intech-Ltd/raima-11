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
**    HELLO WORLD RSQL
**    ---------------
**    This document describes the process to create a simple database, insert
**    a record containing a text field, read the text field from database and
**    print it out.
*/
#include <stdio.h>
#include "rdmsql.h"    /* The RDM SQL API. */

int main(
    int                argc,
    const char *const *argv)
{  
    HCONN             hdbc;
    HSTMT             hstmt;
    RSQL_ERRCODE      stat;
    const RSQL_VALUE *row;
    uint16_t          colCount;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    /* Initialize our PSP system */
    if (psp_init() != PSP_OKAY)
        return EXIT_FAILURE;

    /* Allocate a connection handle */
    if ((stat = rsqlAllocConn(&hdbc)) == errSUCCESS) {
        /* Allocate a statement handle */
    if ((stat = rsqlAllocStmt(hdbc, &hstmt)) == errSUCCESS) {
        /* Connect */
        stat = rsqlOpenDB(hdbc, "hello_world", "s");
        if (stat == errSUCCESS) {
                /* Create a Hello World record */
                stat = rsqlExecDirect(hstmt,
                        "INSERT INTO info(myChar) VALUES ('Hello World!')");
                if (stat == errSUCCESS) {
                    /* Query the database for the record created. */
                    stat = rsqlExecDirect(hstmt,"SELECT myChar FROM info");
                    if (stat == errSUCCESS) {
                        /* Fetch data from database into program variables. */
                        stat = rsqlFetch(hstmt, &row, &colCount);
                        if (stat == errSUCCESS)
                            printf("%s\n\n",row[0].vt.cv);    
                        else
                            fprintf(stderr,"Sorry, I can't fetch any rows of data from the database.");
                    }
                    else
                        fprintf(stderr,"Sorry, I can't run the query statement.");
                }

                /* Close the database. */
                (void) rsqlCloseDB(hdbc, "hello_world");
            } 
        }
        else
            fprintf(stderr,"Sorry, I can't allocate a SQL statement handle.");

        /* Free the connection. */
        (void) rsqlFreeConn(hdbc);
    }
    else
        fprintf(stderr,"Sorry, I can't allocate a connection handle.");

    /* Cleanup PSP system */
    psp_term();

    puts("Press ENTER to quit.");
    (void) getchar();

    return EXIT_SUCCESS;
}

