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

#include <axis2_xml_schema_complex_content.h>

typedef struct axis2_xml_schema_complex_content_impl 
                    axis2_xml_schema_complex_content_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_complex_content_impl
{
    axis2_xml_schema_complex_content_t complex_content;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    void *content;
    
    int content_type;
    
    axis2_bool_t mixed;
};

#define AXIS2_INTF_TO_IMPL(complex_content) \
        ((axis2_xml_schema_complex_content_impl_t *) complex_content)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_complex_content_free(void *complex_content,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_complex_content_get_base_impl(void *complex_content,
                                        axis2_env_t **env);

void* AXIS2_CALL
axis2_xml_schema_complex_content_get_content(void *complex_content,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_set_content(void *complex_content,
                                            axis2_env_t **env,
                                            void* content,
                                            int content_type);

int AXIS2_CALL
axis2_xml_schema_complex_content_get_content_type(void *complex_content,
                                                 axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_xml_schema_complex_content_to_string(void *complex_content,
                                          axis2_env_t **env,
                                          axis2_char_t *prefix, 
                                          int tab);
                                          
axis2_bool_t AXIS2_CALL
axis2_xml_schema_complex_content_is_mixed(void *complex_content,
                                          axis2_env_t **env);
                                                                                    
                                          

AXIS2_DECLARE(axis2_xml_schema_complex_content_t *)
axis2_xml_schema_complex_content_create(axis2_env_t **env)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    complex_content_impl = (axis2_xml_schema_complex_content_impl_t*)AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_xml_schema_complex_content_impl_t));
    if(!complex_content_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    complex_content_impl->annotated = NULL;
    complex_content_impl->methods = NULL;
    complex_content_impl->content = NULL;
    complex_content_impl->content_type = -1;
    complex_content_impl->mixed = AXIS2_FALSE;
    complex_content_impl->complex_content.base.ops = NULL;
    complex_content_impl->complex_content.ops = NULL;
    
    complex_content_impl->complex_content.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_complex_content_ops_t));
    if(!complex_content_impl->complex_content.ops)
    {
        axis2_xml_schema_complex_content_free(&(complex_content_impl->complex_content), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    complex_content_impl->complex_content.ops->free = 
            axis2_xml_schema_complex_content_free;
    complex_content_impl->complex_content.ops->get_base_impl = 
            axis2_xml_schema_complex_content_get_base_impl;
    complex_content_impl->complex_content.ops->get_content_type = 
            axis2_xml_schema_complex_content_get_content_type;
    complex_content_impl->complex_content.ops->set_content = 
            axis2_xml_schema_complex_content_set_content;
    complex_content_impl->complex_content.ops->get_content = 
            axis2_xml_schema_complex_content_get_content;
    complex_content_impl->complex_content.ops->to_string = 
            axis2_xml_schema_complex_content_to_string;
   
    complex_content_impl->methods = axis2_hash_make(env);
   
    if(!complex_content_impl->methods)
    {
        axis2_xml_schema_complex_content_free(&(complex_content_impl->complex_content), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(complex_content_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_complex_content_free);
    axis2_hash_set(complex_content_impl->methods, "get_content", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_get_content);
    axis2_hash_set(complex_content_impl->methods, "set_content", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_set_content);
    axis2_hash_set(complex_content_impl->methods, "get_content_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_get_content_type);
    axis2_hash_set(complex_content_impl->methods, "to_string", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_to_string);
    
    complex_content_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!complex_content_impl->annotated)
    {
        axis2_xml_schema_complex_content_free(&(complex_content_impl->complex_content), env);
        return NULL;
    }
    status = axis2_xml_schema_annotated_resolve_methods(
            &(complex_content_impl->complex_content.base), env, complex_content_impl->annotated, 
            complex_content_impl->methods);
    
    return &(complex_content_impl->complex_content);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_free(void *complex_content,
                                    axis2_env_t **env)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);

    if(NULL != complex_content_impl->content)
    {
        /** TODO */
    }

    if(NULL != complex_content_impl->methods)
    {
        axis2_hash_free(complex_content_impl->methods, env);
        complex_content_impl->methods = NULL;
    }
    if(NULL != complex_content_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(complex_content_impl->annotated, env);
        complex_content_impl->annotated = NULL;
    }
    if(NULL != complex_content_impl->complex_content.base.ops)
    {
        AXIS2_FREE((*env)->allocator, complex_content_impl->complex_content.base.ops);
        complex_content_impl->complex_content.base.ops = NULL;  
    }
    if(NULL != complex_content_impl->complex_content.ops)
    {
        AXIS2_FREE((*env)->allocator, complex_content_impl->complex_content.ops);
        complex_content_impl->complex_content.ops = NULL;
    }
 
    AXIS2_FREE((*env)->allocator, complex_content_impl);
    complex_content_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_complex_content_get_base_impl(void *complex_content,
                                axis2_env_t **env)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);
    return complex_content_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_complex_content_resolve_methods(
                                axis2_xml_schema_complex_content_t *complex_content,
                                axis2_env_t **env,
                                axis2_xml_schema_complex_content_t *complex_content_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, complex_content_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, complex_content, AXIS2_FAILURE);
    
    complex_content_impl_l = (axis2_xml_schema_complex_content_impl_t *) complex_content_impl;
    
    complex_content->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_complex_content_ops_t));
    if(!complex_content->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    complex_content->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    complex_content->ops->get_base_impl = 
            complex_content_impl_l->complex_content.ops->get_base_impl;
    complex_content->ops->get_content_type = 
            complex_content_impl_l->complex_content.ops->get_content_type;
    complex_content->ops->get_content = 
            complex_content_impl_l->complex_content.ops->get_content;
    complex_content->ops->set_content = 
            complex_content_impl_l->complex_content.ops->set_content;
    complex_content->ops->to_string = 
            complex_content_impl_l->complex_content.ops->to_string;
    complex_content->ops->is_mixed = 
            complex_content_impl_l->complex_content.ops->is_mixed;
                        
    
    return axis2_xml_schema_annotated_resolve_methods(&(complex_content->base), 
            env, complex_content_impl_l->annotated, methods);
}

void* AXIS2_CALL
axis2_xml_schema_complex_content_get_content(void *complex_content,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(complex_content)->content;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_set_content(void *complex_content,
                                axis2_env_t **env,
                                void *content,
                                int content_type)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, content, AXIS2_FAILURE);
    
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);

    if(NULL != complex_content_impl->content)
    {
        /** TODO */
        
    }
    complex_content_impl->content = content;
    complex_content_impl->content_type = content_type;
    
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_xml_schema_complex_content_get_content_type(void *complex_content,
                                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, -1);
    return AXIS2_INTF_TO_IMPL(complex_content)->content_type;
}

axis2_char_t* AXIS2_CALL
axis2_xml_schema_complex_content_to_string(void *complex_content,
                                          axis2_env_t **env,
                                          axis2_char_t *prefix, 
                                          int tab)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    axis2_char_t *xml = NULL;
    axis2_char_t *temp_xml = NULL;
    axis2_char_t *temp_prefix = NULL;
    int i = 0;
    
    AXIS2_ENV_CHECK(env, NULL);
    for(i = 0; i < tab; i++)
    {
        if(xml == NULL)
        {
            xml = AXIS2_STRDUP("\t", env);
        }
        else
        {
            temp_xml = xml;
            xml = AXIS2_STRACAT(xml, "\t", env);
            if(NULL != temp_xml)
                AXIS2_FREE((*env)->allocator, temp_xml);
            temp_xml = NULL;
        }
    }
    
    temp_prefix = prefix;
    
    if(NULL != prefix && AXIS2_STRCMP(prefix, "") != 0 && strchr(prefix, ':') == NULL)
    {
        temp_prefix = AXIS2_STRACAT(prefix, ":", env);
    }
        
    temp_xml = AXIS2_STRACAT("<", prefix, env);
    xml = AXIS2_STRACAT(temp_xml, "complexContent>\n", env);
    if(NULL != temp_xml)
    {
        AXIS2_FREE((*env)->allocator, temp_xml);
        temp_xml = NULL;
    }
    
    /** TODO implement swith to get string of content */                            
    
    
    return xml;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_complex_content_is_mixed(void *complex_content,
                                          axis2_env_t **env)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    /** AXIS2_ENV_CHECK(env, AXIS2_FAILURE); */
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);
    return complex_content_impl->mixed;
}                                          
                                            
axis2_status_t AXIS2_CALL 
axis2_xml_scheam_complex_content_set_mixed(void *complex_content,
                                           axis2_env_t **env,
                                           axis2_bool_t mixed)
{
    axis2_xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);
    complex_content_impl->mixed = mixed;
    return AXIS2_SUCCESS;
}
                                           
                                            