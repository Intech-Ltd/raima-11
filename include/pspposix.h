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

#include <pthread.h>
#include <sys/time.h>

#define psp_thread_t pthread_t
#define psp_mutex_t  pthread_mutex_t
#define psp_cond_t   pthread_cond_t

/*--------------------------------------------------------------------------
 * Thread macros
 */
#if defined(RDM_LINUX)
#define psp_set_sigmask\
        {\
            sigset_t orgset;\
            sigset_t newset;\
            __sigemptyset(&newset);\
            __sigaddset(&newset, SIGINT);\
            __sigaddset(&newset, SIGTERM);\
            __sigaddset(&newset, SIGCHLD);\
            pthread_sigmask(SIG_BLOCK, &newset, &orgset);
#define psp_restore_sigmask\
            pthread_sigmask(SIG_SETMASK, &orgset, NULL);\
        }
#else
#define psp_set_sigmask
#define psp_restore_sigmask
#endif

#define psp_thread_create(fcn, data, stack, priority, tid, ret, name)\
        {\
            pthread_attr_t attr;\
            \
            pthread_attr_init(&attr);\
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);\
            if (stack != 0) \
                pthread_attr_setstacksize(&attr, stack);\
            \
            psp_set_sigmask;\
            ret = pthread_create(&tid, &attr, (void *(*)(void *))fcn, arg);\
            psp_restore_sigmask;\
            pthread_attr_destroy(&attr);\
        }
#define psp_thread_exit(ret)         pthread_exit(ret)
#define psp_thread_self()            pthread_self()
#define psp_thread_equal(tid1, tid2) pthread_equal(tid1, tid2)

#if defined(RDM_OPENBSD)
#define psp_thread_maxpriority(tid, prio) prio = PRIO_MAX;
#define psp_thread_minpriority(tid, prio) prio = PRIO_MIN;
#else
#define psp_thread_maxpriority(tid, prio)\
        {\
            int32_t            policy;\
            struct sched_param sched;\
            \
            if (pthread_getschedparam(tid, &policy, &sched) != 0)\
                return -1;\
            \
            prio = sched_get_priority_max(policy);\
        }

#define psp_thread_minpriority(tid, prio)\
        {\
            int32_t            policy;\
            struct sched_param sched;\
            \
            if (pthread_getschedparam(tid, &policy, &sched) != 0)\
                return -1;\
            \
            prio = sched_get_priority_min(policy);\
        }
#endif

#define psp_thread_setpriority(tid, prio, ret)\
        {\
            int32_t            policy;\
            struct sched_param sched;\
            \
            if (pthread_getschedparam(tid, &policy, &sched) != 0)\
                return -1;\
            \
            sched.sched_priority = prio;\
            ret = pthread_setschedparam(tid, policy, &sched);\
        }

/*--------------------------------------------------------------------------
 * Mutex macros
 */
#define psp_mutex_init(m)             pthread_mutex_init(&m, NULL)
#define psp_mutex_shared_init(m, ret) \
        { \
            pthread_mutexattr_t attr; \
            pthread_mutexattr_init(&attr); \
            pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); \
            pthread_mutex_init(&m, &attr); \
            pthread_mutexattr_destroy(&attr); \
            ret = PSP_OKAY; \
        }

#define psp_mutex_destroy(m)          pthread_mutex_destroy(&m)
#define psp_mutex_lock(m)             pthread_mutex_lock(&m)
#define psp_mutex_trylock(m)          pthread_mutex_trylock(&m)
#define psp_mutex_unlock(m)           pthread_mutex_unlock(&m)

#define PSP_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

#if defined(RDM_ANDROID)
    #define pthread_condattr_init(a)          *(a) = 0
    #define pthread_condattr_setpshared(a, b)
    #define pthread_condattr_destroy(a)
#endif

/*--------------------------------------------------------------------------
 * Condition macros
 */
#define psp_cond_init(c, m)             pthread_cond_init(&c, NULL)
#define psp_cond_shared_init(c, m, ret) \
        { \
            pthread_condattr_t attr; \
            pthread_condattr_init(&attr); \
            pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); \
            pthread_cond_init(&c, &attr); \
            pthread_condattr_destroy(&attr); \
            ret = PSP_OKAY; \
        }
#define psp_cond_destroy(c)             pthread_cond_destroy(&c)
#define psp_cond_wait(c, m)             pthread_cond_wait(&c, &m)
#define psp_cond_timedwait(c, m, t)     psp_posix_cond_timedwait(&c, &m, t)
#define psp_cond_broadcast(c)           pthread_cond_broadcast(&c)

int16_t psp_posix_cond_timedwait(psp_cond_t *, psp_mutex_t *, uint32_t);
