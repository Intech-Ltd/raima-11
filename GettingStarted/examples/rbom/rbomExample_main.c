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

#if defined(RDM_VXWORKS)
#define MOD rbom
#endif

#include "rdm.h"

/*lint ++flb -e956 */
#include "rbom_dbd.h"
#include "rbom.h"
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
    uint32_t        key_state_size;
    TFS_HANDLE      hTFS;
    DB_TASK        *task;
    cmdline_status  cmd_status;
} BOM_TASK;

/****************************************
* Function Prototypes
****************************************/
static int32_t GetOptions(int32_t, const char *const *, BOM_OPTS*, BOM_TASK*);
static int32_t rbuild_bill(const char *, BOM_TASK *);
static int32_t create_bom(BOM_TASK *);
static int32_t rget_cost(const char *, double *, BOM_TASK *);
static int32_t rollup_bom(double *, BOM_TASK *);
static void    rbom_random_id(char *, BOM_TASK *);
static int32_t bom_initialize(BOM_OPTS *, BOM_TASK *);
static int32_t bom_cleanup(BOM_TASK *);
static void    MyErr( int32_t err, char *errmsg);
int32_t EXTERNAL_FCN rbom_main(int32_t argc, const char *const *argv);

/************************************************
* rbom_main() : Main function of rbomExample
************************************************/
int32_t EXTERNAL_FCN rbom_main(
    int32_t            argc, 
    const char *const *argv)
{
    int32_t    status;
    BOM_OPTS   bom_opts;
    BOM_TASK   bom_task;
    uint32_t   start_time;
    uint32_t   end_time;
    double     rolled_up_cost;
    char       response[20];

    memset(&bom_opts, 0, sizeof(bom_opts));
    memset(&bom_task, 0, sizeof(bom_task));

    /* Process the command line options */
    if(GetOptions(argc, argv, &bom_opts, &bom_task) != B_CMD_OKAY)
    {
        if(bom_task.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(bom_task.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    bom_task.max_level = bom_opts.max_levels;
    while (bom_task.max_level == 0) 
    {
        printf("\nEnter number of levels: ");
        fflush (stdout);
        fgets(response, sizeof(response), stdin);
        bom_task.max_level = atoi(response);
    }
    
    bom_task.max_members = bom_opts.max_members;
    while (bom_task.max_members == 0) 
    {
        printf("\nEnter number of members per level: ");
        fflush (stdout);
        fgets(response, sizeof(response), stdin);
        bom_task.max_members = atoi(response);
    }
    bom_task.current_level = 0;
    
    printf("Create BOM with %d levels and %d members per level\n", bom_task.max_level, bom_task.max_members);

    status = bom_initialize(&bom_opts, &bom_task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error initializing application\n");
        bom_cleanup(&bom_task);
        return 1;
    }

    printf("   building bill file\n");
    start_time = psp_timeMilliSecs();
    status = create_bom(&bom_task);
    if(status == S_OKAY)
    {
        end_time = psp_timeMilliSecs();
        printf("\tmillisecs: %u\n", end_time - start_time);
    }
    else
    {
        fprintf(stderr, "Error creating BOM\n");
        bom_cleanup(&bom_task);
        return 1;
    }

    printf("   rolling up cost\n");
    start_time = psp_timeMilliSecs();
    status = rollup_bom(&rolled_up_cost, &bom_task);
    if(status == S_OKAY)
    {
        end_time = psp_timeMilliSecs();
        printf("      total rolled up cost = %10.2f\n", rolled_up_cost);
        printf("\tmillisecs: %u\n", end_time - start_time);
    }
    else
    {
        fprintf(stderr, "Error rolling up BOM\n");
        return 1;
    }
    bom_cleanup(&bom_task);
    return (0);
}

/***************************************************************************
* create_bom() : Top level function for creating the BOM
*****************************************************************************/
static int32_t create_bom(
    BOM_TASK   *bom_task)
{
    int32_t      status;
    char         id_code[SIZEOF_RITEM_RID_CODE];
    struct ritem ritem_rec;

    bom_task->txCounter = 0;
    status = d_trbegin("bom", bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d starting a transaction\n", status);
        return status;
    }

    /* create top level ITEM record */
    rbom_random_id(id_code, bom_task);
    strcpy((char *)ritem_rec.rid_code, (char *)id_code);
    ritem_rec.rcomponent_count = bom_task->max_members;
    ritem_rec.rcost = 1.0L;
    if ((status = d_fillnew(RITEM, &ritem_rec, bom_task->task,  CURR_DB)) != S_OKAY) 
    {
        fprintf(stderr, "Error %d creating an ITEM record\n", status);
        return status;
    }

    /* recursive call to build multi-level bill */
    status = rbuild_bill(id_code, bom_task);
    d_trend(bom_task->task);
    return status;
}

/*********************************************************************
* rbuild_bill() : recursive routine to build one level of a bill by
*                   adding components to a parent.
**********************************************************************/
static int32_t rbuild_bill(
    const char *parent,
    BOM_TASK   *bom_task)
{
    int32_t status;
    int32_t ii;
    char    id_code[16];
    struct rbill rbill_rec;
    struct ritem ritem_rec;

    bom_task->current_level++;

    for (ii=0; ii<bom_task->max_members; ii++) 
    {
        /* create an RITEM record which will be a component of the parent
           RITEM record whose id code was passed into this function
        */
        ritem_rec.rcost = 1.0L;
        rbom_random_id((char *)ritem_rec.rid_code, bom_task);
        if (bom_task->current_level < bom_task->max_level) 
        {
            ritem_rec.rcomponent_count = bom_task->max_members;
        }
        else 
        {
            /* this is the bottom of the tree */
            ritem_rec.rcomponent_count = 0;
        }
        if ((status = d_fillnew(RITEM, &ritem_rec, bom_task->task, CURR_DB)) != S_OKAY) 
        {
            fprintf(stderr, "Error %d creating ITEM record\n", status);
            return status;
        }

        /* create an RBILL record, with relational references to the parent
           and component RITEM records - this RBILL record provides a
           linkage in a many-to-many relationship
        */
        strcpy((char *)rbill_rec.rparent, (char *)parent);
        strcpy((char *)rbill_rec.rcomponent, (char *)ritem_rec.rid_code);
        rbill_rec.rquantity = 1.0L;
        rbill_rec.rsequence = ii;
        rbill_rec.rlevel = bom_task->current_level;
        status = d_fillnew(RBILL, &rbill_rec, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            fprintf(stderr, "Error %d creating BILL record\n", status);
            return status;
        }

        ++(bom_task->txCounter);
        if (bom_task->txCounter%5000 == 0) 
        {
            status = d_trend(bom_task->task);
            if(status != S_OKAY)
            {
                fprintf(stderr, "Error %d committing transaction\n", status);
                return status;
            }

            status = d_trbegin("bom", bom_task->task);
            if(status != S_OKAY)
            {
                fprintf(stderr, "Error %d starting transaction\n", status);
                return status;
            }
        }

        /* if we are not at the bottom of the tree, attach further
           components to this component RITEM, by recursively calling
           this function
        */
        if (bom_task->current_level < bom_task->max_level) 
        {
            strcpy((char *)id_code, (char *)ritem_rec.rid_code);
            status = rbuild_bill(id_code, bom_task);
        }
    }
    (bom_task->current_level)--;
    return status;
}

/***************************************************************************
* rollup_bom() : Top level function for rolling the BOM
*****************************************************************************/
static int32_t rollup_bom(
    double     *cost,
    BOM_TASK   *bom_task)
{
    char id_code[SIZEOF_RITEM_RID_CODE];

    d_keyfrst(RITEM_RID_CODE, bom_task->task, CURR_DB);
    d_crread(RITEM_RID_CODE, id_code, bom_task->task, CURR_DB);

    /* recursive call to calculate cost of whole component tree */
    return (rget_cost(id_code, cost, bom_task));
}

/****************************************************************************
* rget_cost - recursive routine to roll up cost from lower levels of bill.
*               The costs are stored only at the lowest levels of the bill.
*****************************************************************************/
static int32_t rget_cost(
    const char *parent,
    double     *total_cost,
    BOM_TASK   *bom_task)
{
    int32_t status;
    double  dTotalCost = 0.0; /* This item and all sub-components */
    double  dCost = 0.0;      /* for one sub-component */
    int32_t component_count;
    struct rbill_rbom Rbom;
    struct rbill RBill_local;
    struct ritem ritem_rec;
    void  *key_state;

    key_state = psp_cGetMemory(bom_task->key_state_size, TAG0);
    if(!key_state)
    {
        return S_NOMEMORY;
    }
    
    /* get the current key state for the RBOM key - on exit, this function
       repositions to this state, so that calls to d_keynext in the previous
       level of recursion will find the right value
    */
    status = d_keyrdstate(RBILL_RBOM, key_state, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        return status;
    }

    /* use RID_CODE key to find the parent item */
    status = d_keyfind(RITEM_RID_CODE, parent, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        return status;
    }
    
    /* read the whole record */
    status = d_recread(&ritem_rec, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        return status;
    }

    component_count = ritem_rec.rcomponent_count;
    if (component_count == 0) 
    {
        /* no components for this parent - return the cost of this
           bottom-level item
        */
        *total_cost = ritem_rec.rcost;
        return status;
    }

    /* there is at least one component, so go down a level - use RBOM key
       to find the first RBILL record that refers to the parent item
    */
    strcpy((char *)Rbom.rparent, (char *)parent);
    Rbom.rsequence = 0;
    status = d_keyfind(RBILL_RBOM, &Rbom, bom_task->task, CURR_DB);  /* find first bill record */
    if(status != S_OKAY)
    {
        return status;
    }
    dTotalCost = 0.0L;

    /* use RBOM key to go through all RBILL records that refer to the
       parent item
    */
    while (status == S_OKAY)
    {
        /* read bill rec to get component ID */
        status = d_recread(&RBill_local, bom_task->task, CURR_DB);
        if(status != S_OKAY)
        {
            return status;
        }

        /* this component may also be a parent - call this function
           recursively to get costs for the component tree attached
           to the RITEM record
        */
        status = rget_cost((const char *)RBill_local.rcomponent, &dCost, bom_task);
        if(status == S_OKAY)
        {
            dTotalCost += (dCost * RBill_local.rquantity); 
        }
        else
        {
            return status;
        }

        if(--component_count == 0) break;

        /* find next bill record */
        status = d_keynext(RBILL_RBOM, bom_task->task, CURR_DB);
    }

    /* reposition to start position in RBOM key */
    status = d_keywrstate(RBILL_RBOM, key_state, bom_task->task, CURR_DB);
    psp_freeMemory(key_state, TAG0);

    *total_cost = dTotalCost;
    return status;    /* for everything below this item */
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
  
    /* Initialize the TFS library (required for TFST and TFSS) */
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
        return status;
    }

    status = d_set_dberr((ERRORPROC)MyErr, bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d setting a dberror handler\n", status);
        d_closetask(bom_task->task);
        return status;
    }
    
    status = d_open_ptr ("rbom", "o", &rbom_dbd, sizeof(rbom_dbd), bom_task->task);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d opening database\n", status);
        d_closetask(bom_task->task);
        return status;
    }

    status = d_initialize(bom_task->task, CURR_DB);    /* erase all old data */
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d initializing database\n", status);
        d_close(bom_task->task);
        d_closetask(bom_task->task);
        return status;
    }

    status = d_keyszstate(RBILL_RBOM, &bom_task->key_state_size, bom_task->task, CURR_DB);
    if(status != S_OKAY)
    {
        fprintf(stderr, "Error %d reading key state size\n", status);
        d_close(bom_task->task);
        d_closetask(bom_task->task);
        return status;
    }
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
* rbom_random_id() : generates 15-character alpha part id
**********************************************************************/
static void rbom_random_id(
    char *string,
    BOM_TASK *bom_task)
{
    sprintf(string, "%-15.15d", ++(bom_task->count));
}

/*********************************************************************
* MyErr() : Custom RDM Error Routine
**********************************************************************/
static void MyErr( int32_t err, char *errmsg)
{
    printf( "Fatal Error: %s\n", errmsg);
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

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("Relational Model BOM Benchmark"),
            argc, argv, opts, NULL, &bom_task->cmd_status);
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
PSP_STARTUP_EXAMPLE_CR(rbom)
