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

#if !defined(__TFSADMIN_H)
#define __TFSADMIN_H

#include "rdmhttp.h"

#if defined(__cplusplus)
extern "C" {
#endif

int32_t EXTERNAL_FCN tfs_admin_handler(RHS_HANDLE, PSP_OUTPUTBUFF, const char *, const char *, const char *);
int32_t EXTERNAL_FCN tfs_proxy_handler(RHS_HANDLE, PSP_OUTPUTBUFF, const char *, const char *, const char *);


#if defined(__cplusplus)
}
#endif

#endif  /* __TFSADMIN_H */

