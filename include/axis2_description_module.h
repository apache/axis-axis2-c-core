/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_DESCRIPTION_MODULE_H
#define AXIS2_DESCRIPTION_MODULE_H

/**
  * @file axis2_description_module.h
  * @brief Parameter handling
  */

#include <axis2_qname.h>
#include <axis2_description_operation.h>
#include <axis2_engine_config.h>
#include <axis2_description_param_include.h>
/*#include <axis2_om_node.h> */

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_description DESCRIPTION (Axis2 Information model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_description_module DESCRIPTION Module
 * @ingroup axis2_description 
 * @{
 */

/***************************** Start of function macros ***********************/

/***************************** End of function macros *************************/
/**
  * This holds the information about a Module
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
        * axis2_description_module_get_name (axis2_description_module_t *
                                             module_desc);

/** To add an parameter*/
    void axis2_description_module_add_parameter (axis2_description_module_t *
                                                 module_desc,
                                                 axis2_description_param_t *
                                                 param);

/** To get all the parameters*/
    axis2_description_param_t
        *axis2_description_module_get_parameters (axis2_description_module_t *
                                                  module_desc);

/** To add an operation */
    void axis2_description_module_add_operation (axis2_description_module_t *
                                                 module_desc,
                                                 axis2_description_operation_t
                                                 * operation);

/** To get all the module_operations*/
      axis2_description_operation_t
        *
        axis2_description_module_get_operations (axis2_description_module_t *
                                                 module_desc);

    void axis2_description_module_set_parent (axis2_description_module_t *
                                              module_descr,
                                              axis2_engine_config_t * parent);

      axis2_engine_config_t
        * axis2_description_module_get_parent (axis2_description_module_t *
                                               module_desc);

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /*AXIS2_DESCRIPTION_MODULE_H */
