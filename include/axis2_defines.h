#ifndef AXIS2_DEFINES_H
#define AXIS2_DEFINES_H

/**
 * @file axis2_defines.h
 * @brief Useful definitions, which may have platform concerns
 */
 
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * Type definitions
  */
typedef char axis2_char_t;
typedef int axis2_bool_t;
typedef int axis2_status_t;
typedef unsigned int axis2_ssize_t;

/**
  * Boolean values
  */
#define AXIS2_TRUE 1
#define AXIS2_FALSE 0

/**
  *	Exporting 
  */
#if defined(WIN32) 
#define AXIS2_EXPORT __declspec(dllexport)
#else
#define AXIS2_EXPORT
#endif

/**
  *	Importing
  */
#if defined(WIN32) 
#define AXIS2_IMPORT __declspec(dllimport)
#else
#define AXIS2_IMPORT
#endif

/**
  *	Calling Conventions
  */
#if defined(__GNUC__)
#define AXIS2_CALL __attribute__((cdecl))
#else 
#if defined(__unix)
#define AXIS2_CALL
#else /* WIN32 */
#define AXIS2_CALL __stdcall
#endif
#endif

#ifdef DOXYGEN
/* define these just so doxygen documents them */

/**
 * AXIS2_DECLARE_STATIC is defined when including Axis2's Core headers,
 * to provide static linkage when the dynamic library may be unavailable.
 *
 * @see AXIS2_DECLARE_EXPORT
 *
 * AXIS2_DECLARE_STATIC and AXIS2_DECLARE_EXPORT are left undefined when
 * including Axis2's Core headers, to import and link the symbols from the 
 * dynamic Axis2 Core library and assure appropriate indirection and calling 
 * conventions at compile time.
 */
# define AXIS2_DECLARE_STATIC
/**
 * AXIS2_DECLARE_EXPORT is defined when building the Axis2 Core dynamic
 * library, so that all public symbols are exported.
 *
 * @see AXIS2_DECLARE_STATIC
 */
# define AXIS2_DECLARE_EXPORT

#endif /* def DOXYGEN */

#if !defined(WIN32)
/**
 * Axis2 Core functions are declared with AXIS2_DECLARE(), so they may
 * use the most appropriate calling convention.  Other
 * Core functions with variable arguments must use AXIS2_DECLARE_NONSTD().
 * @code
 * AXIS2_DECLARE(rettype) axis2_func(args)
 * @endcode
 */
#define AXIS2_DECLARE(type)            type

/**
 * Axis2 Core variable argument and hook functions are declared with 
 * AXIS2_DECLARE_NONSTD(), as they must use the C language calling convention.
 * @see AXIS2_DECLARE
 * @code
 * AXIS2_DECLARE_NONSTD(rettype) axis2_func(args [...])
 * @endcode
 */
#define AXIS2_DECLARE_NONSTD(type)     type

/**
 * Axis2 Core variables are declared with AXIS2_DECLARE_DATA.
 * This assures the appropriate indirection is invoked at compile time.
 *
 * @code
 * AXIS2_DECLARE_DATA type axis2_variable
 * @endcode
 */
#define AXIS2_DECLARE_DATA

#elif defined(AXIS2_DECLARE_STATIC)
#define AXIS2_DECLARE(type)            type AXIS2_CALL
#define AXIS2_DECLARE_NONSTD(type)     type
#define AXIS2_DECLARE_DATA
#elif defined(AXIS2_DECLARE_EXPORT)
#define AXIS2_DECLARE(type)            AXIS2_EXPORT type AXIS2_CALL
#define AXIS2_DECLARE_NONSTD(type)     AXIS2_EXPORT type
#define AXIS2_DECLARE_DATA             AXIS2_EXPORT
#else
#define AXIS2_DECLARE(type)            AXIS2_IMPORT type AXIS2_CALL
#define AXIS2_DECLARE_NONSTD(type)     AXIS2_IMPORT type
#define AXIS2_DECLARE_DATA             AXIS2_IMPORT
#endif


#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DEFINES_H */
