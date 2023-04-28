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

/*----------------------------------------------------------------
  Marketplace database
----------------------------------------------------------------*/
database mkt {
    data file "mkt.d01" contains investor;
    data file "mkt.d02" contains stock;
    data file "mkt.d03" contains asset;
    data file "mkt.d04" contains fund;
    data file "mkt.d05" contains trans;
    data file "mkt.d06" contains cap;
    data file "mkt.d07" contains txcount;
    key  file "mkt.k01" contains invID, stkID;
    hash file "mkt.h01" size = 100000 contains inv_stock;

    record investor {
        double  money_mkt;         /* money not currently invested */
        char    name[32];          /* investor name */
        unique  key short invID;
    }
    record stock {
        uint64_t shares_available;  /* shares available for purchase */
        uint64_t shares_total;      /* total shares in company */
        double   share_price;       /* current share price */
        char     company[32];       /* company name */
        unique   key short stkID;
    }
    record asset {
        uint64_t shares_owned;      /* total shares currently owned by investor */
        short    iID;
        short    sID;
        compound hash inv_stock {
            iID;
            sID;
        }
    }
    record fund {
        double   amount;            /* money added/subtracted from investors funds */
    }
    record trans {
        int64_t  shares;            /* purchase (+) or sale (-) quantity */
        double   value;             /* stock value at moment of purchase/sale */
    }
    record cap {
        uint64_t shares_added;      /* number of shares added to total */
        double   capitalization;    /* market capitalization increase */
    }

    record txcount{
        uint64_t count;
    }

    /* asset is intersection between investor and stock */
    set inv_trans {
        order last;
        owner investor;
        member asset;
    }
    set stock_trans {
        order last;
        owner stock;
        member asset;
    }

    /* log of transfers in/out of investor's money market investment fund */
    set funding {
        order last;
        owner investor;
        member fund;
    }

    /* transaction history of purchases/sales of this stock */
    set history {
        order last;
        owner asset;
        member trans;
    }

    /* capitalization of company */
    set caps {
        order last;
        owner stock;
        member cap;
    }
}

