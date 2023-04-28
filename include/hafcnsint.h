/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

/* hafcns.h - Contains definitions for the HA functions */

#if !defined(__HAFCNSINT_H)
#define __HAFCNSINT_H

#include "rdm.h"
#include "hafcns.h"

#if defined(__cplusplus)
extern "C" {
#endif

int32_t EXTERNAL_FCN ha_rpc_reqhandler(void *);

DB_BOOLEAN  EXTERNAL_FCN ha_init(const char *, uint32_t);
void        EXTERNAL_FCN ha_term(void);
int32_t     EXTERNAL_FCN ha_setstatus(const char *, int32_t);

#if defined(__cplusplus)
}
#endif

#endif  /* __HAFCNSINT_H */

