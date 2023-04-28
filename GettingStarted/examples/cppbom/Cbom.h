/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#include "cppbom_api.h"

using namespace RDM_CPP;

/* This class provides the functionality for the BOM application */
class Cbom
{
public:
    /* constructor */
    Cbom(int32_t max_level=5, int32_t max_members=5);

    /* Open and initialize the BOM database */
    void     open_database() throw(rdm_exception&);

    /* Create a bom with the request levels and members */
    void     create_bom() throw(rdm_exception&);

    /* Get a rollup of the bom */
    double   rollup_bom() throw(rdm_exception&);

    /* Return the elapsed time of an operations */
    uint64_t get_elapsed_time() const;

private:
    Db_cppbom   bom_db;      // bom database API
    int32_t  current_level;  // What level we are currently at in the BOM
    int32_t  max_level;      // The max level for the BOM we are building
    int32_t  max_members;    // The number of members per BOM entry
    uint64_t start_time;     // Start time of an operations
    uint64_t end_time;       // End time of an operation
    int32_t  txCounter;      // Counter used to keep transaction from being too larg
    int32_t  count;          // Current number of items in the BOM

    /* Create and ID for a BOM item */
    void bom_random_id(char *string);

    /* Recursive function to build a BOM and it's sub-level */
    void   build_bill(Cursor_item&) throw(rdm_exception&);

    /* Recursive function to get cost for a item and it's sub-items */
    double get_cost(Cursor_item&) throw(rdm_exception&);

    /* Create a item in the database */
    Cursor_item  create_item() throw(rdm_exception&);
};
