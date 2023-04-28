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

#include <iostream>
#include "rdm.h"
#include "Cbom.h"

using namespace std;

/*****************************************
* Structure Definitions
******************************************/
typedef struct _BOM_OPTS
{
    int32_t        max_levels;
    int32_t        max_members;
    cmdline_status cstat;
    const char    *docroot;
}  BOM_OPTS;

static int32_t GetOptions(int32_t, const char *const *, BOM_OPTS*);

/************************************************
* rbom_main() : Main function of lbomExample
************************************************/
int32_t EXTERNAL_FCN cppbom_main(
    int32_t            argc, 
    const char *const *argv)
{
    BOM_OPTS     bom_opts;
    char         response[20];
    int32_t      status;
    TFS_HANDLE   hTFS;
    Cbom        *bom;
    double       rolled_up_cost = 0.0;

    /* Process the command line options */
    memset(&bom_opts, 0, sizeof(bom_opts));

    status = GetOptions(argc, argv, &bom_opts);
    if(status != B_CMD_OKAY)
        return 1;

    /* Initialize the TFS library (required for TFST and TFSS) */
    status = d_tfsinitEx(bom_opts.docroot ? bom_opts.docroot : "", NULL, &hTFS, bom_opts.cstat.tfs_type);
    if(status != S_OKAY)
    {
        cerr << "Unable to inialize TFS" << endl;
        return 1;
    }

    while (bom_opts.max_levels == 0) 
    {
        cout << endl << "Enter number of levels: ";
        cout.flush();
        cin.getline(response, sizeof(response));
        bom_opts.max_levels = atoi(response);
    }

    while (bom_opts.max_members == 0) 
    {
        cout << endl << "Enter number of members: ";
        cout.flush();
        cin.getline(response, sizeof(response));
        bom_opts.max_members = atoi(response);
    }

    /* Create the bom object */
    bom = new Cbom(bom_opts.max_levels, bom_opts.max_members);
    if(!bom)
    {
        cerr << "Unable to create BOM object" << endl;
        d_tfsterm(hTFS);
        return 1;
    }

    cout << "Create BOM with " << bom_opts.max_levels << " levels and " << bom_opts.max_members << " members per level" << endl;

    /* Open the BOM database */
    try
    {
        bom->open_database();
    }
    catch (rdm_exception& e)
    {        
        cerr << "Error " << e.error() << " (" << e.what() << ") opening BOM database" << endl;
        delete bom;
        d_tfsterm(hTFS);
        return 1;
    }

    /* Create the Bill of Materials entries */
    cout << "   Building bill of materials" << endl;
    bom->create_bom();
    cout << "      millisecs: " << bom->get_elapsed_time() << endl;
    
    /* Roll up the BOM cost */
    cout << "   rolling up cost" << endl;
    rolled_up_cost = bom->rollup_bom();
    cout << "      total rolled up cost = " << rolled_up_cost << endl;
    cout << "      millisecs: " << bom->get_elapsed_time() << endl;

    delete bom;
    d_tfsterm(hTFS);
    return (0);
}

static int32_t GetOptions(
    int32_t            argc, 
    const char *const *argv,
    BOM_OPTS          *bom_opts)
{
    int32_t         optstat;
    const char     *opt;
    const char     *optarg;

    /*lint -save -e785 */
    cmdline_opt opts[] = {
        { ARG_OPT,         ":docroot",             "s=path",       "Document root for the TFS instance" },
        { ARG_OPT,         ":l",                   "i=#",          "Number of levels (default 5)" },
        { ARG_OPT,         ":m",                   "i=#",          "Number of members per level (default 5)" },
        { ARG_END,         NULL,                   NULL,           NULL }
    };
    /*lint -restore */

    RDM_ASSERT(bom_opts);

    bom_opts->max_levels = 0;
    bom_opts->max_members = 0;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("CPP Network Model BOM Benchmark"),
            argc, argv, opts, NULL, &bom_opts->cstat);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'd':
                if(strcmp(opt, "docroot") == 0)
                {
                    if(bom_opts->cstat.tfs_type == TFS_TYPE_RPC)
                    {
                        DisplayMessage(FALSE, FALSE, "Warning: The -docroot parameter is ignored when using a remote TFS\n");
                    }
                    else
                    {
                        bom_opts->docroot = optarg;
                    }
                }
                break;
            
            case 'l':
                bom_opts->max_levels = atoi(optarg);
                break;
            
            case 'm':
                bom_opts->max_members = atoi(optarg);
                break;

        } /*lint !e744 */
    }
    return B_CMD_OKAY;
}
PSP_STARTUP_EXAMPLE_CR(cppbom)
