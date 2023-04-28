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
#include <ctype.h>
#include <time.h>
#include "rdm.h"
#include "mkt_dbd.h"
#include "mkt.h"
#include "market.h"

/* ==================group locks=================== */
static const LOCK_REQUEST txcount_r[] =
{
    {TXCOUNT, 'r'},
};
static const int32_t txcount_r_sz = sizeof(txcount_r) / sizeof(txcount_r[0]);

static const LOCK_REQUEST txcount_w[] =
{
    {TXCOUNT, 'w'},
};
static const int32_t txcount_w_sz = sizeof(txcount_w) / sizeof(txcount_w[0]);

static const LOCK_REQUEST investor_asset_trans_stock_w[] =
{
    {ASSET, 'w'},
    {INVESTOR, 'w'},
    {STOCK, 'w'},
    {TRANS, 'w'},
    {TXCOUNT, 'w'}
};
static const int32_t investor_asset_trans_stock_w_sz = sizeof(investor_asset_trans_stock_w) / sizeof(investor_asset_trans_stock_w[0]);

static const LOCK_REQUEST stock_cap_w[] =
{
    {CAP, 'w'},
    {STOCK, 'w'},
    {TXCOUNT, 'w'}
};
static const int32_t stock_cap_w_sz = sizeof(stock_cap_w) / sizeof(stock_cap_w[0]);

static const LOCK_REQUEST stock_cap_r[] =
{
    {CAP, 'r'},
    {STOCK, 'r'}
};
static const int32_t stock_cap_r_sz = sizeof(stock_cap_r) / sizeof(stock_cap_r[0]);

static const LOCK_REQUEST stock_cap_asset_investor_r[] =
{
    {ASSET, 'r'},
    {CAP, 'r'},
    {INVESTOR, 'r'},
    {STOCK, 'r'}
};
static const int32_t stock_cap_asset_investor_r_sz = sizeof(stock_cap_asset_investor_r) / sizeof(stock_cap_asset_investor_r[0]);

static const LOCK_REQUEST investor_fund_w[] =
{
    {FUND, 'w'},
    {INVESTOR, 'w'},
    {TXCOUNT, 'w'}
};
static const int32_t investor_fund_w_sz = sizeof(investor_fund_w) / sizeof(investor_fund_w[0]);

const LOCK_REQUEST investor_fund_r[] =
{
    {FUND, 'r'},
    {INVESTOR, 'r'}
};
const int32_t investor_fund_r_sz = sizeof(investor_fund_r) / sizeof(investor_fund_r[0]);

const LOCK_REQUEST investor_fund_asset_trans_stock_r[] =
{
    {ASSET, 'r'},
    {FUND, 'r'},
    {INVESTOR, 'r'},
    {STOCK, 'r'},
    {TRANS, 'r'}
};
const int32_t investor_fund_asset_trans_stock_r_sz = sizeof(investor_fund_asset_trans_stock_r) / sizeof(investor_fund_asset_trans_stock_r[0]);

static const LOCK_REQUEST stock_w[] =
{
    {STOCK, 'w'},
    {TXCOUNT, 'w'}
};
static const int32_t stock_w_sz = sizeof(stock_w) / sizeof(stock_w[0]);

/* ===================utility functions=================== */

/* report an unexpected return code */
static int32_t report_err(
    int32_t     rc,
    const char *dbFcn,
    const char *mktFcn,
    DB_TASK    *task)
{
    /* this is harmless if there is no active transaction, and helpful
     * if there is one.
    */
    d_trabort(task);

    printf("*** database error %d returned from %s in %s\n", rc, dbFcn, mktFcn);
    return rc;
}

/* safe gets() */
char *getString(
    const char *prompt,
    char       *s,
    size_t      size)
{
    size_t len;

    if (prompt)
    {
        printf("%s", prompt);
        fflush(stdout);
    }

    if (fgets(s, (int32_t) (uint32_t) size, stdin) == NULL)
        return NULL;

    len = strlen(s);
    if (s[len-1] == '\n')
        s[--len] = '\0';

    if (!len)
        return NULL;

    return s;
}

/* safe get of integer */
static int32_t getInt(
    const char *prompt,
    int32_t    *intVal)
{
    char        intString[20];
    const char *sPtr;

    if (!getString(prompt, intString, sizeof(intString)))
        return 0;

    sPtr = intString;
    while (*sPtr)
    {
        if (!isdigit(*sPtr) && *sPtr != '-')
            return 0;

        sPtr++;
    }

    *intVal = (int32_t)atoi(intString);

    return 1;
}

/* safe get of unsigned 64-bit integer */
static int32_t getUInt64(
    const char *prompt,
    uint64_t   *uint64Val)
{
    char        string[30];
    const char *ptr;

    if (!getString(prompt, string, sizeof(string)))
        return 0;

    for (ptr = string; *ptr; ptr++)
    {
        if (!isdigit(*ptr))
            return 0;
    }

    *uint64Val = psp_atoui64(string);

    return 1;
}

/* safe get of double */
static int32_t getDouble(
    const char *prompt,
    double     *doubVal)
{
    char        doubString[20];
    const char *sPtr;

    if (!getString(prompt, doubString, sizeof(doubString)))
        return 0;

    sPtr = doubString;
    while (*sPtr)
    {
        if (!isdigit(*sPtr) && *sPtr != '.' && *sPtr != '-')
            return 0;

        sPtr++;
    }

    *doubVal = (double)atof(doubString);

    return 1;
}

/* ==============worker functions====================*/
static int32_t inc_count(
    DB_TASK *task)
{
    int32_t        rc;
    const char    *fcn = "inc_count";
    struct txcount txcountRec;

    if ((rc = d_recfrst(TXCOUNT, task, 0)) != S_OKAY)
    {
        if (rc != S_NOTFOUND)
            return report_err(rc, "d_recfrst", fcn, task);

        txcountRec.count = 1;
        if ((rc = d_fillnew(TXCOUNT, &txcountRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_fillnew", fcn, task);
    }
    else
    {
        if ((rc = d_recread(&txcountRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread", fcn, task);

        txcountRec.count++;

        if ((rc = d_recwrite(&txcountRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recwrite", fcn, task);
    }

    return S_OKAY;
}

static int32_t investor_list(
    DB_TASK *task)
{
    int32_t         rc;
    struct investor investorRec;
    const char     *fcn = "investor_list";

    if ((rc = d_lock(investor_fund_r_sz, investor_fund_r, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    printf("ID  Name                    Balance\n");
    printf("--- ----------------------- ----------\n");
    for (rc = d_keyfrst(INVESTOR_INVID, task, 0); rc == S_OKAY;
            rc = d_keynext(INVESTOR_INVID, task, 0))
    {
        if ((rc = d_recread(&investorRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread", fcn, task);

        printf("%3d %-23s $%9.2f\n", investorRec.invID, investorRec.name,
                investorRec.money_mkt);
    }

    if (rc != S_NOTFOUND) /*lint !e850 */
        return report_err(rc, "d_key{first,next}", fcn, task);

    if ((rc = d_freeall(task)) != S_OKAY)
        return report_err(rc, "d_freeall", fcn, task);

    return S_OKAY;
}

static int32_t investor_assets(
    double  *assetsValue,
    DB_TASK *task)
{
    int32_t      rc;
    double       current_value;
    struct asset assetRec;
    struct trans transRec;
    struct stock stockRec;
    const char  *fcn = "investor_assets";

    *assetsValue = 0;

    /* prerequisite: current owner of INV_TRANS */
    for (rc = d_findfm(INV_TRANS, task, 0); rc == S_OKAY;
            rc = d_findnm(INV_TRANS, task, 0))
    {
        if ((rc = d_setor(HISTORY, task, 0)) != S_OKAY)
            return report_err(rc, "d_setor(HISTORY)", fcn, task);

        if ((rc = d_recread(&assetRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(assetRec)", fcn, task);

        if ((rc = d_findco(STOCK_TRANS, task, 0)) != S_OKAY)
            return report_err(rc, "d_findco", fcn, task);

        if ((rc = d_recread(&stockRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(stockRec)", fcn, task);

        printf("ASSET: (%d) %s, currently $%-7.2f\n", stockRec.stkID,
               stockRec.company, stockRec.share_price);
        printf("    Shares owned: "UINT64_SPEC"\n", assetRec.shares_owned);

        for (rc = d_findfm(HISTORY, task, 0); rc == S_OKAY;
                rc = d_findnm(HISTORY, task, 0)) /*lint !e445 */
        {
            if ((rc = d_recread(&transRec, task, 0)) != S_OKAY)
                return report_err(rc, "d_recread(transRec)", fcn, task);

            printf("        %s "INT64_SPEC" @ $%-7.2f\n",
                    transRec.shares > 0 ? "purchased": "sold",
                    transRec.shares > 0 ? transRec.shares : -transRec.shares,
                    transRec.value);
        }

        if (rc != S_EOS) /*lint !e850 */
            return report_err(rc, "d_find{fm,nm}(HISTORY)", fcn, task);

        current_value = (double) assetRec.shares_owned * stockRec.share_price;
        printf("    Current value: $%-7.2f\n", current_value);
        *assetsValue += current_value;
    }

    if (rc != S_EOS) /*lint !e850 */
        return report_err(rc, "d_find{fm,nm}(INV_TRANS)", fcn, task);

    return S_OKAY;
}

static int32_t investor_rpt(
    DB_TASK *task)
{
    int32_t         rc;
    double          total_funds;
    double          assetsValue;
    struct investor investorRec;
    struct fund     fundRec;
    const char     *fcn = "investor_details";

    if ((rc = d_setor(FUNDING, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor(FUNDING)", fcn, task);

    if ((rc = d_setor(INV_TRANS, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor(INV_TRANS)", fcn, task);

    if ((rc = d_recread(&investorRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_recread(investorRec)", fcn, task);

    printf("\nInvestor (%d) %s\n", investorRec.invID, investorRec.name);
    printf("------------------------------------\n");

    printf("FUNDING:\n");
    total_funds = 0;
    for (rc = d_findfm(FUNDING, task, 0); rc == S_OKAY;
            rc = d_findnm(FUNDING, task, 0))
    {
        if ((rc = d_recread(&fundRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(fundRec)", fcn, task);

        printf("    $%-7.2f\n", fundRec.amount);
        total_funds += fundRec.amount;
    }

    if (rc != S_EOS) /*lint !e850 */
        return report_err(rc, "d_find{fm,nm}", fcn, task);

    if ((rc = investor_assets(&assetsValue, task)) != S_OKAY)
        return report_err(rc, "investor_assets", fcn, task);

    printf("Liquid funds: $%-7.2f\n", investorRec.money_mkt);
    printf("Net worth:    $%-8.2f\n", investorRec.money_mkt + assetsValue);
    printf("Profit/Loss:  $%-7.2f\n", investorRec.money_mkt+assetsValue-total_funds);

    return S_OKAY;
}

static int32_t investor_details(
    DB_TASK *task)
{
    int32_t     rc;
    char        cmd[10];
    int16_t     invIDVal;
    const char *fcn = "investor_details";

    while (!getString("All (a) or Specific (#) investor: ", cmd, sizeof(cmd)))
        printf("Enter 'a' or a number.\n");

    rc = d_lock(investor_fund_asset_trans_stock_r_sz,
            investor_fund_asset_trans_stock_r, task, 0);
    if (rc != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    printf("\n===INVESTOR DETAILS===\n");

    if (cmd[0] == 'a' || cmd[0] == 'A')
    {
        for (rc = d_keyfrst(INVESTOR_INVID, task, 0); rc == S_OKAY;
                rc = d_keynext(INVESTOR_INVID, task, 0))
        {
            if ((rc = investor_rpt(task)) != S_OKAY)
            {
                d_freeall(task);
                return report_err(rc, "investor_rpt", fcn, task);
            }
        }

        if (rc != S_NOTFOUND) /*lint !e850 */
        {
            d_freeall(task);
            return report_err(rc, "d_key{first,next}", fcn, task);
        }
    }
    else if (cmd[0] != 'q' && cmd[0] != 'Q')
    {
        /* convert to a number and find it */
        invIDVal = (int16_t) atoi(cmd);
        if ((rc = d_keyfind(INVESTOR_INVID, &invIDVal, task, 0)) != S_OKAY)
        {
            if (rc != S_NOTFOUND)
            {
                d_freeall(task);
                return report_err(rc, "d_keyind", fcn, task);
            }
            else
                printf("Unable to find investor %d.  Try again.\n", invIDVal);
        }
        else if ((rc = investor_rpt(task)) != S_OKAY)
        {
            d_freeall(task);
            return report_err(rc, "investor_rpt", fcn, task);
        }
    }


    if ((rc = d_freeall(task)) != S_OKAY)
        return report_err(rc, "d_freeall", fcn, task);

    return S_OKAY;
}

static int32_t company_investors(
    DB_TASK *task)
{
    int32_t         rc;
    struct asset    assetRec;
    struct investor investorRec;
    const char     *fcn = "company_investors";

    printf("SHAREHOLDERS:\n");

    /* prerequisite: current owner of STOCK_TRANS */
    for (rc = d_findfm(STOCK_TRANS, task, 0); rc == S_OKAY;
            rc = d_findnm(STOCK_TRANS, task, 0))
    {
        if ((rc = d_recread(&assetRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(assetRec)", fcn, task);

        if ((rc = d_findco(INV_TRANS, task, 0)) != S_OKAY)
            return report_err(rc, "d_findco", fcn, task);

        if ((rc = d_recread(&investorRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(stockRec)", fcn, task);

        printf("    (%d) %s, "UINT64_SPEC" shares\n", investorRec.invID, investorRec.name, assetRec.shares_owned);
    }

    if (rc != S_EOS) /*lint !e850 */
        return report_err(rc, "d_find{fm,nm}(STOCK_TRANS)", fcn, task);

    return S_OKAY;
}

static int32_t company_details(
    DB_TASK *task)
{
    int32_t      rc;
    struct stock stockRec;
    struct cap   capRec;
    const char  *fcn = "company_details";

    if ((rc = d_lock(stock_cap_asset_investor_r_sz, stock_cap_asset_investor_r, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    for (rc = d_keyfrst(STOCK_STKID, task, 0); rc == S_OKAY;
            rc = d_keynext(STOCK_STKID, task, 0))
    {
        if ((rc = d_setor(CAPS, task, 0)) != S_OKAY)
            return report_err(rc, "d_setor(CAPS)", fcn, task);

        if ((rc = d_setor(STOCK_TRANS, task, 0)) != S_OKAY)
            return report_err(rc, "d_setor(STOCK_TRANS)", fcn, task);

        if ((rc = d_recread(&stockRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(stockRec)", fcn, task);

        printf("\nCompany (%d) %s\n", stockRec.stkID, stockRec.company);
        printf("------------------------------------\n");

        printf("CAPITALIZATION:\n");
        for (rc = d_findfm(CAPS, task, 0); rc == S_OKAY;
                rc = d_findnm(CAPS, task, 0)) /*lint !e445 */
        {
            if ((rc = d_recread(&capRec, task, 0)) != S_OKAY)
                return report_err(rc, "d_recread(capRec)", fcn, task);

            printf("    "UINT64_SPEC" shares @ $%-7.2f\n", capRec.shares_added, capRec.capitalization);
        }

        if (rc != S_EOS) /*lint !e850 */
            return report_err(rc, "d_find{fm,nm}", fcn, task);

        if ((rc = company_investors(task)) != S_OKAY)
            return report_err(rc, "investor_assets", fcn, task);

        printf("Total shares:     "UINT64_SPEC"\n", stockRec.shares_total);
        printf("Available shares: "UINT64_SPEC"\n", stockRec.shares_available);
        printf("Share Price:      $%-7.2f\n", stockRec.share_price);
        printf("Market Cap:       $%-9.2f\n", stockRec.share_price*(double)stockRec.shares_total);
    }

    if (rc != S_NOTFOUND) /*lint !e850 */
        return report_err(rc, "d_key{first,next}", fcn, task);

    if ((rc = d_freeall(task)) != S_OKAY)
        return report_err(rc, "d_freeall", fcn, task);

    return S_OKAY;
}

static int32_t company_list(
    DB_TASK *task)
{
    int32_t      rc;
    struct stock stockRec;
    const char  *fcn = "company_list";

    if ((rc = d_lock(stock_cap_r_sz, stock_cap_r, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    printf("                            Available   Total      Share\n");
    printf("ID  Company Name            Shares      Shares     Price\n");
    printf("--- ----------------------- ----------- ---------- -------\n");

    for (rc = d_keyfrst(STOCK_STKID, task, 0); rc == S_OKAY;
            rc = d_keynext(STOCK_STKID, task, 0))
    {
        if ((rc = d_recread(&stockRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread", fcn, task);

        printf("%3d %-23s "UINT64_SPEC_SIZE("11")" "UINT64_SPEC_SIZE("10")" $%6.2f\n",
                stockRec.stkID, stockRec.company, stockRec.shares_available,
                stockRec.shares_total, stockRec.share_price);
    }

    if (rc != S_NOTFOUND) /*lint !e850 */
        return report_err(rc, "d_key{first,next}", fcn, task);

    if ((rc = d_freeall(task)) != S_OKAY)
        return report_err(rc, "d_freeall", fcn, task);

    return S_OKAY;
}

int32_t do_create_company(
    const char *company,
    uint64_t    shares_total,
    double      share_price,
    DB_TASK    *task)
{
    int32_t      rc;
    struct stock stockRec;
    struct cap   capRec;
    const char  *fcn = "do_create_company";

    strcpy(stockRec.company, company);
    stockRec.shares_total = shares_total;
    stockRec.share_price = share_price;
    stockRec.shares_available = stockRec.shares_total;

    capRec.shares_added = stockRec.shares_total;
    capRec.capitalization = (double)capRec.shares_added * stockRec.share_price;

    if ((rc = d_trbegin("stock_cap", task)) != S_OKAY)
        return report_err(rc, "d_trbegin", fcn, task);

    if ((rc = d_lock(stock_cap_w_sz, stock_cap_w, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    /* assign a stkID as 1 greater than the last */
    rc = d_keylast(STOCK_STKID, task, 0);
    if (rc == S_NOTFOUND)
        stockRec.stkID = 1;
    else
    {
        if ((rc = d_crread(STOCK_STKID, &stockRec.stkID, task, 0)) != S_OKAY)
            return report_err(rc, "d_crread", fcn, task);

        stockRec.stkID++;
    }

    /* new stock record */
    if ((rc = d_fillnew(STOCK, &stockRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew(STOCK)", fcn, task);

    if ((rc = d_setor(CAPS, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    if ((rc = d_fillnew(CAP, &capRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew(CAP)", fcn, task);

    if ((rc = d_connect(CAPS, task, 0)) != S_OKAY)
        return report_err(rc, "d_connect", fcn, task);

    if ((rc=inc_count(task)) != S_OKAY)
        return report_err(rc, "inc_count", fcn, task);

    if ((rc = d_trend(task)) != S_OKAY)
        return report_err(rc, "d_trend", fcn, task);

    printf("Successful transaction:\n");
    printf("    New stock for %s created with "UINT64_SPEC" shares valued at $%.2f each.\n",
           stockRec.company, stockRec.shares_total, stockRec.share_price);

    return S_OKAY;
}

int32_t do_create_investor(
    const char *name,
    double      money_mkt,
    DB_TASK    *task)
{
    int32_t         rc;
    const char     *fcn = "do_create_investor";
    struct investor investorRec;
    struct fund     fundRec;

    strcpy(investorRec.name, name);
    investorRec.money_mkt = money_mkt;

    fundRec.amount = investorRec.money_mkt;

    if ((rc = d_trbegin("investor_fund", task)) != S_OKAY)
        return report_err(rc, "d_trbegin", fcn, task);

    if ((rc = d_lock(investor_fund_w_sz, investor_fund_w, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    /* assign a invID as 1 greater than the last */
    rc = d_keylast(INVESTOR_INVID, task, 0);
    if (rc == S_NOTFOUND)
        investorRec.invID = 1;
    else
    {
        if ((rc = d_crread(INVESTOR_INVID, &investorRec.invID, task, 0)) != S_OKAY)
            return report_err(rc, "d_crread", fcn, task);

        investorRec.invID++;
    }

    /* new investor record */
    if ((rc = d_fillnew(INVESTOR, &investorRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew(INVESTOR)", fcn, task);

    if ((rc = d_setor(FUNDING, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    if ((rc = d_fillnew(FUND, &fundRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew(FUND)", fcn, task);

    if ((rc = d_connect(FUNDING, task, 0)) != S_OKAY)
        return report_err(rc, "d_connect", fcn, task);

    if ((rc=inc_count(task)) != S_OKAY)
        return report_err(rc, "inc_count", fcn, task);

    if ((rc = d_trend(task)) != S_OKAY)
        return report_err(rc, "d_trend", fcn, task);

    printf("Successful transaction:\n");
    printf("    New investor named %s created with $%.2f to spend.\n",
           investorRec.name, investorRec.money_mkt);

    return S_OKAY;
}

int32_t do_stock_transaction(
    int32_t  buy,
    int16_t  invIDVal,
    int16_t  stkIDVal,
    int64_t  nShares,
    DB_TASK *task)
{
    int32_t     rc;
    struct      asset assetRec;
    struct      trans transRec;
    struct      asset_inv_stock inv_stockKey;
    uint64_t    shares_availableVal;
    uint64_t    shares_ownedVal;
    double      share_priceVal;
    double      money_mktVal;
    double      purchase_price;
    char        nameVal[SIZEOF_INVESTOR_NAME];
    char        companyVal[SIZEOF_STOCK_COMPANY];
    const char *fcn = "do_stock_transaction";

    /* invert nShares if a sale */
    if (!buy)
        nShares = -nShares;

    /* validate the numbers and execute the transaction */
    if ((rc = d_trbegin("investor_asset_trans_stock", task)) != S_OKAY)
        return report_err(rc, "d_trbegin", fcn, task);

    if ((rc = d_lock(investor_asset_trans_stock_w_sz,
            investor_asset_trans_stock_w, task, 0)) != S_OKAY)
    {
        return report_err(rc, "d_lock", fcn, task);
    }

    /* need to verify that the shares (still) exist */
    if ((rc = d_keyfind(STOCK_STKID, &stkIDVal, task, 0)) != S_OKAY)
    {
        if (rc != S_NOTFOUND)
            return report_err(rc, "d_keyfind", fcn, task);

        d_trabort(task);
        printf("Invalid stock number: %d.  Try again.\n", stkIDVal);
        return S_OKAY;
    }

    if ((rc = d_setor(STOCK_TRANS, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    if ((rc = d_crread(STOCK_SHARE_PRICE, &share_priceVal, task, 0)) != S_OKAY)
        return report_err(rc, "d_crread", fcn, task);

    if ((rc = d_crread(STOCK_SHARES_AVAILABLE, &shares_availableVal, task, 0)) != S_OKAY)
        return report_err(rc, "d_crread", fcn, task);

    if (buy && ((uint64_t) nShares > shares_availableVal))
    {
        d_trabort(task);
        printf(INT64_SPEC " shares are not available. Try again.\n", nShares);
        return S_OKAY;
    }

    /* need to verify that the money exists */
    if ((rc = d_keyfind(INVESTOR_INVID, &invIDVal, task, 0)) != S_OKAY)
    {
        if (rc != S_NOTFOUND)
            return report_err(rc, "d_keyfind", fcn, task);

        d_trabort(task);
        printf("Invalid investor number: %d.  Try again.\n", invIDVal);
        return S_OKAY;
    }

    if ((rc = d_setor(INV_TRANS, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    if ((rc = d_crread(INVESTOR_MONEY_MKT, &money_mktVal, task, 0)) != S_OKAY)
        return report_err(rc, "d_crread", fcn, task);

    purchase_price = (double)nShares*share_priceVal;
    if (buy && (money_mktVal < purchase_price))
    {
        d_trabort(task);
        printf("Transaction would cost $%-7.2f, but investor has only $%-7.2f. Try again.\n",
               purchase_price, money_mktVal);
        return S_OKAY;
    }

    /* if this investor has purchased this stock already, simply update it */
    inv_stockKey.iID = invIDVal;
    inv_stockKey.sID = stkIDVal;
    if ((rc = d_keyfind(ASSET_INV_STOCK, &inv_stockKey, task, 0)) == S_NOTFOUND)
    {
        if (!buy)
        {
            d_trabort(task);
            printf("Investor has no shares to sell.  Try again.\n");
            return S_OKAY;
        }

        /* create and attach a new ASSET record */
        assetRec.shares_owned = (uint64_t) nShares;
        assetRec.iID = invIDVal;
        assetRec.sID = stkIDVal;
        if ((rc = d_fillnew(ASSET, &assetRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_fillnew(ASSET)", fcn, task);

        if ((rc = d_connect(INV_TRANS, task, 0)) != S_OKAY)
            return report_err(rc, "d_connect", fcn, task);

        if ((rc = d_connect(STOCK_TRANS, task, 0)) != S_OKAY)
            return report_err(rc, "d_connect", fcn, task);
    }
    else if (rc == S_OKAY)
    {
        /* read the existing ASSET's shares */
        if ((rc = d_crread(ASSET_SHARES_OWNED, &shares_ownedVal, task, 0)) != S_OKAY)
            return report_err(rc, "d_crread", fcn, task);

        if (!buy && shares_ownedVal < (uint64_t) (-nShares))
        {
            d_trabort(task);
            printf("Investor does not own that many shares.  Try again.\n");
            return S_OKAY;
        }

        shares_ownedVal += (uint64_t) nShares;
        if ((rc = d_crwrite(ASSET_SHARES_OWNED, &shares_ownedVal, task, 0)) != S_OKAY)
            return report_err(rc, "d_crwrite", fcn, task);
    }
    else
        return report_err(rc, "d_keyfind", fcn, task);

    /* ASSET is the current record. Create new TRANS record */
    if ((rc = d_setor(HISTORY, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    transRec.shares = nShares;
    transRec.value = share_priceVal;
    if ((rc = d_fillnew(TRANS, &transRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew(TRANS)", fcn, task);

    if ((rc = d_connect(HISTORY, task, 0)) != S_OKAY)
        return report_err(rc, "d_connect", fcn, task);

    /* update fields in INVESTOR and STOCK */
    money_mktVal -= purchase_price;
    shares_availableVal -= (uint64_t) nShares;
    if ((rc = d_csowrite(INV_TRANS, INVESTOR_MONEY_MKT, &money_mktVal, task, 0)) != S_OKAY)
        return report_err(rc, "d_csowrite", fcn, task);

    if ((rc = d_csowrite(STOCK_TRANS, STOCK_SHARES_AVAILABLE, &shares_availableVal, task, 0)) != S_OKAY)
        return report_err(rc, "d_csowrite", fcn, task);

    /* prepare to summarize transaction */
    d_csoread(INV_TRANS, INVESTOR_NAME, nameVal, task, 0);
    d_csoread(STOCK_TRANS, STOCK_COMPANY, companyVal, task, 0);

    if ((rc = inc_count(task)) != S_OKAY)
        return report_err(rc, "inc_count", fcn, task);

    /* successful transaction */
    if ((rc = d_trend(task)) != S_OKAY)
        return report_err(rc, "d_trend", fcn, task);

    /* be polite and report this after the d_trend */
    printf("Successful transaction:\n");
    if (buy)
    {
        printf("    "INT64_SPEC" shares of %s purchased by %s for $%-7.2f\n", nShares, companyVal,
                nameVal, purchase_price);
    }
    else
    {
        printf("    "INT64_SPEC" shares of %s sold by %s for $%-7.2f\n", -nShares, companyVal,
                nameVal, -purchase_price);
    }

    return S_OKAY;
}

static int32_t stock_transaction(
    int32_t  buy,
    DB_TASK *task)
{
    int32_t     rc;
    int16_t     invIDVal;
    int16_t     stkIDVal;
    int32_t     intval;
    uint64_t    nShares;
    const char *fcn = "stock_transactions";

    if ((rc = investor_list(task)) != S_OKAY)
        return rc;

    if (!getInt("Select investor (by number): ", &intval))
        return S_OKAY;

    invIDVal = (int16_t)intval;
    if ((rc = company_list(task)) != S_OKAY)
        return rc;

    if (!getInt("Select a stock (by number): ", &intval))
        return S_OKAY;

    stkIDVal = (int16_t)intval;
    do
    {
        if (!getUInt64(buy ? "Purchasing how may shares? " : "Selling how may shares? ", &nShares))
            return S_OKAY;
    }
    while (nShares == 0);

    if ((rc = do_stock_transaction(buy, invIDVal, stkIDVal, (int64_t) nShares, task)) != S_OKAY)
        return report_err(rc, "do_stock_transaction", fcn, task);

    return S_OKAY;
}

int32_t do_fluctuate_market(
    double   pct,
    int32_t  variance,
    DB_TASK *task)
{
    int32_t      rc;
    int32_t      r;
    struct stock stockRec;
    const char  *fcn = "do_fluctuate_market";

    printf("Adjusting stock prices by %2.2f percent with variance %d.\n", pct, variance);

    pct /= 100.0;

    if ((rc = d_trbegin("stock", task)) != S_OKAY)
        return report_err(rc, "d_trbegin", fcn, task);

    if ((rc = d_lock(stock_w_sz, stock_w, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    /* adjust the share price of each stock */
    for (rc = d_keyfrst(STOCK_STKID, task, 0); rc == S_OKAY;
            rc = d_keynext(STOCK_STKID, task, 0))
    {
        if ((rc = d_recread(&stockRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread(stockRec)", fcn, task);

        printf("    %s, current value $%-7.2f, new value ", stockRec.company, stockRec.share_price);
        /* start with straight adjustment */
        stockRec.share_price *= 1.0 + pct;

        /* now create a modification (either way) on that */
        r = ((rand() % (2 * variance + 1)) - variance);

        stockRec.share_price *= 1.0 + r / 100.0;

        if ((rc = d_recwrite(&stockRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recwrite(stockRec)", fcn, task);

        printf("$%-7.2f.\n", stockRec.share_price);
    }

    if (rc != S_NOTFOUND) /*lint !e850 */
        return report_err(rc, "d_key{first,next}", fcn, task);

    if ((rc = inc_count(task)) != S_OKAY)
        return report_err(rc, "inc_count", fcn, task);

    /* successful transaction */
    if ((rc = d_trend(task)) != S_OKAY)
        return report_err(rc, "d_trend", fcn, task);

    return S_OKAY;
}

static time_t   last_time = 0; /*lint !e956 */
static uint64_t last_count = 0; /*lint !e956 */

/* ===================database functions=================== */

/* sample the transaction counter */
int32_t check_tx_rate(
    DB_TASK *task)
{
    int32_t        rc;
    uint64_t       new_count;
    time_t         new_time;
    struct txcount txcountRec;
    const char    *fcn = "check_tx_rate";

    if ((rc = d_lock(txcount_r_sz, txcount_r, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    if ((rc = d_recfrst(TXCOUNT, task, 0)) == S_NOTFOUND)
    {
        /* new record */
        last_time = time(NULL);
        last_count = 0;
        txcountRec.count = 0;
        if ((rc = d_trbegin("check_tx_rate", task)) != S_OKAY)
            return report_err(rc, "d_trbegin", fcn, task);

        if ((rc = d_lock(txcount_w_sz, txcount_w, task, 0)) != S_OKAY)
            return report_err(rc, "d_lock", fcn, task);

        if ((rc = d_fillnew(TXCOUNT, &txcountRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_fillnew", fcn, task);

        d_trend(task);
        printf("Started counting transactions.\n");
    }
    else if (rc == S_OKAY)
    {
        if ((rc = d_recread(&txcountRec, task, 0)) != S_OKAY)
            return report_err(rc, "d_recread", fcn, task);

        if (last_time == 0)
            printf("Started counting transactions.\n");
        else
        {
            new_count = txcountRec.count - last_count;
            new_time = time(NULL) - last_time;
            printf(UINT64_SPEC" transactions in %d seconds = %.2f tx/sec.\n", new_count,
                (int32_t)new_time, (double)new_count/(double)new_time);
        }

        last_time = time(NULL);
        last_count = txcountRec.count;
    }
    else
        return report_err(rc, "d_recfrst", fcn, task);

    d_freeall(task);

    return S_OKAY;
}

/* create a new company and give it the initial capitalization */
int32_t create_company(
    DB_TASK *task)
{
    int32_t     rc;
    uint64_t    shares_total;
    double      share_price;
    char        company[SIZEOF_STOCK_COMPANY];
    const char *fcn = "create_company";

    if (!getString("Company name: ", company, SIZEOF_STOCK_COMPANY))
        return S_OKAY;

    do
    {
        if (!getUInt64("Initial number of shares: ", &shares_total))
            return S_OKAY;
    }
    while (shares_total == 0);

    do
    {
        if (!getDouble("Initial price of shares: ", &share_price))
            return S_OKAY;
    }
    while (share_price <= 0.0);

    if ((rc = do_create_company(company, shares_total, share_price, task)) != S_OKAY)
        return report_err(rc, "do_create_company", fcn, task);

    return S_OKAY;
}

/* create a new investor and the initial funding */
int32_t create_investor(
    DB_TASK *task)
{
    int32_t rc;
    double  money_mkt;
    char    name[SIZEOF_INVESTOR_NAME];

    if (!getString("Investor name: ", name, SIZEOF_INVESTOR_NAME))
        return S_OKAY;

    do
    {
        if (!getDouble("Initial funding: ", &money_mkt))
            return S_OKAY;
    }
    while (money_mkt <= 0.0);

    if ((rc = do_create_investor(name, money_mkt, task)) != S_OKAY)
        return rc;

    return S_OKAY;
}

/* add capitalization to this company */
int32_t capitalize_stock(
    DB_TASK *task)
{
    int32_t      rc;
    int16_t      stkIDVal;
    int32_t      intval;
    struct cap   capRec;
    struct stock stockRec;
    const char  *fcn = "capitalize_stock";

    if ((rc = company_list(task)) != S_OKAY)
        return rc;

    if (!getInt("Select a stock (by number): ", &intval))
        return S_OKAY;

    stkIDVal = (int16_t)intval;
    do
    {
        if (!getUInt64("Number of shares to add: ", &capRec.shares_added))
            return S_OKAY;
    }
    while (capRec.shares_added == 0);

    if ((rc = d_trbegin("capitalize_stock", task)) != S_OKAY)
        return report_err(rc, "d_trbegin", fcn, task);

    if ((rc = d_lock(stock_cap_w_sz, stock_cap_w, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    /* add the CAP record, connected to the STOCK */
    if ((rc = d_keyfind(STOCK_STKID, &stkIDVal, task, 0)) != S_OKAY)
    {
        if (rc != S_NOTFOUND)
            return report_err(rc, "d_keyfind", fcn, task);

        d_trabort(task);
        printf("Invalid stock number: %d.  Try again.\n", stkIDVal);
        return S_OKAY;
    }

    if ((rc = d_setor(CAPS, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    if ((rc = d_recread(&stockRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_recread()", fcn, task);

    stockRec.shares_available += capRec.shares_added;
    stockRec.shares_total += capRec.shares_added;
    if ((rc = d_recwrite(&stockRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_recwrite()", fcn, task);

    capRec.capitalization = (double) capRec.shares_added * stockRec.share_price;
    if ((rc = d_fillnew(CAP, &capRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew", fcn, task);

    if ((rc = d_connect(CAPS, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew", fcn, task);

    if ((rc=inc_count(task)) != S_OKAY)
        return report_err(rc, "inc_count", fcn, task);

    /* successful transaction */
    if ((rc = d_trend(task)) != S_OKAY)
        return report_err(rc, "d_trend", fcn, task);

    printf("Successful transaction:\n");
    printf("    New capital invested in %s, "UINT64_SPEC" shares costing $%-7.2f each.\n",
           stockRec.company, capRec.shares_added, stockRec.share_price);

    return S_OKAY;
}

/* transfer more funds to this investor */
int32_t fund_investor(
    DB_TASK *task)
{
    int32_t         rc;
    int16_t         invIDVal;
    int32_t         intval;
    struct fund     fundRec;
    struct investor investorRec;
    const char     *fcn = "fund_investor";

    if ((rc = investor_list(task)) != S_OKAY)
        return rc;

    if (!getInt("Select an investor (by number): ", &intval))
        return S_OKAY;

    invIDVal = (int16_t)intval;
    do
    {
        if (!getDouble("Dollar amount to add to investor's money market account: ",
                &fundRec.amount))
        {
            return S_OKAY;
        }
    }
    while (fundRec.amount <= 0.0);

    if ((rc = d_trbegin("fund_investor", task)) != S_OKAY)
        return report_err(rc, "d_trbegin", fcn, task);

    if ((rc = d_lock(investor_fund_w_sz, investor_fund_w, task, 0)) != S_OKAY)
        return report_err(rc, "d_lock", fcn, task);

    /* add the FUND record, connected to the INVESTOR */
    if ((rc = d_keyfind(INVESTOR_INVID, &invIDVal, task, 0)) != S_OKAY)
    {
        if (rc != S_NOTFOUND)
            return report_err(rc, "d_keyfind", fcn, task);

        d_trabort(task);
        printf("Invalid investor number: %d.  Try again.\n", invIDVal);
        return S_OKAY;
    }

    if ((rc = d_setor(FUNDING, task, 0)) != S_OKAY)
        return report_err(rc, "d_setor", fcn, task);

    if ((rc = d_recread(&investorRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_recread()", fcn, task);

    investorRec.money_mkt += fundRec.amount;
    if ((rc = d_recwrite(&investorRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_recwrite()", fcn, task);

    if ((rc = d_fillnew(FUND, &fundRec, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew", fcn, task);

    if ((rc = d_connect(FUNDING, task, 0)) != S_OKAY)
        return report_err(rc, "d_fillnew", fcn, task);

    if ((rc = inc_count(task)) != S_OKAY)
        return report_err(rc, "inc_count", fcn, task);

    /* successful transaction */
    if ((rc = d_trend(task)) != S_OKAY)
        return report_err(rc, "d_trend", fcn, task);

    printf("Successful transaction:\n");
    printf("    New funds transfered to %s, $%-7.2f.\n", investorRec.name, fundRec.amount);

    return S_OKAY;
}

/* investor to buy stock in a company */
int32_t buy_stock(
    DB_TASK *task)
{
    int32_t rc;

    if ((rc = stock_transaction(1, task)) != S_OKAY)
        return report_err(rc, "stock_transaction", "buy_stock", task);

    return S_OKAY;
}

/* investor to sell stock in a company */
int32_t sell_stock(
    DB_TASK *task)
{
    int32_t rc;

    if ((rc = stock_transaction(0, task)) != S_OKAY)
        return report_err(rc, "stock_transaction", "sell_stock", task);

    return S_OKAY;
}

/* report on investor's history and holdings */
int32_t investor_report(
    DB_TASK *task)
{
    int32_t     rc;
    char        cmd[5];
    const char *fcn = "investor_report";

    printf("\n===INVESTOR SUMMARY===\n\n");
    if ((rc = investor_list(task)) != S_OKAY)
        return rc;

    while (!getString("Detailed report? (y/n) ", cmd, sizeof(cmd)))
        printf("Enter y or n.\n");

    if (cmd[0] == 'y' || cmd[0] == 'Y')
    {
        if ((rc = investor_details(task)) != S_OKAY)
            return report_err(rc, "investor_details", fcn, task);
    }

    return S_OKAY;
}

/* report on stock sales and capitalization */
int32_t company_report(
    DB_TASK *task)
{
    int32_t     rc;
    char        cmd[5];
    const char *fcn = "company_report";

    printf("\n===COMPANIES===\n\n");
    if ((rc = company_list(task)) != S_OKAY)
        return rc;

    while (!getString("Detailed report? (y/n) ", cmd, sizeof(cmd)))
        printf("Enter y or n.\n");

    if (cmd[0] == 'y' || cmd[0] == 'Y')
    {
        printf("\n===COMPANY DETAILS===\n");
        if ((rc = company_details(task)) != S_OKAY)
            return report_err(rc, "company_details", fcn, task);
    }

    return S_OKAY;
}

/* "random" fluctuations in stock prices */
int32_t fluctuate_market(
    DB_TASK *task)
{
    int32_t     rc;
    double      pct;
    const char *fcn = "fluctuate_market";

    do
    {
        if (!getDouble("Average percentage increace/decrease for this period: ", &pct))
            return S_OKAY;
    }
    while (pct < -100.0 || pct > 100.0);

    if ((rc = do_fluctuate_market(pct,5, task)) != S_OKAY)
        return report_err(rc, "do_fluctuate_market", fcn, task);

    return S_OKAY;
}

