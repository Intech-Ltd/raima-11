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
**        ddlp -d -c one_to_many.ddl
**
**    Please refer to the Database Design section in the User Manual for details of RDM Database schema.
*/
database one_to_many
{
    data file "one_to_many.d01" contains one;
    data file "one_to_many.d02" contains many;

    record one
    {
        char myChar[32];
    }

    record many
    {
        char myChar[32];
    }

    set relationship 
    {
        order last;
        owner one;
        member many;
    }
}
