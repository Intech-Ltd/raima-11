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

/* mirrpc.h - Contains definitions for the mirroring rpc */
#if !defined(__MIRRPC_H)
#define __MIRRPC_H

#include "rdm.h"

#if defined(__cplusplus)
extern "C" {
#endif

int32_t EXTERNAL_FCN mir_begin(const char *, const char *, const char *, uint16_t, int16_t, REP_SLAVE_TYPE, uint16_t, const char *, uint16_t, uint16_t *);
int32_t EXTERNAL_FCN mir_end(const char *, const char *, const char *, uint16_t);
int32_t EXTERNAL_FCN mir_getNextLog(TX_CONN, const char *, TXID, uint32_t, uint32_t *, char *, uint32_t, TXID *, void *, uint32_t, uint32_t *, uint32_t);
int32_t EXTERNAL_FCN mir_getNextLogBlock(TX_CONN, const char *, const char *, PSP_FILE_OFFSET, void *, uint32_t, uint32_t *, TXID);
int32_t EXTERNAL_FCN mir_startSyncMirror(TX_CONN, const char *);
int32_t EXTERNAL_FCN mir_endSyncMirror(TX_CONN, const char *);
int32_t EXTERNAL_FCN mir_syncSlaveTxApplied(TX_CONN, const char *, TXID);
int32_t EXTERNAL_FCN mir_syncError(TX_CONN, const char *, int32_t);
int32_t EXTERNAL_FCN mir_isDbSynchronous(TX_CONN, const char *, uint16_t *);
int32_t EXTERNAL_FCN mir_doesDbExist(TX_CONN, const char *, uint16_t *);
int32_t EXTERNAL_FCN mir_stopWaitForCommit(TX_CONN, const char *);
int32_t EXTERNAL_FCN mir_dbdCheck(TX_CONN, const char *, uint16_t, const char (*)[FILENMLEN], const uint16_t *);
int32_t EXTERNAL_FCN mir_registerSlave(TX_CONN, const char *, const char *, const char *, uint16_t, uint16_t, uint16_t);
int32_t EXTERNAL_FCN mir_deregisterSlave(TX_CONN);
int32_t EXTERNAL_FCN mir_setSlaveStatus(TX_CONN, const char *, uint16_t);

int32_t EXTERNAL_FCN rep_getNextLog(TX_CONN, const char *, TXID, uint32_t, uint32_t *, char *, uint32_t, TXID *, void *, uint32_t, uint32_t *, uint32_t);
int32_t EXTERNAL_FCN rep_getNextLogBlock(TX_CONN, const char *, const char *, PSP_FILE_OFFSET, void *, uint32_t, uint32_t *, TXID);

#if defined(__cplusplus)
}
#endif

#endif  /* __MIRRPC_H */

