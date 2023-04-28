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
#include "mkt_dbd.h"
#include "mkt.h"
#include "market.h"

void batch_create_investors(
    const char *opts,
    DB_TASK    *task)
{
    char  *ptr;
    FILE  *fptr;
    char   line[80];
    double funds;

    if ((fptr = fopen(opts, "r")) == NULL)
    {
        printf("Unable to open file %s\n", opts);
        return;
    }

    while (fgets(line, sizeof(line), fptr))
    {
        /* expecting name,funds */
        if ((ptr = strchr(line, ',')) != NULL)
        {
            *ptr++ = '\0';
            funds = atof(ptr);
            do_create_investor(line, funds, task); /*lint !e534 */
        }
    }

    fclose(fptr);
}

void batch_create_stocks(
    const char *opts,
    DB_TASK    *task)
{
    char    *ptr;
    char    *ptr2;
    FILE    *fptr;
    char     line[80];
    uint64_t nShares;
    double   price;

    if ((fptr = fopen(opts, "r")) == NULL)
    {
        printf("Unable to open file %s\n", opts);
        return;
    }

    while (fgets(line, sizeof(line), fptr))
    {
        /* expecting name,shares,price */
        if ((ptr = strchr(line, ',')) != NULL)
        {
            *ptr++ = '\0';
            if ((ptr2 = strchr(ptr, ',')) != NULL)
            {
                *ptr2++ = '\0';
                nShares = psp_atoui64(ptr);
                price = atof(ptr2);
                do_create_company(line, nShares, price, task); /*lint !e534 */
            }
        }
    }

    fclose(fptr);
}

static void sortInts(
    uint32_t *ints,
    uint32_t  nInts)
{
    uint32_t ii;
    uint32_t jj;
    uint32_t hold;

    /* very simple sort for short list */
    for (ii = 0; ii < nInts - 1; ii++)
    {
        for (jj = 0; jj < nInts - 1; jj++)
        {
            if (ints[jj] > ints[jj + 1])
            {
                /* swap */
                hold = ints[jj];
                ints[jj] = ints[jj+1];
                ints[jj+1] = hold;
            }
        }
    }
}

static const LOCK_REQUEST investor_stock_r[] =
{
    {INVESTOR, 'r'},
    {STOCK, 'r'}
};
static const int32_t investor_stock_r_sz = (int32_t) (sizeof(investor_stock_r) / sizeof(investor_stock_r[0]));

/* Random trading action:
 *
 * The parameter to this function is trades/day.  A day of 8 hours will be
 * comdensed to 8 seconds.  The trade timings are determined by obtaining
 * random times between 0 and 7999, each representing milliseconds into the
 * 8-second day.  The function will sleep between the trade times.  This assumes
 * that the transaction takes 0 milliseconds, but it's close enough.
*/
void batch_trade_stocks(
    const char *opts,
    DB_TASK    *task)
{
    int16_t   invID;
    int16_t   invIDVal;
    int16_t   stkID;
    int16_t   stkIDVal;
    int32_t   rc;
    int32_t   buy;
    int32_t   tmp;
    uint32_t  ii;
    uint32_t  tx_per_day;
    uint32_t  sleep_amount;
    uint32_t *tx_time;
    int64_t   nShares;

    tmp = atoi(opts);
    if (tmp <= 0)
    {
        printf("Must have at least 1 trade per day\n");
        return;
    }

    tx_per_day = (uint32_t) tmp;

    tx_time = psp_getMemory(tx_per_day * sizeof(uint32_t), 0);

    if (d_lock(investor_stock_r_sz, investor_stock_r, task, 0) != S_OKAY)
    {
        printf("Unable to lock INVESTOR and STOCK for reading\n");
        d_freeall(task);
        return;
    }

    /* derive the number of companies and investors */
    rc = d_keylast(STOCK_STKID, task, 0);
    if (rc == S_NOTFOUND)
        printf("Cannot perform trades - no stocks exist.\n");
    else
    {
        if (d_crread(STOCK_STKID, &stkIDVal, task, 0) != S_OKAY)
        {
            printf("Error reading database\n");
            d_freeall(task);
            return;
        }

        rc = d_keylast(INVESTOR_INVID, task, 0);
        if (rc == S_NOTFOUND)
            printf("Cannot perform trades - no investors exist.\n");
        else
        {
            if (d_crread(INVESTOR_INVID, &invIDVal, task, 0) != S_OKAY)
                printf("Error reading database\n");
            else
            {
                /* done with the read locks */
                d_freeall(task);

                /* this will need to be killed to terminate the loop */
                for (;;)
                {
                    /* a new day, generate transaction times */
                    for (ii = 0; ii < tx_per_day; ii++)
                        tx_time[ii] = (uint32_t) rand() % 8000;

                    sortInts(tx_time, tx_per_day);

                    for (ii = 0; ii < tx_per_day; ii++)
                    {
                        if (ii == 0)
                            sleep_amount = tx_time[0];
                        else
                            sleep_amount = tx_time[ii] - tx_time[ii - 1];

                        if (sleep_amount)
                            psp_sleep(sleep_amount);

                        /* slightly more buys than sells */
                        buy = rand() % 100 < 55;

                        /* equal chance for any investor */
                        invID = rand() % invIDVal + 1;
                        stkID = rand() % stkIDVal + 1;

                        /* trade will be rejected if this is invalid amount */
                        nShares = (int64_t) (rand() % 40 + 1);

                        do_stock_transaction(buy, invID, stkID, nShares, task); /*lint !e534 */
                    }
                }
            }
        }
    }

    d_freeall(task);
}

void batch_fluctuate_market(
    const char *opts,
    DB_TASK    *task)
{
    char   *ptr;
    char   *copy = psp_strdup(opts, 0);
    int32_t annual;
    int32_t variance;
    double  daily;

    if ((ptr = strchr(copy, ',')) != NULL)
    {
        *ptr++ = '\0';
        variance = atoi(ptr);
    }
    else
        variance = 3;

    annual = atoi(copy);

    psp_freeMemory(copy, 0);

    daily = (double) annual / 240.0;

    /* this will need to be killed to terminate the loop */
    for (;;)
    {
        do_fluctuate_market(daily, variance, task); /*lint !e534 */
        /* sleep for 1 day */
        psp_sleep(8000);
    }
} /*lint !e954 */

