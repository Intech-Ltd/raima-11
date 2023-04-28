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
/*-------------------------------------------------------------------------
    sqlcat.h  -  RDM SQL catalog type definitions.

    These tables are designed so that they can be statically initialized.
    Hence, arrays and array subscripts are used for intertable relationships
    and not pointers.
-------------------------------------------------------------------------*/

#if !defined(__SQLCAT_H)
#define __SQLCAT_H

#include "rsqltypes.h"

typedef struct sysdomain {
    char            name[NAMELEN];  /* domain name*/
    int16_t         typeno;         /* index into types array of type definition */
} SYSDOMAIN;

typedef struct systype {
    SQL_T           data_type;      /* SQL data type (internal) */
    uint16_t        length;         /* length */
    uint16_t        disp_width;     /* display width */
    const char     *disp_fmt;       /* display format string */
    SQL_T           deflt_type;     /* type of default value (not nec. same as data_type) */
    uint32_t        deflt_len;      /* length (in bytes) of default value */
    const void     *deflt_val;      /* pointer to default value */
} SYSTYPE;

typedef struct systable {
    char            name[NAMELEN];  /* table name */
    int32_t         recid;          /* core-level record id (-1=virtual, -2=virtual readonly) */
    uint16_t        length;         /* length of record */
    uint32_t        pgsize;         /* page size */
    uint16_t        slots;          /* slots per page or the vtTable index if virtual table */
    uint16_t        nocols;         /* number of columns in table */
    uint16_t        columns;        /* index into columns array of first column in table */
    uint16_t        nokeys;         /* number of keys declared in this table */
    uint16_t        keys;           /* index into keys array of first key in table */
    uint16_t        norefs;         /* number of foreign keys in table */
    uint16_t        refs;           /* index into refs array of first foreign key in table */
    uint16_t        noblobs;        /* number of blob columns in table */
    uint32_t        maxpgs;         /* maxpgs value */
    uint32_t        maxrows;        /* maxrows (maxslots): circular table only */
    char            filetype;       /* file type: 'n'ormal, inmem: 'v'olatile, 'r'ead, 'p'ersistent */
    int32_t         nullfield;      /* core-level fieldid of sysnulls field */
    uint16_t        nulloffset;     /* offset to sysnulls field */
    uint16_t        nopaths;        /* # of entries in paths array */
    const void     *paths;          /* ptr to array of ACCESS_PATH of the tables that can  
                                       be accessed from this one (init'd at load time) */
} SYSTABLE;

typedef struct syscolumn {
    char            name[NAMELEN];  /* column name */
    SQL_T           data_type;      /* SQL data type (internal) */
    uint16_t        length;         /* length (== field_table.fd_len) */
    uint16_t        offset;         /* offset into record struct of start of field */
    int16_t         typeno;         /* index into types array for data type info */
    int16_t         domno;          /* >= 0 => index into domains array */
    uint16_t        tabno;          /* index into tables array of containing table */
    uint16_t        colno;          /* number of column in table (1st = 0) */
    int32_t         fieldid;        /* core-level field id */
    int32_t         recid;          /* core-level record id */
    int32_t         setid;          /* core-level set id when column used in foreign key */
    uint16_t        pkcolno;        /* column number of corresponding pk field if fk */
    uint16_t        nulls_okay;     /* =1 if nulls allowed, =0 otherwise */
    uint16_t        nokeys;         /* number of keys that contain this column */
    int16_t        *keys;           /* pointer to array of indexes into keys array */
/* The following values are only used by the optimizer */    
    uint64_t        nodistinct;     /* number of distinct values */
    HILO_VALUE      lo;             /* low value */
    HILO_VALUE      hi;             /* high value */
} SYSCOLUMN;

typedef struct syskey {
    char            name[NAMELEN];  /* key name */
    CON_TYPE        type;           /* CT_PRIMARY, CT_UNIQUE, CT_KEY, CT_UNIHASH, CT_PRIHASH*/
    int16_t         on_restrict;    /* = 1 if ref'd by a fk with "on update restrict" */
    int32_t         fieldid;        /* core-level field id of compound key */
    uint16_t        tabno;          /* table in which key is declared */
    uint32_t        pgsize;         /* page size */
    uint16_t        keysz;          /* length of key slot */
    uint16_t        slots;          /* key slots per page */
    uint16_t        nocols;         /* number of columns comprising the key */
    int16_t        *cols;           /* pointer to array of indexes into columns array (< 0 => desc order) */
    uint32_t        maxpgs;         /* maxpgs value */
    uint64_t        hashsize;       /* # keys in hash */
    char            filetype;       /* file type: 'n'ormal, inmem: 'v'olatile, 'r'ead, 'p'ersistent */
} SYSKEY;

typedef struct sysref {
    char            name[NAMELEN];  /* set name */
    int32_t         setid;          /* core-level set id */
    uint16_t        pkeyno;         /* syskey number of referenced primary/unique key */
    uint16_t        otabno;         /* owner table number */
    uint16_t        mtabno;         /* member table number */
    char            on_upd;         /* 'c'ascade, 'r'estrict, set 'n'ull */
    char            on_del;         /* 'c'ascade, 'r'estrict, set 'n'ull */
    uint16_t        nocols;         /* number of columns */
    int16_t        *cols;           /* pointer to array of indexes into columns array of cols */
} SYSREF;

/* database optflags values */
#define DB_NO_NULLS 0x0001          /* NULL-valued columns not used */
#define DB_NON_SQL  0x0002          /* Database is a core (non-SQL) database */

typedef struct sysdb {
    char            ver[RDM_SQLCAT_VERLEN]; /* catalog version string */
    char            name[NAMELEN];  /* database name */
    char            inmem_type;     /* 0, 'v'olatile, 'r'ead, 'p'ersistent */
    uint16_t        dba_size;       /* 0 - not specified, 4 - dba4, 8 - dba8 */
    uint16_t        optflags;       /* see above optflags #define's */
    uint32_t        pagesize;       /* default database page size */
    uint16_t        noints;         /* total no. of needed int16_t values */
    uint16_t        notypes;        /* number of data types (= nocols-nodoms) */
    uint16_t        nodoms;         /* number of domains */
    uint16_t        notabs;         /* number of tables */
    uint16_t        novtabs;        /* number of external tables */
    uint16_t        nocols;         /* total number of columns in all tables */
    uint16_t        nokeys;         /* number of primary/unique/non-unique keys */
    uint16_t        norefs;         /* number of foreign keys */
    SYSTYPE        *types;          /* pointer to systype array */
    SYSDOMAIN      *domains;        /* pointer to sysdomain array */
    SYSTABLE       *tables;         /* pointer to systable array */
    SYSCOLUMN      *columns;        /* pointer to syscolumn array */
    SYSKEY         *keys;           /* pointer to syskey array */
    SYSREF         *refs;           /* pointer to sysref array */
    const void     *coredbd;        /* pointer to core dbd struct */
    uint16_t        coredbdsz;      /* size of core dbd struct */
    void           *coreints;       /* ptr to core DB integer array */
    PSP_MEMTAG      cattag;         /* memory tag on which these cat tables were allocated */
} SYSDB;

/* ------------------------------------------------------------------------------------------
   Legacy catalog types
------------------------------------------------------------------------------------------ */
/* RDM SQL 10.1 version of sysdb (other tables are unchanged in 10.2) */
typedef struct sysdb_10_1 {
    char             name[NAMELEN];  /* database name */
    char             inmem_type;     /* 0, 'v'olatile, 'r'ead, 'p'ersistent */
    int16_t          no_nulls;       /* =1 if nulls are not used in this database */
    uint32_t         pagesize;       /* default database page size */
    uint16_t         noints;         /* total no. of needed int16_t values */
    uint16_t         notypes;        /* number of data types (= nocols-nodoms) */
    uint16_t         nodoms;         /* number of domains */
    uint16_t         notabs;         /* number of tables */
    uint16_t         novtabs;        /* number of external tables */
    uint16_t         nocols;         /* total number of columns in all tables */
    uint16_t         nokeys;         /* number of primary/unique/non-unique keys */
    uint16_t         norefs;         /* number of foreign keys */
    SYSTYPE         *types;          /* pointer to systype array */
    SYSDOMAIN       *domains;        /* pointer to sysdomain array */
    SYSTABLE        *tables;         /* pointer to systable array */
    SYSCOLUMN       *columns;        /* pointer to syscolumn array */
    SYSKEY          *keys;           /* pointer to syskey array */
    SYSREF          *refs;           /* pointer to sysref array */
    const void      *coredbd;        /* pointer to core dbd struct */
    uint16_t         coredbdsz;      /* size of core dbd struct */
    void            *coreints;       /* ptr to core DB integer array */
    PSP_MEMTAG       cattag;         /* memory tag on which these cat tables were allocated */
} SYSDB_10_1;

#endif  /* __SQLCAT_H */
