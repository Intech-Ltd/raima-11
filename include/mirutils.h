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

#if !defined(__MIRUTILS_H)
#define __MIRUTILS_H

/* Replication Slave Target */
typedef enum
{
    RST_RDMS    = 0,
    RST_ORACLE  = 1,
    RST_MSSQL   = 2,
    RST_MYSQL   = 3,
    RST_INVALID = 4
} REP_SLAVE_TYPE;


typedef struct _dbrep_init_params
{
    const char      *docroot;
    uint16_t         port;
    uint16_t         verbose;
    uint16_t         diskless;
    const char       *stdout_file;
} DBREP_INIT_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbrep_init_paramsW
{
    const wchar_t   *docroot;
    uint16_t         port;
    uint16_t         verbose;
    uint16_t         diskless;
    const wchar_t   *stdout_file;
} DBREP_INIT_PARAMSW;
#endif

typedef void (*SLAVE_DONE_FCN)(int32_t);
typedef struct _dbrep_connect_params {
    DB_BOOLEAN       quiet;
    const char      *dburl;       /* url to db: name[@tfsdomain[:port]] */
    const char      *dbuserid;    /* userid to use when logging in */
    const char      *hostname;    /* hostname of slave dbmirror */
    const char      *dsn;         /* dbrepsql data source name: dsn;user;pswd*/
    int16_t          synchronize; /* 1 if this replication is synchronous.
                                     -1 if persistent sync should be removed.
                                   */
    uint16_t         port;        /* port of slave dbmirror */
    REP_SLAVE_TYPE   sql_slave_type;/* target server(RST_RDMS,RST_MYSQL..)*/
    uint16_t         override_inmem;/*force inmem master db files to disk on slave */
    SLAVE_DONE_FCN   slave_done_notifier; /* called when slave thread exits */
    const char      *stdout_file;
} DBREP_CONNECT_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbrep_connect_paramsW {
    DB_BOOLEAN       quiet;
    const wchar_t   *dburl;       /* url to db: name[@tfsdomain[:port]] */
    const wchar_t   *dbuserid;    /* userid to use when logging in */
    const wchar_t   *hostname;    /* hostname of slave dbmirror */
    const wchar_t   *dsn;         /* dbrepsql data source name: dsn;user;pswd*/
    int16_t          synchronize; /* 1 if this replication is synchronous.
                                     -1 if persistent sync should be removed.
                                   */
    uint16_t         port;        /* port of slave dbmirror */
    REP_SLAVE_TYPE   sql_slave_type;/* target server(RST_RDMS,RST_MYSQL..)*/
    uint16_t         override_inmem;/*force inmem master db files to disk on slave */
    SLAVE_DONE_FCN   slave_done_notifier; /* called when slave thread exits */
    const wchar_t   *stdout_file;
} DBREP_CONNECT_PARAMSW;
#endif

typedef struct _dbrep_disconnect_params {
    DB_BOOLEAN       quiet;
    const char      *dburl;       /* url to db: name[@tfsdomain[:port]] */
    const char      *dbuserid;    /* userid to use when logging in */
    const char      *hostname;    /* hostname of slave dbmirror */
    uint16_t         port;        /* port of slave dbmirror */
    const char      *stdout_file;
} DBREP_DISCONNECT_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbrep_disconnect_paramsW {
    DB_BOOLEAN       quiet;
    const wchar_t   *dburl;       /* url to db: name[@tfsdomain[:port]] */
    const wchar_t   *dbuserid;    /* userid to use when logging in */
    const wchar_t   *hostname;    /* hostname of slave dbmirror */
    uint16_t         port;        /* port of slave dbmirror */
    const wchar_t   *stdout_file;
} DBREP_DISCONNECT_PARAMSW;
#endif

#if defined(__cplusplus)
extern "C" {
#endif



int32_t EXTERNAL_FCN d_dbrep_start(REP_HANDLE, DB_BOOLEAN, uint16_t *);
int32_t EXTERNAL_FCN d_dbrep_stop(REP_HANDLE);
int32_t EXTERNAL_FCN d_dbrep_term(REP_HANDLE);
int32_t EXTERNAL_FCN d_dbrep_init(const DBREP_INIT_PARAMS *, REP_HANDLE *);
int32_t EXTERNAL_FCN d_dbrepsql_init(const DBREP_INIT_PARAMS *, REP_HANDLE *);
int32_t EXTERNAL_FCN d_dbmir_init(const DBREP_INIT_PARAMS *, REP_HANDLE *);
int32_t EXTERNAL_FCN d_dbrep_connect(const DBREP_CONNECT_PARAMS *, uint16_t *);
int32_t EXTERNAL_FCN d_dbrep_disconnect(const DBREP_DISCONNECT_PARAMS *);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_dbrep_initW(const DBREP_INIT_PARAMSW *, REP_HANDLE *);
int32_t EXTERNAL_FCN d_dbrepsql_initW(const DBREP_INIT_PARAMSW *, REP_HANDLE *);
int32_t EXTERNAL_FCN d_dbmir_initW(const DBREP_INIT_PARAMSW *, REP_HANDLE *);
int32_t EXTERNAL_FCN d_dbrep_connectW(const DBREP_CONNECT_PARAMSW *, uint16_t *);
int32_t EXTERNAL_FCN d_dbrep_disconnectW(const DBREP_DISCONNECT_PARAMSW *);
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* __MIRUTILS_H */

