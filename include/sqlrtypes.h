/***************************************************************************
 * Raima Database Manager                                                  *
 *                                                                         *
 * ODBC Support Package                                                    *
 * --------------------                                                    * 
 * Raima Database Manager extended SQL type definitions                              *
 *                                                                         *
 * Copyright (c) 2011 Raima Inc. All rights reserved.                      *
 *                                                                         *
 * Use of this software, whether in source code format, or in executable,  *
 * binary object code form, is governed by the Raima Inc. LICENSE          *
 * which is fully described in the LICENSE.TXT file, included within this  *
 * distribution of files.                                                  * 
 **************************************************************************/

#if !defined(__SQLRTYPES_H)
#define __SQLRTYPES_H

#include "rdm.h"        /* DB_ADDR8 is defined here */

#if defined(__cplusplus)
extern "C" {
#endif

/* ------------------------------------------------------------------------
   RDM extension - database catalog structure
*/
typedef struct {
    void *ptr;
    char  tfsloc[255];
} DBCAT_STRUCT;

/* ------------------------------------------------------------------------
   RDM extension - const SQLPOINTER
*/
typedef const void *SQLCPOINTER;

/* ------------------------------------------------------------------------
   RDM extension - database address types
*/
typedef DB_ADDR8 SQLROWID;

#ifndef SQL_DB_ADDR
#define SQL_DB_ADDR                 -11
#endif
#define SQL_ROWID                   SQL_DB_ADDR
#define SQL_C_DB_ADDR               SQL_DB_ADDR

/* ------------------------------------------------------------------------
   Default precision and scale (note: default scale should be 0 by ODBC
*/
#define SQL_RDM_DEF_PRECISION       32
#define SQL_RDM_DEF_SCALE           0

/* ------------------------------------------------------------------------
   RDM specific rdbcInitStmt() option
*/
#define SQL_INIT                    5

/* ------------------------------------------------------------------------
   RDM specific error callback function definition
*/
typedef int32_t (SQL_API_PTR ECALLBACK)(int16_t, SQLHANDLE, SQLRETURN);

#if defined(__cplusplus)
}
#endif

#endif  /* __SQLRTYPES_H */

