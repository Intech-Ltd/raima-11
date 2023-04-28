/* ----------------------------------------------------------------------------
 * Raima Database Manager
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include "rdm.h"
#include "mirutils.h"
#include "ia_db1.h"

#include "ia_db1_dbd.h"
#include "ia_db2_dbd.h"

typedef struct _REPL_EXAMPLE_OPTS
{
    const char     *master_host;
    int32_t         master_port;
    const char     *slave_docroot;
    int32_t         slave_port;
    cmdline_status  cmd_status;
}  REPL_EXAMPLE_OPTS;

static int32_t configure_slave(const char *, int32_t, TFS_HANDLE *, REP_HANDLE *);
static int32_t cleanup_slave(TFS_HANDLE, REP_HANDLE);
static int32_t read_slave_data(const char *, int32_t);
static int32_t GetOptions(int32_t, const char *const *, REPL_EXAMPLE_OPTS *);
static char *getstring(char *s, size_t size);
static void EXTERNAL_FCN mirrorSlave_dberr( int32_t, const char *, void *, DB_TASK *);

int32_t EXTERNAL_FCN replSlave_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t                 rdm_status;
    TFS_HANDLE              hTFS = (void *) -1;
    REP_HANDLE              hREP = (void *) -1;
    uint16_t                slaveID1;
    uint16_t                slaveID2;
    REPL_EXAMPLE_OPTS       dbrep_opts;
    DBREP_CONNECT_PARAMS    db_connect;
    DBREP_DISCONNECT_PARAMS db_disconnect;
    int16_t                 quit = 0;
    char                    cmd[20];

    /* Get the command line arguments */
    memset(&dbrep_opts, 0, sizeof(dbrep_opts));
    if (GetOptions(argc, argv, &dbrep_opts) != B_CMD_OKAY)
    {
        if(dbrep_opts.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(dbrep_opts.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    /* Start slave threads */
    printf("Starting slave TFS and dbmirror threads\n");
    rdm_status = configure_slave(dbrep_opts.slave_docroot, dbrep_opts.slave_port, &hTFS, &hREP);
    if(rdm_status != S_OKAY)
    {
        cleanup_slave(hTFS, hREP);
        return 1;
    }

    /* Connect db1 to the master */
    printf("Starting mirroring for db1\n");
    memset(&db_connect, 0, sizeof(db_connect));
    db_connect.dburl = psp_makeString("ia_db1@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
    db_connect.port = dbrep_opts.slave_port;
    db_connect.override_inmem = 1;
    db_connect.hostname = psp_strdup("localhost", TAG0);
    rdm_status = d_dbrep_connect(&db_connect, &slaveID1);
    if(rdm_status != S_OKAY)
    {
        printf("Unable to connect to master for replication (%d)\n", rdm_status);
    }
    psp_freeMemory(db_connect.dburl, TAG0);
    
    /* Connect db2 to the master */
    printf("Starting mirroring for db2\n");
    db_connect.dburl = psp_makeString("ia_db2@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
    rdm_status = d_dbrep_connect(&db_connect, &slaveID2);
    if(rdm_status != S_OKAY)
    {
        printf("Unable to connect to master for replication (%d)\n", rdm_status);
    }
    psp_freeMemory(db_connect.dburl, TAG0);
    psp_freeMemory(db_connect.hostname, TAG0);

    while (!quit)
    {
        /* display command menu */
        printf("\nreplSlave Commands:\n");
        printf("   1 - Display data in slave databases\n");
        printf("   q - Quit\n");
        printf("enter command: ");
        fflush (stdout);

        getstring(cmd, sizeof(cmd));

        switch (cmd[0])
        {
        case 'q':
        case 'Q':
            /* Disconnect db1 from the slave */
            printf("Stopping mirroring for db1\n");
            memset(&db_disconnect, 0, sizeof(db_disconnect));
            db_disconnect.dburl = psp_makeString("ia_db1@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
            db_disconnect.hostname = psp_strdup("localhost", TAG0);
            db_disconnect.port = dbrep_opts.slave_port;
            rdm_status = d_dbrep_disconnect(&db_disconnect);
            psp_freeMemory(db_disconnect.dburl, TAG0);
    
            /* Disconnect db2 from the slave */
            printf("Stopping mirroring for db2\n");
            db_disconnect.dburl = psp_makeString("ia_db2@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
            rdm_status = d_dbrep_disconnect(&db_disconnect); 
            psp_freeMemory(db_disconnect.hostname, TAG0);
            psp_freeMemory(db_disconnect.dburl, TAG0);

            /* Cleanup slave threads */
            printf("Cleaning up slave threads\n");
            cleanup_slave(hTFS, hREP);
            quit = 1;
            break;
       
        case '1':
            /* Read data from the slave database */
            printf("Reading the data from the slave database\n");
            read_slave_data(dbrep_opts.master_host, dbrep_opts.master_port);
            break; 

        default:
            printf("*** incorrect command -- re-enter\n");
            break;
        }
    }

    if(dbrep_opts.slave_docroot)
    {
        psp_freeMemory(dbrep_opts.slave_docroot, TAG0);
    }
    
    if(dbrep_opts.master_host)
    {
        psp_freeMemory(dbrep_opts.master_host, TAG0);
    }
    
    return 0;
}

static int32_t configure_slave(
    const char *docroot,
    int32_t     port,
    TFS_HANDLE *hTFS,
    REP_HANDLE *hREP)
{
    int32_t    rdm_status;
    DBREP_INIT_PARAMS dbmirrorParams;

    /* Initialize the slave TFS threads, note that there are several other ways
       to do this depending on application requirements/needs
          * external daemon running
          * start external process from the command line
    */

    rdm_status = d_tfsinitEx(docroot ? docroot : "", NULL, hTFS, TFS_TYPE_TFS);
    if(rdm_status == S_OKAY)
    {
        rdm_status = d_tfsrun(*hTFS, TRUE, NULL);
        if(rdm_status != S_OKAY)
        {
            printf("Error starting the slave TFS threads\n");
            return rdm_status;
        }
    }
    else
    {
        printf("Error initializing the slave TFS threads\n");
        return rdm_status;
    }

    /* Initialize the slave dbmirror threads, note that there are other ways to
       do this depending on application requirements/needs
          * external daemon running
          * start externally from the command line
    */
    memset(&dbmirrorParams, 0, sizeof(dbmirrorParams));
    dbmirrorParams.docroot = docroot;
    dbmirrorParams.port = port;
    dbmirrorParams.verbose = FALSE;
    dbmirrorParams.diskless = FALSE;
    dbmirrorParams.stdout_file = PSP_STDOUT;
    rdm_status = d_dbmir_init(&dbmirrorParams, hREP);
    if(rdm_status == S_OKAY)
    {
        rdm_status = d_dbrep_start(*hREP, TRUE, NULL);
        if(rdm_status != S_OKAY)
        {
            printf("Error starting the slave dbmirror threads\n");
            return rdm_status;
        }
    }
    else
    {
        printf("Error initializing the slave dbmirror threads\n");
        return rdm_status;
    }
    return S_OKAY;
}

static int32_t cleanup_slave(
    TFS_HANDLE hTFS,
    REP_HANDLE hREP)
{
    int32_t rdm_status;

    if(hREP != (void *) -1)
    {
        rdm_status = d_dbrep_stop(hREP);
        if(rdm_status == S_OKAY)
        {
            rdm_status = d_dbrep_term(hREP);
        }
    }
    
    if(hTFS != (void *) -1)
    {
        rdm_status = d_tfsstop(hTFS);
        if(rdm_status == S_OKAY)
        {
            rdm_status = d_tfsterm(hTFS);
        }
    }
    return S_OKAY;
}

static int32_t read_slave_data(
    const char *master_host,
    int32_t     master_port)
{
    int32_t     rdm_status;
    DB_TASK    *task;
    const char *db = NULL;
    struct unit unit_rec;
    int32_t     ii = 0;
    int32_t     num_dbs = 1;
    int32_t     dbn;

    if((rdm_status = d_opentask(&task)) == S_OKAY)
    {
        if((rdm_status = d_set_dberr_ex(mirrorSlave_dberr, (void*)NULL, task)) != S_OKAY)
        {
            printf("Error %d setting error handler\n", rdm_status);
            d_closetask(task);
            return S_NOMEMORY;
        }

        /* Create the db open name for the slaves (as a union) */
        db = psp_makeString("%s-%d\\ia_db1|%s-%d\\ia_db2", master_host, master_port, master_host, master_port);
        if(!db)
        {
            printf("Error out of memory\n");
            d_closetask(task);
            return S_NOMEMORY;
        }

        if ((rdm_status = d_open(db, "r", task)) != S_OKAY)
        {
            /* We are not using a runtime library that supports unions so
             *   we need to read each database individually */
            if(rdm_status == S_NOTIMPLEMENTED)
            {
                psp_freeMemory(db, TAG0);
                /* Create the db open name for the slaves */
                db = psp_makeString("%s-%d\\ia_db1;%s-%d\\ia_db2", master_host, master_port, master_host, master_port);
                if(!db)
                {
                    printf("Error out of memory\n");
                    d_closetask(task);
                    return S_NOMEMORY;
                }
                if ((rdm_status = d_open(db, "r", task)) != S_OKAY)
                {
                    printf("Error %d opening databases\n", rdm_status);
                    d_closetask(task);
                    return S_NOMEMORY;
                }
                num_dbs = 2;
            }

            if(rdm_status != S_OKAY)
            {
                printf("Error %d opening databases as a union\n", rdm_status);
                psp_freeMemory(db, TAG0);
                d_closetask(task);
                return rdm_status;
            }
        }
        psp_freeMemory(db, TAG0);
       
        for(dbn = 0; dbn < num_dbs; dbn++)
        { 
            rdm_status = d_recfrst(UNIT, task, dbn);
            while(rdm_status == S_OKAY)
            {
                rdm_status = d_recread(&unit_rec, task, dbn);
                if(rdm_status == S_OKAY)
                {
                    printf("Record %d\n\tName: %s\n\tUnit: %s\n\tPhone: %s\n\tFax: %s\n\tE-mail: %s\n", ++ii, unit_rec.name, unit_rec.unitno, unit_rec.phone, unit_rec.fax, unit_rec.email);
                }
                else
                    break;

                rdm_status = d_recnext(task, dbn);
            }
            if(rdm_status != S_NOTFOUND)
            {
                printf("Error %d reading unit record\n", rdm_status);
            }
            else
            {
                rdm_status = S_OKAY;
            }
        }
        d_close(task);
        d_closetask(task);
    }
    return rdm_status;
}

static int32_t GetOptions(
    int32_t            argc, 
    const char *const *argv,
    REPL_EXAMPLE_OPTS *repl_opts)
{
    int32_t      optstat;
    const char  *opt;
    const char  *optarg;
    default_opts defaults;

    /*lint -save -e785 */
    cmdline_opt opts[] = {
        { ARG_OPT,         "=slave_docroot",       "s=path",       "Document root for the slave TFS instance" },
        { ARG_OPT,         ":slave_port",          "i=port",       "Anchort port for the slave TFS instance (default 22553)" },
        { ARG_OPT,         ":master_host",         "s=path",       "Host for the master (default localhost)" },
        { ARG_OPT,         ":master_port",         "i=port",       "Anchor port root for the master (default 21553) " },
        { ARG_END,         NULL,                   NULL,           NULL }
    };
    /*lint -restore */

    RDM_ASSERT(repl_opts);
    memset(&defaults, 0, sizeof(default_opts));
    defaults.no_tfs = 1;

    repl_opts->master_port = 21553;
    repl_opts->slave_port  = 22553;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("Replication"), argc,
            argv, opts, &defaults, &repl_opts->cmd_status);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 's':
                if(strcmp(opt, "slave_docroot") == 0)
                {
                    repl_opts->slave_docroot = psp_strdup(optarg, TAG0);
                }
                else if(strcmp(opt, "slave_port") == 0)
                {
                    repl_opts->slave_port = atoi(optarg);
                }
                break;

            case 'm':
                if(strcmp(opt, "master_host") == 0)
                {
                    repl_opts->master_host = psp_strdup(optarg, TAG0);
                }
                else if(strcmp(opt, "master_port") == 0)
                {
                    repl_opts->slave_port = atoi(optarg);
                }
                break;

        } /*lint !e744 */
    }
    if(repl_opts->master_host == NULL)
    {
        repl_opts->master_host = psp_strdup("localhost", TAG0);
    }

    return B_CMD_OKAY;
}

static char *getstring(char *s, size_t size)
{
    size_t len;

    if (fgets(s, (int32_t) (uint32_t) size, stdin) == NULL)
        return NULL;

    len = strlen(s);
    if (s[len-1] == '\n')
        s[len-1] = '\0';

    return s;
}

static void EXTERNAL_FCN mirrorSlave_dberr(
    int32_t     errnum, 
    const char *msg,
    void       *data,
    DB_TASK    *task)
{
    UNREF_PARM(errnum)
    UNREF_PARM(msg)
    UNREF_PARM(data)
    UNREF_PARM(task)
}

PSP_STARTUP_EXAMPLE(replSlave)
