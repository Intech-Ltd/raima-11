/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2012 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

/*
**    Use following command to compile:
**        ddlp -d -c unique_key.ddl
**
**    Please refer to the Database Design section in the User Manual for details of RDM Database schema.
*/
database unique_key
{
    data file "unique_key.d01" contains info;
    key file "unique_key.k01" contains myChar;

    record info
    {
        unique key char myChar[32];
    }
}