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

#ifndef DB_TYPES_H
#define DB_TYPES_H

#include "rdm.h"
#if defined(__cplusplus)
#if defined(RDM_WINDOWS) || defined(RDM_WINCE)
#pragma warning(disable: 4290)
#endif
#include <exception>
extern "C"
{
#include "berror.h"
}
using namespace std;
#if defined(__cplusplus)
namespace RDM_CPP {
#endif
#else
#include "berror.h"
#endif

/*lint -e1550 */

#define INVALID_TYPE -1
#define BEFORE_FIRST_TYPE -2
#define PAST_LAST_TYPE -3
#define MEMBER_TYPE -4

#define CPP_FIRST       -30000    /* first c++ interface err */

#define CPP_CURSORS_NOT_RELATED  -30014
#define CPP_BLOB_READ            -30013
#define CPP_NOT_IMPLEMENTED      -30012
#define CPP_RECORD_NOT_FOUND     -30011
#define CPP_INVALID_TRANSACTION  -30010
#define CPP_INVALID_DB           -30009
#define CPP_INVALID_DB_TYPE      -30008
#define CPP_NOT_AT_RECORD        -30007
#define CPP_SET_GONE             -30006
#define CPP_CORRUPTED_EXTERNALLY -30005
#define CPP_DISCONNECTED         -30004
#define CPP_DELETED              -30003
#define CPP_INVALID_ITERATION    -30002
#define CPP_INVALID_CURSOR       -30001
#define CPP_INVALID_TYPE         -30000

static const RDM_ERROR rdm_cpp_errors [] = 
{
    { CPP_CURSORS_NOT_RELATED,  "CPP_CURSORS_NOT_RELATED",  "Cursors are not related"},
    { CPP_BLOB_READ,            "CPP_BLOB_READ",            "A blob read call did not read the full amount requested"},
    { CPP_NOT_IMPLEMENTED,      "CPP_NOT_IMPLEMENTED",      "The method is not implemented in the class type"},
    { CPP_RECORD_NOT_FOUND,     "CPP_RECORD_NOT_FOUND",     "The record is not found in the cursor collection"},
    { CPP_INVALID_TRANSACTION,  "CPP_INVALID_TRANSACTION",  "Invalid transaction status"},
    { CPP_INVALID_DB,           "CPP_INVALID_DB",           "Invalid database connection"},
    { CPP_INVALID_DB_TYPE,      "CPP_INVALID_DB_TYPE",      "Invalid db type"},
    { CPP_NOT_AT_RECORD,        "CPP_NOT_AT_RECORD",        "Cursor is not at a valid record"},
    { CPP_SET_GONE,             "CPP_SET_GONE",             "Set is gone"},
    { CPP_CORRUPTED_EXTERNALLY, "CPP_CORRUPTED_EXTERNALLY", "Cursor corrupted externally"},
    { CPP_DISCONNECTED,         "CPP_DISCONNECTED",         "Disconnected"},
    { CPP_DELETED,              "CPP_DELETED",              "Deleted"},
    { CPP_INVALID_ITERATION,    "CPP_INVALID_ITERATION",    "Invalid iteration"},
    { CPP_INVALID_CURSOR,       "CPP_INVALID_CURSOR",       "Invalid cursor"},
    { CPP_INVALID_TYPE,         "CPP_INVALID_TYPE",         "Invalid cursor record type"}
};

#if defined(__cplusplus)
class IDb;
class ICursor;
class ISetCursor;
class Db;
class Cursor;
typedef const void *dbType;
typedef int32_t recordType;
typedef int32_t setType;
typedef int32_t fieldId;
typedef int32_t keyId;

#if defined(RDM_WINCE)
#pragma warning (push)
#pragma warning (disable:4275)
#endif

class PUBLIC_CLASS rdm_exception : public exception 
{
public:
    rdm_exception (int32_t error);
   
    const char* what() const throw();
    int32_t error (void) const;
private:
    int32_t m_error;
};

#if defined(RDM_WINCE)
#pragma warning (pop)
#endif

class rdm_cursors_not_related_exception : public rdm_exception {
public:
    rdm_cursors_not_related_exception () : rdm_exception (CPP_CURSORS_NOT_RELATED) {
    }
};

class rdm_set_gone_exception : public rdm_exception {
public:
    rdm_set_gone_exception () : rdm_exception (CPP_SET_GONE) {
    }
};

class rdm_record_not_found_exception : public rdm_exception {
public:
    rdm_record_not_found_exception () : rdm_exception (CPP_RECORD_NOT_FOUND) {
    }
};

class rdm_not_implemented_exception : public rdm_exception {
public:
    rdm_not_implemented_exception () : rdm_exception (CPP_NOT_IMPLEMENTED) {
    }
};

class rdm_corrupted_externally_exception : public rdm_exception {
public:
    rdm_corrupted_externally_exception () : rdm_exception (CPP_CORRUPTED_EXTERNALLY) {
    }
};

class rdm_not_at_record_exception : public rdm_exception {
public:
    rdm_not_at_record_exception () : rdm_exception (CPP_NOT_AT_RECORD) {
    }
};

class rdm_disconnected_exception : public rdm_exception {
public:
    rdm_disconnected_exception () : rdm_exception (CPP_DISCONNECTED) {
    }
};

class rdm_deleted_exception : public rdm_exception {
public:
    rdm_deleted_exception () : rdm_exception (CPP_DELETED) {
    }
};

class rdm_invalid_iteration_exception : public rdm_exception {
public:
    rdm_invalid_iteration_exception () : rdm_exception (CPP_INVALID_ITERATION) {
    }
};

class rdm_invalid_cursor_exception : public rdm_exception {
public:
    rdm_invalid_cursor_exception () : rdm_exception (CPP_INVALID_CURSOR) {
    }
};

class rdm_invalid_type_exception : public rdm_exception {
public:
    rdm_invalid_type_exception () : rdm_exception (CPP_INVALID_TYPE) {
    }
};

class rdm_invalid_db_type_exception : public rdm_exception {
public:
    rdm_invalid_db_type_exception () : rdm_exception (CPP_INVALID_DB_TYPE) {
    }
};

class rdm_invalid_db_exception : public rdm_exception {
public:
    rdm_invalid_db_exception () : rdm_exception (CPP_INVALID_DB) {
    }
};

class rdm_invalid_transaction_exception : public rdm_exception {
public:
    rdm_invalid_transaction_exception () : rdm_exception (CPP_INVALID_TRANSACTION) {
    }
};

class rdm_blob_read_exception : public rdm_exception {
public:
    rdm_blob_read_exception () : rdm_exception (CPP_BLOB_READ) {
    }
};

/*lint -e1549 */

#endif
#if defined(__cplusplus)
}
#endif
#endif
