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
 * @brief axis2 WSDL  operation
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
	

/** @defgroup axis2_wsdl WSDL (Axis2 wsdl)
  * @ingroup axis2
  * @{
  */


/**************************** Function pointers *******************************/

/** Deallocate memory
  * @return status code
  */
typedef axis2_status_t (*axis2_wsdl_operation_free_t)
		(axis2_env_t *env
		, axis2_wsdl_operation_t *wsdl_opt);

/**
 * get the message exchange pattern
 * @return axis2_char_t* msg_exchange_pattern
 */
typedef axis2_status_t (*axis2_wsdl_operation_get_msg_exchange_pattern_t) 
		(axis2_env_t *env, axis2_wsdl_operation_t *wsdl_opt
		, axis2_char_t **pattern);

/**
 * set the message exchange pattern
 *
 * @param axis2_char_t *message exchange pattern
 * @return status code
 */
typedef axis2_status_t (*axis2_wsdl_operation_set_msg_exchange_pattern_t)
		(axis2_env_t *env, axis2_wsdl_operation_t *wsdl_opt
		, const axis2_char_t *msg_exchange_pattern);

/**
 * get name
 * @return axis2_qname_t** operation name
 */
typedef axis2_status_t (*axis2_wsdl_operation_get_name_t) (axis2_env_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_qname_t **name);

/**
 * set name
 *
 * @param axis2_qname_t* operation name
 */
typedef axis2_status_t (*axis2_wsdl_operation_set_name_t) (axis2_env_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const axis2_qname_t *name);

/**
 * get style
 * return axis2_char_t** style
 */
typedef axis2_status_t (*axis2_wsdl_operation_get_style_t) (axis2_env_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_char_t **style);

/**
 * set style
 * @param axis2_char_t* style
 * @return status code
 */
typedef axis2_status_t (*axis2_wsdl_operation_set_style_t) (axis2_env_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const axis2_char_t *style);

/**
 * Sets the properties of the operation if any
 * @param axis2_hash_t* properties
 */
typedef axis2_status_t (*axis2_wsdl_operation_set_component_properties_t)
		(axis2_env_t *env, axis2_wsdl_operation_t *wsdl_opt
		, axis2_hash_t *properties);

/**
 * Returns the properties that are specific to this WSDL operation
 * @return axis2_hash_t** component properties
 */
typedef axis2_status_t (*axis2_wsdl_operation_get_component_properties_t)
		(axis2_env_t *env, axis2_wsdl_operation_t *wsdl_opt
		, axis2_hash_t **properties);

/**
 * Will set the property keyed with the relavent key
 * @param void* Key in the map
 * @param void* value to be put
 */
typedef axis2_status_t (*axis2_wsdl_operation_set_component_property_t)
		(axis2_env_t *env, axis2_wsdl_operation_t *wsdl_opt
		, const void *key, void *value);
	


typedef  ;
		
/*************************** End of function pointers *************************/

struct axis2_wsdl_operation_ops_s
{
	axis2_wsdl_operation_free_t free;
    
	axis2_wsdl_operation_get_msg_exchange_pattern_t get_msg_exchange_pattern;
    
	axis2_wsdl_operation_set_msg_exchange_pattern_t set_msg_exchange_pattern;
    
	axis2_wsdl_operation_get_name_t get_name;
    
	axis2_wsdl_operation_set_name_t set_name;
    
	axis2_wsdl_operation_get_style_t get_style;
    
	axis2_wsdl_operation_set_style_t set_style;
    
	axis2_wsdl_operation_set_component_properties_t set_component_properties;
    
	axis2_wsdl_operation_get_component_properties_t get_component_properties;
    
	axis2_wsdl_operation_set_component_property_t set_component_property;
    
    /**
 * Gets the component property
 *
 * @param void* key for the map search.
 * @return void* value for the key
 */
	axis2_status_t (*get_component_property)
		(axis2_env_t *env, axis2_wsdl_operation_t *wsdl_opt
		, const void *key, void **value) ;

};

struct axis2_wsdl_operation_s
{
	axis2_wsdl_operation_ops_t *ops;
};

axis2_status_t axis2_wsdl_operation_create
		(axis2_env_t *env, axis2_wsdl_operation_t **wsdl_opt);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_OPERATION_FREE(wsdl_opt, env) \
		((wsdl_opt->ops)->free (wsdl_opt, env))
	
#define AXIS2_WSDL_OPERATION_GET_MSG_EXCHANGE_PATTERN(wsdl_opt, env, pattern) \
		((wsdl_opt->ops)->get_msg_exchange_pattern(wsdl_opt, env, pattern))
		
#define AXIS2_WSDL_OPERATION_SET_MSG_EXCHANGE_PATTERN(wsdl_opt, env, pattern) \
		((wsdl_opt->ops)->set_msg_exchange_pattern(wsdl_opt, env, pattern))
		
#define AXIS2_WSDL_OPERATION_GET_NAME(wsdl_opt, env, name) \
		((wsdl_opt->ops)->get_name(wsdl_opt, env, name))
		
#define AXIS2_WSDL_OPERATION_SET_NAME(wsdl_opt, env, name) \
		((wsdl_opt->ops)->set_name(wsdl_opt, env, name))
		
#define AXIS2_WSDL_OPERATION_SET_STYLE(wsdl_opt, env, style) \
		((wsdl_opt->ops)->get_style(wsdl_opt, env, style))
		
#define AXIS2_WSDL_OPERATION_GET_STYLE(wsdl_opt, env, style) \
		((wsdl_opt->ops)->set_style(wsdl_opt, env, style))
		
#define AXIS2_WSDL_OPERATION_SET_COMPONENT_PROPERTIES(wsdl_opt, env, properties) \
		((wsdl_opt->ops)->set_component_properties(wsdl_opt, env, properties))
		
#define AXIS2_WSDL_OPERATION_GET_COMPONENT_PROPERTIES(wsdl_opt, env, properties) \
		((wsdl_opt->ops)->get_component_properties(wsdl_opt, env, properties))

#define AXIS2_WSDL_OPERATION_SET_COMPONENT_PROPERTY(wsdl_opt, env, property) \
		((wsdl_opt->ops)->set_component_property(wsdl_opt, env, property))
		
#define AXIS2_WSDL_OPERATION_GET_COMPONENT_PROPERTY(wsdl_opt, env, property) \
		((wsdl_opt->ops)->get_component_property(wsdl_opt, env, property))		
		
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WSDL_OPERATION_H  */
