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

/*-----------------------------------------------------------------------

    Parse command line options

    timserver [-d PATH] [-p N] [-v]

        where   -d   PATH location of server document root
                -p   listen to tcp port N for connections (default is TFS_DEFAULT_PORT)
                -m   enable mirroring
                -v   verbose output

-----------------------------------------------------------------------*/

/*
    This example demonstrates how to navigate a network-model database (tims),
    using the d_recfrst, d_recnext and d_recprev function calls.

    This example also shows how to create an HTTP server that can build web
    pages to display your database.

    See also (in httpdocs\tims):
        display.js
        index.html
*/

#include "rdm_version.h"
#include "base.h"
#include "psp.h"
#include "timsnav.h"
#include "tfs.h"
#include "mirutils.h"

#include <assert.h> /*lint !e451 */
#include <signal.h>

static uint16_t sigRec = 0; /*lint !e956 */

static void sighandler(int);

/* Function map of SCRIPT_NAME portion of URL.
    Ex: http://www.mysite.com/navigate.rdm causes navigate_handler to run.

   The functions in the function map will be called when a URL contains
   the 'name' string after the domain name. The handler function should
   parse the URL arguments and generate the appropriate HTML output.
   This example uses AJAX. This means that only portions of a web page
   are generated on each HTTP request. HTML layers are used to define those
   portion locations.  The layers are defined in rdm.css and HTML is generated
   in rdm.js and display.js.
*/
static const TFS_XAPI_FCNMAP timsfcnmap[] = {
    /* name                     function                 */
    { "/navigate.rdm",    (TFS_XAPI_FCN *)navigate_handler  },
};

#define MAX_TFSFCNS     RLEN(timsfcnmap)

static int32_t GetOptions(int32_t, const char *const *, cmdline_status *, uint16_t *, TFS_PARAMS *, char **);

int32_t EXTERNAL_FCN timserver_main(int32_t, const char *const *);


/* =======================================================================
    RDM Mini-HTTP Server main
*/
int32_t EXTERNAL_FCN timserver_main(
    int32_t            argc,
    const char *const *argv)
{
    int32_t         rc;
    TFS_XAPI_PARAMS xapi_params;
    TFS_HANDLE      hTFS = NULL;
    REP_HANDLE      hREP = NULL;
    char           *docroot;
    uint16_t        major;
    uint16_t        minor;
    TFS_LIB_TYPE    type;
    TFS_PARAMS      tparams;
    DBREP_INIT_PARAMS    rparams;
    uint16_t        mirroring = 0;
    uint16_t        tfs_done = 0;
    uint16_t        rep_done = 0;
    RHS_PARAMS      rhsParams;
    RHS_HANDLE      hRHS = NULL;
    cmdline_status  cmd_status = {0, 0, 0};

    signal(SIGINT,  sighandler); /*lint !e534 */
    signal(SIGTERM, sighandler); /*lint !e534 */

    /* Initialize TFS_PARAMS to 0 to use defaults */
    memset(&tparams, 0, sizeof(TFS_PARAMS));
    memset(&rparams, 0, sizeof(DBREP_INIT_PARAMS));

    /* Setup our xapi_params functions. Setup subdocroot, where the Javascript
       files are located (path under docroot/httpdocs).
     */
    xapi_params.xapifcnmap      = timsfcnmap;
    xapi_params.fcnmapsize      = MAX_TFSFCNS;
    xapi_params.subdocroot      = "/timserver";

    tparams.stdout_file       = PSP_STDOUT;

    if (GetOptions(argc, argv, &cmd_status, &mirroring, &tparams, &docroot) != B_CMD_OKAY)
    {
        if(cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    rparams.docroot = docroot;
    rparams.port = b_getMirDefaultPort();
    rparams.verbose = tparams.verbose;

    /* Make sure we have the right version of the TFS library */
    d_tfsversion(&major, &minor, &type);
    if (type == TFS_LIB_TYPE_TFS)
    {
        /* Initialize the TFS */
        rc = d_tfsinitEx(docroot, &tparams, &hTFS, TFS_TYPE_TFS);
        
        if (rc == S_OKAY)
        {
            d_tfsxapiparams(hTFS, &xapi_params);
            
            /* Run as a server. Server code processes requests in thread */
            d_tfsrun(hTFS, TRUE, &tfs_done);
        }
        else
        {
            printf("Error initializing tfs (%d)\n", rc);
            if(cmd_status.stdout_opened)
                psp_stdout_close();
        
            if(cmd_status.stderr_opened)
                psp_stderr_close();

            return 1;
        }
    }
    else if (type == TFS_LIB_TYPE_RPC)
    {
        /* Initialize the TFS */
        memset(&rhsParams, 0, sizeof(RHS_PARAMS));
        rhsParams.docroot = docroot;
        rhsParams.port = tparams.port;
        rhsParams.xapi_params = xapi_params;
        rhsParams.verbose = tparams.verbose;
        rhsParams.rd_only = tparams.rd_only;
        rhsParams.diskless = tparams.no_disk;
        
        
        rc = http_InitServer(&rhsParams, NULL, &hRHS);
        if (rc == S_OKAY)
        {
            /* Run as a server. Server code process requests in thread */
            if ((rc=http_StartServer(hRHS, TRUE, &tfs_done)) != S_OKAY)
            {
                printf("Unable to start server (%d)\n", rc);
                if(cmd_status.stdout_opened)
                    psp_stdout_close();
        
                if(cmd_status.stderr_opened)
                    psp_stderr_close();

                return 1;
            }
        }
        else
        {
            printf("Error initializing server (%d)\n", rc);
            if(cmd_status.stdout_opened)
                psp_stdout_close();
        
            if(cmd_status.stderr_opened)
                psp_stderr_close();

            return 1;
        }
    }
    else
    {
        printf("This example is not compatible with this type of tfs (%d)\n", type);
        if(cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    if(mirroring)
    {
        /* Initialize the REP */
        rc = d_dbmir_init(&rparams, &hREP);
        if (rc == S_OKAY)
        {
            /* Run as a server. */
            d_dbrep_start(hREP, TRUE, &rep_done);
        }
    }

    /* Waits while server code processes requests */
    while (!sigRec && !tfs_done && !rep_done)
        psp_sleep(200);

    if(mirroring && hREP)
    {
        d_dbrep_stop(hREP);
        d_dbrep_term(hREP);
    }

    if (type == TFS_LIB_TYPE_RPC)
    {
        if (http_StopServer(hRHS) == S_OKAY)
            http_CloseServer(hRHS);
    }
    else if (type == TFS_LIB_TYPE_TFS)
    {
        d_tfsstop(hTFS);
        d_tfsterm(hTFS);
    }

    if(cmd_status.stdout_opened)
        psp_stdout_close();
        
    if(cmd_status.stderr_opened)
        psp_stderr_close();

    return rc;
}

/* =======================================================================
*/
/*lint -save -e785 -e956 */
static cmdline_opt opts[] = {
    { ARG_OPT,   ":d", "s=PATH", "PATH location of server document root (absolute, or relative to current directory)" },
    { ARG_OPT,   ":p", "i=N",    "Listen to TCP port N for connections (default is "TFS_DEFAULT_PORT_STR")" },/*lint !e786 */
    { ARG_OPT,   ":v", NULL,     "Verbose output" },
    { ARG_OPT,   ":m", NULL,     "Enable Mirroring" },
    { ARG_END,   NULL, NULL,     NULL }
}; /*lint -restore */

/* =======================================================================
*/
static int32_t GetOptions(
    int32_t            argc,
    const char *const *argv,
    cmdline_status    *cmd_status,
    uint16_t          *mirroring,
    TFS_PARAMS        *params,
    char             **full_docroot)
{
    int32_t     optstat;
    char        opt;
    const char *p = NULL;
    const char *docroot = NULL;
    const char *optarg;

    RDM_ASSERT(params && full_docroot);

    optstat = b_cmd_initopt(RDM_SERVER_DESC("Tims DB Navigation Example"), argc,
            argv, opts, NULL, cmd_status);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptchr(opts, &optarg)) != '\0')
    {
        switch (opt)
        {
            case 'd':
                docroot = psp_strdup(optarg, 0);
                break;

            case 'm':
                *mirroring = 1;
                break;

            case 'p':
                params->port = (uint16_t) atoi(optarg);
                break;

            case 'v':
                params->verbose = 1;
                break;
        } /*lint !e744 */
    }

    if (docroot == NULL && (p = psp_getenv("RDM_DOCROOT")) != NULL)
        docroot = psp_strdup(p, 0);

    if (!params->port)
        params->port = b_getTFSDefaultPort();

    /* get docroot into a standard form FULLPATH DIRCHAR */
    *full_docroot = psp_fullpath(docroot);

    if (!*full_docroot)
    {
        printf("Illegal document root (%s).\n", docroot ? docroot : "<none>");
        printf("Must be absolute path (Windows requires Drive) or relative to current directory.\n");
        optstat = -2;
    }

    if (docroot)
        psp_freeMemory(docroot, 0);

    return optstat;
}

/* =======================================================================
    The third try of Ctl+C (or the equivalent) will not be handled here
    and the program will be killed by the OS.
*/
static void sighandler(
    int sig)
{
    sigRec++;
    
    /* give our code a chance to shutdown cleanly */
    if (sigRec < 2)
        signal(sig, sighandler); /*lint !e534 */
}

PSP_STARTUP_EXAMPLE(timserver)
