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

typedef char axis2_char_t;
typedef int axis2_bool_t;
typedef int axis2_status_t;
typedef unsigned int axis2_ssize_t;

#define AXIS2_TRUE 1
#define AXIS2_FALSE 0

/* Calling convention for exposed functions */
#if defined(WIN32) 
#define AXIS2_STORAGE_CLASS_INFO __declspec(dllexport)
#else
#define AXIS2_STORAGE_CLASS_INFO 
#endif

#if defined(__GNUC__)
#define AXIS2_CALL __attribute__((cdecl))
#else /* unix or win32 */
#if defined(__unix)
#define AXIS2_CALL
#else
#define AXIS2_CALL __stdcall
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DEFINES_H */
