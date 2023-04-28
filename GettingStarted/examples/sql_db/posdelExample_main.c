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
   RDM SQL positioned delete example
*/
int main()
{
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             sel_hstmt, del_hstmt;
    const RSQL_VALUE *row;
    char              reply[30];

    rsqlAllocConn(&hdbc);
    stat = rsqlOpenDB(hdbc, "bookshop", "s");
    if ( stat != errSUCCESS ) {
        printf("unable to open bookshop database\n");
        rsqlFreeConn(hdbc);
        exit((int)stat);
    }
    /* set up select statement cursor */
    rsqlAllocStmt(hdbc, &sel_hstmt);
    rsqlSetCursorName(sel_hstmt, "book_cursor");
    rsqlPrepare(sel_hstmt, "select bookid, last_name, title from book for update");
    
    /* set up delete statement */
    rsqlAllocStmt(hdbc, &del_hstmt);
    rsqlPrepare(del_hstmt, "delete from book where current of book_cursor");

    rsqlTransStart(hdbc, NULL);
    
    rsqlExecute(sel_hstmt);

    while ( rsqlFetch(sel_hstmt, &row, NULL) == errSUCCESS ) {
        printf("bookid   : %s\n", row[0].vt.cv);
        printf("last_name: %s\n", row[1].vt.cv);
        printf("title    : %s\n", row[2].vt.cv);
        gettext("do you want to delete this book (y|n)?", reply, sizeof(reply));
        if ( reply[0] == 'y' )
            rsqlExecute(del_hstmt);
        
        gettext("continue (y|n)?", reply, sizeof(reply));
        if ( reply[0] != 'y' )
            break;
    }
    rsqlTransCommit(hdbc);

    rsqlFreeStmt(sel_hstmt);
    rsqlFreeStmt(del_hstmt);
    rsqlFreeConn(hdbc);
    
    exit(0);
}
