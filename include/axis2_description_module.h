#ifndef AXIS2_DESCRIPTION_MODULE_H
#define AXIS2_DESCRIPTION_MODULE_H

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#include <axis2_qname.h>
#include <axis2_description_operation.h>
#include <axis2_engine_config.h>
#include <axis2_description_param_include.h>
/*#include <axis2_om_node.h> */

/**
 * @file axis2_description_module.h
 * @brief Parameter handling
 */

/**
 * <p>This holds the information about a Module. </p>
 */
typedef struct
{
    axis2_qname_t *qname;
    axis2_engine_config_t *parent;
    axis2_description_param_t *paras;
    axis2_description_operation_t *operation_desc;

} axis2_description_module_t;

void axis2_description_module_free (axis2_description_module_t *
                                     module_description);

/** Create a module description */
axis2_description_module_t *axis2_description_module_create ();

/** To set a name */
void setName (axis2_description_module_t * module_description,
              axis2_qname_t * name);

axis2_qname_t
    *axis2_description_module_get_name (axis2_description_module_t *
                                         module_desc);

/** To add an parameter*/
void axis2_description_module_add_parameter (axis2_description_module_t *
                                              module_desc,
                                              axis2_description_param_t *param);

/** To get all the parameters*/
axis2_description_param_t *axis2_description_module_get_parameters (axis2_description_module_t *
                                               module_desc);

/** To add an operation */
void axis2_description_module_add_operation (axis2_description_module_t *
                                              module_desc,
                                              axis2_description_operation_t *
                                              operation);

/** To get all the module_operations*/
axis2_description_operation_t
    * axis2_description_module_get_operations (axis2_description_module_t *
                                                module_desc);

void axis2_description_module_set_parent (axis2_description_module_t *
                                           module_descr,
                                           axis2_engine_config_t *
                                           parent);

axis2_engine_config_t
    *axis2_description_module_get_parent (axis2_description_module_t *
                                           module_desc);


#endif /*AXIS2_DESCRIPTION_MODULE_H */
