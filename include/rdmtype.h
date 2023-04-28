/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2001-2008 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#if !defined(__RDM_TYPE_H)
#define __RDM_TYPE_H

typedef uint8_t     DB_BYTE;
typedef uint16_t    DB_USHORT;
typedef uint32_t    DB_UINT;
typedef uint32_t    DB_ULONG;
typedef uint16_t    DB_BOOLEAN;
typedef enum REP_DB_TYPE_E
{
    REP_NONE = 0,
    REP_MASTER,
    REP_SLAVE
} REP_DB_TYPE;

/* ==========================================================================
 * The following definitions are for legacy purposes.  Starting in Raima
 * Database Manager 10 the ASCII and Unicode versions of the product were
 * merged into a single version, with all strings being treated as UTF-8
 * internally.  These are provided so existing legacy applications will not
 * need to be modified.
 *
 * These should not be used for any new code.
 */

#if defined(UNICODE)

#define PSP_TCHAR      wchar_t
#define PSP_TEXT(s)    L ## s

#else

#define PSP_TCHAR      char
#define PSP_TEXT(s)    s

#endif

#define DB_TCHAR   PSP_TCHAR
#define DB_TEXT(s) PSP_TEXT(s)

#endif /* __RDMTYPE_H */

