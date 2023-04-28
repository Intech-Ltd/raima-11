/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
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
    const char     *master_docroot;
    const char     *master_host;
    int32_t         master_port;
    const char     *slave_host;
    int32_t         slave_port;
    uint16_t        verbose;
    cmdline_status  cmd_status;
}  REPL_EXAMPLE_OPTS;

static int32_t cleanup_db(DB_TASK *);
static int32_t init_db(const char *, const void *, int32_t, DB_TASK **);

static int32_t add_building(struct unit *, DB_TASK *, int32_t);
static int32_t configure_master(const char *, int32_t, uint16_t, TFS_HANDLE *, REP_HANDLE *);
static int32_t cleanup_master(TFS_HANDLE, REP_HANDLE);
static int32_t GetOptions(int32_t, const char *const *, REPL_EXAMPLE_OPTS *);
static char *getstring(char *s, size_t size);

int32_t EXTERNAL_FCN replMaster_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t                 rdm_status;
    DB_TASK                *task1;
    DB_TASK                *task2;
    struct unit             unit_rec;
    uint16_t                slaveID1;
    uint16_t                slaveID2;
    TFS_HANDLE              hTFS = 0;
    REP_HANDLE              hREP = 0;
    DBREP_CONNECT_PARAMS    db_connect;
    DBREP_DISCONNECT_PARAMS db_disconnect;
    REPL_EXAMPLE_OPTS       dbrep_opts;
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

    /* Start master threads */
    DisplayMessageNow(FALSE, FALSE, "Starting master TFS and dbrep threads\n");
    rdm_status = configure_master(dbrep_opts.master_docroot, dbrep_opts.master_port, dbrep_opts.verbose, &hTFS, &hREP);
    if(rdm_status != S_OKAY)
    {
        cleanup_master(hTFS, hREP);
        return 1;
    }

    /* Setup database 1 */
    DisplayMessageNow(FALSE, FALSE, "Creating master db1\n");
    if ((rdm_status = init_db("ia_db1", &ia_db1_dbd, sizeof(ia_db1_dbd), &task1)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Failed to initialize the db1 task (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        cleanup_master(hTFS, hREP);
        return (1);
    }
    
    /* Setup database 2 */
    DisplayMessageNow(FALSE, FALSE, "Creating master db2\n");
    if ((rdm_status = init_db("ia_db2", &ia_db2_dbd, sizeof(ia_db2_dbd), &task2)) != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Failed to initialize the db task (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        cleanup_db(task1);
        cleanup_master(hTFS, hREP);
        return (1);
    }
    
    /* Connect db1 to the slave */
    DisplayMessageNow(FALSE, FALSE, "Starting replication for db1\n");
    memset(&db_connect, 0, sizeof(db_connect));
    db_connect.dburl = psp_makeString("ia_db1@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
    db_connect.port = dbrep_opts.slave_port;
    db_connect.override_inmem = 1;
    db_connect.hostname = dbrep_opts.slave_host;
    rdm_status = d_dbrep_connect(&db_connect, &slaveID1);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to connect to slave for replication (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
    }
    psp_freeMemory(db_connect.dburl, TAG0);
    
    /* Connect db2 to the slave */
    DisplayMessageNow(FALSE, FALSE, "Starting replication for db2\n");
    db_connect.dburl = psp_makeString("ia_db2@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
    db_connect.port = dbrep_opts.slave_port;
    rdm_status = d_dbrep_connect(&db_connect, &slaveID2);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Unable to connect to slave for replication (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));

    }
    psp_freeMemory(db_connect.dburl, TAG0);

    /* Add a building to db1 */
    DisplayMessageNow(FALSE, FALSE, "Adding a record to db1\n");
    strcpy(unit_rec.name, "Raima HQ");
    strcpy(unit_rec.unitno, "123");
    strcpy(unit_rec.phone, "206-748-5252");
    strcpy(unit_rec.fax, "206-748-5300");
    strcpy(unit_rec.email, "sales@raima.com");
    rdm_status = add_building(&unit_rec, task1, CURR_DB);
    
    /* Add a building to db2 */
    DisplayMessageNow(FALSE, FALSE, "Adding a record to db2\n");
    strcpy(unit_rec.name, "Raima Idaho");
    strcpy(unit_rec.unitno, "3822");
    strcpy(unit_rec.phone, "206-748-5208");
    strcpy(unit_rec.fax, "");
    strcpy(unit_rec.email, "support@raima.com");
    rdm_status = add_building(&unit_rec, task2, CURR_DB);

    while (!quit)
    {
        /* display command menu */
        DisplayMessage(FALSE, FALSE, "\nreplMaster Commands:\n");
        DisplayMessage(FALSE, FALSE, "   1 - Add building record to database 1\n");
        DisplayMessage(FALSE, FALSE, "   2 - Add building record to database 2\n");
        DisplayMessage(FALSE, FALSE, "   q - Quit\n");
        DisplayMessageNow(FALSE, FALSE, "enter command: ");

        getstring(cmd, sizeof(cmd));

        switch (cmd[0])
        {
        case '1':
            DisplayMessageNow(FALSE, FALSE, "Enter Name: ");
            getstring(unit_rec.name, sizeof(unit_rec.name));
            DisplayMessageNow(FALSE, FALSE, "Enter Unit Number: ");
            getstring(unit_rec.unitno, sizeof(unit_rec.unitno));
            DisplayMessageNow(FALSE, FALSE, "Enter Phone: ");
            getstring(unit_rec.phone, sizeof(unit_rec.phone));
            DisplayMessageNow(FALSE, FALSE, "Enter Fax: ");
            getstring(unit_rec.fax, sizeof(unit_rec.fax));
            DisplayMessageNow(FALSE, FALSE, "Enter Email: ");
            getstring(unit_rec.email, sizeof(unit_rec.email));
            rdm_status = add_building(&unit_rec, task1, CURR_DB);
            break;

        case '2':
            DisplayMessageNow(FALSE, FALSE, "Enter Name: ");
            getstring(unit_rec.name, sizeof(unit_rec.name));
            DisplayMessageNow(FALSE, FALSE, "Enter Unit Number: ");
            getstring(unit_rec.unitno, sizeof(unit_rec.unitno));
            DisplayMessageNow(FALSE, FALSE, "Enter Phone: ");
            getstring(unit_rec.phone, sizeof(unit_rec.phone));
            DisplayMessageNow(FALSE, FALSE, "Enter Fax: ");
            getstring(unit_rec.fax, sizeof(unit_rec.fax));
            DisplayMessageNow(FALSE, FALSE, "Enter Email: ");
            getstring(unit_rec.email, sizeof(unit_rec.email));
            rdm_status = add_building(&unit_rec, task2, CURR_DB);
            break;

        case 'q':
        case 'Q':
            /* Disconnect db1 from the slave */
            DisplayMessageNow(FALSE, FALSE, "Stopping replication for db1\n");
            memset(&db_disconnect, 0, sizeof(db_disconnect));
            db_disconnect.dburl = psp_makeString("ia_db1@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
            db_disconnect.port = dbrep_opts.slave_port;
            db_disconnect.hostname = dbrep_opts.slave_host;
            rdm_status = d_dbrep_disconnect(&db_disconnect);
            psp_freeMemory(db_disconnect.dburl, TAG0);
    
            /* Disconnect db2 from the slave */
            DisplayMessageNow(FALSE, FALSE, "Stopping replication for db2\n");
            db_disconnect.dburl = psp_makeString("ia_db2@%s:%d", dbrep_opts.master_host, dbrep_opts.master_port);
            rdm_status = d_dbrep_disconnect(&db_disconnect);    
            psp_freeMemory(db_disconnect.dburl, TAG0);

            /* Cleanup master threads */
            DisplayMessageNow(FALSE, FALSE, "Cleaning up master threads\n");
            cleanup_db(task1);
            cleanup_db(task2);
            cleanup_master(hTFS, hREP);
            quit = 1;
            break;
        }
    }

    if(dbrep_opts.master_docroot)
    {
        psp_freeMemory(dbrep_opts.master_docroot, TAG0);
    }
    if(dbrep_opts.slave_host)
    {
        psp_freeMemory(dbrep_opts.slave_host, TAG0);
    }
    if(dbrep_opts.master_host)
    {
        psp_freeMemory(dbrep_opts.master_host, TAG0);
    }

    return 0;
}

static int32_t configure_master(
    const char *docroot,
    int32_t     port,
    uint16_t    verbose,
    TFS_HANDLE *hTFS,
    REP_HANDLE *hREP)
{
    int32_t    rdm_status;
    TFS_PARAMS tfsParams;
    DBREP_INIT_PARAMS dbrepParams;

    /* Initialize the master TFS threads, note that there are other ways
       to run a TFS depending on application requirements/needs
          * external daemon running
          * start external process from the command line
    */
    memset(&tfsParams, 0, sizeof(tfsParams));
    tfsParams.port = port;
    tfsParams.rd_only = FALSE;
    tfsParams.no_disk = FALSE;
    tfsParams.verbose = verbose;
    tfsParams.logfile = NULL;
    tfsParams.stdout_file = PSP_STDOUT;
    rdm_status = d_tfsinitEx(docroot, &tfsParams, hTFS, TFS_TYPE_TFS);
    if(rdm_status == S_OKAY)
    {
        rdm_status = d_tfsrun(*hTFS, TRUE, NULL);
        if(rdm_status != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error starting the master TFS threads(%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            return rdm_status;
        }
    }
    else
    {
        DisplayErrorNow(FALSE, FALSE, "Error initializing the master TFS threads (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    /* Initialize the internal master replication server, there are other ways to
       do this depending on application requirements/needs
          * external daemon running
          * start externally from the command line
    */
    memset(&dbrepParams, 0, sizeof(dbrepParams));
    dbrepParams.docroot = docroot;
    dbrepParams.port = port;
    dbrepParams.verbose = verbose;
    dbrepParams.diskless = FALSE;
    dbrepParams.stdout_file = PSP_STDOUT;
    rdm_status = d_dbrep_init(&dbrepParams, hREP);
    if(rdm_status == S_OKAY)
    {
        rdm_status = d_dbrep_start(*hREP, TRUE, NULL);
        if(rdm_status != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error starting the master dbrep threads (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
            return rdm_status;
        }
    }
    else
    {
        DisplayErrorNow(FALSE, FALSE, "Error initializing the master dbrep threads (%s - %s)\n", d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }
    return S_OKAY;
}

static int32_t cleanup_master(
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

static int32_t init_db(
    const char  *dbname,
    const void  *dbd_array,
    int32_t      dbd_size,
    DB_TASK    **task)
{
    int32_t rdm_status;
    LOCK_REQUEST lock_pkt[] =
    {
        { UNIT,         'w' },
        { MEASUREMENT,  'w' }
    };
    int32_t lock_pkt_size = sizeof(lock_pkt) / sizeof(lock_pkt[0]);


    /* Create the RDM task */
    if((rdm_status = d_opentask(task)) == S_OKAY)
    {
        /* Open the requested database */
        if ((rdm_status = d_open_ptr (dbname, "s", dbd_array, dbd_size, *task)) != S_OKAY)
        {
            d_closetask(*task);
            return rdm_status;
        }

        /* Turn on the replication configuration flag so the TFS will generate replciation log files */
        if ((rdm_status = d_dbsetini ("configuration", "replication", "1", *task, CURR_DB)) != S_OKAY)
        {
            d_close(*task);
            d_closetask(*task);
            return rdm_status;
        }

        /* Start a transaction */
        if ((rdm_status = d_trbegin("initialize", *task)) != S_OKAY)
        {
            d_close(*task);
            d_closetask(*task);
            return rdm_status;
        }
       
        /* Lock all of the record in the database */ 
        if ((rdm_status = d_lock(lock_pkt_size, lock_pkt, *task, CURR_DB)) != S_OKAY)
        {
            d_trabort(*task);
            d_close(*task);
            d_closetask(*task);
            return rdm_status;
        }

        /* Initialize the database (create new database files - removing any existing data */
        if ((rdm_status = d_initialize(*task, CURR_DB)) != S_OKAY)
        {
            d_trabort(*task);
            d_close(*task);
            d_closetask(*task);
            return rdm_status;
        }
       
        /* End the initialization transaction */ 
        if ((rdm_status = d_trend(*task)) != S_OKAY)
        {
            d_trabort(*task);
            d_close(*task);
            d_closetask(*task);
            return rdm_status;
        }
    }
    return rdm_status;
}

static int32_t cleanup_db(
    DB_TASK *task)
{
    int32_t rdm_status = S_OKAY;

    /* close any open database and the task */
    rdm_status = d_close(task);
    rdm_status = d_closetask(task);

    return rdm_status;
}

static int32_t add_building(
    struct unit *unit_rec,
    DB_TASK     *task,
    int32_t      dbn)
{
    int32_t rdm_status;

    /* Start a transaction */
    rdm_status = d_trbegin("add_building", task);
    if(rdm_status == S_OKAY)
    {
        /* Lock the UNIT (building) record */
        rdm_status = d_reclock(UNIT, "w", task, dbn);
        if(rdm_status == S_OKAY)
        {
            /* Add the record to the database table */
            rdm_status = d_fillnew(UNIT, unit_rec, task, dbn);
        }

        /* Commit the transaction (abort on error) */
        if(rdm_status == S_OKAY)
        {
            rdm_status = d_trend(task);
        }
        else
        {
            d_trabort(task);
        }
    }
    return rdm_status;
}

static int32_t GetOptions(
    int32_t            argc, 
    const char *const *argv,
    REPL_EXAMPLE_OPTS *repl_opts)
{
    int32_t       optstat;
    const char   *opt;
    const char   *optarg;
    default_opts  defaults;

    /*lint -save -e785 */
    cmdline_opt opts[] = {
        { ARG_OPT,         "=master_docroot",      "s=path",       "Document root for the master TFS instance" },        
        { ARG_OPT,         ":master_host",         "s=hostname",   "Hostname for master (default localhost)" },
        { ARG_OPT,         ":master_port",         "i=port",       "Anchor port for the master (default 21553)" },
        { ARG_OPT,         ":slave_host",          "s=hostname",   "Hostname for the slave (default localhost)" },
        { ARG_OPT,         ":slave_port",          "i=port",       "Anchor port for the slave (default 22553)" },
        { ARG_OPT,         ":verbose",             NULL,           "Display verbose server messages" },
        { ARG_END,         NULL,                   NULL,           NULL }
    };
    /*lint -restore */

    RDM_ASSERT(repl_opts);
    memset(&defaults, 0, sizeof(default_opts));
    defaults.no_tfs = 1;
    defaults.no_stdout = 1;
    defaults.no_stderr = 1;

    repl_opts->master_port = 21553;
    repl_opts->slave_port = 22553;
    repl_opts->verbose = 0;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("Replication Master"), argc,
            argv, opts, &defaults, &repl_opts->cmd_status);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'm':
                if(strcmp(opt, "master_docroot") == 0)
                {
                    repl_opts->master_docroot = psp_strdup(optarg, TAG0);
                }
                else if(strcmp(opt, "master_port") == 0)
                {
                    repl_opts->master_port = atoi(optarg);
                }
                else if(strcmp(opt, "master_host") == 0)
                {
                    repl_opts->master_host = psp_strdup(optarg, TAG0);
                }
                break;

            case 's':
                if(strcmp(opt, "slave_port") == 0)
                {
                    repl_opts->slave_port = atoi(optarg);
                }
                else if(strcmp(opt, "slave_host") == 0)
                {
                    repl_opts->slave_host = psp_strdup(optarg, TAG0);
                }
                break;
            
            case 'v':
                repl_opts->verbose = 1;
                break;

        } /*lint !e744 */
    }

    if(repl_opts->slave_host == NULL)
    {
        repl_opts->slave_host = psp_strdup("localhost", TAG0);
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
PSP_STARTUP_EXAMPLE(replMaster)
