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

database blarc {
    data file "blarc.00" contains file_info;
    key file "blarc.01" contains cat_file;
    blob file[2048] "blarc.02" contains file_contents;

    record file_info {
        char base_name[80];
        char category[80];
        blob_id file_contents;
        compound unique key cat_file {
            category;
            base_name;
        }
    }
}
