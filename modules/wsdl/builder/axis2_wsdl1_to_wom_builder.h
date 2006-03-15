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

#ifndef AXIS2_TYPE_TABLE_H
#define AXIS2_TYPE_TABLE_H

/**
 * @file axis2_wsdl1_to_wom_builder.h
 * @brief axis2 wsdl1_to_wom_builder interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_wsdl1_to_wom_builder_ops axis2_wsdl1_to_wom_builder_ops_t;
typedef struct axis2_wsdl1_to_wom_builder axis2_wsdl1_to_wom_builder_t;	
	

/** @defgroup axis2_wsdl1_to_wom_builder Wsdl1 To Wom Builder
  * @ingroup axis2_wsdl1_to_wom_builder
  * @{
  */

/** 
 * @brief Wsdl1 To Wom Builder ops struct
 * Encapsulator struct for ops of axis2_wsdl1_to_wom_builder
 */
struct axis2_wsdl1_to_wom_builder_ops
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_wsdl1_to_wom_builder_t *wsdl1_to_wom_builder,
                            axis2_env_t **env);

};

/** 
 * @brief Wsdl1 To Wom Builder struct
 *	Wsdl1 To Wom Builder  
 */ 
struct axis2_wsdl1_to_wom_builder
{
	axis2_wsdl1_to_wom_builder_ops_t *ops;
};

/**
 * Creates wsdl1_to_wom_builder struct
 * @return pointer to newly created wsdl1_to_wom_builder
 */
AXIS2_DECLARE(axis2_wsdl1_to_wom_builder_t *) 
axis2_wsdl1_to_wom_builder_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_TYPE_TABLE_FREE(wsdl1_to_wom_builder, env) ((wsdl1_to_wom_builder)->ops->free (wsdl1_to_wom_builder, env))

#define AXIS2_TYPE_TABLE_ADD_HANDLER(wsdl1_to_wom_builder, env, handler) \
		((wsdl1_to_wom_builder)->ops->add_handler (wsdl1_to_wom_builder, env, handler))

#define AXIS2_TYPE_TABLE_GET_HANDLER(wsdl1_to_wom_builder, env, index) \
		((wsdl1_to_wom_builder)->ops->get_handler (wsdl1_to_wom_builder, env, index))

#define AXIS2_TYPE_TABLE_GET_HANDLER_COUNT(wsdl1_to_wom_builder, env) \
		((wsdl1_to_wom_builder)->ops->get_handler_count (wsdl1_to_wom_builder, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TYPE_TABLE_H */
