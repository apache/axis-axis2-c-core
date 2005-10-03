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

/**
  * Paramter can be any thing it can be XML element with number of child elements
  */
typedef struct axis2_description_param_include_t
{
    apr_hash_t *parameters;
    boolean_t is_locked;
} axis2_description_param_include_t;

/*
 *	Create a param
 */
axis2_description_param_include_t *create_axis2_description_param_include ();

void free_axis2_description_param_include (axis2_description_param_include_t
    *param_include);

void axis2_description_param_include_add_parameter
    (axis2_description_param_include_t *param_include,
     axis2_parameter_t * param);

axis2_parameter_t *axis2_description_param_include_get_parameter
    (axis2_description_param_include_t * param_include, char *name);

/* To get all the parameters in a given description */
apr_hash_t *axis2_descriptin_param_include_get_parameters
    (axis2_description_param_include_t * param_include);

/* To check whether the paramter is locked at any level */
boolean_t axis2_description_param_include_is_parameter_locked
    (axis2_description_param_include_t * param_include, char *param_name);

#endif //AXIS2_DESCRIPTION_PARAM_INCLUDE_H
