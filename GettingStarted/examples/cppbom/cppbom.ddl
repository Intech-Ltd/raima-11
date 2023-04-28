/* ----------------------------------------------------------------------------
 *
 * RDM
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

database cppbom {
    data file "cppbom.d01" contains item;
    data file "cppbom.d02" contains bill;
    key file  "cppbom.k01" contains id_code;

    record item {
        unique key char id_code[16];
        char   description[58];
        double cost;
    }
    record bill {
        double  quantity;
        int32_t level;
    }

    set bom {
        order last;
        owner item;
        member bill;
    }
    set where_used {
        order last;
        owner item;
        member bill;
    }
}
