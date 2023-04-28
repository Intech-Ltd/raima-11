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

#include <iostream>
#include "rdm.h"
#include "Cbom.h"

/* Constructor */
Cbom::Cbom(int32_t max_level, int32_t max_member) :
    current_level(0),
    max_level(max_level),
    max_members(max_member),
    start_time(0),
    end_time(0),
    txCounter(0),
    count(0)
{
}

/* Open the database and initialize it */
void Cbom::open_database() throw (rdm_exception&)
{
    bom_db = bom_db.Open("cppbom");
    bom_db.Initialize();    
}

/* Return the elapsed time for a BOM operations */
uint64_t Cbom::get_elapsed_time() const
{
    return end_time - start_time;
}

/* Create a new BOM Item record */
Cursor_item Cbom::create_item() throw (rdm_exception&)
{
    Cursor_item ci = bom_db.Get_item_recordsBy_id_code();
    struct item item_rec;

    /* Check to see if the id we selected is already in the database */
    bom_random_id(item_rec.id_code);
    if(ci.AtRecord())
    {
        ci.KeyFindBy_id_code(item_rec.id_code);
        while(ci.AtRecord())
        {
            bom_random_id(item_rec.id_code);
            ci.KeyFindBy_id_code(item_rec.id_code);
        }
    }

    /* We have a unique id so we should be good to insert */
    try
    {
        item_rec.cost = 1.0L;
        item_rec.description[0] = '\0';
        ci = bom_db.New_item_recordWithFieldValues(&item_rec);
    }
    catch (rdm_exception& e)
    {
        throw e;
    }
    return ci;
}

/* Top level function for creating a full BOM */
void Cbom::create_bom() throw (rdm_exception&)
{
    Cursor_item  ci;

    start_time = psp_timeMilliSecs();
    try
    {
        /* Start transaction */
        bom_db.BeginUpdate();
        
        /* create top level ITEM record */
        ci = create_item();

        /* Create the rest of the roll-up */
        build_bill(ci);
    }
    catch (rdm_exception& e)
    {
        bom_db.Abort();
        cerr << "Error " << e.error() << " (" << e.what() << ") creating BOM" << endl;
    }
    bom_db.End();
    end_time = psp_timeMilliSecs();
    
}

/* Top-level function for doing a rollup of a BOM item */
double Cbom::rollup_bom() throw (rdm_exception&)
{
    Cursor_item  ci;
    double       rolled_up_cost;
    
    
    start_time = psp_timeMilliSecs();
    try
    {
        bom_db.BeginRead();
        ci = bom_db.Get_item_recordsBy_id_code();
        rolled_up_cost = get_cost(ci);
    }
    catch (rdm_exception& e)
    {
        bom_db.End();
        cerr << "Error " << e.error() << " (" << e.what() << ") rolling up bom" << endl;
    }
    bom_db.End();
    end_time = psp_timeMilliSecs();
    
    return rolled_up_cost;
}

/* Internal recursive function for creating a bom and sub-items */
void Cbom::build_bill(Cursor_item& ci) throw (rdm_exception&)
{
    Cursor_item new_ci;
    Cursor_bill new_cb;
    struct bill bill_rec;
    int32_t i;

    current_level++;

    for (i=0; i<max_members; i++) 
    {
        new_ci = create_item();

        bill_rec.level = current_level;
        bill_rec.quantity = 1.0L;
        new_cb = bom_db.New_bill_recordWithFieldValues(&bill_rec);
        new_cb.Connect_bom(ci);
        new_cb.Connect_where_used(new_ci);

        txCounter++;
        if (txCounter%5000 == 0) 
        {
            bom_db.End();
            bom_db.BeginUpdate();
        }

        /* if we are not at the bottom level, recurse down another level */
        if (current_level < max_level) 
        {
            build_bill(new_ci);
        }
    }
    current_level--;
}

/* Internal recursive function for calculating the cost for an item and it's subitems */
double Cbom::get_cost(Cursor_item& ci) throw (rdm_exception&)
{
    double total_cost;    /* for this item and below */
    int32_t member_count;
    struct bill bill_rec;
    struct item item_rec;
    Cursor_bill cb_bom_members;
    Cursor_item ci_where_used_owner;

    member_count = ci.Get_bom_count();

    /* no members - we are at the bottom level now */
    if (member_count == 0) 
    {
        /* read the current ITEM to get cost */
        ci.GetFieldValues(&item_rec);

        /* return cost from the ITEM record just read */
        return item_rec.cost;
    }

    /* there is at least one member, so go down a level */
    total_cost = 0.0L;

    /* cycle through all components */
    cb_bom_members = ci.Get_bom_members();
    while (!cb_bom_members.AfterLast()) 
    {
        /* read the bill rec to get quanity */
        cb_bom_members.GetFieldValues(&bill_rec);

        /* find its owner thru WHERE_USED set - this is a component ITEM */
        ci_where_used_owner = cb_bom_members.Get_where_used_owner();

        /* recursive call - get cost of all components connected to this one */
        total_cost += get_cost(ci_where_used_owner) * bill_rec.quantity;
        cb_bom_members++;
    }
    return total_cost;
}

/* Create a new item id */
void Cbom::bom_random_id(char *string)
{
    sprintf(string, "%-15.15d", ++count);
}
