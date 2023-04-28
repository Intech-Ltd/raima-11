/* ----------------------------------------------------------------------------
 * Raima Database Manager
 *
 * Copyright (c) 2007-2008 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */

#if !defined(UTILITIES_H)
#define UTILITIES_H

#include "dbtype.h"
#include "expxml.h"
#include "base.h"

#define MAX_TABLES 20

/* ============================================================================
   Preprocessor definitions
   ============================================================================
*/

#define F_ADDR_SPEC0 UINT64_SPEC_SIZE("015")
#define F_ADDR_SPEC_SIZE UINT64_SPEC_SIZE("15")
#define F_ADDR_SPEC UINT64_SPEC
#define F_ADDR_SPEC_HEX0 UINT64_SPEC_SIZE_HEX("016")
#define F_ADDR_SPEC_LEN(size) UINT64_SPEC_SIZE(size)
#define F_ADDR_SPEC_LEN_HEX(size) UINT64_SPEC_SIZE_HEX(size)
#define F_ADDR_SPEC_HEX UINT64_SPEC_HEX

#define F_ADDR4_SPEC0 "%08u"
#define F_ADDR4_SPEC_SIZE "%8u"
#define F_ADDR4_SPEC "%u"
#define F_ADDR4_SPEC_HEX0 "%07X"
#define F_ADDR4_SPEC_LEN(size) "%" size "u"
#define F_ADDR4_SPEC_LEN_HEX(size) "%" size "X"
#define F_ADDR4_SPEC_HEX "%X"

#define DB_ADDR_SPEC "%hu:" F_ADDR_SPEC
#define DB_ADDR_SPEC0 "%05hu:" F_ADDR_SPEC0
#define DB_ADDR_SPEC_HEX "%hX:" F_ADDR_SPEC_HEX

#define DB_ADDR4_SPEC "%hu:" F_ADDR4_SPEC
#define DB_ADDR4_SPEC0 "%03hu:" F_ADDR4_SPEC0
#define DB_ADDR4_SPEC_HEX "%hX:" F_ADDR4_SPEC_HEX

#define EXIT_OK         0
#define EXIT_ERR        -1

#define ERR_INVERR      -1
#define ERR_CHECKID     1
#define ERR_CLOSETASK   2
#define ERR_CMSTAT      3
#define ERR_CMTYPE      4
#define ERR_CONNECT     5
#define ERR_COSTAT      6
#define ERR_COTYPE      7
#define ERR_CRGET       8
#define ERR_CRREAD      9
#define ERR_CRSET       10
#define ERR_CRSTAT      11
#define ERR_CRTYPE      12
#define ERR_CRWRITE     13
#define ERR_CSMGET      14
#define ERR_CSMREAD     15
#define ERR_CSMSET      16
#define ERR_CSMWRITE    17
#define ERR_CSOGET      18
#define ERR_CSOREAD     19
#define ERR_CSOSET      20
#define ERR_CSOWRITE    21
#define ERR_CSSTAT      22
#define ERR_CTBPATH     23
#define ERR_CURKEY      24
#define ERR_DBDPATH     25
#define ERR_DBFPATH     26
#define ERR_DBLOG       27
#define ERR_DBNUM       28
#define ERR_DBTAF       29
#define ERR_DBUSERID    30
#define ERR_DELETE      31
#define ERR_DESTROY     32
#define ERR_DISCON      33
#define ERR_DISDEL      34
#define ERR_FILLNEW     35
#define ERR_FINDCO      36
#define ERR_FINDFM      37
#define ERR_FINDLM      38
#define ERR_FINDNM      39
#define ERR_FINDPM      40
#define ERR_FREEALL     41
#define ERR_GTSCM       42
#define ERR_GTSCO       43
#define ERR_GTSCR       44
#define ERR_GTSCS       45
#define ERR_ICLOSE      46
#define ERR_INITFILE    47
#define ERR_INITIALIZE  48
#define ERR_IOPEN       49
#define ERR_ISMEMBER    50
#define ERR_ISOWNER     51
#define ERR_KEYDEL      52
#define ERR_KEYEXISTS   53
#define ERR_KEYFIND     54
#define ERR_KEYFREE     55
#define ERR_KEYFRST     56
#define ERR_KEYLAST     57
#define ERR_KEYLOCK     58
#define ERR_KEYLSTAT    59
#define ERR_KEYNEXT     60
#define ERR_KEYPREV     61
#define ERR_KEYREAD     62
#define ERR_KEYSTORE    63
#define ERR_LOCK        64
#define ERR_LOCKMGR     65
#define ERR_MAKENEW     66
#define ERR_MAPCHAR     67
#define ERR_MEMBERS     68
#define ERR_OPEN        69
#define ERR_OPENTASK    70
#define ERR_RDCURR      71
#define ERR_RECFREE     72
#define ERR_RECFRST     73
#define ERR_RECLAST     74
#define ERR_RECLOCK     75
#define ERR_RECLSTAT    76
#define ERR_RECNEXT     77
#define ERR_RECPREV     78
#define ERR_RECREAD     79
#define ERR_RECSET      80
#define ERR_RECSTAT     81
#define ERR_RECWRITE    82
#define ERR_RENCLEAN    83
#define ERR_RENFILE     84
#define ERR_RLBCLR      85
#define ERR_RLBSET      86
#define ERR_RLBTST      87
#define ERR_SETDB       88
#define ERR_SETFILES    89
#define ERR_SETFREE     90
#define ERR_SETKEY      91
#define ERR_SETLOCK     92
#define ERR_SETLSTAT    93
#define ERR_SETMM       94
#define ERR_SETMO       95
#define ERR_SETMR       96
#define ERR_SETOM       97
#define ERR_SETOO       98
#define ERR_SETOR       99
#define ERR_SETPAGES    100
#define ERR_SETRM       101
#define ERR_SETRO       102
#define ERR_STSCM       103
#define ERR_STSCO       104
#define ERR_STSCR       105
#define ERR_STSCS       106
#define ERR_TIMEOUT     107
#define ERR_TRABORT     108
#define ERR_TRBEGIN     109
#define ERR_TREND       110
#define ERR_CLOSE       111
#define ERR_ONOPT       112
#define ERR_LOCKCOMM    113
#define ERR_SETDBERR    114
#define ERR_OPENENC     115
#define ERR_DBEXPORT    116
#define ERR_OFFOPT      117

#define LEV_IGNORE      0
#define LEV_WARNING     1
#define LEV_ERROR       2
#define LEV_FATAL       3

#define ERR_SYSTASK      998
#define ERR_CUSTASK      999
#define ERR_ARGS         1000
#define ERR_DIOOPEN      1001
#define ERR_DIOREAD      1002
#define ERR_DIOWRITE     1003
#define ERR_DIOCLOSE     1004
#define ERR_CHSIZE       1005
#define ERR_PASSONE      1006
#define ERR_PASSTWO      1007
#define ERR_PASSTHREE    1008
#define ERR_PREPROCESS   1009
#define ERR_NOMEMORY     1010
#define ERR_STDERROPEN   1011
#define ERR_STDOUTOPEN   1012
#define ERR_FILEOPEN     1013
#define ERR_CNVDBA       2000
#define ERR_CNVOWF       2001
#define ERR_CNVOWL       2002
#define ERR_CNVMEO       2003
#define ERR_CNVMEP       2004
#define ERR_CNVMEN       2005
#define ERR_BADMVREC     2006
#define ERR_PROCESS      2010
#define ERR_KEYBUILD     2020
#define ERR_CLEANTEMP    2021
#define ERR_DCHAIN       2022

#define ERR_SQLALLOCHANDLE 3000

#define ERR_EXPINIT 4000

/* DBCLUSTER error */
#define DBC_ABORT     8000
#define DBC_NOMEM     8001
#define DBC_NOSCRIPT  8002
#define DBC_NODICT    8003
#define DBC_REFADJUST 8004
#define DBC_FIINIT    8005

#define ERR_SENTINEL     10000

#define NMLEN 48

/* DBCHECK/DBREPAIR Error/Status information */
#define CORRUPTED_FL            0x0001
#define DELETED_FL              0x0002
#define LOCKED_FL               0x0004
#define UNUSED_FL               0x0008

#define LASTPAGE_FL             0x0010
#define TERMPAGE_FL             0x0020

/* owner_fix flags */
#define MEMBERS_MOD_FL          0x0001
#define FIRST_MOD_FL            0x0002
#define LAST_MOD_FL             0x0004
#define REC_CORR_FL             0x0100

/* member_Fix flags */
#define OWNER_MOD_FL            0x0001
#define PREV_MOD_FL             0x0002
#define NEXT_MOD_FL             0x0004

/* own_scan flags */
#define MEMBERS_INVAL_FL        0x0001
#define FIRST_INVAL_FL          0x0002
#define LAST_INVAL_FL           0x0004

/* mem_scan flags */
#define OWNER_INVAL_FL          0x0001
#define PREV_INVAL_FL           0x0002
#define NEXT_INVAL_FL           0x0004
#define CHAIN_HEAD_FL           0x0008
#define CHAIN_TAIL_FL           0x0010
#define DISCONNECT_FL           0x0020
#define ORPHAN_FL               0x0040

#define BLID_INVAL_FL           0x0001

/* both own_scan and mem_scan flags */
#define INCONSIST_FL            0x0100


#define BAD_LOCK        1
#define BAD_RID         2
#define BAD_RDBA        3


#define BAD_OFST        6
#define BAD_OLST        7
#define BAD_OMEM        8
#define BAD_OMDL        9
#define BAD_OMRC        10
#define BAD_OMTP        11
#define BAD_LOOP        12
#define BAD_MFPN        13
#define BAD_MLNN        14
#define BAD_MNPW        15
#define BAD_OMOW        16
#define BAD_OCNT        17
#define BAD_MOPB        18
#define BAD_MODL        19
#define BAD_MORC        20
#define BAD_MOIN        21
#define BAD_MONL        22
#define BAD_MNPP        23
#define BAD_MNNP        24
#define BAD_MOKEY       25
#define BAD_OKEY        26
#define BAD_MKEY        27
#define BAD_SLCT        28
#define BAD_KWDL        29
#define BAD_KEYP        30
#define BAD_KPFL        31
#define BAD_KDBA        32
#define BAD_KDAD        33
#define BAD_KREC        34
#define BAD_KDAT        35
#define BAD_TRLV        36
#define BAD_CHSM        37
#define BAD_CHSL        38
#define BAD_CHPT        39
#define BAD_BTRC        40
#define BAD_BTUO        41
#define BAD_BTDD        42
#define BAD_KPST        43
#define BAD_KBTL        44
#define BAD_DCHN        45
#define BAD_KCNDL       46
#define BAD_DCNDL       47
#define BAD_KDLNC       48
#define BAD_DDLNC       49
#define BAD_NCLP        50
#define BAD_RCLP        51
#define BAD_PCLP        52
#define BAD_DIOR        53
#define BAD_DIOG        54
#define BAD_SYSR        55
#define BAD_DCHAIN      57
#define BAD_UKNU        58
#define BAD_SETP        59
#define BAD_SETN        60
#define BAD_OMPP        61
#define BAD_SIZE        62
#define BAD_VARNUMPTRS  63
#define BAD_VARSLOTPTR  64
#define BAD_VARSLOTREAD 65
#define BAD_VAROWN      66
#define BAD_DIORS       67
#define BAD_VARSLOTDUP  68
#define BAD_VARREF      69
#define BAD_BLOBSEQ     70
#define BAD_BLOBPGSIZE  71
#define BAD_BLOBPREV    72
#define BAD_BLOBNEXT    73
#define BAD_BLOBSIZE    74
#define BAD_BLOBPAGE    75
#define BAD_BLOBCHAIN   76
#define BAD_NSLOTERR    77
#define BAD_VAROWNDEL   78
#define BAD_RECTOT      79
#define BAD_HDCHN       80
#define BAD_HBLNF       81
#define BAD_HKNFN       82

#define RUN_DCHAIN      1000
#define RUN_KEYBUILD    1001

/* dbrepair flags ------------------------------------ */

#define DBR_REPORT_SUMMARY    0x0001
#define DBR_SYSLOG_SUMMARY    0x0002

#define DBR_SIMULATE          0x0010
#define DBR_IGNORE_DBA        0x0020
#define DBR_QUICKCHECK        0x0040
#define DBR_DELBADREC         0x0080

/* ============================================================================
   Type definitions
   ============================================================================
*/

typedef struct {
    int16_t     sNumber;
    const char *szText;
} ERR_ENTRY;

typedef struct _dbrevise_params
{
    DB_BOOLEAN       quiet;
    DB_BOOLEAN       verbose;
    int32_t          Src_dbn;
    int32_t          Dest_dbn;
    char             Rdl_path[FILENMLEN];
    char             Report_path[FILENMLEN];
    const char      *Script;
    int16_t          Intr_enable;
    int16_t          Comp_level;
    int16_t          Exec_level;
    int16_t          Abort_level;
    int16_t          Table_f;
    int16_t          Report_f;
    int16_t          Comp_f;
    int16_t          Fast_f;
    PSP_FILE_MODE    Fmode;
    const char      *stdout_file;
} DBREVISE_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbrevise_paramsW
{
    DB_BOOLEAN       quiet;
    DB_BOOLEAN       verbose;
    int32_t          Src_dbn;
    int32_t          Dest_dbn;
    wchar_t          Rdl_path[FILENMLEN];
    wchar_t          Report_path[FILENMLEN];
    const wchar_t   *Script;
    int16_t          Intr_enable;
    int16_t          Comp_level;
    int16_t          Exec_level;
    int16_t          Abort_level;
    int16_t          Table_f;
    int16_t          Report_f;
    int16_t          Comp_f;
    int16_t          Fast_f;
    PSP_FILE_MODE    Fmode;
    const wchar_t   *stdout_file;
} DBREVISE_PARAMSW;
#endif

typedef struct DBREPAIR_PARAMS_S
{
    uint16_t    verbose;
    int16_t     quiet;
    const char *stdout_file;
    const char *stderr_file;
} DBREPAIR_PARAMS;

typedef struct _dbswap_params
{
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      verbose;
    int16_t         overwrite;
    int16_t         dbd_only;
    int16_t         nonnative;
    const char     *stdout_file;
} DBSWAP_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbswap_paramsW
{
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      verbose;
    int16_t         overwrite;
    int16_t         dbd_only;
    int16_t         nonnative;
    const wchar_t  *stdout_file;
} DBSWAP_PARAMSW;
#endif

typedef struct _dbdefrag_params
{
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      verbose;
    const char     *stdout_file;
} DBDEFRAG_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbdefrag_paramsW
{
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      verbose;
    const wchar_t  *stdout_file;
} DBDEFRAG_PARAMSW;
#endif

typedef struct _dbcluster_params
{
    DB_BOOLEAN        quiet;
    DB_BOOLEAN        verbose;
    uint16_t          nBlock;
    uint16_t          numSets;
    const char      **sets;
    const char       *stdout_file;
} DBCLUSTER_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbcluster_paramsW
{
    DB_BOOLEAN        quiet;
    DB_BOOLEAN        verbose;
    uint16_t          nBlock;
    uint16_t          numSets;
    const wchar_t   **sets;
    const wchar_t    *stdout_file;
} DBCLUSTER_PARAMSW;
#endif

typedef struct _dchain_params
{
    DB_BOOLEAN        quiet;
    DB_BOOLEAN        verbose;
    uint16_t          numFiles;
    const char      **files;
    const char       *stdout_file;
} DCHAIN_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dchain_paramsW
{
    DB_BOOLEAN        quiet;
    DB_BOOLEAN        verbose;
    uint16_t          numFiles;
    const wchar_t   **files;
    const wchar_t    *stdout_file;
} DCHAIN_PARAMSW;
#endif

typedef struct _datdump_params
{
    DB_BOOLEAN       quiet;
    uint16_t         hex;
    uint16_t         formatted;
    uint16_t         header;
    F_ADDR           rno;
    const char      *data_file;
    const char      *stdout_file;
} DATDUMP_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _datdump_paramsW
{
    DB_BOOLEAN       quiet;
    uint16_t         hex;
    uint16_t         formatted;
    uint16_t         header;
    F_ADDR           rno;
    const wchar_t   *data_file;
    const wchar_t   *stdout_file;
} DATDUMP_PARAMSW;
#endif

/* dbcheck command line arguments / options */
typedef struct _dbcheck_params
{
    DB_BOOLEAN        quiet;
    uint16_t          setscan;
    uint16_t          keyscan;
    uint16_t          varscan;
    uint16_t          blobscan;
    uint16_t          datkey;
    uint16_t          keydat;
    uint16_t          datvar;
    uint16_t          vardat;
    uint16_t          datblob;
    uint16_t          ignorekey;
    uint16_t          ignorevar;
    uint16_t          ignoreblob;
    uint16_t          report;
    uint16_t          treetrace;
    uint16_t          counts;
    uint16_t          quick_dchain;
    uint16_t          quick_keycheck;
    uint16_t          log_errors;
    const char      **fnames;
    uint16_t          nfnames;
    const char       *stdout_file;
}  DBCHECK_PARAMS;

#if !defined(RDM_NO_UNICODE)
/* dbcheck command line arguments / options */
typedef struct _dbcheck_paramsW
{
    DB_BOOLEAN        quiet;
    uint16_t          setscan;
    uint16_t          keyscan;
    uint16_t          varscan;
    uint16_t          blobscan;
    uint16_t          datkey;
    uint16_t          keydat;
    uint16_t          datvar;
    uint16_t          vardat;
    uint16_t          datblob;
    uint16_t          ignorekey;
    uint16_t          ignorevar;
    uint16_t          ignoreblob;
    uint16_t          report;
    uint16_t          treetrace;
    uint16_t          counts;
    uint16_t          quick_dchain;
    uint16_t          quick_keycheck;
    uint16_t          log_errors;
    const wchar_t   **fnames;
    uint16_t          nfnames;
    const wchar_t    *stdout_file;
}  DBCHECK_PARAMSW;
#endif

typedef struct _dbgen_params
{
    PSP_FILE_MODE  mode;
    uint16_t       cpp_api;  
    uint16_t       c_array;  
    uint16_t       c_header;  
    uint16_t       objc_api;  
    uint16_t       d_flag;  
    uint32_t       z_flag;  
    int8_t         carray_flag;
    const char    *name_space;
    const char    *stdout_file;
} DBGEN_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbgen_paramsW
{
    const wchar_t   *stdout_file;
} DBGEN_PARAMSW;
#endif

typedef struct _ddlp_params
{
    DB_BOOLEAN    quiet;
    const char   *hostname;         /* Name of tfs to connect to (hostname:port)*/
    const char   *masterTFS;        /* Name of master tfs (to place DBD in subdirectory */
    const char   *name_space;       /* Name space for the database */
    const char   *stdout_file;      /* file to use for stdout */
    const char   *stderr_file;      /* file to use for stderr */
#ifndef NO_PREPRO
    const char   *include_path;     /* Optional include path for pre-processor */
#endif
    const char   *ddlfile;          /* Name of ddl file to process */
    int8_t        objc_flag;        /* generate Objective-C interface files */
    int8_t        cpp_flag;         /* generate C++ interface files */
    int8_t        carray_flag;      /* generate C array containing DBD */
    int8_t        no_dbd_flag;      /* don't generate a .dbd file */
    uint16_t      use_dba4;         /* allow duplicate field names */
    uint16_t      d_flag;           /* allow duplicate field names */
    int32_t       e_flag;           /* generate embed code */
#ifndef NO_PREPRO
    int32_t       i_flag;           /* include path for compiler pre-processor */
    int32_t       pp_flag;          /* use pre-processor */
#endif
    int32_t       nh_flag;          /* no database header generated */
    int32_t       m_flag;           /* memory requirements report */
    int32_t       r_flag;           /* report option flag */
    int32_t       s_flag;           /* case sensitivity - default is case sensitive */
    int32_t       x_flag;           /* print identifier cross reference */
    int32_t       z_flag;           /* don't include SIZEOF_??? consts */
    PSP_FILE_MODE fmode;            /* UTF-8 or UTF-16 mode for files */
} DDLP_PARAMS;

typedef struct _ddlp_paramsW
{
    DB_BOOLEAN     quiet;
    const wchar_t *hostname;         /* Name of tfs to connect to (hostname:port)*/
    const wchar_t *masterTFS;        /* Name of master tfs (to place DBD in subdirectory */
    const wchar_t *name_space;       /* Name space for the database */
    const wchar_t *stdout_file;      /* file to use for stdout */
    const wchar_t *stderr_file;      /* file to use for stderr */
#ifndef NO_PREPRO
    const wchar_t *include_path;     /* Optional include path for pre-processor */
#endif
    const wchar_t *ddlfile;          /* Name of ddl file to process */
    int8_t         objc_flag;        /* generate Objective-C interface files */
    int8_t         cpp_flag;         /* generate C++ interface files */
    int8_t         carray_flag;      /* generate C array containing DBD */
    int8_t         no_dbd_flag;      /* don't generate a .dbd file */
    uint16_t       use_dba4;         /* allow duplicate field names */
    uint16_t       d_flag;           /* allow duplicate field names */
    int32_t        e_flag;           /* generate embed code */
#ifndef NO_PREPRO
    int32_t        i_flag;           /* include path for compiler pre-processor */
    int32_t        pp_flag;          /* use pre-processor */
#endif
    int32_t        nh_flag;          /* no database header generated */
    int32_t        m_flag;           /* memory requirements report */
    int32_t        r_flag;           /* report option flag */
    int32_t        s_flag;           /* case sensitivity - default is case sensitive */
    int32_t        x_flag;           /* print identifier cross reference */
    int32_t        z_flag;           /* don't include SIZEOF_??? consts */
    PSP_FILE_MODE  fmode;            /* UTF-8 or UTF-16 mode for files */
} DDLP_PARAMSW;

typedef struct _keybuild_params
{
    DB_BOOLEAN      quiet;
    uint16_t        report_frequency;
    const char     *keyfile_name;
    const char     *stdout_file;
} KEYBUILD_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _keybuild_paramsW
{
    DB_BOOLEAN      quiet;
    uint16_t        report_frequency;
    const wchar_t  *keyfile_name;
    const wchar_t  *stdout_file;
} KEYBUILD_PARAMSW;
#endif

typedef struct _keydump_params
{
    DB_BOOLEAN       quiet;
    int16_t          hex;
    const char      *key_file;
    const char      *stdout_file;
} KEYDUMP_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _keydump_paramsW
{
    DB_BOOLEAN       quiet;
    int16_t          hex;
    const wchar_t   *key_file;
    const wchar_t   *stdout_file;
} KEYDUMP_PARAMSW;
#endif

typedef struct _dbexpcsv_params
{
    DB_BOOLEAN      verbose;
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      decimal;
    DB_BOOLEAN      unicode;
    DB_BOOLEAN      extended;
    DB_BOOLEAN      rec_addr;
    DB_BOOLEAN      mem_addr;
    DB_BOOLEAN      textblob;
    char            sep_char;
    char            esc_char;
    uint16_t        n_recs;
    const char    **recnames;
    const char     *stdout_file;
    void           *printfData; /* Deprecated */
    uint8_t         openfiles;  /* Deprecated */
} DBEXPCSV_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbexpcsv_paramsW
{
    DB_BOOLEAN      verbose;
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      decimal;
    DB_BOOLEAN      unicode;
    DB_BOOLEAN      extended;
    DB_BOOLEAN      rec_addr;
    DB_BOOLEAN      mem_addr;
    DB_BOOLEAN      textblob;
    char            sep_char;
    char            esc_char;
    uint16_t        n_recs;
    const wchar_t **recnames;
    const wchar_t  *stdout_file;
    void           *printfData; /* Deprecated */
    uint8_t         openfiles;  /* Deprecated */
} DBEXPCSV_PARAMSW;
#endif

typedef struct _dbexpxml_params
{
    DB_BOOLEAN      verbose;
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      decimal;
    DB_BOOLEAN      extended;
    uint16_t        n_recs;
    const char    **recnames;
    const char     *out_file;
    int32_t         schema_type;
    const char     *stdout_file;
    const char     *encoding;
    void           *printfData; /* Deprecated */
    uint8_t         openfiles;  /* Deprecated */
} DBEXPXML_PARAMS;

#if !defined(RDM_NO_UNICODE)
typedef struct _dbexpxml_paramsW
{
    DB_BOOLEAN      verbose;
    DB_BOOLEAN      quiet;
    DB_BOOLEAN      decimal;
    DB_BOOLEAN      extended;
    uint16_t        n_recs;
    const wchar_t **recnames;
    const wchar_t  *out_file;
    int32_t         schema_type;
    const wchar_t  *stdout_file;
    const wchar_t  *encoding;
    void           *printfData; /* Deprecated */
    uint8_t         openfiles;  /* Deprecated */
} DBEXPXML_PARAMSW;
#endif

/* Structures and functions to generate the Objective-C API files */
typedef struct objc_field_info {
    const char             *name;
    const char             *name_upper_case;
    FIELD_ENTRY             fd;
    uint32_t                num;
} OBJC_FIELD_INFO;

/* record table */
typedef struct objc_record_info {
    const char           *name;
    const char           *name_upper_case;
    uint16_t              rec_num;
    RECORD_ENTRY          rt;
} OBJC_RECORD_INFO;

/* set table */
typedef struct objc_set_info
{
    const char              *name;
    uint16_t                 num;
    SET_ENTRY                st;
} OBJC_SET_INFO;

typedef struct objc_info
{
    uint16_t    d_flag;
    uint32_t    tot_fields;
    const char *db_name;
    B_BT        bt_rec_num;
    B_BT        bt_fld_num;
    B_BT        bt_set_num;
} OBJC_INFO;

int32_t EXTERNAL_FCN generate_objc_files(const OBJC_INFO *);
void    EXTERNAL_FCN cleanup_objc_info (const OBJC_INFO *);
int32_t objc_rec_num_cmp(const void *, const void *);
int32_t objc_fld_num_cmp(const void *, const void *);
int32_t objc_set_num_cmp(const void *, const void *);

/* Structures and functions to generate the C++ API files */
/* record table */
typedef struct cpp_record_info {
    const char           *name;
    const char           *name_upper_case;
    uint16_t              rec_num;
    RECORD_ENTRY          rt;
} CPP_RECORD_INFO;

/* set table */
typedef struct cpp_set_info
{
    const char         *name;
    const char         *name_upper_case;
    uint16_t            num;
    CPP_RECORD_INFO    *set_rec;
    SET_ENTRY           st;
} CPP_SET_INFO;

/* member table */
typedef struct cpp_member_info
{
    CPP_RECORD_INFO    *mem_rec;
    MEMBER_ENTRY        mt;
    uint16_t            num;
} CPP_MEMBER_INFO;

/* data field table */
typedef struct cpp_field_info {
    const char             *name;
    const char             *rname;
    FIELD_ENTRY             fd;
    uint32_t                num;
} CPP_FIELD_INFO;

typedef struct cpp_info
{
    uint32_t       tot_fields;
    const char    *db_name;
    const char    *db_name_upper_case;
    PSP_FILE_MODE  fmode;
    uint16_t       d_flag;
    int8_t         carray_flag;
    B_BT           bt_rec_num;
    B_BT           bt_fld_num;
    B_BT           bt_set_num;
    B_BT           bt_mem_num;
} CPP_INFO;

void EXTERNAL_FCN cleanup_cpp_info(const CPP_INFO *);
int32_t EXTERNAL_FCN generate_cpp_files(const CPP_INFO *);
int32_t cpp_rec_num_cmp(const void *, const void *);
int32_t cpp_fld_num_cmp(const void *, const void *);
int32_t cpp_set_num_cmp(const void *, const void *);
int32_t cpp_mem_num_cmp(const void *, const void *);

/* Structures and functions to generate the dbd c header file */
/* file table */
typedef struct hdr_file_info {
    FILE_NO                fileno;
    const char            *fileid;
} HDR_FILE_INFO;

/* record table */
typedef struct hdr_record_info {
    const char           *name;
    const char           *name_upper_case;
    uint16_t              rec_num;
    RECORD_ENTRY          rt;
} HDR_RECORD_INFO;

/* set table */
typedef struct hdr_set_info
{
    const char         *name;
    const char         *name_upper_case;
    uint16_t            num;
    SET_ENTRY           st;
} HDR_SET_INFO;

typedef struct hdr_const_info
{
    const char *name;
    uint64_t    value;
} HDR_CONST_INFO;

typedef struct hdr_type_info
{
    const char     *name;
    char            type_char;
    uint16_t        dims[MAXDIMS];
} HDR_TYPE_INFO;

/* data field table */
typedef struct hdr_field_info {
    const char             *name;
    HDR_CONST_INFO         *dim_const[MAXDIMS];
    HDR_TYPE_INFO          *type;
    FIELD_ENTRY             fd;
    uint32_t                num;
} HDR_FIELD_INFO;

/* compound key table */
typedef struct hdr_key_info {
    uint16_t    num;
    KEY_ENTRY   kt;
} HDR_KEY_INFO;

typedef struct hdr_info
{
    const char     *db_name;
    const char     *db_name_upper_case;
    const char     *ddlfile;
    PSP_FILE_MODE   fmode;
    uint16_t        d_flag;
    uint16_t        z_flag;
    HDR_FIELD_INFO *cur_struct;
    B_BT            bt_file_num;
    B_BT            bt_rec_num;
    B_BT            bt_set_num;
    B_BT            bt_const;
    B_BT            bt_type;
    B_BT            bt_fld_num;
    B_BT            bt_key_num;
} HDR_INFO;

void EXTERNAL_FCN cleanup_hdr_info(const HDR_INFO *);
int32_t EXTERNAL_FCN generate_c_header(HDR_INFO *);
int32_t hdr_file_num_cmp(const void *, const void *);
int32_t hdr_rec_num_cmp(const void *, const void *);
int32_t hdr_set_num_cmp(const void *, const void *);
int32_t hdr_const_cmp(const void *, const void *);
int32_t hdr_type_cmp(const void *, const void *);
int32_t hdr_fld_num_cmp(const void *, const void *);
int32_t hdr_key_num_cmp(const void *, const void *);

/* Structures and functions to generate the dbd c array files */

typedef struct carray_file_info {
    FILE_NO                fileno;
    FILE_ENTRY             ft;
} CARRAY_FILE_INFO;

typedef struct carray_record_info {
    uint16_t              rec_num;
    RECORD_ENTRY          rt;
    const char           *name;
} CARRAY_RECORD_INFO;

typedef struct carray_field_info {
    FIELD_ENTRY             fd;
    uint32_t                num;
    const char             *name;
} CARRAY_FIELD_INFO;

typedef struct carray_set_info
{
    uint16_t            num;
    SET_ENTRY           st;
    const char         *name;
} CARRAY_SET_INFO;

typedef struct carray_member_info
{
    MEMBER_ENTRY        mt;
    uint16_t            num;
} CARRAY_MEMBER_INFO;

typedef struct carray_key_info {
    uint16_t    num;
    KEY_ENTRY   kt;
} CARRAY_KEY_INFO;

typedef struct carray_sort_info {
    uint16_t    num;
    SORT_ENTRY  srt;
} CARRAY_SORT_INFO;

typedef struct carray_info
{
    const char    *db_name;
    const char    *db_name_uppercase;
    const char    *name_space;
    const char    *dba_vernum;
    PSP_FILE_MODE  fmode;
    uint16_t       max_page_size;
    uint16_t       tot_files;
    uint16_t       tot_records;
    uint32_t       tot_fields;
    uint16_t       tot_sets;
    uint16_t       tot_members;
    uint16_t       tot_sort_fields;
    uint16_t       tot_comkeyflds;
    B_BT           bt_file_num;
    B_BT           bt_rec_num;
    B_BT           bt_fld_num;
    B_BT           bt_set_num;
    B_BT           bt_mem_num;
    B_BT           bt_key_num;
    B_BT           bt_sort_num;
} CARRAY_INFO;

void EXTERNAL_FCN cleanup_carray_info(const CARRAY_INFO *);
int32_t EXTERNAL_FCN generate_carray_files(const CARRAY_INFO *);
int32_t carray_file_num_cmp(const void *, const void *);
int32_t carray_rec_num_cmp(const void *, const void *);
int32_t carray_fld_num_cmp(const void *, const void *);
int32_t carray_set_num_cmp(const void *, const void *);
int32_t carray_mem_num_cmp(const void *, const void *);
int32_t carray_key_num_cmp(const void *, const void *);
int32_t carray_sort_num_cmp(const void *, const void *);

/* ============================================================================
   Function prototypes
   ============================================================================
*/
void EXTERNAL_FCN ReportError(const char *, int32_t, const ERR_ENTRY*, int32_t, int16_t);

void DisplayFADDR(uint16_t, uint16_t, F_ADDR);
int32_t dbcheck_chk_dba(const DB_ADDR  *uno, const DB_ENTRY *db);

char * EXTERNAL_FCN FmtDBA(const DB_ADDR*, char *);

int32_t ddatdump(const DATDUMP_PARAMS *, DB_ENTRY *);
int32_t ddbcheck(const DBCHECK_PARAMS *, uint32_t *, uint32_t *, DB_ENTRY *);
int32_t ddbcluster(const DBCLUSTER_PARAMS *, DB_ENTRY *);
int32_t ddbdefrag(const DBDEFRAG_PARAMS *, DB_ENTRY *);
int32_t ddbexpcsv(const DBEXPCSV_PARAMS *, DB_ENTRY *);
int32_t ddbexpxml(const DBEXPXML_PARAMS *, DB_ENTRY *);
int32_t ddbgen(const DBGEN_PARAMS *, const DB_ENTRY *);
int32_t ddbrepair(const DBREPAIR_PARAMS *, const DB_ENTRY *);
int32_t ddbrevise(const DBREVISE_PARAMS *, DB_TASK *);
int32_t ddbswap(const char *, const DBSWAP_PARAMS *);
int32_t ddchain(const DCHAIN_PARAMS *, DB_ENTRY *);
int32_t dkeybuildEx(const KEYBUILD_PARAMS *, const DB_ENTRY *);
int32_t dkeydump(const KEYDUMP_PARAMS *, DB_ENTRY *);

/* ============================================================================
   Extern definitions
   ============================================================================
*/
extern const ERR_ENTRY ErrTable[];

#endif  /* UTILITIES_H */
