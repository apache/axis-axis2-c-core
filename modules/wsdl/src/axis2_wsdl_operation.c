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
 
#include <axis2_wsdl_operation.h>

typedef struct axis2_wsdl_impl_operation_s axis2_wsdl_impl_operation_t;

/** @struct axis2_wsdl_operation_s
  * @brief WSDL operation
  * This holds operations of wsdl operation
  */
struct axis2_wsdl_impl_operation_s
{
	axis2_wsdl_operation_t wsdl_opt;
	axis2_wsdl_component_t *wsdl_component;
	axis2_char_t *msg_exchange_pattern;
	axis2_char_t *style;
	axis2_qname_t *name;
	
	
};

/***************************** Function headers *******************************/

axis2_status_t axis2_wsdl_operation_free (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt);

/**
 * get the message exchange pattern
 * @return axis2_char_t* msg_exchange_pattern
 */
axis2_status_t axis2_wsdl_operation_get_msg_exchange_pattern 
		(axis2_environment_t *env, axis2_wsdl_operation_t *wsdl_opt
		, axis2_char_t **pattern);

/**
 * set the message exchange pattern
 *
 * @param axis2_char_t *message exchange pattern
 * @return status code
 */
axis2_status_t axis2_wsdl_operation_set_msg_exchange_pattern
		(axis2_environment_t *env, axis2_wsdl_operation_t *wsdl_opt
		, const axis2_char_t *msg_exchange_pattern);

/**
 * get name
 * @return axis2_qname_t** operation name
 */
axis2_status_t axis2_wsdl_operation_get_name (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_qname_t **name);

/**
 * set name
 *
 * @param axis2_qname_t* operation name
 */
axis2_status_t axis2_wsdl_operation_set_name (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const axis2_qname_t *name);

/**
 * get style
 * return axis2_char_t** style
 */
axis2_status_t axis2_wsdl_operation_get_style (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_char_t **style);

/**
 * set style
 * @param axis2_char_t* style
 * @return status code
 */
axis2_status_t axis2_wsdl_operation_set_style (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const axis2_char_t *style);
		

/**
 * Sets the properties of the operation if any
 * @param axis2_hash_t* properties
 */
axis2_status_t axis2_wsdl_operation_set_component_properties(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_hash_t *properties);

/**
 * Returns the properties that are specific to this WSDL operation
 * @return axis2_hash_t** component properties
 */
axis2_status_t axis2_wsdl_operation_get_component_properties(axis2_environment_t *env
		,axis2_wsdl_operation_t *wsdl_opt, axis2_hash_t **properties);

/**
 * Will set the property keyed with the relavent key
 * @param void* Key in the map
 * @param void* value to be put
 */
axis2_status_t axis2_wsdl_operation_set_component_property(axis2_environment_t *env
	, axis2_wsdl_operation_t *wsdl_opt, const void *key, void *value);
	

/**
 * Gets the component property
 *
 * @param void* key for the map search.
 * @return void* value for the key
 */
axis2_status_t axis2_wsdl_operation_get_component_property(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const void *key, void **value) ;


/***************************** End of function headers ************************/

axis2_status_t axis2_wsdl_operation_create
		(axis2_environment_t *env, axis2_wsdl_operation_t **wsdl_opt)
{
	axis2_wsdl_impl_operation_t *wsdl_impl_opt 
		= (axis2_wsdl_impl_operation_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_impl_operation_t));
	if(!wsdl_impl_opt)
	{
		return AXIS2_ERROR_NO_MEMORY;
	}
	
	axis2_wsdl_operation_t *wsdl_opt_l = (axis2_wsdl_operation_t *)
		axis2_malloc(env->allocator, sizeof(axis2_wsdl_operation_t));
	if(!wsdl_opt_l)
	{	
		axis2_free(env->allocator, wsdl_impl_opt);
		return AXIS2_ERROR_NO_MEMORY;
	}
	wsdl_opt_l->ops
		= (axis2_wsdl_operation_ops_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_operation_ops_t));
	if(!wsdl_opt_l->ops)
	{
		axis2_free(env->allocator, wsdl_impl_opt);
		axis2_free(env->allocator, wsdl_opt);
		return AXIS2_ERROR_NO_MEMORY;
	}
	(wsdl_opt_l->ops)->free = axis2_wsdl_operation_free;
	
	wsdl_impl_opt->wsdl_opt = *wsdl_opt_l;
	axis2_wsdl_component_create(env, &(wsdl_impl_opt->wsdl_component));
	wsdl_impl_opt->msg_exchange_pattern = NULL;
	wsdl_impl_opt->style = NULL;
	wsdl_impl_opt->name = NULL;
	
	*wsdl_opt = &(wsdl_impl_opt->wsdl_opt);

	return AXIS2_SUCCESS;	
}

/******************************************************************************/

axis2_status_t axis2_wsdl_operation_free(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt)
{
	if(!env || !wsdl_opt)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	axis2_free(env->allocator, wsdl_opt);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_wsdl_operation_get_msg_exchange_pattern 
		(axis2_environment_t *env, axis2_wsdl_operation_t *wsdl_opt
		, axis2_char_t **pattern)
{
	if(NULL == wsdl_opt)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	*pattern = ((axis2_wsdl_impl_operation_t *) wsdl_opt)->msg_exchange_pattern;
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_wsdl_operation_set_msg_exchange_pattern
		(axis2_environment_t *env, axis2_wsdl_operation_t *wsdl_opt
		, const axis2_char_t *msg_exchange_pattern)
{
	if(NULL == wsdl_opt || NULL == msg_exchange_pattern) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
		
	((axis2_wsdl_impl_operation_t *)(wsdl_opt))->msg_exchange_pattern 
		= axis2_strdup(env->string, msg_exchange_pattern);
	
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_wsdl_operation_get_name (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_qname_t **name)
{
	if(NULL == wsdl_opt)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	*name = ((axis2_wsdl_impl_operation_t *) wsdl_opt)->name;
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_wsdl_operation_set_name (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const axis2_qname_t *name)
{
	if(NULL == wsdl_opt || NULL == name) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
		
	((axis2_wsdl_impl_operation_t *)(wsdl_opt))->name = axis2_strdup(env->string
		, name);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_wsdl_operation_get_style (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_char_t **style)
{
	if(NULL == wsdl_opt) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	*style = ((axis2_wsdl_impl_operation_t *)(wsdl_opt))->style;
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_wsdl_operation_set_style (axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const axis2_char_t *style)
{
	if(NULL == wsdl_opt || NULL == style) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	((axis2_wsdl_impl_operation_t *)(wsdl_opt))->style = axis2_strdup(env->string
		, style);
	
	return AXIS2_SUCCESS;		
}

/**
 * Sets the properties of the operation if any
 * @param axis2_hash_t* properties
 */
axis2_status_t axis2_wsdl_operation_set_component_properties(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, axis2_hash_t *properties)
{
	axis2_wsdl_impl_operation_t *wsdl_impl_opt = (axis2_wsdl_impl_operation_t *)
		(wsdl_opt);
	if(NULL == wsdl_impl_opt || wsdl_impl_opt->wsdl_component || NULL == properties) 
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(env
		, wsdl_impl_opt->wsdl_component, properties))
	{
		/* do error handling here */
	}
	return AXIS2_SUCCESS;
}

/**
 * Returns the properties that are specific to this WSDL operation
 * @return axis2_hash_t** component properties
 */
axis2_status_t axis2_wsdl_operation_get_component_properties(axis2_environment_t *env
		,axis2_wsdl_operation_t *wsdl_opt, axis2_hash_t **properties)
{
	axis2_wsdl_impl_operation_t *wsdl_impl_opt = (axis2_wsdl_impl_operation_t *)
		(wsdl_opt);
	if(NULL == wsdl_impl_opt) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTIES(env
		, wsdl_impl_opt->wsdl_component, properties))
	{
		/* do error handling here */
	}
		
	return AXIS2_SUCCESS;
}
/**
 * Will set the property keyed with the relavent key
 * @param void* Key in the map
 * @param void* value to be put
 */
axis2_status_t axis2_wsdl_operation_set_component_property(axis2_environment_t *env
	, axis2_wsdl_operation_t *wsdl_opt, const void *key, void *value)
{
	axis2_wsdl_impl_operation_t *wsdl_impl_opt = (axis2_wsdl_impl_operation_t *)
		(wsdl_opt);
	if(NULL == wsdl_impl_opt || wsdl_impl_opt->wsdl_component || NULL == key 
		|| NULL == value) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(env
		, wsdl_impl_opt->wsdl_component, key, value))
	{
		/* do error handling here */
	}
	return AXIS2_SUCCESS;
}
	

/**
 * Gets the component property
 *
 * @param void* key for the map search.
 * @return void* value for the key
 */
axis2_status_t axis2_wsdl_operation_get_component_property(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt, const void *key, void **value)
{
	axis2_wsdl_impl_operation_t *wsdl_impl_opt = (axis2_wsdl_impl_operation_t *)
		(wsdl_opt);
	if(NULL == wsdl_impl_opt || NULL == key) 
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(env
		, wsdl_impl_opt->wsdl_component, key, value))
	{
		/* do error handling here */
	}
		
	return AXIS2_SUCCESS;
}
