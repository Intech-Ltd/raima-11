/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2001-2008 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#if !defined(RDM_H)
#define RDM_H

#include "psp.h"
#include "rdmtype.h"

/* ======================================================================== */
/* Status Codes                                                             */
/* ======================================================================== */

/*
    User errors
*/
#define S_USER_FIRST       -1    /* first user err */

#define S_DBOPEN           -1    /* database not opened */
#define S_INVSET           -2    /* invalid set */
#define S_INVREC           -3    /* invalid record */
#define S_INVDB            -4    /* invalid database */
#define S_INVFLD           -5    /* invalid field name */
#define S_INVADDR          -6    /* invalid db_address */
#define S_NOCR             -7    /* no current record */
#define S_NOCO             -8    /* set has no current owner */
#define S_NOCM             -9    /* set has no current member */
#define S_KEYREQD          -10   /* key value required */
#define S_BADTYPE          -11   /* invalid lock type */
#define S_HASMEM           -12   /* record is owner of non-empty set(s) */
#define S_ISMEM            -13   /* record is member of set(s) */
#define S_ISOWNED          -14   /* member already owned */
#define S_ISCOMKEY         -15   /* field is a compound key */
#define S_NOTCON           -16   /* record not connected to set */
#define S_NOTKEY           -17   /* field is not a valid key */
#define S_INVOWN           -18   /* record not legal owner of set */
#define S_INVMEM           -19   /* record not legal member of set */
#define S_SETPAGES         -20   /* error in d_setpages (database open or bad param) */
#define S_INCOMPAT         -21   /* incompatible dictionary file */
#define S_DELSYS           -22   /* illegal attempt to delete system record */
#define S_NOTFREE          -23   /* attempt to lock previously locked rec or set */
#define S_NOTLOCKED        -24   /* attempt to access unlocked record or set */
#define S_TRANSID          -25   /* transaction id not be supplied */
#define S_TRACTIVE         -26   /* transaction already active */
#define S_TRNOTACT         -27   /* transaction not currently active */
#define S_DBD_NONAMES      -28   /* incompatible legacy DBD file - does not contain record/field/set names */
#define S_TRFREE           -29   /* attempt to free a lock inside a transaction */
#define S_DBOPENED         -30   /* operation not completed because the database is opened elsewhere */
#define S_NOTRANS          -31   /* attempted update outside of transaction */
#define S_EXCLUSIVE        -32   /* functions requires exclusive db access */
#define S_STATIC           -33   /* Attempted to write lock a static file */
#define S_USERID           -34   /* No user id exists */
#define S_NAMELEN          -35   /* database file/path name too long */
#define S_INVTRID          -36   /* invalid transaction id */
#define S_NOTOPTKEY        -37   /* field is not an optional key */
#define S_BADFIELD         -38   /* field not defined in current record type */
#define S_COMKEY           -39   /* record/field has/in a compound key */
#define S_INVNUM           -40   /* invalid record or set number */
#define S_INVKEY           -41   /* invalid optional key cannot be used in d_setkey */
#define S_BADUSERID        -42   /* invalid user id - not alphanumeric */
#define S_INVREPHANDLE     -43   /* invalid replication handle provided */
#define S_INVPKEY          -44   /* invalid partial key specification */
#define S_INVARG           -45   /* Invalid function argument */
#define S_NOTYPE           -46   /* No current record type */
#define S_INVINDEX         -47   /* Invalid index type */
#define S_DBCLOSE          -48   /* database not closed */
#define S_INVPTR           -49   /* invalid pointer */
#define S_INVID            -50   /* invalid internal ID */
#define S_INVFORUNIONDB    -51   /* operation invalid for a union database */
#define S_INVTASK          -52   /* invalid task id */
#define S_UNIONINVMODE     -53   /* Invalid open mode for a union database */
#define S_NOTIMPLEMENTED   -54
#define S_DEFUNCT          -55   /* Function no longer supported/defunct */
#define S_IMPBADXML        -56   /* bad xml */
#define S_IMPBADDATA       -57   /* bad data */
#define S_REPBADTYPE       -58   /* bad replication type */
#define S_REPINVMODE       -59  /* Invalid open mode for a replicated database */
#define S_REPINVLOCK       -60  /* Invalid 'exclusive lock' request for a replicated database */
#define S_INVFILE          -62  /* Invalid file type for operation */
#define S_INVDBA           -63  /* Invalid DBA format */
#define S_INVENDIAN        -64  /* Invalid endian format */
#define S_INVSTRING        -65  /* Invalid string format */
#define S_DELCIRC          -66  /* Illegal attempt to delete circular record */
#define S_TRPRECOMMITTED   -67  /* A precommitted transaction must be committed or aborted back before further operations are allowed */
#define S_DDLERROR         -68  /* There were errors processing a DDL file */
#define S_DBACTIVE         -69  /* This version only supports single user mode */
#define S_INVDBD           -70  /* The DBD is not compatible with the runtime library */
#define S_REPLOGVER        -71  /* Invalid replication log version */
#define S_REPLOGDBA        -72  /* Invalid replication log DBA format */
#define S_REPLOGENDIAN     -73  /* Invalid replication log endian format */
#define S_REPLOGSTRING     -74  /* Invalid replication log string format */
#define S_RECOVERY_FAILED  -75  /* Recovery failed */
#define S_ROTACTIVE        -76  /* A read-only transaction is already active */
#define S_NOROTACTIVE      -77  /* No read-only transaction is active */
#define S_HASWRITELOCKS    -78  /* A read-only transaction cannot start because the user holds write locks */
#define S_INROT            -79  /* Cannot apply a write or exclusive lock while in a read-only transaction */
#define S_DBALREADYOPEN    -80  /* Task already has this db open */
#define S_SHUTDOWN         -81  /* A server is shutting down */
#define S_INVURLARGFMT     -82  /* Invalid URL argument format */
#define S_INVDBAFILE       -83  /* Invalid DBA file value */
#define S_INVDBASLOT       -84  /* Invalid DBA slot value */
#define S_REPRDMSERR       -85  /* Error replicating to RDM Server Slave */
#define S_UNOMISMATCH      -86  /* Unions mismatch */
#define S_MIRSYNC_DENIED   -88  /* Database already used in synchronous mirroring */
#define S_MIRSYNC_ERROR    -89  /* Synchronous mirroring error */
#define S_IN_CHANGE_TRANS  -90  /* Initialize not allowed when a change transaction is active */
#define S_IN_INIT_TRANS    -91  /* Changes not allowed when an initialization transaction is active */
#define S_NOMIRSERVER      -92  /* Unable to connect to a mirroring server */
#define S_DBDMISMATCH      -93  /* DBD in memory does not match DBD on disk */
#define S_USER_LAST        S_DBDMISMATCH /* last user error */

/*
    TX errors
*/
#define S_TX_FIRST          -200

#define S_TX_ERROR          -200 /* generic tx_ error */
#define S_TX_NOPAGE         -201 /* requested page doesn't exist */
#define S_TX_NETREAD        -202 /* failure to read data from network */
#define S_TX_NETWRITE       -203 /* failure to write data to network */
#define S_TX_WRITEFAIL      -204 /* failed to write to a file */
#define S_TX_DUPUSER        -205 /* same dbuserid already in use */
#define S_TX_MAXUSERS       -206 /* ran out of user connections */
#define S_TX_INVUSERID      -207 /* userid not found in user table */
#define S_TX_NOTVERIFIED    -208 /* user is not verified */
#define S_TX_TXNOTACTIVE    -209 /* no transaction is active */
#define S_TX_INVFILE        -210 /* file not found in file table */
#define S_TX_TRUNCATED      -211 /* data retrieve had to be truncated */
#define S_TX_INVDB          -212 /* database not found in database table */
#define S_TX_DBACTIVE       -213 /* database is currently in use */
#define S_TX_CONNECT        -214 /* failed to connect to TFS */
#define S_TX_HOSTNAME       -215 /* TFS host name not found */
#define S_TX_LISTEN         -216 /* TCP/IP listen failure in TFS */
#define S_TX_ACCEPT         -217 /* TCP/IP accept failure in TFS */
#define S_TX_INVINI         -218 /* invalid ini file */
#define S_TX_DBOPEN         -219 /* Cannot destroy a database that is open by others */
#define S_TX_INVTFS         -220 /* Invalid TFS handle */
#define S_TX_TXPRECOMMIT    -221 /* transaction is precommitted */
#define S_TX_ILLEGAL_MOD    -222 /* illegal attempt to modify read-only device */
#define S_TX_NO_FILE_ACCESS -223 /* illegal attempt to cause a file access on a diskless system */
#define S_TX_TFSNOTINITED   -224 /* TFS not initialized */
#define S_TX_DOCROOT_USED   -225 /* The docroot is already in use */
#define S_TX_NO_REMOTE      -226 /* Remote connections not allowed */
#define S_TX_INVTFSTYPE     -227 /* Invalid TFS type */

#define S_TX_LAST           -227
/*
    Import errors
*/
#define S_IMPORT_FIRST     -501

#define DB_IMP_INVREC      -501 /* unrecognized record name */
#define DB_IMP_INVFLD      -502 /* unrecognized field name */
#define DB_IMP_INVSET      -503 /* unrecognized set name */
#define DB_IMP_FLDTYPE     -504 /* unexpected data type */
#define DB_IMP_FLDSIZE     -505 /* string field truncated */
#define DB_IMP_NOFLD       -506 /* field missing */
#define DB_IMP_NOKEY       -507 /* unique key value missing */
#define DB_IMP_INVMEM      -508 /* record not a valid member of set */
#define DB_IMP_INVOWN      -509 /* specified owner not a valid owner of set */
#define DB_IMP_INVREF      -510 /* reference uses non-unique identifier */
#define DB_IMP_INVDB       -511 /* invalid database name reference */
#define DB_IMP_DUPMEM      -512 /* specified member already member of set */

#define S_IMPORT_LAST      -512

/*
    Embed file errors
*/
#define EMBED_FILE_NOT_FOUND        -700
#define EMBED_UNKNOWN_COMMAND       -701
#define EMBED_ILLEGAL_NAMESPACE     -702
#define EMBED_CONTENT_DONT_MATCH    -703

/*
    QA_FRAME errors
*/
#define S_CASE_DESCRIBED            -800
#define S_NO_TESTS_SELECTED         -801
#define S_MISSING_ARGUMENT          -802
#define S_HELP                      -803
#define S_NEED_DESCRIPTION          -804
#define S_CASE_FAILED               -805
#define S_HAS_FLOAT_REQUIRED        -806
#define S_ILLEGAL_USE_OF_STOP_WATCH -807
#define S_FEATURE_NOTIMPLEMENTED    -808
#define S_ILLEGAL_NAMESPACE         -809
#define S_ILLEGAL_FILENAME          -810
#define S_TEST_TIMEOUT              -811

/*
    System errors
*/
#define S_SYSTEM_FIRST     -900  /* first system error */

#define S_NOSPACE          -900  /* out of disk space */
#define S_SYSERR           -901  /* system error */
#define S_FAULT            -902  /* page fault -- too many locked pages */
#define S_MKDIR            -903  /* unable to create directory */
#define S_NOMEMORY         -904  /* unable to allocate sufficient memory */
#define S_NOFILE           -905  /* unable to locate a file */
#define S_COMMITFAILED     -906  /* commit failed due to file rename failure */
/* -907 */
#define S_BADLOCKS         -908  /* inconsistent database locks */
#define S_RECLIMIT         -909  /* file record limit reached */
#define S_KEYERR           -910  /* key file inconsistency detected */
#define S_VARDATALEN       -911  /* vardata length inconsistency */
/* -912 */
/* -913 */
#define S_BADREAD          -914  /* Bad read on database/overflow file */
/* -915 */
#define S_DEBUG            -916  /* Debugging check interrupt */
/* -917 */
#define S_VARDATAPTR       -918  /* vardata pointer inconsistency */
#define S_BADWRITE         -919  /* Bad write on database/overflow file */
/* -920 */
#define S_DUPUSERID        -921  /* DBUSERID is already used by someone else */
#define S_LMBUSY           -922  /* The lock manager table(s) are full */
/* -923 */
#define S_INVNULL          -924  /* invalid NULL parameter */
/* -925 */
#define S_NOTBLOB          -926  /* the specified field is not a blob field */
#define S_BLOBUPD          -927  /* attempt to update a BLOB directly */
#define S_BLOBPOS          -928  /* invalid BLOB position */
#define S_BLOBERR          -929  /* unexpected BLOB error */
/* -930 */
#define S_INVLOGFILE       -931  /* invalid log file format */
#define S_BUFFER_TOO_SMALL -932  /* supplied buffer is too small */
#define S_FCN_SEQUENCE     -933  /* function(s) called in an incorrect order */
/* -934 */
/* -935 */
/* -936 */
/* -937 */
/* -938 */
#define S_READONLY         -939  /* d_on_opt(READONLY), unable to update file */
#define S_EACCESS          -940  /* sharing violation, file in use */
/* -941 */
/* -942 */
/* -943 */
/* -944 */
/* -945 */
/* -946 */
/* -947 */
/* -948 */
/* -949 */
/* -950 */
/* -951 */
/* -952 */
/* -953 */
/* -954 */
/* -955 */
/* -956 */
/* -957 */
/* -958 */
/* -959 */
/* -960 */
#define S_JNIERROR         -961  /* Java Native Method Error */
/* -962 */
/* -963 */
/* -964 */
#define S_REPLOG_ERROR     -965  /* Error processing/reading/creating/writing the replication log */
/* -966 */
#define S_DBDREAD          -967  /* Error reading dbd file */


#define S_SYSTEM_LAST      -967  /* last system error */

/*
    Internal system errors
*/
#define S_INTERNAL_FIRST   -9001 /* first internal error */

#define SYS_MEMWRITE       -9001 /* unbuffered write to in-memory file */
#define SYS_BADTREE        -9002 /* b-tree malformed */
#define SYS_KEYEXIST       -9003 /* key value already exists */
#define SYS_BADFREE        -9004 /* attempt to free empty table */
#define SYS_BADOPTKEY      -9005 /* calculating optkey index */
#define SYS_INVFLDTYPE     -9006 /* illegal field type */
#define SYS_INVSORT        -9007 /* illegal sort ordering */
#define SYS_INVPGTAG       -9008 /* invalid page tag */
#define SYS_INVHOLD        -9009 /* bad hold count */
#define SYS_HASHCYCLE      -9010 /* cycle detected in hash chain */
#define SYS_INVPGCOUNT     -9011 /* bad page tag page count */
#define SYS_INVPGSIZE      -9012 /* invalid cache page size */
#define SYS_PZACCESS       -9013 /* invalid access to page zero */
#define SYS_INVEXTEND      -9014 /* illegal attempt to extend file */
#define SYS_FILESIZE       -9015 /* maximum file size exceeded */
#define SYS_PZNEXT         -9016 /* bad pznext */
#define SYS_DCHAIN         -9017 /* bad dchain */
#define SYS_EOF            -9018 /* attempt to write past EOF */
#define SYS_FILEMODIFIED   -9019 /* locked file was modified by another user */
#define SYS_INV_RHS_HANDLE -9020 /* invalid internal RHS handle */
#define SYS_BADHASH        -9021 /* b-tree malformed */

#define S_INTERNAL_LAST    -9020 /* last internal error */

/*
    Function statuses
*/
#define S_FUNCTION_FIRST   0     /* first function status */

#define S_OKAY             0     /* normal return, okay */
#define S_EOS              1     /* end of set */
#define S_NOTFOUND         2     /* record not found */
#define S_DUPLICATE        3     /* duplicate key */
#define S_KEYSEQ           4     /* field type used out of sequence in d_keynext */
#define S_UNAVAIL          5     /* database file currently unavailable */
#define S_DELETED          6     /* record/set deleted since last accessed */
#define S_UPDATED          7     /* record/set updated since last accessed */
#define S_LOCKED           8     /* current record's lock bit is set */
#define S_UNLOCKED         9     /* current record's lock bit is clear */
#define S_SETCLASH         10    /* set ptrs invalidated by another user */
#define S_EOF              11    /* end of file encountered */
#define S_TX_TIMEOUT       12    /* timeout in select function */
#define S_REFERENCING      13    /* file can be read locally */

#define S_FUNCTION_LAST    13    /* last function status */


/* ------------------------------------------------------------------------ */

typedef uint16_t        FILE_NO;    /* RDM file number */

typedef uint32_t        DB_ADDR4;   /* 3 bytes for slot, 1 byte for file */
typedef uint64_t        DB_ADDR8;   /* 6 bytes for slot, 2 byte for file */
typedef uint32_t        V_ADDR4;    /* vardata slot address: 4 bytes for slot, no file number */
typedef uint64_t        V_ADDR8;    /* vardata slot address: 8 bytes for slot, no file number */
typedef uint32_t        F_ADDR4;      /* file address: page or slot number */
typedef uint64_t        F_ADDR8;      /* file address: page or slot number */
#define MAXFILES8        0xFFFFU
#define MAXRECORDS8      ULL(0x0000FFFFFFFFFFFF)
#define MAXVARDATASLOTS8 ULL(0x7FFFFFFFFFFFFFFF)
#define MAXPAGES8        ULL(0xFFFFFFFFFFFFFFFF)
#define MAXFILES4        0xFFU
#define MAXRECORDS4      0x00FFFFFF
#define MAXVARDATASLOTS4 0x7FFFFFFF
#define MAXPAGES4        0xFFFFFFFFU

/* Internal database address structure */
typedef struct _DB_ADDR
{
    uint16_t   dba_uno;       /* dba union number */
    FILE_NO    dba_file;      /* dba file number */
    F_ADDR8    dba_offset;    /* dba offset */
} DB_ADDR;

typedef V_ADDR8        V_ADDR;
typedef F_ADDR8        F_ADDR;

typedef F_ADDR         BLOB_ID;
#define NULL_BLOB      ((BLOB_ID) 0)

typedef uint16_t       V_LEN;       /* length in bytes of variable sized field */

#define NULL_DBA4      ((DB_ADDR4) 0)
#define NULL_DBA8      ((DB_ADDR8) 0)

/* field number indicator = rec * FLDMARK + fld_in_rec */
#define FLDMARK 1000

/* record number indicator */
#define RECMARK 10000

/* set number indicator -- must be greater than RECMARK */
#define SETMARK 20000

/* set number indicator -- must be greater than RECMARK */
#define KEYMARK 30000

/* runtime option flags */
/* hi-order byte reserved for specifying option sets */
#define READNAMES       0x00000000L    /* No longer supported */

#define DCHAINUSE       0x00000001L    /* use the dchain for new records */
#define IGNORECASE      0x00000002L    /* use caseblind compares for strings */
#define ONEUSER_DENYNO  0x00000004L    /* one user more opens db in DENYNO mode */
#define IGNOREENV       0x00000008L    /* ignore environment variables */
#define READONLY        0x00000010L    /* open database for readonly access */
#define TXTEST          0x00000020L    /* test transaction recovery */
#define NORECOVER       0x00000040L    /* For internal use only, will corrupt databases */
#define TRUNCATELOG     0x00000080L    /* truncate log file at each d_trbegin */
#define PREALLOC_CACHE  0x00000100L    /* allocate cache pages during d_open */
#define MBSSORT         0x00000400L    /* string sort using mbs functions */
#if 0
#define DBREP           0x00000800L    /* Look for replication config db */
#define DBREP_INMEM     0x00001000L    /* Replication config db is in-memory*/
#endif

/* ------------------------------------------------------------------------ */

/* Version number string at start of DBD file must match one of these: */
#define dbd_UNICODE "U"
#define dbd_ANSI "A"
#define dbd_LITTLEENDIAN "L"
#define dbd_BIGENDIAN "B"
#define dbd_DBA8 "8"
#define dbd_DBA4 "4"
#define dbd_CURRENT_VER "AC"


#if defined(RDM_LITTLEENDIAN)
#define dbd_ENDIAN dbd_LITTLEENDIAN
#else
#define dbd_ENDIAN dbd_BIGENDIAN
#endif

#define dbd_STRING dbd_ANSI

#define dbd_DBA dbd_DBA8

#define dbd_VERSION_AC "AC" dbd_STRING dbd_ENDIAN dbd_DBA
#define dbd_VERSION_AC_DBA4 "AC" dbd_STRING dbd_ENDIAN dbd_DBA4
#define dbd_VERSION_AC_DBA8 "AC" dbd_STRING dbd_ENDIAN dbd_DBA8

/*#define NO_LEGACY */  /* Uncomment to remove legacy support */

#define dbd_VERSION_CURRENT      dbd_VERSION_AC
#define dbd_VERSION_CURRENT_DBA4 dbd_VERSION_AC_DBA4
#define dbd_VERSION_CURRENT_DBA8 dbd_VERSION_AC_DBA8

/* Numeric values representing DBD version strings: */
#define dbd_AC   1200
#define dbd_CURRENT dbd_AC
#if !defined(NO_LEGACY)
#define dbd_V300  300
#define dbd_V301  301
#define dbd_V500  500
#define dbd_U500 -500
#define dbd_V600  600
#define dbd_U600 -600
#define dbd_AA   1000
#define dbd_AB   1100
#endif

#define RECFRST 0
#define RECNEXT 1
#define RECPREV 2
#define RECLAST 3

/* ------------------------------------------------------------------------ */

typedef const void * RDM_DB;

typedef struct lock_request
{
    uint32_t item;                  /* record or set number */
    int32_t  type;                  /* lock type: 'r', 'w', 'x', 'k' */
}  LOCK_REQUEST;

typedef struct
{
    uint32_t item;                  /* record or set number */
    int32_t  type;                  /* lock type: 'r', 'w', 'x', 'k' */
    RDM_DB  hdb;                   /* database to lock */
}  LOCK_REQUEST2;

#define TRANS_ID_LEN 21

typedef enum {B_START, B_CURRENT, B_END} SEEK_TYPE;

typedef enum {TR_UNKNOWN, TR_CHANGE, TR_INIT} TRANS_TYPE;

#ifndef TASK_DEFN
typedef void DB_TASK;
typedef void (EXTERNAL_FCN *ERRORPROCA)(int32_t, char *);
typedef void (EXTERNAL_FCN *ERRORPROC2A)(int32_t, const char *, void *, DB_TASK *);
#if !defined(RDM_NO_UNICODE)
typedef void (EXTERNAL_FCN *ERRORPROCW)(int32_t, wchar_t *);
typedef void (EXTERNAL_FCN *ERRORPROC2W)(int32_t, const wchar_t *, void *, DB_TASK *);
#endif
#else
#include "dbtype.h"
#endif

#define ERRORPROC  ERRORPROCA
#define ERRORPROC2 ERRORPROC2A

#define DEPRECATED_DEFAULT_TASK ((DB_TASK *) -1)

#define  CURR_DB        -1
#define  ALL_DBS        -2
#define  VOID_DB        -3

#define REPLACE_TMP_FILE_EXT ".rpltmp"

#define RDM_OPM_SHARED    0x0000
#define RDM_OPM_EXCLUSIVE 0x0001
#define RDM_OPM_READONLY  0x0002

#define RDM_OPM_MODE_MASK 0x0003
#define RDM_OPM_TRANSOFF  0x0080

#define GET_OPEN_MODE(mode)     ((mode) & RDM_OPM_MODE_MASK)
#define IS_SHARED_TASK(task)    (GET_OPEN_MODE((task)->mode) == RDM_OPM_SHARED)
#define IS_EXCLUSIVE_TASK(task) (GET_OPEN_MODE((task)->mode) == RDM_OPM_EXCLUSIVE)
#define IS_READONLY_TASK(task)  (GET_OPEN_MODE((task)->mode) == RDM_OPM_READONLY)

#define IS_NOTRANS_TASK(task)   ((task)->mode & RDM_OPM_TRANSOFF)

#if defined(__cplusplus)
extern "C"
{
#endif

/*lint ++flb */
#include "dproto.h"
/*lint --flb */
#include "dutilproto.h"

#if defined(__cplusplus)
}
#endif

#endif /* RDM_H */

