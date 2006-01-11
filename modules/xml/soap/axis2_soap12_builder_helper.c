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
 
 #include <axis2_soap12_builder_helper.h>
 #include <axis2_array_list.h>

/********************* impl struct ********************************************/
 
typedef struct axis2_soap12_builder_helper_impl_t
{
    axis2_soap12_builder_helper_t builder_helper;
    
    axis2_soap_model_builder_t *soap_model_builder;
    
    axis2_bool_t code_present;
    
    axis2_bool_t reason_present;
    
    axis2_bool_t role_present;
    
    axis2_bool_t detail_present;
    
    axis2_bool_t subcode_value_present;
    
    axis2_bool_t value_present;
    
    axis2_bool_t sub_code_present;
    
    axis2_bool_t code_processing;
    
    axis2_bool_t sub_code_processing;
    
    axis2_bool_t reason_processing;
    
    axis2_array_list_t *detail_element_names;
    

}axis2_soap12_builder_helper_impl_t;

/******************** Macro ***************************************************/

#define AXIS2_INTF_TO_IMPL(builder_helper) ((axis2_soap12_builder_helper_impl_t*)builder_helper)

/******************** function prototypes *************************************/

axis2_status_t AXIS2_CALL 
axis2_soap12_builder_helper_free(axis2_soap12_builder_helper_t *builder_helper,
                                 axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL 
axis2_soap12_builder_helper_handle_event
                            (axis2_soap12_builder_helper_t *builder_helper,
                             axis2_env_t **env,
                             axis2_soap_model_builder_t *soap_builder,
                             axis2_om_node_t *om_element_node,
                             int element_level);
                             
/******************** function implementations ********************************/

 AXIS2_DECLARE(axis2_soap12_builder_helper_t*)
axis2_soap12_builder_helper_create(axis2_env_t **env, 
                                   axis2_soap_model_builder_t *soap_builder)
{
    axis2_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_builder, NULL);
    
    builder_helper_impl = (axis2_soap12_builder_helper_impl_t*)AXIS2_MALLOC(
                            (*env)->allocator, sizeof(axis2_soap12_builder_helper_impl_t));
    if(!builder_helper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_helper_impl->code_present = AXIS2_FALSE;
    builder_helper_impl->detail_present = AXIS2_FALSE;
    builder_helper_impl->reason_present = AXIS2_FALSE;
    builder_helper_impl->role_present = AXIS2_FALSE;
    builder_helper_impl->sub_code_present = AXIS2_FALSE;
    builder_helper_impl->reason_processing = AXIS2_FALSE;
    builder_helper_impl->code_processing = AXIS2_FALSE;
    builder_helper_impl->sub_code_processing = AXIS2_FALSE;
    builder_helper_impl->detail_element_names = NULL;
    builder_helper_impl->builder_helper.ops = NULL;   
    
    builder_helper_impl->builder_helper.ops = (axis2_soap12_builder_helper_ops_t*) AXIS2_MALLOC(
                                                (*env)->allocator, sizeof(axis2_soap12_builder_helper_ops_t));
    if(!(builder_helper_impl->builder_helper.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, builder_helper_impl->builder_helper.ops);
        return NULL;
    }
    
    builder_helper_impl->builder_helper.ops->free_fn = 
            axis2_soap12_builder_helper_free;
    builder_helper_impl->builder_helper.ops->handle_event =
            axis2_soap12_builder_helper_handle_event;
            
    return &(builder_helper_impl->builder_helper);    
} 

axis2_status_t AXIS2_CALL 
axis2_soap12_builder_helper_free(axis2_soap12_builder_helper_t *builder_helper,
                                 axis2_env_t **env)
{
    axis2_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder_helper, env, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

    if(builder_helper_impl->detail_element_names)
    {
        AXIS2_ARRAY_LIST_FREE(builder_helper_impl->detail_element_names, env);
        builder_helper_impl->detail_element_names = NULL;
    }
    if(builder_helper_impl->builder_helper.ops)
    {
        AXIS2_FREE((*env)->allocator, builder_helper_impl->builder_helper.ops);
        builder_helper_impl->builder_helper.ops = NULL;
    }

    AXIS2_FREE((*env)->allocator, builder_helper_impl);
    builder_helper_impl = NULL;    
    return AXIS2_SUCCESS;
}                                 

axis2_om_node_t* AXIS2_CALL 
axis2_soap12_builder_helper_handle_event (axis2_soap12_builder_helper_t *builder_helper,
                             axis2_env_t **env,
                             axis2_soap_model_builder_t *soap_builder,
                             axis2_om_node_t *om_element_node,
                             int element_level)
{
    axis2_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder_helper, env, NULL);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);


    return NULL;
}                                                                                                                    
                               
                             
                             

