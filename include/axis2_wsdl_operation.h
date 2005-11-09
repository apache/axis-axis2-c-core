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

#ifndef AXIS2_WSDL_OPERATION_H
#define AXIS2_WSDL_OPERATION_H

/**
 * @file axis2_wsdl_operation.h
 * @brief axis2 wsdl operation interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#include <axis2_qname.h>
#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_wsdl_operation_ops_s axis2_wsdl_operation_ops_t;
typedef struct axis2_wsdl_operation_s axis2_wsdl_operation_t;	
	

/** @defgroup axis2_wsdl_operation Wsdl Operation
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_wsdl_operation_ops_s
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_wsdl_operation_t *wsdl_operation,
                            axis2_env_t **env);
        
    /**
     * set the message exchange pattern
     * @param axis2_char_t message exchange pattern
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *set_msg_exchange_pattern)
                                    (axis2_wsdl_operation_t *wsdl_operation, 
                                        axis2_env_t **env, 
                                        const axis2_char_t *msg_exchange_pattern);
  
    /**
     * get the message exchange pattern
     * @return axis2_char_t message exchange pattern
     */
    axis2_char_t *(AXIS2_CALL *get_msg_exchange_pattern) 
		                                (axis2_wsdl_operation_t *wsdl_operation, 
                                            axis2_env_t **env);

    /**
     * Set the wsdl operation name
     * @param axis2_qname_t* operation name
     */
    axis2_status_t (AXIS2_CALL *set_name) (
                                        axis2_wsdl_operation_t *wsdl_operation, 
                                        axis2_env_t **env,
                                        const axis2_qname_t *name);
                                        
    /**
     * Get the name of wsdl operation
     * @return axis2_qname_t wsdl operation name
     */
    axis2_qname_t *(AXIS2_CALL *get_name) (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env);
    

    /**
     * set style
     * @param axis2_char_t style
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *set_style) (axis2_env_t *env
    , axis2_wsdl_operation_t *wsdl_operation, const axis2_char_t *style);
    
    /**
     * Get style
     * @return axis2_char_t style
     */
    axis2_char_t *(AXIS2_CALL *get_style) (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env);
    
    
    
    /**
     * Sets the properties of the operation if any
     * @param axis2_hash_t properties
     */
    axis2_status_t (AXIS2_CALL *set_component_properties) (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties);
    
    /**
     * Returns the properties that are specific to this WSDL operation
     * @return axis2_hash_t component properties
      */
    axis2_hash_t *(AXIS2_CALL *get_component_properties) (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env);
    
    /**
     * Will set the property keyed with the relavent key
     * @param void Key in the map
     * @param void value to be put
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *set_component_property) (
                                    axis2_wsdl_operation_t *wsdl_operation,
                                    axis2_env_t **env, 
                                    const void *key, 
                                    void *value);
    
    /**
     * Gets the component property
     * @param void key for the map search.
     * @return void value for the key
     */
    void *(AXIS2_CALL *get_component_property) (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env, 
                                        const void *key);

};

struct axis2_wsdl_operation_s
{
	axis2_wsdl_operation_ops_t *ops;
};

AXIS2_DECLARE(axis2_wsdl_operation_t *) axis2_wsdl_operation_create (
                                                            axis2_env_t *env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_OPERATION_FREE(wsdl_operation, env) \
		((wsdl_operation->ops)->free (wsdl_operation, env))
	
#define AXIS2_WSDL_OPERATION_GET_MSG_EXCHANGE_PATTERN(wsdl_operation, env, pattern) \
		((wsdl_operation->ops)->get_msg_exchange_pattern(wsdl_operation, env, pattern))
		
#define AXIS2_WSDL_OPERATION_SET_MSG_EXCHANGE_PATTERN(wsdl_operation, env, pattern) \
		((wsdl_operation->ops)->set_msg_exchange_pattern(wsdl_operation, env, pattern))
		
#define AXIS2_WSDL_OPERATION_GET_NAME(wsdl_operation, env, name) \
		((wsdl_operation->ops)->get_name(wsdl_operation, env, name))
		
#define AXIS2_WSDL_OPERATION_SET_NAME(wsdl_operation, env, name) \
		((wsdl_operation->ops)->set_name(wsdl_operation, env, name))
		
#define AXIS2_WSDL_OPERATION_SET_STYLE(wsdl_operation, env, style) \
		((wsdl_operation->ops)->get_style(wsdl_operation, env, style))
		
#define AXIS2_WSDL_OPERATION_GET_STYLE(wsdl_operation, env, style) \
		((wsdl_operation->ops)->set_style(wsdl_operation, env, style))
		
#define AXIS2_WSDL_OPERATION_SET_COMPONENT_PROPERTIES(wsdl_operation, env, properties) \
		((wsdl_operation->ops)->set_component_properties(wsdl_operation, env, properties))
		
#define AXIS2_WSDL_OPERATION_GET_COMPONENT_PROPERTIES(wsdl_operation, env, properties) \
		((wsdl_operation->ops)->get_component_properties(wsdl_operation, env, properties))

#define AXIS2_WSDL_OPERATION_SET_COMPONENT_PROPERTY(wsdl_operation, env, property) \
		((wsdl_operation->ops)->set_component_property(wsdl_operation, env, property))
		
#define AXIS2_WSDL_OPERATION_GET_COMPONENT_PROPERTY(wsdl_operation, env, property) \
		((wsdl_operation->ops)->get_component_property(wsdl_operation, env, property))		
		
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WSDL_OPERATION_H  */
