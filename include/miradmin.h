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

#if !defined(__MIRADMIN_H)
#define __MIRADMIN_H

#include "rdmhttp.h"

#if defined(__cplusplus)
extern "C" {
#endif

int32_t EXTERNAL_FCN mir_admin_handler(RHS_HANDLE, PSP_OUTPUTBUFF, const char *, const char *, const char *);

/* shared by dbmirror, dbrep and dbrepsql */
static const TFS_XAPI_FCNMAP mirfcnmap[] = {
    /* name             function                 */
    { "/admin.rdm",    (TFS_XAPI_FCN *)mir_admin_handler  },
};

#define MAX_MIRFCNS     RLEN(mirfcnmap)

#if defined(__cplusplus)
}
#endif

#endif  /* __MIRADMIN_H */

