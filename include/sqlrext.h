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

#if !defined(__SQLREXT_H)
#define __SQLREXT_H

#include "sqlext.h"
#include "sqlrtypes.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* ------------------------------------------------------------------------
   Values referenced in SQLGetInfo()
*/
#define RDM_DRIVER_VER          RDM_ODBC_DRIVER_VERSION
#define RDM_SOURCE_VER          RDM_ODBC_DRIVER_VERSION
#define RDM_MAX_STATEMENT_LEN   4096    /* defSqlMaxstmtlen */
#define RDM_MAX_USER_NAME_LEN   32
#define RDM_INFO_LAST           SQL_CONVERT_GUID    /* sqlext.h */
#define RDM_XOPEN_INFO_FIRST    SQL_XOPEN_CLI_YEAR
#if (ODBCVER >= 0x0380)
#define RDM_XOPEN_INFO_LAST     SQL_ASYNC_DBC_FUNCTIONS
#else
#define RDM_XOPEN_INFO_LAST     SQL_MAX_ASYNC_CONCURRENT_STATEMENTS
#endif

/* ------------------------------------------------------------------------
*/
#define SQL_RDM_GENFILES_CC     0x0001
#define SQL_RDM_GENFILES_CXX    0x0002
#define SQL_RDM_GENFILES_OBJC   0x0004

/* ------------------------------------------------------------------------
   Extended access modes for SQL_ATTR_ACCESS_MODE (connection attribute)
*/
#define SQL_MODE_EXCLUSIVE  2

/* ------------------------------------------------------------------------
   RDM specific transaction options for SQLExtendedTran()
*/
#define SQL_START           2
#define SQL_SAVEPOINT       3
#define SQL_RELEASE         4

/* ------------------------------------------------------------------------
   RDM specific transaction statuses for SQLTransactStatus(0
*/
#define SQL_TXN_INACTIVE    0
#define SQL_TXN_DDL         1
#define SQL_TXN_REGULAR     2
#define SQL_TXN_READONLY    3

/* ------------------------------------------------------------------------
   RDM specific connection type values
*/
#define SQL_CT_LOCAL        0
#define SQL_CT_REMOTE       1
#define SQL_CT_UNKNOWN      2

/* ------------------------------------------------------------------------
   RDM specific TFS type values
*/
#define SQL_TFSTYPE_TFS         0   /* TFST: built-in tfs */
#define SQL_TFSTYPE_RPC         1   /* TFSR: remove client */
#define SQL_TFSTYPE_STANDALONE  2   /* TFSS: standalone tfs */
#define SQL_TFSTYPE_DEFAULT     3   /* default configuration - tfst */

/* ------------------------------------------------------------------------
   RDM specific connection attributes
*/
#define SQL_ATTR_RDM_CONN_TYPE          401 /* local or remote connection */
#define SQL_ATTR_RDM_DBCAT              402 /* specify the catalog pointer */
#define SQL_ATTR_RDM_DBCAT_COUNT        403
#define SQL_ATTR_RDM_GENCFILES          404
#define SQL_ATTR_RDM_REMOTE_NAME        405
#define SQL_ATTR_RDM_REMOTE_PORT        406
#define SQL_ATTR_RDM_RESERVE_BYTES      407
#define SQL_ATTR_RDM_TFS                408
#define SQL_ATTR_RDM_TFS_NAME           409
#define SQL_ATTR_RDM_TFS_PORT           410
#define SQL_ATTR_RDM_USE_CATALOG        411
#define SQL_ATTR_RDM_TFSINIT_DISKLESS   413 /* TFS no-disk (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_DOCROOT    414 /* TFS DOCROOT (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_LOGFILE    415 /* TFS logfile (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_PORT       416 /* TFS Port Number (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_READONLY   417 /* TFS Read-Only (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_STDOUT     418 /* TFS stdout (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_VERBOSE    419 /* TFS verbose (only for TFST) */
#define SQL_ATTR_RDM_TFSINIT_TYPE       420 /* TFS type */

#if defined(RDBC_USE_INTERNAL)
#define SQL_ATTR_RDMINT_OVERWRITE       901
#define SQL_ATTR_RDMINT_GENFILES        902 
#endif

/* ------------------------------------------------------------------------
   RDM specific statement attributes
*/

#define SQL_ATTR_RDM_DEFER_BLOBS   2001 /* the blob reading mode */

/* ------------------------------------------------------------------------
   RDM specific TFS port number definition
*/
#define SQL_RDM_DEFAULT_PORT    TFS_DEFAULT_PORT

/* ------------------------------------------------------------------------
   RDM specific TFS stdout definition
*/
#define SQL_RDM_DEFAULT_STDOUT  ""

/* ------------------------------------------------------------------------
   RDM specific dynamic function code
*/
#define SQL_DIAG_RDM_OPEN_DB            101
#define SQL_DIAG_RDM_OPEN_DBUNION       102
#define SQL_DIAG_RDM_CLOSE_DB           103
#define SQL_DIAG_RDM_SELECT             104
#define SQL_DIAG_RDM_START              105
#define SQL_DIAG_RDM_COMMIT             106
#define SQL_DIAG_RDM_SAVEPOINT          107
#define SQL_DIAG_RDM_RELEASE            108
#define SQL_DIAG_RDM_ROLLBACK           109
#define SQL_DIAG_RDM_CREATE_PROCEDURE   110
#define SQL_DIAG_RDM_DROP_PROCEDURE     111
#define SQL_DIAG_RDM_EXECUTE_PROCEDURE  112
#define SQL_DIAG_RDM_SET                113
#define SQL_DIAG_RDM_SET_COLUMN         114
#define SQL_DIAG_RDM_LOCK               115
#define SQL_DIAG_RDM_UNLOCK             116
#define SQL_DIAG_RDM_INITIALIZE_SCHEMA  117
#define SQL_DIAG_RDM_CREATE_CATALOG     118
#define SQL_DIAG_RDM_IMPORT_TABLE       119
#define SQL_DIAG_RDM_EXPORT_TABLE       120

/* ------------------------------------------------------------------------
   RDM specific statement type defines for SQLDescribeStmt
*/
#define SQL_RDM_STMT_UNKNOWN    SQL_DIAG_UNKNOWN_STATEMENT
#define SQL_RDM_STMT_OPEN       SQL_DIAG_RDM_OPEN_DB
#define SQL_RDM_STMT_DBUNION    SQL_DIAG_RDM_OPEN_DBUNION
#define SQL_RDM_STMT_CLOSE      SQL_DIAG_RDM_CLOSE_DB 
#define SQL_RDM_STMT_SELECT     SQL_DIAG_RDM_SELECT
#define SQL_RDM_STMT_INSERT     SQL_DIAG_INSERT
#define SQL_RDM_STMT_UPDATE     SQL_DIAG_UPDATE_WHERE
#define SQL_RDM_STMT_DELETE     SQL_DIAG_DELETE_WHERE 
#define SQL_RDM_STMT_START      SQL_DIAG_RDM_START
#define SQL_RDM_STMT_COMMIT     SQL_DIAG_RDM_COMMIT
#define SQL_RDM_STMT_SAVEPOINT  SQL_DIAG_RDM_SAVEPOINT
#define SQL_RDM_STMT_RELEASE    SQL_DIAG_RDM_RELEASE
#define SQL_RDM_STMT_ROLLBACK   SQL_DIAG_RDM_ROLLBACK
#define SQL_RDM_STMT_CRPROC     SQL_DIAG_RDM_CREATE_PROCEDURE
#define SQL_RDM_STMT_DRPROC     SQL_DIAG_RDM_DROP_PROCEDURE
#define SQL_RDM_STMT_EXECUTE    SQL_DIAG_RDM_EXECUTE_PROCEDURE
#define SQL_RDM_STMT_SET        SQL_DIAG_RDM_SET 
#define SQL_RDM_STMT_SETCOLUMN  SQL_DIAG_RDM_SET_COLUMN
#define SQL_RDM_STMT_LOCK       SQL_DIAG_RDM_LOCK
#define SQL_RDM_STMT_UNLOCK     SQL_DIAG_RDM_UNLOCK
#define SQL_RDM_STMT_INITDB     SQL_DIAG_RDM_INITIALIZE_SCHEMA
#define SQL_RDM_STMT_CRCAT      SQL_DIAG_RDM_CREATE_CATALOG
#define SQL_RDM_STMT_IMPORT     SQL_DIAG_RDM_IMPORT_TABLE
#define SQL_RDM_STMT_EXPORT     SQL_DIAG_RDM_EXPORT_TABLE
#define SQL_RDM_STMT_DDL        SQL_DIAG_CREATE_SCHEMA

/* ------------------------------------------------------------------------
   RDM specific ODBC extension APIs
*/
SQLRETURN SQL_API SQLDescribeStmt(
    SQLHSTMT         StatementHandle,   /* IN:  ODBC statement handle */
    SQLUSMALLINT    *pStmtType)         /* OUT: statement type */
;

SQLRETURN SQL_API SQLExtendedTran(
    SQLSMALLINT      HandleType,        /* IN:  handle type */
    SQLHANDLE        Handle,            /* IN:  ODBC handle */
    SQLSMALLINT      OperationType,     /* IN:  transaction type indicator */
    SQLSMALLINT      ReadOnly,          /* IN:  SQL_TRUE for read-only trans */
    const SQLCHAR   *TransactionID,     /* IN:  transaction ID (can be NULL) */
    SQLSMALLINT      StringLengthPtr)   /* IN:  length of transaction ID */
;

#if !defined(RDM_NO_UNICODE)
SQLRETURN SQL_API SQLExtendedTranW(
    SQLSMALLINT      HandleType,        /* IN:  handle type */
    SQLHANDLE        Handle,            /* IN:  ODBC handle */
    SQLSMALLINT      OperationType,     /* IN:  transaction type indicator */
    SQLSMALLINT      ReadOnly,          /* IN:  SQL_TRUE for read-only trans */
    const SQLWCHAR  *TransactionID,     /* IN:  transaction ID (can be NULL) */
    SQLSMALLINT      StringLengthPtr)   /* IN:  length of transaction ID */
;
#endif

SQLRETURN SQL_API SQLSetError(
    SQLSMALLINT      HandleType,        /* IN:  handle type */
    SQLHANDLE        Handle,            /* IN:  ODBC handle */
    SQLRETURN        ErrorCode,         /* IN:  ODBC stat code */
    ECALLBACK        ErrorHandler)      /* IN:  ptr to the user error function */
;
/* ------------------------------------------------------------------------
   Return the command execution plan chosen by the query optimizer
*/
SQLRETURN SQL_API SQLShowPlan(
    SQLHSTMT         StatementHandle,   /* IN:  ODBC statement handle for plan */
    SQLHSTMT         QueryHandle)       /* IN:  ODBC statement handle for query */
;
SQLRETURN SQL_API SQLTransactStatus(
    SQLSMALLINT      HandleType,        /* IN:  handle type */
    SQLHANDLE        Handle,            /* IN:  ODBC handle */
    SQLSMALLINT     *pActive)           /* OUT: TRUE if tranaction is active */
;

#define	SQLExtendedTranA    SQLExtendedTran

#if defined(__cplusplus)
}
#endif

#endif  /* __SQLREXT_H */

