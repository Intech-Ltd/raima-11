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

#if defined(RDM_VXWORKS_6X)
#include <ctype.h>
#else
#include <vxWorks.h>
#endif

#include <time.h>
#include <taskLib.h>
#include <netDrv.h>
#include <sysLib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t count;
    SEM_ID   sem;
} PSP_VXWORKS_COND;

#define psp_thread_t uint32_t
#define psp_mutex_t  SEM_ID
#define psp_cond_t   PSP_VXWORKS_COND

#if !defined(SERVER_PRIORITY)
#define SERVER_PRIORITY     102
#endif
#if !defined(CONNECTION_PRIORITY)
#define CONNECTION_PRIORITY 104
#endif
#if !defined(CHKP_PRIORITY)
#define CHKP_PRIORITY       106
#endif
#if !defined(REP_LOG_PRIORITY)
#define REP_LOG_PRIORITY    108
#endif
#if !defined(SLAVE_PRIORITY)
#define SLAVE_PRIORITY      110
#endif
#if !defined(APP_PRIORITY)
#define APP_PRIORITY      120
#endif

/*--------------------------------------------------------------------------
 * Thread macros
 */
#define psp_thread_create(fcn, data, stack, priority, tid, ret, name) \
        {\
            if ((priority) > 255 || (priority) < 0) \
                priority = 255; \
            if ((stack) == 0) \
                stack = 0x4000; \
            tid = taskSpawn(name, priority, 0, (int32_t) stack, (FUNCPTR) fcn, \
                    (int32_t) arg, 0, 0, 0, 0, 0, 0, 0, 0, 0); \
            ret = (tid == ERROR) ? 1 : 0; \
        }
#define psp_thread_exit(ret)         exit(ret)
#define psp_thread_self()            taskIdSelf()
#define psp_thread_equal(tid1, tid2) tid1 == tid2

#define psp_thread_maxpriority(tid, prio) prio = 0;
#define psp_thread_minpriority(tid, prio) prio = 255;
#define psp_thread_setpriority(tid, prio, ret) ret = taskPrioritySet(tid, prio);

/*--------------------------------------------------------------------------
 * Mutex macros
 */
#define psp_mutex_init(a)             a = semMCreate(SEM_Q_FIFO)
#define psp_mutex_shared_init(a, ret) \
        { \
            a = semMCreate(SEM_Q_FIFO); \
            ret = a == NULL ? PSP_FAILED : PSP_OKAY; \
        }
#define psp_mutex_destroy(a)          semDelete(a)
#define psp_mutex_lock(a)             semTake(a, WAIT_FOREVER)
#define psp_mutex_trylock(a)          semTake(a, 0)
#define psp_mutex_unlock(a)           semGive(a)

#define PSP_MUTEX_INITIALIZER NULL

/*--------------------------------------------------------------------------
 * Condition macros
 */
#define psp_cond_init(c, m) \
        { \
            c.count = 0; \
            c.sem = semCCreate(SEM_Q_FIFO, 0); \
        }
#define psp_cond_shared_init(c, m, ret) \
        { \
            c.count = 0; \
            c.sem = semCCreate(SEM_Q_FIFO, 0); \
            ret = (c.sem == NULL) ? PSP_FAILED : PSP_OKAY; \
        }
#define psp_cond_destroy(c)         semDelete(c.sem)
#define psp_cond_wait(c, m)         psp_vxworks_cond_wait(&c, &m)
#define psp_cond_timedwait(c, m, t) psp_vxworks_cond_timedwait(&c, &m, t)
#define psp_cond_broadcast(c) \
        { \
            while (c.count) \
            { \
                semGive(c.sem); \
                c.count--; \
            } \
        }

int16_t psp_vxworks_cond_wait(psp_cond_t *, psp_mutex_t *);
int16_t psp_vxworks_cond_timedwait(psp_cond_t *, psp_mutex_t *, uint32_t);

int32_t parse_vxworks_startup(int32_t (*)(int32_t, const char *const *),
        const char *, const char *, const char *, const char *, const char *,
        const char *, const char *, const char *, const char *, const char *,
        const char *);

#define PSP_STARTUP_TOOL(name) \
    int32_t name( \
        int32_t a1, \
        int32_t a2, \
        int32_t a3, \
        int32_t a4, \
        int32_t a5, \
        int32_t a6, \
        int32_t a7, \
        int32_t a8, \
        int32_t a9, \
        int32_t a10) \
    { \
        return parse_vxworks_startup(name##_main, #name, (char *) a1, \
                (char *) a2, (char *) a3, (char *) a4, (char *) a5, \
                (char *) a6,  (char *) a7, (char *) a8, (char *) a9, \
                (char *) a10); \
    }

#define PSP_STARTUP_PROGRAM(name)
#define PSP_STARTUP_EXAMPLE(name) PSP_STARTUP_TOOL(name)
#define PSP_STARTUP_TEST(name) PSP_STARTUP_TOOL(name)

#ifdef __cplusplus
}
#endif
