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
   Simple RDM SQL stored proc execution example 3
*/
int main()
{
    const RSQL_VALUE *row, *pColval;
    RSQL_VALUE        arg;
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             hstmt;
    uint32_t          remlen;
    char              short_bio[81];
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
    
    /* turn on deferred blob reading mode */
    rsqlSetDeferBlobMode(hstmt, 1);
    
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
        /* fetch 1st select's result set */
        while ( rsqlFetch(hstmt, &row, NULL) == errSUCCESS ) {
            printf("\nauthor       : %s\n", row[0].vt.cv);
            printf("year of birth: %d\n", row[1].vt.sv);
            printf("--------------------------------------------------------------\n");
 
            /* fetch short_bio blob data */
            while ( rsqlGetData(hstmt, 3, &pColval, 80, &remlen) == errSUCCESS ) {
                if ( pColval->type == tNULL || remlen == 0 ) {
                    printf("None\n");
                    break;
                }
                /* copy blob data block and add null terminator */
                memcpy(short_bio, pColval->vt.lvv.buf, pColval->len);
                short_bio[pColval->len] = '\0';
                printf("%s\n", short_bio);
            }
        }
        /* execute and fetch 2nd select's result set */
        if ( rsqlMoreResults(hstmt) != errSUCCESS ) {
            printf("Second SELECT not in books_by_author\n");
            break;
        }
        printf("\ntitles in stock\n---------------\n");
        while ( rsqlFetch(hstmt, &row, NULL) == errSUCCESS )
            printf("%s\n", row[0].vt.cv);
    }
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    exit(0);
}
