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
   Simple RDM SQL parameter markers example 1
*/
int main()
{
    const RSQL_VALUE *row;
    RSQL_ERRCODE      stat;
    HCONN             hdbc;
    HSTMT             hstmt;
    char              buf[250];
    int16_t           lo_born = 0, hi_born = 0;
    char              gender[2] = "";
    char              stmt[] = "select full_name, yr_born, yr_died from author "
                                   "where gender = ? and yr_born between ? and ?";

    rsqlAllocConn(&hdbc);
    rsqlAllocStmt(hdbc, &hstmt);
    stat = rsqlOpenDB(hdbc, "bookshop", "s");
    if ( stat != errSUCCESS ) {
        printf("unable to open bookshop database\n");
        rsqlFreeConn(hdbc);
        exit((int)stat);
    }
    rsqlPrepare(hstmt, stmt);
    rsqlBindParam(hstmt, 1, tCHAR, gender, NULL);
    rsqlBindParam(hstmt, 2, tSMALLINT, &lo_born, NULL);
    rsqlBindParam(hstmt, 3, tSMALLINT, &hi_born, NULL);

    for ( ; ; ) {
        /* get parameter values from user */
        gettext("\nenter gender (M/F):", gender, sizeof(gender));
        if ( gender[0] != 'M' && gender[0] != 'F' ) {
            printf("gender must be a M or F\n");
            continue;
        }

        gettext("\nenter low  year born:", buf, sizeof(buf));
        lo_born = (int16_t)atoi(buf);
        if ( lo_born == 0 )
            break;
        
        gettext("enter high year born:", buf, sizeof(buf));
        hi_born = (int16_t)atoi(buf);
        if ( hi_born == 0 )
            break;
        
        if ( lo_born > hi_born ) {
            printf("low year born must be less or equal to high!\n");
            continue;
        }
        /* execute select statement */
        rsqlExecute(hstmt);
        
        /* fetch result set */
        printf("NAME                                YR_BORN YR_DIED\n");
        printf("----------------------------------- ------- -------\n");
        while ( rsqlFetch(hstmt, &row, NULL) == errSUCCESS )
            printf("%-35.35s %4d    %4d\n", 
                row[0].vt.cv, row[1].vt.sv, row[2].vt.sv); 
    }
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    exit(0);
}
