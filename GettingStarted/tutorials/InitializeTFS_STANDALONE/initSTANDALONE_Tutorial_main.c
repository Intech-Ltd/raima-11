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
#include "initSTANDALONE_db.h"
#include "initSTANDALONE_db_dbd.h"

/* The context for the tutorial application */
typedef struct _tfs_init_ctx
{
    uint32_t    rd_only;
    uint32_t    no_disk;
    uint32_t    verbose;
    const char *logfile;
    const char *docroot;
    DB_TASK    *task;
    TFS_HANDLE  hTFS;
} TFS_INIT_CTX;

static int32_t GetOptions(int32_t, const char *const *, TFS_INIT_CTX*);
static int32_t initialize(const char *, TFS_INIT_CTX *ctx);
static void cleanup(TFS_INIT_CTX *ctx);

int32_t EXTERNAL_FCN initSTANDALONE_Tutorial_main(
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


/* This function will initialize a standalone TFS, open a task, and open the 
 *   tutorial database
 */
static int32_t initialize(
    const char   *openmode, 
    TFS_INIT_CTX *ctx)
{
    int32_t     rdm_status = S_OKAY;
    const char *dbname = "initSTANDALONE_db";
    TFS_PARAMS  params;

    memset(&params, 0, sizeof(params));

    params.rd_only = ctx->rd_only;
    params.no_disk = ctx->no_disk;
    params.verbose = ctx->verbose;
    params.logfile = ctx->logfile;


    /* Initialize the TFS to use the STANDALONE implementation 
     *    When using the TFS implementation you can set the DOCROOT 
     *    and modify functionality by using TFS_PARAMS parameters
     **/
    rdm_status= d_tfsinitEx (ctx->docroot, &params, &ctx->hTFS, TFS_TYPE_TFS);
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
    rdm_status = d_open_ptr(dbname, openmode, &initSTANDALONE_db_dbd, sizeof(initSTANDALONE_db_dbd), ctx->task);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error opening database %s (%s - %s)\n", 
                dbname , d_errorname(rdm_status), d_errorstr(rdm_status));
        
        return rdm_status;
    }
    DisplayMessageNow(FALSE, FALSE, "Successfully opened the database (%s)\n", dbname);
        
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
        { ARG_OPT, ":docroot",   "s=PATH",     "PATH location of server document root (absolute, or relative to current directory)" },
        { ARG_OPT, ":logfile",   "s=FILE",     "File to write TFS log messages to" },
        { ARG_OPT, ":nodisk",    NULL,         "Don't use any disk I/O" },
        { ARG_OPT, ":rdonly",    NULL,         "Use read-only device" },
        { ARG_OPT, ":v",         NULL,         "Verbose output" },
        { ARG_END, NULL,         NULL,          NULL }
    };
    /*lint -restore */

    RDM_ASSERT(ctx);

    memset(&dflt_opts, 0, sizeof(dflt_opts));
    dflt_opts.no_quiet = dflt_opts.no_stdout = dflt_opts.no_stderr = dflt_opts.no_tfs = 1;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("TFS_TYPE_TFS Initialization"), argc,
            argv, opts, &dflt_opts, NULL);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'd':
                if(stricmp(opt, "docroot") == 0)
                {
                    ctx->docroot = optarg;
                }
                break;

            case 'l':
                if(stricmp(opt, "logfile") == 0)
                {
                    ctx->logfile = optarg;
                }

            case 'n':
                if(stricmp(opt, "nodisk") == 0)
                {
                    ctx->no_disk = 1;
                }
                break;

            case 'r':
                if(stricmp(opt, "readonly") == 0)
                {
                    ctx->rd_only = 1;
                }
                break;

            case 'v':
                ctx->verbose = 1;
                break;
        } /*lint !e744 */
    }
    return B_CMD_OKAY;
}

PSP_STARTUP_EXAMPLE(initSTANDALONE_Tutorial)
