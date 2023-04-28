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

#include <stdio.h>

#include "rdm.h"
#include "base.h"
#include "tfs.h"
#include "mkt_dbd.h"
#include "market.h"

int32_t checking_for_db = 0; /*lint !e956 */

static void EXTERNAL_FCN MyErr(int32_t err, char *errmsg)
{ /*lint !e403 */
    if (err != S_TRNOTACT && !checking_for_db)
        printf("Database Error: %s\n", errmsg);
}

static int32_t market_exit(
    DB_TASK         *task,
    TFS_HANDLE      hTFS,
    cmdline_status *cmd_status,
    int32_t         err)
{
    if (task) {
        d_close(task);
        d_closetask(task);
    }

    d_tfsstop(hTFS);
    d_tfsterm(hTFS);

    if(cmd_status->stdout_opened)
        psp_stdout_close();
        
    if(cmd_status->stderr_opened)
        psp_stderr_close();

    return err != S_OKAY;
}


/*lint -save -e785 -e956 */
static cmdline_opt opts[] = {
    { ARG_OPT,   ":d", "s=PATH",     "PATH location of server document root (absolute, or relative to current directory" },
    { ARG_OPT,   ":i", "s=FILENAME", "FILENAME containing CSV investor data (name,funds)" },
    { ARG_OPT,   ":s", "s=FILENAME", "FILENAME containing CSV company/stock data (name,shares,price)" },
    { ARG_OPT,   ":t", "i=N",        "Trade stocks, N trades/day." },
    { ARG_OPT,   ":f", "s=Y,D",      "Yearly growth percentage, Daily variance percentage." },
    { ARG_OPT,   ":p", "i=N",        "Listen to TCP port N for connections (default is "TFS_DEFAULT_PORT_STR")" }, /*lint !e786 */
    { ARG_OPT,   ":v", NULL,         "Verbose output" },
    { ARG_END,   NULL, NULL,         NULL }
};
/*lint -restore */


/* =======================================================================
    Check the TFS library with d_tfsversion.
    If it is 'd' (direct linked), then start the server. Otherwise, this
    program is a client of another TFS.
*/
static int32_t startAsTFS(
    int32_t             argc,       /*IN:  command line argument count */
    const char *const  *argv,       /*IN:  command line arguments */
    cmdline_status     *cmd_status, /*OUT: information about cmdline args */
    TFS_HANDLE         *hTFS)       /*OUT: TFS handle */
{
    int32_t         rc;
    const char     *docroot = NULL;
    const char     *full_docroot;
    const char     *optarg;
    char            opt;
    TFS_PARAMS      tparams;
    uint16_t        tfs_done = 0;
    uint16_t        major;
    uint16_t        minor;
    TFS_LIB_TYPE    tfs_type;
    
    rc = b_cmd_initopt("Marketplace Example\n", argc, argv, opts, NULL, cmd_status);
    if (rc != B_CMD_OKAY)
        return rc;

    memset(&tparams, 0, sizeof(TFS_PARAMS));

    tparams.port             = b_getTFSDefaultPort();

    /* check for docroot */
    while ((opt=b_cmd_nextoptchr(opts, &optarg)) != '\0') {
        switch (opt) {
            case 'd':
                docroot = psp_strdup(optarg, 0);
                break;

            case 'p':
                tparams.port = (uint16_t) atoi(optarg);
                break;

            case 'v':
                tparams.verbose = 1;
                break;
        } /*lint !e744 */
    }

    /* get docroot into a standard form FULLPATH DIRCHAR */
    full_docroot = psp_fullpath(docroot);

    if (!full_docroot)
    {
        printf("Illegal document root (%s).\n", docroot ? docroot : "<none>");
        printf("Must be absolute path (Windows requires Drive) or relative to current directory.\n");
        rc = B_CMD_ERROR;
    }

    if (docroot)
        psp_freeMemory(docroot, 0);

    if (rc != S_OKAY)
        return rc;

    rc = d_tfsinitEx(full_docroot, &tparams, hTFS, cmd_status->tfs_type);    
    if(rc == S_OKAY)
    {
        /* We check using d_tfsversion so that we can determine what 
         * TFS_TYPE_DEFAULT resolves to
         */
        rc = d_tfsversion(&major, &minor, &tfs_type);    
        {            
            if (rc == S_OKAY)
            {
                /* Direct linked TFS library, start server */
                if (tfs_type == TFS_TYPE_TFS)
                {                    
                    rc = d_tfsrun(*hTFS, TRUE, &tfs_done);
                    if(rc != S_OKAY)
                    {
                        printf("Error starting TFS threads - %s (%d): %s\n", d_errorname(rc), rc, d_errorstr(rc));
                    }
                }
            }
        }
    }
    else
    {
        printf("Error initialzing TFS - %s (%d): %s\n", d_errorname(rc), rc, d_errorstr(rc));
    }


    return rc;
}

/* =======================================================================
*/
int32_t EXTERNAL_FCN marketExample_main(
    int32_t            argc,
    const char *const *argv)
{
    DB_TASK        *task;
    int32_t         status;
    char            cmd[20];
    int32_t         sCont = 1;
    const char     *optarg;
    char            opt;
    TFS_HANDLE      hTFS = NULL;
    cmdline_status  cmd_status = {0, 0, 0};


    if ((status=startAsTFS(argc, argv, &cmd_status, &hTFS)) != S_OKAY)
        return market_exit(NULL, hTFS, &cmd_status, status);
        
    d_opentask(&task);

    d_set_dberr(MyErr, task);
    srand((uint32_t) time(NULL));

    checking_for_db = 1;
    while ((status=d_open("mkt", "s", task)) != S_OKAY)
    {
        checking_for_db = 0;
        if (status != S_INVDB)  {
            fprintf(stderr, "Failed to open database, error = %d\n", status );
            if (status == S_TX_CONNECT)
                fprintf(stderr, "TFS may not be running.  Please check.\n");

            return market_exit(task, hTFS, &cmd_status, 1);
        }

        fprintf(stderr, "Database does not exist\n");
        while (!getString("Create and initialize? (y/n)", cmd, sizeof(cmd)))
            printf("Enter y or n.\n");

        if (cmd[0] == 'y' || cmd[0] == 'Y')
        {
            if ((status=d_open_ptr("mkt", "x", &mkt_dbd, sizeof(mkt_dbd), task)) != S_OKAY)
            {
                fprintf(stderr, "Exclusive open failed, error = %d\n", status );
                return market_exit(task, hTFS, &cmd_status, 1);
            }
            if ((status=d_initialize(task, CURR_DB)) != S_OKAY)
            {
                fprintf(stderr, "Initialization failed, error = %d\n", status );
                return market_exit(task, hTFS, &cmd_status, 1);
            }
            d_close(task);
        }
        else
        {
            fprintf(stderr, "Exiting\n");
            return market_exit(task, hTFS, &cmd_status, 1);
        }
    }

    status = b_cmd_initopt("", argc, argv, opts, NULL, &cmd_status);
    if (status != B_CMD_OKAY)
        return market_exit(task, hTFS, &cmd_status, status);

    /* check for batch mode operations */
    while ((opt=b_cmd_nextoptchr(opts, &optarg)) != '\0') {
        switch (opt) {
            case 'd': /* Ignore docroot, port, verbose here */
            case 'p':
            case 'v':
                continue;
            case 'i':
                batch_create_investors(optarg, task);
                break;
            case 's':
                batch_create_stocks(optarg, task);
                break;
            case 't':
                batch_trade_stocks(optarg, task);
                break;
            case 'f':
                batch_fluctuate_market(optarg, task);
                break;
        } /*lint !e744 */
        return market_exit(task, hTFS, &cmd_status, 0);
    }

    /* interactive mode */
    while (sCont)
    {
        /* display command menu */
        printf("\nMARKET Commands:\n");
        printf("   1 - Create Company/Stock\n");
        printf("   2 - Create Investor\n");
        printf("   3 - Capitalize Stock\n");
        printf("   4 - Fund Investor\n");
        printf("   5 - Buy Stock\n");
        printf("   6 - Sell Stock\n");
        printf("   7 - Report on Investor\n");
        printf("   8 - Report on Stock\n");
        printf("   9 - Market Fluctuation\n");
        printf("   i - Initialize Database\n");
        printf("   x - Check Transaction Rate\n");
        printf("   q - Quit\n");
        while (!getString("Enter command: ", cmd, sizeof(cmd)))
            printf("Enter 'q' to quit.\n");

        switch (cmd[0])
        {
            case 'q':
            case 'Q':
                sCont = 0;
                break;

            case 'i':
            case 'I':
                printf("This operation will delete all existing contents!\n");
                while (!getString("Are you sure you want to initialize? (y/n) ", cmd, sizeof(cmd)))
                    printf("Enter y or n.\n");
                if (cmd[0] != 'y' && cmd[0] != 'Y')
                    break;
                d_close(task);
                if ((status=d_open_ptr("mkt", "x", &mkt_dbd, sizeof(mkt_dbd), task)) != S_OKAY)
                {
                    fprintf(stderr, "Exclusive open failed, error = %d\n", status );
                    return market_exit(task, hTFS, &cmd_status, 1);
                }
                if ((status=d_initialize(task, CURR_DB)) != S_OKAY)
                {
                    fprintf(stderr, "Initialization failed, error = %d\n", status );
                    return market_exit(task, hTFS, &cmd_status, 1);
                }
                d_close(task);
                if ((status=d_open_ptr("mkt", "s", &mkt_dbd, sizeof(mkt_dbd), task)) != S_OKAY)
                {
                    fprintf(stderr, "Initialization failed, error = %d\n", status );
                    return market_exit(task, hTFS, &cmd_status, 1);
                }
                break;

            case 'x':
            case 'X':
                /* check the transaction count/rate */
                if (check_tx_rate(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '1':
                /* Create company/stock */
                if (create_company(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '2':
                /* Create Investor */
                if (create_investor(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '3':
                /* Capitalize Stock */
                if (capitalize_stock(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '4':
                /* Fund Investor */
                if (fund_investor(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '5':
                /* Buy Stock */
                if (buy_stock(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '6':
                /* Sell Stock */
                if (sell_stock(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '7':
                /* Report on Investor */
                if (investor_report(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '8':
                /* Report on Stock */
                if (company_report(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            case '9':
                /* Market Fluctuation */
                if (fluctuate_market(task))
                    return market_exit(task, hTFS, &cmd_status, 1);
                break;
            default:
                printf("\n*** incorrect command -- re-enter\n");
                break;
        }
    }

    return market_exit(task, hTFS, &cmd_status, 0);
}

PSP_STARTUP_EXAMPLE(marketExample)
