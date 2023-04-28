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

#if !defined(__DBMIRROR_H)
#define __DBMIRROR_H

#include "rdm.h"
#include "psp.h"
#include "rdmhttp.h"
#include "mirutils.h"
#include "rdmsql.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Slave log type, request db, mirroring or replication log files */
#define ST_MIRROR       0
#define ST_REPLICATION  1
#define ST_REP2SQL      2
#define ST_DBLOG        3

/* Slave status */
#define SS_STARTING     0
#define SS_LOGIFYING    1
#define SS_READY        2
#define SS_NOTRUNNING   3

/* Mirroring Master commit timeout in millisecs */
#define MM_COMMIT_TIMEOUT        10000
/* Mirroring Slave getNextLog timeout to detect lost connection */
#define MS_GETLOG_TIMEOUT        11000

typedef struct _bt_fileinfo {
    const char             *filename;
    uint16_t                lm_fid;     /* lockmgr file id */
    uint16_t                flags;
} BT_FILEINFO;

typedef void const *          MIRLOCK_HANDLE;


/********************* slave context ************************************/
/* Running slave entries. One per slave thread.  */
typedef struct _mir_slave {
    int8_t                  thread_done;/* set to 1 to signal thread is done*/
    int8_t                  shutdown_thread; /* set to 1 to signal end of thread */
    uint16_t                slaveid;    /* unique slave id - one per thread */
    char                   *docroot;    /* slave server document root */
    char                   *dburl;      /* db to be replicated: db@host:port */
    char                   *dbname;     /* database to be replicated */
    char                   *dbhost;     /* master host of database */
    uint16_t                dbport;     /* master port of database */
    char                   *dbdir;      /* full database file directory */
    char                   *dbrelpath;  /* relative path to database: host\db */
    char                   *dbuserid;   /* user id for login to TFS */
    const char             *odbc_src;   /* slave's ODBC source if sql */
    PSP_MEMTAG              mtag;       /* memory tag for this slave */
    PSP_MEMTAG              btmtag;     /* memory tag for fileinfo btree */
    B_BT                    btDbFiles;  /* db file info from dbd */
    PSP_MEMTAG              btLogifiedMtag;/* mtag for logified files btree */
    B_BT                    btLogifiedFiles;/* logified file names from logs */
    struct _mir_slave      *next;       /* next slave in list */
    uint16_t                verbose;    /* verbose messages */
    TX_DB                   dbID;       /* The one db for this slave thread */
    TX_CONN                 master_conn;/* connection to master server */
    TX_USER                 tfs_userID; /* Logged in user to TFServer */
    uint16_t                tfs_port;   /* port of TFServer */
    struct _mir_slave_ctx  *sctx;       /* parent: global slave context */
    void                   *handlerCtx; /* context of logHandler, dbrepsql */
    REP_SLAVE_TYPE          e2sTarget;  /* target server(RST_RDMS,RST_MYSQL..)*/
    DB_BOOLEAN              synchronous;/* this slave is doing synch mirroring*/
    DB_BOOLEAN              is_master;  /* this slave is also a master */
    DB_BOOLEAN              override_inmem;/* db files stored to disk if true*/
    DFL_TYPE                db_file_location;/* in-mem or on-disk */
    DB_TASK                *rep_task;   /* replication task */
    HCONN                   rep_hdbc;   /* local SQL connection */
    const SYSDB            *sysdb;      /* local SQL information */
    PSP_SEM                 replog_thread_sem;/* thread to process replogs */
    uint16_t                replog_thread_state;/* is replog thread running? */
    int32_t                 replog_thread_rc;/* rc from process_replog */
    TXID                    replog_thread_txid;/*last txid processed by thread*/
    TXID                    last_txid;  /* last txid for mir and sql */
    MIRLOCK_HANDLE          hLockedDbFiles;/* locked db files - mutil_lock*() */
    uint16_t                xflags;     /* TFS optimization stores logs inmem */
    uint32_t                diskless;   /* is slave in diskless mode? */
    uint16_t                status;     /* status: starting, logifying, ready */
    psp_thread_t            TID;        /* The TID of the slave thread */
    psp_thread_t            prTID;      /* The TID of the replication log thread */
} MIR_SLAVE;

typedef int32_t (*LOGHANDLER_FCN)(MIR_SLAVE *, char *, TXID);
typedef int32_t (*PRELOGIFY_FCN)(MIR_SLAVE *);
typedef int32_t (*POSTLOGIFY_FCN)(MIR_SLAVE *);
typedef void    (*HANDLERTERM_FCN)(MIR_SLAVE *);

/* Slave server's global context: One per instance of dbmirror.
   Slave list must be semaphore protected.
 */
typedef struct _mir_slave_ctx {
    MIR_SLAVE           *slave_list;/* list of slave threads */
    uint8_t              slave_type;/* type of logs to request: mir|rep */
    uint16_t             numslaves; /* number of slaves in slave list */
    PSP_SEM              mutex;     /* mutex to protect list */
    LOGHANDLER_FCN       logHandler;/* function to process a log file */
    PRELOGIFY_FCN        preLogify; /* function to run before applying db logs*/
    POSTLOGIFY_FCN       postLogify;/* function to after applying db logs */
    HANDLERTERM_FCN      termHandler;/* function to cleanup logHandler */
} MIR_SLAVE_CTX;



/******************** master context ************************************/
/* Slave node of MIR_MASTER_DB->slave_list which contains the contact 
   information for a slave that is mirroring or replicating a database.
 */
typedef struct _registered_slave {
    RHS_CONN_CTX               *connCtx;        /* slave's connection handle */
    char                       *hostname;       /* slave hostname */
    char                       *odbc_src;       /* slave's ODBC source if sql */
    TXID                        last_txid;      /* lasttxid sent to this slave*/
    TX_FILE                     curr_log;       /* current logfile handle */
    uint16_t                    port;           /* slave's TFS port */
    uint16_t                    type;           /* slave type: mir/rep/sql */
    uint16_t                    status;         /* status: start,logify,ready */
    DB_BOOLEAN                  synchronous;    /* Is this slave synchronous? */
    struct _registered_slave   *next;           /* next node in list */
} REGISTERED_SLAVE;

/* Replicated database info. One per replicated database on the master.
   Multiple slaves can replicate the same db, there will only be one
   MIR_MASTER_DB per db, not per slave.  Multiple dbs can be logified
   at the same time by different connection threads.
*/
typedef struct _mir_mdb {
    PSP_MEMTAG           mtag;        /* memory tag for this master */
    char                *dbname;      /* database to be replicated */
    uint16_t             dblog_creating;/* Creating new set of dblog files */
    PSP_SEM              mdb_mutex;   /* mutex to protect this mdb */
    const RHS_CONN_CTX  *sync_slave;  /* slave doing synchronous mirroring */
    TX_USER              sync_userID; /* logged in user for synch mirroring */
    TX_USER              db_userID;   /* user id for tx_LOGwaitForCommit */
    TX_DB                dbID;        /* dbid for tx_LOGwaitForCommit */
    REGISTERED_SLAVE    *slave_list;  /* list of slaves using this database */
    uint32_t             slave_count; /* Num of connected slaves in slave_list*/
    DB_BOOLEAN           single_slave_mode; /* log file cleanup mode */
    DB_BOOLEAN           iasync_mode; /* entered iasync mode */
    uint16_t             xflags;      /* TFS optimization stores logs inmem */
    struct _mir_mdb     *next;        /* next mdb in list */
} MIR_MASTER_DB;


/* Master server's global context: One per instance of dbmirror.
   mdb_list must be semaphore protected.
*/
typedef struct _mir_master_ctx {
    MIR_MASTER_DB       *mdb_list;  /* list of replicated databases */
    uint16_t             numdbs;    /* number of dbs in mdb_list */
    PSP_SEM              mutex;     /* mutex to protect master_db list */
    char                *docroot;   /* master's document root */
} MIR_MASTER_CTX;



/******************** mirror context ************************************/
typedef struct _mir_ctx {
    MIR_SLAVE_CTX       *slave_ctx;     /* Global slave context */
    MIR_MASTER_CTX      *master_ctx;    /* Global master context */
} MIR_CTX;


/************************************************************************/

MIR_SLAVE_CTX  * EXTERNAL_FCN mir_slave_init(uint8_t, LOGHANDLER_FCN, PRELOGIFY_FCN, POSTLOGIFY_FCN, HANDLERTERM_FCN);
void             EXTERNAL_FCN mir_slave_term(MIR_SLAVE_CTX *);
MIR_MASTER_CTX * EXTERNAL_FCN mir_master_init(const RHS_PARAMS *);
void             EXTERNAL_FCN mir_master_term(MIR_MASTER_CTX *);
int32_t          EXTERNAL_FCN mir_master_discon(TX_USER, void *);

int32_t          EXTERNAL_FCN mir_rpc_reqhandler(void *);
int32_t          EXTERNAL_FCN mir_main(int32_t, const char *const *, const TFS_XAPI_FCNMAP *, uint16_t, uint8_t, LOGHANDLER_FCN, PRELOGIFY_FCN, POSTLOGIFY_FCN, HANDLERTERM_FCN);
void             EXTERNAL_FCN mir_deleteAllLogs(TX_USER, const char *, uint16_t, const char *);
MIR_MASTER_DB  * EXTERNAL_FCN mir_master_getNextMDB(const MIR_CTX *, const MIR_MASTER_DB *);


#if defined(__cplusplus)
}
#endif

#endif  /* __DBMIRROR_H */

