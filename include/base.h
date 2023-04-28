/* ----------------------------------------------------------------------------
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */

#if !defined(BASE_H)
#define BASE_H

#include "psp.h"        /* for TXID */

#if defined(__cplusplus)
extern "C" {
#endif

/* =========================================================================
   Command line handling definitions
*/

/* Mode flags for cmdline_opt.opttype */

#define ARG_END         0
#define ARG_OPT         1
#define ARG_FINAL       2
#define ARG_FINAL_MULTI 3
#define ARG_FINAL_OPT   4
#define ARG_DEPRECATED  5

#define B_CMD_OKAY     (0)
#define B_CMD_USAGE    (1)
#define B_CMD_VERSION  (2)
#define B_CMD_ERROR   (-1)

/* tfs types */
typedef enum {
    TFS_TYPE_TFS, /* TFST: built-in tfs */
    TFS_TYPE_RPC, /* TFSR: remote client */
    TFS_TYPE_STANDALONE, /* TFSS: standalone tfs */ 
    TFS_TYPE_DEFAULT /* default configuration - tfst */
} TFS_TYPE;

typedef struct _cmdline_status 
{
    uint16_t quiet;
    uint16_t stdout_opened;
    uint16_t stderr_opened;
    TFS_TYPE tfs_type;
} cmdline_status;

typedef struct _default_opts 
{
    uint8_t no_help;
    uint8_t no_banner;
    uint8_t no_version;
    uint8_t no_quiet;
    uint8_t no_stdout;
    uint8_t no_stderr;
    uint8_t no_tfs;
} default_opts;

/* Generic usage function option record */
typedef struct tag_cmdline_opt {
    int16_t     opttype;  /* type of option record, see above for choices */
    const char *opt;
    const char *optarg;
    const char *desc;     /* option description string (may be NULL) */
    int32_t     descval;  /* integer for %d (if any) in desc. string */
    int16_t     state;    /* private - processing state */
    int16_t     next_arg; /* private - processing state, only for '*' */
    int32_t     optcnt;   /* private - number of instances, only for '*' */
    union {
        const char  *arg;
        int32_t      final_id;
        const char **arglist;
    } data;                    /* private */
} cmdline_opt;

#define HELP_ARG    "-h"
#define BANNER_ARG  "-B"
#define VERSION_ARG "-V"
#define QUIET_ARG   "-q"
#define TFS_ARG     "-tfs type"
#define STDOUT_ARG  "-stdout file"
#define STDERR_ARG  "-stderr file"

#define OPT_CHAR    "-"

#if defined(RDM_UNIX)
#define CHARSINCRLF 1
#else
#define CHARSINCRLF 2
#endif

#define HELP_DESC    "Display this usage information"
#define BANNER_DESC  "Do not display the banner"
#define VERSION_DESC "Display the version information"
#define QUIET_DESC   "Quiet mode.  No information will be displayed"
#define TFS_DESC     "Specify the TFS type to use (TFST, TFSR, or TFSS) [default = TFST]"
#define STDOUT_DESC  "Redirect stdout to the specified file"
#define STDERR_DESC  "Redirect stderr to the specified file"

int32_t EXTERNAL_FCN b_cmd_finaloptpos(
    const cmdline_opt *opts);

int32_t EXTERNAL_FCN b_cmd_initopt( /*lint -esym(534,b_cmd_initopt*) */
    const char         *banner,
    int32_t             argc,
    const char *const   argv[],
    cmdline_opt        *opts,
    const default_opts *defaults,
    cmdline_status     *status);

char EXTERNAL_FCN b_cmd_nextoptchr(
    cmdline_opt *opts,
    const char **arg);

const char *EXTERNAL_FCN b_cmd_nextoptstr(
    cmdline_opt *opts,
    const char **arg);

int32_t EXTERNAL_FCN b_cmd_nextoptidx(
    cmdline_opt *opts,
    const char **arg);

int32_t EXTERNAL_FCN b_cmd_std_usage(  /*lint -esym(534,b_cmd_std_usage*) */
    const char         *progname,
    const char         *errmsg,
    const cmdline_opt  *opts,
    const default_opts *defaults);

int32_t EXTERNAL_FCN b_cmd_expand(
    int32_t            argc,
    const char  *const argv[],
    int32_t           *exArgc,
    const char *const *exArgv[]);

void EXTERNAL_FCN b_cmd_freeArgv(
    int32_t           argc,
    const char *const argv[]);

/* =========================================================================
   Log file handling defintions
*/

/* Database File Location - used if mirrored slave has database files located
   opposite from master (ie: in-memory to on-disk)
*/
typedef enum {  DFL_DEFAULT = 0,
                DFL_INMEM,
                DFL_DISK
             } DFL_TYPE;
                    

typedef const void *B_LOG_FH;
typedef const void *B_LOG_DATA;

#define B_LOG_VERSION 1

#define LOG_FH_NULL ((B_LOG_FH) 0)

#define LOG_EXTENSION       "log"
#define REPLOG_EXTENSION    "rep"
#define HOLD_EXTENSION      "hold"
#define ARCHIVE_EXTENSION   "arch"
#define PRELOG_EXTENSION    "prelog"
#define REPPRELOG_EXTENSION "rep_prelog"
#define REPTMPLOG_EXTENSION "rep_tmplog"
#define DBLOG_EXTENSION     "db"

/* We specify 8 bytes for the transaction id, but it can actually be 16 bytes
 * long.  */
#define COMMITTED_LOG_FILE UINT64_SPEC_SIZE_HEX("08") "." LOG_EXTENSION
#define COMMITTED_REPLOG_FILE COMMITTED_LOG_FILE "." REPLOG_EXTENSION
#define COMMITTED_REPTMP_FILE UINT64_SPEC_SIZE_HEX("08") "." REPTMPLOG_EXTENSION
#define HELD_LOG_FILE      COMMITTED_LOG_FILE "." HOLD_EXTENSION
#define ARCHIVE_LOG_FILE   COMMITTED_LOG_FILE "." ARCHIVE_EXTENSION
#define DATABASE_LOG_FILE  UINT64_SPEC_SIZE_HEX("08") "." UINT64_SPEC_SIZE_HEX("04") "." LOG_EXTENSION "." DBLOG_EXTENSION

#define COMMITTED_LOG_FILE_PATTERN "*." LOG_EXTENSION
#define HELD_LOG_FILE_PATTERN      COMMITTED_LOG_FILE_PATTERN "." HOLD_EXTENSION
#define ARCHIVE_LOG_FILE_PATTERN   COMMITTED_LOG_FILE_PATTERN "." ARCHIVE_EXTENSION
#define REPLOG_FILE_PATTERN        COMMITTED_LOG_FILE_PATTERN "." REPLOG_EXTENSION
#define DB_LOG_FILE_PATTERN        COMMITTED_LOG_FILE_PATTERN "." DBLOG_EXTENSION
#define ALL_LOG_FILE_PATTERN       COMMITTED_LOG_FILE_PATTERN "*"
#define PRE_LOG_FILE_PATTERN       "*." PRELOG_EXTENSION
#define REPPRE_LOG_FILE_PATTERN    "*." REPPRELOG_EXTENSION
#define REPTMP_LOG_FILE_PATTERN    "*." REPTMPLOG_EXTENSION
#define CLEANUP_LOG_FILE_PATTERN   COMMITTED_LOG_FILE_PATTERN ".*"

typedef enum { COMMITTED, HELD, ARCHIVE, REP, DB } LOG_FILE_TYPE;

typedef enum { NO_COMPRESSION=0, ZERO_COMPRESSION } LOG_COMPRESSION_TYPE;

int32_t EXTERNAL_FCN b_log_init(B_LOG_DATA *);
void    EXTERNAL_FCN b_log_term(B_LOG_DATA);
int32_t EXTERNAL_FCN b_log_open(const char *, uint16_t, char *, size_t, B_LOG_FH *, B_LOG_DATA);
int32_t EXTERNAL_FCN b_log_create(const char *, uint16_t, const char *, size_t, B_LOG_FH *, B_LOG_DATA);
int32_t EXTERNAL_FCN b_log_close(B_LOG_FH);
int32_t EXTERNAL_FCN b_log_getVersion(B_LOG_FH, char *, size_t);
int32_t EXTERNAL_FCN b_log_checkVersion(char *ver);
int32_t EXTERNAL_FCN b_log_getNumFiles(B_LOG_FH, uint32_t *);
int32_t EXTERNAL_FCN b_log_getNextFile(B_LOG_FH, char *, uint32_t, uint16_t *, uint32_t *, uint16_t *, uint32_t *, DFL_TYPE);
int32_t EXTERNAL_FCN b_log_getCurrFilename(B_LOG_FH, char *, uint32_t, uint32_t *);
int32_t EXTERNAL_FCN b_log_getCurrNumPages(B_LOG_FH, uint32_t *);
int32_t EXTERNAL_FCN b_log_getCurrXFlags(B_LOG_FH, uint16_t *);
int32_t EXTERNAL_FCN b_log_getCurrPageSize(B_LOG_FH, uint32_t *);
int32_t EXTERNAL_FCN b_log_getCurrInitializedFlag(B_LOG_FH, uint16_t *);
int32_t EXTERNAL_FCN b_log_getNextPage(B_LOG_FH, void *, uint32_t, uint32_t *, uint64_t *);
int32_t EXTERNAL_FCN b_log_getCurrPageData(B_LOG_FH, void *, uint32_t, uint32_t *);
int32_t EXTERNAL_FCN b_log_getCurrPageAddr(B_LOG_FH, uint64_t *);
int32_t EXTERNAL_FCN b_log_getCurrPageOffset(B_LOG_FH, PSP_FILE_OFFSET *);
int32_t EXTERNAL_FCN b_log_setCurrPageOffset(B_LOG_FH, PSP_FILE_OFFSET);
int32_t EXTERNAL_FCN b_log_putNewFile(B_LOG_FH, const char *, uint16_t, uint32_t, uint16_t);
int32_t EXTERNAL_FCN b_log_putNewPage(B_LOG_FH, LOG_COMPRESSION_TYPE, uint64_t, const void *, uint32_t);
int32_t EXTERNAL_FCN b_log_getTxidFromLogFileName(const char *, TXID *, LOG_FILE_TYPE *);
int32_t EXTERNAL_FCN b_log_findFirstTxidFromDirectory(const char *, uint16_t, TXID *, const char *);
int32_t EXTERNAL_FCN b_log_findLastTxidFromDirectory(const char *, uint16_t, TXID *, const char *);

/* =========================================================================
   Zero-run compression
*/

int32_t EXTERNAL_FCN b_zcmp_compress(const char *, uint32_t, char *, uint32_t, uint32_t *);
int32_t EXTERNAL_FCN b_zcmp_decompress(const char *, uint32_t, char *, uint32_t, uint32_t *);

/* =========================================================================
   Pooled memory
*/

typedef const void *B_POOL;

B_POOL b_poolAlloc(PSP_MEMTAG, int16_t, const char *);
void  *b_poolGetMem(size_t, B_POOL, const char *, uint32_t);
void   b_poolPut(const void *, B_POOL);
void   b_poolZFree(B_POOL *);

#define b_poolGet(sz, pl) b_poolGetMem((sz), (pl), __FILE__, __LINE__)

/* =========================================================================
   B-tree implementation
*/

typedef int32_t (*B_BT_COMPARE)(const void *, const void *);
typedef const void *B_BT;

B_BT     b_btCreate(const char *, B_BT_COMPARE, uint32_t);
void     b_btDestroy(B_BT);
void    *b_btFind(B_BT, const void *);
int16_t  b_btAdd(B_BT, const void *);
void    *b_btDelete(B_BT, const void *);
void    *b_btFirst(B_BT);
void    *b_btLast(B_BT);
void    *b_btNext(B_BT);
void    *b_btPrev(B_BT);
uint32_t b_btCount(B_BT);

/* =========================================================================
   Server functions
*/
#define B_QUERY_INACTIVE    1
#define B_QUERY_ERROR       2

int32_t EXTERNAL_FCN b_server_start(const char *, const char *, uint16_t);
int32_t EXTERNAL_FCN b_server_stop(const char *, const char *, uint16_t);
int32_t EXTERNAL_FCN b_server_query(const char *, uint16_t);
int32_t EXTERNAL_FCN b_server_install(const char *, const char *, const char *, const char *, const char *, uint16_t);
int32_t EXTERNAL_FCN b_server_uninstall(const char *, uint16_t);
int32_t EXTERNAL_FCN b_server_verify(const char *, uint16_t);
int32_t EXTERNAL_FCN b_server_ping(uint16_t);

/* =========================================================================
   Environment variable functions
*/

uint16_t EXTERNAL_FCN b_getTFSDefaultPort(void);
uint16_t EXTERNAL_FCN b_getMirDefaultPort(void);
uint16_t EXTERNAL_FCN b_getSQLDefaultPort(void);

/* =========================================================================
   General purpose cache management functions
   (defined in cache.c and used in sort.c)
*/

typedef void       *BCACHE;
typedef const void *CBCACHE;

typedef int16_t (*BCACHE_LOADER)(int32_t, void **, void *);
typedef int16_t (*BCACHE_ZAPPER)(int32_t, void *, void *);

int16_t  EXTERNAL_FCN b_cacheCreate(uint32_t, BCACHE_LOADER, BCACHE_ZAPPER, PSP_MEMTAG, void *, BCACHE *);
int16_t  EXTERNAL_FCN b_cacheDestroy(int32_t, BCACHE);
int16_t  EXTERNAL_FCN b_cacheGetEntry(int32_t, void **, BCACHE);
int16_t  EXTERNAL_FCN b_cacheReleaseEntry(int32_t, CBCACHE);

/* =========================================================================
   General purpose bitmap management functions
   (defined in bitmap.c)
*/
typedef const void *BBITMAP;

int32_t  EXTERNAL_FCN b_bmCreate(uint64_t, PSP_MEMTAG, BBITMAP *);
void     EXTERNAL_FCN b_bmDestroy(BBITMAP);
int32_t  EXTERNAL_FCN b_bmSetBit(uint64_t, BBITMAP);
int32_t  EXTERNAL_FCN b_bmClearBit(uint64_t, BBITMAP);
int32_t  EXTERNAL_FCN b_bmNegateBit(uint64_t, BBITMAP);
int32_t  EXTERNAL_FCN b_bmTestBit(uint64_t, BBITMAP);
void     EXTERNAL_FCN b_bmClearAll(BBITMAP);
uint64_t EXTERNAL_FCN b_bmSize(BBITMAP);
uint64_t EXTERNAL_FCN b_bmCount(BBITMAP);
/*lint -esym(534,b_bmSetBit,b_bmClearBit,b_bmNegateBit) */

/* =========================================================================
   Prime functions
   (defined in prime.c)
*/
typedef const void *BPRIME;

int32_t         EXTERNAL_FCN b_primeCreate(uint64_t, PSP_MEMTAG, BPRIME *);
void            EXTERNAL_FCN b_primeDestroy(BPRIME);
int32_t         EXTERNAL_FCN b_primeIsNumPrime(uint64_t, BPRIME);
const uint64_t *EXTERNAL_FCN b_primeGetPrimes(uint64_t *, PSP_MEMTAG, BPRIME);

uint64_t EXTERNAL_FCN b_primeNextGreaterOrEqual(uint64_t);

/* =======================================================================
    Docroot functions
*/
int32_t EXTERNAL_FCN b_lockDocroot(const char *, const char *, PSP_FH *, uint32_t);
int32_t EXTERNAL_FCN b_unlockDocroot(const char *, const char *, PSP_FH *, uint32_t);

/* =========================================================================
    Sort/merge constant and type defintions 
*/

#define S_SORT_NOMEMORY      -1

#define S_SORT_KEYNO_ERR    -10
#define S_SORT_TYPE_ERR     -11
#define S_SORT_OFFSET_ERR   -12
#define S_SORT_PATH_ERR     -13
#define S_SORT_POS_ERR      -14
#define S_SORT_BUFSZ_ERR    -15

#define S_SORT_HANDLE_ERR   -20
#define S_SORT_STATE_ERR    -21

#define S_SORT_IO_ERR       -31
#define S_SORT_EOB          -32

#define S_SORT_ERROR        -41

#define SORT_NOTASYNC       0x0001
#define SORT_NOPENDING      0x0010

#define SORT_BYTES 0xFFFF

#define SORT_MINBUFFERS         4 

#define SORT_CHAR_COMPARE_ATTR     "char compare"
#define SORT_WCHAR_COMPARE_ATTR    "wchar compare"

typedef const void *B_SORTCB;

#define SORT_FIELD_CHAR      0x0000  /* field is a character field */
#define SORT_FIELD_WCHAR     0x0001  /* field is a wide character field */
#define SORT_FIELD_INT16     0x0002  /* field is a 16-bit integer field */
#define SORT_FIELD_INT32     0x0003  /* field is a 32-bit integer field */
#define SORT_FIELD_INT64     0x0004  /* field is a 64-bit integer field */
#define SORT_FIELD_FLOAT     0x0005  /* field is a float field */
#define SORT_FIELD_DOUBLE    0x0006  /* field is a double field */
#define SORT_FIELD_DBADDR    0x0007  /* field is a database address field */
#define SORT_FIELD_BLOB      0x0008  /* field is a blob field */
#define SORT_FIELD_GROUPED   0x0009  /* field is a structure */
#define SORT_FIELD_INT8      0x000A  /* field is a 8-bit integer field */
#define SORT_FIELD_DATE      0x000B  /* field is an SQL date */
#define SORT_FIELD_TIME      0x000C  /* field is a SQL time */
#define SORT_FIELD_TIMESTAMP 0x000D  /* field is a SQL timestamp */
#define SORT_FIELD_TYPE_MASK 0x000F  /* mask for field types */
#define SORT_FIELD_UNSIGNED  0x0010  /* field is unsigned */

/* Comparison Function Prototypes  */
typedef int16_t (EXTERNAL_FCN CMPFUNCTION)(const void *, uint32_t, const void *, uint32_t);
typedef int16_t (EXTERNAL_FCN GCMPFUNCTION)(const void *, const void *, const void *);
typedef int16_t (EXTERNAL_FCN STRCMPFUNCTION)(const void *, const void *, uint32_t);

typedef struct _SORTDESCR {
    CMPFUNCTION   *pCmpFcn; /* pointer to the compare function (NULL, if none) */
    void          *ctxt;    /* context for compare function (NULL, if NONE) */
    uint32_t       keyno;   /* the sort key field number */
    uint32_t       len;     /* length of the field (bytes) */
    uint32_t       offset;  /* offset to the start of data in the sort record */
    char           desc;    /* 0 for ascending, 1 for descending */
    uint16_t       type;    /* data type */
} SORTDESCR;

typedef struct _B_SORTATTR {           
    const char     *attrName;   /* name of the attribute */
    STRCMPFUNCTION *attrValue;  /* value of the attribute */
    uint32_t       *puFileNo;   /* temporary file number */
} B_SORTATTR;

/* sort/merge API function prototypes -------------------------------- */

int16_t  EXTERNAL_FCN b_sortOpen(const B_SORTATTR *, PSP_MEMTAG, B_SORTCB *);
int16_t  EXTERNAL_FCN b_sortStart(int, const SORTDESCR *, B_SORTCB);
int16_t  EXTERNAL_FCN b_sortStore(const void *, B_SORTCB);
int16_t  EXTERNAL_FCN b_sortComplete(B_SORTCB);
uint32_t EXTERNAL_FCN b_sortCount(B_SORTCB);
int16_t  EXTERNAL_FCN b_sortSeek(uint32_t, B_SORTCB);
int      EXTERNAL_FCN b_sortRead(int, void*, B_SORTCB);
int16_t  EXTERNAL_FCN b_sortClose(B_SORTCB);

/**************************************************************************
  Date/Time/Timestamp declarations
*/

/* Date, time, and timestamp definitions */
typedef uint32_t DATE_VAL;
typedef uint32_t TIME_VAL;

typedef struct {
    DATE_VAL date;
    TIME_VAL time;
} TIMESTAMP_VAL;

#define MAX_TIME_PREC 10000
#define MAX_PRECISION MAX_TIME_PREC

typedef struct tagCAL_DATE {
    int32_t  year;      /* 1 - ???? */
    uint16_t month;     /* 1 - 12 */
    uint16_t day;       /* 1 - 31 */
} CAL_DATE;

typedef struct tagCAL_TIME {
    uint16_t hour;      /* 0 - 23 */
    uint16_t minute;    /* 0 - 59 */
    uint16_t second;    /* 0 - 59 */
    uint16_t fraction;  /* 0 - 9999 */
} CAL_TIME;

typedef struct tagCAL_TIMESTAMP {
    int32_t  year;      /* 1 - ???? */
    uint16_t month;     /* 1 - 12 */
    uint16_t day;       /* 1 - 31 */
    uint16_t hour;      /* 0 - 23 */
    uint16_t minute;    /* 0 - 59 */
    uint16_t second;    /* 0 - 59 */
    uint16_t fraction;  /* 0 - 9999 */
} CAL_TIMESTAMP;

/* date functions */
const char *EXTERNAL_FCN dt_dateDayAbr(DATE_VAL);
const char *EXTERNAL_FCN dt_dateDayName(DATE_VAL);
uint16_t    EXTERNAL_FCN dt_dateDayOfYear(DATE_VAL);
uint16_t    EXTERNAL_FCN dt_dateDayOfMonth(DATE_VAL);
uint16_t    EXTERNAL_FCN dt_dateDayOfWeek(DATE_VAL);
int16_t     EXTERNAL_FCN dt_dateFromCalDate(const CAL_DATE *, DATE_VAL *);
DATE_VAL    EXTERNAL_FCN dt_dateFromString(const char *);
uint16_t    EXTERNAL_FCN dt_dateMonth(DATE_VAL);
const char *EXTERNAL_FCN dt_dateMonthAbr(DATE_VAL);
const char *EXTERNAL_FCN dt_dateMonthName(DATE_VAL);
uint16_t    EXTERNAL_FCN dt_dateQuarter(DATE_VAL);
int16_t     EXTERNAL_FCN dt_dateToCalDate(DATE_VAL, CAL_DATE *);
void        EXTERNAL_FCN dt_dateToday(DATE_VAL *);
uint16_t    EXTERNAL_FCN dt_dateValidateMonth(const char *);
uint16_t    EXTERNAL_FCN dt_dateWeek(DATE_VAL);
int32_t     EXTERNAL_FCN dt_dateYear(DATE_VAL);

/* time functions */
uint16_t    EXTERNAL_FCN dt_timeFraction(TIME_VAL);
int16_t     EXTERNAL_FCN dt_timeFromCalTime(const CAL_TIME *, TIME_VAL *);
TIME_VAL    EXTERNAL_FCN dt_timeFromString(const char *);
uint16_t    EXTERNAL_FCN dt_timeHour(TIME_VAL);
uint16_t    EXTERNAL_FCN dt_timeMinute(TIME_VAL);
void        EXTERNAL_FCN dt_timeNow(TIME_VAL *);
uint16_t    EXTERNAL_FCN dt_timeSecond(TIME_VAL);
int16_t     EXTERNAL_FCN dt_timeToCalTime(TIME_VAL, CAL_TIME *);

/* timestamp functions and macros */
TIMESTAMP_VAL EXTERNAL_FCN dt_timestampFromString(const char *);
void          EXTERNAL_FCN dt_timestampNow(TIMESTAMP_VAL *);
void          EXTERNAL_FCN dt_timestampDaysDiff(TIMESTAMP_VAL, TIMESTAMP_VAL, uint32_t *);

const char *EXTERNAL_FCN dt_todaysDate(char *, size_t);

#define dt_timestampDayAbr(ts)     dt_dateDayAbr(ts.date)
#define dt_timestampDayName(ts)    dt_dateDayName(ts.date)
#define dt_timestampDayOfMonth(ts) dt_dateDayOfMonth(ts.date)
#define dt_timestampDayOfWeek(ts)  dt_dateDayOfWeek(ts.date)
#define dt_timestampDayOfYear(ts)  dt_dateDayOfYear(ts.date)
#define dt_timestampFraction(ts)   dt_timeFraction(ts.time)
#define dt_timestampHour(ts)       dt_timeHour(ts.time)
#define dt_timestampMinute(ts)     dt_timeMinute(ts.time)
#define dt_timestampMonthAbr(ts)   dt_timeMonthAbr(ts.time)
#define dt_timestampMonthName(ts)  dt_timeMonthName(ts.time)
#define dt_timestampQuarter(ts)    dt_dateQuarter(ts.date)
#define dt_timestampSecond(ts)     dt_timeSecond(ts.time)
#define dt_timestampWeek(ts)       dt_dateWeek(ts.date)
#define dt_timestampYear(ts)       dt_dateYear(ts.date)
/*lint -esym(534,dt_dateToCalDate,dt_timeToCalTime) */

/* =========================================================================
   Miscellaneous functions
*/
typedef struct tagBCD_PACKED {
    uint8_t prec;
    uint8_t scale;
    uint8_t flags;
    uint8_t data[EXPANDABLE];
} BCD_PACKED;

typedef struct tagBCD_UNPACKED {
    uint8_t prec;
    int8_t  exp;
    uint8_t flags;
    int8_t  sign;
    uint8_t data[EXPANDABLE];
} BCD_UNPACKED;

#define BCDPACKEDLEN(prec)   (sizeof(BCD_PACKED) + ((prec) / 2 + (prec) % 2))
#define BCDUNPACKEDLEN(prec) (sizeof(BCD_UNPACKED) + (prec))

#define BCD_OKAY         0
#define BCD_PRECTOOBIG (-1)

#define BCD_FLAGS_PRECLOSS 0x01
#define BCD_FLAGS_OVERFLOW 0x02
#define BCD_FLAGS_ZERODIV  0x04
#define BCD_FLAGS_TOOSMALL 0x08
#define BCD_FLAGS_PACKED   0x40
#define BCD_FLAGS_POSITIVE 0x80

uint8_t       EXTERNAL_FCN bcd_add(const BCD_UNPACKED *, const BCD_UNPACKED *, BCD_UNPACKED *);
BCD_UNPACKED *EXTERNAL_FCN bcd_allocate(PSP_MEMTAG);

uint8_t       EXTERNAL_FCN bcd_div(const BCD_UNPACKED *, const BCD_UNPACKED *, BCD_UNPACKED *);
void          EXTERNAL_FCN bcd_floor(BCD_UNPACKED *);
void          EXTERNAL_FCN bcd_free(const BCD_UNPACKED *, PSP_MEMTAG);
uint8_t       EXTERNAL_FCN bcd_fromChar(const char *, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromFloat(float, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromDouble(double, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromInt8(int8_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromInt16(int16_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromInt32(int32_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromInt64(int64_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromUInt8(uint8_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromUInt16(uint16_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromUInt32(uint32_t, BCD_UNPACKED *);
uint8_t       EXTERNAL_FCN bcd_fromUInt64(uint64_t, BCD_UNPACKED *);
#if !defined(RDM_NO_UNICODE)
uint8_t       EXTERNAL_FCN bcd_fromWChar(const wchar_t *, BCD_UNPACKED *);
#endif
int16_t       EXTERNAL_FCN bcd_isZero(const BCD_UNPACKED *);

uint8_t       EXTERNAL_FCN bcd_maxprec(void);
uint8_t       EXTERNAL_FCN bcd_maxscale(void);
int16_t       EXTERNAL_FCN bcd_pack(const BCD_UNPACKED *, BCD_PACKED *, uint8_t, uint8_t);

uint8_t       EXTERNAL_FCN bcd_toChar(const BCD_UNPACKED *, char *, uint32_t);
uint8_t       EXTERNAL_FCN bcd_toCharWithScale(const BCD_UNPACKED *, uint8_t, char *, uint32_t);
#if !defined(RDM_NO_UNICODE)
uint8_t       EXTERNAL_FCN bcd_toWChar(const BCD_UNPACKED *, wchar_t *, uint32_t);
#endif
int16_t       EXTERNAL_FCN bcd_unpack(const BCD_PACKED *, BCD_UNPACKED *, int16_t);

void        EXTERNAL_FCN DisplayMessage(uint16_t, uint16_t, const char *, ...);
void        EXTERNAL_FCN DisplayMessageNow(uint16_t, uint16_t, const char *, ...);
void        EXTERNAL_FCN DisplayError(uint16_t, uint16_t, const char *, ...);
void        EXTERNAL_FCN DisplayErrorNow(uint16_t, uint16_t, const char *, ...);
int32_t     EXTERNAL_FCN b_OpenStdFiles(int16_t, const char *, PSP_FILE_STREAM *, const char *, PSP_FILE_STREAM *, const char *, PSP_FILE_STREAM *);

/* C++ API class generation functions */
void EXTERNAL_FCN b_write_cpp_header_header(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cpp_header_footer(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_header_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_open_declaration(PSP_FILE_STREAM, int16_t, const char*);
void EXTERNAL_FCN b_write_db_constructor_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_operator_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_destructor_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_add_record_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_get_record_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_get_record_by_key_declaration(PSP_FILE_STREAM, const char*, const char*);
void EXTERNAL_FCN b_write_db_private_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_footer_declaration(PSP_FILE_STREAM);
void EXTERNAL_FCN b_write_cursor_compound_key_find_declaration(PSP_FILE_STREAM, const char *, const char *, const char *, const char *, uint16_t);
void EXTERNAL_FCN b_write_cursor_compound_key_find_definition(PSP_FILE_STREAM, const char *, const char *, const char *, const char *, uint16_t);
void EXTERNAL_FCN b_write_cursor_header_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_constructor_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_operator_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_set_owner_declaration(PSP_FILE_STREAM, const char*, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_set_member_declaration(PSP_FILE_STREAM, const char*, const char*, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_system_set_member_declaration(PSP_FILE_STREAM, const char*, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_key_declaration(PSP_FILE_STREAM, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_key_find_declaration(PSP_FILE_STREAM, const char*, const char*, const char *, const char *);
void EXTERNAL_FCN b_write_cursor_record_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_blob_field_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_field_declaration(PSP_FILE_STREAM, const char*, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_struct_field_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_destructor_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_private_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_friend_declaration(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_footer_declaration(PSP_FILE_STREAM);
void EXTERNAL_FCN b_write_cpp_source_header(PSP_FILE_STREAM, int16_t, const char*);
void EXTERNAL_FCN b_write_db_open_definition(PSP_FILE_STREAM, int16_t, const char*);
void EXTERNAL_FCN b_write_db_misc_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_constructor_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_operator_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_destructor_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_db_add_record_definition(PSP_FILE_STREAM, const char*, const char*);
void EXTERNAL_FCN b_write_db_get_record_definition(PSP_FILE_STREAM, const char*, const char*);
void EXTERNAL_FCN b_write_db_get_record_by_key_definition(PSP_FILE_STREAM, const char*, const char*, const char*, uint16_t);
void EXTERNAL_FCN b_write_cursor_misc_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_constructor_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_operator_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_set_owner_definition(PSP_FILE_STREAM, const char *, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_set_member_definition(PSP_FILE_STREAM, const char*, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_system_set_member_definition(PSP_FILE_STREAM, const char*, const char*);
void EXTERNAL_FCN b_write_cursor_key_definition(PSP_FILE_STREAM, const char*, const char*, uint16_t);
void EXTERNAL_FCN b_write_cursor_key_find_definition(PSP_FILE_STREAM, const char*, const char*, const char*, const char*, uint16_t);
void EXTERNAL_FCN b_write_cursor_record_definition(PSP_FILE_STREAM, const char*);
void EXTERNAL_FCN b_write_cursor_blob_field_definition(PSP_FILE_STREAM, const char*, const char*, uint16_t);
void EXTERNAL_FCN b_write_cursor_field_definition(PSP_FILE_STREAM, const char*, const char*, const char*, const char*, uint16_t);
void EXTERNAL_FCN b_write_cursor_struct_field_definition(PSP_FILE_STREAM, const char*, const char*, uint16_t);
void EXTERNAL_FCN b_write_cursor_destructor_definition(PSP_FILE_STREAM, const char*);

#if defined(__cplusplus)
}
#endif
#endif  /* BASE_H */

