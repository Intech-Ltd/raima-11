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

#if !defined(__HAFCNS_H)
#define __HAFCNS_H

#include "rdm.h"

/* HA Status Codes */
#define HA_FAIL            -1000
#define HA_OKAY             0
#define HA_NODB             1000
#define HA_DBMSTOPEN        1001
#define HA_DBMST            1002
#define HA_DBSLV            1003
#define HA_DBSLVOPEN        1004
#define HA_DBSLVUPD         1005
#define HA_DBMSTUPD         1006
#define HA_DBSLVDISCON      1007
#define HA_DBMSTDISCON      1008
#define HA_DBSLVSYNC        1009
#define HA_DBMSTSYNC        1010
#define HA_MSTIASYNC        1011
#define HA_QUEUEOVERFLOW    1012
#define HA_DESTROYDATABASE  1013
#define HA_DBSYNCERROR      1014


#if defined(__cplusplus)
extern "C" {
#endif

typedef uint64_t  HA_USER;

int32_t EXTERNAL_FCN ha_login(const char *, uint16_t, HA_USER *);
int32_t EXTERNAL_FCN ha_logout(HA_USER);
int32_t EXTERNAL_FCN ha_status(HA_USER, const char *, int32_t *);
int32_t EXTERNAL_FCN ha_iasync(HA_USER, const char *, int32_t);

#if defined(__cplusplus)
}
#endif

#endif  /* __HAFCNS_H */

