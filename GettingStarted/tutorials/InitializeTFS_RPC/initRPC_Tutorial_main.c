/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2012 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */
#include "rdm.h"
#include "base.h"
#include "initRPC_db.h"
#include "initRPC_db_dbd.h"

typedef struct _tfs_init_ctx
{
    const char *tfs_host;
    DB_TASK    *task;
    TFS_HANDLE  hTFS;
} TFS_INIT_CTX;

static int32_t GetOptions(int32_t, const char *const *, TFS_INIT_CTX*);
static int32_t initialize(const char *, TFS_INIT_CTX *ctx);
static void cleanup(TFS_INIT_CTX *ctx);

/* Main function to demonstrate initializing a TFS_TYPE_RPC tfs */
int32_t EXTERNAL_FCN initRPC_Tutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t      rdm_status = S_OKAY;
    TFS_INIT_CTX ctx;

    /* Initialize the application context */
    memset(&ctx, 0, sizeof(ctx));

    /* Process command line parameters */
    if(GetOptions(argc, argv, &ctx) != B_CMD_OKAY)
    {
        return 1;
    }

    /* Initialize the RDM environment */ 
    rdm_status = initialize("s", &ctx);

    /* Here the application will perform database related operations */
    
   
    /* Now cleanup the RDM environment */ 
    cleanup(&ctx);

    return rdm_status == S_OKAY ? 0 : 1;
}


/* This function will initialize a remote TFS, open a task, and open the 
 *   tutorial database
 */
static int32_t initialize(
    const char   *openmode, 
    TFS_INIT_CTX *ctx)
{
    int32_t     rdm_status = S_OKAY;
    const char *dbname;

    /* Create the dbname with the tfs_host (if one was provided) */
    if(ctx->tfs_host)
    {
        dbname = psp_makeString("initRPC_db@%s", ctx->tfs_host);
    }
    else
    {
        dbname = psp_makeString("initRPC_db");
    }

    /* Validate we were able to successfully allocate the dbname */
    if(!dbname)
    {
        return S_NOMEMORY;
    }

    /* Initialize the TFS to use the RPC implementation 
     *    When using the RPC implementation there is no need to specify a
     *    DOCROOT or TFS_PARAMS parameters*/
    rdm_status= d_tfsinitEx (NULL, NULL, &ctx->hTFS, TFS_TYPE_RPC);
    if (rdm_status != S_OKAY) 
    {
        DisplayErrorNow(FALSE, FALSE, "Error initializing TFS (%s - %s)\n", 
                d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }
    DisplayMessageNow(FALSE, FALSE, "Successfully initialized the TFS\n");
   
    /* Create a runtime task for the application to use */ 
    rdm_status = d_opentask (&ctx->task);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error opening task (%s - %s)\n", 
                d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }
    DisplayMessageNow(FALSE, FALSE, "Successfully created the RDM runtime task\n");
   
    /* Open the application database using an embedded dbd */ 
    rdm_status = d_open_ptr(dbname, openmode, &initRPC_db_dbd, sizeof(initRPC_db_dbd), ctx->task);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error opening database %s (%s - %s)\n", 
                dbname, d_errorname(rdm_status), d_errorstr(rdm_status));

        if(rdm_status == S_TX_NO_REMOTE)
        {
            DisplayMessageNow(FALSE, FALSE, "Only certain packages allow for connection to a remote TFS\n");
        }
        
        psp_freeMemory(dbname, TAG0);
        return rdm_status;
    }
    DisplayMessageNow(FALSE, FALSE, "Successfully opened the database (%s)\n", dbname);
    psp_freeMemory(dbname, TAG0);
        
    return rdm_status;
}

/* This function will cleanup the TFS handle and runtime task */
static void cleanup(
    TFS_INIT_CTX *ctx)
{
    /* If we have created a task we need to clean it up */
    if(ctx->task)
    {
        d_close(ctx->task);
        d_closetask(ctx->task);
        ctx->task = NULL;
    }

    /* terminate the TFS */
    d_tfsterm(ctx->hTFS);
    ctx->hTFS = NULL;
}

static int32_t GetOptions(
    int32_t            argc, 
    const char *const *argv,
    TFS_INIT_CTX      *ctx)
{
    int32_t      optstat;
    const char  *opt;
    const char  *optarg;
    default_opts dflt_opts;

    /*lint -save -e785 */
    cmdline_opt opts[] = {
        { ARG_OPT,         ":a",   "s=tfs",       "Servername of the TFS to connect to (default localhost)" },
        { ARG_END,         NULL,   NULL,          NULL }
    };
    /*lint -restore */

    RDM_ASSERT(ctx);

    memset(&dflt_opts, 0, sizeof(dflt_opts));
    dflt_opts.no_quiet = dflt_opts.no_stdout = dflt_opts.no_stderr = dflt_opts.no_tfs = 1;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("TFS_TYPE_RPC Initialization"), 
            argc, argv, opts, &dflt_opts, NULL);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'a':
                ctx->tfs_host = optarg;
                break;
        } /*lint !e744 */
    }
    return B_CMD_OKAY;
}

PSP_STARTUP_EXAMPLE(initRPC_Tutorial)
