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

/* List authors
*/
int32_t list_authors(DB_TASK *task)
{
    int32_t stat;
    char start[SIZEOF_AUTHOR_NAME], name[SIZEOF_AUTHOR_NAME];

    printf("start name: ");
    getstring(start,SIZEOF_AUTHOR_NAME);
    if (start[0] != '\0')
    {
        /* scan for first name */
        for (stat = d_findfm(AUTHOR_LIST, task, CURR_DB); stat == S_OKAY;
             stat = d_findnm(AUTHOR_LIST, task, CURR_DB))
        {
            d_crread(AUTHOR_NAME, name, task, CURR_DB); /*lint !e534 */
            if (strcmp(start, name) <= 0)
                break;
        }
    }
    else
        stat = d_findfm(AUTHOR_LIST, task, CURR_DB);

    for ( ; stat == S_OKAY; stat = d_findnm(AUTHOR_LIST, task, CURR_DB))
    {
        d_crread(AUTHOR_NAME, name, task, CURR_DB); /*lint !e534 */
        printf("   %s\n", name);
    }

    printf("--- press <enter> to continue");
    getstring(name,SIZEOF_AUTHOR_NAME);

    return 0;
}

