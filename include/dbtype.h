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

#if !defined(DBTYPE_H)
#define DBTYPE_H

#include "dbtype_legacy.h"
#include "transport.h"
#include "base.h"

/* If INTERNAL_H is defined, then this file is being included only for the
    struct definitions needed to use d_internals().  Therefore, much of the
    stuff in here is not needed and may even cause conflicts with other
    software packages.
    If this file is needed for all the other stuff as well as the struct
    definitions and "internal.h" is also needed, then this file must be
    #include-d BEFORE "internal.h" is #include-d.
*/

#include "tfs_lm.h"     /* must be before dbxtrn.h */

/* ---------------------------------------------------------------------------
    Configure the runtime
*/

#ifdef DBSTAT
#include "dbstat.h"     /* must be before dbxtrn.h */
#endif

/* ------------------------------------------------------------------------ */

#define MAX_ALLOC   ((uint32_t) USHRT_MAX)  /* max allocation size */
#define MAX_TABLE   ((uint32_t) INT_MAX)    /* max table entries */

/* Data definitions used for the B-tree algorithms */
#define ROOT_ADDR    ((F_ADDR)  1)            /* node number of root */
#define NULL_NODE    ((F_ADDR) -1)            /* null node pointer */

#if !defined(INTERNAL_H)

#define NOPGHOLD  0
#define PGHOLD_R  1
#define PGHOLD_W  2

#define KEYFIND   0
#define KEYNEXT   1
#define KEYPREV   2
#define KEYFRST   3
#define KEYLAST   4

/* last key status value */
typedef enum {
    KEYINSTEOF,
    KEYINSTINIT,
    KEYINSTFOUND,
    KEYINSTNOTFOUND
} KEY_INST_STATUS;

typedef enum {
    KEYINIT,
    KEYFOUND,
    KEYNOTFOUND,
    KEYREPOS
} KEY_STATUS;

/* last key status value */
typedef enum {
    DB_READ_WRITE    =  0,
    DB_READ_ONLY     =  1,
    DB_RO_BEFORE_ROT =  2
} DB_READ_ONLY_OPTS;

/* mode values for setting options */
#define OPT_OFF 0
#define OPT_ON  1
#define OPT_DEF 2

/* mode values for r_optkey function */
#define CLROPT    0
#define TSTOPT    1
#define SETOPT    2

/* Constants used by d_open and d_iopen */
#define FIRST_OPEN 0
#define INCR_OPEN  1

#endif /* INTERNAL_H */

/* Masks for different files that can be in-memory */
#define IN_MEM_DBD      0x01
#define IN_MEM_LOG      0x04

/* dictionary attribute flags */
#define SORTFLD         0x0001   /* field is a set sort field */
#define STRUCTFLD       0x0002   /* field is sub-field of struct */
#define UNSIGNEDFLD     0x0004   /* field is unsigned */
#define ONLYKEY         0x0008   /* this key field is the only one in its key file */
#define COMKEYED        0x0010   /* (record contains) or (field included in) compound key */

#define STATIC          0x0002   /* file/record is static */
#define VARCHARREC      0x0004   /* record contains varchar fields */
#define CIRCULAR        0x0008   /* file/record is circular */

#define NEEDS_COMMIT    0x0001
#define TEMPORARY       0x0020   /* file is temporary (no lockmgr & or logging) */
#define VARCOMPACT      0x0040   /* file is a compact vardata file (no pointers back to records) */
#define INMEMORY        0x0080   /* file is in-memory */
#define READ_FLAG       0x0100   /* file is in-memory, read */
#define INMEMREAD       (INMEMORY|READ_FLAG)
#define PERSIST_FLAG    0x0200   /* file is in-memory, persistent */
#define INMEMPERSIST    (INMEMORY|READ_FLAG|PERSIST_FLAG)
#define INMEM_FLAGS     (INMEMORY|READ_FLAG|PERSIST_FLAG)

#define DBD_MEM_PATH    0x0400   /* database dictionary was supplied as in-memory structure */
#define DBD_UNICODE     0x0800   /* database dictionary is in unicode format */
#define DBD_DBA8        0x1000   /* database uses dba8 */
#define DBD_BIGENDIAN   0x2000   /* database dictionary is big endian */
#define DBD_UTF32       0x4000   /* database uses 32-bit widechar */

#if !defined(INTERNAL_H)

#ifndef TRUE
#define FALSE  0
#define TRUE   1
#endif

#define DBUSER_ALIVE_BYTE  1
#define DBUSER_COMMIT_BYTE 2

/* Number of elements in a vector */
#define arraysize(v) (sizeof(v)/sizeof(*(v)))

#define DBD_COMPAT_LEN 6
#define INT_SIZE      sizeof(int32_t)
#define SHORT_SIZE    sizeof(int16_t)
#define LONG_SIZE     sizeof(int32_t)
#define CHAR_SIZE     sizeof(char)
#define DB_ADDR_SIZE  sizeof(DB_ADDR)
#define DB_ADDR4_SIZE sizeof(DB_ADDR4)
#define DB_ADDR8_SIZE sizeof(DB_ADDR8)
#define PGHDRSIZE     4
#define FLOAT_SIZE    sizeof(float)
#define DOUBLE_SIZE   sizeof(double)

#endif /* INTERNAL_H */

#define DBA_NONE      NONE_DBA
#define DBA4_NONE     ((DB_ADDR4) -1)
#define DBA8_NONE     ((DB_ADDR8) -1)
#define DCH_NONE      ((F_ADDR) -1)

#define FILEMASK8     0xFFFFUL
#define FILESHIFT8    48
#define FILEMASK4     0xFFU
#define FILESHIFT4    24

#define RECHDRSIZE    (sizeof(int16_t) + DB_ADDR_SIZE)
#define RECHDRSIZE4   (sizeof(int16_t) + DB_ADDR4_SIZE)
#define RECHDRSIZE8   (sizeof(int16_t) + DB_ADDR8_SIZE)
#define ADDRMASK      db->max_records
#define ADDRMASK4     MAXRECORDS4
#define ADDRMASK8     MAXRECORDS8
#define OPTKEYNDX     0x003F
#define OPTKEY_LIMIT  63
#define OPTKEYSHIFT   10
#define OPTKEYMASK    0xFC00
#define RLBMASK       0x4000
#define DELMASK       0x8000

typedef struct PGZERO_REP_S
{
    DB_ULONG    pz_trans_num;      /* current transaction number */
    REP_DB_TYPE pz_db_type;        /* is this database a master or slave */
}  PGZERO_REP;

#define PGZERO_REP_SIZE sizeof(PGZERO_REP)

/* replication information structure definition */
typedef struct
{
    REP_DB_TYPE      rep_type;               /* replication type of the db */
    TX_FILE          rep_log;                /* the replog file handle   */
    uint16_t         rec_bm_size;            /* total number of records */
    uint16_t         key_bm_size;            /* total number of keys */
    uint16_t         set_bm_size;            /* total number of sets */
    uint8_t         *key_writelock_bm;       /* key read lock bitmap */
    uint8_t         *rec_writelock_bm;       /* record read lock bitmap */
    uint8_t         *set_writelock_bm;       /* key read lock bitmap */
    char            *precommit_replog_name;  /* temporary replication log file name */
    PSP_FILE_OFFSET  rep_log_offset;         /* current offset in rep file */
}  REPLICATION_INFO;

/* Runtime TFS connection definition */
typedef struct _rt_conn_info
{
    TX_USER               userID;
    uint32_t              num_dbs;
    uint32_t              modified;
    TX_CONN_INFO          conn_info;
    struct _rt_conn_info *next;
    struct _rt_conn_info *prev;
} RT_CONN_INFO;

/* set pointer structure definition */
typedef struct _set_ptr4
{
    F_ADDR4  total;     /* total number of members in set */
    DB_ADDR4 first;     /* database address of first member in set */
    DB_ADDR4 last;      /* database address of last member in set */
}  SET_PTR4;

/* set pointer structure definition */
typedef struct
{
    F_ADDR8  total;     /* total number of members in set */
    DB_ADDR8 first;     /* database address of first member in set */
    DB_ADDR8 last;      /* database address of last member in set */
}  SET_PTR8;

/* uno set pointer structure definition */
typedef struct _set_ptr
{
    F_ADDR   total;    /* total number of members in set */
    DB_ADDR  first;    /* database address of first member in set */
    DB_ADDR  last;     /* database address of last member in set */
}  SET_PTR;

/* member pointer structure definition */
typedef struct _mem_ptr4
{
    DB_ADDR4 owner;     /* database address of owner record */
    DB_ADDR4 prev;      /* database address of previous member in set */
    DB_ADDR4 next;      /* database address of next member in set */
}  MEM_PTR4;

/* member pointer structure definition */
typedef struct _mem_ptr8
{
    DB_ADDR8 owner;     /* database address of owner record */
    DB_ADDR8 prev;      /* database address of previous member in set */
    DB_ADDR8 next;      /* database address of next member in set */
}  MEM_PTR8;

/* uno member pointer structure definition */
typedef struct _mem_ptr
{
    DB_ADDR owner;     /* database address of owner record */
    DB_ADDR prev;      /* database address of previous member in set */
    DB_ADDR next;      /* database address of next member in set */
}  MEM_PTR;

/* For replication utility functions */
typedef void *        REP_HANDLE;

/* For RDMHTTP utility functions */
typedef void *        RHS_HANDLE;

/* For d_tfs functions */
typedef void *        TFS_HANDLE;

/* define old tfs lib types for backward compatibility */
#define TFS_LIB_TYPE TFS_TYPE
#define TFS_LIB_TYPE_TFS TFS_TYPE_TFS
#define TFS_LIB_TYPE_RPC TFS_TYPE_RPC
#define TFS_LIB_TYPE_STANDALONE TFS_TYPE_STANDALONE

typedef int32_t (EXTERNAL_FCN *TFS_ITERATE_USERS_FCN)(const char *);
typedef int32_t (EXTERNAL_FCN *TFS_ITERATE_USERS_W_FCN)(const wchar_t *);
typedef union tagDB_ITERATE_FCN {
    TFS_ITERATE_USERS_FCN   fcn;
    TFS_ITERATE_USERS_W_FCN wfcn;
} DB_ITERATE_FCN;


typedef int32_t (EXTERNAL_FCN TFS_XAPI_FCN)(RHS_HANDLE, PSP_OUTPUTBUFF, const char *, const char *, const char *);

typedef struct tfs_functionmap {
    const char         *name;           /* part of a URL, will not be wchar */
    TFS_XAPI_FCN       *fcn;
} TFS_XAPI_FCNMAP;

typedef struct _tfs_xapi_params {
    const TFS_XAPI_FCNMAP  *xapifcnmap;  /* fcn map for http functions */
    uint16_t                fcnmapsize;  /* number of entries in xapifcnmap */
    const char             *subdocroot;  /* dir under httpdocs for html files */
} TFS_XAPI_PARAMS;

typedef struct _tfs_params {
    uint16_t            port;       /* server listen port */
    uint32_t            rd_only;    /* Use read-only device */
    uint32_t            no_disk;    /* diskless system - True|False */
    uint16_t            verbose;    /* verbose - True|False */
    const char         *logfile;
    const char         *stdout_file;
} TFS_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _tfs_xapi_paramsw {
    const TFS_XAPI_FCNMAP  *xapifcnmap;  /* fcn map for http functions */
    uint16_t                fcnmapsize;  /* number of entries in xapifcnmap */
    const wchar_t          *subdocroot;  /* dir under httpdocs for html files */
} TFS_XAPI_PARAMSW;

typedef struct _tfs_paramsw {
    uint16_t            port;       /* server listen port */
    uint32_t            rd_only;    /* Use read-only device */
    uint32_t            no_disk;    /* diskless system - True|False */
    uint16_t            verbose;    /* verbose - True|False */
    const wchar_t      *logfile;
    const wchar_t      *stdout_file;
} TFS_PARAMSW;
#endif

#define LITERAL_NULL_DBA        { 0, 0, 0 }
#define LITERAL_NONE_DBA        { (uint16_t) -1, (FILE_NO) -1, (F_ADDR8) -1 }

static const DB_ADDR NULL_DBA = LITERAL_NULL_DBA;
static const DB_ADDR NONE_DBA = LITERAL_NONE_DBA;

#define DBA_ISNULL(d)       ((d).dba_file == 0 && (d).dba_offset == 0)
#define DBA_ISNONE(d)       ((d).dba_uno == -1 && (d).dba_file == -1 && (d).dba_offset == -1)
#define DBA_EQUAL(d1, d2)       ((d1).dba_uno == (d2).dba_uno && (d1).dba_file == (d2).dba_file && (d1).dba_offset == (d2).dba_offset)

/* max size of set pointer */
/* member count + prior + next */
#define SETP4SIZE  (sizeof(SET_PTR4))
#define SETP8SIZE  (sizeof(SET_PTR8))
#define SP4_MEMBERS (offsetof(SET_PTR4, total))  /* Offset to total members in set ptr */
#define SP8_MEMBERS (offsetof(SET_PTR8, total))  /* Offset to total members in set ptr */
#define SP4_FIRST   (offsetof(SET_PTR4, first))  /* Offset to first member ptr in set ptr */
#define SP8_FIRST   (offsetof(SET_PTR8, first))  /* Offset to first member ptr in set ptr */
#define SP4_LAST    (offsetof(SET_PTR4, last))   /* Offset to last member ptr in set ptr */
#define SP8_LAST    (offsetof(SET_PTR8, last))   /* Offset to last member ptr in set ptr */

#define MEMP4SIZE   (sizeof(MEM_PTR4))           /* Size of member pointer = 3*DB_ADDR_SIZE */
#define MEMP8SIZE   (sizeof(MEM_PTR8))           /* Size of member pointer = 3*DB_ADDR_SIZE */
#define MP4_OWNER   (offsetof(MEM_PTR4, owner))  /* Offset to owner ptr in member ptr */
#define MP8_OWNER  (offsetof(MEM_PTR8, owner))  /* Offset to owner ptr in member ptr */
#define MP4_PREV   (offsetof(MEM_PTR4, prev))   /* Offset to previous member ptr in member ptr */
#define MP8_PREV   (offsetof(MEM_PTR8, prev))   /* Offset to previous member ptr in member ptr */
#define MP4_NEXT   (offsetof(MEM_PTR4, next))   /* Offset to next member ptr in member ptr */
#define MP8_NEXT   (offsetof(MEM_PTR8, next))   /* Offset to next member ptr in member ptr */

/* LMC transport name length in TAF file */
#define LMCTYPELEN      12

#define NAMELEN         32       /* max record/set/field name + null */
#define MAXKEYLEN       256      /* if changed, qdefns.h needs updating */

#define MAXKEYSIZE      MAXKEYLEN

#if !defined(INTERNAL_H)

#define DEFIXPAGES      5        /* default number of index cache pages */
#define DEFDBPAGES      100      /* default number of database cache pages */
#define MINIXPAGES      3        /* minimum number of index cache pages */
#define MINDBPAGES      5        /* minimum number of database cache pages */
#define LENVDBID        48

/* Default value for ft_maxpages, i.e.
   default maximum number of pages for in-memory files.
*/
#if 0
#define DEFAULT_MAX_PAGES     2048
#else
#define DEFAULT_MAX_PAGES        0
#endif

#endif /* INTERNAL_H */

/* record/set lock descriptor */
typedef struct lock_descr
{
    char       fl_type;       /* 'r'ead, 'w'rite, e'x'clusive, 'f'ree */
    char       fl_prev;       /* previous lock type */
    FILE_NO   *fl_list;       /* array of files used by record/set */
    uint32_t   fl_cnt;        /* Number of elements in fl_list */
    DB_BOOLEAN fl_kept;       /* Is lock kept after transaction? */
} LOCK_DESCR;

#define MAXDIMS         3

#define RDM_DATA_FILE    'd'
#define RDM_KEY_FILE     'k'
#define RDM_HASH_FILE    'h'
#define RDM_VARDATA_FILE 'v'
#define RDM_BLOB_FILE    'b'

typedef struct PG0_S
{
    F_ADDR   dchain;                /* delete chain pointer */
    F_ADDR   next;                  /* next page or record slot */
    DB_ULONG timestamp;             /* file's timestamp value */
    F_ADDR   circnext;              /* circ tables only: next record slot */
    uint32_t bdate;                 /* date/time of last backup */
    char     vdb_id[LENVDBID];      /* RDM id mark */
} PG0;

struct pgver;

typedef struct TRMARK_S {
    PSP_FILE_OFFSET  replog_pos;    /* curr pos in replog file for rollback */
    struct TRMARK_S *trnext;
    struct pgver    *pgver_list;
    char             trid[EXPANDABLE];
} TRMARK;

typedef struct pgver {
    char         *buff;            /* page buffer */
    struct pgver *next;            /* next page version (older) for this page */
    TRMARK       *trmark;          /* transaction mark that this change is for */
    struct pgver *prev_for_trmark; /* prev pgver in the trmark's pgver list */
    struct pgver *next_for_trmark; /* next pgver in the trmark's pgver list */
} PGVER;

/* virtual page table entry */
typedef struct page_entry
{
    F_ADDR                      pageno;        /* database page number */
    const struct DB_INSTANCE_S *dbi;         /* cache this page comes from */
    struct page_entry          *next;          /* next in modified or lru list */
    struct page_entry          *prev;          /* prev in modified or lru list */
    struct page_entry          *n_pg;          /* next page for file */
    struct page_entry          *p_pg;          /* prev page for file */
    PGVER                      *pgver;         /* stack of page versions */
    FILE_NO                     file;          /* file table entry number (0..task->size_ft-1) */
    int16_t                     holdcount;     /* "hold-in-cache" counter */
    uint16_t                    buff_size;     /* size of page buffer */
} PAGE_ENTRY;

struct RECORD_ENTRY_S;

typedef struct
{
    char     ac_ft_name[FILENMLEN];  /* name of file */
    uint64_t ac_ft_hashsize;         /* initial size of the hash array */
    F_ADDR8  ac_ft_maxpages;         /* maximum number of pages in a file */
    F_ADDR8  ac_ft_maxslots;         /* maximum number of slots in a file */
    uint16_t ac_ft_slots;            /* record slots per page */
    uint16_t ac_ft_slsize;           /* size of record slots in bytes */
    uint16_t ac_ft_pgsize;           /* size of page */
    uint16_t ac_ft_flags;            /* e.g. STATIC */
    uint16_t ac_ft_ptroffset;        /* for vardata files: offset from start
                                        of slot to delete chain pointer, if
                                        slot is deleted */
    char     ac_ft_type;             /* 'd'ata, 'k'ey, 'v'ardata, 'b'lob, or 'h'ash */
    char     ac_ft_unused[5];        /* padding so this structure aligns correctly */
} AC_FILE_ENTRY;

typedef struct
{
    FILE_NO    fi_fref;            /* lock manager file reference */
    int32_t    fi_applocks;        /* number of read locks or -1 for write lock */
#if defined(RDM_HAS_XLOCKS)
    int32_t    fi_excllocks;       /* number of exclusive locks */
#endif
    int32_t    fi_keptlocks;       /* number of kept locks */
    TX_FILE    fi_txfh;            /* TX file handle */
    DB_BOOLEAN fi_open;            /* TRUE if file is open, FALSE if it is closed */
    uint32_t   fi_lmtimestamp;     /* file timestamp from lock mgr */
    uint32_t   fi_cachetimestamp;  /* previous file timestamp from lock mgr */
} FILE_INSTANCE;

typedef struct
{
    uint64_t       ft_hashsize;        /* initial size of the hash array */
    uint64_t       ft_buckets;         /* number of buckets in a hash file */
    F_ADDR         ft_maxpages;        /* maximum number of pages in a file */
    F_ADDR         ft_maxslots;        /* maximum number of slots in a file */
    FILE_INSTANCE *ft_inst;            /* Information about a particular file instance */
    FILE_NO        ft_ndx;             /* index in file table of this entry */
    uint16_t       ft_slots;           /* record slots per page */
    uint16_t       ft_slsize;          /* size of record slots in bytes */
    uint16_t       ft_pgsize;          /* size of page */
    uint16_t       ft_flags;           /* e.g. STATIC */
    uint16_t       ft_ptroffset;       /* for vardata files: offset from start
                                          of slot to delete chain pointer, if
                                          slot is deleted */
    uint16_t       ft_count;           /* count of the number of items in this file */
    char           ft_name[FILENMLEN]; /* name of file */
    char           ft_type;            /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */

} FILE_ENTRY;

typedef struct AA_FIELD_ENTRY_S
{
    char     aa_fd_key;           /* see the #defines just above */
    char     aa_fd_type;          /* see the #defines just above */
    uint16_t aa_fd_len;           /* length of field in bytes */
    uint16_t aa_fd_dim[MAXDIMS];  /* size of each array dimension */
    uint16_t aa_fd_keyfile;       /* task->file_table entry for key file */
    uint16_t aa_fd_keyno;         /* key prefix number */
    uint16_t aa_fd_ptr;           /* offset to field in record or 1st compound key field in task->key_table */
    uint16_t aa_fd_rec;           /* record table entry of record containing field */
    uint16_t aa_fd_flags;         /* see the #defines at the top of the file */
    uint16_t aa_fd_extfile;       /* file_table entry for vardata file */
    uint16_t aa_fd_varmax;        /* maximum size in bytes of varchar data
                                     excluding null terminator, which is not stored */
    uint16_t aa_fd_nvptrs;        /* number of pointers to slots in vardata file */
    uint16_t aa_fd_len_ex;        /* length of field in bytes, in external structures passed into runtime
                                     library, i.e. with varchar fields expanded to full size. (same as
                                     fd_len except for varchar fields) */
    uint16_t aa_fd_ptr_ex;        /* offset to field in bytes, in external structures passed into runtime lib
                                     (same as fd_ptr except for fields in records which contain varchars) */
    uint16_t aa_fd_keylen;        /* length of key value stored in B-tree, same as fd_len except for
                                     partial keys - only applies to strings and binary char arrays */
} AA_FIELD_ENTRY;

typedef struct {
    const char *val;  /* pointer to the key value */
    uint16_t    len;  /* length of the key value */
    uint16_t    fmt;  /* internal or external format */
} FIELD_DATA;

/* Forward declaration to avoid compiler warnings on the following line */
struct DB_ENTRY_S;
struct FIELD_ENTRY_S;

typedef int32_t (*FLDCMP_FCN)(const struct FIELD_ENTRY_S *, const FIELD_DATA *,
        const FIELD_DATA *, const struct DB_ENTRY_S *);

typedef struct FIELD_ENTRY_S
{
    char     fd_key;           /* see the #defines just above */
    char     fd_type;          /* see the #defines just above */
    uint16_t fd_len;           /* length of field in bytes */
    uint16_t fd_dim[MAXDIMS];  /* size of each array dimension */
    uint16_t fd_keyfile;       /* task->file_table entry for key file */
    uint16_t fd_keyno;         /* key prefix number */
    uint16_t fd_ptr;           /* offset to field in record or 1st compound key field in task->key_table */
    uint16_t fd_rec;           /* record table entry of record containing field */
    uint16_t fd_flags;         /* see the #defines at the top of the file */
    uint16_t fd_extfile;       /* file_table entry for vardata file */
    uint16_t fd_varmax;        /* maximum size in bytes of varchar data
                                  excluding null terminator, which is not stored */
    uint16_t fd_nvptrs;        /* number of pointers to slots in vardata file */
    uint16_t fd_len_ex;        /* length of field in bytes, in external structures passed into runtime
                                  library, i.e. with varchar fields expanded to full size. (same as
                                  fd_len except for varchar fields) */
    uint16_t fd_ptr_ex;        /* offset to field in bytes, in external structures passed into runtime lib
                                  (same as fd_ptr except for fields in records which contain varchars) */
    uint16_t fd_keylen;        /* length of key value stored in B-tree, same as fd_len except for
                                  partial keys - only applies to strings and binary char arrays */
    uint16_t fd_entries;
    uint16_t fd_strdim;
    uint16_t fd_binary;
    uint32_t fd_ndx;           /* index in file table of this entry */
    FLDCMP_FCN                   fd_fldcmp; /* function used to compare this field */
    const struct RECORD_ENTRY_S *fd_rt; /* fields record entry */
} FIELD_ENTRY;

#define fd_blobno fd_keyno     /* We are reusing the fd_keyno field for blobs */

#define FIRST      'f'
#define LAST       'l'
#define NEXT       'n'

#define ASCENDING  'a'
#define DESCENDING 'd'
#define NOORDER    'n'

typedef struct AA_SET_ENTRY_S
{
    int16_t  aa_st_order;      /* 'f'=first, 'l'=last, 'a'=ascending,
                               'd'=descending, 'n'=no order */
    uint16_t aa_st_own_rt;     /* record table entry of owner */
    uint16_t aa_st_own_ptr;    /* offset to set pointers in record */
    uint16_t aa_st_members;    /* index of first member record in member table */
    uint16_t aa_st_memtot;     /* total number of members of set */
    uint16_t aa_st_flags;      /* unused (formerly indicated timestamps */
} AA_SET_ENTRY;

typedef struct
{
    int16_t                 st_order;   /* 'f'=first, 'l'=last, 'a'=ascending,
                                           'd'=descending, 'n'=no order */
    uint16_t                st_own_rt;  /* record table entry of owner */
    uint16_t                st_own_ptr; /* offset to set pointers in record */
    uint16_t                st_members; /* index of first member record in member table */
    uint16_t                st_memtot;  /* total number of members of set */
    uint16_t                st_flags;   /* unused (formerly indicated timestamps */
    uint16_t                st_ndx;     /* index into set table for this entry */
    struct RECORD_ENTRY_S  *st_rt;      /* record table entry for set owner */
    struct MEMBER_ENTRY_S **st_mt;      /* array of member table entries for this set */
} SET_ENTRY;

typedef struct AA_MEMBER_ENTRY_S
{
    uint16_t aa_mt_record;     /* record table entry for this member */
    uint16_t aa_mt_mem_ptr;    /* offset to member ptrs in record */
    uint16_t aa_mt_sort_fld;   /* sort table entry of first sort field */
    uint16_t aa_mt_totsf;      /* total number of sort fields */
} AA_MEMBER_ENTRY;

typedef struct MEMBER_ENTRY_S
{
    uint16_t                mt_record;    /* record table entry for this member */
    uint16_t                mt_mem_ptr;   /* offset to member ptrs in record */
    uint16_t                mt_sort_fld;  /* sort table entry of first sort field */
    uint16_t                mt_totsf;     /* total number of sort fields */
    uint32_t                mt_ndx;       /* index in member table of this entry */
    SET_ENTRY              *mt_set;       /* set entry of set containing this member */
    struct RECORD_ENTRY_S  *mt_rt;        /* record table entry for this member */
#if 0
    /* JRP - we need to add this */
    FIELD_ENTRY           **mt_sort_flds; /* array of field entries for sorting */
#endif
} MEMBER_ENTRY;

typedef struct AA_SORT_ENTRY_S
{
    uint16_t aa_se_fld;     /* field table entry of sort field */
    uint16_t aa_se_set;     /* set table entry of sorted set */
}  AA_SORT_ENTRY;

typedef struct SORT_ENTRY_S
{
    uint32_t         se_fld;        /* field table entry of sort field */
    uint16_t         se_set;        /* set table entry of sorted set */
    uint32_t         se_ndx;        /* index in sort table of this entry */
    const SET_ENTRY *se_st;
}  SORT_ENTRY;

typedef struct AA_RECORD_ENTRY_S
{
    uint16_t aa_rt_file;    /* file table entry of file containing record */
    uint16_t aa_rt_len;     /* total length of record */
    uint16_t aa_rt_data;    /* offset to start of data in record */
    uint16_t aa_rt_fields;  /* first field def in task->field_table */
    uint16_t aa_rt_fdtot;   /* total number of fields in record */
    uint16_t aa_rt_flags;
    uint16_t aa_rt_len_ex;  /* length of record in external format, i.e.
                               with all varchar fields expanded to full
                               size - same as aa_rt_len except in records
                               with varchar fields. */
} AA_RECORD_ENTRY;

typedef struct RECORD_ENTRY_S
{
    uint16_t             rt_file;      /* file table entry of file containing record */
    uint16_t             rt_len;       /* total length of record */
    uint16_t             rt_data;      /* offset to start of data in record */
    uint32_t             rt_fields;    /* first field def in task->field_table */
    uint16_t             rt_fdtot;     /* total number of fields in record */
    uint16_t             rt_flags;
    uint16_t             rt_len_ex;    /* length of record in external format, i.e.
                                          with all varchar fields expanded to full
                                          size - same as rt_len except in records
                                          with varchar fields. */
    uint16_t             rt_totndx;    /* index into the record counts in the file */
    uint32_t             rt_keytot;    /* total number of keys in record */
    uint32_t             rt_hashtot;   /* total number of hashes in record */
    uint32_t             rt_noowns;    /* number of sets record is an owner of */
    uint32_t             rt_nomems;    /* number of sets record is a member of */
    uint32_t             rt_noblobs;   /* number of blobs in the record */
    uint32_t             rt_novarchar; /* number of varchar fields in the record */
    uint16_t             rt_ndx;       /* index into record table for this record */
    const FILE_ENTRY    *rt_ft;        /* owning file */
    const SET_ENTRY    **rt_ownsets;   /* list of sets this record is an owner of */
    const MEMBER_ENTRY **rt_memsets;   /* list of sets this record is a member of */
    const FIELD_ENTRY  **rt_blobs;     /* list of blob fields in this record */
    const FIELD_ENTRY  **rt_varchar;   /* list of varchar fields in this record */
    const FIELD_ENTRY  **rt_flds;      /* array of fields in this record */
} RECORD_ENTRY;

/* kinds of keys */
#define DB_INDEX_NONE      'n'
#define DB_INDEX_DUPLICATE 'd'
#define DB_INDEX_UNIQUE    'u'
#define DB_INDEX_HASH      'h'

#define IS_UNIQUE_KEY_INDEX(fd)    ((fd)->fd_key == DB_INDEX_UNIQUE)
#define IS_DUPLICATE_KEY_INDEX(fd) ((fd)->fd_key == DB_INDEX_DUPLICATE)

#define IS_KEY_INDEX(fd)        (IS_UNIQUE_KEY_INDEX(fd) || IS_DUPLICATE_KEY_INDEX(fd))
#define IS_HASH_INDEX(fd)       ((fd)->fd_key == DB_INDEX_HASH)
#define IS_NOT_INDEX(fd)        ((fd)->fd_key == DB_INDEX_NONE)

/* kinds of fields */
#define DB_FLOAT_TYPE      'f'
#define DB_DOUBLE_TYPE     'F'
#define DB_CHARACTER_TYPE  'c'
#define DB_WIDECHAR_TYPE   'C'
#define DB_VARCHAR_TYPE    'v'
#define DB_VARWCHAR_TYPE   'V'
#define DB_VARBINARY_TYPE  'n'
#define DB_SHORTINT_TYPE   's'  /* Should always be considered 16-bit integer */
#define DB_REGINT_TYPE     'i'  /* Should always be considered 32-bit integer */
#define DB_LONGINT_TYPE    'l'  /* Should always be considered 32-bit integer */
#define DB_INT64_TYPE      'L'  /* Should always be considered 64-bit integer */
#define DB_ADDR_EXT_TYPE   'D'
#define DB_BLOB_TYPE       'b'
#define DB_ADDR4_TYPE      'd'
#define DB_ADDR8_TYPE      'a'
#define DB_GROUPED_TYPE    'g'
#define DB_COMKEY_TYPE     'k'

/* Field formats (varchar fields and partial keys) for field values passed
   to comparison functions:

    DB_DATASLOT Format used in data file slots, i.e. short value containing
                string length followed by array of vardata pointers, in the
                case of varchar fields.
    DB_KEYSLOT  Format used in key files, i.e. key string value truncated
                to length of partial key, followed by database address of
                record slot.
    DB_EXTVAL   External format used by the application, i.e. fully expanded
                string field.
*/
#define DB_DATASLOT 0x01
#define DB_KEYSLOT  0x02
#define DB_EXTVAL   0x04

typedef struct AA_KEY_ENTRY_S
{
    uint16_t aa_kt_key;        /* compound key field number */
    uint16_t aa_kt_field;      /* field number of included field */
    uint16_t aa_kt_ptr;        /* offset to start of field data in key,
                                  as stored in database, which may contain
                                  partial strings */
    uint16_t aa_kt_len;        /* length of field data in key, as stored
                                  in database - different from original
                                  field length for varchar and partial
                                  strings */
    uint16_t aa_kt_ptr_ex;     /* offset to start of field data in
                                  external structure passed in from
                                  application, which contains full
                                  strings only */
    uint16_t aa_kt_len_ex;     /* length of field data in external structure
                                  passed in from application - must be same
                                  as fd_len_ex in field definition */
    int16_t  aa_kt_sort;       /* 'a' = ascending, 'd' = descending */
} AA_KEY_ENTRY;

typedef struct
{
    uint32_t kt_key;           /* compound key field number */
    uint32_t kt_field;         /* field number of included field */
    uint16_t kt_ptr;           /* offset to start of field data in key,
                                  as stored in database, which may contain
                                  partial strings */
    uint16_t kt_len;           /* length of field data in key, as stored
                                  in database - different from original
                                  field length for varchar and partial
                                  strings */
    uint16_t kt_ptr_ex;        /* offset to start of field data in
                                  external structure passed in from
                                  application, which contains full
                                  strings only */
    uint16_t kt_len_ex;        /* length of field data in external structure
                                  passed in from application - must be same
                                  as fd_len_ex in field definition */
    int16_t  kt_sort;          /* 'a' = ascending, 'd' = descending */
} KEY_ENTRY;

typedef struct PGZERO8_S
{
    F_ADDR8  pz_dchain;             /* delete chain pointer */
    F_ADDR8  pz_next;               /* next available slot number */
    DB_ULONG pz_unused;             /* obsolete */
    DB_ULONG pz_unused2;            /* padding for obsolete value */
    F_ADDR8  pz_circnext;           /* circ tables only: next slot */
    F_ADDR8  pz_totals[EXPANDABLE]; /* record totals */
} PGZERO8;

typedef struct PGZERO_DATA_S
{
    PGZERO8              *pz8;    /* page zero data */
    const TRMARK         *trmark; /* the transaction mark that this was changed under */
    struct PGZERO_DATA_S *next;   /* next transaction mark */
} PGZERO_DATA;

/* page zero table entry */
typedef struct PGZERO_S
{
    PGZERO_DATA *pz_data;
    DB_BOOLEAN   pz_initialized; /* TRUE if the file has just been initialized */
    B_BT         pg_list;
}  PGZERO;

/* page zero table entry */
typedef struct PGZERO4_S
{
    F_ADDR4  pz_dchain;             /* delete chain pointer */
    F_ADDR4  pz_next;               /* next available slot number */
    DB_ULONG pz_unused;             /* placeholder for file's timestamp value */
    F_ADDR4  pz_circnext;           /* circ tables only: next slot */
    F_ADDR4  pz_totals[EXPANDABLE]; /* record totals */
}  PGZERO4;

/* node key search path stack entry: one per level per key field */
typedef struct NODE_PATH_S
{
    F_ADDR   node;  /* node (page) number  */
    uint16_t slot;  /* slot number of key */
} NODE_PATH;

typedef struct {
    FIELD_DATA data;
    DB_ADDR    dba;
} KEY_POSITION;

/* index key information: one entry per key field */
typedef struct KEY_INFO_S
{
    KEY_POSITION      *pos;        /* current position in the index */
    KEY_POSITION       userpos;    /* user data */
    uint32_t           keysize;    /* maximum size of the key (curr_pos.val
                                      will be allocated to this size */
    uint16_t           max_lvls;   /* maximum possible levels for key */
    int32_t            keydir;     /* key direction (via d_keydir) */
    int32_t            reposdir;   /* key direction (via d_keydir) */
    KEY_STATUS         lstat;      /* status of the key */
    const FIELD_ENTRY *fd;
    const FILE_ENTRY  *ft;
    struct DB_ENTRY_S *db;
} KEY_INFO;

/* index file node structure */
typedef struct
{
    uint32_t last_chgd;         /* date/time of last change of this node */
    uint16_t used_slots;        /* current # of used slots in node */
    char     slots[EXPANDABLE]; /* start of slot area */
} KEY_NODE;

typedef struct _USED_KEY_PAGES
{
    struct _USED_KEY_PAGES *next;
    KEY_NODE               *node;
    void                   *pg;
    F_ADDR                  addr;
    int32_t                 hold;
    int32_t                 modified;
    FILE_NO                 fno;
} USED_KEY_PAGES;

typedef struct 
{
    KEY_POSITION                pos;
    NODE_PATH                  *node_path;
    KEY_INST_STATUS             lstat;      /* status of the key instance */
    uint16_t                    value_used; /* has this union value been used in key fcns */
    uint16_t                    level;      /* current level # in node path */
    int32_t                     lastdir;    /* last direction travelled */
    USED_KEY_PAGES             *used;
    KEY_INFO                   *ki;
    const struct DB_INSTANCE_S *dbi;
} KEY_INST_INFO;

typedef struct
{
    uint32_t           hashsize;
    char              *data;
    const FIELD_ENTRY *fd;
    const FILE_ENTRY  *ft;
} HASH_INFO;

typedef struct
{
    F_ADDR                      root;
    F_ADDR                      curr;
    uint16_t                    slotno;
    char                       *normalized;
    HASH_INFO                  *hi;
    const struct DB_INSTANCE_S *dbi;
} HASH_INST_INFO;

#define HASH_VALUE      uint64_t
#define HASH_VALUE_HALF uint32_t

/* key slot structure */
typedef struct key_slot
{
    F_ADDR   child;            /* child node pointer */
    uint16_t keyno;            /* key number */
    char     data[MAXKEYSIZE]; /* start of key data */
} KEY_SLOT;

typedef struct key_slot_ptr
{
    F_ADDR      child;         /* child node pointer */
    uint16_t    keyno;         /* key number */
    const char *data;          /* start of key data */
} KEY_SLOT_PTR;

typedef struct ll_elem
{
    struct ll_elem *next;
    char           *data;
}  ll_elem;

typedef struct
{
    ll_elem *head;
    ll_elem *tail;
    ll_elem *curr;
}  llist;

/* blob information; one entry per blob field */
typedef struct
{
    int32_t    dbn;
    DB_ADDR    dba;
    BLOB_ID    bid;
    F_ADDR     pgno;
    uint32_t   pos;
    uint32_t   size;
    uint32_t   fldno;
    FILE_NO    bfile;
    uint16_t   slsize;
} BLOB_INFO;

/* Define Cache table */
typedef struct CACHE_S
{
    uint16_t      num_tags;
    uint16_t      largest_page;   /* size of largest page in pg_cache */
    DB_BOOLEAN    prealloc;
    uint32_t      initsize;
    uint32_t      currsize;
    uint32_t      num_modified;
    uint32_t      num_held;
    B_BT          lookup;
    PAGE_ENTRY   *lru;
    PAGE_ENTRY   *mru;
    PAGE_ENTRY   *modified;
    PAGE_ENTRY  **last_page;
#ifdef DBSTAT
    MEM_STATS     mem_stats;
    CACHE_STATS   cache_stats;
#endif
} CACHE;

struct TASK_S;
struct DB_ENTRY_S;

/* database instance entry declaration */
typedef struct DB_INSTANCE_S
{
    uint16_t         uno;
    TX_DB            dbID;
    PGZERO          *pgzero;
    RT_CONN_INFO    *tfs_conn;
    int16_t          lm_session_active; /* have we called tx_lmDbOpen */
    FILE_NO         *file_index;
    uint16_t         size_fi;
    REPLICATION_INFO ri;                /* replication information */
    CACHE           *cache;
#ifdef DBSTAT
    FILE_STATS      *file_stats;
#endif
    KEY_INST_INFO   *key_inst_info;     /* formerly indexed by key_offset */
    HASH_INST_INFO  *hash_inst_info;
    char            *db_name;           /* name of this database */

    TFS_LM_LOCK     *lock_pkt;

    /* scope: keyl_cnt */
    LOCK_DESCR      *key_locks;

    /* scope: size_rt */
    LOCK_DESCR      *rec_locks;

    /* scope: size_st */
    LOCK_DESCR      *set_locks;

    struct DB_ENTRY_S *db;
} DB_INSTANCE;

typedef struct
{
    uint16_t Page_size;
    uint16_t Size_ft;
    uint16_t Size_rt;
    uint16_t Size_fd;
    uint16_t Size_st;
    uint16_t Size_mt;
    uint16_t Size_srt;
    uint16_t Size_kt;
    uint16_t dummy;
} DICT_SIZE;

typedef struct {
    uint16_t      page_size;
    uint16_t      size_ft;
    uint16_t      size_rt;
    uint16_t      size_st;
    uint16_t      size_mt;
    uint16_t      size_srt;
    uint32_t      size_fd;
    uint16_t      size_kt;
    FILE_ENTRY   *file_table;
    RECORD_ENTRY *record_table;
    FIELD_ENTRY  *field_table;
    KEY_ENTRY    *key_table;
    SET_ENTRY    *set_table;
    MEMBER_ENTRY *member_table;
    SORT_ENTRY   *sort_table;
} DB_DBD;

/* database table entry declaration */
typedef struct DB_ENTRY_S
{
    uint32_t            magic;
    DB_ADDR             curr_rec;

    /* Dictionary */
    DB_DBD              dbd;

    /* size and offsets for this db */
    uint16_t            dba_size;          /* database address size (in bytes) */
    uint16_t            faddr_size;        /* faddr size (in bytes) */
    uint16_t            vaddr_size;        /* vaddr size (in bytes) */
    uint16_t            mem_ptr_size;      /* mem_ptr size (in bytes) */
    uint16_t            mp_owner;          /* owner offset for mem_ptr */
    uint16_t            mp_prev;           /* prev offset for mem_ptr */
    uint16_t            mp_next;           /* next offset for mem_ptr */
    uint16_t            set_ptr_size;      /* set_ptr size (in bytes) */
    uint16_t            sp_members;        /* members offset for set_ptr */
    uint16_t            sp_first;          /* first offset for set_ptr */
    uint16_t            sp_last;           /* last offset for set_ptr */
    uint16_t            rec_hdr_size;      /* record header size (in bytes) */
    uint32_t            max_files;         /* max files allowed in this db */
    uint64_t            max_pages;         /* max pages per file in this db */
    uint64_t            max_vardataslots;  /* max slots in a vardata file for this db */
    uint64_t            max_records;       /* max records per file in this db */

    /* scope: size_ft */
    size_t              lp_size;      /* size of lock_pkt allocation */
    size_t              fp_size;      /* size of free_pkt allocation */
    TFS_LM_FREE        *free_pkt;

    /* scope: size_rt */
    char              **record_names;

    /* scope: size_st */
    char              **set_names;
    DB_ADDR            *curr_own;
    DB_ADDR            *curr_mem;

    /* scope: size_fd */
    char              **field_names;

    uint32_t            keyl_cnt;
    uint32_t            no_of_keys;
    uint32_t            no_of_hashs;

    /* scope: no_of_blobs */
    BLOB_INFO          *blob_info;  /* [WLW] formerly indexed by blob_offset */
    uint32_t            no_of_blobs;

    /* Keys */
    uint16_t            key_size;     /* Must be as big as any compound
                                      key field, or key file slot. */
    int32_t             struct_key_chk;
    KEY_INFO           *key_info;
    KEY_INFO           *curkey;
    HASH_INFO          *hash_info;
    HASH_INFO          *curhash;
    char               *key_buff1;         /* Buffers for expanding com- */
    char               *key_buff2;         /* pound keys to full length. */

    llist               sk_list;             /* key list for d_makenew */
    DB_ADDR             sysdba;              /* database address of system record */
    char               *Objnames;            /* buffer for rec, fld & set names */
    int16_t             db_ver;              /* version of dbd file */
    char                endian;              /* endianess of dbd file */
    char                dba_type;            /* dba type of dbd file */
    char                string_format;       /* string format of dbd file */
    uint16_t            db_flags;            /* e.g. INMEMORY */
    DB_READ_ONLY_OPTS   db_read_only;      /* is this database read-only */
    DB_BOOLEAN          rot;                 /* do we have a ROT open on this database */
    struct TASK_S      *task;

    const RECORD_ENTRY *rn_rt;   /* Last record type supplied to recfrst/recset */
    DB_ADDR             rn_dba;     /* Last db addr computed by recfrst/recset/recnext */

    /* misc. informatino */
    int32_t             use_locale_sort;

    /* union information */
    uint16_t            no_of_unions;
    DB_INSTANCE        *db_inst;
} DB_ENTRY;

#if !defined(INTERNAL_H)

#define DB_CLOSE  close
#define DB_READ   read
#define DB_WRITE  write
#define DB_LSEEK  lseek


#define DB_TELL(fd)   DB_LSEEK(fd, 0, PSP_SEEK_CUR)

/* File stream functions used in dbedit */
#define DB_FTELL ftell

#endif /* INTERNAL_H */

struct sk
{
    char    *sk_val;
    uint32_t sk_fld;
};

/* file rename table entry declarations */
typedef struct ren_entry
{
    char   *file_name;
    char   *ren_db_name;
    FILE_NO file_no;
} REN_ENTRY;

typedef struct {
    union {
        FILE_ENTRY                 *p;
        AB_FILE_ENTRY              *ab;
        AA_FILE_ENTRY_DBA4         *aaa4;
        AA_FILE_ENTRY_DBA8         *aaa8;
        V300_FILE_ENTRY            *v300;
        V301_FILE_ENTRY            *v301;
        V500_FILE_ENTRY            *v500;
        V600_FILE_ENTRY            *v600;
#if !defined(RDM_NO_UNICODE)
        AA_FILE_ENTRY_UNICODE_DBA4 *aau4;
        AA_FILE_ENTRY_UNICODE_DBA8 *aau8;
        U500_FILE_ENTRY            *u500;
        U600_FILE_ENTRY            *u600;
#endif
    } fptr;
    union {
        RECORD_ENTRY      *p;
        V300_RECORD_ENTRY *v300;
        V600_RECORD_ENTRY *v600;
        AA_RECORD_ENTRY   *aa;
    } rptr;
    union {
        FIELD_ENTRY      *p;
        V300_FIELD_ENTRY *v300;
        V600_FIELD_ENTRY *v600;
        AA_FIELD_ENTRY   *aa;
    } fdptr;
    union {
        KEY_ENTRY      *p;
        V300_KEY_ENTRY *v300;
        V600_KEY_ENTRY *v600;
        AA_KEY_ENTRY   *aa;
    } kptr;
} TABLE_PTRS;

#if !defined(INTERNAL_H)

typedef struct
{
    char  *strbuf;
    size_t buflen;
    size_t strlen;
} DB_STRING;

char  *STRinit(DB_STRING *, char *, size_t); /*lint -esym(534,STRinit) */
char  *STRcpy(DB_STRING *, const char *); /*lint -esym(534,STRcpy) */
char  *STRcat(DB_STRING *, const char *); /*lint -esym(534,STRcat) */
char  *STRccat(DB_STRING *, int32_t); /*lint -esym(534,STRccat) */
size_t STRavail(const DB_STRING *);

#ifdef DB_TRACE

#define API_ENTER(fn)   {if (task->db_trace & TRACE_API) api_enter(fn, task);}
#define API_RETURN(c)   return((task->db_trace & TRACE_API) ? api_exit(task) : 0, (c))
#define API_EXIT()      {if (task->db_trace & TRACE_API) api_exit(task);}

#else  /* DB_TRACE */

#define API_ENTER(fn)   UNREF_PARM(fn)
#define API_RETURN(c)   return(c)
#define API_EXIT()      /**/

#endif /* DB_TRACE */

#define DBA_FILE(d) ((d).dba_file)
#define DBA_SLOT(d) ((F_ADDR) ((d).dba_offset))
#define DBA_UNO(d)  ((d).dba_uno)

#if !defined(RELEASE)
    /* This removes some uninitialized memory problems for valgrind */
#define INITIALIZE_DBA(db_addr) memset(&(db_addr), 0, sizeof(DB_ADDR));
#else
#define INITIALIZE_DBA(db_addr)
#endif

#define ENCODE_DBA(uno, file, slot, db_addr) \
    INITIALIZE_DBA(db_addr) \
    (db_addr).dba_uno = (uno); \
    (db_addr).dba_file = (file); \
    (db_addr).dba_offset = (slot);

#define ENCODE_DBA4(file, slot, dba) \
    (*(dba) = (((F_ADDR4) (file) & FILEMASK4) << FILESHIFT4) | \
            ((F_ADDR4) (slot) & ADDRMASK4))

#define ENCODE_DBA8(file, slot, dba) \
    (*(dba) = (((F_ADDR8) (file) & FILEMASK8) << FILESHIFT8) | \
            ((F_ADDR8) (slot) & ADDRMASK8))

#define DB_ADDR_FILE(d) ((d).dba_file)
#define DB_ADDR_FILE4(dba) ((FILE_NO) (((F_ADDR) (dba) >> FILESHIFT4) & FILEMASK4))
#define DB_ADDR_FILE8(dba) ((FILE_NO) (FILEMASK8 & (dba >> FILESHIFT8)))

#define DB_ADDR_SLOT(d) ((F_ADDR) ((d).dba_offset))
#define DB_ADDR_SLOT4(dba) ((F_ADDR4) (dba) & ADDRMASK4)
#define DB_ADDR_SLOT8(dba) ((F_ADDR8) (dba) & ADDRMASK8)

#define DB_ADDR_UNO(d)  ((d).dba_uno)

#define DECODE_DBA(dba, uno, file, slot) {\
    *(uno) =  DBA_UNO(dba);\
    *(file) = DBA_FILE(dba);\
    *(slot) = DBA_SLOT(dba);\
}

#define DECODE_DBA4(dba, file, slot) {\
    *(file) = DB_ADDR_FILE4(dba);\
    *(slot) = DB_ADDR_SLOT4(dba);\
}

#define DECODE_DBA8(dba, file, slot) {\
    *(file) = DB_ADDR_FILE8(dba);\
    *(slot) = DB_ADDR_SLOT8(dba);\
}

#define DB_ADDR_EQ(d1, d2)      ((DBA_UNO(d1) == DBA_UNO(d2)) && (DB_ADDR_SLOT(d1) == DB_ADDR_SLOT(d2)) && (DB_ADDR_FILE(d1) == DB_ADDR_FILE(d2)))


#endif /* INTERNAL_H */

#define DB_TIMEOUT     10     /* lock request wait 10 seconds in queue */

#define NET_TIMEOUT    3      /* how long to wait for connection */

/* size of the struct as stored in the files */
#define PGZERO8SZ(ft) (sizeof(PGZERO8) + EXPANDABLE_COUNT(ft->ft_count) * sizeof(F_ADDR8))
#define PGZERO4SZ(ft) (sizeof(PGZERO4) + EXPANDABLE_COUNT(ft->ft_count) * sizeof(F_ADDR4))

#ifndef MSGVER
#define MSGVER  0x500
#endif


#if defined(TASK_DEFN)
#include "dbxtrn.h"

#if !defined(INTERNAL_H)
#include "proto.h"
#endif /* INTERNAL_H */
#endif

#ifdef QNX

#define LOCK_FCN   LK_LOCK
#define UNLOCK_FCN LK_UNLCK

#else /* QNX */

#define UNLOCK_FCN 0
#define LOCK_FCN   1

#endif /* QNX */

#define READ_FCN   0
#define WRITE_FCN  1

#endif /* DBTYPE_H */

