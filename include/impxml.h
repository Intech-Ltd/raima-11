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

#if !defined(__IMPXML_H)
#define __IMPXML_H

#include "rdm.h"
#include "xmlb64coder.h"
#include "xmlparse.h"
#include "dbtype.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define IMPXML_CONTINUE 0
#define IMPXML_BREAK    1

typedef struct
{
    uint32_t    line;
    int32_t     code;
    const char *text;
    const char *name;
} DB_IMPERR;

typedef struct
{
    uint32_t owner_fldno;
    int32_t  set;
    uint16_t reftype;
    uint32_t lineno;
    union {
        uint64_t rowid;
        char     keydata[MAXKEYSIZE];
    } ownerref;
} DB_IMPSET;


typedef struct
{
    char     name[NAMELEN];
    uint32_t idno;
} XML_FIELD;

typedef struct
{
    char       name[NAMELEN];
    uint16_t   idno;
    int32_t    dbno;
    XML_FIELD *fieldTable;
} XML_RECORD;

typedef struct
{
    char     name[NAMELEN];
    uint16_t idno;
} XML_SET;

typedef struct DB_IMPBLOB_
{
    struct DB_IMPBLOB_ *next;
    int32_t             field;
    uint32_t            size;
    const char         *data;
} DB_IMPBLOB;

typedef struct
{
    size_t     size;
    size_t     pos;
    XML_Char  *ptr;
} DB_IMPMEM;

struct DB_IMPENV_S;

typedef int32_t (*IMPXMLRECPROC)(DB_TASK *, int32_t, char *, void *);
typedef int32_t (*IMPXMLERRPROC)(struct DB_IMPENV_S *, int32_t);
typedef int32_t (*IMPXMLCNVPROC)(DB_TASK *, int32_t, char *, char *, char *, size_t, void *);

typedef struct DB_IMPENV_S
{
    int32_t     State;
    int32_t     fRowid;           /* true/false */
    int32_t     fBase64;
    uint64_t    rowidref;
    int32_t     fAbortElement;
    int32_t     fCdata;

    XML_Parser  Parser;
    int32_t     fDataRequired;
    DB_IMPMEM   dataBuffer;
    DB_IMPMEM   CdataBuffer;

    DB_TASK    *dbTask;
    int32_t     lockretrycnt;    /* default 100 */
    int32_t     fDbopen;         /* TRUE/FALSE: was dbopen before imp_init? */
    int32_t     fTransact;       /* 0 - one transaction for entire import (default);
                                    N - Transaction end after N inserts */
    int32_t     fInsertCount;    /* Counter for fTransact */
    uint16_t    rectype;         /* current record type */
    int32_t     dbno;            /* current database number */
    uint16_t    recid;           /* current record location in internal table */
    uint32_t   *fldlen;          /* length of one array element */
    uint32_t   *fldarray;
    uint32_t   *fldarraymax;
    uint32_t    structbeg;
    uint32_t    structend;
    uint32_t    arraypos;
    void       *recbuf;
    uint32_t    setcnt;
    DB_IMPSET  *setdata;
    DB_IMPBLOB *blobdata;

    char       *imptmpfile;      /* tmpdb fwdref file */
    int32_t     impdbpages;      /* default 100 */
    int32_t     impovpages;      /* default 3 */
    int32_t     tmpdb_dbn;
    PSP_FH      tmphndl;

    uint32_t    iErrorCount;
    uint32_t    iErrorSize;
    DB_IMPERR  *pErrorList;
    int32_t     fInvFldNameTest; /* set to TRUE to validate tag names as valid fields */

    XML_RECORD *recordTable;    /* sorted record table (includes field names) */
    uint32_t    size_recordTable;
    XML_SET    *setTable;        /* sorted set table */

    IMPXMLRECPROC recHandler;
    IMPXMLERRPROC errHandler;
    IMPXMLCNVPROC cnvHandler;

    void      *userExt;
} DB_IMPENV;

enum {DB_IMP_REF_STORE, DB_IMP_MAPPING_STORE};

int32_t EXTERNAL_FCN imp_init(DB_IMPENV **, int32_t, const char *, int32_t, int32_t, const char *, DB_TASK *);
int32_t EXTERNAL_FCN imp_term(DB_IMPENV *);
int32_t EXTERNAL_FCN imp_xmlParse(DB_IMPENV *, const char *, size_t, int32_t);
void    EXTERNAL_FCN imp_xmlGetErrors(DB_IMPENV *, uint32_t *, DB_IMPERR **);
void    EXTERNAL_FCN imp_xmlSetRecordCallback(DB_IMPENV *, IMPXMLRECPROC);
void    EXTERNAL_FCN imp_xmlSetErrorCallback(DB_IMPENV *, IMPXMLERRPROC);
void    EXTERNAL_FCN imp_xmlSetConversionCallback(DB_IMPENV *, IMPXMLCNVPROC);

#if defined(__cplusplus)
}
#endif

#endif  /* __IMPXML_H */

