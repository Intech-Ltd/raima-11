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

#ifndef EXPXML_H
#define EXPXML_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xmlb64coder.h"
#include "rdm_version.h"

/* Tokens and symbols for XML instance */

#define DB_XML_OPEN_START_TAG       ("<")
#define DB_XML_CLOSE_START_TAG      (">")
#define DB_XML_OPEN_END_TAG         ("</")
#define DB_XML_CLOSE_END_TAG        (">")

#define DB_XML_END_OF_LINE          ("\n")

#define DB_XML_INDENT               ("   ")
#define DB_XML_ROWID_TAG            ("ROWID")
#define DB_XML_SET_ATTR_NAME        ("set")
#define DB_XML_OPEN_ATTR            ("=\"")
#define DB_XML_CLOSE_ATTR           ("\"")
#define DB_XML_OPEN_CHAR_REF        ("&#")
#define DB_XML_CLOSE_CHAR_REF       (";")
#define DB_XML_VERSION_HEADER       ("?xml version=\"1.0\" encoding=\"utf-8\"?")
#define DB_XML_VERSION_HEADER_START ("?xml version=\"1.0\" encoding=\"")
#define DB_XML_VERSION_HEADER_END   ("\"?")

#define DB_XML_RDM_VERSION			("!-- " RDM_PRODUCT_NAME " " RDM_PRODUCT_VER  " --")

#define DB_XML_B64_NAMESPACE		("")
#define DB_XML_DTD_DOCTYPE			("!DOCTYPE ")
#define DB_XML_DTD_SYSTEM			(" SYSTEM ")
#define DB_XML_DTD_FILEEXT			(".dtd")
#define DB_XML_BINARY_ATTR  		(" dt:dt=\"bin.base64\"")

/* Tokens and symbols for XML Schema */

#define DB_XML_OPEN_TAG             ("<")
#define DB_XML_CLOSE_TAG            ("/>")
#define DB_XML_SCHEMA_TAG           ("schema")
#define DB_XML_ANNOTATION_TAG       ("annotation")
#define DB_XML_DOCUMENTATION_TAG    ("documentation")
#define DB_XML_ELEMENT_TAG          ("element")
#define DB_XML_COMPLEX_TAG          ("complexType")
#define DB_XML_SEQUENCE_TAG         ("sequence")
#define DB_XML_SIMPLE_TAG           ("simpleContent")
#define DB_XML_EXTENSION_TAG        ("extension")
#define DB_XML_ATTRIBUTE_TAG        ("attribute")
#define DB_XML_GROUP_TAG            ("group")
#define DB_XML_LANG_ATTR_NAME       ("xml:lang")
#define DB_XML_XMLNS_ATTR_NAME      ("xmlns")
#define DB_XML_RDMNS_ATTR_NAME      ("xmlns:rdm")
#define DB_XML_TARGETNS_ATTR_NAME   ("targetNamespace")
#define DB_XML_BASE_ATTR_NAME       ("base")
#define DB_XML_NAME_ATTR_NAME       ("name")
#define DB_XML_TYPE_ATTR_NAME       ("type")
#define DB_XML_MIN_ATTR_NAME        ("minOccurs")
#define DB_XML_MAX_ATTR_NAME        ("maxOccurs")
#define DB_XML_USE_ATTR_NAME        ("use")
#define DB_XML_ENGLISH_ATTR_VAL     ("en")
#define DB_XML_W3ORG_ATTR_VAL       ("http://www.w3.org/2001/XMLSchema")
#define DB_XML_ZERO_ATTR_VAL        ("0")
#define DB_XML_UNBOUNDED_ATTR_VAL   ("unbounded")
#define DB_XML_INT32_ATTR_VAL       ("int32")
#define DB_XML_UINT32_ATTR_VAL      ("uint32")
#define DB_XML_INT16_ATTR_VAL       ("int16")
#define DB_XML_UINT16_ATTR_VAL      ("uint16")
#define DB_XML_INT64_ATTR_VAL       ("int64")
#define DB_XML_UINT64_ATTR_VAL      ("uint64")
#define DB_XML_FLOAT_ATTR_VAL       ("float")
#define DB_XML_DOUBLE_ATTR_VAL      ("double")
#define DB_XML_STRING_ATTR_VAL      ("string")
#define DB_XML_DATE_ATTR_VAL        ("date")
#define DB_XML_TIME_ATTR_VAL        ("time")
#define DB_XML_REQUIRED_ATTR_VAL    ("required")
#define DB_XML_OPTIONAL_ATTR_VAL    ("optional")
#define DB_XML_BIRDSTEP_TEXT        ("Raima RDM Database Export of ")
#define DB_XML_BIRDSTEP_DB_SUFFIX   ("db")
#define DB_XML_BIRDSTEP_DB_PATH     ("http://www.raima.com/schema/")
#define DB_XML_RDMNS_PREFIX         ("rdm:")


/* Tokens and symbols for XML DTD */

#define DB_XML_OPEN_DTD_TAG         ("<")
#define DB_XML_CLOSE_DTD_TAG        (">")
#define DB_XML_ELEMENT_TAG_NAME     ("!ELEMENT")
#define DB_XML_ATTLIST_TAG_NAME     ("!ATTLIST")
#define DB_XML_OPEN_CONTENT_LIST    (" (")
#define DB_XML_CLOSE_CONTENT_LIST   (")")
#define DB_XML_AND_CONTENT_LIST     (",")
#define DB_XML_OR_CONTENT_LIST      ("|")
#define DB_XML_REPEAT_CONTENT_LIST  ("*")
#define DB_XML_REPEAT_ELEMENT       ("+")
#define DB_XML_PCDATA_CONTENT       ("#PCDATA")
#define DB_XML_ATTR_CDATA_TYPE      ("CDATA")
#define DB_XML_ATTR_IMPLIED         ("#IMPLIED")
#define DB_XML_AMPERSAND			("&")
#define DB_XML_AMP_ESCAPE			"&amp;"
#define DB_XML_LT_ESCAPE			"&lt;"
#define DB_XML_GT_ESCAPE			"&gt;"
#define DB_XML_SPEC_CHARS			"&<>"


#define DB_XML_SQL_SELECT			"select db_addr "
#define DB_XML_SQL_FROM			    "from"

#define MAX_TABLES 20

#define DB_XML_BUF_LEN              1024
#define DB_XML_LINE_LEN             80
//#define DB_XML_HEADER_LEN           80
//#define DB_XML_DOCTYPE_LEN          1024
#define DB_XML_MAX_B64_LEN			76

typedef struct DB_EXPENV_S
{
    DB_TASK        *task;
    int32_t         dbn;
    char           *enc;
    char           *xmlbuf;
    size_t          xmlbuflen;
    size_t          xmlbufpos;
    char           *indentbuf;
    size_t          indentbuflen;
    size_t          indentbufpos;
    char           *recbuf;
    char           *fldbuf;
    uint32_t       *keyfld;
    int32_t        *ownertypes;
    uint32_t        maxownertypes;
    int32_t         usecount;
	PSP_FILE_STREAM hOutput;
	int32_t         fetch_started;
} DB_EXPENV;


#define DB_EXP_XML_NONE	  0x0000
#define DB_EXP_XML_SCHEMA 0x0001
#define DB_EXP_XML_DTD    0x0002


int32_t EXTERNAL_FCN exp_init(DB_EXPENV **, const char *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN exp_term(const DB_EXPENV *);
int32_t EXTERNAL_FCN exp_xmlschema(size_t *, char **, int32_t, DB_EXPENV *);
int32_t EXTERNAL_FCN exp_xmlschemarec(size_t *, char **, int32_t, int32_t, DB_EXPENV *);
int32_t EXTERNAL_FCN exp_xmlrec(size_t *, char **, DB_ADDR, DB_EXPENV *);
int32_t EXTERNAL_FCN exp_xmlgetownertypes(int32_t *, int32_t **, int32_t, DB_ADDR, const DB_EXPENV *);

int32_t EXTERNAL_FCN exp_xmlheader(const char *, int32_t, DB_EXPENV *);
int32_t EXTERNAL_FCN exp_xmlstarttag(const char *, const char *, DB_EXPENV *);
int32_t EXTERNAL_FCN exp_xmlendtag(const char *, DB_EXPENV *);

#ifdef __cplusplus
}
#endif
#endif /* EXPXML_H */
