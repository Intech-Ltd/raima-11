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

#include "mkt.h"

extern const LOCK_REQUEST investor_fund_r[];
extern const int32_t investor_fund_r_sz;
extern const LOCK_REQUEST investor_fund_asset_trans_stock_r[];
extern const int32_t investor_fund_asset_trans_stock_r_sz;

int32_t check_tx_rate(DB_TASK *);
int32_t create_company(DB_TASK *);
int32_t create_investor(DB_TASK *);
int32_t capitalize_stock(DB_TASK *);
int32_t fund_investor(DB_TASK *);
int32_t buy_stock(DB_TASK *);
int32_t sell_stock(DB_TASK *);
int32_t investor_report(DB_TASK *);
int32_t company_report(DB_TASK *);
int32_t fluctuate_market(DB_TASK *);
char   *getString(const char *, char *, size_t);
int32_t do_create_company(const char *, uint64_t, double, DB_TASK *);
int32_t do_create_investor(const char *, double, DB_TASK *);
void    batch_create_investors(const char *, DB_TASK *);
void    batch_create_stocks(const char *, DB_TASK *);
void    batch_trade_stocks(const char *, DB_TASK *);
void    batch_fluctuate_market(const char *, DB_TASK *);
int32_t do_fluctuate_market(double, int32_t, DB_TASK *);
int32_t do_stock_transaction(int32_t, int16_t, int16_t, int64_t, DB_TASK *);
