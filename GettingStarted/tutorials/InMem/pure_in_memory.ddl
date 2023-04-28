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
**        ddlp -d -c pure_in_memory.ddl
**
**    Please refer to the Database Design section in the User Manual for details of RDM Database schema.
*/
database pure_in_memory inmemory
{
    data file "pure_in_memory.d01" contains info;

    record info
    {
        char myChar[32];
    }
}

