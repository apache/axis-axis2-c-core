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

#include <xml_schema/axis2_xml_schema_simple_content.h>

typedef struct axis2_xml_schema_simple_content_impl 
                    axis2_xml_schema_simple_content_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_simple_content_impl
{
    axis2_xml_schema_simple_content_t sim_content;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    void *content;
    
    int content_type;
};

#define AXIS2_INTF_TO_IMPL(sim_content) \
        ((axis2_xml_schema_simple_content_impl_t *) sim_content)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_simple_content_free(void *sim_content,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_simple_content_get_base_impl(void *sim_content,
                                        axis2_env_t **env);

void* AXIS2_CALL
axis2_xml_schema_simple_content_get_content(void *sim_content,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_content_set_content(void *sim_content,
                                            axis2_env_t **env,
                                            void* content,
                                            int content_type);

int AXIS2_CALL
axis2_xml_schema_simple_content_get_content_type(void *sim_content,
                                                 axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_xml_schema_simple_content_to_string(void *sim_content,
                                          axis2_env_t **env,
                                          axis2_char_t *prefix, 
                                          int tab);
                                          

AXIS2_DECLARE(axis2_xml_schema_simple_content_t *)
axis2_xml_schema_simple_content_create(axis2_env_t **env)
{
    axis2_xml_schema_simple_content_impl_t *group_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    group_impl = (axis2_xml_schema_simple_content_impl_t*)AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_xml_schema_simple_content_impl_t));
    if(!group_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    group_impl->annotated = NULL;
    group_impl->methods = NULL;
    group_impl->content = NULL;
    group_impl->content_type = -1;
    group_impl->sim_content.base.ops = NULL;
    group_impl->sim_content.ops = NULL;
    
    group_impl->sim_content.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_simple_content_ops_t));
    if(!group_impl->sim_content.ops)
    {
        axis2_xml_schema_simple_content_free(&(group_impl->sim_content), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    group_impl->sim_content.ops->free = 
            axis2_xml_schema_simple_content_free;
    group_impl->sim_content.ops->get_base_impl = 
            axis2_xml_schema_simple_content_get_base_impl;
    group_impl->sim_content.ops->get_content_type = 
            axis2_xml_schema_simple_content_get_content_type;
    group_impl->sim_content.ops->set_content = 
            axis2_xml_schema_simple_content_set_content;
    group_impl->sim_content.ops->get_content = 
            axis2_xml_schema_simple_content_get_content;
    group_impl->sim_content.ops->to_string = 
            axis2_xml_schema_simple_content_to_string;
   
    group_impl->methods = axis2_hash_make(env);
   
    if(!group_impl->methods)
    {
        axis2_xml_schema_simple_content_free(&(group_impl->sim_content), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_simple_content_free);
    axis2_hash_set(group_impl->methods, "get_content", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_content_get_content);
    axis2_hash_set(group_impl->methods, "set_content", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_content_set_content);
    axis2_hash_set(group_impl->methods, "get_content_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_content_get_content_type);
    axis2_hash_set(group_impl->methods, "to_string", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_content_to_string);
    
    group_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!group_impl->annotated)
    {
        axis2_xml_schema_simple_content_free(&(group_impl->sim_content), env);
        return NULL;
    }
    status = axis2_xml_schema_annotated_resolve_methods(
            &(group_impl->sim_content.base), env, group_impl->annotated, 
            group_impl->methods);
    
    return &(group_impl->sim_content);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_content_free(void *sim_content,
                                    axis2_env_t **env)
{
    axis2_xml_schema_simple_content_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(sim_content);

    if(NULL != group_impl->content)
    {
        /** TODO */
    }

    if(NULL != group_impl->methods)
    {
        axis2_hash_free(group_impl->methods, env);
        group_impl->methods = NULL;
    }
    if(NULL != group_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(group_impl->annotated, env);
        group_impl->annotated = NULL;
    }
    if(NULL != group_impl->sim_content.base.ops)
    {
        AXIS2_FREE((*env)->allocator, group_impl->sim_content.base.ops);
        group_impl->sim_content.base.ops = NULL;  
    }
    if(NULL != group_impl->sim_content.ops)
    {
        AXIS2_FREE((*env)->allocator, group_impl->sim_content.ops);
        group_impl->sim_content.ops = NULL;
    }
 
    AXIS2_FREE((*env)->allocator, group_impl);
    group_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_simple_content_get_base_impl(void *sim_content,
                                axis2_env_t **env)
{
    axis2_xml_schema_simple_content_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_impl = AXIS2_INTF_TO_IMPL(sim_content);
    return group_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_simple_content_resolve_methods(
                                axis2_xml_schema_simple_content_t *sim_content,
                                axis2_env_t **env,
                                axis2_xml_schema_simple_content_t *group_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_simple_content_impl_t *group_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, group_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, sim_content, AXIS2_FAILURE);
    
    group_impl_l = (axis2_xml_schema_simple_content_impl_t *) group_impl;
    
    sim_content->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_simple_content_ops_t));
    if(!sim_content->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    sim_content->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    sim_content->ops->get_base_impl = 
            group_impl_l->sim_content.ops->get_base_impl;
    sim_content->ops->get_content_type = 
            group_impl_l->sim_content.ops->get_content_type;
    sim_content->ops->get_content = 
            group_impl_l->sim_content.ops->get_content;
    sim_content->ops->set_content = 
            group_impl_l->sim_content.ops->set_content;
    sim_content->ops->to_string = 
            group_impl_l->sim_content.ops->to_string;
    
    return axis2_xml_schema_annotated_resolve_methods(&(sim_content->base), 
            env, group_impl_l->annotated, methods);
}

void* AXIS2_CALL
axis2_xml_schema_simple_content_get_content(void *sim_content,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(sim_content)->content;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_content_set_content(void *sim_content,
                                axis2_env_t **env,
                                void *content,
                                int content_type)
{
    axis2_xml_schema_simple_content_impl_t *group_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, content, AXIS2_FAILURE);
    
    group_impl = AXIS2_INTF_TO_IMPL(sim_content);

    if(NULL != group_impl->content)
    {
        /** TODO */
        
    }
    group_impl->content = content;
    group_impl->content_type = content_type;
    
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_xml_schema_simple_content_get_content_type(void *sim_content,
                                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, -1);
    return AXIS2_INTF_TO_IMPL(sim_content)->content_type;
}

axis2_char_t* AXIS2_CALL
axis2_xml_schema_simple_content_to_string(void *sim_content,
                                          axis2_env_t **env,
                                          axis2_char_t *prefix, 
                                          int tab)
{
    axis2_xml_schema_simple_content_impl_t *group_impl = NULL;
    axis2_char_t *xml = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    group_impl = AXIS2_INTF_TO_IMPL(sim_content);
    if(NULL != prefix && AXIS2_STRCMP(prefix, "") != 0 && strchr(prefix, ':') == NULL)
    {
        xml = AXIS2_STRACAT(prefix, ":", env);
    }
    return xml;
}

