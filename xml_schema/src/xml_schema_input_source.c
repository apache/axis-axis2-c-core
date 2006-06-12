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
 
 #include <axis2_xml_schema_includes.h>
 #include <axis2_env.h>
 
 typedef struct xml_schema_input_source_impl 
                    xml_schema_input_source_impl_t;
 
 struct xml_schema_input_source_impl
 {
    xml_schema_input_source_t input_source;
    
    axis2_char_t *encoing;
 
    axis2_char_t *system_id;
    
    axis2_char_t *public_id;
 };
 
#define AXIS2_INTF_TO_IMPL(source) \
        ((xml_schema_input_source_impl_t*)source)
        
axis2_char_t* AXIS2_CALL
xml_schema_input_source_get_system_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env);
                
axis2_status_t AXIS2_CALL                  
xml_schema_input_source_set_system_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env,
        const axis2_char_t *system_id);
        
axis2_char_t* AXIS2_CALL
xml_schema_input_source_get_public_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
xml_schema_input_source_set_public_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env,
        const axis2_char_t *public_id);
        
axis2_status_t AXIS2_CALL
xml_schema_input_source_set_encoding(
        xml_schema_input_source_t *source,
        const axis2_env_t *env,
        const axis2_char_t *encoding);
        
axis2_char_t * AXIS2_CALL
xml_schema_input_source_get_encoding(
        xml_schema_input_source_t *source,
        const axis2_env_t *env);
                                    
axis2_status_t AXIS2_CALL
xml_schema_input_source_free
        (xml_schema_input_source_t *source,
        const axis2_env_t *env); 


AXIS2_EXTERN xml_schema_input_source_t* AXIS2_CALL
xml_schema_input_source_create(const axis2_env_t *env)
{
    xml_schema_input_source_impl_t *input_source_impl = NULL;
    
    input_source_impl = (xml_schema_input_source_impl_t *)
        AXIS2_MALLOC(env->allocator, sizeof(xml_schema_input_source_impl_t));
        
    if(!input_source_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    }        
    
    input_source_impl->input_source.ops = NULL;
    input_source_impl->public_id = NULL;
    input_source_impl->system_id = NULL;
    input_source_impl->encoing = NULL;
    
    input_source_impl->input_source.ops = (xml_schema_input_source_ops_t*)
        AXIS2_MALLOC(env->allocator, sizeof(xml_schema_input_source_ops_t));
    if(!input_source_impl->input_source.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, input_source_impl);
        return NULL;
    }        

    input_source_impl->input_source.ops->get_system_id =
        xml_schema_input_source_get_system_id;
    input_source_impl->input_source.ops->set_system_id =
        xml_schema_input_source_set_system_id;
    input_source_impl->input_source.ops->get_public_id =
        xml_schema_input_source_get_public_id;
    input_source_impl->input_source.ops->set_public_id =
        xml_schema_input_source_set_public_id;
    input_source_impl->input_source.ops->set_encoding =
        xml_schema_input_source_set_encoding;
    input_source_impl->input_source.ops->get_encoding =
        xml_schema_input_source_get_encoding;                                
    input_source_impl->input_source.ops->free =
        xml_schema_input_source_free;                
        
    return &(input_source_impl->input_source);
}

AXIS2_EXTERN xml_schema_input_source_t* AXIS2_CALL
xml_schema_input_source_create_with_system_id(
        const axis2_env_t *env,
        const axis2_char_t *id)
{
    xml_schema_input_source_impl_t *input_source_impl = NULL;
    xml_schema_input_source_t *input_source = NULL;
    
    input_source = xml_schema_input_source_create(env);
    
    input_source_impl = AXIS2_INTF_TO_IMPL(input_source);
        
    input_source_impl->system_id = AXIS2_STRDUP(id, env);
    return &(input_source_impl->input_source);
} 

axis2_status_t AXIS2_CALL
xml_schema_input_source_free
        (xml_schema_input_source_t *source,
        const axis2_env_t *env)
{
    xml_schema_input_source_impl_t *input_source_impl = NULL;
    input_source_impl = AXIS2_INTF_TO_IMPL(source);
    if(NULL != input_source_impl->public_id)
    {
        AXIS2_FREE(env->allocator, input_source_impl->public_id);
        input_source_impl->public_id = NULL;
    }
    if(NULL != input_source_impl->encoing)
    {
        AXIS2_FREE(env->allocator, input_source_impl->encoing);
        input_source_impl->encoing = NULL;
    }    
    if(NULL != input_source_impl->system_id)
    {
        AXIS2_FREE(env->allocator, input_source_impl->system_id);
        input_source_impl->system_id = NULL;
    }
    return AXIS2_FAILURE;
}       

axis2_char_t* AXIS2_CALL
xml_schema_input_source_get_system_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env)
{
    xml_schema_input_source_impl_t *input_source = NULL;
    
    input_source = AXIS2_INTF_TO_IMPL(source);
    return input_source->system_id;
}
                
axis2_status_t AXIS2_CALL                  
xml_schema_input_source_set_system_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env,
        const axis2_char_t *system_id)
{
    xml_schema_input_source_impl_t *input_source = NULL;
    input_source = AXIS2_INTF_TO_IMPL(source);
    if(NULL != input_source->system_id)
    {
        AXIS2_FREE(env->allocator, input_source->system_id);
        input_source->system_id = NULL;
    }
    input_source->system_id = AXIS2_STRDUP(system_id, env);
    return AXIS2_SUCCESS;
}
        
axis2_char_t* AXIS2_CALL
xml_schema_input_source_get_public_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env)
{
    xml_schema_input_source_impl_t *input_source = NULL;
    input_source = AXIS2_INTF_TO_IMPL(source);
    return input_source->public_id; 
}
        
axis2_status_t AXIS2_CALL
xml_schema_input_source_set_public_id(
        xml_schema_input_source_t *source,
        const axis2_env_t *env,
        const axis2_char_t *public_id)
{
    xml_schema_input_source_impl_t *input_source = NULL;
    input_source = AXIS2_INTF_TO_IMPL(source);
    if(NULL != input_source->public_id)
    {
        AXIS2_FREE(env->allocator, input_source->public_id);
        input_source->public_id = NULL;
    }
    input_source->public_id = AXIS2_STRDUP(public_id, env);
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL
xml_schema_input_source_set_encoding(
        xml_schema_input_source_t *source,
        const axis2_env_t *env,
        const axis2_char_t *encoding)
{
    xml_schema_input_source_impl_t *input_source = NULL;
    input_source = AXIS2_INTF_TO_IMPL(source);
    if(NULL != input_source->encoing)
    {
        AXIS2_FREE(env->allocator, input_source->encoing);
        input_source->encoing = NULL;
    }
    input_source->encoing = AXIS2_STRDUP(encoding, env);
    return AXIS2_SUCCESS;
}
        
axis2_char_t * AXIS2_CALL
xml_schema_input_source_get_encoding(
        xml_schema_input_source_t *source,
        const axis2_env_t *env)
{
    xml_schema_input_source_impl_t *input_source_impl = NULL;
    input_source_impl = AXIS2_INTF_TO_IMPL(source);
    return input_source_impl->encoing;
}
                                    



