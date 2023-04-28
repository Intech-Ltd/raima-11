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
**    HELLO WORLD SQL
**    ---------------
**    This document describes the process to create a simple database, insert a record 
**    containing a text field, read the text field from database and print it out.
*/

#include <stdio.h>
#include "sqlrext.h" /* The RDM SQL API. */
/*lint -e838 */

#define SQL_EMPSTR      ((SQLCHAR *) "") /* any string */

int main(
    int                argc,
    const char *const *argv)
{  
    SQLRETURN iRet; /* holds return value, 0 is success */
    SQLCHAR   sz[32]; 
    SQLHDBC   hCon;  /* connection handle  */
    SQLHSTMT  hStmt; /* statement handle   */
    SQLHENV   hEnv;  /* environment handle */
    SQLLEN    iLen = 0;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    iRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);   /* Allocates env handle */
    iRet = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0); 
    iRet = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hCon);  /* Allocates connection handle */
    /* Connects to server */
    iRet = SQLConnect(hCon, SQL_EMPSTR, SQL_NTS, SQL_EMPSTR, SQL_NTS,
            SQL_EMPSTR, SQL_NTS); 

    if (iRet == SQL_SUCCESS) {
        /* connects to database*/
        iRet = SQLAllocHandle(SQL_HANDLE_STMT, hCon, &hStmt);  /* Allocates statement handle   */
        iRet = SQLExecDirect(hStmt, (SQLCHAR *) "open hello_worldODBC", SQL_NTS);  /* Opens the database           */
        if (iRet == SQL_SUCCESS) {
            /* Create a Hello World record */
            iRet = SQLAllocHandle(SQL_HANDLE_STMT, hCon, &hStmt); /* allocates the statement handle */
            if (iRet == SQL_SUCCESS) {
                iRet = SQLExecDirect(hStmt, (SQLCHAR *) "INSERT INTO info(myChar) VALUES('Hello World!')",
                        SQL_NTS);
                if (iRet == SQL_SUCCESS) {
                    /* Commit the insert, transaction is started by the first insert statement. */
                    iRet = SQLEndTran(SQL_HANDLE_DBC,hCon,SQL_COMMIT);
                    if (iRet != SQL_SUCCESS)
                        fprintf(stderr,"Sorry, I can't commit my changes to the database.");
                    if (iRet == SQL_SUCCESS) {
                        /* Query the database for the record created. */
                        iRet = SQLExecDirect(hStmt, (SQLCHAR *) "SELECT myChar FROM info",SQL_NTS);
                        if (iRet == SQL_SUCCESS) {
                            /* Bind SQL fields to program variables. */
                            iRet = SQLBindCol(hStmt,1,SQL_C_CHAR,sz,sizeof(sz),&iLen);
                            if (iRet == SQL_SUCCESS) {
                                /* Fetch data from database into program variables. */
                                if (SQLFetch(hStmt) == SQL_SUCCESS)
                                    printf("%s\n\n",sz);    
                                else
                                    fprintf(stderr,"Sorry, I can't fetch any rows of data from the database.");
                            }
                            else
                                fprintf(stderr,"Sorry, I can't bind myChar to my program variable.");
                        }
                        else
                            fprintf(stderr,"Sorry, I can't run the query statement.");
                    }
                }

                /* Free the SQL statement handle. */
                (void) SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
            }
            else
                fprintf(stderr,"Sorry, I can't allocate a SQL statement handle.");

            /* Close the database. */
            SQLDisconnect(hCon); /*lint !e534 */
        }
        else
            fprintf(stderr,"Sorry, I can't open the hello_world database.");

        /* Free the database handles. */
        (void) SQLFreeHandle(SQL_HANDLE_DBC, hCon);
        (void) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
    else
        fprintf(stderr, "Sorry, I can't allocate a database handle.");

    puts("Press ENTER to quit.");
    (void) getchar();

    return 0;
}
