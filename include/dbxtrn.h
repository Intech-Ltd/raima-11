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

#ifndef DBXTRN_H
#define DBXTRN_H

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Database Dictionary Tables */

#define LOCK_STACK_SIZE    10

struct TASK_S;

typedef void (EXTERNAL_FCN *ERRORPROCA)(int32_t, char *);
typedef void (EXTERNAL_FCN *ERRORPROC2A)(int32_t, const char *, void *, struct TASK_S *);
#if !defined(RDM_NO_UNICODE)
typedef void (EXTERNAL_FCN *ERRORPROCW)(int32_t, wchar_t *);
typedef void (EXTERNAL_FCN *ERRORPROC2W)(int32_t, const wchar_t *, void *, struct TASK_S *);
#endif

/* used to verify sanity of task pointer */
#define TASK_MAGIC1 0x7d24a90f
#define TASK_MAGIC2 0xb85ecd62
#define DB_MAGIC    0xa4f73bc9

typedef struct TASK_S
{
    int32_t              db_status;
    RT_CONN_INFO        *tfs_conns;
    char                *dbuserid;
    uint32_t             magic1;
    PSP_MEMTAG           tag;

    /* Open databases */
    int32_t              curr_db;
    int32_t              set_db;
    uint16_t             no_of_dbs;
    DB_ENTRY           **db_table;     /* scope: no_of_dbs */
    uint16_t             mode;         /* open mode flags */
    uint32_t             initsize;

    /* Logging data */
    B_LOG_DATA           logdata;
    B_LOG_FH             lfh;
    uint32_t             log_curr_file;


    int32_t              errnum;       /* primary error code */
    int32_t              err_ex;       /* secondary error code */
    int32_t              errno_c;      /* O/S error code */
    const char          *errfile;
    int32_t              errline;
    void                *err_userdata;
    void  (EXTERNAL_FCN *error_func)(void);
    int16_t              which_err_func;
    char*                iniFile;
    int32_t              dbopen;
    int16_t              inmemory;
    DB_ULONG             dboptflag;    /* which Dboptions have been explicitly set */
    DB_ULONG             dboptions;
    uint16_t             old_no_of_dbs;

    /* Locking */
    uint32_t             readlocksecs;
    uint32_t             writelocksecs;
    uint32_t             dbwait_time;
    uint32_t             db_timeout;
    int32_t              db_lockmgr;

    /* Replication */
    /*void                *repTask; */
    DB_BOOLEAN           bReadOnly;
    DB_BOOLEAN           bRepClient;

    /* Transactions */
    int16_t              tr_precommitted;
    TRANS_TYPE           tr_type;
    TRMARK              *trmark;
    TRMARK              *excl_mark;

    /* Memory pools */
    PSP_POOL            *pgver_pool;
    PSP_POOL            *pgverbuff_pool;
    PSP_POOL            *pzdata_pool;
    PSP_POOL            *pzdatapz8_pool;
    PSP_POOL            *trmark_pool;
    PSP_POOL            *usedkeypage_pool;

    /* Miscellaneous */
    char                *vdata_buff1;  /* Buffers for expanding varchar */
    char                *vdata_buff2;  /* fields to full length strings */
    uint16_t             vdata_size;   /* Must be as big as any varchar
                                          field in any open database. */
    DB_BOOLEAN           session_active;
    int32_t              cnt_open_files;    /* count of currently open files */
    TFS_LIB_TYPE         tfs_type;
    char                *default_tfs_hostname;
    uint16_t             default_tfs_port;

#ifdef DBSTAT
    GEN_STATS            gen_stats;
#endif /* DBSTAT */

    DB_ULONG             db_trace;
    int16_t              db_indent;
    DB_ULONG             db_debug;
    uint32_t             magic2;
} TASK, DB_TASK;

typedef struct {
    uint32_t updtime;
    //uint32_t updtime_padding;
    F_ADDR   next;
    F_ADDR   prev;
    uint32_t seq;
    uint32_t size;
    char    *data;
} BLOB_HDR;

typedef struct {
    uint32_t updtime;
    F_ADDR4  next;
    F_ADDR4  prev;
    uint32_t seq;
    uint32_t size;
    char    *data;
} BLOB_HDR4;

#ifdef __cplusplus
}
#endif

#endif      /* DBXTRN_H */

