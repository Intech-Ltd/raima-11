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
**        ddlp -d -c hello_world.ddl
**
**    Please refer to the Database Design section in the User Manual for details of RDM Database schema.
*/
database hello_world {
    data file "hello_world.d01" contains info;

    record info{
        char myChar[32];
    }
}
