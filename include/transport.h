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

/* transport.h - Contains definitions for the transport layer */
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "psp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TFS default tcp port */
#define TFS_DEFAULT_PORT        21553
#define TFS_DEFAULT_PORT_STR    "21553"
/* The dbmirror port is 1 plus the tfs port */
#define MIRROR_PORT_OFFSET      1
#define MIR_DEFAULT_PORT        (TFS_DEFAULT_PORT + MIRROR_PORT_OFFSET)
#define MIR_DEFAULT_PORT_STR    "21554"

#define SQL_PORT_OFFSET         2
#define SQL_DEFAULT_PORT        (TFS_DEFAULT_PORT + SQL_PORT_OFFSET)
#define SQL_DEFAULT_PORT_STR    "21555"

typedef enum { TX_TCP } TX_CONN_TYPE;

typedef struct {
    const char *hostname;
    uint16_t    port;
} TX_TCP_CONN_INFO;

typedef struct {
    TX_CONN_TYPE connType;
    union {
        TX_TCP_CONN_INFO tcp;
    } info;
} TX_CONN_INFO;

typedef struct {
    uint16_t port;
} TX_TCP_LIST_INFO;

typedef struct {
    uint16_t        verbose;
    TX_CONN_TYPE    connType;
    union {
        TX_TCP_LIST_INFO tcp;
    } info;
} TX_LISTEN_INFO;

typedef const void *TX_CONNECTION;

typedef struct {
    int32_t (*startup_fcn)(const char *, uint32_t);
    void    (*cleanup_fcn)(void);
    int32_t (*listen_fcn)(const TX_LISTEN_INFO *, TX_CONNECTION *);
    int32_t (*accept_fcn)(TX_CONNECTION, TX_CONNECTION *);
    int32_t (*connect_fcn)(const TX_CONN_INFO *, TX_CONNECTION *);
    void    (*discon_fcn)(TX_CONNECTION);
    int32_t (*read_fcn)(TX_CONNECTION, void *, uint32_t, uint32_t *);
    int32_t (*read_timeout_fcn)(TX_CONNECTION, void *, uint32_t, uint32_t *, uint32_t);
    int32_t (*write_fcn)(TX_CONNECTION, const void *, uint32_t);
    int32_t (*gethostname_fcn)(char *, int32_t);
    void    (*free_fcn)(TX_CONNECTION);
} TX_FCNS;



/*********************** TRANSPORT LAYER CONNECTIONS *******************/
typedef const void *TX_CONN;

const TX_FCNS * EXTERNAL_FCN tl_getTcpFunctions(void);

int32_t EXTERNAL_FCN tl_connect(const TX_CONN_INFO *, TX_CONN *);
void    EXTERNAL_FCN tl_discon(TX_CONN);
int32_t EXTERNAL_FCN tl_gethostname(const TX_CONN_INFO *, char *, int32_t);

int32_t EXTERNAL_FCN tl_send(TX_CONN);
void    EXTERNAL_FCN tl_beginReceive(TX_CONN);
int32_t EXTERNAL_FCN tl_sendAndReceive(TX_CONN);
int32_t EXTERNAL_FCN tl_sendAndReceiveTimeout(TX_CONN, uint32_t);

int32_t EXTERNAL_FCN tl_putParamChar(TX_CONN, char);
int32_t EXTERNAL_FCN tl_putParamInt16(TX_CONN, int16_t);
int32_t EXTERNAL_FCN tl_putParamInt32(TX_CONN, int32_t);
int32_t EXTERNAL_FCN tl_putParamInt64(TX_CONN, int64_t);
int32_t EXTERNAL_FCN tl_putParamFloat32(TX_CONN, float);
int32_t EXTERNAL_FCN tl_putParamFloat64(TX_CONN, double);
int32_t EXTERNAL_FCN tl_putParamString(TX_CONN, const char *);
int32_t EXTERNAL_FCN tl_putParamStringWithLength(TX_CONN, const char *);
int32_t EXTERNAL_FCN tl_putParamBytes(TX_CONN, const void *, size_t);
int32_t EXTERNAL_FCN tl_putParamUser(TX_CONN);

#define tl_putParamFcnID(c, f)    tl_putParamBytes((c), (f), sizeof(f) - 1)
#define tl_putParamFile(c, f)     tl_putParamBytes((c), &(f), sizeof(f))
#define tl_putParamDb(c, d)       tl_putParamBytes((c), &(d), sizeof(d))
#define tl_putParamIni(c, i)      tl_putParamBytes((c), &(i), sizeof(i))

#define tl_putParamUInt16(c, u16) tl_putParamInt16((c), (int16_t) (u16))
#define tl_putParamUInt32(c, u32) tl_putParamInt32((c), (int32_t) (u32))
#define tl_putParamUInt64(c, u64) tl_putParamInt64((c), (int64_t) (u64))

int32_t EXTERNAL_FCN tl_getParamChar(PSP_INPUTBUFF, char *);
int32_t EXTERNAL_FCN tl_getParamInt16(PSP_INPUTBUFF, int16_t *);
int32_t EXTERNAL_FCN tl_getParamInt32(PSP_INPUTBUFF, int32_t *);
int32_t EXTERNAL_FCN tl_getParamInt64(PSP_INPUTBUFF, int64_t *);
int32_t EXTERNAL_FCN tl_getParamFloat32(PSP_INPUTBUFF, float *);
int32_t EXTERNAL_FCN tl_getParamFloat64(PSP_INPUTBUFF, double *);
int32_t EXTERNAL_FCN tl_getParamString(PSP_INPUTBUFF, const char **);
int32_t EXTERNAL_FCN tl_getParamStringWithLength(PSP_INPUTBUFF, char *, size_t);
int32_t EXTERNAL_FCN tl_getParamBytes(PSP_INPUTBUFF, void *, size_t);
int32_t EXTERNAL_FCN tl_getParamUser(TX_CONN);

#define tl_getParamFile(c, f)     tl_getParamBytes((c), (f), sizeof(*(f)))
#define tl_getParamDb(c, d)       tl_getParamBytes((c), (d), sizeof(*(d)))
#define tl_getParamIni(c, i)      tl_getParamBytes((c), (i), sizeof(*(i)))

#define tl_getParamUInt16(c, u16) tl_getParamInt16((c), ((int16_t *) u16))
#define tl_getParamUInt32(c, u32) tl_getParamInt32((c), ((int32_t *) u32))
#define tl_getParamUInt64(c, u64) tl_getParamInt64((c), ((int64_t *) u64))


#ifdef __cplusplus
}
#endif

#endif


