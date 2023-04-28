/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

/* tfs.h - Contains definitions for the TFS */
#if !defined(TFS_H)
#define TFS_H

#include "rdm.h"
#include "tfs_lm.h"
#include "transport.h"

typedef enum {  CPS_MIRRORING = 0,
                CPS_LOGFILEAGE,
                CPS_LOGFILESPACE,
                CPS_CHKPTFREQ,
                CPS_DBFILELOC,
                CPS_SINGLESLAVE_LASTTXID,
                CPS_RESET_TXID
             } CHKPT_TYPE;

/* Mirroring Synchronous Mode */
typedef enum {  MSM_ENABLE = 0,
                MSM_DISABLE,
                MSM_IASYNC
             } MSM_TYPE;
             
/* Delete By TXID Modes */
typedef enum {  DBTM_EXCEPT = 0,
                DBTM_LESS,
                DBTM_EQUAL,
                DBTM_ALL
             } DBTM_TYPE;


#if defined(__cplusplus)
extern "C" {
#endif



#define TFS_INI "tfs.ini"
int32_t EXTERNAL_FCN tx_discon(TX_USER, void *);
int32_t EXTERNAL_FCN tx_rpc_reqhandler(void *);
int32_t EXTERNAL_FCN tx_setParams(const char *, uint32_t, uint32_t);
int32_t EXTERNAL_FCN tx_tfsinit(const char *, const TFS_PARAMS *, TFS_HANDLE *, TFS_TYPE);
int32_t EXTERNAL_FCN tx_tfsxapiparams(TFS_HANDLE, const TFS_XAPI_PARAMS *);
int32_t EXTERNAL_FCN tx_tfsterm(TFS_HANDLE);
int32_t EXTERNAL_FCN tx_tfsrun(TFS_HANDLE, DB_BOOLEAN, uint16_t *);
int32_t EXTERNAL_FCN tx_tfsstop(TFS_HANDLE);
int32_t EXTERNAL_FCN tx_tfsdisconnect(TFS_HANDLE, TX_USER, const char *);
int32_t EXTERNAL_FCN tx_tfsversion(uint16_t *, uint16_t *, TFS_LIB_TYPE *);
int32_t EXTERNAL_FCN tx_chkptSetting(TX_USER, TX_DB, const char *, CHKPT_TYPE, const char *);

/***************************************************************************
  User functions
*/
int32_t EXTERNAL_FCN tx_ping(const TX_CONN_INFO *);
int32_t EXTERNAL_FCN tx_login(const TX_CONN_INFO *, const char *, TX_USER *);
int32_t EXTERNAL_FCN tx_logout(TX_USER userID);
int32_t EXTERNAL_FCN tx_verify(TX_USER userID);
int32_t EXTERNAL_FCN tx_getDbUserId(TX_USER, char *, uint32_t, uint32_t *);
int32_t EXTERNAL_FCN tx_shutdown(const TX_CONN_INFO *);

/***************************************************************************
  INI functions
*/
int32_t EXTERNAL_FCN tx_iniGetInt32(TX_USER, const char*, const char*, const char*, int32_t*);
int32_t EXTERNAL_FCN tx_iniGetString(TX_USER, const char*, const char*, const char*, char*, uint32_t);
int32_t EXTERNAL_FCN tx_iniSet(TX_USER, const char*, const char*, const char*, const char*);
int32_t EXTERNAL_FCN tx_iniSetChkptSetting(TX_USER, const char*, const char*, const char*, const char*);
int32_t EXTERNAL_FCN tx_iniRemoveFile(TX_USER, const char *);


/***************************************************************************
  File functions
*/
int32_t EXTERNAL_FCN tx_fileCreate(TX_USER userID, TX_DB dbID, const char *fileName, uint32_t pageSize, uint16_t flags, TX_FILE *file);
int32_t EXTERNAL_FCN tx_fileOpen(TX_USER userID, TX_DB dbID, const char *fileName, uint32_t pageSize, uint16_t flags, TX_FILE *file);
int32_t EXTERNAL_FCN tx_fileClose(TX_USER userID, TX_FILE file);
int32_t EXTERNAL_FCN tx_fileRead(TX_USER userID, TX_FILE file, PSP_FILE_OFFSET offset, uint32_t length, uint32_t *nBytes, void *ptr);
int32_t EXTERNAL_FCN tx_fileWrite(TX_USER userID, TX_FILE file, PSP_FILE_OFFSET offset, uint32_t length, const void *ptr);
int32_t EXTERNAL_FCN tx_fileReadPage(TX_USER userID, TX_FILE file, uint64_t page, uint32_t size, void *ptr);
int32_t EXTERNAL_FCN tx_fileLength(TX_USER userID, TX_FILE file, PSP_FILE_OFFSET *length);
int32_t EXTERNAL_FCN tx_fileSetSize(TX_USER userID, TX_FILE file, PSP_FILE_OFFSET length);
int32_t EXTERNAL_FCN tx_fileRename(TX_USER userID, TX_DB dbID, const char *oldName, uint16_t, const char *newName, uint16_t);
int32_t EXTERNAL_FCN tx_fileRemove(TX_USER userID, TX_DB dbID, const char *fileName, uint16_t xflags);
int32_t EXTERNAL_FCN tx_fileCopy(TX_USER userID, TX_DB dbID, const char *oldName, const char *newName);
int32_t EXTERNAL_FCN tx_fileValidate(TX_USER userID, TX_DB dbID, const char *fileName, uint16_t xflags);

/***************************************************************************
  Log file functions
*/
int32_t EXTERNAL_FCN tx_LOGbegin(TX_USER userID);
int32_t EXTERNAL_FCN tx_LOGwrite(TX_USER userID, TX_FILE file, uint64_t page, uint32_t length, const void *pgptr);
int32_t EXTERNAL_FCN tx_LOGend(TX_USER userID);
int32_t EXTERNAL_FCN tx_LOGcommit(TX_USER userID);
int32_t EXTERNAL_FCN tx_LOGprecommit(TX_USER userID);
int32_t EXTERNAL_FCN tx_LOGrollback(TX_USER userID);
int32_t EXTERNAL_FCN tx_LOGinit(TX_USER, TX_FILE);
int32_t EXTERNAL_FCN tx_LOGsynchronous(TX_USER, const char *, MSM_TYPE);
int32_t EXTERNAL_FCN tx_LOGmirSync(TX_USER, TX_DB, TXID);
int32_t EXTERNAL_FCN tx_LOGmirSyncError(TX_USER, TX_DB, int32_t);
int32_t EXTERNAL_FCN tx_LOGisDbSynchronous(TX_USER, const char *, uint16_t *);
int32_t EXTERNAL_FCN tx_LOGwaitForCommit(TX_USER, TX_DB, TXID, uint32_t);
int32_t EXTERNAL_FCN tx_LOGstopWaitForCommit(TX_USER, TX_DB);
int32_t EXTERNAL_FCN tx_LOGsetNextTxid(TX_USER, TX_DB, TXID);
int32_t EXTERNAL_FCN tx_LOGsDeleteByTxid(TX_USER, const char *, const char *, TXID, DBTM_TYPE, uint16_t);

/***************************************************************************
  Read-Only Transaction functions
*/
int32_t EXTERNAL_FCN tx_ROTbegin(TX_USER userID, TX_DB dbID, TXID *txID);
int32_t EXTERNAL_FCN tx_ROTend(TX_USER userID, TX_DB dbID);

/***************************************************************************
  Database functions
*/
int32_t EXTERNAL_FCN tx_dbCreate(TX_USER userID, const char *dbName, TX_DB *db);
int32_t EXTERNAL_FCN tx_dbCreateMir(TX_USER userID, const char *dbName, DFL_TYPE, TX_DB *db);
int32_t EXTERNAL_FCN tx_dbOpen(TX_USER userID, const char *dbName, TX_DB *db);
int32_t EXTERNAL_FCN tx_dbOpenMir(TX_USER userID, const char *dbName, DFL_TYPE, TX_DB *db);
int32_t EXTERNAL_FCN tx_dbClose(TX_USER userID, TX_DB db);
int32_t EXTERNAL_FCN tx_dbFileInfo(TX_USER, TX_DB, const char *, uint16_t, uint16_t *, char (*)[FILENMLEN], uint16_t *, uint16_t *, uint16_t *, uint16_t *, PSP_FILE_OFFSET *);
int32_t EXTERNAL_FCN tx_dbLastCommittedTxid(TX_USER, TX_DB, TXID *);
int32_t EXTERNAL_FCN tx_dbLastTxid(TX_USER, TX_DB, TXID *);
int32_t EXTERNAL_FCN tx_dbNames(TX_USER, uint16_t *, const char ***);
int32_t EXTERNAL_FCN tx_dbDestroy(TX_USER userID, TX_DB db);
int32_t EXTERNAL_FCN tx_dbDumpCache(TX_USER userID, TX_DB dbID);
int32_t EXTERNAL_FCN tx_dbIMFlush(TX_USER userID, TX_DB dbID);
int32_t EXTERNAL_FCN tx_dbSetInMemory(TX_USER userID, TX_DB dbID);
int32_t EXTERNAL_FCN tx_dbGetInMemory(TX_USER userID, TX_DB dbID, uint16_t *);
int32_t EXTERNAL_FCN tx_dbUsers(TX_USER, const char *, uint16_t *, const char ***, TXID **);
int32_t EXTERNAL_FCN tx_dbMove(TX_USER, TX_DB, const char *);
int32_t EXTERNAL_FCN tx_dbGetFilesLike(TX_USER userID, TX_DB dbID, const char *, uint32_t *, const char ***);

/***************************************************************************
  Lock Manager Transaction functions
*/
int32_t EXTERNAL_FCN tx_lmSettime(TX_USER userID, uint32_t timeout);
int32_t EXTERNAL_FCN tx_lmDbOpen(TX_USER userID, uint16_t num_files, uint16_t mode, const char *const *fnames, uint16_t *fref);
int32_t EXTERNAL_FCN tx_lmDbClose(TX_USER userID, uint16_t num_files, const uint16_t *frefs);
int32_t EXTERNAL_FCN tx_lmFree(TX_USER userID, uint16_t num_files, const uint16_t *frefs, uint16_t *num_timestamps, uint16_t *file_ids, uint32_t *timestamps);
int32_t EXTERNAL_FCN tx_lmLock(TX_USER userID, uint16_t num_files, uint16_t read_lock_secs, uint16_t write_locks_secs, const TFS_LM_LOCKREQ *locks, uint16_t *num_timestamps, uint16_t *file_ids, uint32_t *timestamps);
int32_t EXTERNAL_FCN tx_lmClearUser(TX_USER userID, const char *lm_user);
int32_t EXTERNAL_FCN tx_lmUserStatus(TX_USER userID, const char *lm_username, int16_t *user_status);
int32_t EXTERNAL_FCN tx_lmGeneralStatus(TX_USER userID, TFS_LM_STATUS *lm_status);
int32_t EXTERNAL_FCN tx_lmUserInfo(TX_USER userID, uint16_t lm_userno, TFS_LM_USERINFO *lm_userinfo, uint16_t lm_num_bmbytes, uint8_t *lm_uopen, uint8_t *lm_ulock, uint8_t *lm_ureq, uint16_t lm_num_files, TFS_LM_FILEINFO *lm_files);
int32_t EXTERNAL_FCN tx_lmEndConnection(TX_USER, RHS_HANDLE, const char *);

/***************************************************************************
  b_log functions
*/
int32_t EXTERNAL_FCN tx_blog_init(TX_USER, uint16_t, B_LOG_DATA *);
int32_t EXTERNAL_FCN tx_blog_term(TX_USER, uint16_t, B_LOG_DATA);
int32_t EXTERNAL_FCN tx_blog_create(TX_USER, uint16_t, const char *, uint16_t, const char *, size_t, B_LOG_FH *, B_LOG_DATA);
int32_t EXTERNAL_FCN tx_blog_open(TX_USER, uint16_t, const char *, uint16_t, char *, size_t, B_LOG_FH *, B_LOG_DATA);
int32_t EXTERNAL_FCN tx_blog_close(TX_USER, uint16_t, B_LOG_FH);
int32_t EXTERNAL_FCN tx_blog_putNewFile(TX_USER, uint16_t, B_LOG_FH, const char *, uint16_t, uint32_t, uint16_t);
int32_t EXTERNAL_FCN tx_blog_putNewPage(TX_USER, uint16_t, B_LOG_FH, LOG_COMPRESSION_TYPE, uint64_t, const void *, uint32_t);
int32_t EXTERNAL_FCN tx_blog_getNextFile(TX_USER, uint16_t, B_LOG_FH, char *, uint32_t, uint16_t *, uint32_t *, uint16_t *, uint32_t *, DFL_TYPE);
int32_t EXTERNAL_FCN tx_blog_getNextPage(TX_USER, uint16_t, B_LOG_FH, void *, uint32_t, uint32_t *, uint64_t *);
int32_t EXTERNAL_FCN tx_blog_getCurrFilename(TX_USER, uint16_t, B_LOG_FH, char *, uint32_t, uint32_t *);
int32_t EXTERNAL_FCN tx_blog_getNumFiles(TX_USER, uint16_t, B_LOG_FH, uint32_t *);
int32_t EXTERNAL_FCN tx_blog_getTxidFromLogFileName(TX_USER, uint16_t, const char *, TXID *, LOG_FILE_TYPE *);
int32_t EXTERNAL_FCN tx_blog_findLastTxidFromDirectory(TX_USER, uint16_t, const char *, uint16_t, TXID *, const char *);



#if defined(__cplusplus)
}
#endif

#endif  /* TFS_H */

