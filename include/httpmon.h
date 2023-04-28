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
#if !defined(HTTPMON_H)
#define HTTPMON_H

#include "psp.h"
#include "rdmhttp.h"

#if defined(__cplusplus)
extern "C" {
#endif


typedef struct {
    const char *fcnName;
    int32_t   (*fcn)(RHS_HANDLE, char **, const URLARGS *, const char *);
} HTTPACTIONS;

int32_t EXTERNAL_FCN httpmon_RunAction(RHS_HANDLE, PSP_OUTPUTBUFF, const char *, const char *, const HTTPACTIONS *, uint32_t);
char  * EXTERNAL_FCN httpmon_urlEscape(const char *, size_t, PSP_MEMTAG);

#if defined(__cplusplus)
}
#endif

#endif /* HTTPMON_H */
