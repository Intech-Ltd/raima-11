/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#ifndef _DB_H
#define _DB_H

#include "DbTypes.h"

namespace RDM_CPP {

/*! \brief The Db base class
 * 
 * This Db class provides the base database implmentation
*/
class PUBLIC_CLASS Db {
public:
    /* Handle more than one reference to the underlying database */
    //! Db Default Constructor
    Db (void) throw ();

    //! Db Copy Constructor
    Db (const Db& db) throw ();

    //! Db assignment operator
    Db& operator = (const Db& db) throw ();

    //! Initialize or reinitialize the database
    void Initialize (void) const throw (rdm_exception&);

    /*! \brief Begin reading from a database
     *  \param types An array of record types to be read locked
     *  \param num The number of records to be locked
     *
     * Use this method before reading data from the database.  Specify
     * the record types you want access to or NULL for every record
     * type.  You are not allowed to combine this method with
     * BeginUpdate or BeginSnapshot.  Call End when you are done
     * reading 
     */
    void BeginRead (const recordType types[]= NULL, int16_t num = 0) const throw (rdm_exception&);

    /*! \brief Begin updating a database
     *  \param types An array of record types to be write locked
     *  \param num The number of records to be locked
     *  
     * Use this method before making any updates to the database.
     * Specify the record types you want access to or NULL for every
     * record type.  You are not allowed to combine this method with
     * BeginRead or BeginSnapshot.  Call End when you are done with
     * the update */
    void BeginUpdate (const recordType types[]= NULL, int16_t num = 0) const throw (rdm_exception&);

    /*! \brief Create a static snapshot of the database
     * 
     * Take a snapshot of the databases for reading data from the
     * database.  You are not allowed to combine this method with
     * BeginRead or BeginSnapshot.  Call End when you are done
     * reading 
     */
    void BeginSnapshot () const throw (rdm_exception&);

    /*! \brief Precommit a database transaction
     *
     * Prepare an update transaction for commit.  Use of this method
     * is optional. */
    void Precommit (void) const throw (rdm_exception&);

    //! Commit a database transaction
    /** 
     * End any transaction type.  This method will commit an update
     * transaction. */
    void End (void) const throw (rdm_exception&);

    //! Abort a database transaction
    /** 
     * Abort an update transaction.  Aborting other types of
     * transactions has the same effect as End (). */
    void Abort (void) const throw (rdm_exception&);
    
    //! Destroy the database
    void Destroy (void) const throw (rdm_exception&);

    //! Check to see if database(s) are open exclusively
    bool IsExclusiveTask (void) const throw (rdm_exception&);

    //! Check to see if database(s) are open in shared mode
    bool IsSharedTask (void) const throw (rdm_exception&);

    //! Check to see if database(s) are open in read-only mode
    bool IsReadOnlyTask (void) const throw (rdm_exception&);


    //! Release the database
    Db Release (void) throw (rdm_exception&);
    
    //! Db destructor
    ~Db ();

private:
    //! Db reference operator
    Db* operator & (void);
protected:
    //! Internal IDb pointer
    IDb *m_idb;
    
    //! Constructor
    /*! 
     * \param idb The internal IDb to use with this instance
     */
    Db (IDb *idb) throw ();  
    
    //! Db Constructor
    /*! 
     * \param db The internal IDb to use with this instance
     * \param type The Db type
     */
    Db (const Db& db, dbType type) throw (rdm_exception&);
    
    //! Create a new record with default values
    /*! 
     * \param type The type of record to create
     * \return The singleton Cursor object positioned at the new record
     */
    Cursor NewRecords (recordType type) const throw (rdm_exception&);
    
    //! Create a new record with the specified value
    /*! 
     * \param type The type of record to create
     * \param fields The field values for the new record
     * \return The singleton Cursor object positioned at the new record
     */
    Cursor NewRecordsWithFieldValues (recordType type, const void *fields) const throw (rdm_exception&);

    //! Obtain a Cursor collection for a record type
    /*! 
     * \param type The type of record to create a Cursor for
     * \return The Cursor object positioned at the first record in the collection
     */
    Cursor GetRecords (recordType type) const throw (rdm_exception&);
    
    //! Obtain a Cursor collection for a record type by a key
    /*! 
     * \param id The id of record key for the Cursor
     * \return The Cursor object positioned at the first record in the collection
     */
    Cursor GetRecordsByKey (keyId id) const throw (rdm_exception&);
    
    //! Open a database and obtain a Db object
    /*! 
     * \param type The dbType
     * \param name The database name
     * \param mode The open mode
     * \param dbd_array The dbd array generated by ddlp -c
     * \param dbdsize sizeof(dbd_array)
     * \return A Db object
     */
    static Db Open (dbType type, const char *name, const char *mode, const void *dbd_array=NULL, int32_t dbdsize=0) throw (rdm_exception&);

#if !defined(RDM_NO_UNICODE)
    //! Open a database and obtain a Db object
    /*! 
     * \param type The dbType
     * \param name The database name
     * \param mode The open mode
     * \param dbd_array The dbd array generated by ddlp -c
     * \param dbdsize sizeof(dbd_array)
     * \return A Db object
     */
    static Db Open (dbType type, const wchar_t *name, const wchar_t *mode, const void *dbd_array=NULL, int32_t dbdsize=0) throw (rdm_exception&);
#endif
   
    //! Get the dbType for a Db object
    /*! 
     * \return The dbType for the Db object
     */
    dbType GetType (void) const throw (rdm_exception&);
    
    //! Validate a Db object's dbType
    /*! 
     * \param type The dbType to validate against
     */
    void VerifyType (dbType type) const throw (rdm_exception&);
    
    //! Validate the IDb
    void ValidateIDb () const throw (rdm_exception&);
    
    //! Validate a Db object's dbType
    /*! 
     * \param type The dbType to validate against
     * \param db The Db reference to assign
     * \return A Db reference
     */
    Db& AssignAndVerifyType (dbType type, const Db& db) throw (rdm_exception&);
    
    friend class Cursor;
};
}
#endif
