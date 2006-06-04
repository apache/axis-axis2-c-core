#ifndef GUTHTHILA_DEFINES_H
#define GUTHTHILA_DEFINES_H
/**
 * @file guththila_defines.h
 * @brief Useful definitions, which may have platform concerns
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef char guththila_char_t;
    typedef unsigned char guththila_unsigned_char_t;
    typedef int guththila_bool_t;
    typedef int guththila_status_t;
    typedef unsigned int guththila_ssize_t;

#define GUTHTHILA_TRUE 1
#define GUTHTHILA_FALSE 0

/**
  *   Exporting 
  */
#if defined(WIN32)
#define GUTHTHILA_EXPORT __declspec(dllexport)
#else
#define GUTHTHILA_EXPORT
#endif

/**
  *   Importing
  */
#if defined(WIN32)
#define GUTHTHILA_IMPORT __declspec(dllimport)
#else
#define GUTHTHILA_IMPORT
#endif

/**
  *   Calling Conventions
  */
#if defined(__GNUC__)
#define GUTHTHILA_CALL __attribute__((cdecl))
#else
#if defined(__unix)
#define GUTHTHILA_CALL
#else                           /* WIN32 */
#define GUTHTHILA_CALL __stdcall
#endif
#endif

#ifdef DOXYGEN
/* define these just so doxygen documents them */

/**
 * GUTHTHILA_DECLARE_STATIC is defined when including Axis2's Core headers,
 * to provide static linkage when the dynamic library may be unavailable.
 *
 * @see GUTHTHILA_DECLARE_EXPORT
 *
 * GUTHTHILA_DECLARE_STATIC and GUTHTHILA_DECLARE_EXPORT are left undefined when
 * including Axis2's Core headers, to import and link the symbols from the 
 * dynamic Axis2 Core library and assure appropriate indirection and calling 
 * conventions at compile time.
 */
# define GUTHTHILA_DECLARE_STATIC
/**
 * GUTHTHILA_DECLARE_EXPORT is defined when building the Axis2 Core dynamic
 * library, so that all public symbols are exported.
 *
 * @see GUTHTHILA_DECLARE_STATIC
 */
# define GUTHTHILA_DECLARE_EXPORT

#endif                          /* def DOXYGEN */

#if !defined(WIN32)
/**
 * Axis2 Core functions are declared with GUTHTHILA_DECLARE(), so they may
 * use the most appropriate calling convention.  Other
 * Core functions with variable arguments must use GUTHTHILA_DECLARE_NONSTD().
 * @code
 * GUTHTHILA_DECLARE(rettype) axis2_func(args)
 * @endcode
 */
#define GUTHTHILA_DECLARE(type)            type GUTHTHILA_CALL

/**
 * Axis2 Core variable argument and hook functions are declared with 
 * GUTHTHILA_DECLARE_NONSTD(), as they must use the C language calling convention.
 * @see GUTHTHILA_DECLARE
 * @code
 * GUTHTHILA_DECLARE_NONSTD(rettype) axis2_func(args [...])
 * @endcode
 */
#define GUTHTHILA_DECLARE_NONSTD(type)     type

/**
 * Axis2 Core variables are declared with GUTHTHILA_DECLARE_DATA.
 * This assures the appropriate indirection is invoked at compile time.
 *
 * @code
 * GUTHTHILA_DECLARE_DATA type axis2_variable
 * @endcode
 */
#define GUTHTHILA_DECLARE_DATA

#elif defined(GUTHTHILA_DECLARE_STATIC)
#define GUTHTHILA_DECLARE(type)            type GUTHTHILA_CALL
#define GUTHTHILA_DECLARE_NONSTD(type)     type
#define GUTHTHILA_DECLARE_DATA
#elif defined(GUTHTHILA_DECLARE_EXPORT)
#define GUTHTHILA_DECLARE(type)            GUTHTHILA_EXPORT type GUTHTHILA_CALL
#define GUTHTHILA_DECLARE_NONSTD(type)     GUTHTHILA_EXPORT type
#define GUTHTHILA_DECLARE_DATA             GUTHTHILA_EXPORT
#else
#define GUTHTHILA_DECLARE(type)            GUTHTHILA_IMPORT type GUTHTHILA_CALL
#define GUTHTHILA_DECLARE_NONSTD(type)     GUTHTHILA_IMPORT type
#define GUTHTHILA_DECLARE_DATA             GUTHTHILA_IMPORT
#endif


#ifdef __cplusplus
}
#endif

#endif                          /* GUTHTHILA_DEFINES_H */
