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

#if defined(RDM_WINDOWS) || (defined(RDM_WINCE) && !defined(RDM_UC_FS))

#define WIN32_LEAN_AND_MEAN
#include <stddef.h>
#include <time.h>
#include <locale.h>

#if defined(RDM_WINCE)
    #include <windows.h>
    #define O_RDONLY 0x0000
    #define O_WRONLY 0x0001
    #define O_RDWR   0x0002
    #define O_APPEND 0x0008
    #define O_CREAT  0x0100
    #define O_TRUNC  0x0200
    #define O_EXCL   0x0400

    #define strerror(errno) "no error information"
#if !defined(__cplusplus)
    #define EACCES          ERROR_ACCESS_DENIED
    #define EPIPE           ERROR_ACCESS_DENIED
    #define ENOENT          ERROR_FILE_NOT_FOUND
    #define EINVAL          -1
#endif

    #define psp_mbsncmp    strncmp
    #define psp_mbsnicmp   strnicmp
    #define psp_mbsncoll   strncoll
    #define psp_mbsnicoll  strnicoll
    #define snprintf       _snprintf
    #define stricmp        _stricmp
    #define strnicmp       _strnicmp
    #define psp_wcsncoll   wcsncmp
    #define psp_wcsnicoll  wcsnicmp
    #define strlwr         _strlwr
    #define strupr         _strupr
    #define psp_wcscoll  wcscmp
#else
    #include <fcntl.h>
    #include <errno.h>
    #include <mbstring.h>
    #include <sys\timeb.h>
    #define snprintf      _snprintf
    #define psp_mbsncmp(a, b, c)   _mbsncmp((const uint8_t *) (a), (const uint8_t *) (b), (c))
    #define psp_mbsnicmp(a, b, c)  _mbsnicmp((const uint8_t *) (a), (const uint8_t *) (b), (c))
    #define psp_strncoll _strncoll
    #define psp_strnicoll _strnicoll
    #if !defined(RDM_NO_UNICODE)
        #define psp_wcsncoll   _wcsncoll
        #define psp_wcsnicoll  _wcsnicoll
        #define psp_wcscoll  wcscoll
    #endif
#endif

#if _MSC_VER < 1400
/* Versions prior to VS2005 only support a traditional K&R preprocessors */
#define RDM_TRADITIONAL_CPP
#define RDM_NEEDS_WMEMCPY
#endif

#if !defined(RELEASE) || defined(_lint)
#include <assert.h>
#define RDM_ASSERT(x) /*lint -save -e909 */ assert(x); /*lint -restore */
#else
#define RDM_ASSERT(x)
#endif

#if !defined(_UINTPTR_T_DEFINED)
    #if defined(RDM_32BIT)
        typedef uint32_t uintptr_t;
    #else /* defined(RDM_64BIT) */
        typedef uint64_t uintptr_t;
    #endif
    #define _UINTPTR_T_DEFINED
#endif

#if !defined(_INTPTR_T_DEFINED)
    #if defined(RDM_32BIT)
        typedef int32_t intptr_t;
    #else /* defined(RDM_64BIT) */
        typedef int64_t intptr_t;
    #endif
    #define _INTPTR_T_DEFINED
#endif

#define RDM_64BIT_FILES
#define RDM_NO_VACOPY
#if _MSC_VER < 1400
#define RDM_NO_VARIADIC_MACROS
#endif

#define PUBLIC_CLASS __declspec(dllexport)
#define EXTERNAL_FCN __stdcall
#define EXTVARARG_FCN __cdecl

#define INT64_FMT_HEX              "%I64X"
#define UINT64_FMT_HEX             "%I64X"
#define INT64_FMT_LEN_HEX          "%*I64X"
#define UINT64_FMT_LEN_HEX         "%*I64X"
#define INT64_FMT                  "%I64d"
#define UINT64_FMT                 "%I64u"
#define INT64_FMT_LEN              "%*I64d"
#define UINT64_FMT_LEN             "%*I64u"

#define INT64_SPEC_HEX             "%I64X"
#define UINT64_SPEC_HEX            "%I64X"
#define INT64_SPEC_LEN_HEX         "%*I64X"
#define UINT64_SPEC_LEN_HEX        "%*I64X"
#define INT64_SPEC                 "%I64d"
#define UINT64_SPEC                "%I64u"
#define INT64_SPEC_LEN             "%*I64d"
#define UINT64_SPEC_LEN            "%*I64u"
#define INT64_SPEC_SIZE_HEX(size)  "%" size "I64X"
#define UINT64_SPEC_SIZE_HEX(size) "%" size "I64X"
#define INT64_SPEC_SIZE(size)      "%" size "I64d"
#define UINT64_SPEC_SIZE(size)     "%" size "I64u"

#if _MSC_VER < 1310
    #define LL(x)  x
    #define LL_DEFINED
    #define ULL(x) x
    #define ULL_DEFINED

    #define NO_UINT64_TO_REAL_CONV 1
#endif

#define psp_atoi64(a)  _atoi64(a)
#if defined(RDM_WINCE)
    #define NEED_PSP_ATOUI64
#else
    #define psp_atoui64(a) _strtoui64(a, NULL, 10)
#endif

#if defined(RDM_NEEDS_STRCASESTR)
    char *EXTERNAL_FCN psp_strcasestr(const char *haystack, const char *needle);
#else
    #define psp_strcasestr  strcasestr
#endif

#if !defined(RDM_NO_UNICODE) && !defined(RDM_WINCE)
    #define psp_wcscoll    wcscoll
    #define psp_wcsncoll   _wcsncoll
    #define psp_wcsnicoll  _wcsnicoll
#endif

#define psp_mkdir(a, b) _mkdir(a)

#define popen _popen
#define pclose _pclose

#define fnupper(a)    toupper(a)
#define DIRCHAR '\\'

#define psp_thread_t int32_t

/* PSP_STARTUP_EXAMPLE_CR we want to prompt the user upon termination */
#define PSP_STARTUP_EXAMPLE_CR(name)                                     \
/*lint ++flb */                                                          \
int32_t EXTERNAL_FCN name##_main(int32_t argc, const char *const *argv); \
/*lint --flb */                                                          \
int main(int32_t argc, const char *const *argv) {                        \
    int32_t ret;                                                         \
                                                                         \
    if ((ret = psp_init()) != PSP_OKAY)                                  \
        return ret;                                                      \
                                                                         \
    PSP_STARTUP_HOOK                                                     \
    ret = name##_main(argc, argv);                                       \
    psp_term();                                                          \
    puts("Press RETURN to exit.");                                       \
    getchar();                                                           \
                                                                         \
    return ret;                                                          \
}

#if defined(RDM_WINCE)
void psp_setArgs (wchar_t *argv[]);
#define PSP_STARTUP_PROGRAM(name)                                     \
    int32_t EXTERNAL_FCN name ## _mainW(int32_t, const wchar_t *const *); \
    int32_t WINAPI WinMain (HINSTANCE hInstance,                      \
                            HINSTANCE hPrevInstance,                  \
                            LPWSTR    lpCmdLine,                      \
                            int       nCmdShow) {                     \
        int32_t  argc= 1;                                             \
        wchar_t *argv[11];                                            \
        int32_t  ret;                                                 \
        argv[0]= L ## #name;                                          \
                                                                      \
        MessageBox(NULL, L"Start Program:  Attach a debugger, "       \
                   L"break in psp_setArgs to set arguments.",         \
                   L ## #name, 0);                                    \
        psp_setArgs (argv);                                           \
                                                                      \
        for (argc= 1; argc <= 10; argc++) {                           \
            if (*argv[argc] == L'\0') {                               \
                break;                                                \
            }                                                         \
        }                                                             \
                                                                      \
        ret= psp_init ();                                             \
        if (ret == PSP_OKAY) {                                        \
            ret= name ## _mainW(argc, argv);                           \
            MessageBox(NULL, L"Terminate program", L ## #name, 0);    \
            psp_term ();                                              \
        }                                                             \
        return ret;                                                   \
    }
#define PSP_STARTUP_EXAMPLE(name) PSP_STARTUP_TOOL(name) PSP_STARTUP_PROGRAM(name)
#endif

#endif /* RDM_WINDOWS && !RDM_UC_FS */
