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

#include <xml_schema_simple_content.h>
#include <axis2_utils.h>

typedef struct xml_schema_simple_content_impl 
                    xml_schema_simple_content_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct xml_schema_simple_content_impl
{
    xml_schema_simple_content_t sim_content;
    
    xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    void *content;
    
    xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
};

#define AXIS2_INTF_TO_IMPL(sim_content) \
        ((xml_schema_simple_content_impl_t *) sim_content)

axis2_status_t AXIS2_CALL 
xml_schema_simple_content_free(void *sim_content,
                        const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_simple_content_get_base_impl(void *sim_content,
                                        const axis2_env_t *env);

void* AXIS2_CALL
xml_schema_simple_content_get_content(void *sim_content,
                                            const axis2_env_t *env);
                                            
xml_schema_types_t AXIS2_CALL
xml_schema_simple_content_get_type(void *sim_content,
                                            const axis2_env_t *env);
                                            
axis2_hash_t* AXIS2_CALL
xml_schema_simple_content_super_objs(void *sim_content,
                                            const axis2_env_t *env);                                                                                        

axis2_status_t AXIS2_CALL
xml_schema_simple_content_set_content(void *sim_content,
                                            const axis2_env_t *env,
                                            void* content);

axis2_char_t* AXIS2_CALL
xml_schema_simple_content_to_string(void *sim_content,
                                          const axis2_env_t *env,
                                          axis2_char_t *prefix, 
                                          int tab);
                                          

AXIS2_EXTERN xml_schema_simple_content_t * AXIS2_CALL
xml_schema_simple_content_create(const axis2_env_t *env)
{
    xml_schema_simple_content_impl_t *sim_cnt_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    sim_cnt_impl = (xml_schema_simple_content_impl_t*)AXIS2_MALLOC(env->allocator, 
        sizeof(xml_schema_simple_content_impl_t));
    if(!sim_cnt_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    sim_cnt_impl->annotated = NULL;
    sim_cnt_impl->methods = NULL;
    sim_cnt_impl->content = NULL;
    sim_cnt_impl->sim_content.base.ops = NULL;
    sim_cnt_impl->sim_content.ops = NULL;
    sim_cnt_impl->ht_super = NULL;
    sim_cnt_impl->obj_type = XML_SCHEMA_SIMPLE_CONTENT;
    
    sim_cnt_impl->sim_content.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_simple_content_ops_t));
    if(!sim_cnt_impl->sim_content.ops)
    {
        xml_schema_simple_content_free(&(sim_cnt_impl->sim_content), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    sim_cnt_impl->sim_content.ops->free = 
            xml_schema_simple_content_free;
    sim_cnt_impl->sim_content.ops->get_type = 
            xml_schema_simple_content_get_type;
    sim_cnt_impl->sim_content.ops->super_objs = 
            xml_schema_simple_content_super_objs;
    sim_cnt_impl->sim_content.ops->get_base_impl = 
            xml_schema_simple_content_get_base_impl;
    sim_cnt_impl->sim_content.ops->set_content = 
            xml_schema_simple_content_set_content;
    sim_cnt_impl->sim_content.ops->get_content = 
            xml_schema_simple_content_get_content;
    sim_cnt_impl->sim_content.ops->to_string = 
            xml_schema_simple_content_to_string;
   
    sim_cnt_impl->methods = axis2_hash_make(env);
   
    if(!sim_cnt_impl->methods)
    {
        xml_schema_simple_content_free(&(sim_cnt_impl->sim_content), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(sim_cnt_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            xml_schema_simple_content_free);
    axis2_hash_set(sim_cnt_impl->methods, "get_type", AXIS2_HASH_KEY_STRING, 
            xml_schema_simple_content_get_type);            
    axis2_hash_set(sim_cnt_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            xml_schema_simple_content_super_objs);            
    axis2_hash_set(sim_cnt_impl->methods, "get_content", 
            AXIS2_HASH_KEY_STRING, xml_schema_simple_content_get_content);
    axis2_hash_set(sim_cnt_impl->methods, "set_content", 
            AXIS2_HASH_KEY_STRING, xml_schema_simple_content_set_content);
    axis2_hash_set(sim_cnt_impl->methods, "to_string", 
            AXIS2_HASH_KEY_STRING, xml_schema_simple_content_to_string);
    
    sim_cnt_impl->ht_super = axis2_hash_make(env);
    if(!sim_cnt_impl->ht_super)
    {
        xml_schema_simple_content_free(&(sim_cnt_impl->sim_content), env);
        return NULL;
    }
    sim_cnt_impl->annotated = xml_schema_annotated_create(env);
    if(!sim_cnt_impl->annotated)
    {
        xml_schema_simple_content_free(&(sim_cnt_impl->sim_content), env);
        return NULL;
    }
    axis2_hash_set(sim_cnt_impl->ht_super, "XML_SCHEMA_SIMPLE_CONTENT",
        AXIS2_HASH_KEY_STRING, &(sim_cnt_impl->sim_content));
        
    axis2_hash_set(sim_cnt_impl->ht_super, "XML_SCHEMA_ANNOTATED",
        AXIS2_HASH_KEY_STRING, sim_cnt_impl->annotated);
    
    axis2_hash_set(sim_cnt_impl->ht_super, "XML_SCHEMA_OBJ",
        AXIS2_HASH_KEY_STRING, 
        XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(sim_cnt_impl->annotated, env));
    
    status = xml_schema_annotated_resolve_methods(
            &(sim_cnt_impl->sim_content.base), env, sim_cnt_impl->annotated, 
            sim_cnt_impl->methods);
    
    return &(sim_cnt_impl->sim_content);
}

axis2_status_t AXIS2_CALL
xml_schema_simple_content_free(void *sim_content,
                                    const axis2_env_t *env)
{
    xml_schema_simple_content_impl_t *sim_cnt_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sim_cnt_impl = AXIS2_INTF_TO_IMPL(sim_content);

    if(NULL != sim_cnt_impl->content)
    {
        /** TODO */
    }

    if(NULL != sim_cnt_impl->methods)
    {
        axis2_hash_free(sim_cnt_impl->methods, env);
        sim_cnt_impl->methods = NULL;
    }
    if(NULL != sim_cnt_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(sim_cnt_impl->annotated, env);
        sim_cnt_impl->annotated = NULL;
    }
    if(NULL != sim_cnt_impl->ht_super)
    {
        axis2_hash_free(sim_cnt_impl->ht_super, env);
        sim_cnt_impl->ht_super = NULL;
    }
    if(NULL != sim_cnt_impl->sim_content.base.ops)
    {
        AXIS2_FREE(env->allocator, sim_cnt_impl->sim_content.base.ops);
        sim_cnt_impl->sim_content.base.ops = NULL;  
    }
    if(NULL != sim_cnt_impl->sim_content.ops)
    {
        AXIS2_FREE(env->allocator, sim_cnt_impl->sim_content.ops);
        sim_cnt_impl->sim_content.ops = NULL;
    }
 
    AXIS2_FREE(env->allocator, sim_cnt_impl);
    sim_cnt_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_simple_content_get_base_impl(void *sim_content,
                                const axis2_env_t *env)
{
    xml_schema_simple_content_impl_t *sim_cnt_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    sim_cnt_impl = AXIS2_INTF_TO_IMPL(sim_content);
    return sim_cnt_impl->annotated;
}

void* AXIS2_CALL
xml_schema_simple_content_get_content(void *sim_content,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(sim_content)->content;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_content_set_content(void *sim_content,
                                const axis2_env_t *env,
                                void *content)
{
    xml_schema_simple_content_impl_t *sim_cnt_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content, AXIS2_FAILURE);
    
    sim_cnt_impl = AXIS2_INTF_TO_IMPL(sim_content);

    if(NULL != sim_cnt_impl->content)
    {
        /** TODO */
        
    }
    sim_cnt_impl->content = content;
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
xml_schema_simple_content_to_string(void *sim_content,
                                          const axis2_env_t *env,
                                          axis2_char_t *prefix, 
                                          int tab)
{
    xml_schema_simple_content_impl_t *sim_cnt_impl = NULL;
    axis2_char_t *xml = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    sim_cnt_impl = AXIS2_INTF_TO_IMPL(sim_content);
    if(NULL != prefix && AXIS2_STRCMP(prefix, "") != 0 && strchr(prefix, ':') == NULL)
    {
        xml = AXIS2_STRACAT(prefix, ":", env);
    }
    return xml;
}

xml_schema_types_t AXIS2_CALL
xml_schema_simple_content_get_type(void *sim_content,
                                            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(sim_content)->obj_type;
}                                            
                                            
axis2_hash_t* AXIS2_CALL
xml_schema_simple_content_super_objs(void *sim_content,
                                            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(sim_content)->ht_super;
}                                            

