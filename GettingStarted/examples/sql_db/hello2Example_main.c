#include "rdmsql.h"     /*lint -e534 */ /* Turn off return value checking for examples */

/* error data structure */
typedef struct {
    jmp_buf    errexit;
    HCONN      hdbc;
    HSTMT      hstmt;
    int        erractive;
} ERR_DATA;

/* =======================================================================
   Report error
*/
static RSQL_ERRCODE EXTERNAL_FCN report_error(
    HRSQL           hrsql,
    RSQL_ERRCODE    stat,
    void           *data)
{
    const char *emsg;
    char        errmsg[131];
    ERR_DATA   *errdata = (ERR_DATA *) data;
    
    if ( errdata->erractive ) {
        errdata->erractive = 0;
        return stat;
    }
    if ( errdata && errdata->hstmt ) {
        errdata->erractive = 1;
        rsqlGetErrorInfo(errdata->hstmt, errmsg, 132);
        printf("*** error: %s\n", errmsg);
        rsqlTransRollback(errdata->hdbc, NULL);
        longjmp(errdata->errexit, (int32_t)stat);
    }
    rsqlGetErrorMsg(stat, &emsg);
    printf("*** error: %s\n", emsg);
            
    return stat;
} /*lint !e715 */ /* hrsql is not used in the function */

/* =======================================================================
   Simple RDM SQL "Hello World!" Example #2
*/
int main()
{
    const RSQL_VALUE *row;
    RSQL_ERRCODE      stat;
    HCONN             hdbc  = NULL;
    HSTMT             hstmt = NULL;
    ERR_DATA          errdata;
    
    errdata.erractive = 0;
    
    stat = (RSQL_ERRCODE)setjmp(errdata.errexit);
    if (stat != errSUCCESS)
        exit((int) stat);
        
    stat = rsqlAllocConn(&hdbc);
    if ( stat != errSUCCESS )
        return (int) report_error(NULL, stat, NULL);

    errdata.hdbc = hdbc;
        
    rsqlSetErrorCallback(hdbc, report_error, &errdata);
    
    rsqlAllocStmt(hdbc, &hstmt);
    errdata.hstmt = hstmt;

    /* create the database */
    rsqlExecDirect(hstmt, "create database hellodb");
    rsqlExecDirect(hstmt, "create table hellotab(txtln char(24))");
    rsqlTransCommit(hdbc);

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
