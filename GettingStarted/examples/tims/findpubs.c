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

static void pr_keywords(DB_TASK *task);
static void pr_abstract(DB_TASK *task);

extern char *getstring(char *, size_t); /*lint -esym(534,getstring) */

/* Find publications by key word
*/
void by_key(DB_TASK *task)
{
    uint16_t    len;
    int32_t     status;
    struct info irec;                /* info record variable */
    char        name[SIZEOF_AUTHOR_NAME];          /* author's name */
    char        key[SIZEOF_KEY_WORD_KWORD];          /* key word */
    char        cmd[10];


    /* find key word record */
    printf("key word (partial okay): ");
    getstring(key,SIZEOF_KEY_WORD_KWORD);
    len = (uint16_t) strlen(key);

    if (d_pkeyfind(KEY_WORD_KWORD, 0, len, key, task, CURR_DB) == S_NOTFOUND)
    {
        printf("no records found\n");
        return;
    }

    do {
        /* scan thru key_to_info set */
        d_setor(KEY_TO_INFO, task, CURR_DB);
        for (status = d_findfm(KEY_TO_INFO, task, CURR_DB); status == S_OKAY;
             status = d_findnm(KEY_TO_INFO, task, CURR_DB))
        {
            /* find current owner (info) of current record (intersect) */
            d_findco(INFO_TO_KEY, task, CURR_DB);

            /* read contents of info record */
            d_recread(&irec, task, CURR_DB);

            /* find author of info record */
            d_findco(HAS_PUBLISHED, task, CURR_DB);
            d_crread(AUTHOR_NAME, name, task, CURR_DB);

            /* print results */
            printf("id_code: %s\n", irec.id_code);
            printf("author : %s\n", name);
            printf("title  : %s\n", irec.info_title);
            printf("publ.  : %s, %s\n", irec.publisher, irec.pub_date);
            pr_keywords(task);
            pr_abstract(task);
            printf("--- press <enter> to continue");
            getstring(cmd,sizeof(cmd));
        }
    }
    while (d_pkeynext(KEY_WORD_KWORD, 0, len, key, task, CURR_DB) == S_OKAY);
}


/* Find publication by author
*/
int32_t by_author(DB_TASK *task)
{
    int32_t status;
    size_t searchLen;
    struct info irec;                /* info record variable */
    char search[SIZEOF_AUTHOR_NAME];        /* author to search for */
    char name[SIZEOF_AUTHOR_NAME];
    char cmd[10];

    /* find author record */
    printf("author: ");
    getstring(search,SIZEOF_AUTHOR_NAME);
    searchLen = strlen(search);
    for (status = d_findfm(AUTHOR_LIST, task, CURR_DB); status == S_OKAY;
         status = d_findnm(AUTHOR_LIST, task, CURR_DB))
    {
        d_crread(AUTHOR_NAME, name, task, CURR_DB);
        if (strncmp(search, name, searchLen) == 0)
        {
            d_setor(HAS_PUBLISHED, task, CURR_DB);
            for (status = d_findfm(HAS_PUBLISHED, task, CURR_DB);
                 status == S_OKAY;
                 status = d_findnm(HAS_PUBLISHED, task, CURR_DB)) /*lint !e445 */
            {
                d_recread(&irec, task, CURR_DB);

                /* read and print info record */
                printf("id_code: %s\n", irec.id_code);
                printf("author : %s\n", name);
                printf("title  : %s\n", irec.info_title);
                printf("publ.  : %s, %s\n", irec.publisher, irec.pub_date);
                pr_keywords(task);
                pr_abstract(task);
                printf("--- press <enter> to continue");
                getstring(cmd,sizeof(cmd));
            }
        }
        else if (strcmp(search, name) < 0)
        {
            printf("author record not found\n");
            return (0);
        }
    }
    return (0); /*lint !e850 */
}


/* Print key words
*/
static void pr_keywords(DB_TASK *task)
{
    int32_t count;                      /* number of info_to_key members */
    char key[SIZEOF_KEY_WORD_KWORD];          /* key word or phrase */
    DB_ADDR dba;                     /* db addr of key_to_info member */

    /* the current member of the has_published set is the info record whose
     * key words are to be listed */
    d_setom(INFO_TO_KEY, HAS_PUBLISHED, task, CURR_DB);

    /* fetch number of members of info_to_key */
    d_members(INFO_TO_KEY, &count, task, CURR_DB);

    /* list the key words, if any */
    if (count > 0L)
    {
        /* save current member of key_to_info because it's going to change and
         * we may be currently scanning through that set */
        d_csmget(KEY_TO_INFO, &dba, task, CURR_DB);

        printf("key words:\n----------\n");
        /* find each intersect member record */
        while (d_findnm(INFO_TO_KEY, task, CURR_DB) == S_OKAY)
        {
            /* find, read and print corresponding key_word */
            d_findco(KEY_TO_INFO, task, CURR_DB);
            d_crread(KEY_WORD_KWORD, key, task, CURR_DB);
            printf("   %s\n", key);
        }
        printf("\n");

        /* reset key_to_info current member and owner */
        if (!DBA_ISNULL(dba))
            d_csmset(KEY_TO_INFO, &dba, task, CURR_DB);
    }
}



/* Print abstract
*/
static void pr_abstract(DB_TASK *task)
{
    int32_t count;                      /* number of abstract members */
    char txt[80];                    /* line of abstract text */

    /* the current member of has_published is the info record whose abstract
     * is to be printed */
    d_setom(ABSTRACT, HAS_PUBLISHED, task, CURR_DB);

    /* fetch number of lines in abstract */
    d_members(ABSTRACT, &count, task, CURR_DB);

    /* print abstract, if one exists */
    if (count > 0)
    {
        printf("abstract:\n---------\n");

        /* find, read and print each abstract text line */
        while (d_findnm(ABSTRACT, task, CURR_DB) != S_EOS)
        {
            d_csmread(ABSTRACT, INFOTEXT_LINE, txt, task, CURR_DB);
            printf("  %s\n", txt);
        }
    }
    printf("\n");
}

