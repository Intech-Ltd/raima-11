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

/* ----------------------------------------------------------------------------
 *    Display Users
 *    ----------
 *    This tutorial demonstrates the process to display a list of users 
 *    connected to a TFS instance
 *
 * ----------------------------------------------------------------------------
 */
#include "rdm.h"
#include "displayUsers_db_dbd.h"

/* The tutorial context passed to different functions */
typedef struct _DISP_USER_CTX
{
    const char     *docroot;
    const char     *host;
    uint16_t        port;
    cmdline_status  cmd_status;
    TFS_HANDLE      hTFS;
    DB_TASK        *task;
} DISP_USER_CTX;

/* Function Prototypes */
static int32_t EXTERNAL_FCN DisplayUser(const char *userName);
static int32_t Initialize(DISP_USER_CTX *);
static void Cleanup(DISP_USER_CTX *);
static int32_t GetOptions(int32_t, const char *const *, DISP_USER_CTX *);

/* The main tutorial function that demonstrates to process to get a list of
 * users active connected to a TFS
 */
static int32_t EXTERNAL_FCN displayUsersTutorial_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t        rdm_status;
    TFS_HANDLE     hTFS = 0;
    DISP_USER_CTX  ctx;

    memset(&ctx, 0, sizeof(DISP_USER_CTX));

    /* Process the command line options */
    if (GetOptions(argc, argv, &ctx) != B_CMD_OKAY)
    {
        if(ctx.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(ctx.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    /* The STANDALONE TFS does not have any users, display a message and exit */
    if(ctx.cmd_status.tfs_type == TFS_TYPE_STANDALONE)
    {
        DisplayMessageNow(FALSE, FALSE, "The STANDALONE TFS does not support users\n\n");
        return 0;
    }

    /* Initialize the TFS, task, and open our database */
    rdm_status = Initialize(&ctx);
    if(rdm_status == S_OKAY)
    {
        /* Display the list header */
        DisplayMessage(FALSE, FALSE, "\nUsers Connected to the TFS\n");
        DisplayMessage(FALSE, FALSE, "--------------------------\n");
    
        /* Get the list of users currently connect to the TFS */
        rdm_status = d_tfsIterateConnectedUsers(hTFS, ctx.host, ctx.port, 
                DisplayUser);
        if(rdm_status != S_OKAY)
        {
            DisplayErrorNow(FALSE, FALSE, "Error %s - %s\n" , 
                    d_errorname(rdm_status), d_errorstr(rdm_status));
        }
        DisplayMessage(FALSE, FALSE, "\n");
    }

    /* Cleanup */
    Cleanup(&ctx);

    return (rdm_status == S_OKAY ? 0 : 1);
}

/* Call back function that prints the current user name to stdout 
 *   This function will get called on time for each user that is connected
 *   to the TFS
 */
static int32_t EXTERNAL_FCN DisplayUser(
    const char *userName)
{
    DisplayMessage(FALSE, FALSE, "%s\n", userName);
    return 0;
}

static int32_t Initialize(
    DISP_USER_CTX *ctx)
{
    int32_t rdm_status = S_OKAY;
    const char *dbname = NULL;

    /* Initialize the TFS library - 
     *   When using the TFST (TFS) we will only see two users (the user we 
     *   added and the users that is created to view the user list).
     *
     *   When using the TFSR (RPC) it is possible there may be other users 
     *   listed.
     *
     *   The TFSS (STANDALONE) does not support users
     */
    rdm_status = d_tfsinitEx(ctx->docroot, NULL, &ctx->hTFS, 
            ctx->cmd_status.tfs_type);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error initializing the TFS (%s %s)\n", 
                d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    /* Create a RDM task */
    rdm_status = d_opentask(&ctx->task);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error creating RDM task (%s - %s)\n" , 
                d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    /* Set a known userid - this is not necessary as the RDM runtime will
     *   automatically generate one for us, but this way we can validate
     *   that the name we use is in the list displayed
     */
    rdm_status = d_dbuserid("displayTFS_UsersTutorial", ctx->task);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error setting dbuserid for the task (%s - %s)\n" , d_errorname(rdm_status), d_errorstr(rdm_status));
        return rdm_status;
    }

    /* Create the database name (may need host & port) */
    if(ctx->host && ctx->cmd_status.tfs_type == TFS_TYPE_RPC)
    {
        dbname = psp_makeString("displayUsers_db@%s:%d", ctx->host, ctx->port);
    }
    else
    {
        dbname = psp_makeString("displayUsers_db");
    }

    if(dbname == NULL)
    {
        DisplayErrorNow(FALSE, FALSE, "Error generating database name (%s - %s)\n", d_errorname(S_NOMEMORY), d_errorstr(S_NOMEMORY));
        return S_NOMEMORY;
    }

    /* Open the database - which will add a user to the TFS */
    rdm_status = d_open_ptr(dbname, "s", &displayUsers_db_dbd, 
            sizeof(displayUsers_db_dbd), ctx->task);
    if(rdm_status != S_OKAY)
    {
        DisplayErrorNow(FALSE, FALSE, "Error opening %s database (%s - %s)\n", dbname, d_errorname(rdm_status), d_errorstr(rdm_status));
        psp_freeMemory(dbname, TAG0);
        return rdm_status;
    }
    psp_freeMemory(dbname, TAG0);

    return rdm_status;
}

static void Cleanup(
    DISP_USER_CTX *ctx)
{
    if(ctx->task)
    {
        d_close(ctx->task);
        d_closetask(ctx->task);
    }

    d_tfsterm(ctx->hTFS);
}


/*********************************************************************
* GetOptions() : Process command line arguments
**********************************************************************/
static int32_t GetOptions(
    int32_t            argc, 
    const char *const *argv,
    DISP_USER_CTX     *ctx)
{
    int32_t      optstat;
    const char  *opt;
    const char  *optarg;

    /*lint -save -e785 */
    cmdline_opt opts[] = {
        { ARG_OPT, ":docroot", "s=path", "Document root for the TFS instance" },
        { ARG_OPT, ":a",       "s=host", "Hostname for the TFS (default is " RDM_LOCALHOST ")" },
        { ARG_OPT, ":port",    "i=port", "Port for the TFS (default is " TFS_DEFAULT_PORT_STR ")" },
        { ARG_END, NULL,       NULL,     NULL }
    };
    /*lint -restore */

    RDM_ASSERT(ctx);

    ctx->port = TFS_DEFAULT_PORT;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("Display TFS Users Tutorial"), 
            argc, argv, opts, NULL, &ctx->cmd_status);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'd':
                if(strcmp(opt, "docroot") == 0)
                {
                    if(ctx->cmd_status.tfs_type == TFS_TYPE_RPC)
                    {
                        DisplayMessageNow(FALSE, FALSE, "Warning: The -docroot parameter is ignored when using a remote TFS\n");
                    }
                    else
                    {
                        ctx->docroot = optarg;
                    }
                }
                break;

            case 'a':
                if(ctx->cmd_status.tfs_type != TFS_TYPE_RPC)
                {
                    DisplayMessageNow(FALSE, FALSE, "Warning: The -host parameter is ignored when not using a remote TFS\n");
                }
                else
                {
                    ctx->host = optarg;
                }

            case 'p':
                if(strcmp(opt, "port") == 0)
                {
                    if(ctx->cmd_status.tfs_type != TFS_TYPE_RPC)
                    {
                        DisplayMessageNow(FALSE, FALSE, "Warning: The -port parameter is ignored when not using a remote TFS\n");
                    }
                    else
                    {
                        ctx->port = atoi(optarg);
                        if(ctx->host == NULL)
                        {
                            ctx->host = RDM_LOCALHOST;
                        }
                    }
                }
                break;


        } /*lint !e744 */
    }
    return B_CMD_OKAY;
}

PSP_STARTUP_EXAMPLE(displayUsersTutorial)
