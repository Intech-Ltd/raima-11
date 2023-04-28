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

#if !defined(TFS_LM_H)
#define TFS_LM_H

#include "rdmtype.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Lock Manager/Runtime function interaction */

#define LM_USERID_LEN 128
#define MAX_PENDING_USERS   8

/*lint -emacro(845,DWORD*,WORD*) */
#define DWORDPAD(a) ((sizeof(int64_t)-((uint32_t) (uintptr_t) (a)%sizeof(int64_t)))%sizeof(int64_t))
#define DWORDALIGN(a) (a) += DWORDPAD(a)
#define DWORDALIGNED(a) ((a)+DWORDPAD(a))
#define WORDPAD(a) ((sizeof(int32_t)-((uint32_t) (uintptr_t) (a)%sizeof(int32_t)))%sizeof(int32_t))
#define WORDALIGN(a) (a) += WORDPAD(a)
#define WORDALIGNED(a) ((a)+WORDPAD(a))
#define BITMAPBYTES(a) WORDALIGNED((a + BITS_PER_BYTE - 1) / BITS_PER_BYTE)

typedef struct {
    int16_t  q_locktype;
    uint16_t q_user;
    uint16_t q_next;
} TFS_LM_QUEUETAB;

typedef struct
{
    uint16_t fref;
    int16_t  type;
}  TFS_LM_LOCKREQ;

typedef struct
{
    uint16_t lm_maxusers;
    uint16_t lm_maxfiles;
    uint16_t lm_maxqueue;
    uint16_t lm_numusers;
    uint16_t lm_numfiles;
    uint16_t lm_filebmbytes;
    uint16_t lm_userbmbytes;

    uint32_t lm_locks_granted;
    uint32_t lm_locks_rejected;
    uint32_t lm_locks_timedout;
    uint32_t lm_locks_freed;
    uint32_t lm_queued_reqs;
} TFS_LM_STATUS;


/* ==========================================================================
    Message packets, Runtime to Lock Manager
*/

typedef struct
{
    uint16_t   nfiles;
    uint32_t   read_lock_secs;
    uint32_t   write_lock_secs;
    TFS_LM_LOCKREQ locks[1];
}  TFS_LM_LOCK;

typedef struct
{
    uint16_t nfiles;
    uint16_t frefs[1];
}  TFS_LM_FREE;

/* ==========================================================================
    Message packets, Lock Manager to Runtime
*/
#if 0
typedef struct
{
    DB_ULONG t_msgs_received;
    DB_ULONG t_msgs_sent;
    DB_ULONG t_locks_granted;
    DB_ULONG t_locks_rejected;
    DB_ULONG t_locks_timedout;
    DB_ULONG t_locks_freed;
}  LR_TABLES;
#endif

typedef struct
{
    uint16_t ui_pending;
    uint32_t ui_timeout;
    int16_t  ui_status;
    char     ui_name[LM_USERID_LEN];
}  TFS_LM_USERINFO;

typedef struct
{
    char     fi_name[FILENMLEN];
    int16_t  fi_lockstat;
    uint16_t fi_queue;
    uint16_t fi_numlocks;
    uint16_t fi_user_with_lock;     /* userid currently with lock on file */
    uint16_t fi_pending_count;      /* number of users waiting to lock file */
    uint16_t fi_pending_users[MAX_PENDING_USERS];
                                    /* array of waiting user ids (8 max).
                                       for utility informational use only.
                                     */

} TFS_LM_FILEINFO;

#if 0
typedef struct
{
    char     ut_logfile[FILENMLEN];
    uint32_t ut_timer;
    uint16_t ut_pending;
    int16_t  ut_status;
    uint16_t ut_recover;
} LR_USERTAB;
#endif

#if 0
typedef struct
{
    char     ft_name[FILENMLEN];
    int16_t  ft_lockstat;
    uint16_t ft_numlocks;
    uint16_t ft_user_with_lock;
    uint16_t ft_queue_entry;
} LR_FILETAB;
#endif

#if 0
typedef struct
{
    char     tt_name[FILENMLEN];
    int16_t  tt_status;
    uint16_t tt_recuser;
    uint16_t tt_nusers;
} LR_TAFTAB;
#endif

#if 0
typedef struct
{
    uint16_t num_logs;
    char     replfiles[1];
} LR_REPLNOTIFY;
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* TFS_LM_H */

