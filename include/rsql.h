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
/*-----------------------------------------------------------------------
    module      - RDM SQL Direct Interface API header file

    rsql.h      - Native SQL API definitions and prototypes.
-----------------------------------------------------------------------*/

#if !defined(__RSQL_H)
#define __RSQL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "rdm.h"
#include "rsqltypes.h"
#include "sqlcat.h"

/* ======================================================================
    Initialize TFS operation 
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTFSInit(
    const char         *docroot,     /* in:  TFS root database directory */
    const TFS_PARAMS   *tparams)     /* in:  TFS operational parameters */
;
/* ======================================================================
    Initialize TFS operation 
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTFSInitEx(
    const char         *docroot,     /* in:  TFS root database directory */
    const TFS_PARAMS   *tparams,     /* in:  TFS operational parameters */
    TFS_TYPE            tfsType)     /* in:  TFS type */
;
/* ======================================================================
    Terminate TFS operation 
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTFSTerm(void)
;
/* ======================================================================
   Query the TFS version (type)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTFSVersion(
    TFS_TYPE           *tfsType)
;
/* ======================================================================
   Return the auto commit mode
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetAutoCommit(
    const HCONN         hConn,       /* in:  connection handle */
    int16_t            *pAutocommit) /* out: 1 for ON, 0 for OFF */
;
/* ======================================================================
   Set the auto commit mode
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetAutoCommit(
    HCONN               hConn,       /* in:  connection handle */
    int16_t             autocommit)  /* in:  1 for ON, 0 for OFF */
;
/* ======================================================================
    Allocate a connection handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAllocConn(
    HCONN              *pHconn)      /* out: connection handle */
;
/* ======================================================================
    Free connection handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlFreeConn(
    HCONN               hConn)       /* in:  connection handle */
; /*lint -esym(534,rsqlFreeConn) */
/* ======================================================================
    Register C-based User-Defined Functions 
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlRegisterUDFs(
    HCONN               hConn,        /* in:  connection handle */
    uint16_t            noUdfs,       /* in:  number of UDFs */
    const UDFLOADTABLE *udftab,       /* in:  ptr to UDF load table */
    void               *pRegCtx,      /* in:  ptr to user's registration context */
    const size_t        szFcnCtx)     /* in:  size of function context space to be alloc'd */
;
/* ======================================================================
    Register Virtual Table Functions 
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlRegisterVirtualTables(
    HCONN               hConn,        /* in:  connection handle */
    const char         *dbname,       /* in:  name of db */
    uint16_t            novts,        /* in:  # of virtual tables (0 => NULL terminated vtftab) */
    const VTFLOADTABLE *vtftab,       /* in:  ptr to VTF load table */
    void               *pRegCtx,      /* in:  ptr to user's registration context */
    const size_t        szFetchCtx)   /* in:  size of fetch context space to be alloc'd */
;
/* ======================================================================
    Register a pre-compiled SQL stored procedure
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlRegisterProc(
    HCONN               hConn,       /* in:  connection handle */
    const PROC_EXEC          *pProc)       /* in:  ptr to stored procedure pkt */
;
/* ======================================================================
    Open a database on connection 
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlOpenDB(
    HCONN               hConn,      /* in:  connection handle */
    const char         *dbnames,    /* in:  "dbname[;dbname]..." */
    const char         *mode)       /* in:  db open mode ("s"hared, e"x"clusive) */
;
/* ======================================================================
    Open a database catalog
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlOpenCat(
    HCONN              hConn,       /* connection handle */
    const SYSDB       *pCat,        /* pointer to catalog to be opened */
    const char        *tfsloc,      /* dbname/tfs specification (NULL use default) */
    const char        *mode)        /* db open mode ("s"hared, e"x"clusive) */
;
/* ======================================================================
    Close an open database on connection (except syscat)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlCloseDB(
    HCONN               hConn,       /* in:  connection handle */
    const char         *dbname)      /* in:  name of database */
;
/* ======================================================================
    Close all open databases on connection (except syscat)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlCloseDBAll(
    HCONN               hConn)       /* in:  connection handle */
; /*lint -esym(534,rsqlCloseDBAll) */
/* ======================================================================
    Allocate a statement handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAllocStmtEx(
    HCONN               hConn,       /* in:  connection handle */
    int16_t             defer_blobs, /* in:  defer blobs */
    HSTMT              *pHstmt)      /* out: statement handle */
;
/* ======================================================================
    Allocate a statement handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAllocStmt(
    HCONN               hConn,       /* in:  connection handle */
    HSTMT              *pHstmt)      /* out: statement handle */
;
/* ======================================================================
    Close statement handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlCloseStmt(
    HSTMT               hStmt)       /* in:  statement handle */
;
/* ======================================================================
    Free statement handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlFreeStmt(
    HSTMT               hStmt)       /* in:  statement handle */
;
/* ======================================================================
    Compile an SQL Statement
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlPrepare(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *sqlstr)      /* in:  SQL statement to be compiled */
;
/* ======================================================================
    Return the number of parameter markers
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetNumParams(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t           *noPars)      /* out: number of parameter markers */
;
/* ======================================================================
    Return the number of result columns
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetNumResultCols(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t           *pNoCols)     /* out: number of result columns */
;
/* ======================================================================
    Execute a next SELECT Statement in currently executing stored proc.
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlMoreResults(
    HSTMT               hStmt)       /* in:  statement handle */
;
/* ======================================================================
    Execute a compiled SQL Statement
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlExecute(
    HSTMT               hStmt)       /* in:  statement handle */
;
/* ======================================================================
    Retrieve number of rows processed
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetRowCount(
    HSTMT               hStmt,       /* in:  statement handle */
    uint64_t           *pNoRows)     /* out: total number of rows */
;
/* ======================================================================
    Fetch next row of SELECT statement
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlFetch(
    HSTMT               hStmt,       /* in:  statement handle */
    const RSQL_VALUE  **pResult,     /* out: pointer result column values pointer */
    uint16_t           *pNoCols)     /* out: number of result columns */
;
/* ======================================================================
    Get select stmt result column data value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetData(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t            colno,       /* in:  result column number (1..nocols) */
    const RSQL_VALUE  **pResult,     /* out: pointer to result value container pointer */
    uint32_t            lenBlob,     /* in:  max. amount of blob value to be read */
    uint32_t           *pLenBlob)    /* out: actual amount of blob value read */
;
/* ======================================================================
    Get select stmt result column type value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnType(
    HSTMT              hStmt,   /* in:  statement handle */
    uint16_t           colno,   /* in:  result column number (1..nocols) */
    SQL_T             *type)    /* out: column data type */
;
/* ======================================================================
    Get select stmt result column string value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnString(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    char              *data,    /* out:    column string data buffer */
    uint32_t          *len)     /* in/out: length of input buffer and output string */
;
#if !defined(RDM_NO_UNICODE)
/* ======================================================================
    Get select stmt result column Unicode string value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnWString(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    wchar_t           *data,    /* out:    column Unicode string data buffer */
    uint32_t          *len)     /* in/out: length of input buffer and output string (in chars) */
;
#endif
/* ======================================================================
    Get select stmt result column binary value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnBinary(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    uint8_t           *data,    /* out:    column int8 data buffer */
    uint32_t          *len)     /* in/out: length of input buffer and output buffer */
;
/* ======================================================================
    Get select stmt result column int8 value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnInt8(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    int8_t            *data)    /* out:    column int8 data buffer */
;
/* ======================================================================
    Get select stmt result column int16 value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnInt16(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    int16_t           *data)    /* out:    column int16 data buffer */
;
/* ======================================================================
    Get select stmt result column int32 value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnInt32(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    int32_t           *data)    /* out:    column int32 data buffer */
;
/* ======================================================================
    Get select stmt result column int64 value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnInt64(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    int64_t           *data)    /* out:    column int64 data buffer */
;
/* ======================================================================
    Get select stmt result column float value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnFloat(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    float            *data)    /* out:    column float data buffer */
;
/* ======================================================================
    Get select stmt result column double value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnDouble(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    double            *data)    /* out:    column double data buffer */
;
/* ======================================================================
    Get select stmt result column date value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnDate(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    DATE_VAL          *data)    /* out:    column date data buffer */
;
/* ======================================================================
    Get select stmt result column time value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnTime(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    TIME_VAL          *data)    /* out:    column time data buffer */
;
/* ======================================================================
    Get select stmt result column timestamp value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnTimestamp(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    TIMESTAMP_VAL     *data)    /* out:    column timestamp data buffer */
;
/* ======================================================================
    Get select stmt result column BLOB value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnBlob(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    uint32_t           offset,  /* in:     offset in the blob to read */
    void              *data,    /* out:    column timestamp data buffer */
    uint32_t          *len)     /* in/out: buffer size/bytes copied */
;
/* ======================================================================
    Get select stmt result column CLOB value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnClob(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    uint32_t           offset,  /* in:     offset in the blob to read */
    char              *data,    /* out:    column timestamp data buffer */
    uint32_t          *len)     /* in/out: buffer size (chars)/chars copied */
;
/* ======================================================================
    Get select stmt result column BLOB value
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColumnWClob(
    HSTMT              hStmt,   /* in:     statement handle */
    uint16_t           colno,   /* in:     result column number (1..nocols) */
    uint32_t           offset,  /* in:     offset in the blob to read */
    wchar_t           *data,    /* out:    column timestamp data buffer */
    uint32_t          *len)     /* in/out: buffer size (chars)/chars copied */
;
/* ======================================================================
    Update column of current row of cursor
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUpdateCol(
    HSTMT               hStmt,       /* in:  select's statement handle */
    uint16_t            colno,       /* in:  result column number (1..nocols) */
    const RSQL_VALUE   *value)       /* in:  pointer to column value container */
;
/* ======================================================================
    Update current row of cursor
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUpdateRow(
    HSTMT               hStmt)       /* in:  select's statement handle */
;
/* ======================================================================
    Cancel any column updates on current row of cursor
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlCancelRow(
    HSTMT               hStmt)       /* in:  select's statement handle */
;
/* ======================================================================
    Fetch select stmt result column description
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetColDescr(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t            colno,       /* in:  result column number (1..nocols) */
    char               *colname,     /* out: pointer to column name buffer */
    uint16_t            lenColname,  /* in:  size of colname buffer */
    uint16_t           *pLenColname, /* out: actual length of colname */
    SQL_T              *pType,       /* out: result data type */
    int16_t            *pWidth,      /* out: result max. display width */
    int16_t            *pPrec,       /* out: precision */
    int16_t            *pScale,      /* out: scale */
    int16_t            *pNullable)   /* out: nullable? */
;
/* ======================================================================
    Fetch select stmt paramater description
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetParamDescr(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t            parno,       /* in:  parameter number (1..nocols) */
    SQL_T              *pType,       /* out: parameter data type */
    uint32_t           *varlen,      /* out: parameter max. size of var-len data (in bytes) */
    int16_t            *nullable)    /* out: == 1 if nullable */
;
/* ======================================================================
    Fetch parameter name
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetParamName(
    HSTMT              hStmt,      /* in:  statement handle */
    uint16_t           parno,      /* in:  parameter number (1..nocols) */
    char              *name,       /* out: name of the parameter */
    uint16_t           usStrMax,   /* in:  max length in char of name */
    uint16_t          *pusStrLen)  /* out: result length of name (incl null byte) */
;
/* ======================================================================
    Assign ASCII string data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignNullParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno)       /* in:  parameter number (>= 1) */
;
/* ======================================================================
    Assign ASCII string data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignStringParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    const char         *value,       /* in:  parameter value */
    int32_t             len)         /* in:  length of parameter value */
;
/* ======================================================================
    Assign Unicode string data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignWStringParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    const wchar_t      *value,       /* in:  parameter value */
    int32_t             len)         /* in:  length of parameter value */
;
/* ======================================================================
    Assign binary string data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignBinaryParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    const void         *value,       /* in:  parameter value */
    int32_t             len)         /* in:  length of parameter value */
;
/* ======================================================================
    Assign int8_t data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignInt8Param(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    int8_t              value)       /* in:  parameter value */
;
/* ======================================================================
    Assign int16_t data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignInt16Param(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    int16_t             value)       /* in:  parameter value */
;
/* ======================================================================
    Assign int32_t data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignInt32Param(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    int32_t             value)       /* in:  parameter value */
;
/* ======================================================================
    Assign int64_t data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignInt64Param(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    int64_t             value)       /* in:  parameter value */
;
/* ======================================================================
    Assign float data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignFloatParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    float               value)       /* in:  parameter value */
;
/* ======================================================================
    Assign double data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignDoubleParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    double              value)       /* in:  parameter value */
;
/* ======================================================================
    Assign date data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignDateParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    DATE_VAL            value)       /* in:  parameter value */
;
/* ======================================================================
    Assign time data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignTimeParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    TIME_VAL            value)       /* in:  parameter value */
;
/* ======================================================================
    Assign timestamp data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignTimestampParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    TIMESTAMP_VAL       value)       /* in:  parameter value */
;
/* ======================================================================
    Assign CLOB data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignClobParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno)       /* in:  parameter number (>= 1) */
;
/* ======================================================================
    Assign WCLOB data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignWClobParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno)       /* in:  parameter number (>= 1) */
;
/* ======================================================================
    Assign BLOB data to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignBlobParam(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of parameter */
    uint16_t            parno)       /* in:  parameter number (>= 1) */
;
/* ======================================================================
    Bind a value type and data to a named parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAssignParam(
    HSTMT              hStmt,        /* in:  statement handle */
    const char        *name,         /* in:  name of parameter */
    uint16_t           parno,        /* in:  parameter number (>= 1) if name is null */
    SQL_T              type,         /* in:  parameter data type */
    const void        *value,        /* in:  parameter value */
    int32_t            len)          /* in:  length of parameter value */
;
/* ======================================================================
    Bind a value type and location to a parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlBindParam(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t            parno,       /* in:  parameter number (>= 1) */
    SQL_T               type,        /* in:  parameter data type */
    const void         *value,       /* in:  pointer parameter value */
    const int32_t      *pLenValue)   /* in:  ptr to value length variable */
;
/* ======================================================================
    Bind a value type and location to a named parameter marker
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlBindNamedParam(
    HSTMT              hStmt,        /* in:  statement handle */
    const char        *name,         /* in:  name of parameter */
    SQL_T              type,         /* in:  parameter data type */
    const void        *value,        /* in:  ptr to parameter value */
    const int32_t     *pLenValue,    /* in:  ptr to value length variable */
    uint16_t          *pParno)       /* out: ptr to returned param number */
;
/* ======================================================================
    Check for and initialize rsqlPutData for next data-at-exec parameter
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlParamData(
    HSTMT               hStmt,       /* in:  statement handle */
    uint16_t           *parno,       /* in:  parameter number (>= 1) */
    void              **pValPtr)     /* out: parameter value pointer */
;
/* ======================================================================
    Send value for data-at-exec parameter (blobs only)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlPutData(
    HSTMT               hStmt,       /* in:  statement handle */
    const void         *value,       /* in:  ptr to parameter value pointer */
    uint32_t            lenValue)    /* in:  length (in bytes) of value */
;
/* ========================================================================
    Retrieve rsql error message for the specified code
*/
const char *EXTERNAL_FCN rsqlGetErrorMsg(
    RSQL_ERRCODE        stat,        /* in:  RSQL error code */
    const char        **pErrmsg)     /* out: error message for the code */
;
/* ========================================================================
    Retrieve rsql error message
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetErrorInfo(
    HRSQL               hndl,        /* in:  connection or statement handle */
    char               *msgbuf,      /* out: error message buffer */
    int16_t             lenMsgbuf)   /* in:  length of message buffer */
;
/* ======================================================================
    Return pointer to catalog for specified open database
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetCatalog(
    HCONN               hConn,       /* in:  connection handle */
    const char         *dbname,      /* in:  name of database */
    const SYSDB       **pCatPtr)     /* out: returned ptr to catalog */
;
/* ======================================================================
   Directly execute a transaction statement
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransact(
    HCONN               hConn,       /* in:  connection handle */
    STMT_TYPE           stype,       /* in:  sqlSTART, sqlCOMMIT, or sqlROLLBACK */
    int16_t             rdonly,      /* in:  read only transaction flag */
    const char         *txid)        /* in:  transaction id */
;
/* ======================================================================
   Start a read-only transaction
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransStartReadOnly(
    HCONN               hConn)       /* in:  connection handle */
;
/* ======================================================================
   End a read-only transaction
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransEndReadOnly(
    HCONN               hConn)       /* in:  connection handle */
;
/* ======================================================================
   Start a transaction
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransStart(
    HCONN               hConn,       /* in:  connection handle */
    const char         *txid)        /* in:  transaction id */
;
/* ======================================================================
   Issue a transaction savepoint
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransSavepoint(
    HCONN               hConn,       /* in:  connection handle */
    const char         *txid)        /* in:  transaction id */
;
/* ======================================================================
   Release (delete) a transaction savepoint
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransRelease(
    HCONN               hConn,       /* in:  connection handle */
    const char         *txid)        /* in:  transaction id */
;
/* ======================================================================
   Commit a transaction
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransCommit(
    HCONN               hConn)       /* in:  connection handle */
;
/* ======================================================================
   Rollback a transaction
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransRollback(
    HCONN               hConn,       /* in:  connection handle */
    const char         *txid)        /* in:  transaction id */
;
/* ========================================================================
   Return the status of an ongoing transaction
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlTransStatus(
    HCONN               hConn,       /* in:  connection handle */
    TRANS_STAT         *pTractive)   /* out: transaction status */
;
/* ========================================================================
   Request locks on the specified list of tables
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlLockTables(
    HCONN               hConn,       /* in:  connection handle */
    const char         *dbname,      /* in:  name of db containing tables */
    int16_t             noTablocks,  /* in:  # of tables in tablock array */
    const TABLE_LOCK   *tablocks)    /* in:  pointer to table lock request array */
;
/* ========================================================================
   Unlock a read locked table
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUnlockTable(
    HCONN               hConn,       /* in:  connection handle */
    const char         *dbname,      /* in:  name of db containing table */
    const char         *tabname)     /* in:  name of table to be unlocked */
;
/* ========================================================================
   Unlock all locked tables
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUnlockTableAll(
    HCONN               hConn)       /* in:  connection handle */
;
/* ========================================================================
   Get lock wait timeout in seconds for connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetTimeout(
    HCONN           hConn,        /* in:  connection handle */
    int32_t        *pSecs)        /* out: ptr to timeout values in seconds */
;
/* ========================================================================
   Set lock wait timeout in seconds for connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetTimeout(
    HCONN               hConn,       /* in:  connection handle */
    int32_t             secs)        /* in:  timeout values in seconds */
;
/* ========================================================================
   Return the ERRSTAT pointer to the outside world
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetErrorStat(
    const HSTMT         hStmt,       /* in:  statement handle */
    const ERRSTAT     **pErrstat)    /* out: error status buffer */
;
/* ========================================================================
   Get the RDM task handle associated with a connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetDBTask(
    HCONN               hConn,       /* in:  connection handle */
    DB_TASK           **pTask)       /* out: ptr to task handle */
;
/* ======================================================================
   Return name of table containing ref'd SELECT result column
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetTableName(
    HSTMT               hStmt,       /* in:  statment handle */
    uint16_t            colno,       /* in:  column number (1..# result cols) */
    char               *dbname,      /* out: name of database */
    uint16_t            lenDbname,   /* in:  size of dbname array */
    uint16_t           *pLenDbname,  /* out: actual length of dbname (incl null byte) */
    char               *tabname,     /* out: name of table */
    uint16_t            lenTabname,  /* in:  size of tabname array */
    uint16_t           *pLenTabname) /* out: actual length of tabname (incl null byte) */
;
/* ======================================================================
    Directly execute a SQL Statement
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlExecDirect(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *sqlstr)      /* in:  SQL statement to be compiled */
;
/* ======================================================================
    Directly execute a stored procedure
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlExecProc(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *name,        /* in:  name of stored procedure */
    uint16_t            noArgs,      /* in:  number of arguments (size of args array) */    
    const RSQL_VALUE   *args)        /* in:  proc arg values array */
;
/* ======================================================================
    Obtain the date format code for the connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetDateFormat(
    HCONN               hconn,       /* in:  connection handle */
    RSQL_DATE_FORMAT   *pDatefmt)    /* out: date format (MMMMDDYY, etc) */
;
/* ======================================================================
    Obtain the date separator character for the connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetDateSeparator(
    HCONN               hconn,       /* in:  connection handle */
    char               *pDatesep)    /* out: date separator character */
;
/* ======================================================================
    Set the date format code for the connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetDateFormat(
    HCONN               hconn,       /* in:  connection handle */
    RSQL_DATE_FORMAT    datefmt)     /* in:  date format (MMMMDDYY, etc) */
;
/* ======================================================================
    Set the date separator character for the connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetDateSeparator(
    HCONN               hconn,       /* in:  connection handle */
    char                datesep)     /* in:  date separator */
;
/* ======================================================================
   Obtain the name of the cursor
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetCursorName(
    HSTMT               hStmt,       /* in:  statement handle */
    char               *cursor,      /* out: name of the cursor */
    uint16_t            lenCursor,   /* in:  max length curname array (incl null byte) */
    uint16_t           *pLenCursor)  /* out: actual length of curname (incl null byte) */
;
/* ======================================================================
   Set the name of the cursor
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetCursorName(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *cursor)      /* out: name of the cursor */
;
/* ======================================================================
    Get the current read only transaction mode setting
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetReadOnlyTrmode(
    HCONN               hConn,       /* in:  connection handle */
    int16_t            *pRomode)     /* out: 0 == manual, 1 = auto */
;
/* ======================================================================
    Set the current read only transaction mode setting
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetReadOnlyTrmode(
    HCONN               hConn,       /* in:  connection handle */
    int16_t             romode)      /* in: 0 == manual, 1 = auto */
;
/* ======================================================================
    Get the deferred blob reading mode
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetDeferBlobMode(
    HSTMT              hStmt,        /* in:  statement handle */
    int16_t           *pMode)        /* out: 0 == normal, 1 = deferred */
;
/* ======================================================================
    Set the deferred blob reading mode
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetDeferBlobMode(
    HSTMT               hStmt,       /* in:  statement handle */
    int16_t             mode)        /* in: 0 == normal, 1 = deferred */
;
/* ======================================================================
    Initialize a database
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlInitDB(
    HCONN               hConn,       /* in:  connection handle */
    const char         *dbname)      /* in:  name of db to be initialized */
;
/* ======================================================================
    Drop a database
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlDropDB(
    HCONN       hConn,               /* in:  connection handle */
    const char *dbname)              /* in:  "dbname[@host[:port]]"  */
;
/* ======================================================================
    Set amount of memory to be reserved per connection
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetDBNames(
    const HCONN         hConn,       /* in:  connection handle */
    char               *dbnames,     /* out: list of db names */
    uint16_t            lenDbnames,  /* in:  max size of dbnames (incl null byte) */
    uint16_t           *pLenDbnames) /* out: actual size of dbnames list (incl null byte) */
;
/* ======================================================================
    Return the statement type
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetStmtType(
    HSTMT               hStmt,       /* in:  statement handle */
    STMT_TYPE          *pStype)      /* out: result statement type */
;
/* ======================================================================
    Return the statement state
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetStmtState(
    HSTMT       hStmt,          /* in:  statement handle */
    STMT_STATE *pState)         /* out: result statement type */
;
/* ======================================================================
    Return the select statement type
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetSelectType(
    HSTMT               hStmt,       /* in:  statement handle */
    SELECT_TYPE        *pSeltype)    /* out: result select statement type */
;
/* ======================================================================
    Register an error-handling callback function
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetErrorCallback(
    HCONN               hConn,       /* in:  connection handle */
    RSQL_ERRHANDLER     errfcn,      /* in:  user's error handler function */
    void               *data)        /* in:  ptr to user data to be passed to handler */
;
/* ======================================================================
    Return the connection handle associated with the specified stmt handle
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetConnHandle(
    HSTMT      hStmt,                /* in:  statement handle */
    HCONN     *pHConn)               /* out: ptr to connection handle */
;
/* ======================================================================
    Obtain the current "generate C files" option
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetGenCFiles(
    HCONN              hConn,       /* in:  connection handle */
    int16_t           *psGenCFiles) /* out: 1 to generate C files */
;
/* ======================================================================
    Set/clear the "generate C files" option
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetGenCFiles(
    HCONN              hConn,       /* in:  connection handle */
    int16_t            sGenCFiles)  /* in:  1 to generate C files */
;
/* ======================================================================
    Set/clear the "generate files" option
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetGenFiles(
    HCONN              hConn,       /* in:  connection handle */
    uint16_t           usGenFlags)  /* in:  bitmap flag for file gen */
;
/* ======================================================================
    Return a copy of the SQL statement string associated with hstmt
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlGetStmtString(
    HSTMT              hStmt,      /* in:  statement handle */
    char              *pStmtbuf,   /* out: ptr to statement string buffer */
    uint16_t           buflen,     /* in:  length of statement string buffer */
    uint16_t          *pStrlen)    /* out: length of statement string */
;
/* ======================================================================
    Pack a CAL_DATE into a binary DATE_VAL
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlPackDate(
    const CAL_DATE *pUDt,         /* in:  ptr to unpacked date value */
    DATE_VAL       *pPDt)         /* out: ptr to packed date value */
;
/* ======================================================================
    Unpack a binary DATE_VAL into a CAL_DATE
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUnpackDate(
    const DATE_VAL *pPDt,         /* in:  ptr to packed date value */
    CAL_DATE       *pUDt)         /* out: ptr to unpacked date value */
;
/* ======================================================================
    Pack a CAL_TIME into a binary TIME_VAL
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlPackTime(
    const CAL_TIME *pUTm,         /* in:  ptr to unpacked time value */
    TIME_VAL       *pPTm)         /* out: ptr to packed time value */
;
/* ======================================================================
    Pack a CAL_DATE into a binary DATE_VAL
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUnpackTime(
    const TIME_VAL       *pPTm,   /* in:  ptr to packed time value */
    CAL_TIME             *pUTm)   /* out: ptr to unpacked time value */
;
/* ======================================================================
    Pack a CAL_TIMESTAMP into a binary TIMESTAMP_VAL
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlPackTimestamp(
    const CAL_TIMESTAMP  *pUTs,   /* in:  ptr to unpacked timestamp value */
    TIMESTAMP_VAL        *pPTs)   /* out: ptr to packed timestamp value */
;
/* ======================================================================
    Pack a CAL_DATE into a binary DATE_VAL
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlUnpackTimestamp(
    const TIMESTAMP_VAL  *pPTs,   /* in:  ptr to packed timestamp value */
    CAL_TIMESTAMP        *pUTs)   /* out: ptr to unpacked timestamp value */
;
/* ======================================================================
    Show a query's execution plan as a result set
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlShowPlan(
    HSTMT              hPlan,      /* in:  handle of statement for the plan */
    HSTMT              hQuery)     /* in:  handle of query's statement */
;

#if defined(__cplusplus)
}
#endif

#endif  /* __RSQL_H */

