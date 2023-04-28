/* ----------------------------------------------------------------------------
 * Raima Database Manager
 *
 * Copyright (c) 2001-2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */

/* psp.h - Contains definitions for the PSP */
#if !defined(__PSP_H)
#define __PSP_H

#define FILENMLEN   256
#define TFSPATHLEN  512
#define FILEIDLEN   148
#define LOGFILELEN  48
#define LOCKMGRLEN  48

#include "pspplat.h"

#if defined(RDM_UNIX) && !defined(RDM_VXWORKS)
#include <inttypes.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <setjmp.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define PSP_OKAY            0
#define PSP_TIMEOUT         1
#define PSP_FAILED          2
#define PSP_NOMEMORY        3
#define PSP_NOSERVER        4
#define PSP_DUPUSERID       5
#define PSP_DUPLICATE       6
#define PSP_NOSOURCE        7
#define PSP_NODEST          8
#define PSP_PRIV            9
#define PSP_NOTIMPLEMENTED 10
#define PSP_INVALID_UTF8    11

#define PSP_THREAD_LOW     -1
#define PSP_THREAD_NORMAL   0
#define PSP_THREAD_HIGH     1

int32_t EXTERNAL_FCN psp_init(void);
void    EXTERNAL_FCN psp_term(void);
void    EXTERNAL_FCN psp_rdonly(void);
void    EXTERNAL_FCN psp_nodisk(void);

/* Totally portable macro to get the number of elements in an array of
 * ANYTHING:
 */
#define RLEN(x)     (sizeof(x) / sizeof(x[0]))

#if !defined(max)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/***************************************************************************
  Memory functions
*/
typedef void *PSP_MEMTAG;
#define NULL_MEMTAG ((PSP_MEMTAG) -1)
#define TAG0        ((PSP_MEMTAG) NULL)

void       *EXTERNAL_FCN psp_getMem(size_t, PSP_MEMTAG, int32_t, const void *, const char *, uint32_t);
void       *EXTERNAL_FCN psp_extendMem(const void *, size_t, PSP_MEMTAG, int32_t, size_t, const char *, uint32_t);
void        EXTERNAL_FCN psp_freeMem(const void *, PSP_MEMTAG);
const char *EXTERNAL_FCN psp_locModId(uint32_t, char *, size_t);
int32_t     EXTERNAL_FCN psp_memCmp(const void *, const void *, size_t);
void        EXTERNAL_FCN psp_moveMemTag(const void *, PSP_MEMTAG, PSP_MEMTAG);

#define PSP_TAG_NOSEM   0x0000
#define PSP_TAG_USESEM  0x0001

PSP_MEMTAG EXTERNAL_FCN psp_createTagEx(const jmp_buf, int16_t, const char *, uint32_t);
void       EXTERNAL_FCN psp_freeTagMemory(PSP_MEMTAG, int16_t);
void       EXTERNAL_FCN psp_resetTag(const jmp_buf, PSP_MEMTAG);

#define MEM_ALLOC  1
#define MEM_CALLOC 2
#define MEM_STRDUP 3
#define MEM_MEMDUP 4
#define MEM_WCSDUP 5

#define psp_createTag(jb, fl)                 psp_createTagEx(jb, fl, __FILE__, __LINE__)

#define psp_getMemory(sz, tag)                psp_getMem(sz, (PSP_MEMTAG) tag, MEM_ALLOC, NULL, __FILE__, __LINE__)
#define psp_cGetMemory(sz, tag)               psp_getMem(sz, (PSP_MEMTAG) tag, MEM_CALLOC, NULL, __FILE__, __LINE__)
#define psp_strdup(str, tag)                  ((char *) psp_getMem(0, (PSP_MEMTAG) tag, MEM_STRDUP, str, __FILE__, __LINE__))
#if !defined(RDM_NO_UNICODE)
#define psp_wcsdup(wcs, tag)                  ((wchar_t *) psp_getMem(0, (PSP_MEMTAG) tag, MEM_WCSDUP, wcs, __FILE__, __LINE__))
#endif
#define psp_memdup(sz, mem, tag)              psp_getMem(sz, (PSP_MEMTAG) tag, MEM_MEMDUP, mem, __FILE__, __LINE__)
#define psp_extendMemory(mem, sz, tag)        psp_extendMem(mem, sz, (PSP_MEMTAG) tag, 0, 0, __FILE__, __LINE__)
#define psp_cExtendMemory(mem, newsz, incsz, tag) psp_extendMem(mem, newsz, (PSP_MEMTAG) tag, 1, incsz, __FILE__, __LINE__)
#define psp_zFreeMemory(pptr, tag)            { psp_freeMem(*pptr, (PSP_MEMTAG) tag); *pptr = NULL; }
#define psp_freeMemory(ptr, tag)              psp_freeMem(ptr, (PSP_MEMTAG) tag)

/***************************************************************************
  File functions
*/
#if !defined(PSP_FILE_OFFSET)
typedef uint64_t PSP_FILE_OFFSET;
#endif

typedef const void *PSP_FH;
typedef void       *PSP_MEM_FH;

typedef uint64_t TX_FILE;
typedef uint64_t TX_USER;
typedef uint64_t TX_DB;
typedef uint64_t TX_INI;
typedef uint64_t TXID;
#define TX_FILE_FMT UINT64_FMT
#define TX_USER_FMT UINT64_FMT
#define TX_DB_FMT   UINT64_FMT
#define TX_INI_FMT   UINT64_FMT

#define DBADDR8_FMT UINT64_FMT
#define DBADDR4_FMT "%u"

#define INVALID_TX_FILE ((TX_FILE) 0)

#if !defined(O_BINARY)
#if defined(_O_BINARY)
#define O_BINARY _O_BINARY
#else
#define O_BINARY 0
#endif
#endif

/* extended flags */
#define PSP_FLAG_DENYNO         0x0000
#define PSP_FLAG_DENYRD         0x0001
#define PSP_FLAG_DENYWR         0x0002
#define PSP_FLAG_DENYRW         (PSP_FLAG_DENYRD | PSP_FLAG_DENYWR)
#define PSP_FLAG_STICKY         0x0004
#define PSP_FLAG_INMEMORY       0x0040
#define PSP_FLAG_INMEMORY_READ  0x0080
#define PSP_FLAG_INMEMORY_WRITE 0x0100
#define PSP_FLAG_NO_SYNC        0x0200

/* Value returned by psp_fileRead(), psp_fileSeekRead(), psp_fileWrite and
   psp_fileSeekWrite() when I/O error occurs.
*/
#define PSP_IO_ERROR ((uint32_t)-1)

char           *EXTERNAL_FCN psp_currdir(void);
char           *EXTERNAL_FCN psp_fullpath(const char *);
void            EXTERNAL_FCN psp_fileClose(PSP_FH);
PSP_FILE_OFFSET EXTERNAL_FCN psp_fileLength(PSP_FH);
PSP_FILE_OFFSET EXTERNAL_FCN psp_fileLengthStat(const char *, uint16_t);
int16_t         EXTERNAL_FCN psp_fileLock(PSP_FH);
time64_t        EXTERNAL_FCN psp_fileModTimeStat(const char *, uint16_t);
void            EXTERNAL_FCN psp_fileScan(PSP_FH);
PSP_FH          EXTERNAL_FCN psp_fileOpen(const char *, uint32_t, uint16_t);
uint32_t        EXTERNAL_FCN psp_fileRead(PSP_FH, void *, uint32_t);
uint32_t        EXTERNAL_FCN psp_fileSeek(PSP_FH, PSP_FILE_OFFSET, int32_t);
uint32_t        EXTERNAL_FCN psp_fileSeekRead(PSP_FH, PSP_FILE_OFFSET, void *, uint32_t);
uint32_t        EXTERNAL_FCN psp_fileSeekWrite(PSP_FH, PSP_FILE_OFFSET, const void *, uint32_t);
int32_t         EXTERNAL_FCN psp_fileSetSize(PSP_FH, PSP_FILE_OFFSET);
uint32_t        EXTERNAL_FCN psp_fileSync(PSP_FH);
void            EXTERNAL_FCN psp_fileUnlock(PSP_FH);
uint32_t        EXTERNAL_FCN psp_fileWrite(PSP_FH, const void *, uint32_t);

int32_t         EXTERNAL_FCN psp_fileCmp(const char *, const char *);
int32_t         EXTERNAL_FCN psp_fileCmpEx(const char *, uint16_t, const char *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileCopy(const char *, const char *);
int32_t         EXTERNAL_FCN psp_fileCopyEx(const char *, uint16_t, const char *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileMove(const char *, const char *);
int32_t         EXTERNAL_FCN psp_fileMoveEx(const char *, uint16_t, const char *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileRemove(const char *);
int32_t         EXTERNAL_FCN psp_fileRemoveEx(const char *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileRename(const char *, const char *);
int32_t         EXTERNAL_FCN psp_fileRenameEx(const char *, uint16_t, const char *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileValidate(const char *);
int32_t         EXTERNAL_FCN psp_fileValidateEx(const char *, uint16_t, uint32_t *);

int32_t         EXTERNAL_FCN psp_flClose(PSP_FDESC);
int16_t         EXTERNAL_FCN psp_flOpen(const char *, uint32_t, uint16_t, PSP_FDESC *);
uint32_t        EXTERNAL_FCN psp_flRead(PSP_FDESC, void *, uint32_t);
uint32_t        EXTERNAL_FCN psp_flWrite(PSP_FDESC, const void *, uint32_t);
int32_t         EXTERNAL_FCN psp_flSetSize (PSP_FDESC desc, PSP_FILE_OFFSET size);
uint32_t        EXTERNAL_FCN psp_flSeek (PSP_FDESC desc, PSP_FILE_OFFSET addr, int32_t origin);
const char     *EXTERNAL_FCN psp_fileCreateTmpName(uint32_t *, const char *, const char *, PSP_MEMTAG);

#if !defined(RDM_NO_UNICODE)
PSP_FH          EXTERNAL_FCN psp_fileOpenW(const wchar_t *, uint32_t, uint16_t);
int32_t         EXTERNAL_FCN psp_fileCmpW(const wchar_t *, const wchar_t *);
int32_t         EXTERNAL_FCN psp_fileCmpExW(const wchar_t *, uint16_t, const wchar_t *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileCopyW(const wchar_t *, const wchar_t *);
int32_t         EXTERNAL_FCN psp_fileCopyExW(const wchar_t *, uint16_t, const wchar_t *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileMoveW(const wchar_t *, const wchar_t *);
int32_t         EXTERNAL_FCN psp_fileMoveExW(const wchar_t *, uint16_t, const wchar_t *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileRemoveW(const wchar_t *);
int32_t         EXTERNAL_FCN psp_fileRemoveExW(const wchar_t *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileRenameW(const wchar_t *, const wchar_t *);
int32_t         EXTERNAL_FCN psp_fileRenameExW(const wchar_t *, uint16_t, const wchar_t *, uint16_t);
int32_t         EXTERNAL_FCN psp_fileValidateW(const wchar_t *);
int32_t         EXTERNAL_FCN psp_fileValidateExW(const wchar_t *, uint16_t, uint32_t *);

int16_t         EXTERNAL_FCN psp_flOpenW(const wchar_t *, uint32_t, uint16_t, PSP_FDESC *);
#endif

/* Issue 3826: Do case insensitive comparisons on all platforms */
#define psp_pathcmp  stricmp
#define psp_pathncmp strnicmp
#define psp_pathchrcmp(a, b) (toupper((int32_t) (a)) == toupper((int32_t) (b)))

typedef const void *PSP_FILE_STREAM;

typedef enum {
    PSP_FILE_MODE_UNKNOWN,
    PSP_FILE_MODE_UTF8,
    PSP_FILE_MODE_UTF16
} PSP_FILE_MODE;

PSP_FILE_STREAM EXTERNAL_FCN  psp_fopen(const char *, const char *, PSP_FILE_MODE);
PSP_FILE_STREAM EXTERNAL_FCN  psp_fopenEx(const char *, const char *, PSP_FILE_MODE, uint16_t);
PSP_FILE_STREAM EXTERNAL_FCN  psp_fopen_std(const char *);
void            EXTERNAL_FCN  psp_fclose(PSP_FILE_STREAM);
void            EXTERNAL_FCN  psp_flush(void);
void            EXTERNAL_FCN  psp_flushErr(void);
int32_t         EXTERNAL_FCN  psp_fgetc(PSP_FILE_STREAM);
char           *EXTERNAL_FCN  psp_fGetLine(PSP_FILE_STREAM, char **, uint32_t *);
char           *EXTERNAL_FCN  psp_fgets(char *, int32_t, PSP_FILE_STREAM);
int32_t         EXTERNAL_FCN  psp_fputc(int32_t, PSP_FILE_STREAM);
int32_t         EXTERNAL_FCN  psp_fputs(const char *, PSP_FILE_STREAM);
int32_t         EXTVARARG_FCN psp_fprintf(PSP_FILE_STREAM, const char *, ...);
int32_t         EXTERNAL_FCN  psp_fflush(PSP_FILE_STREAM);
int32_t         EXTERNAL_FCN  psp_fseek(PSP_FILE_STREAM, int32_t, int32_t);
int32_t         EXTERNAL_FCN  psp_fungetc(int32_t, PSP_FILE_STREAM);
int32_t         EXTVARARG_FCN psp_printf(const char *, ...);
int32_t         EXTERNAL_FCN  psp_printfV(const char *, va_list);
int32_t         EXTVARARG_FCN psp_printfErr(const char *, ...);
int32_t         EXTERNAL_FCN  psp_printfErrV(const char *, va_list);
int32_t         EXTERNAL_FCN  psp_vfprintf(PSP_FILE_STREAM, const char *, va_list);

/**************************************************************************
  Logging functions
*/
#define PSP_STDOUT NULL
#define PSP_STDERR NULL
#define PSP_STDIN  NULL
typedef const void *PSP_LOG_CHUNK;

int32_t       EXTVARARG_FCN psp_log(const char *, ...);
int32_t       EXTVARARG_FCN psp_log_err(const char *, ...);
int32_t       EXTERNAL_FCN  psp_logV(const char *, va_list);
PSP_LOG_CHUNK EXTERNAL_FCN  psp_log_chunk_init(void);
void          EXTVARARG_FCN psp_log_chunk(PSP_LOG_CHUNK, const char *, ...);
void          EXTVARARG_FCN psp_log_chunk_end(PSP_LOG_CHUNK, const char *, ...);
int32_t       EXTERNAL_FCN  psp_log_open(const char *);
int32_t       EXTERNAL_FCN  psp_log_create(const char *, const char *);
void          EXTERNAL_FCN  psp_log_flush(void);
void          EXTERNAL_FCN  psp_log_close(void);
int32_t       EXTERNAL_FCN  psp_stdout_open(const char *);
void          EXTERNAL_FCN  psp_stdout_close(void);
int32_t       EXTERNAL_FCN  psp_stderr_open(const char *);
void          EXTERNAL_FCN  psp_stderr_close(void);

/**************************************************************************
  Queue declarations
*/

#define PSP_QUEUE_NORMAL   0x0000
#define PSP_QUEUE_WAIT     0x0001
#define PSP_QUEUE_PRIORITY 0x0002
#define PSP_QUEUE_EMPTY  2
#define PSP_QUEUE_CLOSED 3
typedef void    *PSP_QUEUE_HANDLE;

void    EXTERNAL_FCN psp_queueClose(PSP_QUEUE_HANDLE);
PSP_QUEUE_HANDLE EXTERNAL_FCN psp_queueCreate(const char*);
int32_t EXTERNAL_FCN psp_queueCount(PSP_QUEUE_HANDLE);
void    EXTERNAL_FCN psp_queueDelete(PSP_QUEUE_HANDLE);
PSP_QUEUE_HANDLE EXTERNAL_FCN psp_queueFind(const char *);
void    EXTERNAL_FCN psp_queuePurge(PSP_QUEUE_HANDLE, int32_t);
int16_t EXTERNAL_FCN psp_queueRead(PSP_QUEUE_HANDLE, void **, uint32_t *, int32_t *, uint32_t);
void    EXTERNAL_FCN psp_queueWrite(PSP_QUEUE_HANDLE, void *, uint32_t, int32_t, int16_t);
void    EXTERNAL_FCN psp_queueTerm(int16_t);
int16_t EXTERNAL_FCN psp_queueInit(int16_t);

/***************************************************************************
  Synchronization functions
*/
typedef const void *PSP_SEM;
#define NO_PSP_SEM ((PSP_SEM) -1)
#define PSP_INDEFINITE_WAIT ((uint32_t) (-1))

#define PSP_EXCLUSIVE 0x0000
#define PSP_SHAREABLE 0x0100

#define PSP_UNDEF_SEM 0
#define PSP_MUTEX_SEM 1
#define PSP_EVENT_SEM 2

#define psp_syncCreate(type) psp_syncCreate_check((type), __FILE__, __LINE__)

PSP_SEM EXTERNAL_FCN psp_syncCreate_check(int16_t, const char *, int32_t);
void    EXTERNAL_FCN psp_syncDelete(PSP_SEM);
void    EXTERNAL_FCN psp_syncEnterExcl(PSP_SEM);
void    EXTERNAL_FCN psp_syncExitExcl(PSP_SEM);
void    EXTERNAL_FCN psp_syncResume(PSP_SEM);
void    EXTERNAL_FCN psp_syncStart(PSP_SEM);
int16_t EXTERNAL_FCN psp_syncWait(PSP_SEM, uint32_t);

void    EXTERNAL_FCN psp_enterCritSec(void);
void    EXTERNAL_FCN psp_exitCritSec(void);

/***************************************************************************
  Memory pooling
*/
typedef struct _psp_pool_hdr {
    struct _psp_pool_hdr *next;
    size_t                size;
    uint64_t              magic;
} PSP_POOL_HDR;

typedef struct {
    PSP_POOL_HDR *next;
    PSP_MEMTAG    tag;
    PSP_SEM       sem;
    uint32_t      count;
    int16_t       static_size;
} PSP_POOL;

PSP_POOL *EXTERNAL_FCN psp_allocPool(PSP_MEMTAG, int16_t, const char *);
void     *EXTERNAL_FCN psp_getMemFromPool(size_t, PSP_POOL *, int32_t, const void *, const char *, uint32_t);
void      EXTERNAL_FCN psp_putInPool(const void *, PSP_POOL *);
void      EXTERNAL_FCN psp_zFreePool(PSP_POOL **);

#define psp_getFromPool(sz, pl)          psp_getMemFromPool((sz), (pl), MEM_ALLOC, NULL, __FILE__, __LINE__)
#define psp_cGetFromPool(sz, pl)         psp_getMemFromPool((sz), (pl), MEM_CALLOC, NULL, __FILE__, __LINE__)
#define psp_memdupFromPool(sz, data, pl) psp_getMemFromPool((sz), (pl), MEM_MEMDUP, (data), __FILE__, __LINE__)

/***************************************************************************
  UTF-8/Unicode conversion functions
*/
#if !defined(RDM_NO_UNICODE)
void     EXTERNAL_FCN psp_utf8_codes(uint32_t, uint8_t *, uint32_t *);
uint32_t EXTERNAL_FCN psp_wchar_to_utf8(char *, const wchar_t *, uint32_t, uint32_t);
uint32_t EXTERNAL_FCN psp_wchar_to_utf8_len(char *, uint32_t, const wchar_t *, uint32_t, uint32_t);
uint32_t EXTERNAL_FCN psp_utf8_to_wchar(wchar_t *, const char *, uint32_t, uint32_t);
uint32_t EXTERNAL_FCN psp_utf8_to_wchar_len(wchar_t *, uint32_t, const char *, uint32_t, uint32_t);
char    *EXTERNAL_FCN psp_convertToUTF8(const wchar_t *, uint32_t);
wchar_t *EXTERNAL_FCN psp_convertToWchar(const char *, uint32_t);
int32_t  EXTERNAL_FCN psp_convertArgvToUTF8(int32_t, const wchar_t *const *, const char ***);

#define PSP_UTF8_ERROR_BREAK    0
#define PSP_UTF8_ERROR_CONTINUE 1
#endif


/***************************************************************************
  INI file functions
*/
int32_t EXTERNAL_FCN psp_iniInit(void);
void EXTERNAL_FCN psp_iniTerm(void);
void EXTERNAL_FCN psp_iniCleanup(void);
int32_t EXTERNAL_FCN psp_readIniFile(const char *);
int32_t EXTERNAL_FCN psp_readIniFileEx(const char *, uint16_t);
int32_t EXTERNAL_FCN psp_iniSet(const char *, const char *, const char *, const char *);
int32_t EXTERNAL_FCN psp_iniSetEx(const char *, const char *, const char *, const char *, uint16_t);
int32_t EXTERNAL_FCN psp_iniGet(const char *, const char *, const char *, char *, uint32_t);
int32_t EXTERNAL_FCN psp_iniGetEx(const char *, const char *, const char *, char *, uint32_t, uint16_t);
int32_t EXTERNAL_FCN psp_iniString(const char *, const char *, const char *, char *, uint32_t);
int32_t EXTERNAL_FCN psp_iniStringEx(const char *, const char *, const char *, char *, uint32_t, uint16_t);
int32_t EXTERNAL_FCN psp_iniInt32(const char *, const char *, const char *, int32_t *);
int32_t EXTERNAL_FCN psp_iniInt32Ex(const char *, const char *, const char *, int32_t *, uint16_t);
#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN psp_readIniFileW(const wchar_t *);
int32_t EXTERNAL_FCN psp_readIniFileExW(const wchar_t *, uint16_t);
int32_t EXTERNAL_FCN psp_iniSetW(const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
int32_t EXTERNAL_FCN psp_iniSetExW(const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, uint16_t);
int32_t EXTERNAL_FCN psp_iniGetW(const wchar_t *, const wchar_t *, const wchar_t *, wchar_t *, uint32_t);
int32_t EXTERNAL_FCN psp_iniGetExW(const wchar_t *, const wchar_t *, const wchar_t *, wchar_t *, uint32_t, uint16_t);
int32_t EXTERNAL_FCN psp_iniStringW(const wchar_t *, const wchar_t *, const wchar_t *, wchar_t *, uint32_t);
int32_t EXTERNAL_FCN psp_iniStringExW(const wchar_t *, const wchar_t *, const wchar_t *, wchar_t *, uint32_t, uint16_t);
int32_t EXTERNAL_FCN psp_iniInt32W(const wchar_t *, const wchar_t *, const wchar_t *, int32_t *);
int32_t EXTERNAL_FCN psp_iniInt32ExW(const wchar_t *, const wchar_t *, const wchar_t *, int32_t *, uint16_t);
#endif

/***************************************************************************
  Communication functions
*/
typedef const void *PSP_LMC;
typedef const void *PSP_CONN;

typedef int32_t (LMC_SEND_FCN)(const void *, uint32_t, const void *);
typedef int32_t (LMC_RECV_FCN)(void **, uint32_t *, const void *);

typedef int32_t (LMC_CHECKID_FCN)(const char *);
typedef int32_t (LMC_CONNECT_FCN)(const char *, const char *, const char *, void **);
typedef void    (LMC_DISCONNECT_FCN)(const void *);

typedef int32_t (LMC_LISTEN_FCN)(const char *, const char *, uint32_t,
        void **);
typedef void   *(LMC_WAITMSG_FCN)(void *, int32_t);
typedef void    (LMC_STOPLISTEN_FCN)(void *);
typedef void    (LMC_DISCONCLIENT_FCN)(const void *, const void *);
typedef void    (LMC_INFO_FCN)(const void *, char *, char *, char *);

typedef struct {
    const char           *name;
    LMC_SEND_FCN         *send;
    LMC_RECV_FCN         *recv;

    LMC_CHECKID_FCN      *checkid;
    LMC_CONNECT_FCN      *connect;
    LMC_DISCONNECT_FCN   *disconnect;

    LMC_LISTEN_FCN       *listen;
    LMC_WAITMSG_FCN      *waitmsg;
    LMC_STOPLISTEN_FCN   *stoplisten;
    LMC_DISCONCLIENT_FCN *disconclient;

    LMC_INFO_FCN         *info;
} LOCKCOMM_FCNS;

typedef int32_t (LMC_AVAIL_FCN)(int32_t *, const LOCKCOMM_FCNS **);

#define PSP_FLAG_NETWORK_ORDER 0x0001
#define PSP_FLAG_NO_FILE_LOCKS 0x0004

/* Alignment calculations */
typedef enum {
    ALN_CHAR, ALN_INT16, ALN_WCHAR, ALN_INT32, ALN_FLOAT, ALN_INT64, ALN_DOUBLE
} ALIGN_TYPE;


uint16_t EXTERNAL_FCN psp_swaporder16(uint16_t);
uint32_t EXTERNAL_FCN psp_swaporder32(uint32_t);
uint64_t EXTERNAL_FCN psp_swaporder64(uint64_t);
float    EXTERNAL_FCN psp_swaporderf32(float);
double   EXTERNAL_FCN psp_swaporderf64(double);

#if defined(RDM_BIGENDIAN)
#define psp_ntoh_i16(v) (v)
#define psp_ntoh_i32(v) (v)
#define psp_ntoh_i64(v) (v)
#define psp_ntoh_u16(v) (v)
#define psp_ntoh_u32(v) (v)
#define psp_ntoh_u64(v) (v)
#define psp_ntoh_f32(v) (v)
#define psp_ntoh_f64(v) (v)
#define psp_hton_i16(v) (v)
#define psp_hton_i32(v) (v)
#define psp_hton_i64(v) (v)
#define psp_hton_u16(v) (v)
#define psp_hton_u32(v) (v)
#define psp_hton_u64(v) (v)
#define psp_hton_f32(v) (v)
#define psp_hton_f64(v) (v)
#else
#define psp_ntoh_i16(v) (int16_t) psp_swaporder16((uint16_t) v)
#define psp_ntoh_i32(v) (int32_t) psp_swaporder32((uint32_t) v)
#define psp_ntoh_i64(v) (int64_t) psp_swaporder64((uint64_t) v)
#define psp_ntoh_u16(v) psp_swaporder16(v)
#define psp_ntoh_u32(v) psp_swaporder32(v)
#define psp_ntoh_u64(v) psp_swaporder64(v)
#define psp_ntoh_f32(v) psp_swaporderf32(v)
#define psp_ntoh_f64(v) psp_swaporderf64(v)
#define psp_hton_i16(v) (int16_t) psp_swaporder16((uint16_t) v)
#define psp_hton_i32(v) (int32_t) psp_swaporder32((uint32_t) v)
#define psp_hton_i64(v) (int64_t) psp_swaporder64((uint64_t) v)
#define psp_hton_u16(v) psp_swaporder16(v)
#define psp_hton_u32(v) psp_swaporder32(v)
#define psp_hton_u64(v) psp_swaporder64(v)
#define psp_hton_f32(v) psp_swaporderf32(v)
#define psp_hton_f64(v) psp_swaporderf64(v)
#endif

#define psp_ntoh_faddr  psp_ntoh_u64
#define psp_hton_faddr  psp_hton_u64
#if defined(RDM_WCHAR_IS_UCS4)
#define psp_ntoh_wchar(w)  (wchar_t) psp_ntoh_u32((uint32_t) w)
#define psp_hton_wchar(w)  (wchar_t) psp_hton_u32((uint32_t) w)
#elif defined(RDM_WCHAR_IS_UTF16)
#define psp_ntoh_wchar(w)  (wchar_t) psp_ntoh_u16((uint16_t) w)
#define psp_hton_wchar(w)  (wchar_t) psp_hton_u16((uint16_t) w)
#endif

/***************************************************************************
  Thread and Process functions
*/
typedef void (*THREAD_FCN)(void *);
uint32_t     EXTERNAL_FCN psp_get_pid(void);
void         EXTERNAL_FCN psp_sleep(uint32_t);
int16_t      EXTERNAL_FCN psp_threadBegin(THREAD_FCN, uint32_t, void *);
int16_t      EXTERNAL_FCN psp_threadBeginEx(THREAD_FCN, uint32_t, void *, psp_thread_t *, int32_t, const char *name);
psp_thread_t EXTERNAL_FCN psp_threadId(void);
int16_t      EXTERNAL_FCN psp_threadSetPriority(psp_thread_t, int32_t);
void         EXTERNAL_FCN psp_threadEnd(void);
int32_t EXTERNAL_FCN psp_threadJoin (psp_thread_t);

typedef const void *PSP_DAEMON_CTX;
int32_t      EXTERNAL_FCN psp_daemonStart(const char *, PSP_DAEMON_CTX *);
void         EXTERNAL_FCN psp_daemonComplete(PSP_DAEMON_CTX);

int32_t EXTERNAL_FCN psp_serviceStart(const char *, uint16_t *, int32_t, const char *const *, int32_t (EXTERNAL_FCN *)(int32_t, const char *const *));
void    EXTERNAL_FCN psp_serviceStarted(void);
void    EXTERNAL_FCN psp_serviceDone(void);

int32_t EXTERNAL_FCN psp_serviceInstall(const char *, const char *, const char *, const char *, const char *, const char *);
int32_t EXTERNAL_FCN psp_serviceUninstall(const char *, const char *);


#if defined(RDM_VXWORKS) || defined(RDM_UC_OS_II)
#define PSP_DEFAULT_STACKSIZE  0x8000
#else
#define PSP_DEFAULT_STACKSIZE  0
#endif

/***************************************************************************
  Path (File and Directory) functions
*/

int32_t        EXTERNAL_FCN psp_dirCreate(const char *);
int32_t        EXTERNAL_FCN psp_dirCreateAll(const char *);
int32_t        EXTERNAL_FCN psp_dirRemove(const char *);
int32_t        EXTERNAL_FCN psp_dirMove(const char *, const char *);
int32_t        EXTERNAL_FCN psp_dirMoveEx(const char *, uint16_t, const char *, uint16_t);

#if !defined(RDM_NO_UNICODE)
int32_t        EXTERNAL_FCN psp_dirCreateW(const wchar_t *);
int32_t        EXTERNAL_FCN psp_dirCreateAllW(const wchar_t *);
int32_t        EXTERNAL_FCN psp_dirRemoveW(const wchar_t *);
int32_t        EXTERNAL_FCN psp_dirMoveW(const wchar_t *, const wchar_t *);
int32_t        EXTERNAL_FCN psp_dirMoveExW(const wchar_t *, uint16_t, const wchar_t *, uint16_t);
#endif

PSP_DIR        EXTERNAL_FCN psp_pathOpen(const char *, const char *);
PSP_DIR        EXTERNAL_FCN psp_pathOpenEx(const char *, uint16_t, const char *);
const char    *EXTERNAL_FCN psp_pathNext(PSP_DIR, uint32_t *);
void           EXTERNAL_FCN psp_pathClose(PSP_DIR);

const char    *EXTERNAL_FCN psp_pathDefTmp(void);
const char    *EXTERNAL_FCN psp_pathGetTmp(void);
int16_t        EXTERNAL_FCN psp_pathSetTmp(char *);
int32_t        EXTERNAL_FCN psp_pathIsDir(const char *);
uint8_t        EXTERNAL_FCN psp_pathIsLink(const char *);
int32_t        EXTERNAL_FCN psp_pathIsFullyQualified(const char *);
void           EXTERNAL_FCN psp_pathSplit(const char *, char **, char **);
const char    *EXTERNAL_FCN psp_pathStripDrive(const char *);

#if !defined(RDM_NO_UNICODE)
PSP_DIR        EXTERNAL_FCN psp_pathOpenW(const wchar_t *, const wchar_t *);
PSP_DIR        EXTERNAL_FCN psp_pathOpenExW(const wchar_t *, uint16_t, const wchar_t *);
const wchar_t *EXTERNAL_FCN psp_pathNextW(PSP_DIR, uint32_t *);
#endif

#if defined(NEED_TEMPNAM)
char         *EXTERNAL_FCN tempnam(char *, char *);
#endif

/***************************************************************************
  Miscellaneous functions
*/
typedef void (INTERRUPT_FCN)(void);
int32_t     EXTERNAL_FCN psp_errno(void);
void        EXTERNAL_FCN psp_errPrint(const char *);
const char *EXTERNAL_FCN psp_getenv(const char *);
int32_t     EXTERNAL_FCN psp_seed(void);
struct tm  *EXTERNAL_FCN psp_timeLocaltime(time64_t, struct tm *);
time64_t    EXTERNAL_FCN psp_timeSecs(void);
uint32_t    EXTERNAL_FCN psp_timeMilliSecs(void);
uint64_t    EXTERNAL_FCN psp_timeNanoSecs(void);
const char *EXTERNAL_FCN psp_timeCtime(time64_t);
const char *EXTERNAL_FCN psp_tmpdir(PSP_MEMTAG);
int32_t     EXTERNAL_FCN psp_handleInterrupt(INTERRUPT_FCN *);

char       *EXTERNAL_FCN psp_setlocale(int32_t, const char *);
void        EXTERNAL_FCN psp_version(int16_t *, int16_t *, int16_t *);
ALIGN_TYPE  EXTERNAL_FCN psp_alignType(char);
uint16_t    EXTERNAL_FCN psp_align(uint16_t, ALIGN_TYPE);

#if !defined(RDM_NO_UNICODE)
const wchar_t *EXTERNAL_FCN psp_getenvW(const wchar_t *);
wchar_t       *EXTERNAL_FCN psp_setlocaleW(int32_t, const wchar_t *);
#endif

#if defined(NEED_STRUPR)
char    *EXTERNAL_FCN strlwr(char *);
char    *EXTERNAL_FCN strupr(char *);
#if !defined(RDM_NO_UNICODE)
wchar_t *EXTERNAL_FCN wcslwr(wchar_t *);
wchar_t *EXTERNAL_FCN wcsupr(wchar_t *);
#endif
#endif

#if defined(NEED_STRNCMP)
int32_t EXTERNAL_FCN psp_strncmp (const char *s1, const char *s2, size_t len);
#else
#define psp_strncmp(s1,s2,n) strncmp(s1,s2,n)
#endif
    
#if defined(NEED_PSP_ATOI64)
int64_t psp_atoi64 (const char * s);
#endif

#if defined(NEED_PSP_ATOUI64)
uint64_t psp_atoui64 (const char * s);
#endif

char    *EXTERNAL_FCN psp_strncpyZ(char *, const char *, size_t); /*lint -esym(534,psp_strncpyZ) */
#if !defined(RDM_NO_UNICODE)
wchar_t *EXTERNAL_FCN psp_wcsncpyZ(wchar_t *, const wchar_t *, size_t); /*lint -esym(534,psp_wcsncpyZ) */
#endif
double   EXTERNAL_FCN psp_round(double);

int   EXTVARARG_FCN psp_snprintf(char *, size_t, const char *, ...); /*lint -esym(534,psp_snprintf) */
int   EXTERNAL_FCN  psp_vsnprintf(char *, size_t, const char *, va_list);
char *EXTVARARG_FCN psp_makeStrVA(const char *, uint32_t, PSP_MEMTAG, const char *, ...);
#if defined(RDM_NO_VARIADIC_MACROS) || defined(RDM_WINDOWS) || defined(RDM_WINCE)
char *EXTVARARG_FCN psp_makeStr(PSP_MEMTAG, const char *, ...);
char *EXTVARARG_FCN psp_makeString(const char *, ...);
#endif

int32_t EXTERNAL_FCN psp_isPurifyRunning(void);
#if defined(RDM_WINDOWS) && defined(RDM_X86) && !defined(RELEASE)
#define RDM_PURIFY_CHECK
#endif

#if !defined(RDM_NO_VARIADIC_MACROS)
#define psp_makeStr(tag, fmt, ...) psp_makeStrVA(__FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define psp_makeString(fmt, ...) psp_makeStrVA(__FILE__, __LINE__, TAG0, fmt, ##__VA_ARGS__)
#endif

/* User table status */
#define U_EMPTY         0
#define U_LIVE          1
#if defined(RDM_HAS_XLOCKS)
#define U_HOLDING_X     6
#endif

/* Buffered I/O Handling */
typedef int32_t (*PSP_BUFFREADFCN)(const void *, void *, uint32_t, uint32_t *);
typedef int32_t (*PSP_BUFFREADTIMEOUTFCN)(const void *, void *, uint32_t, uint32_t *, uint32_t);
typedef int32_t (*PSP_BUFFWRITEFCN)(const void *, const void *, uint32_t);

typedef const void *PSP_INPUTBUFF;
typedef const void *PSP_OUTPUTBUFF;

int32_t EXTERNAL_FCN psp_inputBuffCreate(PSP_BUFFREADFCN, PSP_BUFFREADTIMEOUTFCN, const void *, int16_t, PSP_INPUTBUFF *);
void    EXTERNAL_FCN psp_inputBuffDestroy(PSP_INPUTBUFF);
int32_t EXTERNAL_FCN psp_inputBuffGetChar(PSP_INPUTBUFF, char *);
int32_t EXTERNAL_FCN psp_inputBuffGetInt16(PSP_INPUTBUFF, int16_t *);
int32_t EXTERNAL_FCN psp_inputBuffGetInt32(PSP_INPUTBUFF, int32_t *);
int32_t EXTERNAL_FCN psp_inputBuffGetInt32Timeout(PSP_INPUTBUFF, int32_t *, uint32_t);
int32_t EXTERNAL_FCN psp_inputBuffGetInt64(PSP_INPUTBUFF, int64_t *);
int32_t EXTERNAL_FCN psp_inputBuffGetFloat32(PSP_INPUTBUFF, float *);
int32_t EXTERNAL_FCN psp_inputBuffGetFloat64(PSP_INPUTBUFF, double *);
int32_t EXTERNAL_FCN psp_inputBuffGetStringWithLength(PSP_INPUTBUFF, char *, size_t);
int32_t EXTERNAL_FCN psp_inputBuffGetString(PSP_INPUTBUFF, const char **);
int32_t EXTERNAL_FCN psp_inputBuffGetBytes(PSP_INPUTBUFF, void *, size_t);
void    EXTERNAL_FCN psp_inputBuffReset(PSP_INPUTBUFF);

#define psp_inputBuffGetUser(b, u)     psp_inputBuffGetBytes((b), (u), sizeof(*(u)))
#define psp_inputBuffGetFile(b, f)     psp_inputBuffGetBytes((b), (f), sizeof(*(f)))
#define psp_inputBuffGetDb(b, d)       psp_inputBuffGetBytes((b), (d), sizeof(*(d)))
#define psp_inputBuffGetIni(b, i)      psp_inputBuffGetBytes((b), (i), sizeof(*(i)))

#define psp_inputBuffGetInt8(b, i8)    psp_inputBuffGetChar((b), ((char *) (i8)))
#define psp_inputBuffGetUInt8(b, u8)   psp_inputBuffGetChar((b), ((char *) (u8)))
#define psp_inputBuffGetUInt16(b, u16) psp_inputBuffGetInt16((b), ((int16_t *) (u16)))
#define psp_inputBuffGetUInt32(b, u32) psp_inputBuffGetInt32((b), ((int32_t *) (u32)))
#define psp_inputBuffGetUInt64(b, u64) psp_inputBuffGetInt64((b), ((int64_t *) (u64)))

int32_t EXTERNAL_FCN psp_outputBuffCreate(PSP_BUFFWRITEFCN, const void *, int16_t, PSP_OUTPUTBUFF *);
void    EXTERNAL_FCN psp_outputBuffDestroy(PSP_OUTPUTBUFF);
int32_t EXTERNAL_FCN psp_outputBuffFlush(PSP_OUTPUTBUFF);
int32_t EXTERNAL_FCN psp_outputBuffPutChar(PSP_OUTPUTBUFF, char);
int32_t EXTERNAL_FCN psp_outputBuffPutInt16(PSP_OUTPUTBUFF, int16_t);
int32_t EXTERNAL_FCN psp_outputBuffPutInt32(PSP_OUTPUTBUFF, int32_t);
int32_t EXTERNAL_FCN psp_outputBuffPutInt64(PSP_OUTPUTBUFF, int64_t);
int32_t EXTERNAL_FCN psp_outputBuffPutFloat32(PSP_OUTPUTBUFF, float);
int32_t EXTERNAL_FCN psp_outputBuffPutFloat64(PSP_OUTPUTBUFF, double);
int32_t EXTERNAL_FCN psp_outputBuffPutString(PSP_OUTPUTBUFF, const char *);
int32_t EXTERNAL_FCN psp_outputBuffPutStringWithLength(PSP_OUTPUTBUFF, const char *);
int32_t EXTERNAL_FCN psp_outputBuffPutBytes(PSP_OUTPUTBUFF, const void *, size_t);
int32_t EXTERNAL_FCN psp_outputBuffSendDirect(PSP_OUTPUTBUFF, const void *, size_t);

#define psp_outputBuffPutFcnID(b, f)    psp_outputBuffPutBytes((b), (f), sizeof(f) - 1)
#define psp_outputBuffPutUser(b, u)     psp_outputBuffPutBytes((b), &(u), sizeof(u))
#define psp_outputBuffPutFile(b, f)     psp_outputBuffPutBytes((b), &(f), sizeof(f))
#define psp_outputBuffPutDb(b, d)       psp_outputBuffPutBytes((b), &(d), sizeof(d))
#define psp_outputBuffPutIni(b, i)      psp_outputBuffPutBytes((b), &(i), sizeof(i))

#define psp_outputBuffPutInt8(b, i8)    psp_outputBuffPutChar((b), (char) (u8))
#define psp_outputBuffPutUInt8(b, u8)   psp_outputBuffPutChar((b), (char) (i8))
#define psp_outputBuffPutUInt16(b, u16) psp_outputBuffPutInt16((b), (int16_t) (u16))
#define psp_outputBuffPutUInt32(b, u32) psp_outputBuffPutInt32((b), (int32_t) (u32))
#define psp_outputBuffPutUInt64(b, u64) psp_outputBuffPutInt64((b), (int64_t) (u64))

/* Parse definitions for psp_yyparse */
typedef void *PSP_PYYCONTEXT;
typedef void *PSP_PYYSTYPE;

typedef int16_t (*PSP_YYLEX)(PSP_PYYCONTEXT, PSP_PYYSTYPE);
typedef int16_t (*PSP_YYACT)(PSP_PYYCONTEXT, PSP_PYYSTYPE, int16_t, PSP_PYYSTYPE);
typedef void    (*PSP_YYSYNTAXERR)(PSP_PYYCONTEXT);
typedef void    (*PSP_YYDEBUG)(PSP_PYYCONTEXT, char *, ...);

#if defined(YYDEBUG)
typedef struct {
    char   *t_name;
    int16_t t_val;
} PSP_YYTOKTYPE;
#endif

typedef struct {
    int16_t              yyerrcode;
    int16_t              yylast;
    const int16_t       *yyexca;
    const int16_t       *yyact;
    const int16_t       *yypact;
    const int16_t       *yypgo;
    const uint16_t      *yyr1;
    const uint16_t      *yyr2;
    const int16_t       *yychk;
    const int16_t       *yydef;
#if defined(YYDEBUG)
    const PSP_YYTOKTYPE *yytoks;
    const char   *const *yyreds;
#endif
} PSP_YYTABLES;

typedef struct {
    uint32_t            yytoksize;
    PSP_PYYCONTEXT      yyctx;
    const PSP_YYTABLES *yytab;
    PSP_YYLEX           yylex;
    PSP_YYACT           yyacts;
    PSP_YYSYNTAXERR     yysyntaxerr;
#if defined(YYDEBUG)
    PSP_YYDEBUG         yydebug;
#endif
    PSP_MEMTAG          yytag;
} PSP_YYPDATA;

int16_t EXTERNAL_FCN psp_yyparse(const PSP_YYPDATA *);

/* Random number generator */
typedef const void *PSP_RAND;

PSP_RAND EXTERNAL_FCN psp_randCreate(uint32_t seed);
void     EXTERNAL_FCN psp_randDestroy(PSP_RAND);
int32_t  EXTERNAL_FCN psp_randGet(PSP_RAND);
uint32_t EXTERNAL_FCN psp_randGetUInt32(PSP_RAND);
uint64_t EXTERNAL_FCN psp_randGetUInt64(PSP_RAND);
double   EXTERNAL_FCN psp_randGetDouble(PSP_RAND);
void     EXTERNAL_FCN psp_randResetSeed(PSP_RAND, uint32_t seed);

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

#if defined(__cplusplus)
}
#endif

#endif  /* __PSP_H */

