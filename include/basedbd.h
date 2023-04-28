/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
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

#if !defined(BASEDBD_H)
#define BASEDBD_H

#include "dbtype.h"        /* for TXID */

/* dbd utility functions and definitions */

int32_t EXTERNAL_FCN b_get_dbd_dict_size_len( int32_t, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_file_entry_len( int32_t, char, char, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_record_entry_len( int32_t, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_field_entry_len( int32_t, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_key_entry_len( int32_t, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_set_entry_len( int32_t, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_member_entry_len( int32_t, uint16_t *);
int32_t EXTERNAL_FCN b_get_dbd_sort_entry_len( int32_t, uint16_t *);

int32_t EXTERNAL_FCN b_get_dict_size(DICT_SIZE *, const void *, int32_t, char);
int32_t EXTERNAL_FCN b_get_file_entry(FILE_ENTRY *, const void *, FILE_NO, int32_t, char, char, char );
int32_t EXTERNAL_FCN b_get_record_entry(RECORD_ENTRY *, const void *, uint16_t, FILE_ENTRY *, int32_t, char);
int32_t EXTERNAL_FCN b_get_field_entry(FIELD_ENTRY *, const void *, RECORD_ENTRY *, uint32_t, int32_t, char );
int32_t EXTERNAL_FCN b_get_key_entry(KEY_ENTRY *, const void *, int32_t, char, const FIELD_ENTRY *);
int32_t EXTERNAL_FCN b_get_set_entry(SET_ENTRY *, const void *, RECORD_ENTRY *, uint16_t, int32_t, char );
int32_t EXTERNAL_FCN b_get_member_entry(MEMBER_ENTRY *, const void *, RECORD_ENTRY *, SET_ENTRY *, uint32_t, int32_t, char );
int32_t EXTERNAL_FCN b_get_sort_entry(SORT_ENTRY *, const void *, const SET_ENTRY *, uint32_t, int32_t, char );

int32_t EXTERNAL_FCN b_dbd_check_version(char *, int32_t *, char *, char *, char *);
int32_t EXTERNAL_FCN b_dbd_check_version_all_endian(char *, int32_t *, char *, char *, char *);
void    EXTERNAL_FCN b_copy_filename(char *, const char *, uint32_t);

char        EXTERNAL_FCN b_get_external_type(char);
const char *EXTERNAL_FCN b_get_field_type_text(const FIELD_ENTRY *);
char       *EXTERNAL_FCN b_get_field_array_text(char *, size_t, const FIELD_ENTRY *);
const char *EXTERNAL_FCN b_get_objc_field_type_text(const FIELD_ENTRY *);
uint16_t b_calc_entries(const FIELD_ENTRY *, uint16_t *, uint16_t *);
uint16_t b_is_string_field(const FIELD_ENTRY *);
/*lint -esym(534,b_get_field_array_text) */

#endif  /* BASEDBD_H */

