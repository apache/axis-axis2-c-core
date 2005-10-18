#ifndef AXIS2_MODULE_DESCRIPTION_H
#define AXIS2_MODULE_DESCRIPTION_H

#include <stdio.h>
#include <stdlib.h>
#include <axis2.h>
#include <axis2_errno.h>
#include <axis2_qname.h>
#include <axis2_operation_description.h>
#include <axis2_engine_config.h>
#include <axis2_description_param_include.h>
#include <axis2_om_node.h>
#include <apr_hash.h>

/**
 * @file axis2_module_description.h
 * @brief Parameter handling
 */

/**
 * <p>This holds the information about a Module. </p>
 */
typedef struct
{
    axis2_qname_t *qname;
    axis2_engine_config_t *parent;
    axis2_parameter_t *parameters;
    axis2_operation_description_t *operations;

} axis2_module_description_t;

void axis2_module_description_free (axis2_module_description_t *
                                     module_description);

/** Create a module description */
axis2_module_description_t *axis2_module_description_create ();

/** To set a name */
void setName (axis2_module_description_t * module_description,
              axis2_qname_t * name);

axis2_qname_t
    *axis2_module_description_get_name (axis2_module_description_t *
                                         module_description);

/** To add an parameter*/
void axis2_module_description_add_parameter (axis2_module_description_t *
                                              module_description,
                                              axis2_parameter_t * param);

/** To get all the parameters*/
axis2_parameter_t *axis2_module_description_get_parameters (axis2_module_description_t *
                                               module_description);

/** To add an operation */
void axis2_module_description_add_operation (axis2_module_description_t *
                                              module_description,
                                              axis2_operation_description_t *
                                              operation);

/** To get all the module_operations*/
axis2_operation_description_t
    * axis2_module_description_get_operations (axis2_module_description_t *
                                                module_description);

void axis2_module_description_set_parent (axis2_module_description_t *
                                           module_description,
                                           axis2_engine_config_t *
                                           parent);

axis2_engine_config_t
    *axis2_module_description_get_parent (axis2_module_description_t *
                                           module_description);


#endif //AXIS2_MODULE_DESCRIPTION_H
