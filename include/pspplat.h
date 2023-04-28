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

#if defined(SOLARIS)
#define SOLARIS_SPARC_32
#endif

#if defined(AIX)
#define AIX_POWERPC
#endif

#if defined(FREEBSD)
#define FREEBSD_X86
#endif

#if defined(HPUX)
#define HPUX_PARISC_32
#endif

#if defined(ISOLARIS)
#define SOLARIS_X86
#endif

#if defined(LINUX)
#define LINUX_X86
#endif

#if defined(NEUTRINO)
#define NEUTRINO_X86
#endif

#if defined(UNIXWARE)
#define UNIXWARE_X86
#endif

#if defined(VXWORKS)
#define VXWORKS_POWERPC
#endif

#define RDM_NEED_C99_VSNPRINTF /* default to using our own vsnprintf */

/* ========================================================================
   32-bit Linux on x86 (Red Hat 7.X, 8.0, 9.0)
*/
#if defined(LINUX_X86)
#define RDM_LINUX
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN4
#endif

#if defined(LINUX_X64)
#define RDM_LINUX
#define RDM_X64
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN8
#endif

#if defined(LINUX_ITANIUM_64)
#define RDM_LINUX
#define RDM_ITANIUM
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN8
#endif

#if defined(LINUX_ARM)
#define RDM_LINUX
#define RDM_ARM
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_MUSTALIGN
#define RDM_ALIGN4
#endif

#if defined(NEUTRINO_X86)
#define RDM_NEUTRINO
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSNCASECMP
#define NEED_WCSCASECMP
#define NEED_STRUPR
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(NEUTRINO_ARM)
#define RDM_NEUTRINO
#define RDM_ARM
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_MUSTALIGN
#define NEED_WCSNCASECMP
#define NEED_WCSCASECMP
#define NEED_STRUPR
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(NEUTRINO_PPC)
#define RDM_NEUTRINO
#define RDM_PPC
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_MUSTALIGN
#define NEED_WCSNCASECMP
#define NEED_WCSCASECMP
#define NEED_STRUPR
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(IPHONE_ARM)
#define RDM_IPHONE
#define RDM_DARWIN
#define RDM_ARM
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_MUSTALIGN
#define RDM_ALIGN4
#endif

#if defined(OSX_X86)
#define RDM_OSX
#define RDM_DARWIN
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(OSX_X64)
#define RDM_OSX
#define RDM_DARWIN
#define RDM_X64
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN8
#endif

#if defined(SOLARIS_X86)
#define RDM_SOLARIS
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define NEED_WCSNCASECMP
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(SOLARIS_X64)
#define RDM_SOLARIS
#define RDM_X64
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(SOLARIS_SPARC_32)
#define RDM_SOLARIS
#define RDM_SPARC
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN8
#endif

#if defined(SOLARIS_SPARC_64)
#define RDM_SOLARIS
#define RDM_SPARC
#define RDM_UNIX
#define RDM_64BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN8
#endif

#if defined(AIX_POWERPC_32)
#define RDM_AIX
#define RDM_PPC
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UTF16
#define RDM_MUSTALIGN
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN4
#endif

#if defined(AIX_POWERPC_64)
#define RDM_AIX
#define RDM_PPC
#define RDM_UNIX
#define RDM_64BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN8
#endif

#if defined(HPUX_PARISC_32)
#define RDM_HPUX
#define RDM_PARISC
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN8
#endif

#if defined(HPUX_PARISC_64)
#define RDM_HPUX
#define RDM_PARISC
#define RDM_UNIX
#define RDM_64BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN8
#endif

#if defined(HPUX_ITANIUM_32)
#define RDM_HPUX
#define RDM_ITANIUM
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN8
#endif

#if defined(HPUX_ITANIUM_64)
#define RDM_HPUX
#define RDM_ITANIUM
#define RDM_UNIX
#define RDM_64BIT
#define RDM_BIGENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_MUSTALIGN
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN8
#endif

#if defined(WINCE)
#define RDM_WINCE
#define RDM_ARM
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UTF16
#define RDM_MUSTALIGN
#define RDM_NO_DAEMON
#define NO_PREPRO
#define RDM_NO_LOCALE
#define RDM_NO_SIGNAL
#define RDM_NEED_ALTERNATE_STDFILES
#define RDM_NEEDS_WMEMCPY
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN8
#endif

#if defined(WINDOWS_X86)
#define RDM_WINDOWS
#define RDM_X86
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UTF16
#define RDM_NO_DAEMON
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN8
#endif

#if defined(WINDOWS_X64)
#define RDM_WINDOWS
#define RDM_X64
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UTF16
#define RDM_NO_DAEMON
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN8
#endif

#if defined(FREEBSD_X86)
#define RDM_FREEBSD
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(FREEBSD_X64)
#define RDM_FREEBSD
#define RDM_X64
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(NETBSD_X86)
#define RDM_NETBSD
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(NETBSD_X64)
#define RDM_NETBSD
#define RDM_X64
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(OPENBSD_X86)
#define RDM_OPENBSD
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(OPENBSD_X64)
#define RDM_OPENBSD
#define RDM_X64
#define RDM_UNIX
#define RDM_64BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(UNIXWARE_X86)
#define RDM_UNIXWARE
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN4
#endif

#if defined(VXWORKS_X86)
#define RDM_VXWORKS
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_HAVE_CLOCK_GETTIME
#define RDM_NO_UNICODE
#define NEED_STRCASECMP
#define NEED_STRNCASECMP
#define NO_MEMORY_MAP
#define MAIN MOD##_main
#define RDM_NEEDS_TIME64_T
#define RDM_NO_DAEMON
#define RDM_HAVE_SYSTEM
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(VXWORKS_POWERPC)
#define RDM_VXWORKS
#define RDM_PPC
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_HAVE_CLOCK_GETTIME
#define RDM_NO_UNICODE
#define NEED_STRCASECMP
#define NEED_STRNCASECMP
#define NO_MEMORY_MAP
#define MAIN MOD##_main
#define RDM_NEEDS_TIME64_T
#define RDM_NO_DAEMON
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(VXWORKS_SH7750)
#define RDM_VXWORKS
#define RDM_SH
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_MUSTALIGN
#define RDM_HAVE_CLOCK_GETTIME
#define RDM_NO_UNICODE
#define NEED_STRCASECMP
#define NEED_STRNCASECMP
#define NO_MEMORY_MAP
#define MAIN MOD##_main
#define RDM_NEEDS_TIME64_T
#define RDM_NO_DAEMON
#define RDM_NEEDS_STRCASESTR
#define RDM_ALIGN4
#endif

#if defined(INTEGRITY_X86)
#define RDM_INTEGRITY
#define RDM_INTEGRITY_TASKS
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define RDM_LOCALHOST "127.0.0.1"
#define NEED_STRCASECMP
#define NEED_STRNCASECMP
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_NEEDS_STRCASESTR
#define NO_MEMORY_MAP
#define NO_PREPRO
#define RDM_SHM_HEAP
#define RDM_NEEDS_TIME64_T
#define RDM_NO_DAEMON
#define RDM_ALIGN4
#endif

#if defined(INTEGRITY_POWERPC)
#define RDM_INTEGRITY
#define RDM_INTEGRITY_TASKS
#define RDM_PPC
#define RDM_UNIX
#define RDM_32BIT
#define RDM_BIGENDIAN
#define RDM_MUSTALIGN
#define RDM_WCHAR_IS_UCS4
#define RDM_LOCALHOST "127.0.0.1"
#define NEED_STRCASECMP
#define NEED_STRNCASECMP
#define NEED_WCSCASECMP
#define NEED_WCSNCASECMP
#define RDM_NEEDS_STRCASESTR
#define NO_MEMORY_MAP
#define NO_PREPRO
#define RDM_SHM_HEAP
#define RDM_NEEDS_TIME64_T
#define RDM_NO_DAEMON
#define RDM_ALIGN4
#endif

#if defined(LYNX_X86)
#define RDM_LYNX
#define RDM_X86
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_WCHAR_IS_UCS4
#define NEED_WCSNCASECMP
#define RDM_ALIGN4
#endif

#if defined(ANDROID_ARM)
#define RDM_ANDROID
#define RDM_ARM
#define RDM_UNIX
#define RDM_32BIT
#define RDM_LITTLEENDIAN
#define RDM_NO_UNICODE
#if 0
#define RDM_WCHAR_IS_UCS4
#define NEED_WCSNCASECMP
#define RDM_NEED_WCSTOMBS
#define RDM_NEED_MBSTOWCS
#endif
#define RDM_NO_SHARED_MEM
#define RDM_NO_LOCALE
#define RDM_NEEDS_TIME64_T
#define RDM_HAVE_SYSTEM
#define RDM_ALIGN4
#endif

#if !defined(RDM_LOCALHOST)
#define RDM_LOCALHOST "localhost"
#endif

#if !defined(RDM_BIGENDIAN) && !defined(RDM_LITTLEENDIAN)
#error "Must either define RDM_BIGENDIAN or RDM_LITTLEENDIAN"
#endif

#if !defined(RDM_ALIGN4) && !defined(RDM_ALIGN8)
#error "Must either define RDM_ALIGN4 or RDM_ALIGN8"
#endif

#if !defined(RDM_NO_UNICODE)
#if !defined(RDM_WCHAR_IS_UTF16) && !defined(RDM_WCHAR_IS_UCS4)
#error "Must either define RDM_WCHAR_IS_UTF16 or RDM_WCHAR_IS_UCS4"
#endif
#endif

#if !defined(MAIN)
#define MAIN main
#endif

#if defined(RDM_VXWORKS)
#define NEED_STRNCMP
#endif

#if defined(RDM_UNIX) && !defined(RDM_VXWORKS)
#if defined(RDM_SOLARIS)
#include <inttypes.h>
#elif defined(RDM_HPUX) || defined(INTEGRITY)
#include <sys/types.h>
#else
#include <stdint.h>
#endif
#elif defined(RDM_UC_FS)
#else
#if defined(RDM_VXWORKS)
#include <vxWorks.h>
#include <errno.h>
#include <fcntl.h>
#include <semLib.h>
#include <float.h>
#else
#if defined(RDM_WINCE)
#else
#include <direct.h>
#endif
#if !defined(int8_t)
typedef signed char int8_t;
#endif
#if !defined(uint8_t)
typedef unsigned char uint8_t;
#endif
#if !defined(int16_t)
typedef short int16_t;
#endif
#if !defined(uint16_t)
typedef unsigned short uint16_t;
#endif
#if !defined(int32_t)
typedef int int32_t;
#endif
#if !defined(uint32_t)
typedef unsigned int uint32_t;
#endif
#if !defined(int64_t)
#if defined(RDM_WINDOWS) || defined(RDM_WINCE)
typedef __int64 int64_t;
#else
typedef long long int64_t;
#endif
#endif
#if !defined(uint64_t)
#if defined(RDM_WINDOWS) || defined(RDM_WINCE)
typedef unsigned __int64 uint64_t;
#else
typedef unsigned long long uint64_t;
#endif
#endif
#endif

#endif


#if defined(RDM_NEEDS_TIME64_T) || defined(RDM_WINDOWS) || defined(RDM_WINCE)
typedef int64_t time64_t;
#endif

#if defined(RDM_AIX)
#define time64(a) (time64_t) time(a)
#endif

#define RDM_INT8_MIN   ((int8_t) 0x80)
#define RDM_INT8_MAX   ((int8_t) 0x7F)
#define RDM_UINT8_MAX  ((uint8_t) 0xFF)
#define RDM_INT16_MIN  ((int16_t) 0x8000)
#define RDM_INT16_MAX  ((int16_t) 0x7FFF)
#define RDM_UINT16_MAX ((uint16_t) 0xFFFF)
#define RDM_INT32_MIN  ((int32_t) 0x80000000)
#define RDM_INT32_MAX  ((int32_t) 0x7FFFFFFF)
#define RDM_UINT32_MAX ((uint32_t) 0xFFFFFFFF)
#define RDM_INT64_MIN  ( -LL(9223372036854775807) - 1)
#define RDM_INT64_MAX  ( LL(9223372036854775807) )
#define RDM_UINT64_MAX ( ULL(18446744073709551615) )

#if defined(BITSPERBYTE)
    #define BITS_PER_BYTE BITSPERBYTE
#else
    #define BITS_PER_BYTE 8
#endif

#if defined(RDM_MUSTALIGN)
#define MEMCOPY(a, b, type) memcpy(&(a), (b), sizeof(type))
#define ALTCOPY(a, b, type) memcpy((a), &(b), sizeof(type))
#else
#define MEMCOPY(a, b, type) (a) = *(const type *) (b)
#define ALTCOPY(a, b, type) *(type *) (a) = (b)
#endif

/* This helps get rid of compiler warnings on unused variables */
#define UNREF_PARM(a) ((void) a);

#if defined(__GNUC__)
#define NORETURN __attribute((noreturn))
#else
#define NORETURN
#endif

#if !defined(RDM_NO_UNICODE)
    #if defined(RDM_NEED_MBSTOWCS)
	#define atow(a, w, l) psp_utf8_to_wchar((w), (a), (l), PSP_UTF8_ERROR_BREAK)
    #else
	#define atow(a, w, l) mbstowcs((w), (a), (l))
    #endif

    #if defined(RDM_NEED_WCSTOMBS)
        #define wtoa(w, a, l) psp_wchar_to_utf8((a), (w), (l), PSP_UTF8_ERROR_BREAK)
    #else
        #define wtoa(w, a, l) wcstombs((a), (w), (l))
    #endif
#endif

#ifdef RDM_UC_FS
    #include <pspucfs.h>
#else
    typedef const void *PSP_DIR;
    #define psp_stdin stdin
    #define psp_stdout stdout
    #define psp_stderr stderr
    #define PSP_FILE FILE
    #define PSP_SEEK_SET SEEK_SET
    #define PSP_SEEK_CUR SEEK_CUR
    #define PSP_SEEK_END SEEK_END

    typedef const void *PSP_FDESC;
#endif

#if defined(RDM_UNIX)
#define NULL_FDESC ((PSP_FDESC) -1)
#else
#define NULL_FDESC ((PSP_FDESC) 0)
#endif

#if defined(RDM_WINDOWS) || defined(RDM_WINCE)
    #include "pspwin.h"
#endif

#if defined(RDM_UNIX)
    #include "pspunix.h"

    #if defined(RDM_INTEGRITY_TASKS)
        #include "pspintegrity.h"
    #elif defined(RDM_VXWORKS)
        #include "pspvxworks.h"
    #else
        #include "pspposix.h"
    #endif
#endif

#if defined(__SYMBIAN32__)
    #include "pspepoc.h"
#endif

#if !defined(SERVER_PRIORITY)
#define SERVER_PRIORITY     0
#endif
#if !defined(CONNECTION_PRIORITY)
#define CONNECTION_PRIORITY 0
#endif
#if !defined(CHKP_PRIORITY)
#define CHKP_PRIORITY       0
#endif
#if !defined(REP_LOG_PRIORITY)
#define REP_LOG_PRIORITY    0
#endif
#if !defined(SLAVE_PRIORITY)
#define SLAVE_PRIORITY      0
#endif
#if !defined(APP_PRIORITY)
#define APP_PRIORITY      0
#endif

#if !defined(LL_DEFINED)
#define LL(x) x##LL
#endif

#if !defined(ULL_DEFINED)
#define ULL(x) x##ULL
#endif

#define INTERNAL_FCN
/* function may be exposed in shared library or DLL */
#if !defined(EXTERNAL_FCN)
    #define EXTERNAL_FCN
    #define EXTVARARG_FCN
#endif

#define EXTERNAL_FCN_PTR EXTERNAL_FCN *

#if !defined(FLT_EPSILON)
    #define FLT_EPSILON     1.192092896e-07F
#endif
#if !defined(DBL_EPSILON)
    #define DBL_EPSILON     2.2204460492503131e-016
#endif

#define RDM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define RDM_MAX(a, b) ((a) > (b) ? (a) : (b))

#if defined(RDM_NO_VACOPY)
#define VA_COPY(dst, src)   memcpy(&(dst), &(src), sizeof(va_list))
#else
#define VA_COPY(dst, src)   va_copy((dst), (src))
#endif

#if !defined(PSP_STARTUP_HOOK)
#define PSP_STARTUP_HOOK
#endif

#if !defined(PSP_STARTUP_PROGRAM)
/*lint -emacro(762,PSP_STARTUP_PROGRAM) */
#define PSP_STARTUP_PROGRAM(name) \
/*lint ++flb */\
int32_t EXTERNAL_FCN name##_main(int32_t argc, const char *const *argv); \
/*lint --flb */\
int main(int32_t argc, const char *const *argv) { \
    int32_t ret; \
    \
    if ((ret = psp_init()) != PSP_OKAY) \
        return ret; \
    \
    PSP_STARTUP_HOOK \
    ret = name##_main(argc, argv); \
    psp_term(); \
    \
    return ret; \
}
#endif

#if !defined(PSP_STARTUP_TOOL)
#if defined(RDM_NO_UNICODE)
#define PSP_STARTUP_TOOL(name)
#else
#define PSP_STARTUP_TOOL(name)                                        \
    int32_t EXTERNAL_FCN name ## _mainW(                              \
        int32_t               argc,                                   \
        const wchar_t *const *wchar_argv)                             \
    {                                                                 \
        const char **argv;                                            \
        int32_t      ii;                                              \
        int32_t      status;                                          \
                                                                      \
        /* Convert the argv array to UTF8 */                          \
        status = psp_convertArgvToUTF8(argc, wchar_argv, &argv);      \
        if(status == PSP_FAILED)                                      \
            return 1;                                                 \
                                                                      \
        /* Call the UTF8 version of this function */                  \
        status = name ## _main(argc, argv);                           \
                                                                      \
        /* Free memory from UTF8 conversion */                        \
        for(ii=0; ii<argc; ii++)                                      \
        {                                                             \
            if(argv[ii])                                              \
                psp_freeMemory(argv[ii], 0);                          \
        }                                                             \
        psp_freeMemory(argv, TAG0);                                   \
                                                                      \
        return status;                                                \
    }
#endif
#endif

#if !defined(PSP_STARTUP_EXAMPLE)
#define PSP_STARTUP_EXAMPLE(name) PSP_STARTUP_PROGRAM(name)
#endif

#if !defined(PSP_STARTUP_TEST)
#define PSP_STARTUP_TEST(name) PSP_STARTUP_EXAMPLE(name)
#endif

/* PSP_STARTUP_EXAMPLE_CR is used for examples where we want to prompt the user upon termination on some platforms */
#if !defined(PSP_STARTUP_EXAMPLE_CR)
#define PSP_STARTUP_EXAMPLE_CR(name) PSP_STARTUP_EXAMPLE(name)
#endif

#if !defined(RDM_ASSERT)
#define RDM_ASSERT(x)
#endif

#if defined(RDM_WINDOWS) && defined(RDM_IMPORT_DATA)
#define RDM_EXTERN _declspec(dllimport)
#else
#define RDM_EXTERN extern
#endif /* RDM_WINDOWS */

/* Floating-point #defines ------------------------------------------------ */
#if defined(RDM_SOLARIS)
#include <ieeefp.h>
#elif defined(RDM_VXWORKS)
#define isfinite(a)         __builtin_isfinite(a)
#elif defined(RDM_WINDOWS) || defined(RDM_WINCE) || defined(RDM_VXWORKS)
#include <float.h>
#define isfinite(a)         _finite(a)
#endif

/* EXPANDABLE defines ----------------------------------------------------- */
#if defined(_lint)
#define EXPANDABLE
#define EXPANDABLE_COUNT(x) x
#else
#define EXPANDABLE 1
#define EXPANDABLE_COUNT(x) (x - 1)
#endif


