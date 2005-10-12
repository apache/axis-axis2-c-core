#ifndef AXIS2_DESCRIPTION_PARAM_INCLUDE_H
#define AXIS2_DESCRIPTION_PARAM_INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <axis2_om_element.h>
#include <axis2_qname.h>
#include <axis2.h>
#include <axis2_parameter.h>
#include <axis2_errno.h>
#include <apr_hash.h>

/**
 * @file axis2_description_param_include.h
 * @brief Parameter handling
 */
struct axis2_description_param_include_ops_s;
	
struct axis2_description_param_include_s;
	
typedef struct axis2_description_param_include_s 
		axis2_description_param_include_t;
	
typedef void (*axis2_description_param_include_add_parameter_t)
    (axis2_description_param_include_t *param_include,
     axis2_parameter_t * param);

typedef axis2_parameter_t *(*axis2_description_param_include_get_parameter_t)
    (axis2_description_param_include_t * param_include, char *name);

typedef apr_hash_t *(*axis2_description_param_include_get_parameters_t)
    (axis2_description_param_include_t * param_include);

typedef int (*axis2_description_param_include_is_parameter_locked_t)
    (axis2_description_param_include_t * param_include, char *param_name);

/**
  * Paramter can be any thing it can be XML element with number of child 
  * elements
  */
typedef struct axis2_description_param_include_ops_s
{
    axis2_description_param_include_add_parameter_t add_parameter;
	axis2_description_param_include_get_parameter_t get_parameter;
	axis2_description_param_include_get_parameters_t get_parameters;
	axis2_description_param_include_is_parameter_locked_t is_parameter_locked;
} axis2_description_param_include_ops_t;

struct axis2_description_param_include_s
{
    apr_hash_t *parameters;
    int is_locked;
	axis2_description_param_include_ops_t *ops;	
};

/*
 *	Create a param
 */
axis2_description_param_include_t *create_axis2_description_param_include ();

void free_axis2_description_param_include (axis2_description_param_include_t
    *param_include);

#endif //AXIS2_DESCRIPTION_PARAM_INCLUDE_H
