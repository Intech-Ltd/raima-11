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
**        ddlp -d -c hybrid_in_memory.ddl
**
**    Please refer to the Database Design section in the User Manual for details of RDM Database schema.
*/
database hybrid_in_memory
{
  data file "hybrid_in_memory.d01" contains on_disk;
  data file "hybrid_in_memory.d02" inmemory volatile contains in_memory;
  
  record on_disk
  {
    char myChar[32];
  }
  
  record in_memory
  {
    char myChar[32];
  }
}

