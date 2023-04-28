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

    rdmsql.h    - Native SQL API definitions and prototypes.
-----------------------------------------------------------------------*/

#if !defined(__INTERNAL_RDMSQL_H)
#define __INTERNAL_RDMSQL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "rdmsql.h"

/************************************************************************
    All of the following functions are for internal use only
************************************************************************/

/* ======================================================================
    Connect to RDM SQL
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlConnect(
    HCONN               hConn,       /* in:  connection handle */
    const char         *tfsloc)      /* in:  TFS location string: 
                                            "@hostname:port" (NULL => use default) */
;
/* ======================================================================
    Disconnect from RDM SQL
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlDisconnect(
    HCONN               hConn)       /* in:  connection handle */
;
/* ======================================================================
    Specify a database catalog to be opened upon connect
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetCatalog(
    HCONN               hConn,       /* in:  connection handle */
    const SYSDB        *pCat,        /* in:  pointer to catalog to be opened */
    const char         *tfsloc,      /* in:  tfs specification (NULL use default) */
    const char         *mode)        /* in:  db open mode ("s"hared, e"x"clusive */
;
/* ======================================================================
    Specify the database name(s) to be opened upon connect
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetDBNames(
    HCONN               hConn,       /* in:  connection handle */
    const char         *dbnames,     /* in:  "dbname[;dbname]..." */
    const char         *mode)        /* in:  db open mode ("s"hared, e"x"clusive */
;
/* ======================================================================
    Specify the database open mode
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetOpenMode(
    HCONN               hConn,       /* in:  connection handle */
    const char         *mode)        /* in:  db open mode ("s"hared, e"x"clusive */
;
/* ======================================================================
    Allocate a connection handle
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlAllocConnectEx(
    HCONN              *pHconn,      /* out: connection handle */
    int16_t             sAutoCommit, /* in:  auto commit on/off */
    int16_t             sODBC2,      /* in:  return ODBC 2 data types */
    uint32_t            szReserve)   /* in:  bytes of memory to be reserved */
;
/* ======================================================================
    Compile an SQL Statement
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlPrepareEx(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *sqlstr,      /* in:  SQL statement to be compiled */
    STMT_TYPE          *stype,       /* out: compiled statement type */
    uint16_t           *pNoPars,     /* out: number of spec'd param markers */
    uint16_t           *pNoCols)     /* out: number of result columns */
;
/* ======================================================================
    Directly execute a SQL Statement
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlExecDirectEx(
    HSTMT               hStmt,       /* in:  statement handle */
    const char         *sqlstr,      /* in:  SQL statement to be compiled */
    STMT_TYPE          *pStype,      /* out: compiled statement type */
    uint16_t           *pNopars,     /* out: number of spec'd param markers */
    uint16_t           *pNocols)     /* out: number of result columns */
;
/* ======================================================================
    Fetch next set of rows of SELECT statement
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlFetchScroll(
    HSTMT               hStmt,       /* in:  statement handle */
    FETCH_TYPE          fetchType,   /* in:  fetch type */
    int32_t             nRowNum,     /* in:  row number */
    uint32_t            uMaxRows,    /* in:  max # of rows to retrieve */
    const uint32_t     *uLenArray,   /* in:  array of column lengths */
    const RSQL_ROW    **ppRows,      /* out: row list array */
    uint32_t           *puRowCount)  /* out: # of returned rows */
;
/* ======================================================================
   Set the SQL cursor position
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetCursorPos(
    HSTMT               hStmt,       /* in:  statement handle */
    uint32_t            curpos)      /* in:  cursor position */
;
/* ========================================================================
    Retrieve rsql error message for the specified code
    (INTERNAL USE ONLY)
*/
const char *EXTERNAL_FCN rsqlGetErrorMsgEx(
    RSQL_ERRCODE        stat,        /* in:  RSQL error code */
    const char        **pSqlstate,   /* out: SQLSTATE message for the code */
    const char        **pErrMsg)     /* out: error message for the code */
;
/* ========================================================================
    Return TRUE if connected
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlConnected(
    HCONN               hConn,       /* in:  connection handle */
    int16_t            *pConnected)  /* out: 1 if connected */
;
/* ======================================================================
    Set the Query statement handle for ShowPlan
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetQueryHandle(
    HSTMT               hStmt,      /* in:  statement handle */
    HSTMT               hQuery)
;
/* ======================================================================
    Set the CREATE DATABASE overwrite flag
    (INTERNAL USE ONLY)
*/
RSQL_ERRCODE EXTERNAL_FCN rsqlSetOverwrite(
    HCONN               hConn,      /* in:  connection handle */
    int16_t             sOverwrite) /* in:  true to allow overwriting */
;

/* ======================================================================
    Return a string for the SQL_T value supplied
    (INTERNAL USE ONLY)
*/
const char *EXTERNAL_FCN rsqlGetSqlTypeStr(
    SQL_T type);                    /* in:  SQL_T type */

/* ======================================================================
    Return a string for the STMT_TYPE value supplied
    (INTERNAL USE ONLY)
*/
const char *EXTERNAL_FCN rsqlGetStmtTypeStr(
    STMT_TYPE type);                /* in:  STMT_TYPE type */

/* ========================================================================
    Retrieve rsql fetch type string for the specified fetch type
    (INTERNAL USE ONLY)
*/
const char *EXTERNAL_FCN rsqlGetFetchTypeStr(
    FETCH_TYPE type);            /* in:  FETCH_TYPE type */

#if defined(__cplusplus)
}
#endif

#endif  /* __RDMSQL_H */

