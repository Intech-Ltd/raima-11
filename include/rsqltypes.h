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
/*-------------------------------------------------------------------------
    rsqltypes.h  -  RDM SQL data type and constant definitions.
-------------------------------------------------------------------------*/

#if !defined(__RSQLTYPES_H)
#define __RSQLTYPES_H

#include "rdm.h"
#include "rsqlerrs.h"

typedef const void *HSTMT;    /* rsql statement handle */
typedef const void *HCONN;    /* rsql connection handle */
typedef const void *HRSQL;    /* either connection or statement handle */

/* date format codes */
typedef enum {
    /* GEN_RSQL_DATE_FORMAT_BEGIN */
    MMDDYYYY = 1,
    YYYYMMDD,
    DDMMYYYY
    /* GEN_RSQL_DATE_FORMAT_END */
} RSQL_DATE_FORMAT;

/* SQL engine data type values 
   Note: higher precision types must have higher values 
   If this enum is updated, please update sql_type_code_str in
   source/rsql/sql.c */
typedef enum sql_t {
    /* GEN_SQL_T_BEGIN */
    tZERODIV = -1,
    tNOVAL = 0,     /* must be 0 */
    tCHAR,
    tVARCHAR,
    tWCHAR,
    tWVARCHAR,
    tBINARY,
    tVARBINARY,
    tBOOL,
    tTINYINT,
    tSMALLINT,
    tINTEGER,
    tBIGINT,
    tREAL,
    tFLOAT,
    tDOUBLE,
    tBCD,           /* not implemented */
    tDATE,
    tTIME,
    tTIMESTAMP,
    tROWID,
    tCLOB,
    tWCLOB,
    tBLOB,
    tDEFAULT,
    tNULL,
    tPROCVAR,
    tPARAMREF,
    tVALUE,
    tCOLREF,
    tRESULTCOL,
    tMAXSQLTYPE
    /* GEN_SQL_T_END */
} SQL_T;

/* statement type definitions
   (specified order corresponds to position in jump table xfcns[] in sql.c)
*/
typedef enum stmt_type {
    /* GEN_STMT_TYPE_BEGIN */
    sqlNOSTMT = 0,
    sqlOPEN,
    sqlDBUNION,
    sqlCLOSE,
    sqlSELECT,
    sqlINSERT,
    sqlUPDATE,
    sqlDELETE,
    sqlSTART,       /* These 5 entries must... */
    sqlCOMMIT,      /*    be ...               */
    sqlSAVEPOINT,   /*      in ...             */
    sqlRELEASE,     /*        this ...         */
    sqlROLLBACK,    /*           order!        */
    sqlCRPROC,
    sqlDRPROC,
    sqlEXECUTE,  
    sqlSET,
    sqlSETCOLUMN,
    sqlLOCK,     
    sqlUNLOCK,   
    sqlINITDB,
    sqlCRCAT,
    sqlIMPORT,
    sqlEXPORT,
    sqlDROPDB,
    sqlDDL,       /* must be next to last entry */
    sqlMAXSTMT
    /* GEN_STMT_TYPE_END */
} STMT_TYPE;

/* ------------------------------------------------------------------------
   Select statement type
*/
typedef enum select_type {
    selNORMAL = 0,
    selREADONLY,
    selUPDATE
} SELECT_TYPE;

/* ------------------------------------------------------------------------
   State of the statement 
*/
typedef enum stmt_state {
    SS_LIMBO = 0,
    SS_COMPILING,
    SS_PREPARED,
    SS_EXECREADY,
    SS_EXECUTED,
    SS_NEED_DATA,
    SS_FETCHING,
    SS_NODATA
} STMT_STATE;

/* ------------------------------------------------------------------------
   Transaction status type
*/
typedef enum trans_stat {
    /* GEN_TRANS_STAT_BEGIN */
    transINACTIVE = 0,
    transACTIVE,
    transREADONLY,
    transDDL
    /* GEN_TRANS_STAT_END */
} TRANS_STAT;



/* ------------------------------------------------------------------------
   fetchScroll fetch type
*/
typedef enum fetch_type {
    fetchNext = 1,
    fetchPrior,
    fetchFirst,
    fetchLast,
    fetchAbsolute,
    fetchRelative,
    fetchBookmark,
    fetchMAX
} FETCH_TYPE;

/* constraint type definitions */
typedef enum con_type {
    CT_NOKEY = 0,
    CT_KEY,
    CT_UNIQUE,
    CT_UNIHASH,
    CT_PRIMARY,
    CT_PRIHASH
} CON_TYPE;

#define IS_PRIMARY(x)   ((x) == CT_PRIMARY || (x) == CT_PRIHASH)
#define IS_UNIQUE(x)    (IS_PRIMARY(x) || (x) == CT_UNIQUE || (x) == CT_UNIHASH)
#define IS_HASH(x)      ((x) == CT_PRIHASH || (x) == CT_UNIHASH)

/* parameter length value indicator constants (same as ODBC) */
#define RSQL_NULL_DATA              -1
#define RSQL_DATA_AT_EXEC           -2
#define RSQL_LEN_DATA_AT_EXEC(len)  (-len-100)

#define RSQL_GENFILES_CC    0x0001
#define RSQL_GENFILES_CXX   0x0002
#define RSQL_GENFILES_OBJC  0x0004

/* RDM SQL Catalog Versioning:
   A seven character string of the form: "CAT$Vxx" where xx is a
   2 character alphanumeric value beginning with "00".."aa".."ZZ".
   
   The version MUST be incremented for any RDM release in which any
   of the catalog tables change.
   
   The version string (including null terminator) is stored as the first 
   8 bytes of the .cat file (and SYSDB struct).
*/
#define RDM_SQLCAT_VERSION "CAT$V00"
#define RDM_SQLCAT_VERLEN 8

/* RDM SQL Stored Procedure Versioning:
   A seven character string of the form: "PROC$Vxx" where xx is a
   2 character alphanumeric value beginning with "00".."aa".."ZZ".
   
   The version MUST be incremented for any RDM release in which any
   of the stored procedure tables change.
   
   The version string (including null terminator) is stored as the first 
   8 bytes of the .ssp file (and CRPROC_STMT struct).
*/
#define RDM_SQLPROC_VERSION "PRC$V00"
#define RDM_SQLPROC_VERLEN 8

/* ------------------------------------------------------------------------
   fetchScroll rowset packet
*/
typedef struct {
    RSQL_ERRCODE         status;        /* status of each row */ 
    struct _rsql_value  *pVals;         /* array of result columns */
    uint16_t             usValCount;    /* # of result columns */
} RSQL_ROW;

/* container for blob (long var...) data values */
typedef struct {
    void       *buf;     /* pointer to blob data buffer (VALUE.len==amount of blob data in buf) */
    uint32_t    pos;     /* current position==total bytes read so far */
} LONGVAR;

typedef union _value {
    int8_t         tv;      /* tTINYINT, tBOOL */
    int16_t        sv;      /* tSMALLINT */
    int32_t        lv;      /* tINTEGER */
    int64_t        llv;     /* tBIGINT */
    float          fv;      /* tREAL */
    double         dv;      /* tFLOAT, tDOUBLE */
    const char    *cv;      /* tCHAR, tVARCHAR */
    const void    *pv;      /* tBINARY, tVARBINARY */
    LONGVAR        lvv;     /* tCLOB, tWCLOB, tBLOB */
    DATE_VAL       dtv;     /* tDATE */
    TIME_VAL       tmv;     /* tTIME */
    TIMESTAMP_VAL  tsv;     /* tTIMESTAMP */
    DB_ADDR        dba;     /* tROWID (internal use only) */
#if !defined(RDM_NO_UNICODE)
    const wchar_t *wcv;     /* tWCHAR, tWVARCHAR */
#endif    
} VALUE;

typedef enum _val_status {
    /* GEN_VAL_STATUS_BEGIN */
    vsOKAY = 0,
    vsTRUNCATE = 1,        /* string truncation */
    vsNOVAL = 2,
    vsNULL = 3             /* currently, only used with stored proc args */
    /* GEN_VAL_STATUS_END */
} VAL_STATUS;

/* general purpose SQL data value container */
typedef struct _rsql_value { 
    SQL_T      type;       /* internal data type code */
    uint32_t   len;        /* length in bytes of any var-length data (e.g., strlen+1) or 0 if scalar */
    VAL_STATUS status;     /* operation status code */
    VALUE      vt;         /* generic data type container */
} RSQL_VALUE;

/* error status descriptor packet */
typedef struct error_status {
    RSQL_ERRCODE         code;          /* rsql status code */
    int32_t              rowno;         /* row number (for stmt only) */
    int32_t              colno;         /* column number (for stmt only) */
    char                *info;          /* buffer for supplementary error info */
    struct error_status *next;          /* next error in list */
} ERRSTAT;

/* descriptor for each stmt defd in stored proc */
typedef struct stmt_descr {
    STMT_TYPE      stype;              /* statement type */
    uint32_t       pktsz;              /* total size of packet */
    uint32_t       pktpos;             /* .data offset to stmt packet */
} STMT_DESCR;

/* stored procedure execution packet definition */
typedef struct proc_exec {
    char               ver[RDM_SQLPROC_VERLEN]; /* stored procedure version string */
    char               name[NAMELEN];      /* name of stored procedure */
    int16_t            ptype;              /* -1: not loaded, 0: select, 1: modify, 2: modify w/trans */
    int16_t            ignore;             /* ignore errors flag when ptype == 1,2 */
    uint16_t           noargs;             /* number of proc arguments */
    uint16_t           nostmts;            /* number of sql statements in it */
    uint16_t           currstmt;           /* currently executing statement */
    uint32_t           stmtslen;           /* length of compiled stmts data area */
    uint16_t           nolocks;            /* number of lrpkt entries */
    RSQL_VALUE        *args;               /* array of proc arg values */
    const char *const *argnames;           /* array of argument names */
    const STMT_DESCR  *stmts;              /* array of stmt descriptors */
    uint8_t           *stmtpkts;           /* compiled stmts data area */
    LOCK_REQUEST2     *lrpkt;              /* lock request packet when ptype == 2 */
    char               dbname[NAMELEN];    /* name of db when ptype == 2 */
    void              *exec;               /* pointer to original execute stmt packet */
    const char        *sqlstr;             /* original sql string (for possible recompilation) */
} PROC_EXEC;

/* stored procedure execution packet definition (10.1 version) */
typedef struct proc_exec_10_1 {
    char           name[NAMELEN];      /* name of stored procedure */
    int16_t        ptype;              /* -1: not loaded, 0: select, 1: modify, 2: modify w/trans */
    int16_t        ignore;             /* ignore errors flag when ptype == 1,2 */
    uint16_t       noargs;             /* number of proc arguments */
    uint16_t       nostmts;            /* number of sql statements in it */
    uint16_t       currstmt;           /* currently executing statement */
    uint32_t       stmtslen;           /* length of compiled stmts data area */
    uint16_t       nolocks;            /* number of lrpkt entries */
    RSQL_VALUE    *args;               /* array of proc arg values */
    STMT_DESCR    *stmts;              /* array of stmt descriptors */
    uint8_t       *stmtpkts;           /* compiled stmts data area */
    LOCK_REQUEST2 *lrpkt;              /* lock request packet when ptype == 2 */
    char           dbname[NAMELEN];    /* name of db when ptype == 2 */
    void          *exec;               /* pointer to original execute stmt packet */
} PROC_EXEC_10_1;

typedef enum _lock_type {
    lockDEFAULT = 0,                   /* not specified */
    lockREAD = 'r',                    /* read lock */
    lockWRITE = 'w'                    /* write lock */
} LOCK_TYPE;

/* table lock request packet definition */
typedef struct table_lock {
    char           name[NAMELEN];      /* name of the table to be locked */
    LOCK_TYPE      type;               /* kind of lock to apply: read, write, default */
} TABLE_LOCK;

typedef struct {
    SQL_T    type;
    uint32_t len;
    void    *val;
} HILO_VALUE;

#define EXTERNAL_FCN_PTR EXTERNAL_FCN *

/* rsqlErrorCallback function pointer typedef */
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR RSQL_ERRHANDLER) (HRSQL, RSQL_ERRCODE, void *);

/* ========================================================================
     User Defined Function Definitions 
   ======================================================================== */
typedef RSQL_ERRCODE (EXTERNAL_FCN UDFCHECK)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pRegCtx,      /* in:  ptr to registration context */
    uint16_t            noargs,       /* in:  number of arguments */ 
    const RSQL_VALUE   *pArgs,        /* in:  ptr to array of argument values (types) */
    SQL_T              *pType,        /* out: result data type */
    int16_t            *pDeterm);     /* out: deterministic fcn flag (0 or 1) */
    
typedef RSQL_ERRCODE (EXTERNAL_FCN UDFINIT)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pRegCtx,      /* in:  ptr to registration context */
    void               *pFcnCtx);     /* in:  ptr to fcn execution context data area */
    
typedef void (EXTERNAL_FCN UDFTERM)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pFcnCtx);     /* in:  ptr to fcn execution context data area */
    
typedef RSQL_ERRCODE (EXTERNAL_FCN UDFSCALARCALL)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pFcnCtx,      /* in:  ptr to fcn execution context data area */
    uint16_t            noargs,       /* in:  number of arguments */ 
    const RSQL_VALUE   *pArgs,        /* in:  ptr to array of argument values */
    RSQL_VALUE         *pResult);     /* out: ptr to function result value */
    
typedef RSQL_ERRCODE (EXTERNAL_FCN UDFAGGCALL)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pFcnCtx,      /* in:  ptr to fcn execution context data area */
    uint16_t            noargs,       /* in:  number of arguments */ 
    const RSQL_VALUE   *pArgs);       /* in:  ptr to array of argument values */
    
typedef RSQL_ERRCODE (EXTERNAL_FCN UDFAGGRESULT)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pFcnCtx,      /* in:  ptr to fcn execution context data area */
    RSQL_VALUE         *pResult);     /* out: ptr to function result value */
    
typedef RSQL_ERRCODE (EXTERNAL_FCN UDFAGGRESET)(
    HSTMT               hstmt,        /* in:  statement handle */
    void               *pFcnCtx);     /* in:  ptr to fcn execution context data area */

/* typedefs for pointers to each of the above functions */
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PUDFCHECK)      (HSTMT, void *, uint16_t, const RSQL_VALUE*, SQL_T *, int16_t *);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PUDFINIT)       (HSTMT, void *, void *);
typedef void         (EXTERNAL_FCN_PTR PUDFTERM)       (HSTMT, void *);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PUDFSCALARCALL) (HSTMT, void *, uint16_t, const RSQL_VALUE*, RSQL_VALUE*);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PUDFAGGCALL)    (HSTMT, void *, uint16_t, const RSQL_VALUE*);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PUDFAGGRESULT)  (HSTMT, void *, RSQL_VALUE*);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PUDFAGGRESET)   (HSTMT, void *);

/* New functions added for future relases must be added to the end of this table */
typedef struct udfloadtable {
    char           udfName[NAMELEN]; /* name of user-defined function */
    SQL_T          udfType;          /* result data type or tNOVAL if statically unknown */
    PUDFCHECK      udfCheck;         /* address of arg type checking function (req'd) */
    PUDFINIT       udfInit;          /* address of execution initialization function (NULL okay) */
    PUDFTERM       udfTerm;          /* address of execution termination function (NULL okay) */
    PUDFSCALARCALL udfScalarCall;    /* address of scalar calc function (reg'd for scalars, NULL for aggs) */
    PUDFAGGCALL    udfAggCall;       /* address of aggregate item calc function (req'd for aggs, NULL for scalars) */
    PUDFAGGRESULT  udfAggResult;     /* address of aggregate result calc function (req'd for aggs, NULL for scalars) */
    PUDFAGGRESET   udfAggReset;      /* address of aggregate reset function (req'd for aggs, NULL for scalars) */
} UDFLOADTABLE;

typedef UDFLOADTABLE       *PUDFLOADTABLE;
typedef const UDFLOADTABLE *CPUDFLOADTABLE;

typedef void (EXTERNAL_FCN_PTR UDFDESC_FCNPTR) (uint16_t *, const UDFLOADTABLE **, const char **);
typedef void (EXTERNAL_FCN UDFDESC_FCN) (uint16_t *, const UDFLOADTABLE **, const char **);

/* ========================================================================
     Virtual Table Definitions 
   ======================================================================== */

/* virtual table column info packet */
typedef struct vcol_info {
    int16_t          colno;            /* column number in table (0 = 1st) */
    uint32_t         len;              /* column length */
    int16_t         *is_null;          /* == 1 if column value is null (-1 == indeterminate) */
    void            *data;             /* ptr to buffer containing column value (allocated by SQL) */
} VCOL_INFO;

/* ========================================================================
    Virtual table INSERT statement execution function 
*/
typedef RSQL_ERRCODE (EXTERNAL_FCN VTINSERT)(  /* vtInsert() */
    HSTMT        hstmt,        /* in:  statement handle */
    uint16_t     nocols,       /* in:  no. of ref'd columns */
    VCOL_INFO   *colsvals,     /* in:  array of ref'd column value containers */
    void        *pRegCtx)      /* in:  ptr to user's registration context */
/*  
    Called by SQL when INSERT statement is executed (rsqlExecute).
    
    The new column values are contained in colvals.

    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    Returns errSUCCESS or errDUPLICATE or some other appropriate
    error code.  
*/    
;
/* ========================================================================
    Virtual table SELECT row count function 
*/
typedef RSQL_ERRCODE (EXTERNAL_FCN VTROWCOUNT)(  /* vtRowCount() */
    HSTMT            hstmt,    /* in:  statement handle */
    void            *pRegCtx,  /* in:  ptr to user's registration context */
    uint64_t        *pNoRows)  /* out: ptr to row count value */
/*
    Called by SQL optimizer to retrieve the number of rows in the table.

    The hstmt can be used for some calls (e.g., rsqlGetStmtString)
    if desired.
    
    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    The count (or estimate) of the number of rows in the table
    is returned in *pNoRows.
    
    The function must return errSUCCESS unless some application-
    dependent error needs to be reported.
*/
;
/* ========================================================================
    Virtual table select row count function 
*/
typedef RSQL_ERRCODE (EXTERNAL_FCN VTSELECTCOUNT)(  /* vtSelectCount() */
    HSTMT          hstmt,      /* in:  statement handle */
    void          *pRegCtx,    /* in:  ptr to user's registration context */
    void          *pFetchCtx,  /* in:  ptr to fetch context */
    uint64_t      *pNoRows)    /* out: ptr to row count value */
/*
    Called by SQL to retrieve the actual number of rows in the table
    during execution of SELECT COUNT(*) FROM virtab.
*/
;
/* ========================================================================
    Virtual table SELECT execution function 
*/
typedef RSQL_ERRCODE (EXTERNAL_FCN VTSELECTOPEN)(  /* vtSelectOpen() */
    HSTMT        hstmt,        /* in:  statement handle */
    uint16_t     nocols,       /* in:  no. of ref'd columns */
    VCOL_INFO   *colsvals,     /* in:  array of ref'd column value containers */
    void        *pRegCtx,      /* in:  ptr to registration context */
    void        *pFetchCtx,    /* in:  ptr to fetch context */
    RSQL_VALUE  *pkeyval)      /* in:  ptr to primary key value */
/*  
    Called by SQL when SELECT statement is executed (rsqlExecute).
    
    The colvals contains the value containers for each ref'd row.

    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    The pFetchCtx points to an allocated data space of szFetchCtx (see
    rsqlRegisterVirtualTables) bytes long.  This is NULL is szFetchCtx
    was 0. 

    The pkeyval contains the value of the primary key if specified, else NULL.
    If specified then the select applies to that specific value only.
*/
;
/* ========================================================================
    Virtual table fetch function 
*/
typedef RSQL_ERRCODE (EXTERNAL_FCN VTFETCH)(  /* vtFetch() */
    HSTMT        hstmt,        /* in:  statement handle */
    uint16_t     nocols,       /* in:  no. of ref'd columns */
    VCOL_INFO   *colsvals,     /* in:  array of ref'd column value containers */
    void        *pRegCtx,      /* in:  ptr to registration context */
    void        *pFetchCtx)    /* in:  ptr to fetch context */
/*
    Called by SQL to retrieve next row. Returns errSUCCESS until after the
    last row has been fetched then errNOMOREDATA.

    The colvals contains the value containers for each ref'd row.

    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    The pFetchCtx points to an allocated data space of szFetchCtx (see
    rsqlRegisterVirtualTables) bytes long.  This is NULL if szFetchCtx
    was 0. 
*/
;
/* ========================================================================
    Virtual table close function 
*/
typedef void (EXTERNAL_FCN VTSELECTCLOSE)(  /* vtSelectClose() */  
    HSTMT        hstmt,        /* in:  statement handle */
    void        *pRegCtx,      /* in:  ptr to registration context */
    void        *pFetchCtx)    /* in:  ptr to fetch context */
/*
    Called by SQL when SELECT statement containing virtual table reference 
    completes execution (i.e., when cursor is closed).  
    
    Use this function to do any needed cleanup and device termination actions.
*/
;

/* typedefs for pointers to each of the above functions */
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PVTINSERT)      (HSTMT, uint16_t, VCOL_INFO *, void *);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PVTROWCOUNT)    (HSTMT, void *, uint64_t *);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PVTSELECTCOUNT) (HSTMT, void *, void *, uint64_t *);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PVTSELECTOPEN)  (HSTMT, uint16_t, VCOL_INFO *, void *, void *, RSQL_VALUE *);
typedef RSQL_ERRCODE (EXTERNAL_FCN_PTR PVTFETCH)       (HSTMT, uint16_t, VCOL_INFO *, void *, void *);
typedef void         (EXTERNAL_FCN_PTR PVTSELECTCLOSE) (HSTMT, void *, void *);

/* Pass into rsqlRegisterExtTables an array of following type with one entry per 
   virtual table defined in DDL that will be referenced in an SQL SELECT or 
   INSERT statement.
*/
/* If this table is changed (functions added) they should be added to the end
 * to avoid causing problems with existing applications. */
typedef struct vtfloadtable {
    char           vtName[NAMELEN];  /* name of the virtual table */
    PVTINSERT      vtInsert;         /* ptr to INSERT execution function */
    PVTROWCOUNT    vtRowCount;       /* ptr to row count est. function*/
    PVTSELECTCOUNT vtSelectCount;    /* ptr to actual row count function */
    PVTSELECTOPEN  vtSelectOpen;     /* ptr to SELECT init function */
    PVTFETCH       vtFetch;          /* ptr to fetch next row function */
    PVTSELECTCLOSE vtSelectClose;    /* ptr to SELECT term function */
} VTFLOADTABLE;

#if !defined(SQL_CLOSE)
#define SQL_CLOSE           0 
#endif

#if !defined(SQL_DROP)
#define SQL_DROP            1 
#endif

#endif /* __RSQLTYPES_H */
