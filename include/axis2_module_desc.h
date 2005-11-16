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
#ifndef AXIS2_MODULE_DESC_H
#define AXIS2_MODULE_DESC_H

/**
 * @file axis2_module_desc.h
 * @brief axis2 module_desc interface
 * <p>This holds the information about a Module. </p>
 * <ol>
 * <li>parameters<li>
 * <li>handlers<li>
 * <ol>
 * <p>Handler are registered once they are avlible but they avalibe to all 
 * services if axis2.xml has a module ref="." or avalible to a single service 
 * if services.xml have module ref=".."</p>
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>

#include <axis2_array_list.h>
#include <axis2_param_container.h>
#include <axis2_flow_container.h>
#include <axis2_param.h>
#include <axis2_operation.h>
#include <axis2_engine_config.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_module_desc_ops_s axis2_module_desc_ops_t;
typedef struct axis2_module_desc_s axis2_module_desc_t;	
	

/** @defgroup axis2_module_desc Module Description
  * @ingroup axis2_module_desc
  * @{
  */

/** 
 * @brief Module Description operations struct
 * Encapsulator struct for operations of axis2_module_desc
 */
struct axis2_module_desc_ops_s
{
};

/**
 * @brief Module Description struct
 * Axis2 Module Description
 */
struct axis2_module_desc_s
{
	axis2_module_desc_ops_t *ops;
    
};

/** create Module Description struct
 * @return pointer to newly created module description
 */
AXIS2_DECLARE(axis2_module_desc_t *) 
axis2_module_desc_create (axis2_env_t **env);

/** create Module Description struct
 * @return pointer to newly created module description
 */
AXIS2_DECLARE(axis2_module_desc_t *) 
axis2_module_desc_create_with_qname (axis2_env_t **env, 
                                        axis2_qname_t *qname);


/** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_MODULE_DESC_H */
