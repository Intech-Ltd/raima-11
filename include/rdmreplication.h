/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2007-2009 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#ifndef RDMREPLICATION_H
#define RDMREPLICATION_H

#include "psp.h"
#include "rdm.h"
#include "rdmtype.h"

typedef enum REPLOG_ENTRY_TYPE_E
{
    RLE_OPTIONS,
    RLE_FILLNEW,
    RLE_MAKENEW,
    RLE_RECWRITE,
    RLE_CRWRITE,
    RLE_CSOWRITE,
    RLE_CSMWRITE,
    RLE_CONNECT,
    RLE_DISCON,
    RLE_DELETE,
    RLE_DISDEL,
    RLE_KEYSTORE,
    RLE_KEYDEL,
    RLE_BLOBWRITE,
    RLE_BLOBTRUNC,
    RLE_INITFILE
} REPLOG_ENTRY_TYPE;

typedef struct replog_file_header {
    char     ver[DBD_COMPAT_LEN];
    uint16_t rec_bm_size;
    uint16_t set_bm_size;
    DB_ULONG task_options;
} REPLOG_FILE_HEADER;

typedef struct replog_entry {
    uint32_t           size;
    REPLOG_ENTRY_TYPE  type;
} REPLOG_ENTRY;

typedef struct replog_entry_options {
    uint32_t      options;
    int32_t       mode;
} REPLOG_ENTRY_OPTIONS;

typedef struct replog_entry_fillnew {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    uint32_t  dummy;
} REPLOG_ENTRY_FILLNEW;

typedef struct replog_entry_makenew {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    uint32_t  dummy;
} REPLOG_ENTRY_MAKENEW;

typedef struct replog_entry_recwrite {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    uint32_t  dummy;
} REPLOG_ENTRY_RECWRITE;

typedef struct replog_entry_fieldwrite {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    int32_t   field;
} REPLOG_ENTRY_FIELDWRITE;

typedef struct replog_entry_connect {
    int32_t   setnum;
    uint16_t  orectype;
    uint16_t  mrectype;
    uint32_t  dummy;
    DB_ADDR8  owner_dba8;
    DB_ADDR8  member_dba8;
    DB_ADDR8  previous_dba8;
} REPLOG_ENTRY_CONNECT;

typedef struct replog_entry_discon {
    int32_t   setnum;
    uint16_t  orectype;
    uint16_t  mrectype;
    DB_ADDR8  owner_dba8;
    DB_ADDR8  member_dba8;
} REPLOG_ENTRY_DISCON;

typedef struct replog_entry_delete {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    uint32_t  dummy;
} REPLOG_ENTRY_DELETE;

typedef struct replog_entry_disdel {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    uint32_t  dummy;
} REPLOG_ENTRY_DISDEL;

typedef struct replog_entry_keystore {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    int32_t   field;
} REPLOG_ENTRY_KEYSTORE;

typedef struct replog_entry_keydel {
    DB_ADDR8  dba8;
    uint16_t  rectype;
    int32_t   field;
} REPLOG_ENTRY_KEYDEL;

typedef struct replog_entry_blobwrite {
    DB_ADDR8  dba8;
    int32_t   field;
    uint32_t  pos;
} REPLOG_ENTRY_BLOBWRITE;

typedef struct replog_entry_blobtrunc {
    DB_ADDR8  dba8;
    int32_t   field;
    uint32_t  pos;
} REPLOG_ENTRY_BLOBTRUNC;

typedef struct replog_entry_initfile {
    FILE_NO fno;
} REPLOG_ENTRY_INITFILE;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t log_fillnew(
    const RECORD_ENTRY *rt,
    uint16_t            size,
    const DB_ADDR      *dba,
    const void         *data,
    const DB_ENTRY     *db);

int32_t log_makenew(
    const RECORD_ENTRY *rt,
    const DB_ADDR      *dba,
    const DB_ENTRY     *db);

int32_t log_fieldwrite(
    REPLOG_ENTRY_TYPE  rle_type,
    const FIELD_ENTRY *fd,
    uint16_t           size,
    const DB_ADDR     *dba,
    const void        *data,
    const DB_ENTRY    *db);

int32_t log_recwrite(
    const RECORD_ENTRY *rt,
    uint16_t            size,
    const DB_ADDR      *dba,
    const void         *data,
    const DB_ENTRY     *db);

int32_t log_options(
    uint16_t        uno,
    uint32_t        options,
    int32_t         mode,
    const DB_ENTRY *db);

int32_t log_connect(
    const SET_ENTRY *st,
    const DB_ADDR   *owner_dba,
    const DB_ADDR   *member_dba,
    uint16_t         mrecndx,
    const DB_ADDR   *previous_dba,
    const DB_ENTRY  *db);

int32_t log_discon(
    const SET_ENTRY *st,
    const DB_ADDR   *owner_dba,
    const DB_ADDR   *member_dba,
    uint16_t         mrecndx,
    const DB_ENTRY  *db);

int32_t log_delete(
    const RECORD_ENTRY *rt,
    const DB_ADDR      *dba,
    const DB_ENTRY     *db);

int32_t log_disdel(
    const RECORD_ENTRY *rt,
    const DB_ADDR      *dba,
    const DB_ENTRY     *db);

int32_t log_keystore(
    const FIELD_ENTRY *fd,
    const DB_ADDR     *dba,
    const DB_ENTRY    *db);

int32_t log_keydel(
    const FIELD_ENTRY *fd,
    const DB_ADDR     *dba,
    const DB_ENTRY    *db);

int32_t log_blobwrite(
    const FIELD_ENTRY *fd,
    const void        *data,
    uint32_t           size,
    uint32_t           pos,
    const DB_ADDR     *dba,
    const DB_ENTRY    *db);

int32_t log_blobdelete(
    const FIELD_ENTRY *fd,
    const DB_ADDR     *dba,
    const DB_ENTRY    *db);

int32_t log_blobtrunc(
    const FIELD_ENTRY *fd,
    uint32_t           pos,
    const DB_ADDR     *dba,
    const DB_ENTRY    *db);

int32_t log_trmark(
    const DB_INSTANCE *dbi,
    TRMARK            *trmark);
    
int32_t log_trdeletemark(
    DB_INSTANCE *dbi,
    TRMARK      *trmark);
    
int32_t log_trrollback(
    DB_INSTANCE       *dbi,
    const TRMARK      *trmark);
    
int32_t log_initfile(
    FILE_NO         fno,
    const DB_ENTRY *db);

int32_t finalize_replogs(
    const DB_TASK *task);

void abort_replogs(
    const DB_TASK *task,
    int32_t        dbn);

void abort_replogs_db(
    const DB_ENTRY *);

int32_t initialize_replication(
    DB_TASK   *task);

void cleanup_replication(
    const DB_ENTRY *dB);

int32_t set_replication_rec_lock_bit(
    uint16_t        uno,
    DB_ULONG        item,
    int32_t         type,
    const DB_ENTRY *db);

int32_t set_replication_set_lock_bit(
    uint16_t        uno,
    DB_ULONG        item,
    int32_t         type,
    const DB_ENTRY *db);

int32_t EXTERNAL_FCN process_replog(
    const char *log_name,
    DB_TASK    *task,
    int32_t     dbn,
    uint16_t    xflags);

int32_t EXTERNAL_FCN initialize_replog_read(
    uint16_t             uno,
    const char          *log_name,
    uint16_t             xflags,
    REPLOG_FILE_HEADER  *header,
    uint8_t            **rec_writelock_bm,
    uint8_t            **set_writelock_bm,
    PSP_FILE_OFFSET     *log_offset,
    TX_FILE             *log_file,
    const DB_ENTRY      *db);

int32_t EXTERNAL_FCN process_replog_entries(
    uint16_t         uno,
    PSP_FILE_OFFSET *log_offset,
    TX_FILE          log_file,
    const DB_ENTRY  *db);

#ifdef __cplusplus
}
#endif
#endif /* RDMREPLICATION_H */


