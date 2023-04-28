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

#include "rdm.h"

/*lint ++flb -e956 */
#include "bom_dbd.h"
#include "bom.h"
/*lint --flb */

/*****************************************
* Structure Definitions
******************************************/
typedef struct _BOM_OPTS
{
    int32_t        max_levels;
    int32_t        max_members;
    const char    *docroot;
}  BOM_OPTS;

typedef struct _BOM_TASK
{
    int32_t         current_level;
    int32_t         max_level;
    int32_t         max_members;
    int32_t         txCounter;
    int32_t         count;
    TFS_HANDLE      hTFS;
    DB_TASK        *task;
    cmdline_status  cmd_status;
} BOM_TASK;

/****************************************
* Function Prototypes
****************************************/
static int32_t GetOptions(int32_t, const char *const *, BOM_OPTS *, BOM_TASK *);
static int32_t   create_bom(BOM_TASK *);
static int32_t   build_bill(BOM_TASK *);
static int32_t rollup_bom(double *, BOM_TASK *);
static int32_t get_cost(double *, BOM_TASK *);
static void   bom_random_id(BOM_TASK *, char *);
static int32_t bom_initialize(BOM_OPTS *bom_opts, BOM_TASK *);
static int32_t bom_cleanup(BOM_TASK *);
static void EXTERNAL_FCN MyErr( int32_t err, char *errmsg );

/************************************************
* bom_main() : Main function of bomExample
************************************************/
int32_t EXTERNAL_FCN bom_main(
    int32_t            argc, 
    const char *const *argv)
{
    BOM_OPTS        bom_opts;
    BOM_TASK        bom_task;
    int32_t         status;
    double          rolled_up_cost;
    char            response[20];
    uint32_t        start_time;
    uint32_t        end_time;

    memset(&bom_opts, 0, sizeof(bom_opts));
    memset(&bom_task, 0, sizeof(bom_task));

    /* Process the command line options */
    if (GetOptions(argc, argv, &bom_opts, &bom_task) != B_CMD_OKAY)
    {
        if(bom_task.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(bom_task.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    bom_task.max_level = bom_opts.max_levels;
    while (bom_task.max_level == 0) {
        printf("\nEnter number of levels: ");
        fflush (stdout);
        fgets(response, sizeof(response), stdin);
        bom_task.max_level = atoi(response);
    }
    
    bom_task.max_members = bom_opts.max_members;
    while (bom_task.max_members == 0) {
        printf("\nEnter number of members per level: ");
        fflush (stdout);
        fgets(response, sizeof(response), stdin);
        bom_task.max_members = atoi(response);
    }
    bom_task.current_level = 0;

    status = bom_initialize(&bom_opts, &bom_task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error initializing application\n");
        if(bom_task.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(bom_task.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    printf("Create BOM with %d levels and %d members per level\n", bom_task.max_level, bom_task.max_members);
    printf("   Building bill of materials\n");
    
    start_time = psp_timeMilliSecs();
    create_bom(&bom_task);
    end_time = psp_timeMilliSecs();
    if(status == S_OKAY)
    {
        printf("      millisecs: %u\n", end_time - start_time);
    }
    else
    {
        fprintf(stderr, "Error creating BOM\n");
        bom_cleanup(&bom_task);
        return 1;
    }

    printf("   rolling up cost\n");

    start_time = psp_timeMilliSecs();
    /* recursive call to calculate cost of whole component tree */
    status = rollup_bom(&rolled_up_cost, &bom_task);
    end_time = psp_timeMilliSecs();
    if(status == S_OKAY)
    {
        printf("      total rolled up cost = %10.2f\n", rolled_up_cost);
        printf("      millisecs: %u\n", end_time - start_time);
    }
    else
    {
        fprintf(stderr, "Error rolling up BOM\n");
        bom_cleanup(&bom_task);
        return 1;
    }

    /* close the database and task */
    bom_cleanup(&bom_task);
    return (0);
}

/***************************************************************************
* create_bom() : Top level function for creating the BOM
*****************************************************************************/
static int32_t create_bom(
    BOM_TASK *bom_task) 
{
    int32_t      status;
    struct  item item_rec;
    char         id_code[SIZEOF_ITEM_ID_CODE];

    item_rec.id_code[0] = '\0';
    item_rec.cost = 1.0L;

    /* create top level ITEM record */
    bom_random_id(bom_task, id_code);
    strcpy((char *)item_rec.id_code,  (char *)id_code);

    bom_task->txCounter = 0;
    status = d_trbegin("bom", bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: starting transaction\n", status);
        return status;
    }

    if (d_fillnew(ITEM, &item_rec, bom_task->task, CURR_DB) != S_OKAY) 
    {
        fprintf(stderr, "Error %d: creating ITEM record\n", status);
        return status;
    }

    /* recursive call to build multi-level bill */
    status = build_bill(bom_task);
    d_trend(bom_task->task);
    return status;
}

/*********************************************************************
* build_bill() : recursive routine to build one level of a bill by
*                   adding components to a parent.
**********************************************************************/
static int32_t build_bill(
    BOM_TASK *bom_task) 
{
    int32_t     status;
    struct bill bill_rec;
    DB_ADDR     bom_owner = NULL_DBA;
    DB_ADDR     bom_member = NULL_DBA;
    int32_t     i;
    struct item item_rec;

    bom_task->current_level++;

    /* get current owner and member of BOM set - restore these before
       the function returns, so that the currency will be correct for
       the previous level of recursion
    */
    status = d_csoget(BOM, &bom_owner, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        /* We may not have a current owner yet */
        if(status == S_NOCO)
        {
            bom_owner = NULL_DBA;
            status = S_OKAY;
        }
        else
        {
            fprintf(stderr, "Error %d: getting current BOM owner\n", status);
            return status;
        }
    }

    status = d_csmget(BOM, &bom_member, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        /* We may not have a current member yet */
        if(status == S_NOCM)
        {
            bom_member = NULL_DBA;
            status = S_OKAY;
        }
        else
        {
            fprintf(stderr, "Error %d: getting current BOM member\n", status);
            return status;
        }
    }

    /* make the current record the current owner of BOM set */
    status = d_setor(BOM, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: setting current BOM owner\n", status);
        return status;
    }

    for (i=0; i<bom_task->max_members; i++) {

        /* create new ITEM record, for component item */
        item_rec.cost = 1.0L;
        bom_random_id(bom_task, (char *)(item_rec.id_code));
        if (d_fillnew(ITEM, &item_rec, bom_task->task, CURR_DB) != S_OKAY) {    /* new component ITEM record */
            printf("duplicate part %s\n", item_rec.id_code);
        }

        status = d_setor(WHERE_USED, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: setting current WHERE_USED owner\n", status);
            return status;
        }

        /* create new BILL record */
        bill_rec.quantity = 1.0L;
        bill_rec.level = bom_task->current_level;
        status = d_fillnew(BILL, &bill_rec, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: creating new BILL record\n", status);
            return status;
        }

        /* connect new BILL record to parent ITEM and component ITEM,
           to form a linkage between the two items, in a many-to-many
           relationship
        */
        status = d_connect(BOM, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: connecting BOM set\n", status);
            return status;
        }

        status = d_connect(WHERE_USED, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: connecting WHERE_USED set\n", status);
            return status;
        }

        bom_task->txCounter++;
        if (bom_task->txCounter%5000 == 0) 
        {
            status = d_trend(bom_task->task);
            if(status != S_OKAY)
            {
                fprintf(stderr, "Error %d: committing transaction\n", status);
                return status;
            }

            status = d_trbegin("bom", bom_task->task);
            if(status != S_OKAY)
            {
                fprintf(stderr, "Error %d: starting transaction\n", status);
                return status;
            }
        }

        /* if we are not at the bottom level, recurse down another level */
        if (bom_task->current_level < bom_task->max_level) 
        {

            /* make the new component ITEM record the current record,
               so that the recursive call to this function will connect
               further components to it
            */
            status = d_setro(WHERE_USED, bom_task->task, CURR_DB);
            if(status != S_OKAY)
            {
                fprintf(stderr, "Error %d: setting current record from WHERE_USED owner\n", status);
                return status;
            }

            status = build_bill(bom_task);
            if(status != S_OKAY)
            {
                return status;
            }
        }
    }

    bom_task->current_level--;

    /* restore original currency */
    status = d_csoset(BOM, &bom_owner, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: setting BOM owner\n", status);
        return status;
    }

    status = d_csmset(BOM, &bom_member, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: setting BOM member\n", status);
        return status;
    }
    return status;
}

/***************************************************************************
* rollup_bom() : Top level function for rolling the BOM
*****************************************************************************/
static int32_t rollup_bom(
    double   *total_cost,
    BOM_TASK *bom_task)
{
    int32_t status;

    /* Navigate the top level ITEM record */
    status = d_recfrst(ITEM, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: navigating to first ITEM record\n", status);
        return status;
    }
    return get_cost(total_cost, bom_task);
}

/****************************************************************************
* get_cost - recursive routine to roll up cost from lower levels of bill.
*               The costs are stored only at the lowest levels of the bill.
*****************************************************************************/
static int32_t get_cost(
    double   *total_cost,
    BOM_TASK *bom_task) 
{
    int32_t     status;
    DB_ADDR     bom_owner;
    DB_ADDR     bom_member;
    int32_t     member_count;
    struct bill Bill_local;
    struct item item_rec;
    double      dCost;
    double      dTotalCost;

    /* get current owner and member of BOM set - restore these before
       the function returns, so that the currency will be correct for
       the previous level of recursion
    */
    status = d_csoget(BOM, &bom_owner, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        /* We may not have a current member yet */
        if(status == S_NOCO)
        {
            bom_member = NULL_DBA;
            status = S_OKAY;
        }
        else
        {
            fprintf(stderr, "Error %d: getting current BOM member\n", status);
            return status;
        }
    }

    status = d_csmget(BOM, &bom_member, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        /* We may not have a current member yet */
        if(status == S_NOCM)
        {
            bom_member = NULL_DBA;
            status = S_OKAY;
        }
        else
        {
            fprintf(stderr, "Error %d: getting current BOM member\n", status);
            return status;
        }
    }

    /* make the current record the current owner of BOM set */
    status = d_setor(BOM, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: setting current BOM owner\n", status);
        return status;
    }

    /* get number of components connected to this ITEM record */
    status = d_members(BOM, &member_count, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: getting number of BOM members\n", status);
        return status;
    }

    /* no members - we are at the bottom level now */
    if (member_count == 0) {

        /* read the current ITEM to get cost */
        status = d_recread(&item_rec, bom_task->task, CURR_DB);

        /* restore original currency */
        status = d_csoset(BOM, &bom_owner, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: setting current BOM owner\n", status);
            return status;
        }

        status = d_csmset(BOM, &bom_member, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: setting current BOM member\n", status);
            return status;
        }

        /* return cost from the ITEM record just read */
        *total_cost = item_rec.cost;
        return status;
    }

    /* there is at least one member, so go down a level */
    dTotalCost = 0.0L;

    /* cycle through all components */
    while (member_count--) 
    {

        /* find next member - if there's no current member this will
           find the first member
        */
        status = d_findnm(BOM, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: getting next BOM member\n", status);
            return status;
        }

        /* read the bill rec to get quanity */
        status = d_recread(&Bill_local, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: reading BILL record\n", status);
            return status;
        }

        /* find its owner thru WHERE_USED set - this is a component ITEM */
        status = d_findco(WHERE_USED, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d: finding WHERE_USED owner\n", status);
            return status;
        }

        /* recursive call - get cost of all components connected to this one */
        status = get_cost(&dCost, bom_task);
        if(status == S_OKAY)
        {
            dTotalCost += (dCost * Bill_local.quantity);
        }
        else
        {
            return status;
        }
    }

    /* restore original currency */
    status = d_csoset(BOM, &bom_owner, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: setting current BOM owner\n", status);
        return status;
    }

    status = d_csmset(BOM, &bom_member, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d: setting current BOM member\n", status);
        return status;
    }

    *total_cost = dTotalCost;
    return status;
}

/*********************************************************************
* bom_initialize() : Create task and configure task
*                    Open and initialize database
**********************************************************************/
static int32_t bom_initialize(
    BOM_OPTS   *bom_opts,
    BOM_TASK   *bom_task)
{
    int32_t status;
  
    /* Initialize the TFS library */
    status = d_tfsinitEx(bom_opts->docroot ? bom_opts->docroot : "", NULL, &bom_task->hTFS, bom_task->cmd_status.tfs_type);
    if(status != S_OKAY)
    {
        fprintf(stderr,  "Error %d unable to inialize TFS\n", status);
        return status;
    }

    /* create a database task, open the database, and initialize it */ /*lint -esym(534,d_*) */
    status = d_opentask(&bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d creating a dbtask\n", status);
        d_tfsterm(bom_task->hTFS);
        return status;
    }

    status = d_set_dberr((ERRORPROC)MyErr, bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d setting a dberror handler\n", status);
        d_closetask(bom_task->task);
        d_tfsterm(bom_task->hTFS);
        return status;
    }
    
    status = d_open_ptr ("bom", "o", &bom_dbd, sizeof(bom_dbd), bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d opening database\n", status);
        d_closetask(bom_task->task);
        d_tfsterm(bom_task->hTFS);
        return status;
    }

    status = d_initialize(bom_task->task, CURR_DB);    /* erase all old data */
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d initializing database\n", status);
        d_close(bom_task->task);
        d_closetask(bom_task->task);
        d_tfsterm(bom_task->hTFS);
        return status;
    }
    d_tfsterm(bom_task->hTFS);
    return status;
}

/*********************************************************************
* bom_cleanup() : Disconnect from the database and free statement 
*                 handles
**********************************************************************/
static int32_t bom_cleanup(
    BOM_TASK   *bom_task)
{
    int32_t status;

    status = d_close(bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d closing database\n", status);
        d_closetask(bom_task->task);
        return status;
    }

    status = d_closetask(bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d closing task\n", status);
        return status;
    }
    d_tfsterm(bom_task->hTFS);

    if(bom_task->cmd_status.stdout_opened)
        psp_stdout_close();
        
    if(bom_task->cmd_status.stderr_opened)
        psp_stderr_close();

    return status;
}

/*********************************************************************
* bom_random_id() : generates 15-character alpha part id
**********************************************************************/
static void bom_random_id(
    BOM_TASK *bom_task,
    char     *string)    /* generates 15-character alpha part id */
{
    sprintf(string, "%-15.15d", ++(bom_task->count));
}

/*********************************************************************
* MyErr() : Custom RDM Error Routine
**********************************************************************/
static void EXTERNAL_FCN MyErr( int32_t err, char *errmsg )
{ /*lint !e403 */
    printf("Fatal Error: %s\n", errmsg);
}

/*********************************************************************
* GetOptions() : Process command line arguments
**********************************************************************/
static int32_t GetOptions(
    int32_t            argc, 
    const char *const *argv,
    BOM_OPTS          *bom_opts,
    BOM_TASK          *bom_task)
{
    int32_t      optstat;
    const char  *opt;
    const char  *optarg;

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

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("Network Model BOM Benchmark"), argc,
            argv, opts, NULL, &bom_task->cmd_status);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
            case 'd':
                if(strcmp(opt, "docroot") == 0)
                {
                    if(bom_task->cmd_status.tfs_type == TFS_TYPE_RPC)
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
PSP_STARTUP_EXAMPLE_CR(bom)
