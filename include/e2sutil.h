/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2001-2008 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#ifndef E2SUTIL_H
#define E2SUTIL_H

#include "rdm.h"
#include "rdmsql.h"
#include "psp.h"

#if defined(__cplusplus)
extern "C" {
#endif


typedef int32_t (*FKCALLBACKFCN)(void *, uint32_t, uint32_t);
typedef int32_t (*RFCALLBACKFCN)(const DB_TASK *, void *, uint32_t, uint32_t, const char *, uint32_t, uint32_t);


int32_t EXTERNAL_FCN e2s_isSysnullField(DB_TASK *, const SYSDB *, uint32_t);
int32_t EXTERNAL_FCN e2s_openDatabase(DB_TASK *, const char *, int32_t *);
RSQL_ERRCODE EXTERNAL_FCN e2s_openSQLDatabase(DB_TASK **, const char *, HCONN *, int32_t *);
void    EXTERNAL_FCN e2s_closeDatabase(DB_TASK *);
void    EXTERNAL_FCN e2s_closeSQLDatabase(HCONN);
const char *EXTERNAL_FCN e2s_createRecordName(const DB_TASK *, uint32_t);
const char *EXTERNAL_FCN e2s_createFieldName(const char *, const char *, uint32_t, uint32_t);
const char *EXTERNAL_FCN e2s_createKeyName(const DB_TASK *, const char *, uint32_t);
const char *EXTERNAL_FCN e2s_createInstanceName(const char *, uint32_t);
const char *EXTERNAL_FCN e2s_createAddrName(const char *, uint32_t);
int32_t EXTERNAL_FCN e2s_iterateForeignKeys(const DB_TASK *, uint32_t, uint32_t *, FKCALLBACKFCN, void *);
int32_t EXTERNAL_FCN e2s_iterateRecFields(DB_TASK *, uint32_t, uint32_t *, const SYSDB *, RFCALLBACKFCN, void *);
int8_t EXTERNAL_FCN e2s_isFieldArray(const DB_TASK *, uint32_t, uint32_t *, uint32_t *);
uint32_t EXTERNAL_FCN e2s_getFKFieldCount(const DB_TASK *, uint32_t);
uint32_t EXTERNAL_FCN e2s_getRecFieldCount(DB_TASK *, const SYSDB *, uint32_t);
int32_t  EXTERNAL_FCN e2s_isNameValid(const char *);
void     EXTERNAL_FCN e2s_initInfo(const DB_TASK *, int32_t *);
int32_t EXTERNAL_FCN  e2s_buildOrderedRecordList(const DB_TASK *, uint32_t *, uint32_t *);



#if defined(__cplusplus)
}
#endif


#endif


