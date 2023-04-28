/* ----------------------------------------------------------------------------
 * Raima Database Manager
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */
#if !defined(__RDMSQLSERVER_H)
#define __RDMSQLSERVER_H

#include "psp.h"
#include "rdmhttp.h"

typedef struct {
    uint16_t    port;
    uint32_t    rd_only;
    uint32_t    no_disk;
    uint16_t    verbose;
    TFS_TYPE    tfs_type;
    uint16_t    no_tfsrun;
    const char *stdout_file;
    const char *docroot;
} SQL_PARAMS;

#if defined(RDMSQL_DEFN)
typedef struct {
    PSP_MEMTAG mtag;
    RHS_PARAMS params;
    RHS_HANDLE hRHS;
    TFS_HANDLE hTFS;
    TFS_TYPE   tfs_type;
    uint16_t   no_tfsrun;
    uint16_t   tfs_done;
    int16_t    stdout_opened;
} RDMSQL_CTX;
#else
typedef void RDMSQL_CTX;
#endif /* RDMSQL_DEFN */

int32_t EXTERNAL_FCN rdmsqlserver_init(
    const SQL_PARAMS  *sparams,
    RDMSQL_CTX       **psql_ctx)
;
int32_t EXTERNAL_FCN rdmsqlserver_run(
    const RDMSQL_CTX  *sql_ctx,
    DB_BOOLEAN         threaded,
    uint16_t          *sql_done)
;
int32_t EXTERNAL_FCN rdmsqlserver_stop(
    const RDMSQL_CTX *sql_ctx)
;
int32_t EXTERNAL_FCN rdmsqlserver_term(
    const RDMSQL_CTX *sql_ctx)
;

#endif  /* __RDMSQLSERVER_H */

