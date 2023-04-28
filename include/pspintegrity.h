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

#include <time.h>

#if !defined(SERVER_PRIORITY)
#define SERVER_PRIORITY     127
#endif
#if !defined(CONNECTION_PRIORITY)
#define CONNECTION_PRIORITY 127
#endif
#if !defined(CHKP_PRIORITY)
#define CHKP_PRIORITY       127
#endif
#if !defined(REP_LOG_PRIORITY)
#define REP_LOG_PRIORITY    127
#endif
#if !defined(SLAVE_PRIORITY)
#define SLAVE_PRIORITY      127
#endif
#if !defined(APP_PRIORITY)
#define APP_PRIORITY      127
#endif

#define psp_thread_t Task
#define psp_mutex_t  Semaphore
#define psp_cond_t   Condition

/*--------------------------------------------------------------------------
 * Thread macros
 */
#define psp_thread_create(fcn, data, stack, priority, tid, ret, name) \
    ret = psp_integrity_thread_create(fcn, data, stack, priority, &tid, name)
#define psp_thread_exit(ret)         Exit(ret)
#define psp_thread_self()            CurrentTask()
#define psp_thread_equal(tid1, tid2) tid1 == tid2

#define psp_thread_minpriority(tid, prio) prio = 0
#define psp_thread_maxpriority(tid, prio) \
        { \
            Value val; \
            GetMaximumPriorityAndWeight(tid, &val, NULL); \
            prio = val; \
        }
#define psp_thread_setpriority(tid, prio, ret) ret = SetTaskPriority(tid, prio, false)

/*--------------------------------------------------------------------------
 * Mutex macros
 */
#define psp_mutex_init(m)             CreateBinarySemaphore(&m)
#define psp_mutex_shared_init(m, ret) \
        { \
            if (CreateBinarySemaphore(&m) == Success) \
                ret = PSP_OKAY; \
            else \
                ret = PSP_FAILED; \
        }

#define psp_mutex_destroy(m)          CloseSemaphore(m)
#define psp_mutex_lock(m)             WaitForSemaphore(m)
#define psp_mutex_trylock(m)          TryToObtainSemaphore(m)
#define psp_mutex_unlock(m)           ReleaseSemaphore(m)

#define PSP_MUTEX_INITIALIZER NULLSemaphore

/*--------------------------------------------------------------------------
 * Condition macros
 */
#define psp_cond_init(c, m)             CreateCondition(&c, m, true)
#define psp_cond_shared_init(c, m, ret) \
        { \
            if (CreateBinarySemaphore(&m) == Success) \
                ret = PSP_OKAY; \
            else \
                ret = PSP_FAILED; \
        }

#define psp_cond_destroy(c)             CloseCondition(c)
#define psp_cond_wait(c, m)             WaitForCondition(c, NULL, false)
#define psp_cond_timedwait(c, m, t)     psp_integrity_cond_timedwait(&c, t)
#define psp_cond_broadcast(c)           BroadcastCondition(c)

int16_t psp_integrity_cond_timedwait(psp_cond_t *, uint32_t);
