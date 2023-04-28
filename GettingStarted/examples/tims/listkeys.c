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
/*lint ++flb */
#include "tims.h"
/*lint --flb */

extern char *getstring(char *, size_t); /*lint -esym(534,getstring) */

/* List key words
*/
int32_t list_keys(DB_TASK *task)
{
    int32_t stat;
    char    key[SIZEOF_KEY_WORD_KWORD];

    printf("start key: ");
    getstring(key,SIZEOF_KEY_WORD_KWORD);

    if (key[0] != '\0')
    {
        if ((stat = d_keyfind(KEY_WORD_KWORD, key, task, CURR_DB)) == S_NOTFOUND)
            stat = d_keynext(KEY_WORD_KWORD, task, CURR_DB);
    }
    else
        stat = d_keyfrst(KEY_WORD_KWORD, task, CURR_DB);

    /* scan thru keys */
    for ( ; stat == S_OKAY; stat = d_keynext(KEY_WORD_KWORD, task, CURR_DB))
    {
        d_keyread(key, task); /*lint !e534 */
        printf("   %s\n", key);
    }

    printf("--- press <enter> to continue");
    getstring(key,SIZEOF_KEY_WORD_KWORD);

    return 0;
}

