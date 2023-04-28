#include "rdmsql.h" /*lint -e534 */ /* Turn off return value checking for example */

extern const VTFLOADTABLE vtFcnTable[];
extern const size_t vtFetchSz;

static void gettext(
    const char *prompt,
    char       *text,
    size_t      len)
{
    printf("%s ", prompt);
    if (fgets(text, len, stdin) == NULL )
        text[0] = '\0';
    else {
        char *nl = strchr(text, '\n');
        if ( nl )
            *nl = '\0';
    }
}

/* =======================================================================
   RDM SQL virtual table demo example
*/
int main()
{
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             hstmt;
    STMT_TYPE         stype;
    uint16_t          nocols, cno;
    const RSQL_VALUE *row;
    char              msgbuf[133];
    char              sqlstmt[256];

    rsqlAllocConn(&hdbc);
    
    rsqlRegisterVirtualTables(hdbc, "vtabs", 1, vtFcnTable, NULL, vtFetchSz);
    
    stat = rsqlOpenDB(hdbc, "vtabs", "x");
    if ( stat != errSUCCESS ) {
        printf("unable to open vtabs database\n");
        rsqlFreeConn(hdbc);
        exit((int)stat);
    }
    /* set up select statement cursor */
    rsqlAllocStmt(hdbc, &hstmt);
    
    for ( ; ; ) {
        gettext("enter sql (<enter> to quit):", sqlstmt, sizeof(sqlstmt));
        if ( ! sqlstmt[0] )
            break;
            
        stat = rsqlExecDirect(hstmt, sqlstmt);
        if ( stat != errSUCCESS ) {
            rsqlGetErrorInfo(hstmt, msgbuf, 132);
            printf("*** %s\n", msgbuf);
            continue;
        }
        rsqlGetStmtType(hstmt, &stype);
        if ( stype != sqlSELECT ) {
            printf("statement executed successfully\n");
            continue;
        }
        while ( rsqlFetch(hstmt, &row, &nocols) == errSUCCESS ) {
            for ( cno = 0; cno < nocols; ++cno ) {
                if ( cno > 0 ) printf(", ");
                if ( row[cno].type == tINTEGER )
                    printf("%d", row[cno].vt.lv);
                else
                    printf("\"%s\"", row[cno].vt.cv);
            }
            printf("\n");
        }
    }
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    
    exit(0);
}
