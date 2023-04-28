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

#ifndef _CURSOR_H
#define _CURSOR_H

#include "DbTypes.h"

namespace RDM_CPP {

class PUBLIC_CLASS Cursor {
public:
    /* Handle more than one reference to the underlying cursor */
    //! Cursor Default Cursor
    Cursor (void) throw ();
    
    //! Cursor Copy Cursor
    Cursor (const Cursor& cursor) throw ();
    
    //! Cursor assignment operator
    Cursor& operator = (const Cursor& cursor) throw ();
    
    //! Cursor Destructor
    ~Cursor ();

    /*! \brief Get the record type for the current cursor position
     *
     * \return The record type
     */
    recordType GetType (void) const throw (rdm_exception&);

    /*! \brief Get a count of the number of records contained in a cursor collection
     *
     * \return The number of records contained in the cursor collection
     */
    uint64_t GetCount (void) const throw (rdm_exception&);

    /*! \brief Mark the end of an array of record types
     *
     * Use this when setting up an array of record types that is
     * passed in to BeginRead or BeginUpdate.
     *
     * \return A record type end marker
     */
    static recordType NoMoreTypes (void) throw () {
        return (recordType) 0;
    }

    /*! \brief Delete the record at the current cursor position
     *
     * \return The Cursor you are using
     */
    Cursor Delete (void) const throw (rdm_exception&);
    
    /*! \brief Disconnect and Delete the record at the current cursor position
     *
     * \return The Cursor you are using
     */
    Cursor DisconnectAndDelete (void) const throw (rdm_exception&);

    /* Basic navigation for this cursor */
    /*! \brief Navigate to the first record in the Cursor collection
     *
     * \return The Cursor you are using
     */
    Cursor First (void) const throw (rdm_exception&);
    
    /*! \brief Navigate to the next record in the Cursor collection
     *
     * \return The Cursor you are using
     */
    Cursor Next (void) const throw (rdm_exception&);
    
    /*! \brief Navigate to the previous record in the Cursor collection
     *
     * \return The Cursor you are using
     */
    Cursor Prev (void) const throw (rdm_exception&);
    
    /*! \brief Navigate to the last record in the Cursor collection
     *
     * \return The Cursor you are using
     */
    Cursor Last (void) const throw (rdm_exception&);

    /* Operators instead of functions above */
    /*! \brief Prefix increment operator
     *
     * \return The Cursor you are using
     */
    Cursor operator ++ (void) const throw (rdm_exception&) { return Next ();}
    
    /*! \brief Postfix increment operator
     *
     * \return The Cursor you are using
     */
    Cursor operator ++ (int) const throw (rdm_exception&) { return Next ();}
    
    /*! \brief Prefix decrement operator
     *
     * \return The Cursor you are using
     */
    Cursor operator -- (void) const throw (rdm_exception&) { return Prev ();}
    
    /*! \brief Postfix decrement operator
     *
     * \return The Cursor you are using
     */
    Cursor operator -- (int) const throw (rdm_exception&) { return Prev ();}
    
    /*! \brief Obtain an IRecScanCursor for the current record type
     *  \return A Cursor positioned at the same record this Cursor is 
     *          positioned at
     * 
     * Create a new cursor doing a table scan for records of the same
     * type as this record.  Position the cursor at this record. 
     */
    Cursor GetRecords (void) const throw (rdm_exception&);

    /*! \brief Obtain an IRecScanCursor for the current record type
     *  \return A singleton Cursor positioned at the record this Cursor is 
     *          positioned at
     * 
     * Create a new singleton cursor for this record, before first
     * record, or past last record
     */
    Cursor GetThis (void) const throw (rdm_exception&);

    /*! \brief Obtain the static BeforeFirst Cursor
     *  \return A singleton Cursor at the BeforeFirst position
     */
    static Cursor GetBeforeFirst (void) throw ();
    
    /*! \brief Obtain the static AfterLast Cursor
     *  \return A singleton Cursor at the AfterLast position
     */
    static Cursor GetAfterLast (void) throw ();
    
    /*! \brief Check to see if the Cursor in the the BeforeFirst position
        \retval true If the current cursor position is before the first record in the collection
        \retval false Otherwise
    */
    bool BeforeFirst (void) const throw (rdm_exception&);
    
    /*! \brief Check to see if the Cursor in the the AfterLast position
        \retval true If the current cursor position is after the last record in the collection
        \retval false Otherwise
    */
    bool AfterLast (void) const throw (rdm_exception&);

    /*! \brief Check to see if the cursor is position at a valid record
     *  \retval true If the cursor is positioned at a valid record
     *  \retval false Otherwise
     *
     * For key lookups, or when records have been deleted or disconnect
     * from sets, do we have a record or not
    */
    bool AtRecord (void) const throw (rdm_exception&);
    
    /*! \brief Check to see if the cursor is not conditioned at a valid record
     *  \retval true If the cursor is not positioned at a valid record
     *  \retval false Otherwise
     *
     * For key lookups, or when records have been deleted or disconnect
     * from sets, do we have a record or not
    */
    bool AtNoRecord (void) const throw (rdm_exception&);

    /*! \brief Check to see if passed in cursor is positioned before ours current position
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is before our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool Before (const Cursor& cursor) const throw (rdm_exception&);

    /*! \brief Check to see if passed in cursor is positioned before or at our current position
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is before or at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool BeforeAt (const Cursor& cursor) const throw (rdm_exception&);

    /*! \brief Check to see if passed in cursor is positioned at our current position
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool At (const Cursor& cursor) const throw (rdm_exception&);

    /*! \brief Check to see if passed in cursor is positioned after or at our current position
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is after or at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool AfterAt (const Cursor& cursor) const throw (rdm_exception&);
    
    /*! \brief Check to see if passed in cursor is positioned after our current position
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is after our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool After (const Cursor& cursor) const throw (rdm_exception&);
    
    /*! \brief Check to see if passed in cursor is not positioned at our current position
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is not at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool NotAt (const Cursor& cursor) const throw (rdm_exception&);

    /* Operators instead of functions above */
    /*! \brief Less than operator
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is before our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool operator < (const Cursor& cursor) const throw (rdm_exception&) { return Before (cursor);}

    /*! \brief Less than or equal to operator
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool operator <= (const Cursor& cursor) const throw (rdm_exception&) { return BeforeAt (cursor);}

    /*! \brief Equal to operator
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool operator == (const Cursor& cursor) const throw (rdm_exception&) { return At (cursor);}

    /*! \brief Greator than or equal to operator
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is after or at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool operator >= (const Cursor& cursor) const throw (rdm_exception&) { return AfterAt (cursor);}

    /*! \brief Greater than operator
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is after our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool operator > (const Cursor& cursor) const throw (rdm_exception&) { return After (cursor);}

    /*! \brief Not equal to operator
     *  \param cursor The Cursor to compare against
     *  \retval true If the cursor position is not at our current position
     *  \retval false Otherwise
     *
     * Compare two cursor positions
    */
    bool operator != (const Cursor& cursor) const throw (rdm_exception&) { return NotAt (cursor);}

    /*! \brief Get associated Db object
     *  \return The Db object associated with this Cursor
    */
    Db GetDb (void) throw (rdm_exception&);

    //! Release the cursor
    Cursor Release (void) throw (rdm_exception&);
private:
    //! Cursor reference operator
    Cursor* operator & (void);
protected:
    //! Internal IDb pointer
    ICursor *m_icursor;

    /* Protected members for internal use only */
    Cursor (ICursor *icursor) throw ();
    Cursor (const Cursor& cursor, recordType record_type) throw (rdm_exception&);
    Cursor GetOwner (setType type) const throw (rdm_exception&);
    Cursor GetMembers (setType type) const throw (rdm_exception&);
    Cursor GetSiblings (setType type) const throw (rdm_exception&);
    Cursor GetRecordsByKey (keyId id) const throw (rdm_exception&);
    void FindRecordsByKey (keyId id, uint16_t nfields, uint16_t partialStrLen, const void *value) const throw (rdm_exception&);
    void SetFieldValues (recordType type, const void*record) const throw (rdm_exception&);
    void GetFieldValues (recordType type, void *record) const throw (rdm_exception&);
    void SetField (fieldId id, const void *field) const throw (rdm_exception&);
    void GetField (fieldId id, void *field) const throw (rdm_exception&);
    void SetBlobField (fieldId id, uint32_t offset, const void *buf, uint32_t size) const throw (rdm_exception&);
    void GetBlobField (fieldId id, uint32_t offset, void *buf, uint32_t len, uint32_t *size=NULL) const throw (rdm_exception&);
    void SetBlobFieldSize (fieldId id, uint32_t size) const throw (rdm_exception&);
    void GetBlobFieldSize (fieldId id, uint32_t *wize) const throw (rdm_exception&);
    void VerifyType (recordType record_type) const throw (rdm_exception&);
    Cursor& AssignAndVerifyType (recordType type, const Cursor& cursor) throw (rdm_exception&);
    int32_t GetMemberCount (setType type) const throw (rdm_exception&);
    void Connect (setType type, const Cursor& owner, bool reconnect) const throw (rdm_exception&);
    void Disconnect (setType type) const throw (rdm_exception&);
    void ValidateICursor() const throw (rdm_exception&);
    friend class Db;
};
}
#endif
