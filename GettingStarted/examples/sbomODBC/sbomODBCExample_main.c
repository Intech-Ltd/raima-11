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
 * ----------------------------------------------------------------------------
 */

#include "sqlrext.h"

/*****************************************
* Structure Definitions
******************************************/
struct ritem {
   char rid_code[17];
   char rdescription[59];
   double rcost;
   int32_t rcomponent_count;
};

struct rbill {
   int32_t rsequence;
   double rquantity;
   int32_t rlevel;
};

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
    SQLHENV         hEnv;
    SQLHDBC         hDbc;
    SQLHSTMT        hItemStmt;
    SQLHSTMT        hBillStmt;
    SQLLEN          lSize;
    cmdline_status  cmd_status;
} BOM_TASK;

#define errTXACTIVE 2501

/****************************************
* Function Prototypes
****************************************/
static int32_t GetOptions(int32_t, const char *const *, BOM_OPTS *, BOM_TASK *);
static SQLRETURN sbomPrepareItemStmtForInsert(BOM_TASK *);
static SQLRETURN sbomPrepareBillStmtForInsert(BOM_TASK *);
static SQLRETURN sbomPrepareItemStmtForSelect(BOM_TASK *);
static SQLRETURN sbomBuildBill(BOM_TASK *);
static SQLRETURN sbomGetCost(double *, BOM_TASK *);
static void      sbomGetRandomId(char *, BOM_TASK *);
static void      sbomDisplaySqlError(SQLHSTMT, char *);
static SQLRETURN create_bom(BOM_TASK *);
static SQLRETURN rollup_bom(double *, BOM_TASK *);
static SQLRETURN bom_initialize(BOM_TASK *);
static SQLRETURN bom_cleanup(BOM_TASK *);
int32_t EXTERNAL_FCN sbomODBC_main(int32_t, const char *const *);

/****************************************************
* sbomODBC_main() : Main function of sbomODBCExample
*****************************************************/
int32_t EXTERNAL_FCN sbomODBC_main(
    int32_t            argc,
    const char *const *argv)
{
    char      response[20];
    SQLRETURN sqlError;
    BOM_OPTS  bom_opts;
    BOM_TASK  bom_task;
    uint32_t  ulStartTime;
    uint32_t  ulEndTime;
    double    dRolledUpCost;

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

    bom_task.RItem.rcost = 1.0L;
    bom_task.RBill.rquantity = 1.0L;

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

    sqlError = bom_initialize(&bom_task);
    if(sqlError != SQL_SUCCESS)
    {
        if(bom_task.cmd_status.stdout_opened)
            psp_stdout_close();
        
        if(bom_task.cmd_status.stderr_opened)
            psp_stderr_close();

        return 1;
    }

    /* Create the BOM */
    printf("   building bill file\n");
    ulStartTime = psp_timeMilliSecs();
    sqlError = create_bom(&bom_task);
    ulEndTime = psp_timeMilliSecs();
    if (sqlError == SQL_SUCCESS)
    {
        printf("\tmillisecs: %u\n", ulEndTime - ulStartTime);
    }
    else
    {
        fprintf(stderr, "Error: %d creating BOM\n", sqlError);
        bom_cleanup(&bom_task);
        return 1;
    }

    /* Roll up the BOM */
    printf("   rolling up cost\n");
    ulStartTime = psp_timeMilliSecs();
    sqlError = rollup_bom(&dRolledUpCost, &bom_task);
    ulEndTime = psp_timeMilliSecs();
    if(sqlError == SQL_SUCCESS)
    {
        printf("      total rolled up cost = %10.2f\n", dRolledUpCost);
        printf("\tmillisecs: %u\n", ulEndTime - ulStartTime);
    }
    else
    {
        fprintf(stderr, "Error: %d rolling up BOM\n", sqlError);
        bom_cleanup(&bom_task);
        return 1;
    }

    bom_cleanup(&bom_task);
    return 0;
}

/***************************************************************************
* create_bom() : Top level function for creating the BOM
*****************************************************************************/
static SQLRETURN create_bom(
    BOM_TASK *bom_task)
{
    SQLRETURN sqlError;
    
    sqlError = sbomPrepareItemStmtForInsert(bom_task);
    if (sqlError == SQL_SUCCESS)
    {
        sqlError = sbomPrepareBillStmtForInsert(bom_task);
        if (sqlError == SQL_SUCCESS)
        {
            sbomGetRandomId(bom_task->szRootId, bom_task);
            strcpy(bom_task->RItem.rid_code, bom_task->szRootId);
            bom_task->RItem.rcomponent_count = bom_task->max_members;
            bom_task->txCounter = 0;
            sqlError = SQLExecute(bom_task->hItemStmt);
            if (sqlError == SQL_SUCCESS)
            {
                sqlError = sbomBuildBill(bom_task);
            }
        }
        SQLCloseCursor(bom_task->hBillStmt); /*lint !e534 */
    }
    SQLCloseCursor(bom_task->hItemStmt); /*lint !e534 */
    SQLEndTran(SQL_HANDLE_DBC, bom_task->hDbc, SQL_COMMIT); /*lint !e534 */

    return sqlError;
}

/*********************************************************************
* sbomBuildBill() : recursive routine to build one level of a bill by
*                   adding components to a parent.
**********************************************************************/
static SQLRETURN sbomBuildBill(
    BOM_TASK *bom_task)
{
    SQLRETURN sqlError = SQL_SUCCESS;
    int32_t i;
    char szParent[16];

    strcpy(szParent, bom_task->RItem.rid_code);

    (bom_task->current_level)++;

    for (i = 0; i < bom_task->max_members && sqlError == SQL_SUCCESS; i++)
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
        if ((sqlError = SQLExecute(bom_task->hItemStmt)) == SQL_SUCCESS)
        {
            /* assign values to bill record, with relational references to
               the parent and component item records - this bill record
               provides a linkage in a many-to-many relationship
            */
            strcpy(bom_task->parent, szParent);
            strcpy(bom_task->rcomponent, bom_task->RItem.rid_code);
            bom_task->RBill.rsequence = i;
            bom_task->RBill.rlevel = bom_task->current_level;

            /* insert new bill record */
            if ((sqlError = SQLExecute(bom_task->hBillStmt)) == SQL_SUCCESS)
            {
                if (++(bom_task->txCounter)%5000 == 0)
                {
                    SQLEndTran(SQL_HANDLE_DBC, bom_task->hDbc, SQL_COMMIT); /*lint !e534 */
                }

                /* if we are not at the bottom of the tree, attach further
                   components to this component item, by recursively calling
                   this function
                */
                if (bom_task->current_level < bom_task->max_levels)
                {
                    sqlError = sbomBuildBill(bom_task);
                }
            }
            else
            {
                sbomDisplaySqlError(bom_task->hBillStmt, bom_task->parent);
            }
        }
        else
        {
            sbomDisplaySqlError(bom_task->hItemStmt, bom_task->RItem.rid_code);
        }
    }
    (bom_task->current_level)--;
    return sqlError;
}

/***************************************************************************
* sbomPrepareItemStmtForInsert() : Prepares item statement for insertion by
*                                  binding parameters to be used later
*****************************************************************************/
static SQLRETURN sbomPrepareItemStmtForInsert(
    BOM_TASK *bom_task)
{
    char        szState[8];
    char        szError[128];
    SQLRETURN   sqlError;
    SQLINTEGER  sqliDiagError = 0;
    SQLSMALLINT sqlsCount;

    sqlError = SQLPrepare(bom_task->hItemStmt, (SQLCHAR *) "insert into ritem values (?,?,?,?)", SQL_NTS);
    if (sqlError != SQL_SUCCESS)
    {
        SQLGetDiagRec(SQL_HANDLE_STMT, bom_task->hItemStmt, 1, (SQLCHAR *)szState, &sqliDiagError, 
            (SQLCHAR *)szError, sizeof(szError), &sqlsCount);
        return sqlError;
    }
    if (sqlError == SQL_SUCCESS)
    {
        sqlError = SQLBindParameter(bom_task->hItemStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 
            sizeof(bom_task->RItem.rid_code), 0, bom_task->RItem.rid_code, SQL_NTS, NULL);
        if (sqlError == SQL_SUCCESS)
        {
            sqlError = SQLBindParameter(bom_task->hItemStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 
                sizeof(bom_task->RItem.rdescription), 0, bom_task->RItem.rdescription, SQL_NTS, NULL);
            if (sqlError == SQL_SUCCESS)
            {
                sqlError = SQLBindParameter(bom_task->hItemStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, 
                    SQL_DOUBLE, 0, 0, &bom_task->RItem.rcost, 0, NULL);
                if (sqlError == SQL_SUCCESS)
                    sqlError = SQLBindParameter(bom_task->hItemStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, 
                    SQL_INTEGER, 0, 0, &bom_task->RItem.rcomponent_count, 0, NULL);
            }
        }
    }
    return sqlError;
}

/***************************************************************************
* sbomPrepareBillStmtForInsert() : Prepares Bill statement for insertion by
*                                  binding parameters to be used later
*****************************************************************************/
static SQLRETURN sbomPrepareBillStmtForInsert(
    BOM_TASK *bom_task)
{
    SQLRETURN sqlError;

    sqlError = SQLPrepare(bom_task->hBillStmt, (SQLCHAR *) "insert into rbill values (?,?,?,?,?)", SQL_NTS);
    if (sqlError == SQL_SUCCESS)
    {
        
        sqlError = SQLBindParameter(bom_task->hBillStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 
             sizeof(bom_task->rcomponent), 0, bom_task->rcomponent, SQL_NTS, NULL);
        if (sqlError == SQL_SUCCESS)
        {      
            sqlError = SQLBindParameter(bom_task->hBillStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 
                sizeof(bom_task->parent), 0, bom_task->parent, SQL_NTS, NULL);
            if (sqlError == SQL_SUCCESS)
            {
                sqlError = SQLBindParameter(bom_task->hBillStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, 
                    SQL_INTEGER, 0, 0, &bom_task->RBill.rsequence, 0, NULL);
                if (sqlError == SQL_SUCCESS)
                {
                    sqlError = SQLBindParameter(bom_task->hBillStmt, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, 
                        SQL_DOUBLE, 0, 0, &bom_task->RBill.rquantity, 0, NULL);
                    if (sqlError == SQL_SUCCESS)
                    {
                        sqlError = SQLBindParameter(bom_task->hBillStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, 
                            SQL_INTEGER, 0, 0, &bom_task->RBill.rlevel, 0, NULL);
                    }
                }
            }
        }
    }
    return sqlError;
}

/***************************************************************************
* rollup_bom() : Top level function for rolling the BOM
*****************************************************************************/
static SQLRETURN rollup_bom(
    double   *dRolledUpCost,
    BOM_TASK *bom_task)
{
    SQLRETURN sqlError;
 
    sqlError = sbomPrepareItemStmtForSelect(bom_task);
    if (sqlError == SQL_SUCCESS)
    {
        strcpy(bom_task->RItem.rid_code, bom_task->szRootId);
        sqlError = sbomGetCost(dRolledUpCost, bom_task);
    }
    SQLCloseCursor(bom_task->hItemStmt); /*lint !e534 */

    return sqlError;
}

/****************************************************************************
* sbomGetCost - recursive routine to roll up cost from lower levels of bill.
*               The costs are stored only at the lowest levels of the bill.
*****************************************************************************/
static SQLRETURN sbomGetCost(
    double   *pdCost,
    BOM_TASK *bom_task)
{
    SQLRETURN  sqlError;
    double     dTotalCost = 0.0; /* for this item and all sub-components */
    double     dCost;          /* for one sub-component */
    double     quantity = 0;
    char       szParent[16];
    SQLCHAR   *stmt;
    SQLHSTMT   hBillSelectStmt;

    strcpy(szParent, bom_task->RItem.rid_code);

    sqlError = SQLAllocHandle(SQL_HANDLE_STMT, bom_task->hDbc, &hBillSelectStmt);
    if (sqlError != SQL_SUCCESS)
    {
        fprintf(stderr, "Error allocating statement handle (hBillSelectStmt)\n");
        return sqlError;
    }

    /* select the item record specified in RItem.id */
    sqlError = SQLExecute(bom_task->hItemStmt);
    if (sqlError == SQL_SUCCESS)
    {
        /* there should only be one item that matches the select criteria */
        sqlError = SQLFetch(bom_task->hItemStmt);
        SQLCloseCursor(bom_task->hItemStmt); /*lint !e534 */
        if (sqlError == SQL_SUCCESS)
        {
            if (bom_task->RItem.rcomponent_count == 0)
            {
                /* no sub-components for this item - return the cost of this
                   bottom-level item
                */
                *pdCost = (double) bom_task->RItem.rcost;
                return sqlError;
            }

            stmt = (SQLCHAR*) psp_makeString("select rcomponent, rquantity from rbill where rparent=\"%s\"", szParent, TAG0);
            sqlError = SQLExecDirect(hBillSelectStmt, stmt, SQL_NTS);
            psp_freeMemory(stmt, TAG0);
            if (sqlError == SQL_SUCCESS)
            {
                SQLBindCol(hBillSelectStmt, 1, SQL_C_CHAR, bom_task->RItem.rid_code, 
                    sizeof(bom_task->RItem.rid_code), &bom_task->lSize);
                SQLBindCol(hBillSelectStmt, 2, SQL_C_DOUBLE, &quantity, sizeof(quantity), &bom_task->lSize);
                while((sqlError = SQLFetch(hBillSelectStmt)) == SQL_SUCCESS)
                {
                    /* get the cost for this component, by recursively calling this function */                    
                    sqlError = sbomGetCost(&dCost, bom_task);
                    dTotalCost += (dCost * quantity);
                }

                /* We should break out of look with a SQL_NO_DATA_FOUND */
                if (sqlError == SQL_NO_DATA_FOUND)
                {
                    sqlError = SQL_SUCCESS;
                }
                else
                {
                    sbomDisplaySqlError(hBillSelectStmt, szParent);
                }
                SQLCloseCursor(hBillSelectStmt); /*lint !e534 */
            }
        }
        else if (sqlError == SQL_NO_DATA_FOUND)
        {
            printf("\nPart number %s not found\n", szParent);
        }
    }
    else
    {
        sbomDisplaySqlError(bom_task->hItemStmt, bom_task->RItem.rid_code);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hBillSelectStmt); /*lint !e534 */
    *pdCost = (double) dTotalCost;
    return sqlError;
}

/***************************************************************************
* sbomPrepareItemStmtForSelect() : Prepares item statement for selection by
*                                  binding parameters to be fetched later
*****************************************************************************/
static SQLRETURN sbomPrepareItemStmtForSelect(
    BOM_TASK *bom_task)
{
    SQLRETURN sqlError;

    sqlError = SQLPrepare(bom_task->hItemStmt, (SQLCHAR *)"select rcomponent_count, rcost from ritem where rid_code=?", SQL_NTS);
    if (sqlError == SQL_SUCCESS)
    {
        sqlError = SQLBindParameter(bom_task->hItemStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 
            sizeof(bom_task->RItem.rid_code), 0, bom_task->RItem.rid_code, SQL_NTS, NULL);
        if (sqlError == SQL_SUCCESS)
        {
            sqlError = SQLBindCol(bom_task->hItemStmt, 1, SQL_C_LONG, &bom_task->RItem.rcomponent_count, 
                sizeof(bom_task->RItem.rcomponent_count), &bom_task->lSize);
            if (sqlError == SQL_SUCCESS)
            {
                sqlError = SQLBindCol(bom_task->hItemStmt, 2, SQL_C_DOUBLE, &bom_task->RItem.rcost, 
                    sizeof(bom_task->RItem.rcost), &bom_task->lSize);
            }
        }
    }
    return sqlError;
}

/*********************************************************************
* bom_initialize() : Create connection to the database and allocate
*                    statement handles
**********************************************************************/
#define CREATEDB    (SQLCHAR *)"CREATE DATABASE sbomODBC"
#define CREATERITEM (SQLCHAR *)"create table ritem (\
                                     rid_code CHAR(16) PRIMARY KEY, \
                                     rdescription CHAR(58), \
                                     rcost DOUBLE, \
                                     rcomponent_count INT)"
#define CREATERBILL (SQLCHAR *)"create table rbill (\
                                     rcomponent CHAR(16), \
                                     rparent CHAR(16), \
                                     rsequence INT, \
                                     rquantity DOUBLE, \
                                     rlevel INT, \
                                     FOREIGN KEY(rcomponent) REFERENCES ritem(rid_code), \
                                     FOREIGN KEY(rparent) REFERENCES ritem(rid_code))"

static SQLRETURN bom_initialize(
    BOM_TASK *bom_task)
{
    char        szState[8];
    char        szError[128];
    SQLRETURN   sqlError;
    SQLINTEGER  sqliDiagError = 0;
    SQLSMALLINT sqlsCount;
    SQLCHAR     szServer[20];
    SQLCHAR     szUserid[20];
    SQLCHAR     szPassword[20];
    SQLHSTMT    hStmt;

    strcpy((char *)szServer, "");
    strcpy((char *)szUserid, "userid");
    strcpy((char *)szPassword, "password");

    /* Allocate an environment handle */
    sqlError = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &bom_task->hEnv);
    if (!SQL_SUCCEEDED(sqlError)) 
    {
        fprintf(stderr, "SQLAllocHandle failed for env handle...\n");
        return sqlError;
    }
    
    /* Allocate a connection handle */
    sqlError = SQLAllocHandle(SQL_HANDLE_DBC, bom_task->hEnv, &bom_task->hDbc);
    if (sqlError != SQL_SUCCESS)
    {
        fprintf(stderr, "SQLAllocHandle failed for conection handle...\n");
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hEnv);
        return sqlError;
    }

    sqlError = SQLSetConnectAttr(bom_task->hDbc, SQL_ATTR_ACCESS_MODE, (SQLCHAR *)SQL_MODE_EXCLUSIVE, SQL_NTS);
    if (sqlError != SQL_SUCCESS)
    {
        fprintf(stderr, "SQLSetConnectAttr failed for ACCESS_MODE...\n");
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
        SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);
        return sqlError;
    }

    sqlError = SQLConnect(bom_task->hDbc, szServer, SQL_NTS, szUserid, SQL_NTS, szPassword, SQL_NTS);
    if (sqlError != SQL_SUCCESS)
    {
        SQLGetDiagRec(SQL_HANDLE_DBC, bom_task->hDbc, 1, (SQLCHAR *)szState, &sqliDiagError, 
            (SQLCHAR *)szError, sizeof(szError), &sqlsCount);
        fprintf(stderr, "SQLConnect failed (%s %s)...\n", szState, szError);
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
        SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);
        return sqlError;
    }

    sqlError = SQLAllocHandle(SQL_HANDLE_STMT, bom_task->hDbc, &hStmt);
    if (sqlError != SQL_SUCCESS)
    {
        fprintf(stderr, "Error allocating statement handle (hItemStmt)\n");
        SQLDisconnect(bom_task->hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
        SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);
        return sqlError;
    }
    sqlError = SQLExecDirect(hStmt, CREATEDB, SQL_NTS);
    if (sqlError == SQL_SUCCESS)
    {
        sqlError = SQLExecDirect(hStmt, CREATERITEM, SQL_NTS);
        if (sqlError == SQL_SUCCESS)
        {
            sqlError = SQLExecDirect(hStmt, CREATERBILL, SQL_NTS);
            if (sqlError == SQL_SUCCESS)
            {
                sqlError = SQLEndTran(SQL_HANDLE_DBC, bom_task->hDbc, SQL_COMMIT); /*lint !e534 */
            }
        }
    }
    if (sqlError != SQL_SUCCESS)
    {
        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLCHAR *)szState, &sqliDiagError, 
            (SQLCHAR *)szError, sizeof(szError), &sqlsCount);
        fprintf(stderr, "create database failed (%s %s)...\n", szState, szError);
        SQLDisconnect(bom_task->hDbc);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
        SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);
        return sqlError;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    sqlError = SQLAllocHandle(SQL_HANDLE_STMT, bom_task->hDbc, &bom_task->hItemStmt);
    if (sqlError != SQL_SUCCESS)
    {
        fprintf(stderr, "Error allocating statement handle (hItemStmt)\n");
        SQLDisconnect(bom_task->hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
        SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);
        return sqlError;
    }

    sqlError = SQLAllocHandle(SQL_HANDLE_STMT, bom_task->hDbc, &bom_task->hBillStmt);
    if (sqlError != SQL_SUCCESS)
    {
        fprintf(stderr, "Error allocating statement handle (hBillStmt)\n");
        SQLFreeHandle(SQL_HANDLE_STMT, bom_task->hItemStmt); /*lint !e534 */
        SQLDisconnect(bom_task->hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
        SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);
        SQLFreeHandle(SQL_HANDLE_STMT, bom_task->hItemStmt); /*lint !e534 */
        return sqlError;
    }
    return sqlError;
}

/*********************************************************************
* bom_cleanup() : Disconnect from the database and free statement 
*                 handles
**********************************************************************/
static SQLRETURN bom_cleanup(
    BOM_TASK *bom_task)
{
    SQLRETURN sqlError;
    char        szState[8];
    char        szError[128];
    SQLINTEGER  sqliDiagError = 0;
    SQLSMALLINT sqlsCount;

    SQLFreeHandle(SQL_HANDLE_STMT, bom_task->hItemStmt); /*lint !e534 */
    SQLFreeHandle(SQL_HANDLE_STMT, bom_task->hBillStmt); /*lint !e534 */

    sqlError = SQLDisconnect(bom_task->hDbc);
    if (sqlError == SQL_ERROR)
    {
        SQLGetDiagRec(SQL_HANDLE_DBC, bom_task->hDbc, 1, (SQLCHAR *)szState,
                &sqliDiagError, (SQLCHAR *)szError, sizeof(szError),
                &sqlsCount); /*lint !e534 */
        if (sqliDiagError == errTXACTIVE)
        {
            SQLEndTran(SQL_HANDLE_DBC, bom_task->hDbc, SQL_ROLLBACK); /*lint !e534 */
            sqlError = SQLDisconnect(bom_task->hDbc);
        }

        if (sqlError == SQL_ERROR)
        {
            SQLGetDiagRec(SQL_HANDLE_DBC, bom_task->hDbc, 1, (SQLCHAR *)szState,
                    &sqliDiagError, (SQLCHAR *)szError, sizeof(szError),
                    &sqlsCount); /*lint !e534 */
            printf("\nSQL Error %d\n\t%s\n\tState %s\n\n",
                    (int32_t)sqliDiagError, szError, szState);
        }
    }

    SQLFreeHandle(SQL_HANDLE_DBC, bom_task->hDbc); /*lint !e534 */
    SQLFreeHandle(SQL_HANDLE_ENV, bom_task->hEnv);

    if(bom_task->cmd_status.stdout_opened)
        psp_stdout_close();
        
    if(bom_task->cmd_status.stderr_opened)
        psp_stderr_close();
    
    return sqlError;
}

/*********************************************************************
* sbomGetRandomId() : generates 15-character alpha part id
**********************************************************************/
static void sbomGetRandomId(
    char     *string,
    BOM_TASK *bom_task)
{
    sprintf(string, "%-15.15d", ++(bom_task->count));
}

/*********************************************************************
* sbomDisplayrsqlError() : Display error information
**********************************************************************/
static void sbomDisplaySqlError(SQLHSTMT hStmt, char *pszId)
{
    char szState[8];
    char szError[128];
    SQLINTEGER sqliError = 0;
    SQLSMALLINT sqlsCount;

    szError[0] = '\0';

    SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLCHAR *)szState, &sqliError,
            (SQLCHAR *)szError, sizeof(szError), &sqlsCount); /*lint !e534 */
    printf("\nSQL Error %d\n\t%s\n\tState %s\n\nItem Id %s\n\n",
            (int32_t) sqliError, szError, szState, pszId);
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
        { ARG_OPT,         ":l",                   "i=#",          "Number of levels (default 5)" },
        { ARG_OPT,         ":m",                   "i=#",          "Number of members per level (default 5)" },
        { ARG_END,         NULL,                   NULL,           NULL }
    };
    /*lint -restore */

    RDM_ASSERT(bom_opts);

    bom_opts->max_levels = 0;
    bom_opts->max_members = 0;

    optstat = b_cmd_initopt(RDM_EXAMPLE_DESC("ODBC SQL BOM Benchmark"),
            argc, argv, opts, NULL, &bom_task->cmd_status);
    if (optstat != B_CMD_OKAY)
        return optstat;

    while ((opt = b_cmd_nextoptstr(opts, &optarg)) != NULL)
    {
        switch (opt[0])
        {
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
PSP_STARTUP_EXAMPLE_CR(sbomODBC)
