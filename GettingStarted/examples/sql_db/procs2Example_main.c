#include "rdmsql.h"     /*lint -e534 */ /* Turn off return value checking for examples */

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
   Simple RDM SQL stored proc execution example 2
*/
int main()
{
    const RSQL_VALUE *row;
    RSQL_VALUE        arg;
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             hstmt;
    char              last_name[35];

    rsqlAllocConn(&hdbc);
    rsqlAllocStmt(hdbc, &hstmt);
    stat = rsqlOpenDB(hdbc, "bookshop", "s");
    if ( stat != errSUCCESS ) {
        printf("unable to open bookshop database\n");
        rsqlFreeConn(hdbc);
        exit((int)stat);
    }
    /* set up argument value container */
    arg.type = tCHAR;
    arg.status = vsOKAY;
    arg.len = 0;
    arg.vt.cv = last_name;
    
    for ( ; ; ) {
        /* get parameter values from user */
        gettext("\nenter author's last_name:", last_name, sizeof(last_name));
        if ( !last_name[0] )
            break;

        /* execute the execute statement */
        stat = rsqlExecProc(hstmt, "books_by_author", 1, &arg);
        if ( stat != errSUCCESS ) {
            printf("error attempting to execute proc\n");
            continue;
        }        
        /* fetch result set */
        printf("NAME                                TITLE\n");
        printf("----------------------------------- -----\n");
        while ( rsqlFetch(hstmt, &row, NULL) == errSUCCESS )
            printf("%-35.35s %s\n", row[0].vt.cv, row[1].vt.cv); 
    }
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    exit(0);
}
