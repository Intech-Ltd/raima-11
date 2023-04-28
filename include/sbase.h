/***************************************************************************
 * Raima Database Manager                                                  *
 *                                                                         *
 * Copyright (c) 2011 Raima Inc. All rights reserved.                      *
 *                                                                         *
 * Use of this software, whether in source code format, or in executable,  *
 * binary object code form, is governed by the Raima Inc. LICENSE          *
 * which is fully described in the LICENSE.TXT file, included within this  *
 * distribution of files.                                                  * 
 **************************************************************************/

#if !defined(__SBASE_H)
#define __SBASE_H

#include "rsqltypes.h"

#define CNV_INVCODE 0 

/* -----------------------------------------------------------------------
   RSQL type definitions
*/
typedef enum tagRSQL_TYPE {
    RSQL_CHAR = 0,
    RSQL_WCHAR,
    RSQL_BIT,           /* tBOOL */
    RSQL_INT8,       
    RSQL_UINT8,       
    RSQL_INT16,
    RSQL_UINT16,
    RSQL_INT32,
    RSQL_UINT32,
    RSQL_INT64,
    RSQL_UINT64,
    RSQL_FLOAT,
    RSQL_DOUBLE,
    RSQL_DECIMAL,
    RSQL_DATE,
    RSQL_TIME,
    RSQL_TIMESTAMP,
    RSQL_BINARY,
#if defined(NOT_USED)
    RSQL_ROWID,
#endif
    RSQL_TYPE_MAX,
    RSQL_NULL           /* must be placed after RSQL_TYPE_MAX */
} RSQL_TYPE;

/* -----------------------------------------------------------------------
   conversion support functions
*/
double EXTERNAL_FCN sb_ascToDouble(const char *);
#if !defined(RDM_NO_UNICODE)
double EXTERNAL_FCN sb_wcsToDouble(const wchar_t *);
#endif

/* -----------------------------------------------------------------------
   Obtain the proper conversion code for the data types
*/
uint8_t EXTERNAL_FCN sb_getConvCode(
    SQL_T              tDst,        /* IN:  destination type */
    SQL_T              tSrc)        /* IN:  source type */

;
/* -----------------------------------------------------------------------
   Convert SQL_T data (designed for RDBC)
*/
RSQL_ERRCODE EXTERNAL_FCN sb_rdbcConvert(
    const RSQL_VALUE  *pSrcVal,     /* IN:  source VALUE struct */
    SQL_T              sDstType,    /* IN:  destination data type */
    void              *pvDstValue,  /* OUT: destination value pointer */
    uint32_t           uDstMaxLen,  /* IN:  max length of dest. data */
    uint32_t          *puDstOutLen,
    int32_t           *pnOffset,    /* IN/OUT: offset for char/bin type */
    RSQL_DATE_FORMAT   date_format, /* IN:  date format */
    char               date_sep,    /* IN:  date separator */
    PSP_MEMTAG         tag)         /* IN:  memory tag */
;
/* -----------------------------------------------------------------------
   Convert an embedded ODBC escapes to RDM SQL
*/
RSQL_ERRCODE EXTERNAL_FCN sb_rdbcNative(
    const char        *pszIn,
    char              *pszOut,
    size_t             tOutLen,
    size_t            *ptResLen)
;
/* -----------------------------------------------------------------------
   Convert SQL_T data 
*/
RSQL_ERRCODE EXTERNAL_FCN sb_rsqlConvert(
    RSQL_VALUE        *pDstVal,     /* OUT: destination VALUE struct */
    const RSQL_VALUE  *pSrcVal,     /* IN:  source VALUE struct */
    SQL_T              sDstType,    /* IN:  destination data type */
    uint32_t           usDstMaxLen, /* IN:  max length of dest. data */
    RSQL_DATE_FORMAT   date_format, /* IN:  date format */
    char               date_sep,    /* IN:  date separator */
    PSP_MEMTAG         tag)         /* IN:  memory tag */
;
/* -----------------------------------------------------------------------
    Return the SQL_T type length
*/
uint16_t EXTERNAL_FCN sb_rsqlTypeLength(
    SQL_T              type)        /* IN: SQL_T type */
;
/* -----------------------------------------------------------------------
    Check date value
*/
RSQL_ERRCODE EXTERNAL_FCN sb_checkDate(
    const CAL_DATE    *dp)          /* IN:  date value */
;
/* -----------------------------------------------------------------------
    Check time value
*/
RSQL_ERRCODE EXTERNAL_FCN sb_checkTime(
    const CAL_TIME    *tp)          /* IN:  time value */
;

#endif  /* __SBASE_H */

