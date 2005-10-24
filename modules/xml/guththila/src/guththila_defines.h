#ifndef GUTHTHILA_DEFINES_H
#define GUTHTHILA_DEFINES_H
/**
 * @file guththila_defines.h
 * @brief Useful definitions, which may have platform concerns
 */
 
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char guththila_char_t;
typedef unsigned char guththila_unsigned_char_t;
typedef int guththila_bool_t;
typedef int guththila_status_t;
typedef unsigned int guththila_ssize_t;

#define GUTHTHILA_TRUE 1
#define GUTHTHILA_FALSE 0

#ifdef __cplusplus
}
#endif

#endif /* GUTHTHILA_DEFINES_H */
