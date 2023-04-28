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

/* Delete technical information records from TIMS database
*/
int32_t del_info(DB_TASK *task)
{
    int32_t status;
    struct info irec;
    int32_t count;
    char id[SIZEOF_INFO_ID_CODE], name[SIZEOF_AUTHOR_NAME];

    /* find info to be deleted */
    printf("id_code: ");
    getstring(id,SIZEOF_INFO_ID_CODE);
    if (d_keyfind(INFO_ID_CODE, id, task, CURR_DB) == S_NOTFOUND)
    {
        printf("id_code %s not on file\n", id);
        return (0);
    }
    d_recread(&irec, task, CURR_DB);

    /* get author name */
    d_findco(HAS_PUBLISHED, task, CURR_DB);
    d_crread(AUTHOR_NAME, name, task, CURR_DB);

    /* confirm delete request */
    printf("author: %s\n", name);
    printf("title : %s\n", irec.info_title);
    printf("delete (y/n)? ");
    getstring(id,SIZEOF_INFO_ID_CODE);
    if (id[0] != 'Y' && id[0] != 'y')
        return (0);

    /* disconnect any listed articles */
    d_setom(ARTICLE_LIST, HAS_PUBLISHED, task, CURR_DB);
    for (status = d_findfm(ARTICLE_LIST, task, CURR_DB); status == S_OKAY;
         status = d_discon(ARTICLE_LIST, task, CURR_DB))
        ;

    /* disconnect and delete borrowers */
    d_setom(LOANED_BOOKS, HAS_PUBLISHED, task, CURR_DB);
    while (d_findfm(LOANED_BOOKS, task, CURR_DB) == S_OKAY)
    {
        d_discon(LOANED_BOOKS, task, CURR_DB);
        d_setmr(LOAN_HISTORY, task, CURR_DB);
        d_discon(LOAN_HISTORY, task, CURR_DB);
        d_delete(task, CURR_DB);
    }

    /* disconnect and delete abstract */
    d_setom(ABSTRACT, HAS_PUBLISHED, task, CURR_DB);
    while (d_findfm(ABSTRACT, task, CURR_DB) == S_OKAY)
    {
        d_discon(ABSTRACT, task, CURR_DB);
        d_delete(task, CURR_DB);
    }

    /* disconnect and delete intersect and (possibly) key word */
    d_setom(INFO_TO_KEY, HAS_PUBLISHED, task, CURR_DB);
    while (d_findfm(INFO_TO_KEY, task, CURR_DB) == S_OKAY)
    {
        d_discon(INFO_TO_KEY, task, CURR_DB);
        d_setmr(KEY_TO_INFO, task, CURR_DB);
        d_discon(KEY_TO_INFO, task, CURR_DB);
        d_delete(task, CURR_DB);
        d_members(KEY_TO_INFO, &count, task, CURR_DB);
        if (count == 0L)
        {
            /* delete key word */
            d_setro(KEY_TO_INFO, task, CURR_DB);
            d_delete(task, CURR_DB);
        }
    }

    /* disconnect info record from author and delete */
    d_discon(HAS_PUBLISHED, task, CURR_DB);
    d_delete(task, CURR_DB);

    /* delete author too, if he has no other pubs */
    d_members(HAS_PUBLISHED, &count, task, CURR_DB);
    if (count == 0L)
    {
        d_setmo(AUTHOR_LIST, HAS_PUBLISHED, task, CURR_DB);
        d_discon(AUTHOR_LIST, task, CURR_DB);
        d_delete(task, CURR_DB);
    }
    return (0);
}


