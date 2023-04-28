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

#include <errno.h>
#include <fcntl.h>
#include <float.h>

#if defined(RDM_HPUX)
#include <sys/time.h>
#endif
#include <time.h>
#include <locale.h>

#if defined(NEUTRINO) || defined(RDM_VXWORKS) || defined(INTEGRITY)
#define NO_UNIX_DOMAIN
#endif

#if !defined(RDM_NO_UNICODE)
    #include <wchar.h>
#endif

#if defined(RDM_SOLARIS)
#define _POSIX_C_SOURCE 200112
#define __EXTENSIONS__
#endif

#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#if !defined(RELEASE) || defined(_lint)
#include <assert.h>
#define RDM_ASSERT(x) assert(x);
#else
#define RDM_ASSERT(x)
#endif

#if !defined(RDM_NEUTRINO) && !defined(RDM_VXWORKS) && !defined(RDM_OSX) && \
        !defined(RDM_FREEBSD) && !defined(RDM_IPHONE) && \
        !defined(RDM_INTEGRITY) && !defined(RDM_VXWORKS_RTP) && \
        !defined(RDM_NETBSD) && !defined(RDM_OPENBSD) && \
        !defined(RDM_LYNX) && !defined(RDM_ANDROID)
    #include <values.h>
#endif

#if defined(RDM_SOLARIS)
    /* undef is necessary to include wsncasecmp() in widec.h */
    #undef _POSIX_C_SOURCE
    #if !defined(RDM_NO_UNICODE)
        #include <wchar.h>
        #include <widec.h>
    #endif
#endif

#if defined(RDM_AIX)
    #include <strings.h>
    #if !defined(RDM_NO_UNICODE)
        #include <wcstr.h>
        #define getwc    getw
    #endif
#endif

#if defined(RDM_FREEBSD)
    typedef int32_t LONG;
    #define ETIME ETIMEDOUT
    #if !defined(RDM_NO_UNICODE)
        #include <wctype.h>
    #endif
#endif

#if defined(RDM_OPENBSD)
    #define ETIME ETIMEDOUT
#endif

#if defined(RDM_HPUX)
    #define RDM_NO_VACOPY
    #include <strings.h>

    #if !defined(RDM_NO_UNICODE)
        #define getwc getw
    #endif
    #include <inttypes.h>
    #if defined(__LP64__)
        #define strtoll  strtol
        #define strtoull strtoul
    #else
        #define strtoll  __strtoll
        #define strtoull __strtoull
    #endif
#endif

#if defined(RDM_LINUX)
    #if !defined(__USE_GNU)
        #define __USE_GNU
    #endif

    #if !defined(_GNU_SOURCE)
        #define _GNU_SOURCE
    #endif

    #if !defined(RDM_NO_UNICODE)
        #include <wctype.h>
        #define getwc getw
    #endif
#endif

#if defined(RDM_NEUTRINO)
    #if !defined(RDM_NO_UNICODE)
        #define getwc getw
        #include <wctype.h>
    #endif
#endif

#if defined(RDM_INTEGRITY)
    #if !defined(RDM_NO_UNICODE)
        #include <wctype.h>
    #endif
#endif

#if defined(RDM_UNIXWARE)
    #include <strings.h>
    #if !defined(RDM_NO_UNICODE)
        #include <wctype.h>
    #endif

    typedef size_t socklen_t;
#endif

#if defined(RDM_OSX) || defined(RDM_IPHONE)
    typedef long LONG;
    #if !defined(RDM_NO_UNICODE)
        #include <wctype.h>
    #endif
#endif

#if defined(RDM_64BIT) && !defined(RDM_OSX)
#define RDM_64BIT_CODE "l"
#else
#define RDM_64BIT_CODE "ll"
#endif

#define MIN_FILE_LIMIT 0x400

#define INT64_FMT      "%"RDM_64BIT_CODE"d"
#define UINT64_FMT     "%"RDM_64BIT_CODE"u"
#define INT64_FMT_LEN  "%*"RDM_64BIT_CODE"d"
#define UINT64_FMT_LEN "%*"RDM_64BIT_CODE"u"
#define INT64_FMT_HEX      "%"RDM_64BIT_CODE"X"
#define UINT64_FMT_HEX     "%"RDM_64BIT_CODE"X"
#define INT64_FMT_LEN_HEX  "%*"RDM_64BIT_CODE"X"
#define UINT64_FMT_LEN_HEX "%*"RDM_64BIT_CODE"X"

#define INT64_SPEC_HEX      "%"RDM_64BIT_CODE"X"
#define UINT64_SPEC_HEX     "%"RDM_64BIT_CODE"X"
#define INT64_SPEC_LEN_HEX  "%*"RDM_64BIT_CODE"X"
#define UINT64_SPEC_LEN_HEX "%*"RDM_64BIT_CODE"X"
#define INT64_SPEC      "%"RDM_64BIT_CODE"d"
#define UINT64_SPEC     "%"RDM_64BIT_CODE"u"
#define INT64_SPEC_LEN  "%*"RDM_64BIT_CODE"d"
#define UINT64_SPEC_LEN "%*"RDM_64BIT_CODE"u"
#define INT64_SPEC_SIZE(size)  "%" size RDM_64BIT_CODE"d"
#define UINT64_SPEC_SIZE(size) "%" size RDM_64BIT_CODE"u"
#define INT64_SPEC_SIZE_HEX(size)  "%" size RDM_64BIT_CODE"X"
#define UINT64_SPEC_SIZE_HEX(size) "%" size RDM_64BIT_CODE"X"

#if defined(RDM_VXWORKS)
#define NEED_PSP_ATOI64
#define NEED_PSP_ATOUI64
#else
#define psp_atoi64(a)  strtoll(a, NULL, 10)
#define psp_atoui64(a) strtoull(a, NULL, 10)
#endif

#define PUBLIC_CLASS

#if !defined(RDM_NEUTRINO)
#define NEED_STRUPR
#endif

#if !defined(RDM_SOLARIS)
    #define MILLISEC    1000
    #define MICROSEC    1000000
    #define NANOSEC     1000000000
#endif

#if !defined(RDM_NO_UNICODE)
    #define psp_wcsncoll   wcsncmp
    #define psp_wcsnicoll  wcsnicmp
    #define psp_wcscoll    wcscmp
#endif
#if defined(RDM_VXWORKS)
#define psp_mkdir(a, b)      mkdir(a)
#else
#define psp_mkdir(a, b)      mkdir(a, b)
#endif
#define psp_rmdir     rmdir
#define psp_strncoll(a, b, c)  strcoll(a, b)
#define psp_strnicoll(a, b, c) strcoll(a, b)
#define psp_mbsncmp   strncmp
#define psp_mbsnicmp  strnicmp
#define stricmp       psp_strcasecmp
#define strnicmp      psp_strncasecmp
#define wcsicmp       psp_wcscasecmp
#define wcsnicmp      psp_wcsncasecmp

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NEED_STRCASECMP)
int32_t psp_strcasecmp(const char *s1, const char *s2);
#else
#define psp_strcasecmp  strcasecmp
#endif

#if defined(NEED_STRNCASECMP)
int32_t psp_strncasecmp(const char *s1, const char *s2, size_t n);
#else
#define psp_strncasecmp strncasecmp
#endif

#if defined(RDM_NEEDS_STRCASESTR)
char *psp_strcasestr(const char *haystack, const char *needle);
#else
#define psp_strcasestr  strcasestr
#endif

#if defined(NEED_WCSCASECMP)
int32_t psp_wcscasecmp(const wchar_t *w1, const wchar_t *w2);
#elif defined(RDM_SOLARIS)
#define psp_wcscasecmp   wscasecmp
#else
#define psp_wcscasecmp   wcscasecmp
#endif

#if defined(NEED_WCSNCASECMP)
int32_t psp_wcsncasecmp(const wchar_t *w1, const wchar_t *w2, size_t n);
#elif defined(RDM_SOLARIS)
#define psp_wcsncasecmp   wsncasecmp
#else
#define psp_wcsncasecmp   wcsncasecmp
#endif

#ifdef __cplusplus
}
#endif

#define fnupper(a) (a)
#define DIRCHAR '/'
