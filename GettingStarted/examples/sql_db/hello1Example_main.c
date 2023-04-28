#include "rdmsql.h"     /*lint -e534 */ /* Turn off return value checking for examples */

/* =======================================================================
   Simple RDM SQL "Hello World!" Example #1
*/
int main()
{
    const RSQL_VALUE *row;
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             hstmt;
    
    rsqlAllocConn(&hdbc);
    rsqlAllocStmt(hdbc, &hstmt);

    /* create the database */
    rsqlExecDirect(hstmt, "create database hellodb");
    rsqlExecDirect(hstmt, "create table hellotab(txtln char(24))");
    stat = rsqlTransCommit(hdbc);
    if ( stat != errSUCCESS ) {
        printf("*** unable to connect to TFS\n");
        exit((int)stat);
    }
    
    /* insert a couple of rows into hellotab */
    rsqlExecDirect(hstmt, "insert into hellotab values \"Hello\"");
    rsqlExecDirect(hstmt, "insert into hellotab values \"World!\"");
    rsqlTransCommit(hdbc);

    /* retrieve and display the rows */
    rsqlExecDirect(hstmt, "select txtln from hellotab");
    while ( rsqlFetch(hstmt, &row, NULL) == errSUCCESS )
        printf("%s\n", row->vt.cv);
        
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    exit(0);
}
