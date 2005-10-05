#ifndef AXIS2_H
#define AXIS2_H

/**
 * @file axis2.h
 * @brief Axis2c specific global declarations
 */

#include <apr_general.h>
#include <apr.h>
#include <apr_pools.h>
#include <apr_hash.h>

/* Parametrized macro definitions for memory handling */
#define MALLOC(s) (malloc((s)))
#define REALLOC(p,s) (realloc((p),(s)))
#define FREE(p) (free((p)))

/*enum boolean_s {false, true};
typedef enum boolean_s boolean_t; */

apr_hash_t *axis2_apr_hash_make(apr_pool_t *om_pool);                                        

#endif /* AXIS2_H */
