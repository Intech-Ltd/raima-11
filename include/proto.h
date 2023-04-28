/* ----------------------------------------------------------------------------
 * Raima Database Manager
 *
 * Copyright (c) 2001-2008 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */

#if !defined(PROTO_H)
#define PROTO_H

#if defined(__cplusplus)
extern "C" {
#endif

/* From dberr.c */
int32_t EXTERNAL_FCN dberr_msg(char *, uint32_t, const DB_TASK *);

int32_t EXTERNAL_FCN _dberr(int32_t, const char *, int32_t, DB_TASK *); /*lint -esym(534,_dberr) */
#define dberr(n)   _dberr(n, __FILE__, __LINE__, db->task)
#define taskerr(n) _dberr(n, __FILE__, __LINE__, task)

/* From alloc.c: */

/* From currency.c: */
int32_t              _cmtype(const DB_ADDR *, uint16_t *, const DB_ENTRY *);
int32_t              _crtype(uint16_t *, DB_ENTRY *);

/* From dberr.c: */
void              dbautorec(DB_TASK *);

/* From lockfcns.c: */
#if defined(RDM_HAS_XLOCKS)
int32_t              free_dblocks(int32_t, const DB_TASK *);
#else
int32_t              free_dblocks(const DB_TASK *);
#endif

void                 termfree(DB_ENTRY *);
int32_t              neterr(void);

int32_t              dupid_check(void);

#if defined(DBSTAT)
/* From dbstat.c: */
void sync_MEM_STATS(MEM_STATS *, MEM_STATS *);
void sync_CACHE_STATS(CACHE_STATS *, CACHE_STATS *);

void              STAT_mem_alloc(CACHE *cache, size_t size);
void              STAT_lookups(FILE_NO fno, const DB_INSTANCE *);
void              STAT_hits(FILE_NO fno, const DB_INSTANCE *);
void              STAT_pages(FILE_NO fno, int16_t num, const DB_INSTANCE *);
void              STAT_file_open(FILE_NO fno, const DB_INSTANCE *);
void              STAT_pz_read(FILE_NO fno, size_t amt, const DB_INSTANCE *);
void              STAT_pz_write(FILE_NO fno, size_t amt, const DB_INSTANCE *);
void              STAT_pg_read(FILE_NO fno, size_t amt, const DB_INSTANCE *);
void              STAT_pg_write(FILE_NO fno, size_t amt, const DB_INSTANCE *);
void              STAT_new_page(FILE_NO fno, const DB_INSTANCE *);
void              STAT_log_open(DB_TASK *);
void              STAT_log_read(size_t amt, DB_TASK *);
void              STAT_log_write(size_t amt, DB_TASK *);
void              STAT_dbl_open(DB_TASK *);
void              STAT_dbl_read(size_t amt, DB_TASK *);
void              STAT_dbl_write(size_t amt, DB_TASK *);
void              STAT_max_open(int32_t num, DB_TASK *);
void              STAT_trbegin(DB_TASK *);
void              STAT_trend(DB_TASK *);
void              STAT_trprecommit(DB_TASK *);
void              STAT_trabort(DB_TASK *);
void              STAT_lock(FILE_NO fno, int32_t type, const DB_INSTANCE *);


/* Constants for use by STAT_lock() - they are offsets into the
    LOCK_STATS structure
*/
#define STAT_LOCK_r       0   /* r = read lock */
#define STAT_LOCK_w       1   /* w = write lock */
#define STAT_LOCK_x       2   /* x = exclusive lock */
#define STAT_LOCK_r2w     3
#define STAT_LOCK_r2x     4
#define STAT_LOCK_w2r     5
#define STAT_LOCK_x2r     6
#define STAT_FREE_r       7
#define STAT_FREE_w       8
#define STAT_FREE_x       9

#else
#define sync_MEM_STATS(a, b) /**/
#define sync_CACHE_STATS(a, b) /**/

#define STAT_mem_alloc(a, b) /**/
#define STAT_lookups(a, b) /**/
#define STAT_hits(a, b) /**/
#define STAT_pages(a, b, c) /**/
#define STAT_file_open(a, b) /**/
#define STAT_pz_read(a, b, c) /**/
#define STAT_pz_write(a, b, c) /**/
#define STAT_pg_read(a, b, c) /**/
#define STAT_pg_write(a, b, c) /**/
#define STAT_rlb_read(a, b, c) /**/
#define STAT_rlb_write(a, b, c) /**/
#define STAT_new_page(a, b) /**/
#define STAT_log_open(a) /**/
#define STAT_log_read(a, b) /**/
#define STAT_log_write(a, b) /**/
#define STAT_dbl_open(a) /**/
#define STAT_dbl_read(a, b) /**/
#define STAT_dbl_write(a, b) /**/
#define STAT_max_open(a, b) /**/
#define STAT_trbegin(a) /**/
#define STAT_trend(a) /**/
#define STAT_trprecommit(a) /**/
#define STAT_trabort(a) /**/
#define STAT_lock(a, b, c) /**/
#define STAT_send_msg(a, b, c, d) /**/
#define STAT_recv_msg(a, b, c, d) /**/
#endif   /* DBSTAT */

/* From blobs.c */
int32_t              blob_open(const DB_TASK *);
void                 blob_reset(const DB_ENTRY *, FILE_NO);

/* From connect.c */
int32_t EXTERNAL_FCN sortcmp(const SET_ENTRY *, const char *, const char *, const DB_ENTRY *);
/* From dio.c: */
int32_t EXTERNAL_FCN dio_open(FILE_NO, const DB_INSTANCE *);
int32_t              dio_closelru(DB_TASK *);
int32_t EXTERNAL_FCN dio_close(FILE_NO, const DB_INSTANCE *);
void                 dio_delete_mark(const TRMARK *, const DB_TASK *);
int32_t              dio_init(DB_TASK *);
void                 dio_free(const DB_TASK *);
void                 dio_cleanup_mark(const TRMARK *, const DB_TASK *);
void    EXTERNAL_FCN dio_clear_db(const DB_ENTRY *);
void    EXTERNAL_FCN dio_clear(const DB_TASK *);
int32_t              dio_clrfile(FILE_NO, const DB_INSTANCE *);

int32_t EXTERNAL_FCN dio_blobget(FILE_NO, F_ADDR, char **, int32_t, void **, const DB_INSTANCE *);
int32_t              dio_in(const PAGE_ENTRY *, const DB_INSTANCE *);
void                 dio_ixclear(DB_TASK *);
int32_t EXTERNAL_FCN dio_flush(DB_TASK *);
int32_t EXTERNAL_FCN dio_flush_precommit(DB_TASK *);
int32_t EXTERNAL_FCN dio_flush_commit(DB_TASK *);
int32_t EXTERNAL_FCN dio_keyget(FILE_NO, F_ADDR, char **, int32_t, void **, const DB_INSTANCE *);
int32_t              dio_writeable(const void *);
int32_t EXTERNAL_FCN dio_unget(void *); /*lint -esym(534,dio_unget) */
int32_t EXTERNAL_FCN dio_read(const DB_ADDR *, char **, int32_t, void **, const DB_ENTRY *);
int32_t EXTERNAL_FCN dio_write(void *); /*lint -esym(534,dio_write) */
int32_t EXTERNAL_FCN dio_release(void *); /*lint -esym(534,dio_release) */
int32_t EXTERNAL_FCN dio_read_slot(FILE_NO, F_ADDR, char **, int32_t, void **, const DB_INSTANCE *);
int32_t              dio_releasepg(void *, int32_t, int32_t); /*lint -esym(534,dio_releasepg) */

int32_t EXTERNAL_FCN dio_pzread(FILE_NO, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzalloc(FILE_NO, F_ADDR *, int16_t *, const RECORD_ENTRY *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzdel(FILE_NO, F_ADDR, F_ADDR, const RECORD_ENTRY *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzgetcircnext(FILE_NO, F_ADDR *, const DB_INSTANCE *);
void                 dio_pzdbclr(const DB_ENTRY *);
PSP_FILE_OFFSET EXTERNAL_FCN dio_filesize(FILE_NO, const DB_INSTANCE *);
int32_t         EXTERNAL_FCN dio_truncate(FILE_NO, PSP_FILE_OFFSET, const DB_INSTANCE *);

int32_t EXTERNAL_FCN dio_readfile(FILE_NO, PSP_FILE_OFFSET, void *, uint32_t, DB_ULONG, int32_t *, DB_TASK *);

/* From inittab.c: */
int32_t              dbd_check_version(char *, int32_t *, int8_t *, int8_t *, int8_t *);
void              copy_filename(char *, const char *, uint32_t);

/* From keyfcns.c: */
#define KEY_FORWARD  (1)
#define KEY_REVERSE (-1)
int32_t EXTERNAL_FCN key_open(const DB_TASK *);
void    EXTERNAL_FCN key_close(const DB_ENTRY *);
int32_t EXTERNAL_FCN key_init(uint32_t, const FIELD_ENTRY **, const DB_ENTRY *);
void                 key_reset(FILE_NO, const DB_ENTRY *);
int32_t EXTERNAL_FCN key_locate(const FIELD_ENTRY *, const void *, const DB_ADDR *, DB_ADDR *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN key_scan(KEY_INST_INFO *, int32_t);
int32_t EXTERNAL_FCN key_boundary(KEY_INST_INFO *, int32_t);
int32_t EXTERNAL_FCN key_insert(const FIELD_ENTRY *, const char *, const DB_ADDR *, const DB_INSTANCE *);
int32_t              key_delete(const FIELD_ENTRY *, const char *, const DB_ADDR *, const DB_INSTANCE *);

int32_t EXTERNAL_FCN key_bldcom(const FIELD_ENTRY *, const char *, int32_t, char *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN key_bldgrp(const FIELD_ENTRY *, const char *, int32_t, char *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN hash_insert(const FIELD_ENTRY *, const char *, const DB_ADDR *, const DB_INSTANCE *);
int32_t              hash_delete(const FIELD_ENTRY *, const char *, const DB_ADDR *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN hash_locate(const FIELD_ENTRY *, const void *, DB_ADDR *, const DB_INSTANCE *);
void EXTERNAL_FCN    get_hash_prev(F_ADDR *, const KEY_NODE *, const FILE_ENTRY *, const DB_ENTRY *);
void EXTERNAL_FCN    get_hash_next(F_ADDR *, const KEY_NODE *, const FILE_ENTRY *, const DB_ENTRY *);

/* From libfcns.c: */
int32_t EXTERNAL_FCN fldcmp(const FIELD_ENTRY *, const FIELD_DATA *, const FIELD_DATA *, const DB_ENTRY *);
FLDCMP_FCN           fldcmp_fcn(const FIELD_ENTRY *, const DB_ENTRY *);
int32_t              uint16_cmp(uint16_t, const void *);
int32_t              null_uno_dba(const DB_ADDR *);
int32_t              check_dba(const DB_ADDR *, const DB_ENTRY *);
int32_t EXTERNAL_FCN check_dbfiles_w_locks(const DB_ENTRY *);
int32_t EXTERNAL_FCN check_transaction_type(TRANS_TYPE, DB_TASK *);
int32_t EXTERNAL_FCN DBAcmp(const DB_ADDR *, const DB_ADDR *);
int32_t EXTERNAL_FCN dio_pzgetdchain(FILE_NO, F_ADDR *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzgetnext(FILE_NO, F_ADDR *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzgettotal(FILE_NO, uint16_t, F_ADDR *, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzsetdchain(FILE_NO, F_ADDR, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzsetnext(FILE_NO, F_ADDR, const DB_INSTANCE *);
int32_t EXTERNAL_FCN dio_pzsettotal(FILE_NO, uint16_t, F_ADDR, const DB_INSTANCE *);
void EXTERNAL_FCN MemPtr4ToUnoMemPtr(const MEM_PTR4 *, MEM_PTR *, uint16_t);
void EXTERNAL_FCN MemPtr8ToUnoMemPtr(const MEM_PTR8 *, MEM_PTR *, uint16_t);
void EXTERNAL_FCN UnoMemPtrToMemPtr4(const MEM_PTR *, MEM_PTR4 *);
void EXTERNAL_FCN UnoMemPtrToMemPtr8(const MEM_PTR *, MEM_PTR8 *);
void EXTERNAL_FCN SetPtr4ToUnoSetPtr(const SET_PTR4 *, SET_PTR *, uint16_t);
void EXTERNAL_FCN SetPtr8ToUnoSetPtr(const SET_PTR8 *, SET_PTR *, uint16_t);
void EXTERNAL_FCN UnoSetPtrToSetPtr4(const SET_PTR *, SET_PTR4 *);
void EXTERNAL_FCN UnoSetPtrToSetPtr8(const SET_PTR *, SET_PTR8 *);
void EXTERNAL_FCN DbAddr48ToDbAddr(const void *, DB_ADDR *, uint16_t, const DB_ENTRY *);
void EXTERNAL_FCN DbAddr4ToDbAddr(const DB_ADDR4 *, DB_ADDR *, uint16_t);
void EXTERNAL_FCN DbAddr8ToDbAddr(const DB_ADDR8 *, DB_ADDR *, uint16_t);

void EXTERNAL_FCN DbAddrToDbAddr48(const DB_ADDR *, void *, const DB_ENTRY *);
void EXTERNAL_FCN DbAddrToDbAddr4(const DB_ADDR *, DB_ADDR4 *);
void EXTERNAL_FCN DbAddrToDbAddr8(const DB_ADDR *, DB_ADDR8 *);

void EXTERNAL_FCN get_uno_set_ptr(SET_PTR *, const void *, uint16_t, const DB_ENTRY *);
void EXTERNAL_FCN get_uno_set_ptr4(SET_PTR *, const SET_PTR4 *, uint16_t);
void EXTERNAL_FCN get_uno_set_ptr8(SET_PTR *, const SET_PTR8 *, uint16_t);

void EXTERNAL_FCN put_uno_set_ptr(void *, const SET_PTR *, const DB_ENTRY *);

void EXTERNAL_FCN get_uno_mem_ptr(MEM_PTR *, const void *, uint16_t, const DB_ENTRY *);
void EXTERNAL_FCN get_uno_mem_ptr4(MEM_PTR *, const void *, uint16_t);
void EXTERNAL_FCN get_uno_mem_ptr8(MEM_PTR *, const void *, uint16_t);

void EXTERNAL_FCN put_uno_mem_ptr(void *, const MEM_PTR *, const DB_ENTRY *);

void EXTERNAL_FCN get_key_slot(KEY_SLOT *, const char *, uint16_t, const DB_ENTRY *);
void EXTERNAL_FCN get_key_slot_ptr(KEY_SLOT_PTR *, const char *, const DB_ENTRY *);

void EXTERNAL_FCN get_vaddr(V_ADDR *, const void *, const DB_ENTRY *);
void EXTERNAL_FCN put_vaddr(void *, V_ADDR, const DB_ENTRY *);

void EXTERNAL_FCN get_faddr(F_ADDR *, const void *, const DB_ENTRY *);
void EXTERNAL_FCN put_faddr(void *, F_ADDR, const DB_ENTRY *);

char   *vstrnzcpy(char *, const char *, size_t);

int32_t EXTERNAL_FCN          nfld_check(int32_t, const FIELD_ENTRY **, const DB_ENTRY *);
int32_t EXTERNAL_FCN          nkey_check(int32_t, const FIELD_ENTRY **, const DB_ENTRY *);
int32_t EXTERNAL_FCN          nset_check(int32_t, const SET_ENTRY **, const DB_ENTRY *);
int32_t EXTERNAL_FCN          nhashorkey_check( int32_t, const FIELD_ENTRY **, const DB_ENTRY *);
int32_t EXTERNAL_FCN          nrec_check(int32_t, const RECORD_ENTRY **, const DB_ENTRY *);
const KEY_ENTRY *EXTERNAL_FCN get_next_key_field(const FIELD_ENTRY *, const KEY_ENTRY *, const DB_DBD *);

/* From makenew.c: */
void                 rdm_sk_free(DB_ENTRY *);

/* From opens.c: */
PSP_FH  EXTERNAL_FCN open_b(const char *, uint32_t, uint16_t, DB_TASK *);
void    EXTERNAL_FCN commit_file(PSP_FH, const DB_TASK *);

/* From recfcns.c: */
int32_t              r_chkfld(const FIELD_ENTRY *, char *, const DB_ADDR *, const char *, DB_ENTRY *);
int32_t              r_delrec(const RECORD_ENTRY *, const DB_ADDR *, DB_ENTRY *);
int32_t              r_del_keys_vardata(const RECORD_ENTRY *, char *, const DB_ADDR *, DB_ENTRY *);
int32_t EXTERNAL_FCN r_gfld(const FIELD_ENTRY *, const char *, uint32_t, char *, const DB_INSTANCE *);
int32_t              r_gvarfld(const FIELD_ENTRY *, const char *, char *, const DB_INSTANCE *);
int32_t              r_gvarelt(const FIELD_ENTRY *, const char *, char *, const DB_INSTANCE *);
int32_t              r_pfld(const FIELD_ENTRY *, char *, const char *, const DB_ADDR *, DB_INSTANCE *);
int32_t              r_pvarfld(const FIELD_ENTRY *, char *, const char *, const DB_ADDR *, const DB_ENTRY *);
int32_t              r_pmem(const SET_ENTRY *, char *, const MEM_PTR *, const DB_ENTRY *);
int32_t              r_pset(const SET_ENTRY *, char *, const SET_PTR *, const DB_ENTRY *);
int32_t EXTERNAL_FCN r_optkey(int32_t, const FIELD_ENTRY *, char *, const DB_ENTRY *);

/* From recs.c */
int32_t rec_search(int32_t, const RECORD_ENTRY *, const DB_INSTANCE *);

/* From pathfcns.c */
int32_t EXTERNAL_FCN con_dbd(char *, const char *, char *, DB_TASK *);
int32_t EXTERNAL_FCN con_dbf(char *, const char *, const char *, char *, DB_TASK *);
char   *EXTERNAL_FCN get_element(const char *, int32_t);

/* From rdmreplication.c: */
void EXTERNAL_FCN get_final_replog_name(char *, DB_ULONG, const DB_TASK *);

/* From enter.c: */
int32_t              db_enter(const char *, int32_t, DB_TASK *, DB_ENTRY **);
int32_t              db_exit(int32_t, DB_TASK *);
int32_t              dh_enter(const char *, RDM_DB, DB_ENTRY **);
int32_t              dh_exit(int32_t, const DB_ENTRY *);

/* From alloc.c */
DB_BOOLEAN               ll_access(llist *);
int32_t                  ll_append(llist *, char *, DB_TASK *);
void                     ll_deaccess(llist *);
void                     ll_free(llist *);
int32_t                  ll_prepend(llist *, char *, DB_TASK *);
char       *             ll_first(llist *);
char       *             ll_next(llist *);

/* Internal functions called by d_ functions */
int32_t              dblobdelete(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dblobread(const FIELD_ENTRY *, void *, uint32_t, uint32_t *, const DB_ENTRY *);
int32_t              dblobseek(const FIELD_ENTRY *, uint32_t, SEEK_TYPE, const DB_ENTRY *);
int32_t              dblobsize(const FIELD_ENTRY *, uint32_t *, const DB_ENTRY *);
int32_t              dblobtell(const FIELD_ENTRY *, uint32_t *, const DB_ENTRY *);
int32_t              dblobtruncate(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dblobwrite(const FIELD_ENTRY *, const void *, uint32_t, DB_ENTRY *);
int32_t              dclose(const DB_TASK *);
int32_t              dcloseall(const DB_TASK *);
int32_t              dcmtype(const SET_ENTRY *, int32_t *, const DB_ENTRY *);
int32_t              dconnect(const SET_ENTRY *, const DB_ENTRY *);
int32_t              dcotype(const SET_ENTRY *, int32_t *, const DB_ENTRY *);
int32_t              dcrget(DB_ADDR *, const DB_ENTRY *);
int32_t              dcrread(const FIELD_ENTRY *, void *, const DB_ENTRY *);
int32_t              dcrset(const DB_ADDR *, int16_t, DB_ENTRY *);
int32_t              dcrtype(int32_t *, DB_ENTRY *);
int32_t              dcrwrite(const FIELD_ENTRY *, const void *, DB_ENTRY *);
int32_t              dcsmget(const SET_ENTRY *, DB_ADDR *, const DB_ENTRY *);
int32_t              dcsmread(const SET_ENTRY *, const FIELD_ENTRY *, void *, const DB_ENTRY *);
int32_t              dcsmset(const SET_ENTRY *, const DB_ADDR *, const DB_ENTRY *);
int32_t              dcsmwrite(const SET_ENTRY *, const FIELD_ENTRY *, const void *, DB_ENTRY *);
int32_t              dcsoget(const SET_ENTRY *, DB_ADDR *, const DB_ENTRY *);
int32_t              dcsoread(const SET_ENTRY *, const FIELD_ENTRY *, void *, const DB_ENTRY *);
int32_t              dcsoset(const SET_ENTRY *, const DB_ADDR *, const DB_ENTRY *);
int32_t              dcsowrite(const SET_ENTRY *, const FIELD_ENTRY *, const void *, DB_ENTRY *);
int32_t              dcurkey(const DB_ENTRY *);
int32_t              ddbinitrectot(DB_ENTRY *);
int32_t              ddbmove(const char *, const DB_ENTRY *);
int32_t              ddbnum(const char *, DB_TASK *);
int32_t              ddbstat(int32_t, int32_t, void *, int32_t, DB_TASK *);
int32_t              ddbsetini(const char *, const char *, const char *, const DB_ENTRY *);
int32_t              ddbuserid(const char *, DB_TASK *);
int32_t              ddelete(DB_ENTRY *);
int32_t              ddestroy(const char *, DB_TASK *);
int32_t              ddiscon(const SET_ENTRY *, DB_ENTRY *);
int32_t              ddisdel(DB_ENTRY *);
int32_t              dfillnew(const RECORD_ENTRY *, const void *, DB_ENTRY *);
int32_t              dfindco(const SET_ENTRY *, DB_ENTRY *);
int32_t              dfindfm(const SET_ENTRY *, DB_ENTRY *);
int32_t              dfindlm(const SET_ENTRY *, DB_ENTRY *);
int32_t              dfindnm(const SET_ENTRY *, DB_ENTRY *);
int32_t              dfindpm(const SET_ENTRY *, DB_ENTRY *);
int32_t              dfldnum(int32_t *, int32_t, DB_TASK *);
int32_t              dfreeall(DB_TASK *);
int32_t              dhashfind(const FIELD_ENTRY *, const void *, DB_ENTRY *);
int32_t              diclose(int32_t, DB_ENTRY *);
int32_t              dimflush(const DB_ENTRY *);
int32_t              dinitfile(uint16_t, FILE_NO, const DB_ENTRY *);
int32_t              dinitialize(const DB_ENTRY *);
int32_t              dinternals(DB_TASK *, int32_t, int32_t, int32_t, void *, uint32_t);
int32_t              dismember(const SET_ENTRY *, const DB_ENTRY *);
int32_t              disowner(const SET_ENTRY *, const DB_ENTRY *);
int32_t              dkeybuild(const DB_ENTRY *);
int32_t              dkeyfilebuild(const DB_ENTRY *, const char *);
int32_t              dkeydel(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dkeydir(const FIELD_ENTRY *, uint16_t, const DB_ENTRY *);
int32_t              dkeyexist(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dkeyfind(const FIELD_ENTRY *, uint16_t, uint16_t, const void *, DB_ENTRY *);
int32_t              dkeyfree(const FIELD_ENTRY *, const DB_ENTRY *);
int32_t              dkeyfrst(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dkeylast(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dkeylock(const FIELD_ENTRY *, const char *, const DB_ENTRY *);
int32_t              dkeylstat(const FIELD_ENTRY *, char *, const DB_ENTRY *);
int32_t              dkeynext(const FIELD_ENTRY *, uint16_t, uint16_t, const void *, DB_ENTRY *);
int32_t              dkeyprev(const FIELD_ENTRY *, uint16_t, uint16_t, const void *, DB_ENTRY *);
int32_t              dkeyread(void *, const DB_ENTRY *);
int32_t              dkeystore(const FIELD_ENTRY *, DB_ENTRY *);
int32_t              dkeyrdstate(const FIELD_ENTRY *, void *, const DB_ENTRY *);
int32_t              dkeyszstate(const FIELD_ENTRY *, uint32_t *, const DB_ENTRY *);
int32_t              dkeywrstate(const FIELD_ENTRY *, const void *, DB_ENTRY *);
int32_t              dlmstat(const char *, int32_t *, DB_TASK *);
int32_t              dlock(uint32_t, const LOCK_REQUEST2 *, DB_TASK *);
int32_t              dlocktimeout(uint32_t, uint32_t, DB_TASK *);
int32_t              dmakenew(const RECORD_ENTRY *, DB_ENTRY *);
int32_t              dmapchar(uint8_t, uint8_t, const char *, uint8_t, DB_TASK *);
int32_t              dmembers(const SET_ENTRY *, int32_t *, const DB_ENTRY *);
int32_t              dopen(const char *, const char *, int32_t, const uint8_t *, uint32_t, DB_TASK *, DB_ENTRY **);
int32_t              dopentask(DB_TASK **);
int32_t              dopt(uint32_t, int32_t, DB_TASK *);
int32_t              drdcurr(void **, int32_t *, const DB_ENTRY *);
int32_t              ddbini(const char *, DB_TASK *);
int32_t              ddbver(const char *, char *, uint32_t);
int32_t              drecfilesize(const RECORD_ENTRY *, const DB_ENTRY *, uint64_t *);
int32_t              drecfree(const RECORD_ENTRY *, const DB_ENTRY *);
int32_t              drecfrst(const RECORD_ENTRY *, const DB_ENTRY *);
int32_t              dreclast(const RECORD_ENTRY *, const DB_ENTRY *);
int32_t              dreclock(const RECORD_ENTRY *, const char *, const DB_ENTRY *);
int32_t              dreclstat(const RECORD_ENTRY *, char *, const DB_ENTRY *);
int32_t              drecnext(DB_ENTRY *);
int32_t              drecnum(int32_t *, int32_t , DB_ENTRY *);
int32_t              drecprev(DB_ENTRY *);
int32_t              drecread(void *, const DB_ENTRY *);
int32_t              drecset(const RECORD_ENTRY *, DB_ENTRY *);
int32_t              drectot(const RECORD_ENTRY *, uint64_t *, const DB_ENTRY *);
int32_t              drecwrite(const void *, DB_ENTRY *);
int32_t              drerdcurr(void *, const DB_ENTRY *);
int32_t              drewrcurr(const void *, DB_ENTRY *);
int32_t              dsetdb(int32_t, DB_TASK *);
int32_t              dsetfiles(int32_t, const DB_TASK *);
int32_t              dsetfree(const SET_ENTRY *, const DB_ENTRY *);
int32_t              dsetkey(const FIELD_ENTRY *, const void *, DB_ENTRY *);
int32_t              dsetlock(const SET_ENTRY *, const char *, const DB_ENTRY *);
int32_t              dsetlstat(const SET_ENTRY *, char *, const DB_ENTRY *);
int32_t              dsetmm(const SET_ENTRY *, const SET_ENTRY *, const DB_ENTRY *);
int32_t              dsetmo(const SET_ENTRY *, const SET_ENTRY *, const DB_ENTRY *);
int32_t              dsetmr(const SET_ENTRY *, DB_ENTRY *);
int32_t              dsetnum(int32_t *, const SET_ENTRY *, DB_ENTRY *);
int32_t              dsetom(const SET_ENTRY *, const SET_ENTRY *, const DB_ENTRY *);
int32_t              dsetoo(const SET_ENTRY *, const SET_ENTRY *, const DB_ENTRY *);
int32_t              dsetor(const SET_ENTRY *, DB_ENTRY *);
int32_t              dsetpages(uint32_t, uint32_t, DB_TASK *);
int32_t              dsetrm(const SET_ENTRY *, DB_ENTRY *);
int32_t              dsetro(const SET_ENTRY *, DB_ENTRY *);
int32_t              dtfsversion(uint16_t *, uint16_t *, TFS_LIB_TYPE *);
int32_t              dtfsinit(const char *, const TFS_PARAMS *, TFS_HANDLE *, TFS_TYPE);
int32_t              dtfsxapiparams(TFS_HANDLE, const TFS_XAPI_PARAMS *);
int32_t              dtfsterm(TFS_HANDLE);
int32_t              dtfsrun(TFS_HANDLE, DB_BOOLEAN, uint16_t *);
int32_t              dtfsstop(TFS_HANDLE);
int32_t              dtfsdisconnect(TFS_HANDLE, const char *, uint16_t, const char *);
int32_t              dtfsIterateConnectedUsers(TFS_HANDLE, const char *, uint16_t, const DB_ITERATE_FCN *, uint16_t);
int32_t              dtimeout(uint32_t, DB_TASK *);
int32_t              dtrabort(DB_TASK *);
int32_t              dtractive(int16_t *, DB_TASK *);
int32_t              dtrbegin(const char *, DB_TASK *);
int32_t              dtrend(DB_TASK *);
int32_t              dtrdeletemark(const char *, DB_TASK *);
int32_t              dtrget(DB_ULONG*, DB_BOOLEAN, DB_TASK *);
int32_t              dtrincrement(DB_ULONG*, DB_TASK *);
int32_t              dtrmark(const char *, DB_TASK *);
int32_t              dtrprecommit(DB_TASK *);
int32_t              dtrrollback(const char *, DB_TASK *);
int32_t              dtrset(DB_ULONG, DB_TASK *);
int32_t              dtrrobegin(DB_ENTRY *, TXID *);
int32_t              dtrroend(DB_ENTRY *);
int32_t              dwrcurr(const void *, DB_ENTRY *);

#if defined(__cplusplus)
}
#endif

#endif  /* PROTO_H */

