
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXUTIL_UTILS_DEFINES_H
#define AXUTIL_UTILS_DEFINES_H

#include <stddef.h>

#if !defined(WIN32)
#include <stdint.h> 
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(WIN32) && !defined(AXIS2_SKIP_INT_TYPEDEFS)
    /**
     * ANSI Type definitions for Windows
     */
    typedef unsigned __int8 uint8_t;
    typedef __int8 int8_t;
    typedef unsigned __int16 uint16_t;
    typedef __int16 int16_t;
    typedef unsigned __int32 uint32_t;
    typedef __int32 int32_t;
    typedef unsigned __int64 uint64_t;
    typedef __int64 int64_t;
#endif

/**
 * Definition of format specifiers
 * for printf family of functions
 */

#if defined(WIN32)
#define AXIS2_PRINTF_INT64_FORMAT_SPECIFIER "%I64d"
#define AXIS2_PRINTF_UINT64_FORMAT_SPECIFIER "%I64u"
#define AXIS2_PRINTF_INT32_FORMAT_SPECIFIER "%I32d"
#define AXIS2_PRINTF_UINT32_FORMAT_SPECIFIER "%I32u"
#else
#if __WORDSIZE == 64
#define AXIS2_PRINTF_INT64_FORMAT_SPECIFIER "%ld"
#define AXIS2_PRINTF_UINT64_FORMAT_SPECIFIER "%lu"
#else
#define AXIS2_PRINTF_INT64_FORMAT_SPECIFIER "%lld"
#define AXIS2_PRINTF_UINT64_FORMAT_SPECIFIER "%llu"
#endif
#define AXIS2_PRINTF_INT32_FORMAT_SPECIFIER "%d"
#define AXIS2_PRINTF_UINT32_FORMAT_SPECIFIER "%u"
#endif

    /**
      * Type definitions
      */
    typedef char axis2_char_t;
    typedef int axis2_bool_t;
    typedef int axis2_status_t;
    typedef int axis2_scope_t;
    typedef unsigned int axis2_ssize_t;
    typedef char axis2_byte_t;
	typedef unsigned char axis2_unsigned_byte_t;

#define AXIS2_STRING(s) s

#define AXIS2_CHAR(c) c

#define AXIS2_CRLF_LENGTH 2

#define AXIS2_CRLF "\r\n"

    /* These constant definitions should later be moved to platform dependant
     * files
     */

#define AXIS2_EOLN '\0'

    /**
      * Boolean values
      */
#define AXIS2_TRUE 1
#define AXIS2_FALSE 0

    /**
      *   Exporting
      */
#if defined(WIN32) && !defined(AXIS2_DECLARE_STATIC)
#define AXIS2_EXPORT __declspec(dllexport)
#else
#define AXIS2_EXPORT
#endif

    /**
      *   Importing
      */
#if defined(WIN32)
#define AXIS2_IMPORT __declspec(dllimport)
#else
#define AXIS2_IMPORT
#endif

    /**
      *   Calling Conventions
      */
#if defined(__GNUC__)
#if defined(__i386)
#define AXIS2_CALL __attribute__((cdecl))
#define AXIS2_WUR __attribute__((warn_unused_result))
#else
#define AXIS2_CALL
#define AXIS2_WUR


#endif
#else
#if defined(__unix)
#define AXIS2_CALL
#define AXIS2_WUR


#else                           /* WIN32 */
#define AXIS2_CALL __stdcall
#define AXIS2_WUR
#endif
#endif
#define AXIS2_THREAD_FUNC AXIS2_CALL


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

#endif                          /* def DOXYGEN */

#if !defined(WIN32)

    /**
     * Axis2 Core functions are declared with AXIS2_EXTERN  AXIS2_CALL

       , so they may
     * use the most appropriate calling convention.  Other
     * Core functions with variable arguments must use AXIS2_DECLARE_NONSTD().
     * @code
     * AXIS2_EXTERN rettype) axis2_func(args AXIS2_CALL


     * @endcode
     */
#define AXIS2_EXTERN

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
#define AXIS2_EXTERN
#define AXIS2_EXTERN_NONSTD
#define AXIS2_DECLARE_DATA
#elif defined(AXIS2_DECLARE_EXPORT)
#define AXIS2_EXTERN                    AXIS2_EXPORT
#define AXIS2_EXTERN_NONSTD             AXIS2_EXPORT
#define AXIS2_DECLARE_DATA
#else
#define AXIS2_EXTERN                    AXIS2_IMPORT
#define AXIS2_EXTERN_NONSTD             AXIS2_IMPORT
#define AXIS2_DECLARE_DATA
#endif

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_UTILS_DEFINES_H */
