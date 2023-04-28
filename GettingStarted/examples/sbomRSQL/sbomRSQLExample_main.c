/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2001-2011 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * sbomRSQLExample - RSQL version of the Bill of Materials example
 * ----------------------------------------------------------------------------
 */

 /****************************************
 * Includes
 *****************************************/
#include "rdmsql.h"
/*lint ++flb */
#include "sbomRSQL_cat.h"
/*lint --flb */
#include "sbomRSQL.h"

/*****************************************
* Structure Definitions
******************************************/
typedef struct _BOM_OPTS
{
    int32_t        max_levels;
    int32_t        max_members;
}  BOM_OPTS;

typedef struct _BOM_TASK
{
    int32_t         current_level;
    int32_t         max_levels;
    int32_t         max_members;
    int32_t         txCounter;
    int32_t         count;
    struct rbill    RBill;
    struct ritem    RItem;
    char            parent[51];
    char            rcomponent[51];
    char            szRootId[20];
    HCONN           hDbc;
    HSTMT           hItemStmt;
    HSTMT           hBillStmt;
    const char     *docroot;
    cmdline_status  cmd_status;
} BOM_TASK;

/****************************************
* Function Prototypes
****************************************/
static int32_t GetOptions(int32_t, const char *const *, BOM_OPTS *, BOM_TASK *);
static RSQL_ERRCODE sbomPrepareItemStmtForInsert(BOM_TASK *);
static RSQL_ERRCODE sbomPrepareBillStmtForInsert(BOM_TASK *);
static RSQL_ERRCODE sbomPrepareItemStmtForSelect(BOM_TASK *);
static RSQL_ERRCODE sbomBuildBill(BOM_TASK *);
static RSQL_ERRCODE sbomGetCost(double *, BOM_TASK *);
static void         sbomGetRandomId(char *, BOM_TASK *);
static void         sbomDisplayrsqlError(HSTMT, char *);
static RSQL_ERRCODE create_bom(BOM_TASK *);
static RSQL_ERRCODE rollup_bom(double *, BOM_TASK *);
static RSQL_ERRCODE bom_initialize(BOM_TASK *);
static RSQL_ERRCODE bom_cleanup(BOM_TASK *);
int32_t EXTERNAL_FCN sbomRSQL_main(int32_t, const char *const *);

/************************************************
* sbomRSQL_main() : Main function of sbomRSQLExample
************************************************/
int32_t EXTERNAL_FCN sbomRSQL_main(
    int32_t            argc,
    const char *const *argv)
{
    RSQL_ERRCODE rsqlError;
    BOM_OPTS     bom_opts;
    BOM_TASK     bom_task;
    uint32_t     ulStartTime;
    uint32_t     ulEndTime;
    double       dRolledUpCost;    
    char         response[20];

    memset(&bom_opts, 0, sizeof(bom_opts));
    memset(&bom_task, 0, sizeof(bom_task));

    if( GetOptions(argc, argv, &bom_opts, &bom_task) != B_CMD_OKAY)
    {
        if(bom_task.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(bom_task.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    bom_task.max_levels = bom_opts.max_levels;
    while (bom_task.max_levels == 0) 
    {
        printf("\nEnter number of levels: ");
        fflush (stdout);
        fgets(response, sizeof(response), stdin);
        bom_task.max_levels = atoi(response);
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

    printf("Create BOM with %d levels and %d members per level\n", bom_task.max_levels, bom_task.max_members);

    rsqlError = bom_initialize(&bom_task);
    if (rsqlError != errSUCCESS)
    {
        fprintf(stderr, "Initialization failed...\n");
        if(bom_task.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(bom_task.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    /* Create the BOM */
    printf("   building bill file\n");
    ulStartTime = psp_timeMilliSecs();
    rsqlError = create_bom(&bom_task);
    ulEndTime = psp_timeMilliSecs();
    if (rsqlError == errSUCCESS)
    {
        printf("\tmillisecs: %u\n", ulEndTime - ulStartTime);
    }
    else
    {
        printf("Error: %d creating BOM\n", rsqlError);
        bom_cleanup(&bom_task);
        return 1;
    }
        
    /* Roll up the BOM */
    printf("   rolling up cost\n");
    ulStartTime = psp_timeMilliSecs();
    rsqlError = rollup_bom(&dRolledUpCost, &bom_task);
    ulEndTime = psp_timeMilliSecs();
    if(rsqlError == errSUCCESS)
    {
        printf("      total rolled up cost = %10.2f\n", dRolledUpCost);
        printf("\tmillisecs: %u\n", ulEndTime - ulStartTime);
    }
    else
    {
        printf("Error: %d rolling up BOM\n", rsqlError);
        bom_cleanup(&bom_task);
        return 1;
    }
    bom_cleanup(&bom_task);
    return 0;
}

/***************************************************************************
* create_bom() : Top level function for creating the BOM
*****************************************************************************/
static RSQL_ERRCODE create_bom(
    BOM_TASK *bom_task)
{    
    RSQL_ERRCODE rsqlError;

    /* Prepares statements for insertion */
    rsqlError = sbomPrepareItemStmtForInsert(bom_task); 
    if (rsqlError == errSUCCESS)
    {
        rsqlError = sbomPrepareBillStmtForInsert(bom_task);
        if (rsqlError == errSUCCESS)
        {
            rsqlTransStart(bom_task->hDbc, NULL);
            sbomGetRandomId(bom_task->szRootId, bom_task);
            strcpy(bom_task->RItem.rid_code, bom_task->szRootId);
            bom_task->RItem.rcomponent_count = bom_task->max_members;
            bom_task->txCounter = 0;
            rsqlError = rsqlExecute(bom_task->hItemStmt);
            if (rsqlError == errSUCCESS)
            {
                rsqlError = sbomBuildBill(bom_task);
                rsqlTransCommit(bom_task->hDbc);
            }
        }
        rsqlCloseStmt(bom_task->hBillStmt); /*lint !e534 */
    }
    rsqlCloseStmt(bom_task->hItemStmt); /*lint !e534 */
    return rsqlError;
}

/*********************************************************************
* sbomBuildBill() : recursive routine to build one level of a bill by
*                   adding components to a parent.
**********************************************************************/
static RSQL_ERRCODE sbomBuildBill(
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError = errSUCCESS;
    int32_t i;
    char szParent[16];

    strcpy(szParent, bom_task->RItem.rid_code);

    (bom_task->current_level)++;

    for (i = 0; i < bom_task->max_members && rsqlError == errSUCCESS; i++)
    {
    /* create an item record which will be a component of the parent
    item record whose id code was passed into this function
        */
        sbomGetRandomId(bom_task->RItem.rid_code, bom_task);

        if (bom_task->current_level < bom_task->max_levels)
        {
            /* if this is the bottom of the tree then component count is zero */
            
            bom_task->RItem.rcomponent_count = bom_task->max_members;
            
            bom_task->RItem.rcost = 0.0;
        }
        else
        {
            /* if this is the bottom of the tree then component count is zero */
            bom_task->RItem.rcomponent_count = 0;
            
            bom_task->RItem.rcost = 1.0;
        }

        /* insert new item record */
        if ((rsqlError = rsqlExecute(bom_task->hItemStmt)) == errSUCCESS)
        {
            /* assign values to bill record, with relational references to
               the parent and component item records - this bill record
               provides a linkage in a many-to-many relationship
            */
            strcpy(bom_task->parent, szParent);
            strcpy(bom_task->rcomponent, bom_task->RItem.rid_code);
            bom_task->RBill.rquantity = 1.0L;
            bom_task->RBill.rsequence = i;
            bom_task->RBill.rlevel = bom_task->current_level;

            /* insert new bill record */
            if ((rsqlError = rsqlExecute(bom_task->hBillStmt)) == errSUCCESS)
            {
                if (++(bom_task->txCounter)%5000 == 0)
                    rsqlTransCommit(bom_task->hDbc); /*lint !e534 */

            /* if we are not at the bottom of the tree, attach further
            components to this component item, by recursively calling
            this function
                */
                if (bom_task->current_level < bom_task->max_levels)
                {
                    rsqlError = sbomBuildBill(bom_task);
                }
            }
            else
            {
                sbomDisplayrsqlError(bom_task->hBillStmt, bom_task->parent);
            }
        }
        else
        {
            sbomDisplayrsqlError(bom_task->hItemStmt, bom_task->RItem.rid_code);
        }
    }
    (bom_task->current_level)--;
    return rsqlError;
}

/***************************************************************************
* sbomPrepareItemStmtForInsert() : Prepares item statement for insertion by
*                                  binding parameters to be used later
*****************************************************************************/
static RSQL_ERRCODE sbomPrepareItemStmtForInsert(
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;

    rsqlError = rsqlPrepare(bom_task->hItemStmt,  "insert into ritem values (?,?,?,?)");
    if (rsqlError == errSUCCESS)
    {
        rsqlError = rsqlBindParam(bom_task->hItemStmt, 1, tCHAR, &bom_task->RItem.rid_code, 0);
        if (rsqlError == errSUCCESS)
        {
            rsqlError = rsqlBindParam(bom_task->hItemStmt, 2, tCHAR, &bom_task->RItem.rdescription, 0);
            if (rsqlError == errSUCCESS)
            {
                rsqlError = rsqlBindParam(bom_task->hItemStmt, 3, tDOUBLE, &bom_task->RItem.rcost, 0);
                if (rsqlError == errSUCCESS)
                    rsqlError = rsqlBindParam(bom_task->hItemStmt, 4, tINTEGER, &bom_task->RItem.rcomponent_count, 0);
            }
        }
    }
    return rsqlError;
}

/***************************************************************************
* sbomPrepareBillStmtForInsert() : Prepares Bill statement for insertion by
*                                  binding parameters to be fetched later
*****************************************************************************/
static RSQL_ERRCODE sbomPrepareBillStmtForInsert(
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;

    rsqlError = rsqlPrepare(bom_task->hBillStmt, "insert into rbill values (?,?,?,?,?)");
    if (rsqlError == errSUCCESS)
    {
         rsqlError = rsqlBindParam(bom_task->hBillStmt, 1, tCHAR,  &bom_task->rcomponent, 0);
        if (rsqlError == errSUCCESS)
        {      
            rsqlError = rsqlBindParam(bom_task->hBillStmt, 2, tCHAR, &bom_task->parent, 0);
            if (rsqlError == errSUCCESS)
            {
                rsqlError = rsqlBindParam(bom_task->hBillStmt, 3, tINTEGER, &bom_task->RBill.rsequence, 0);
                if (rsqlError == errSUCCESS)
                {
                    rsqlError = rsqlBindParam(bom_task->hBillStmt, 4, tDOUBLE, &bom_task->RBill.rquantity, 0);
                    if (rsqlError == errSUCCESS)
                    {
                        rsqlError = rsqlBindParam(bom_task->hBillStmt, 5, tINTEGER, &bom_task->RBill.rlevel, 0);
                    }
                }
            }
        }
    }
    return rsqlError;
}

/***************************************************************************
* rollup_bom() : Top level function for rolling the BOM
*****************************************************************************/
static RSQL_ERRCODE rollup_bom(
    double   *dRolledUpCost,
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;
    rsqlError = sbomPrepareItemStmtForSelect(bom_task);
    if (rsqlError == errSUCCESS)
    {
        strcpy(bom_task->RItem.rid_code, bom_task->szRootId);
        rsqlError = sbomGetCost(dRolledUpCost, bom_task);
    }
    rsqlCloseStmt(bom_task->hItemStmt); /*lint !e534 */
    return rsqlError;
}

/****************************************************************************
* sbomGetCost - recursive routine to roll up cost from lower levels of bill.
*               The costs are stored only at the lowest levels of the bill.
*****************************************************************************/
static RSQL_ERRCODE sbomGetCost(
    double   *pdCost,
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;
    double            dTotalCost = 0.0; /* for item and all sub-components */
    double            dCost;            /* for one sub-component */
    double            quantity;
    char              szParent[16];
    const RSQL_VALUE *Result;
    uint16_t          pNoCols;
    HSTMT             hBillSelectStmt;
    char             *stmt;

    strcpy(szParent, bom_task->RItem.rid_code);

    /* Allocates bill statement */
    rsqlError = rsqlAllocStmt(bom_task->hDbc, &hBillSelectStmt);
    if (rsqlError != errSUCCESS)
    {
        printf("Error has occured allocating bill select statement");
        return rsqlError;
    }

    /* select the item record specified in RItem.id */
    rsqlError = rsqlExecute(bom_task->hItemStmt);
    if (rsqlError == errSUCCESS)
    {
        /* there should only be one item that matches the select criteria */
        rsqlError = rsqlFetch(bom_task->hItemStmt, &Result, &pNoCols);
        rsqlCloseStmt(bom_task->hItemStmt); /*lint !e534 */
        if (rsqlError == errSUCCESS)
        {
            bom_task->RItem.rcomponent_count =  Result[0].vt.lv;
            if (bom_task->RItem.rcomponent_count == 0)
            {
                /* no sub-components for this item - return the cost of this
                   bottom-level item
                */
                *pdCost = (double) Result[1].vt.dv;
                return rsqlError;
            }

            stmt = psp_makeString("select rcomponent, rquantity from rbill where rparent=\"%s\"", szParent, TAG0);
            rsqlError = rsqlExecDirect(hBillSelectStmt, stmt);
            psp_freeMemory(stmt, TAG0);
            if (rsqlError == errSUCCESS)
            {
                while((rsqlError = rsqlFetch(hBillSelectStmt, &Result, &pNoCols)) == errSUCCESS)
                {
                    strcpy(bom_task->RItem.rid_code, Result[0].vt.cv);
                    quantity = (double) Result[1].vt.dv;
            
                    /* get the cost for this component, by recursively calling this function */
                    rsqlError = sbomGetCost(&dCost, bom_task);
                        
                    dTotalCost += (dCost * quantity);
                }

                /* We should break out of the loop when we get errNOMOREDATA */
                if (rsqlError == errNOMOREDATA)
                {
                    rsqlError = errSUCCESS;
                }
                else
                {
                    sbomDisplayrsqlError(hBillSelectStmt, bom_task->parent);
                }
            }
            else
            {
                sbomDisplayrsqlError(hBillSelectStmt, bom_task->parent);
            }
            rsqlCloseStmt(hBillSelectStmt); /*lint !e534 */
            rsqlFreeStmt(hBillSelectStmt); /*lint !e534 */
        }
        else if (rsqlError == errNOMOREDATA)
        {
            printf("\nPart number %s not found\n", szParent);
        }
    }
    else
    {
        sbomDisplayrsqlError(bom_task->hItemStmt, bom_task->RItem.rid_code);
    }

    *pdCost = (double) dTotalCost;
    return rsqlError;
}

/***************************************************************************
* sbomPrepareItemStmtForSelect() : Prepares item statement for selection by
*                                  binding parameters to be fetched later
*****************************************************************************/
static RSQL_ERRCODE sbomPrepareItemStmtForSelect(
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;

    rsqlError = rsqlPrepare(bom_task->hItemStmt, "select rcomponent_count, rcost from ritem where rid_code=?");
    if (rsqlError == errSUCCESS)
    {
        rsqlError = rsqlBindParam(bom_task->hItemStmt, 1, tCHAR, &bom_task->RItem.rid_code, 0);
    }
    return rsqlError;
}

/*********************************************************************
* bom_initialize() : Create connection to the database and allocate
*                    statement handles
**********************************************************************/
static RSQL_ERRCODE bom_initialize(
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;

    rsqlError = rsqlTFSInitEx(bom_task->docroot ? bom_task->docroot : "", NULL, bom_task->cmd_status.tfs_type);
    if (rsqlError == errSUCCESS)
    {
        rsqlError = rsqlAllocConn(&bom_task->hDbc);
        if (rsqlError == errSUCCESS)
        {
            rsqlError = rsqlOpenCat(bom_task->hDbc, &sbomRSQL_cat, "sbomRSQL", "x");
            if (rsqlError != errSUCCESS)
            {
                rsqlFreeConn(&bom_task->hDbc); /*lint !e534 */
                printf("Error has occured opening database");
                return rsqlError;
            }

            rsqlError = rsqlInitDB(bom_task->hDbc, "sbomRSQL");
            if (rsqlError != errSUCCESS)
            {
                rsqlFreeConn(&bom_task->hDbc); /*lint !e534 */
                printf("Error has occured initializing database");
                return rsqlError;
            }

            /* allocates item statement */
            rsqlError = rsqlAllocStmt(bom_task->hDbc, &bom_task->hItemStmt);
            if (rsqlError != errSUCCESS)
            {
                rsqlFreeConn(&bom_task->hDbc); /*lint !e534 */
                printf("Error has occured allocating item statement");
                return rsqlError;
            }

            /* Allocates bill statement */
            rsqlError = rsqlAllocStmt(bom_task->hDbc, &bom_task->hBillStmt);
            if (rsqlError != errSUCCESS)
            {
                rsqlFreeStmt(bom_task->hItemStmt); /*lint !e534 */
                rsqlFreeConn(&bom_task->hDbc); /*lint !e534 */
                printf("Error has occured allocating bill statement");
                return rsqlError;
            }
        }
    }
    else
    {
        printf("Error has occured initializing the TFS");
        return rsqlError;
    }
    return rsqlError;
}

/*********************************************************************
* bom_cleanup() : Disconnect from the database and free statement 
*                 handles
**********************************************************************/
static RSQL_ERRCODE bom_cleanup(
    BOM_TASK *bom_task)
{
    RSQL_ERRCODE rsqlError;

    rsqlFreeStmt(bom_task->hBillStmt); /*lint !e534 */
    rsqlFreeStmt(bom_task->hItemStmt); /*lint !e534 */
    
    rsqlError = rsqlCloseDB(bom_task->hDbc, "sbomRSQL");
    if (rsqlError == errSQLERROR)
    {
        printf(" SQLERROR %d \n", errSQLERROR);         
    }
    rsqlError = rsqlFreeConn(&bom_task->hDbc); /*lint !e534 */

    rsqlTFSTerm();

    if(bom_task->cmd_status.stdout_opened)
        psp_stdout_close();
        
    if(bom_task->cmd_status.stderr_opened)
        psp_stderr_close();

    return rsqlError;
}

/*********************************************************************
* sbomGetRandomId() : generates 15-character alpha part id
**********************************************************************/
static void sbomGetRandomId(
    char*     string,
    BOM_TASK *bom_task)
{
    sprintf(string, "%-15.15d", ++(bom_task->count));
}


/*********************************************************************
* sbomDisplayrsqlError() : Display error information
**********************************************************************/
static void sbomDisplayrsqlError(HSTMT hStmt, char *pszId)
{

    char szError[128];
    szError[0] = '\0';

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
        { ARG_OPT,  ":docroot", "s=path", "Document root for the TFS instance" },
        { ARG_OPT,  ":l",       "i=#",     "Number of levels (default 5)" },
        { ARG_OPT,  ":m",       "i=#",     "Number of members per level (default 5)" },
        { ARG_END,  NULL,       NULL,      NULL }
    };
    /*lint -restore */

    RDM_ASSERT(bom_opts);

    bom_opts->max_levels = 0;
    bom_opts->max_members = 0;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("RSQL BOM Benchmark"),
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
                    bom_task->docroot = opt;
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
PSP_STARTUP_EXAMPLE_CR(sbomRSQL)
