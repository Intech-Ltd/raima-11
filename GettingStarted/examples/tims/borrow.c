/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include "rdm.h"
/*lint ++flb -e957 */
#include "tims.h"
/*lint --flb */

extern char *getstring(char *, size_t); /*lint -esym(534,getstring) */

/* Loan book
*/
int32_t loan_book(DB_TASK *task)
{
    char id[SIZEOF_INFO_ID_CODE];
    char date[20];
    struct borrower brec;

    printf("id_code of book to be loaned: ");
    if (getstring(id,SIZEOF_INFO_ID_CODE) && id[0] != '\0')
    {
        if (d_keyfind(INFO_ID_CODE, id, task, CURR_DB) == S_NOTFOUND)
            printf("id_code %s not on file\n", id);
        else
        {
            d_setor(LOANED_BOOKS, task, CURR_DB);
            printf("name of borrower: ");
            getstring(brec.frnd,SIZEOF_BORROWER_FRND);
            printf("date borrowed   : ");
            getstring(date,sizeof(date));
            sscanf(date, "%d", &brec.date_borrowed);
            brec.date_returned = 0L;
            d_fillnew(BORROWER, &brec, task, CURR_DB);
            d_connect(LOANED_BOOKS, task, CURR_DB);
            d_connect(LOAN_HISTORY, task, CURR_DB);
        }
    }
    return (0);
}


/* Returned borrowed book
*/
int32_t return_book(DB_TASK *task)
{
    char id[SIZEOF_INFO_ID_CODE];
    char frnd[SIZEOF_BORROWER_FRND];
    char date[20];
    int32_t bdate;
    struct borrower brec;

    printf("id_code of returned book: ");
    if (getstring(id,SIZEOF_INFO_ID_CODE) && id[0] != '\0')
    {
        if (d_keyfind(INFO_ID_CODE, id, task, CURR_DB) == S_NOTFOUND)
            printf("id_code %s not on file\n", id);
        else
        {
            d_setor(LOANED_BOOKS, task, CURR_DB);
            printf("name of borrower: ");
            getstring(frnd,SIZEOF_BORROWER_FRND);
            while (d_findnm(LOANED_BOOKS, task, CURR_DB) == S_OKAY)
            {
                d_recread(&brec, task, CURR_DB);
                if (strcmp(brec.frnd, frnd) == 0)
                {
                    if (brec.date_returned == 0L)
                    {
                        printf("book borrowed on: %d\n", brec.date_borrowed);
                        printf("date returned   : ");
                        getstring(date,sizeof(date));
                        sscanf(date, "%d", &bdate);
                        d_crwrite(BORROWER_DATE_RETURNED, &bdate, task, CURR_DB);
                        return (0);
                    }
                }
            }
            printf("borrower not on file\n");
        }
    }
    return (0);
}


/* List loaned out books
*/
int32_t list_loaners(DB_TASK *task)
{
    int32_t status;
    struct borrower brec;
    char title[80];

    for (status = d_findfm(LOAN_HISTORY, task, CURR_DB); status == S_OKAY;
          status = d_findnm(LOAN_HISTORY, task, CURR_DB))
    {
        d_recread(&brec, task, CURR_DB);
        if (brec.date_returned == 0L)
        {
            d_findco(LOANED_BOOKS, task, CURR_DB);
            d_crread(INFO_INFO_TITLE, title, task, CURR_DB);
            printf("title: %s\n", title);
            printf("borrowed by %s on %d\n\n", brec.frnd, brec.date_borrowed);
        }
    }
    return (0);
}


