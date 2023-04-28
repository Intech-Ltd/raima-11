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

#ifndef DPROTO_H
#define DPROTO_H

#include "dbtype.h"

int32_t EXTERNAL_FCN d_blobdelete(int32_t field, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_blobread(int32_t field, void *buf, uint32_t len, uint32_t *size, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_blobseek(int32_t field, uint32_t offset, SEEK_TYPE origin, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_blobsize(int32_t field, uint32_t *size, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_blobtell(int32_t field, uint32_t *offset, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_blobtruncate(int32_t field, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_blobwrite(int32_t field, const void *buf, uint32_t size, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_checkid(const char *userid, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_checkidW(const wchar_t *userid, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_close(DB_TASK *task);
int32_t EXTERNAL_FCN d_closeall(DB_TASK *task);
int32_t EXTERNAL_FCN d_closetask(DB_TASK *task);
int32_t EXTERNAL_FCN d_cmtype(int32_t SET, int32_t *rectype, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_connect(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_cotype(int32_t SET, int32_t *rectype, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crget(DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crread(int32_t FIELD, void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crset(const DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crsetnr(const DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crtype(int32_t *rectype, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crwrite(int32_t FIELD, const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csmget(int32_t SET, DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csmread(int32_t SET, int32_t FIELD, void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csmset(int32_t SET, const DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csmwrite(int32_t SET, int32_t FIELD, const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csoget(int32_t SET, DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csoread(int32_t SET, int32_t FIELD, void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csoset(int32_t SET, const DB_ADDR *dba, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csowrite(int32_t SET, int32_t FIELD, const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_curkey(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_dberr(int32_t errnum, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbinitrectot(DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbmove(const char *dirname, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbmoveW(const wchar_t *dirname, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_dbnum(const char *dbname, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbnumW(const wchar_t *dbname, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_dbstat(int32_t, int32_t, void *, int32_t, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbsetini(const char *section, const char *key, const char *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_dbtmp(const char *, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbtmpW(const wchar_t *, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_dbuserid(const char *userid, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbuseridW(const wchar_t *userid, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_decode_dba4(DB_ADDR4 dba, FILE_NO *file, F_ADDR4 *slot);
int32_t EXTERNAL_FCN d_decode_dba8(DB_ADDR8 dba, FILE_NO *file, F_ADDR8 *slot);
int32_t EXTERNAL_FCN d_decode_dba(DB_ADDR dba, FILE_NO *file, F_ADDR8 *slot);
int32_t EXTERNAL_FCN d_decode_ldba(const DB_ADDR *dba, uint16_t *uno, FILE_NO *file, F_ADDR8 *slot);
int32_t EXTERNAL_FCN d_def_opt(uint32_t optflag, DB_TASK *task);
int32_t EXTERNAL_FCN d_delete(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_destroy(const char *dbname, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_destroyW(const wchar_t *dbname, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_discon(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_disdel(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_encode_dba4(FILE_NO file, F_ADDR4 slot, DB_ADDR4 *dba);
int32_t EXTERNAL_FCN d_encode_dba8(FILE_NO file, F_ADDR8 slot, DB_ADDR8 *dba);
int32_t EXTERNAL_FCN d_encode_dba(FILE_NO file, F_ADDR8 slot, DB_ADDR *dba);
int32_t EXTERNAL_FCN d_encode_ldba(uint16_t uno, FILE_NO file, F_ADDR8 slot, DB_ADDR *dba);
const char * EXTERNAL_FCN d_errorstr(int32_t errnum);
const char * EXTERNAL_FCN d_errorname(int32_t errnum);

int32_t EXTERNAL_FCN d_fillnew(int32_t REC, const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_findco(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_findfm(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_findlm(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_findnm(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_findpm(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_fldnum(int32_t *index, int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_freeall(DB_TASK *task);
int32_t EXTERNAL_FCN d_get_db_rep_type(REP_DB_TYPE *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_iclose(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_imflush(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_initfile(FILE_NO FILEID, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_initialize(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_internals(DB_TASK *task, int32_t topic, int32_t id, int32_t elem, void *ptr, uint32_t size);
int32_t EXTERNAL_FCN d_iopen(const char *dbnames, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_iopenW(const wchar_t *dbnames, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_iopen_ptr(const char *dbname, const void *dbd_array, int32_t dbdsize, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_iopen_ptrW(const wchar_t *dbname, const void *dbd_array, int32_t dbdsize, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_ismember(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_isowner(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keydel(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keydir(int32_t keyfield, int16_t dir, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyexist(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyfind(int32_t FIELD, const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyfree(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyfrst(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keylast(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keylock(int32_t FIELD, const char *type, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_keylockW(int32_t FIELD, const wchar_t *type, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_keylstat(int32_t FIELD, char *type, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_keylstatW(int32_t FIELD, wchar_t *type, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_keynext(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keynum(int32_t *, int32_t, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyprev(int32_t FIELD, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyread(void *value, DB_TASK *task);
int32_t EXTERNAL_FCN d_keystore(int32_t, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyrdstate(int32_t keyfield, void *buf, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keyszstate(int32_t keyfield, uint32_t *pSize, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_keywrstate(int32_t keyfield, const void *buf, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_lmclear(const char *, const char *, LMC_AVAIL_FCN *, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_lmclearW(const wchar_t *, const wchar_t *, LMC_AVAIL_FCN *, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_lmstat(const char *name, int32_t *status, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_lmstatW(const wchar_t *name, int32_t *status, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_lock(int32_t count, const LOCK_REQUEST *lrpkt, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_locktimeout(int32_t read_secs, int32_t write_secs, DB_TASK *task);
int32_t EXTERNAL_FCN d_makenew(int32_t REC, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_members(int32_t SET, int32_t *count, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_off_opt(uint32_t optflag, DB_TASK *task);
int32_t EXTERNAL_FCN d_on_opt(uint32_t optflag, DB_TASK *task);
int32_t EXTERNAL_FCN d_open(const char *dbnames, const char *type, DB_TASK *task);
int32_t EXTERNAL_FCN d_open_ptr(const char *dbname, const char *, const void *dbd_array, int32_t dbdsize, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_openW(const wchar_t *dbnames, const wchar_t *type, DB_TASK *task);
int32_t EXTERNAL_FCN d_open_ptrW(const wchar_t *dbname, const wchar_t *type, const void *dbd_array, int32_t dbdsize, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_opentask(DB_TASK **task);
int32_t EXTERNAL_FCN d_rdcurr(void **currbuff, int32_t *currsize, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_dbini(const char *dir_name, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbiniW(const wchar_t *dir_name, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_dbver(const char *fmt, char *buf, int32_t len);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbverW(const wchar_t *fmt, wchar_t *buf, int32_t len);
#endif
int32_t EXTERNAL_FCN d_pkeyfind(int32_t keyfield, uint16_t nFields, uint16_t partialStrLen, const void *fldval, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_pkeynext(int32_t keyfield, uint16_t nFields, uint16_t partialStrLen, const void *fldval, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_pkeyprev(int32_t keyfield, uint16_t nFields, uint16_t partialStrLen, const void *fldval, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recfilesize(int32_t REC, uint64_t *pTotPgs, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recfree(int32_t REC, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recfrst(int32_t REC, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_reclast(int32_t REC, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_reclock(int32_t REC, const char *type, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_reclockW(int32_t REC, const wchar_t *type, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_reclstat(int32_t REC, char *type, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_reclstatW(int32_t REC, wchar_t *type, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_recnext(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recnum(int32_t *index, int32_t REC, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recprev(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recread(void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recset(int32_t REC, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_rectot(int32_t REC, uint64_t *pTot, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_recwrite(const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_rerdcurr(void *const *currbuff, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_rewrcurr(const void *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_set_dberr(ERRORPROCA errfunc, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_set_dberrW(ERRORPROCW errfunc, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_set_dberr_ex(ERRORPROC2A errfunc, void *userdata, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_set_dberr_exW(ERRORPROC2W errfunc, void *userdata, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_setdb(int32_t dbn, DB_TASK *task);
int32_t EXTERNAL_FCN d_set_db_rep_type(REP_DB_TYPE, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setfree(int32_t SET, DB_TASK *task, int32_t dbn);
/* TODO JNI HEADER */
int32_t EXTERNAL_FCN d_setkey(int32_t FIELD, const void *value, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setlock(int32_t SET, const char *type, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_setlockW(int32_t SET, const wchar_t *type, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_setlstat(int32_t SET, char *type, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_setlstatW(int32_t SET, wchar_t *type, DB_TASK *task, int32_t dbn);
#endif
int32_t EXTERNAL_FCN d_setmm(int32_t TO_SET, int32_t FR_SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setmo(int32_t M_SET, int32_t O_SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setmr(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setnum(int32_t *index, int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setom(int32_t O_SET, int32_t M_SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setoo(int32_t TO_SET, int32_t FR_SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setor(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setpages(int32_t dbpages, int32_t ovpages, DB_TASK *task);
int32_t EXTERNAL_FCN d_setrm(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setro(int32_t SET, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_tfsversion(uint16_t *major, uint16_t *minor, TFS_TYPE *type);
int32_t EXTERNAL_FCN d_tfsinit(const char *docroot, const TFS_PARAMS *tparams, TFS_HANDLE *hTFS);
int32_t EXTERNAL_FCN d_tfsinitEx(const char *docroot, const TFS_PARAMS *tparams, TFS_HANDLE *hTFS, TFS_TYPE type);
int32_t EXTERNAL_FCN d_tfsxapiparams(TFS_HANDLE hTFS, const TFS_XAPI_PARAMS *xparams);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_tfsinitW(const wchar_t *docroot, const TFS_PARAMSW *tparams, TFS_HANDLE *hTFS);
int32_t EXTERNAL_FCN d_tfsinitExW(const wchar_t *docroot, const TFS_PARAMSW *tparams, TFS_HANDLE *hTFS, TFS_TYPE type);
int32_t EXTERNAL_FCN d_tfsxapiparamsW(TFS_HANDLE hTFS, const TFS_XAPI_PARAMSW *xparams);
#endif
int32_t EXTERNAL_FCN d_tfsterm(TFS_HANDLE hTFS);
int32_t EXTERNAL_FCN d_tfsrun(TFS_HANDLE hTFS, DB_BOOLEAN threaded, uint16_t *tfs_done);
int32_t EXTERNAL_FCN d_tfsstop(TFS_HANDLE hTFS);
int32_t EXTERNAL_FCN d_tfsdisconnect(TFS_HANDLE hTFS, const char *, uint16_t, const char *);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_tfsdisconnectW(TFS_HANDLE hTFS, const wchar_t *, uint16_t, const wchar_t *);
#endif
int32_t EXTERNAL_FCN d_tfsIterateConnectedUsers(TFS_HANDLE, const char *, uint16_t, TFS_ITERATE_USERS_FCN);
int32_t EXTERNAL_FCN d_tfsIterateConnectedUsersW(TFS_HANDLE, const wchar_t *, uint16_t, TFS_ITERATE_USERS_W_FCN);
int32_t EXTERNAL_FCN d_timeout(int32_t secs, DB_TASK *task);
int32_t EXTERNAL_FCN d_trabort(DB_TASK *task);
int32_t EXTERNAL_FCN d_tractive(int16_t *boolean, DB_TASK *task);
int32_t EXTERNAL_FCN d_trbegin(const char *tid, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_trbeginW(const wchar_t *tid, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_trdeletemark(const char *mtag, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_trdeletemarkW(const wchar_t *mtag, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_trend(DB_TASK *task);
int32_t EXTERNAL_FCN d_trmark(const char *mtag, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_trmarkW(const wchar_t *mtag, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_trprecommit(DB_TASK *task);
int32_t EXTERNAL_FCN d_trrobegin(DB_TASK *task, int32_t, TXID *txid);
int32_t EXTERNAL_FCN d_trroend(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_trrollback(const char *mtag, DB_TASK *task);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_trrollbackW(const wchar_t *mtag, DB_TASK *task);
#endif
int32_t EXTERNAL_FCN d_wrcurr(const void *currbuff, DB_TASK *task, int32_t dbn);

/* Handle versions of the functions */
int32_t EXTERNAL_FCN dh_blobdelete(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_blobread(int32_t, void *, uint32_t, uint32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_blobseek(int32_t, uint32_t, SEEK_TYPE, RDM_DB);
int32_t EXTERNAL_FCN dh_blobsize(int32_t, uint32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_blobtell(int32_t, uint32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_blobtruncate(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_blobwrite(int32_t, const void *, uint32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_cmtype(int32_t, int32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_connect(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_cotype(int32_t, int32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_crget(DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_crread(int32_t, void *, RDM_DB);
int32_t EXTERNAL_FCN dh_crset(const DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_crsetnr(const DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_crtype(int32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_crwrite(int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_csmget(int32_t, DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_csmread(int32_t, int32_t, void *, RDM_DB);
int32_t EXTERNAL_FCN dh_csmset(int32_t, const DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_csmwrite(int32_t, int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_csoget(int32_t, DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_csoread(int32_t, int32_t, void *, RDM_DB);
int32_t EXTERNAL_FCN dh_csoset(int32_t, const DB_ADDR *, RDM_DB);
int32_t EXTERNAL_FCN dh_csowrite(int32_t, int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_curkey(RDM_DB);
int32_t EXTERNAL_FCN dh_dberr(int32_t, DB_TASK *task);
int32_t EXTERNAL_FCN dh_dbhandle(const char *, DB_TASK *task, RDM_DB *);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_dbhandleW(const wchar_t *, DB_TASK *task, RDM_DB *);
#endif
int32_t EXTERNAL_FCN dh_dbinitrectot(RDM_DB);
int32_t EXTERNAL_FCN dh_dbmove(const char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_dbmoveW(const wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_dbsetini(const char *, const char *, const char *, RDM_DB);
int32_t EXTERNAL_FCN dh_delete(RDM_DB);
int32_t EXTERNAL_FCN dh_discon(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_disdel(RDM_DB);
int32_t EXTERNAL_FCN dh_fillnew(int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_findco(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_findfm(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_findlm(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_findnm(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_findpm(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_fldnum(int32_t *, int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_iclose(RDM_DB);
int32_t EXTERNAL_FCN dh_imflush(RDM_DB);
int32_t EXTERNAL_FCN dh_initfile(FILE_NO, RDM_DB);
int32_t EXTERNAL_FCN dh_initialize(RDM_DB);
int32_t EXTERNAL_FCN dh_internals(RDM_DB);
int32_t EXTERNAL_FCN dh_iopen(const char *, DB_TASK *task, RDM_DB *);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_iopenW(const wchar_t *, DB_TASK *task, RDM_DB *);
#endif
int32_t EXTERNAL_FCN dh_iopen_ptr(const char *, const void *, int32_t, DB_TASK *task, RDM_DB *);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_iopen_ptrW(const wchar_t *, const void *, int32_t, DB_TASK *task, RDM_DB *);
#endif
int32_t EXTERNAL_FCN dh_ismember(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_isowner(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keydel(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keydir(int32_t, int16_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keyexist(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keyfind(int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_keyfree(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keyfrst(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keylast(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keylock(int32_t, const char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_keylockW(int32_t, const wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_keylstat(int32_t, char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_keylstatW(int32_t, wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_keynext(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keynum(int32_t *, int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keyprev(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keyread(void *, RDM_DB);
int32_t EXTERNAL_FCN dh_keystore(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_keyrdstate(int32_t, void *, RDM_DB);
int32_t EXTERNAL_FCN dh_keyszstate(int32_t, uint32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_keywrstate(int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_lock(int32_t, const LOCK_REQUEST2 *, DB_TASK *task);
int32_t EXTERNAL_FCN dh_makenew(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_members(int32_t, int32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_open(const char *, const char *, DB_TASK *task, RDM_DB *);
int32_t EXTERNAL_FCN dh_open_ptr(const char *, const char *, const void *, int32_t, DB_TASK *task, RDM_DB *);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_openW(const wchar_t *, const wchar_t *, DB_TASK *task, RDM_DB *);
int32_t EXTERNAL_FCN dh_open_ptrW(const wchar_t *, const wchar_t *, const void *, int32_t, DB_TASK *task, RDM_DB *);
#endif
int32_t EXTERNAL_FCN dh_rdcurr(void **, int32_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_pkeyfind(int32_t, uint16_t, uint16_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_pkeynext(int32_t, uint16_t, uint16_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_pkeyprev(int32_t, uint16_t, uint16_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_recfree(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_recfrst(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_reclast(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_reclock(int32_t, const char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_reclockW(int32_t, const wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_reclstat(int32_t, char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_reclstatW(int32_t, wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_recnext(RDM_DB);
int32_t EXTERNAL_FCN dh_recnum(int32_t *, int32_t , RDM_DB);
int32_t EXTERNAL_FCN dh_recprev(RDM_DB);
int32_t EXTERNAL_FCN dh_recread(void *, RDM_DB);
int32_t EXTERNAL_FCN dh_recset(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_rectot(int32_t, uint64_t *, RDM_DB);
int32_t EXTERNAL_FCN dh_recwrite(const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_rerdcurr(void *const *, RDM_DB);
int32_t EXTERNAL_FCN dh_rewrcurr(const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_setfree(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setkey(int32_t, const void *, RDM_DB);
int32_t EXTERNAL_FCN dh_setlock(int32_t, const char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_setlockW(int32_t, const wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_setlstat(int32_t, char *, RDM_DB);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN dh_setlstatW(int32_t, wchar_t *, RDM_DB);
#endif
int32_t EXTERNAL_FCN dh_setmm(int32_t, int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setmo(int32_t, int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setmr(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setnum(int32_t *, int32_t , RDM_DB);
int32_t EXTERNAL_FCN dh_setom(int32_t, int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setoo(int32_t, int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setor(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setrm(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_setro(int32_t, RDM_DB);
int32_t EXTERNAL_FCN dh_trrobegin(RDM_DB, TXID *);
int32_t EXTERNAL_FCN dh_trroend(RDM_DB);
int32_t EXTERNAL_FCN dh_wrcurr(const void *, RDM_DB);

DB_TASK * EXTERNAL_FCN dh_getTask(RDM_DB);

/* Defunct functions */
int32_t EXTERNAL_FCN d_cmstat(int32_t, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_costat(int32_t, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_crstat(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_csstat(int32_t, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_ctbpath(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_ctscm(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_ctsco(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_ctscr(DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_dbdpath(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbfpath(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbmfpath(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dblog(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbrep(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbtaf(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_gtscm(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_gtsco(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_gtscr(DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_gtscs(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_lockcomm(LMC_AVAIL_FCN *, DB_TASK *task);
int32_t EXTERNAL_FCN d_lockmgr(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_mapchar(uint8_t, uint8_t, const char *, uint8_t, DB_TASK *task);
int32_t EXTERNAL_FCN d_recover(const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_recstat(DB_ADDR, DB_ULONG, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_renclean(DB_TASK *task);
int32_t EXTERNAL_FCN d_renfile(const char *, FILE_NO, const char *, DB_TASK *task);
int32_t EXTERNAL_FCN d_rlbclr(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_rlbset(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_rlbtst(DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_setfiles(int32_t, DB_TASK *task);
int32_t EXTERNAL_FCN d_stscm(int32_t, DB_ULONG, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_stsco(int32_t, DB_ULONG, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_stscr(DB_ULONG, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_stscs(int32_t, DB_ULONG, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_trincrement(DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_trget(DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_trset(DB_ULONG, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_utscm(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_utsco(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_utscr(DB_ULONG *, DB_TASK *task, int32_t dbn);
int32_t EXTERNAL_FCN d_utscs(int32_t, DB_ULONG *, DB_TASK *task, int32_t dbn);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_ctbpathW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbdpathW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbfpathW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbmfpathW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dblogW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbrepW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_dbtafW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_lockmgrW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_recoverW(const wchar_t *, DB_TASK *task);
int32_t EXTERNAL_FCN d_renfileW(const wchar_t *, FILE_NO, const wchar_t *, DB_TASK *task);
#endif

#endif /* ! DPROTO_H */

