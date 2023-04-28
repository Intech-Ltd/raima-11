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

#if !defined(INTERNAL_H)
#define INTERNAL_H

#include "dbtype.h"

#define TOPIC_GLOBALS             0
#define TOPIC_TASK                1
#define TOPIC_FILE_TABLE          2
#define TOPIC_RECORD_TABLE        3
#define TOPIC_SET_TABLE           4
#define TOPIC_MEMBER_TABLE        5
#define TOPIC_SORT_TABLE          6
#define TOPIC_FIELD_TABLE         7
#define TOPIC_KEY_TABLE           8
#define TOPIC_DB_TABLE            9
#if 0
#define TOPIC_PGZERO_TABLE       10
#endif
#define TOPIC_RECORD_NAMES       11
#define TOPIC_SET_NAMES          12
#define TOPIC_FIELD_NAMES        13

#define ID_DBUSERID            1
#define ID_SIZE_FT             2
#define ID_SIZE_RT             3
#define ID_SIZE_ST             4
#define ID_SIZE_MT             5
#define ID_SIZE_SRT            6
#define ID_SIZE_FD             7
#define ID_SIZE_KT             8
#define ID_NO_OF_DBS           9
#define ID_TRANS_ID           10
#define ID_DB_TIMEOUT         11
#if 0
#define ID_CACHE_LOOKUP       12
#define ID_CACHE_HITS         13
#endif
#define ID_READLOCKSECS       14
#define ID_WRITELOCKSECS      15

#endif  /* INTERNAL_H */

