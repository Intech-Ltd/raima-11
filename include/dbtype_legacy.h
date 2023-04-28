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

#if !defined(DBTYPE_LEGACY_H)
#define DBTYPE_LEGACY_H

/* If INTERNAL_H is defined, then this file is being included only for the
    struct definitions needed to use d_internals().  Therefore, much of the
    stuff in here is not needed and may even cause conflicts with other
    software packages.
    If this file is needed for all the other stuff as well as the struct
    definitions and "internal.h" is also needed, then this file must be
    #include-d BEFORE "internal.h" is #include-d.
*/

/* ---------------------------------------------------------------------------
    Configure the runtime
*/
#define   V3_FILENMLEN  48
#define   V3_MAXDIMS    3
#define   V6_MAXDIMS    3
typedef struct V300_FILE_ENTRY_S
{
    char     v3_ft_name[V3_FILENMLEN];     /* name of file */
    int16_t  v3_ft_desc;                   /* file descriptor */
    char     v3_ft_status;                 /* 'o'=opened, 'c'=closed */
    char     v3_ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow */
    uint16_t v3_ft_slots;                  /* record slots per page */
    uint16_t v3_ft_slsize;                 /* size of record slots in bytes */
    uint16_t v3_ft_pgsize;                 /* size of page */
    uint16_t v3_ft_flags;                  /* e.g. STATIC */
} V300_FILE_ENTRY;

typedef struct V301_FILE_ENTRY_S
{
    char     v3_ft_name[V3_FILENMLEN];     /* name of file */
    int16_t  v3_ft_desc;                   /* file descriptor */
    char     v3_ft_status;                 /* 'o'=opened, 'c'=closed */
    char     v3_ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow */
    uint16_t v3_ft_slots;                  /* record slots per page */
    uint16_t v3_ft_slsize;                 /* size of record slots in bytes */
    uint16_t v3_ft_pgsize;                 /* size of page */
    uint16_t v3_ft_flags;                  /* e.g. STATIC */
    uint16_t v3_ft_pctincrease;            /* % increase of each extension */
    uint32_t v3_ft_initial;                /* initial allocation in pages */
    uint32_t v3_ft_next;                   /* initial extension in pages */
} V301_FILE_ENTRY;

typedef struct V500_FILE_ENTRY_S
{
    char     v5_ft_name[FILENMLEN];        /* name of file */
    int32_t  v5_ft_desc;                   /* file descriptor */
    uint32_t v5_ft_index;                  /* see FILE_ENTRY */
    char     v5_ft_status;                 /* 'o'=opened, 'c'=closed */
    char     v5_ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow */
    uint16_t v5_ft_slots;                  /* record slots per page */
    uint16_t v5_ft_slsize;                 /* size of record slots in bytes */
    uint16_t v5_ft_pgsize;                 /* size of page */
    uint16_t v5_ft_flags;                  /* e.g. STATIC */
    uint16_t v5_ft_pctincrease;            /* % increase of each extension */
    uint32_t v5_ft_initial;                /* initial allocation in pages */
    uint32_t v5_ft_next;                   /* initial extension in pages */
    uint16_t v5_ft_recently_used;
} V500_FILE_ENTRY;

typedef struct U500_FILE_ENTRY_S
{
    wchar_t  v5_ft_name[FILENMLEN];        /* name of file */
    int32_t  v5_ft_desc;                   /* file descriptor */
    uint32_t v5_ft_index;                  /* see FILE_ENTRY */
    char     v5_ft_status;                 /* 'o'=opened, 'c'=closed */
    char     v5_ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow */
    uint16_t v5_ft_slots;                  /* record slots per page */
    uint16_t v5_ft_slsize;                 /* size of record slots in bytes */
    uint16_t v5_ft_pgsize;                 /* size of page */
    uint16_t v5_ft_flags;                  /* e.g. STATIC */
    uint16_t v5_ft_pctincrease;            /* % increase of each extension */
    uint32_t v5_ft_initial;                /* initial allocation in pages */
    uint32_t v5_ft_next;                   /* initial extension in pages */
    uint16_t v5_ft_recently_used;
} U500_FILE_ENTRY;

typedef struct V600_FILE_ENTRY_S
{
    char     v6_ft_name[FILENMLEN];        /* name of file */
    char     v6_ft_type;                   /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */
    uint16_t v6_ft_slots;                  /* record slots per page */
    uint16_t v6_ft_slsize;                 /* size of record slots in bytes */
    uint16_t v6_ft_pgsize;                 /* size of page */
    uint16_t v6_ft_flags;                  /* e.g. STATIC */
    uint16_t v6_ft_pctincrease;            /* % increase of each extension */
    uint32_t v6_ft_initial;                /* initial allocation in pages */
    uint32_t v6_ft_next;                   /* extension in pages */
    uint32_t v6_ft_maxpages;               /* maximum number of pages in a file */
    uint16_t v6_ft_ptroffset;              /* for vardata files: offset from start
                                              of slot to delete chain pointer, if
                                              slot is deleted */
} V600_FILE_ENTRY;

typedef struct U600_FILE_ENTRY_S
{
    wchar_t  v6_ft_name[FILENMLEN];    /* name of file */
    char     v6_ft_type;               /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */
    uint16_t v6_ft_slots;              /* record slots per page */
    uint16_t v6_ft_slsize;             /* size of record slots in bytes */
    uint16_t v6_ft_pgsize;             /* size of page */
    uint16_t v6_ft_flags;              /* e.g. STATIC */
    uint16_t v6_ft_pctincrease;        /* % increase of each extension */
    uint32_t v6_ft_initial;            /* initial allocation in pages */
    uint32_t v6_ft_next;               /* initial extension in pages */
    uint32_t v6_ft_maxpages;           /* maximum number of pages in a file */
    uint16_t v6_ft_ptroffset;          /* for vardata files: offset from start
                                          of slot to delete chain pointer, if
                                          slot is deleted */
} U600_FILE_ENTRY;

typedef struct
{
    char     aa_ft_name[FILENMLEN];  /* name of file */
    char     aa_ft_type;             /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */
    uint16_t aa_ft_slots;            /* record slots per page */
    uint16_t aa_ft_slsize;           /* size of record slots in bytes */
    uint16_t aa_ft_pgsize;           /* size of page */
    uint16_t aa_ft_flags;            /* e.g. STATIC */
    uint16_t aa_ft_pctincrease;      /* % increase of each extension */
    uint32_t aa_ft_initial;          /* initial allocation in pages */
    uint32_t aa_ft_next;             /* extension in pages */
    F_ADDR4  aa_ft_maxpages;         /* maximum number of pages in a file */
    F_ADDR4  aa_ft_maxslots;         /* maximum number of slots in a file */
    uint16_t aa_ft_ptroffset;        /* for vardata files: offset from start
                                        of slot to delete chain pointer, if
                                        slot is deleted */
} AA_FILE_ENTRY_DBA4;

typedef struct
{
    char     aa_ft_name[FILENMLEN];  /* name of file */
    char     aa_ft_type;             /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */
    uint16_t aa_ft_slots;            /* record slots per page */
    uint16_t aa_ft_slsize;           /* size of record slots in bytes */
    uint16_t aa_ft_pgsize;           /* size of page */
    uint16_t aa_ft_flags;            /* e.g. STATIC */
    uint16_t aa_ft_pctincrease;      /* % increase of each extension */
    uint32_t aa_ft_initial;          /* initial allocation in pages */
    uint32_t aa_ft_next;             /* extension in pages */
    F_ADDR8  aa_ft_maxpages;         /* maximum number of pages in a file */
    F_ADDR8  aa_ft_maxslots;         /* maximum number of slots in a file */
    uint16_t aa_ft_ptroffset;        /* for vardata files: offset from start
                                        of slot to delete chain pointer, if
                                        slot is deleted */
} AA_FILE_ENTRY_DBA8;

typedef struct AA_FILE_ENTRY_UNICODE_DBA4_S
{
    wchar_t  aa_ft_name[FILENMLEN];    /* name of file */
    char     aa_ft_type;               /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */
    uint16_t aa_ft_slots;              /* record slots per page */
    uint16_t aa_ft_slsize;             /* size of record slots in bytes */
    uint16_t aa_ft_pgsize;             /* size of page */
    uint16_t aa_ft_flags;              /* e.g. STATIC */
    uint16_t aa_ft_pctincrease;        /* % increase of each extension */
    uint32_t aa_ft_initial;            /* initial allocation in pages */
    uint32_t aa_ft_next;               /* initial extension in pages */
    F_ADDR4  aa_ft_maxpages;           /* maximum number of pages in a file */
    F_ADDR4  aa_ft_maxslots;           /* maximum number of slots in a file */
    uint16_t aa_ft_ptroffset;          /* for vardata files: offset from start
                                          of slot to delete chain pointer, if
                                          slot is deleted */
} AA_FILE_ENTRY_UNICODE_DBA4;

typedef struct AA_FILE_ENTRY_UNICODE_DBA8_S
{
    wchar_t  aa_ft_name[FILENMLEN];    /* name of file */
    char     aa_ft_type;               /* 'd'=data, 'k'=key, 'o'=overflow, 'v'=vardata */
    uint16_t aa_ft_slots;              /* record slots per page */
    uint16_t aa_ft_slsize;             /* size of record slots in bytes */
    uint16_t aa_ft_pgsize;             /* size of page */
    uint16_t aa_ft_flags;              /* e.g. STATIC */
    uint16_t aa_ft_pctincrease;        /* % increase of each extension */
    uint32_t aa_ft_initial;            /* initial allocation in pages */
    uint32_t aa_ft_next;               /* initial extension in pages */
    F_ADDR8  aa_ft_maxpages;           /* maximum number of pages in a file */
    F_ADDR8  aa_ft_maxslots;           /* maximum number of slots in a file */
    uint16_t aa_ft_ptroffset;          /* for vardata files: offset from start
                                          of slot to delete chain pointer, if
                                          slot is deleted */
} AA_FILE_ENTRY_UNICODE_DBA8;

typedef struct
{
    char     ab_ft_name[FILENMLEN];  /* name of file */
    F_ADDR8  ab_ft_maxpages;         /* maximum number of pages in a file */
    F_ADDR8  ab_ft_maxslots;         /* maximum number of slots in a file */
    uint32_t ab_ft_initial;          /* initial allocation in pages */
    uint32_t ab_ft_next;             /* extension in pages */
    uint16_t ab_ft_slots;            /* record slots per page */
    uint16_t ab_ft_slsize;           /* size of record slots in bytes */
    uint16_t ab_ft_pgsize;           /* size of page */
    uint16_t ab_ft_flags;            /* e.g. STATIC */
    uint16_t ab_ft_pctincrease;      /* % increase of each extension */
    uint16_t ab_ft_ptroffset;        /* for vardata files: offset from start
                                        of slot to delete chain pointer, if
                                        slot is deleted */
    char     ab_ft_type;             /* 'd'ata, 'k'ey, 'v'ardata, 'b'lob, or 'h'ash */
} AB_FILE_ENTRY;

typedef struct V300_RECORD_ENTRY_S
{
    uint16_t v3_rt_file;   /* file table entry of file containing record */
    uint16_t v3_rt_len;    /* total length of record */
    uint16_t v3_rt_data;   /* offset to start of data in record */
    uint16_t v3_rt_fields; /* first field def in task->field_table */
    uint16_t v3_rt_fdtot;  /* total number of fields in record */
    uint16_t v3_rt_flags;
} V300_RECORD_ENTRY;

typedef struct V600_RECORD_ENTRY_S
{
    uint16_t v6_rt_file;    /* file table entry of file containing record */
    uint16_t v6_rt_len;     /* total length of record */
    uint16_t v6_rt_data;    /* offset to start of data in record */
    uint16_t v6_rt_fields;  /* first field def in task->field_table */
    uint16_t v6_rt_fdtot;   /* total number of fields in record */
    uint16_t v6_rt_flags;
    uint16_t v6_rt_len_ex;  /* length of record in external format, i.e.
                               with all varchar fields expanded to full
                               size - same as v6_rt_len except in records
                               with varchar fields. */
} V600_RECORD_ENTRY;

typedef struct V300_FIELD_ENTRY_S
{
    char     v3_fd_key;          /* see the #defines just above */
    char     v3_fd_type;         /* see the #defines just above */
    uint16_t v3_fd_len;          /* length of field in bytes */
    uint16_t v3_fd_dim[V3_MAXDIMS]; /* size of each array dimension */
    uint16_t v3_fd_keyfile;      /* task->file_table entry for key file */
    uint16_t v3_fd_keyno;        /* key prefix number */
    uint16_t v3_fd_ptr;          /* offset to field in record or 1st compound key field in task->key_table */
    uint16_t v3_fd_rec;          /* record table entry of record containing field */
    uint16_t v3_fd_flags;        /* see the #defines at the top of the file */
} V300_FIELD_ENTRY;

typedef struct V600_FIELD_ENTRY_S
{
    char     v6_fd_key;           /* see the #defines just above */
    char     v6_fd_type;          /* see the #defines just above */
    uint16_t v6_fd_len;           /* length of field in bytes */
    uint16_t v6_fd_dim[V6_MAXDIMS];  /* size of each array dimension */
    uint16_t v6_fd_keyfile;       /* task->file_table entry for key file */
    uint16_t v6_fd_keyno;         /* key prefix number */
    uint16_t v6_fd_ptr;           /* offset to field in record or 1st compound key field in task->key_table */
    uint16_t v6_fd_rec;           /* record table entry of record containing field */
    uint16_t v6_fd_flags;         /* see the #defines at the top of the file */
    uint16_t v6_fd_extfile;       /* file_table entry for vardata file */
    uint16_t v6_fd_varmax;        /* maximum size in bytes of varchar data
                                     excluding null terminator, which is not stored */
    uint16_t v6_fd_nvptrs;        /* number of pointers to slots in vardata file */
    uint16_t v6_fd_len_ex;        /* length of field in bytes, in external structures passed into runtime
                                     library, i.e. with varchar fields expanded to full size. (same as
                                     fd_len except for varchar fields) */
    uint16_t v6_fd_ptr_ex;        /* offset to field in bytes, in external structures passed into runtime lib
                                     (same as fd_ptr except for fields in records which contain varchars) */
    uint16_t v6_fd_keylen;        /* length of key value stored in B-tree, same as fd_len except for
                                     partial keys - only applies to strings and binary char arrays */
} V600_FIELD_ENTRY;

/* compound key table entry declaration */
typedef struct V300_KEY_ENTRY_S
{
    uint16_t v3_kt_key;        /* compound key field number */
    uint16_t v3_kt_field;      /* field number of included field */
    uint16_t v3_kt_ptr;        /* offset to start of field data in key */
    int16_t  v3_kt_sort;       /* 'a' = ascending, 'd' = descending */
} V300_KEY_ENTRY;

typedef struct V600_KEY_ENTRY_S
{
    uint16_t v6_kt_key;        /* compound key field number */
    uint16_t v6_kt_field;      /* field number of included field */
    uint16_t v6_kt_ptr;        /* offset to start of field data in key,
                                  as stored in database, which may contain
                                  partial strings */
    uint16_t v6_kt_len;        /* length of field data in key, as stored
                                  in database - different from original
                                  field length for varchar and partial
                                  strings */
    uint16_t v6_kt_ptr_ex;     /* offset to start of field data in
                                  external structure passed in from
                                  application, which contains full
                                  strings only */
    uint16_t v6_kt_len_ex;     /* length of field data in external structure
                                  passed in from application - must be same
                                  as fd_len_ex in field definition */
    int16_t  v6_kt_sort;       /* 'a' = ascending, 'd' = descending */
} V600_KEY_ENTRY;

typedef struct AA_DICT_SIZE_S
{
    uint16_t aa_Page_size;
    uint16_t aa_Size_ft;
    uint16_t aa_Size_rt;
    uint16_t aa_Size_fd;
    uint16_t aa_Size_st;
    uint16_t aa_Size_mt;
    uint16_t aa_Size_srt;
    uint16_t aa_Size_kt;
} AA_DICT_SIZE;

#endif /* DBTYPE_LEGACY_H */

