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
   Simple RDM SQL parameter markers example 2 including blobs
*/
int main()
{
    HCONN         hdbc;
    HSTMT         hstmt;
    
    char          last_name[14] = "";
    char          full_name[35] = "";
    int32_t       full_name_len = 0;
    char          gender[2] = " ";
    int32_t       gender_len = 0;
    int16_t       yr_born = 0;
    int32_t       yr_born_len = 0;
    int16_t       yr_died = 0;
    int32_t       yr_died_len = 0;
    char          year[5];
    char          bio[132] = "";
    int32_t       data_at_exec = -2;
    uint32_t      short_bio_len;
    
    char          stmt[] = "insert into author values ?, ?, ?, ?, ?, ?";

    rsqlAllocConn(&hdbc);
    rsqlAllocStmt(hdbc, &hstmt);
    rsqlOpenDB(hdbc, "bookshop", "s");
    
    rsqlPrepare(hstmt, stmt);
    
    /* bind all 6 parameters */
    rsqlBindParam(hstmt, 1, tCHAR,      last_name, NULL);
    rsqlBindParam(hstmt, 2, tCHAR,      full_name, &full_name_len);
    rsqlBindParam(hstmt, 3, tCHAR,      gender,    &gender_len);
    rsqlBindParam(hstmt, 4, tSMALLINT, &yr_born,   &yr_born_len);
    rsqlBindParam(hstmt, 5, tSMALLINT, &yr_died,   &yr_died_len);
    rsqlBindParam(hstmt, 6, tCLOB,      bio,       &data_at_exec);
 
    for ( ; ; ) {
        /* get parameter values from user */
        gettext("enter last_name:", last_name, sizeof(last_name));
        if ( !last_name[0] ) break;

        gettext("enter full_name:", full_name, sizeof(full_name));
        full_name_len = full_name[0] ? 0 : -1;
        
        gettext("enter gender (M/F):", gender, sizeof(gender));
        if ( !gender[0] )
            gender_len = -1;
        else if ( gender[0] == 'M' || gender[0] == 'F' ) 
            gender_len = 0;
        else {
            printf("gender must be a M or F\n");
            continue;
        }
            
        gettext("enter year born:", year, sizeof(year));
        if ( year[0] ) {
            yr_born = (int16_t)atoi(year);
            yr_born_len = 0;
        }
        else
            yr_born_len = -1;
        
        gettext("enter year died:", year, sizeof(year));
        if ( year[0] ) {
            yr_died = (int16_t)atoi(year);
            yr_died_len = 0;
        }
        else
            yr_died_len = -1;
            
        rsqlTransStart(hdbc, NULL);
            
        /* execute select statement */
        if ( rsqlExecute(hstmt) != errNEEDDATA ) {
            printf("rsqlExecute did NOT return errNEEDDATA!!\n");
            break;
        }
        while ( rsqlParamData(hstmt, NULL, NULL) == errNEEDDATA ) {
            for ( ; ; ) {
                gettext("enter short_bio:", bio, sizeof(bio));
                if ( !bio[0] )
                    break;
                short_bio_len = (uint32_t)strlen(bio);
                rsqlPutData(hstmt, bio, short_bio_len);
            }
            /* add a null terminator */
            rsqlPutData(hstmt, "", 1);
        }
        rsqlTransCommit(hdbc);
    }
    rsqlFreeStmt(hstmt);
    rsqlFreeConn(hdbc);
    exit(0);
}
