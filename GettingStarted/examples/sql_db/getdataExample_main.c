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
   Simple RDM SQL example retrieving blob data using rsqlGetData 
*/
int main()
{
    const RSQL_VALUE *pColval;
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             hstmt;
    char              last_name[40] = "";
    char              short_bio[81];
    uint32_t          remlen;
    char              stmt[] = "select full_name, short_bio from author where last_name like ? for read only";

    rsqlAllocConn(&hdbc);
    rsqlAllocStmt(hdbc, &hstmt);
    stat = rsqlOpenDB(hdbc, "bookshop", "s");
    if ( stat != errSUCCESS ) {
        printf("unable to open bookshop database\n");
        rsqlFreeConn(hdbc);
        exit((int)stat);
    }
    rsqlPrepare(hstmt, stmt);
    rsqlBindParam(hstmt, 1, tCHAR, last_name, NULL);

    for ( ; ; ) {
        /* get parameter value from user */
        gettext("\nenter author's last_name:", last_name, sizeof(last_name)-1);
        if (!last_name[0]) break;
        strcat(last_name, "%");
            
        /* execute select statement */
        rsqlExecute(hstmt);
        stat = rsqlFetch(hstmt, NULL, NULL);
        if ( stat != errSUCCESS ) {
            printf("author %s not in database\n", last_name);
            continue;
        }
        /* author's full_name */
        rsqlGetData(hstmt, 1, &pColval, 0, NULL);
        printf("%s:\n", pColval->vt.cv);
        
        /* fetch short_bio blob data */
        while ( rsqlGetData(hstmt, 2, &pColval, 80, &remlen) == errSUCCESS ) {
            if ( pColval->type == tNULL || remlen == 0 ) {
                printf("No short_bio has been entered\n");
                break;
            }
            /* copy blob data block and add null terminator */
            memcpy(short_bio, pColval->vt.lvv.buf, pColval->len);
            short_bio[pColval->len] = '\0';
            printf("%s\n", short_bio);
        }
        rsqlCloseStmt(hstmt);
    }
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    exit(0);
}
