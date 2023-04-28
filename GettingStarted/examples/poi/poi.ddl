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
    Points of Interest (POI) Database
----------------------------------------------------------------*/

database poi {
    data    file "poi.d00" contains pointsofinterest;
    key     file "poi.k00" contains zvalue;
    key     file "poi.k01" contains city;
    key     file "poi.k02" contains zip;

    record pointsofinterest
    {
        key uint64_t zvalue;
        key char     city[30]; /* data analyzed, and has max length of 26 */
        char         state[3];
        key int32_t  zip;
        int32_t      tz;
    }
}

