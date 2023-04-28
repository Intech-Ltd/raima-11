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

static struct info irec; /*lint !e956 */
static struct author arec; /*lint !e956 */

static void enter_key_words(DB_TASK *task);
static void enter_abstract(DB_TASK *task);
static int32_t get_info(void);

extern char *getstring(char *, size_t); /*lint -esym(534,getstring) */

/* Enter technical information records into TIMS database
*/
int32_t ent_info(DB_TASK *task)
{
    int32_t status;
    char s[SIZEOF_AUTHOR_NAME];  /* generic string variable */

    /* enter tech info into TIMS database */
    while (get_info() != EOF)
    {
        /* see if author exists */
        for (status = d_findfm(AUTHOR_LIST, task, CURR_DB); status == S_OKAY;
             status = d_findnm(AUTHOR_LIST, task, CURR_DB))
        {
            d_crread(AUTHOR_NAME, s, task, CURR_DB);
            if (strcmp(arec.name, s) == 0)
                break;                        /* author record on file */
        }
        if (status == S_EOS)
        {
            /* author not on file -- create record and connect to author list */
            d_fillnew(AUTHOR, &arec, task, CURR_DB);
            d_connect(AUTHOR_LIST, task, CURR_DB);
        }
        /* make author current owner of has_published set */
        d_setor(HAS_PUBLISHED, task, CURR_DB);

        /* create new tech. info record */
        if (d_fillnew(INFO, &irec, task, CURR_DB) == S_DUPLICATE)
            printf("duplicate id_code: %s\n", irec.id_code);
        else
        {
            /* connect to author record */
            d_connect(HAS_PUBLISHED, task, CURR_DB);

            /* set current owner for key words and abstract */
            d_setor(INFO_TO_KEY, task, CURR_DB);
            d_setor(ABSTRACT, task, CURR_DB);

            enter_key_words(task);

            enter_abstract(task);
        }
    }
    return (0);
}



/* Enter any key words
*/
static void enter_key_words(DB_TASK *task)
{
    char s[SIZEOF_KEY_WORD_KWORD];

    for (;;)
    {
        printf("key word: ");
        if (getstring(s,SIZEOF_KEY_WORD_KWORD) == NULL || s[0] == '\0')
            break;
        /* see if key word record exists */
        if (d_keyfind(KEY_WORD_KWORD, s, task, CURR_DB) == S_NOTFOUND)
        {
            /* create new key word record */
            d_fillnew(KEY_WORD, s, task, CURR_DB);
        }
        d_setor(KEY_TO_INFO, task, CURR_DB);

        /* create intersection record */
        d_fillnew(INTERSECT, &irec.info_type, task, CURR_DB);
        d_connect(KEY_TO_INFO, task, CURR_DB);
        d_connect(INFO_TO_KEY, task, CURR_DB);
    }
}


/* Enter abstract description
*/
static void enter_abstract(DB_TASK *task)
{
    char text_line[SIZEOF_INFOTEXT_LINE];

    for (;;)
    {
        printf("abstract: ");
        if (getstring(text_line,SIZEOF_INFOTEXT_LINE) == NULL || text_line[0] == '\0')
            return;

        d_fillnew(INFOTEXT, text_line, task, CURR_DB);
        d_connect(ABSTRACT, task, CURR_DB);
    }
}



/* Fill irec, arec with info data from user
*/
static int32_t get_info()
{
    char txt[40];

    printf("author   : ");
    if (getstring(arec.name,SIZEOF_AUTHOR_NAME) == NULL || arec.name[0] == '\0')
        return (EOF);
    else
    {
        for (;;)
        {
            printf("id_code  : ");
            getstring(irec.id_code,SIZEOF_INFO_ID_CODE);
            printf("title    : ");
            getstring(irec.info_title,SIZEOF_INFO_INFO_TITLE);
            printf("publisher: ");
            getstring(irec.publisher,SIZEOF_INFO_PUBLISHER);
            printf("pub. date: ");
            getstring(irec.pub_date,SIZEOF_INFO_PUB_DATE);
            for (;;)
            {
                printf("info type: ");
                getstring(txt,sizeof(txt));
                sscanf(txt, "%hd", &irec.info_type);
                if (irec.info_type >= 0 && irec.info_type <= 2)
                    break;
                printf("invalid info type - correct types are:\n");
                printf("0 - book, 1 = magazine, 2 = article\n");
            }
            printf("enter data (y/n)? ");
            getstring(txt,sizeof(txt));
            if (txt[0] == 'y' || txt[0] == 'Y')
                return (0);
        }
    }
}

