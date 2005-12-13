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
 
#include <axis2_wsdl_op.h>

typedef struct axis2_wsdl_op_impl axis2_wsdl_op_impl_t;

/** 
 * @brief Wsdl Component struct impl
 *	Axis2 Wsdl Component impl  
 */
struct axis2_wsdl_op_impl
{
	axis2_wsdl_op_t wsdl_op;
    /**
     * URI of the MEP
     */
	axis2_char_t *msg_exchange_pattern;
    /** 
     * Field style
     */
	axis2_char_t *style;
    /**
     * Field name
     */
	axis2_qname_t *name;
    
    /**
     * Field inputMessage
     */
    axis2_wsdl_msg_ref_t *input_msg;

    /**
     * Field outputMessage
     */
    axis2_wsdl_msg_ref_t *output_msg;

    /**
     * Field infaults
     */
    axis2_linked_list_t *infaults;

    /**
     * Field outfaults
     */
    axis2_linked_list_t *outfaults;
    
    /**
     * Field safety
     * value of parent if not specified
     */
    axis2_bool_t safety;
		
};

#define AXIS2_INTF_TO_IMPL(wsdl_op) \
        ((axis2_wsdl_op_impl_t *)(wsdl_op))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_free (axis2_wsdl_op_t *wsdl_op, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_set_msg_exchange_pattern (
		                                axis2_wsdl_op_t *wsdl_op,
                                        axis2_env_t **env,
                                        axis2_char_t *msg_exchange_pattern);

axis2_char_t * AXIS2_CALL 
axis2_wsdl_op_get_msg_exchange_pattern 
		                                (axis2_wsdl_op_t *wsdl_op, 
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_set_name (axis2_wsdl_op_t *wsdl_op, 
                                axis2_env_t **env,
		                        axis2_qname_t *name);

axis2_qname_t * AXIS2_CALL 
axis2_wsdl_op_get_name (axis2_wsdl_op_t *wsdl_op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_set_style (axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
		                            axis2_char_t *style);

axis2_char_t * AXIS2_CALL 
axis2_wsdl_op_get_style (axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env);
		

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_op_get_infaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_infaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *infaults);

axis2_wsdl_msg_ref_t *AXIS2_CALL 
axis2_wsdl_op_get_input_msg(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_input_msg(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_wsdl_msg_ref_t *input_msg);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_op_get_outfaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_outfaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *outfaults);

axis2_wsdl_msg_ref_t *AXIS2_CALL
axis2_wsdl_op_get_output_msg(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_output_msg(axis2_wsdl_op_t *wsdl_op,
                                        axis2_env_t **env,
                                        axis2_wsdl_msg_ref_t *output_msg);

axis2_bool_t AXIS2_CALL
axis2_wsdl_op_is_safe(axis2_wsdl_op_t *wsdl_op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_safety(axis2_wsdl_op_t *wsdl_op,
                                axis2_env_t **env,
                                axis2_bool_t safe);

axis2_char_t *AXIS2_CALL
axis2_wsdl_op_get_target_namespace(axis2_wsdl_op_t *wsdl_op,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_add_infault(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_wsdl_fault_ref_t *infault);

axis2_status_t AXIS2_CALL
axis2_wsdl_op_add_outfault(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_wsdl_fault_ref_t *outfault);

/***************************** End of function headers ************************/

axis2_wsdl_op_t * AXIS2_CALL 
axis2_wsdl_op_create (axis2_env_t **env)
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
	wsdl_op_impl = (axis2_wsdl_op_impl_t *) AXIS2_MALLOC ((*env)->
        allocator, sizeof(axis2_wsdl_op_impl_t));
    
	if(!wsdl_op_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    wsdl_op_impl->infaults = NULL;
    wsdl_op_impl->outfaults = NULL;
    wsdl_op_impl->wsdl_op.extensible_component = NULL;
    wsdl_op_impl->msg_exchange_pattern = NULL;
	wsdl_op_impl->style = STYLE_DOC;
	wsdl_op_impl->name = NULL;
    wsdl_op_impl->input_msg = NULL;
    wsdl_op_impl->output_msg = NULL;
    wsdl_op_impl->safety = AXIS2_FALSE;
    
    wsdl_op_impl->infaults = axis2_linked_list_create(env);
    if(NULL == wsdl_op_impl->infaults)
    {
        axis2_wsdl_op_free(&(wsdl_op_impl->wsdl_op), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    wsdl_op_impl->outfaults = axis2_linked_list_create(env);
    if(NULL == wsdl_op_impl->outfaults)
    {
        axis2_wsdl_op_free(&(wsdl_op_impl->wsdl_op), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    wsdl_op_impl->wsdl_op.extensible_component = axis2_wsdl_extensible_component_create(env);
    if(NULL == wsdl_op_impl->wsdl_op.extensible_component)
    {
        axis2_wsdl_op_free(&(wsdl_op_impl->wsdl_op), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
	wsdl_op_impl->wsdl_op.ops = (axis2_wsdl_op_ops_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_op_ops_t));
    
	if(NULL == wsdl_op_impl->wsdl_op.ops)
	{
        axis2_wsdl_op_free(&(wsdl_op_impl->wsdl_op), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
	wsdl_op_impl->wsdl_op.ops->free = axis2_wsdl_op_free;
	
    wsdl_op_impl->wsdl_op.ops->set_msg_exchange_pattern =
        axis2_wsdl_op_set_msg_exchange_pattern;
    
    wsdl_op_impl->wsdl_op.ops->get_msg_exchange_pattern =
        axis2_wsdl_op_get_msg_exchange_pattern;
    
    wsdl_op_impl->wsdl_op.ops->set_name =
        axis2_wsdl_op_set_name;
    
    wsdl_op_impl->wsdl_op.ops->get_name =
        axis2_wsdl_op_get_name;
    
    wsdl_op_impl->wsdl_op.ops->set_style =
        axis2_wsdl_op_set_style;
    
    wsdl_op_impl->wsdl_op.ops->get_style =
        axis2_wsdl_op_get_style;
    
    wsdl_op_impl->wsdl_op.ops->get_infaults =
        axis2_wsdl_op_get_infaults;
        
    wsdl_op_impl->wsdl_op.ops->set_infaults =
        axis2_wsdl_op_set_infaults;
 
    wsdl_op_impl->wsdl_op.ops->get_input_msg =
        axis2_wsdl_op_get_input_msg;
    
    wsdl_op_impl->wsdl_op.ops->set_input_msg = 
        axis2_wsdl_op_set_input_msg;
    
    wsdl_op_impl->wsdl_op.ops->get_outfaults =
        axis2_wsdl_op_get_outfaults;
        
    wsdl_op_impl->wsdl_op.ops->set_outfaults =
        axis2_wsdl_op_set_outfaults;
        
    wsdl_op_impl->wsdl_op.ops->get_output_msg =
        axis2_wsdl_op_get_output_msg;
    
    wsdl_op_impl->wsdl_op.ops->set_output_msg = 
        axis2_wsdl_op_set_output_msg;
        
    wsdl_op_impl->wsdl_op.ops->is_safe =
        axis2_wsdl_op_is_safe;
    
    wsdl_op_impl->wsdl_op.ops->set_safety = 
        axis2_wsdl_op_set_safety; 
        
    wsdl_op_impl->wsdl_op.ops->get_target_namespace = 
        axis2_wsdl_op_get_target_namespace;
        
    wsdl_op_impl->wsdl_op.ops->add_infault = 
        axis2_wsdl_op_add_infault;
        
    wsdl_op_impl->wsdl_op.ops->add_outfault = 
        axis2_wsdl_op_add_outfault;
	
	return &(wsdl_op_impl->wsdl_op);	
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_free (axis2_wsdl_op_t *wsdl_op,
                            axis2_env_t **env)
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
	AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
    
    if(NULL != wsdl_op->ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_op->ops);
        wsdl_op->ops = NULL;
    }
    
    if(NULL != wsdl_op->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(wsdl_op->extensible_component, env);
        wsdl_op->extensible_component = NULL;
    }
    
    if(NULL != wsdl_op_impl->infaults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(wsdl_op_impl->infaults, env); i++)
        {
            struct axis2_wsdl_fault_ref *fault = NULL;
            fault = AXIS2_LINKED_LIST_GET(wsdl_op_impl->infaults, env, i);
            
            fault = (struct axis2_wsdl_fault_ref *) val;
            if (fault)
               AXIS2_PHASE_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
        AXIS2_LINKED_LIST_FREE(wsdl_op_impl->infaults, env);
        wsdl_op_impl->infaults = NULL;
    }
    
    if(NULL != wsdl_op_impl->outfaults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(wsdl_op_impl->outfaults, env); i++)
        {
            struct axis2_wsdl_fault_ref *fault = NULL;
            fault = AXIS2_LINKED_LIST_GET(wsdl_op_impl->outfaults, env, i);
            
            fault = (struct axis2_wsdl_fault_ref *) val;
            if (fault)
               AXIS2_PHASE_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
        AXIS2_LINKED_LIST_FREE(wsdl_op_impl->outfaults, env);
        wsdl_op_impl->outfaults = NULL;
    }
    
    if(NULL != wsdl_op_impl->name)
    {
        AXIS2_FREE((*env)->allocator, wsdl_op_impl->name);
        wsdl_op_impl->name = NULL;
    }
    
    if(NULL != wsdl_op_impl->style)
    {
        AXIS2_FREE((*env)->allocator, wsdl_op_impl->style);
        wsdl_op_impl->style = NULL;
    }
    if(wsdl_op_impl)
    {
        AXIS2_FREE((*env)->allocator, wsdl_op_impl);
        wsdl_op_impl = NULL;
    }
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_set_msg_exchange_pattern (
		                                axis2_wsdl_op_t *wsdl_op, 
                                        axis2_env_t **env, 
		                                axis2_char_t *msg_exchange_pattern)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_exchange_pattern, AXIS2_FAILURE);
	
    axis2_char_t *pattern_l = AXIS2_STRDUP(msg_exchange_pattern, env);
    if(NULL == pattern_l)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    
	AXIS2_INTF_TO_IMPL(wsdl_op)->msg_exchange_pattern = pattern_l;
	
	return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL 
axis2_wsdl_op_get_msg_exchange_pattern (
                                        axis2_wsdl_op_t *wsdl_op, 
                                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, NULL);
    
	return AXIS2_INTF_TO_IMPL(wsdl_op)->msg_exchange_pattern;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_set_name (axis2_wsdl_op_t *wsdl_op, 
                                axis2_env_t **env,
		                        axis2_qname_t *name)
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
	
    if(wsdl_op_impl->name)
    {
        AXIS2_FREE((*env)->allocator, wsdl_op_impl->name);
        wsdl_op_impl->name = NULL;
    }
	wsdl_op_impl->name = AXIS2_STRDUP(name, env);
    if(!wsdl_op_impl->name)
    {
        return AXIS2_FAILURE;
    }
    
	return AXIS2_SUCCESS;
}


axis2_qname_t * AXIS2_CALL 
axis2_wsdl_op_get_name (axis2_wsdl_op_t *wsdl_op, 
                                axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, NULL);
    
	return AXIS2_INTF_TO_IMPL(wsdl_op)->name;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_op_set_style (axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
		                            axis2_char_t *style)
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
    
    if(wsdl_op_impl->style)
    {
        AXIS2_FREE((*env)->allocator, wsdl_op_impl->style);
        wsdl_op_impl->style = NULL;
    }
	wsdl_op_impl->style = style;
	
	return AXIS2_SUCCESS;		
}

axis2_char_t * AXIS2_CALL 
axis2_wsdl_op_get_style (axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, NULL);
	
	return AXIS2_INTF_TO_IMPL(wsdl_op)->style;
}

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_op_get_infaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_op)->infaults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_infaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *infaults) 
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, infaults, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
    
    if(wsdl_op_impl->infaults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(wsdl_op_impl->infaults, env); i++)
        {
            struct axis2_wsdl_fault_ref *fault = NULL;
            fault = AXIS2_LINKED_LIST_GET(wsdl_op_impl->infaults, env, i);
            
            fault = (struct axis2_wsdl_fault_ref *) val;
            if (fault)
               AXIS2_PHASE_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
    }
    AXIS2_INTF_TO_IMPL(wsdl_op)->infaults = infaults;
    return AXIS2_SUCCESS;
}

axis2_wsdl_msg_ref_t *AXIS2_CALL 
axis2_wsdl_op_get_input_msg(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_op)->input_msg;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_input_msg(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_wsdl_msg_ref_t *input_msg) 
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, env, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
    
    if(wsdl_op_impl->input_msg)
    {
        AXIS2_WSDL_MSG_REF_FREE(wsdl_op_impl->input_msg, env);
        wsdl_op_impl->input_msg = NULL;
    }
    wsdl_op_impl->input_msg = input_msg;
    
    return AXIS2_SUCCESS;
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_op_get_outfaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(wsdl_op)->outfaults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_outfaults(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *outfaults) 
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outfaults, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
    
    if(wsdl_op_impl->infaults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(wsdl_op_impl->outfaults, env); i++)
        {
            struct axis2_wsdl_fault_ref *fault = NULL;
            fault = AXIS2_LINKED_LIST_GET(wsdl_op_impl->outfaults, env, i);
            
            fault = (struct axis2_wsdl_fault_ref *) val;
            if (fault)
               AXIS2_PHASE_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
    }
    AXIS2_INTF_TO_IMPL(wsdl_op)->outfaults = outfaults;
    return AXIS2_SUCCESS;
}

axis2_wsdl_msg_ref_t *AXIS2_CALL
axis2_wsdl_op_get_output_msg(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_op)->output_msg;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_output_msg(axis2_wsdl_op_t *wsdl_op,
                                        axis2_env_t **env,
                                        axis2_wsdl_msg_ref_t *output_msg) 
{
    axis2_wsdl_op_impl_t *wsdl_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, output_msg, AXIS2_FAILURE);
    
    wsdl_op_impl = AXIS2_INTF_TO_IMPL(wsdl_op);
    
    if(wsdl_op_impl->output_msg)
    {
        AXIS2_WSDL_MSG_REF_FREE(wsdl_op_impl->output_msg, env);
        wsdl_op_impl->output_msg = NULL;
    }
    wsdl_op_impl->output_msg = output_msg;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_wsdl_op_is_safe(axis2_wsdl_op_t *wsdl_op,
                                axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_op)->safety;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_set_safety(axis2_wsdl_op_t *wsdl_op,
                                axis2_env_t **env,
                                axis2_bool_t safe) 
{
    AXIS2_INTF_TO_IMPL(wsdl_op)->safety = safe;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_op_get_target_namespace(axis2_wsdl_op_t *wsdl_op,
                                            axis2_env_t **env) 
{
    
    return AXIS2_QNAME_GET_URI(AXIS2_INTF_TO_IMPL(wsdl_op)->name, env);
    
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_add_infault(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_wsdl_fault_ref_t *infault) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, infault, AXIS2_FAILURE);
    
    return AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(wsdl_op)->infaults, env, infault);
}

axis2_status_t AXIS2_CALL
axis2_wsdl_op_add_outfault(axis2_wsdl_op_t *wsdl_op,
                                    axis2_env_t **env,
                                    axis2_wsdl_fault_ref_t *outfault) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outfault, AXIS2_FAILURE);
    
    return AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(wsdl_op)->outfaults, env, outfault);
}
