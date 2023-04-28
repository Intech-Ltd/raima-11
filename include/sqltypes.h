/***************************************************************************
 *                                                                         *
 * Raima Database Manager                                                  *
 *                                                                         *
 * Copyright (c) 2010 Raima Inc. All rights reserved.                      *
 *                                                                         *
 * Use of this software, whether in source code format, or in executable,  *
 * binary object code form, is governed by the Raima Inc. LICENSE          *
 * which is fully described in the LICENSE.TXT file, included within this  *
 * distribution of files.                                                  * 
 *                                                                         *
 **************************************************************************/

#if !defined(__SQLTYPES_H)
#define __SQLTYPES_H

#if !defined(ODBCVER)
#define ODBCVER	0x0351
#endif

#include "rdm.h"

#if defined(RDM_WINDOWS) && !defined(_WINDOWS_)
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HINSTANCE);
#define _WINDOWS_
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#define SQL_API     EXTERNAL_FCN
#define SQL_API_PTR EXTERNAL_FCN_PTR

#if !defined(BOOL)
#define BOOL        int32_t
#endif  /* BOOL */

typedef signed char SCHAR;
typedef SCHAR       SQLSCHAR;
typedef uint8_t     SQLCHAR;
typedef int16_t     SQLSMALLINT;
typedef uint16_t    SQLUSMALLINT;
typedef int32_t     SQLINTEGER;
typedef uint32_t    SQLUINTEGER;
typedef double      SQLDOUBLE;
typedef double      SQLFLOAT;
typedef uint8_t     SQLNUMERIC;
typedef float       SQLREAL;
typedef uint8_t     SQLTIME;
typedef uint8_t     SQLTIMESTAMP;
typedef uint8_t     SQLVARCHAR;
typedef void *      SQLPOINTER;
typedef int16_t     SQLRETURN;

#if !defined(RDM_NO_UNICODE)
typedef wchar_t     SQLWCHAR;
#endif

#if defined(RDM_64BIT)
typedef int64_t         SQLLEN;
typedef uint64_t        SQLULEN;
typedef uint64_t        SQLSETPOSIROW;
#else
#define SQLLEN          SQLINTEGER
#define SQLULEN         SQLUINTEGER
#define SQLSETPOSIROW   SQLUSMALLINT
#endif

typedef SQLULEN     BOOKMARK;

typedef const void *      SQLHANDLE;
typedef const void *      SQLHENV;
typedef const void *      SQLHDBC;
typedef const void *      SQLHSTMT;
typedef const void *      SQLHDESC;

/* define our own HWND if windows.h is not included already */
#if !defined(_WINDOWS_) && !defined(UNDER_CE)
typedef const void *      HWND;
#endif

typedef HWND        SQLHWND;

/* ----------------------------------------------------------------------------
   standard structs for working with date/time
*/
#if !defined(__SQLDATE)
#define	__SQLDATE

typedef struct tagDATE_STRUCT {
    SQLSMALLINT     year; 
    SQLUSMALLINT    month; 
    SQLUSMALLINT    day; 
} DATE_STRUCT;

typedef DATE_STRUCT SQL_DATE_STRUCT;

typedef struct tagTIME_STRUCT {
     SQLUSMALLINT   hour; 
     SQLUSMALLINT   minute; 
     SQLUSMALLINT   second; 
} TIME_STRUCT;

typedef TIME_STRUCT SQL_TIME_STRUCT;

typedef struct tagTIMESTAMP_STRUCT {
     SQLSMALLINT    year; 
     SQLUSMALLINT   month; 
     SQLUSMALLINT   day; 
     SQLUSMALLINT   hour; 
     SQLUSMALLINT   minute; 
     SQLUSMALLINT   second; 
     SQLUINTEGER    fraction;
} TIMESTAMP_STRUCT;

typedef TIMESTAMP_STRUCT SQL_TIMESTAMP_STRUCT;

typedef enum {
    SQL_IS_YEAR             = 1,
    SQL_IS_MONTH            = 2,
    SQL_IS_DAY              = 3,
    SQL_IS_HOUR             = 4,
    SQL_IS_MINUTE           = 5,
    SQL_IS_SECOND           = 6,
    SQL_IS_YEAR_TO_MONTH    = 7,
    SQL_IS_DAY_TO_HOUR      = 8,
    SQL_IS_DAY_TO_MINUTE    = 9,
    SQL_IS_DAY_TO_SECOND    = 10,
    SQL_IS_HOUR_TO_MINUTE   = 11,
    SQL_IS_HOUR_TO_SECOND   = 12,
    SQL_IS_MINUTE_TO_SECOND = 13
} SQLINTERVAL;

typedef struct tagSQL_YEAR_MONTH {
    SQLUINTEGER     year;
    SQLUINTEGER	    month;
} SQL_YEAR_MONTH_STRUCT;

typedef struct tagSQL_DAY_SECOND {
    SQLUINTEGER     day;
    SQLUINTEGER     hour;
    SQLUINTEGER     minute;
    SQLUINTEGER     second;
    SQLUINTEGER     fraction;
} SQL_DAY_SECOND_STRUCT;

typedef struct tagSQL_INTERVAL_STRUCT {
    SQLINTERVAL     interval_type;
    SQLSMALLINT     interval_sign;
    union {
        SQL_YEAR_MONTH_STRUCT year_month;
        SQL_DAY_SECOND_STRUCT day_second;
    } intval;
} SQL_INTERVAL_STRUCT;

#endif  /* __SQLDATE */

/* ----------------------------------------------------------------------------
   internal representation of numeric data types
*/
#define SQL_MAX_NUMERIC_LEN             16
typedef struct tagSQL_NUMERIC_STRUCT {
    SQLCHAR     precision;
    SQLSCHAR    scale;
    SQLCHAR     sign;       /* 1 if positive, 0 if negative */
    SQLCHAR     val[SQL_MAX_NUMERIC_LEN];
} SQL_NUMERIC_STRUCT;

/* ----------------------------------------------------------------------------
   BIGINT definitions
*/
#define ODBCINT64       int64_t
#define ODBCUINT64      uint64_t 
typedef ODBCINT64       SQLBIGINT;
typedef ODBCUINT64      SQLUBIGINT;

/* ----------------------------------------------------------------------------
   GUID definition
*/
#if defined(GUID)
typedef GUID    SQLGUID
#else
typedef struct tagSQLGUID {
    uint32_t    Data1;
    uint16_t    Data2;
    uint16_t    Data3;
    uint8_t     Data4[8];
} SQLGUID;
#endif  /* GUID */

#if defined(__cplusplus)
}
#endif

#endif  /* __SQLTYPES_H */

