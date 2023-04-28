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

#if !defined(__RDMHTTP_H)
#define __RDMHTTP_H

#include "rdm.h"
#include "transport.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DEFAULT_CONTENT_LEN      0xFFFF
#define DEFAULT_MAX_BUFFER_SIZE  4096

#define RHS_MAGIC 0x8DEAF807

/*********************** HTTP xapi functions *****************************/

typedef int32_t  (EXTERNAL_FCN HTTP_HANDLER)(void *);
typedef int32_t  (EXTERNAL_FCN DISCON_HANDLER)(TX_USER, void *);


/************************************************************************/

typedef struct rhs_params {
    char                   *docroot;
    uint32_t                rd_only;
    uint32_t                diskless;
    uint32_t                maxContentLen;
    uint32_t                maxBufferSize; /* user defined internal buffer size */
    uint16_t                port;
    uint16_t                verbose;
    uint32_t                stackSize;
    uint16_t                nostartup;
    TFS_XAPI_PARAMS         xapi_params; /* xapi functions */
    HTTP_HANDLER           *reqhandler;  /* TFS or HTTP request handler fcn */
    DISCON_HANDLER         *discon_fcn;  /* called when connection closed */
} RHS_PARAMS;

/* HTTP access list, stored in globalCtx->accessList */
typedef struct _userlist {
    char                *username;    /* user name */
    void                *enc;         /* Encryption ctx with AES key */
    time64_t             last_access; /* last time accessed, used for timeout */
    struct _userlist    *next;        /* next user logged in */
} RHS_USERLIST;

typedef struct {
    uint16_t         sigRec;
    uint16_t         thread_done;   /* StartServer thread done */
    uint32_t         magic;
    psp_thread_t     TID;           /* http_StartServer thread id */
    uint16_t        *server_done;   /* http_StartServer caller's variable */
    const TX_FCNS   *fcns;
    RHS_PARAMS      *params;
    TX_CONNECTION    list;
    void            *serverCtx;
    RHS_USERLIST    *accessList;          /* list of logged in HTTP users */
    PSP_SEM          sem;
    PSP_SEM          eventSem;
    int32_t          fcnRet;
    void            *userdata;
    PSP_FILE_STREAM *pStdout;
    PSP_FILE_STREAM *pStderr;
} RHS_GLOBAL_CTX;

typedef struct rhs_conn_ctx {
    uint8_t                httptype;      /* TFS, GET, POST or HEAD */
    uint8_t                conttype;      /* HTTP Content-Type */
    uint32_t               contlen;       /* HTTP Content-Length */
    char                  *contboundary;  /* HTTP content-type boundary string */
    char                  *pURL;          /* HTTP request URL, URL decoded */
    char                  *pRawURL;       /* HTTP URL un-urldecoded */
    char                  *vfyname;
    uint16_t               vfyxflags;
    uint32_t               verifyID;
    uint32_t               vCode;
    TX_USER                userID;
    PSP_INPUTBUFF          inBuff;
    PSP_OUTPUTBUFF         outBuff;
    TX_CONNECTION          conn;
    char                  *line;
    uint32_t               linepos;
    uint32_t               linesize;
    void                  *buf;
    uint32_t               bufsize;
    void                  *userdata;      /* connection specific data */
    RHS_GLOBAL_CTX        *globalCtx;
} RHS_CONN_CTX;

/********************** URL Args ****************************************/

/* Name/value pairs from parsed URL argument */
typedef struct {
    char                *name;      /* pointer to name in arg string */
    char                *value;     /* pointer to value in arg string */
} NVPAIRS;

/* Array of name/value pairs from url args */
typedef struct {
    RHS_HANDLE          hRHS;       /* RDM HTTP handle */
    uint32_t            numpairs;   /* number of name/value pairs in argstr */
    char               *argcopy;    /* copy of arg string */
    NVPAIRS            *nvpairs;    /* name/value pairs */
} URLARGS;


/********************* External functions *******************************/

void       *EXTERNAL_FCN validateConnBuff(RHS_CONN_CTX *, uint32_t);
void        EXTERNAL_FCN http_freeURLArgs(const URLARGS *);
const char *EXTERNAL_FCN http_getURLArgValue(const char *, const URLARGS *);
int32_t     EXTERNAL_FCN http_initURLArgs(RHS_HANDLE, const char *, const URLARGS **);
int32_t     EXTERNAL_FCN http_printf(PSP_OUTPUTBUFF, const char *, ...);
int32_t     EXTERNAL_FCN http_InitServer(RHS_PARAMS *, void *, RHS_HANDLE *);
int32_t     EXTERNAL_FCN http_StartServer(RHS_HANDLE, DB_BOOLEAN, uint16_t *);
int32_t     EXTERNAL_FCN http_StopServer(RHS_HANDLE);
void        EXTERNAL_FCN http_CloseServer(RHS_HANDLE);
void        EXTERNAL_FCN http_URLDecode(char *, uint32_t *);
int32_t     EXTERNAL_FCN http_EndConnection(RHS_HANDLE, TX_USER);

int32_t     EXTERNAL_FCN http_getUserLogin(RHS_HANDLE, const char *, time64_t, void **);
int32_t     EXTERNAL_FCN http_setUserLogin(RHS_HANDLE, const char *, void *);
int32_t     EXTERNAL_FCN http_removeUserLogin(RHS_HANDLE, const char *);

int32_t     EXTERNAL_FCN http_getGlobalDiskless(RHS_HANDLE, uint32_t *);
int32_t     EXTERNAL_FCN http_getGlobalDocroot(RHS_HANDLE, const char **);
int32_t     EXTERNAL_FCN http_getGlobalPort(RHS_HANDLE, uint16_t *);
int32_t     EXTERNAL_FCN http_getGlobalUserdata(RHS_HANDLE, void **);


/************************** Errors **************************************/
#define RE_BADREQUEST       (-500)
#define RE_NOTIMPLEMENTED   (-501)
#define RE_URITOOLONG       (-502)
#define RE_ENTTOOLARGE      (-503)
#define RE_NOLENGTH         (-504)

#if defined(__cplusplus)
}
#endif

#endif /* __RDMHTTP_H */

