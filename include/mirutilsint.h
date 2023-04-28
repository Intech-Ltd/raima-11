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

#if !defined(__MIRUTILSINT_H)
#define __MIRUTILSINT_H

#include "dbmirror.h"

#define REP_MAGIC          0x52847182

typedef struct DBREP_CTX_S
{
    uint32_t   magic;
    PSP_MEMTAG mtag;
    RHS_PARAMS params;
    MIR_CTX    mir_ctx;
    RHS_HANDLE hRHS;
    PSP_FH     lockFile;
} DBREP_CTX;

/* Prelog rename destination type */
#define PRT_LOG           1
#define PRT_REPLOG        2
#define PRT_ARCHIVE       3
#define PRT_PRELOG        4
#define PRT_REPTMP        5

/* RDSLOGIN param lengths */
#define RS_SERVER_LEN   128
#define RS_USERID_LEN   33
#define RS_PASSWD_LEN   33

#if defined(__cplusplus)
extern "C" {
#endif


int32_t EXTERNAL_FCN mutil_createLogFile(TX_USER, TX_DB, TXID, const char *, uint16_t);
int32_t EXTERNAL_FCN mutil_removeLogFile(TX_USER, TX_DB, TXID, const char *, uint16_t);
void    EXTERNAL_FCN mutil_freeDBDInfo(const MIR_SLAVE *);
int32_t EXTERNAL_FCN mutil_getDbLogName(char *, uint32_t, TXID, uint32_t);
char   *EXTERNAL_FCN mutil_getFullFilePath(const char *, const char *, const char *, PSP_MEMTAG);
char   *EXTERNAL_FCN mutil_getFullDbPath(char *, const char *, uint16_t, PSP_MEMTAG);
int32_t EXTERNAL_FCN mutil_getRepLogName(char *, uint32_t, TXID);
int32_t EXTERNAL_FCN mutil_getServerType(const void *, uint16_t *);
int32_t EXTERNAL_FCN mutil_getArchLogName(char *, uint32_t, TXID);
int32_t EXTERNAL_FCN mutil_getHeldLogName(char *, uint32_t, TXID);
int32_t EXTERNAL_FCN mutil_getCommittedLogName(char *, uint32_t, TXID);
DB_BOOLEAN EXTERNAL_FCN mutil_isNameDbLog(const char *, uint32_t *);
int32_t EXTERNAL_FCN mutil_lockAllDbFiles(MIR_SLAVE *);
int32_t EXTERNAL_FCN mutil_lockLogDbFiles(MIR_SLAVE *, const char *);
int32_t EXTERNAL_FCN mutil_loginToTFS(const char *, uint16_t, const char *, const char *, TX_USER *);
int32_t EXTERNAL_FCN mutil_logoutTFS(TX_USER);
int32_t EXTERNAL_FCN mutil_readDBDInfo(MIR_SLAVE *);
int32_t EXTERNAL_FCN mutil_removeInmemFlagFromDBD(MIR_SLAVE *);
int32_t EXTERNAL_FCN mutil_renamePrelogFile(const MIR_SLAVE *, const char *, TXID, uint16_t);
void    EXTERNAL_FCN mutil_repsqlLoginParms(const char *, char *, char *, char *);
void    EXTERNAL_FCN mutil_unlockDbFiles(MIR_SLAVE *);
DB_BOOLEAN EXTERNAL_FCN mutil_DBDCheck(TX_USER, const char *, uint16_t, const char (*)[FILENMLEN], const uint16_t *);

int32_t EXTERNAL_FCN tlog_buildDatabaseLogs(const RHS_CONN_CTX *, char *, uint16_t, TXID *, uint32_t *);
int32_t tlog_postLogify(MIR_SLAVE *);
int32_t tlog_sendLogToTFS(MIR_SLAVE *, char *, TXID);
int32_t dblog_sendLogToTFS(MIR_SLAVE *, char *);

#if defined(__cplusplus)
}
#endif

#endif /* __MIRUTILSINT_H */
