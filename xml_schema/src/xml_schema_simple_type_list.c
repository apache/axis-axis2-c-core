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

#include <xml_schema/axis2_xml_schema_simple_type_list.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_simple_type_list_impl 
                    axis2_xml_schema_simple_type_list_impl_t;

/** 
 * @brief axis2_xml_schema_simple_type_list_impl
 *   
 */ 
struct axis2_xml_schema_simple_type_list_impl
{
    axis2_xml_schema_simple_type_list_t simple_type_list;
    
    axis2_xml_schema_simple_type_content_t *sim_type_content;
    
    axis2_hash_t *methods;
    
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_xml_schema_simple_type_t *item_type;
    
    axis2_qname_t *item_type_name;
    
};

#define AXIS2_INTF_TO_IMPL(simple_type_list) \
        ((axis2_xml_schema_simple_type_list_impl_t *) simple_type_list)
        
/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_simple_type_list_free(void *simple_type_list,
                        const axis2_env_t *env);

axis2_xml_schema_simple_type_content_t *AXIS2_CALL
axis2_xml_schema_simple_type_list_get_base_impl(void *simple_type_list,
                                        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_simple_type_list_super_objs(void *simple_type_list,
                                            const axis2_env_t *env);
                                            
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_simple_type_list_type(void *simple_type_list,
                                       const axis2_env_t *env);                                            

axis2_xml_schema_simple_type_t* AXIS2_CALL
axis2_xml_schema_simple_type_list_get_item_type(void *simple_type_list,
                                            const axis2_env_t *env);
                                            
axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_list_set_item_type(void *simple_type_list,
                                            const axis2_env_t *env,
                                            axis2_xml_schema_simple_type_t *base_type);

axis2_qname_t* AXIS2_CALL
axis2_xml_schema_simple_type_list_get_item_type_name(void *simple_type_list,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_list_set_item_type_name(void *simple_type_list,
                                            const axis2_env_t *env,
                                            axis2_qname_t *base_type_name);

/*************** function prototypes *****************************************/

AXIS2_EXTERN axis2_xml_schema_simple_type_list_t * AXIS2_CALL
axis2_xml_schema_simple_type_list_create(const axis2_env_t *env)
{
    axis2_xml_schema_simple_type_list_impl_t *simple_type_list_impl = NULL;
    axis2_xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    simple_type_list_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_simple_type_list_impl_t));
    
    if(!simple_type_list_impl)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    simple_type_list_impl->sim_type_content = NULL;
    simple_type_list_impl->simple_type_list.ops = NULL;
    simple_type_list_impl->simple_type_list.base.ops = NULL;
    simple_type_list_impl->methods = NULL;
    simple_type_list_impl->item_type = NULL;
    simple_type_list_impl->item_type_name = NULL;
    simple_type_list_impl->ht_super = NULL;
    simple_type_list_impl->obj_type = AXIS2_XML_SCHEMA_SIMPLE_TYPE_LIST;
    
    simple_type_list_impl->simple_type_list.ops = 
        AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_simple_type_list_ops_t));

    if(!simple_type_list_impl->simple_type_list.ops)
    {
        axis2_xml_schema_simple_type_list_free(
            &(simple_type_list_impl->simple_type_list), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    simple_type_list_impl->simple_type_list.ops->free = 
            axis2_xml_schema_simple_type_list_free;
    simple_type_list_impl->simple_type_list.ops->type =
            axis2_xml_schema_simple_type_list_type;
    simple_type_list_impl->simple_type_list.ops->super_objs =
            axis2_xml_schema_simple_type_list_super_objs;            
    simple_type_list_impl->simple_type_list.ops->get_base_impl = 
            axis2_xml_schema_simple_type_list_get_base_impl;
    simple_type_list_impl->simple_type_list.ops->get_item_type = 
            axis2_xml_schema_simple_type_list_get_item_type;
    simple_type_list_impl->simple_type_list.ops->set_item_type = 
            axis2_xml_schema_simple_type_list_set_item_type;
    simple_type_list_impl->simple_type_list.ops->get_item_type_name = 
            axis2_xml_schema_simple_type_list_get_item_type_name;
    simple_type_list_impl->simple_type_list.ops->set_item_type_name = 
            axis2_xml_schema_simple_type_list_set_item_type_name;
   
    simple_type_list_impl->methods = axis2_hash_make(env);
    simple_type_list_impl->ht_super = axis2_hash_make(env);
    if(!simple_type_list_impl->methods || !simple_type_list_impl->ht_super)
    {
        axis2_xml_schema_simple_type_list_free(
            &(simple_type_list_impl->simple_type_list), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(simple_type_list_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_simple_type_list_free);
    axis2_hash_set(simple_type_list_impl->methods, "get_base_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_list_get_item_type);
    axis2_hash_set(simple_type_list_impl->methods, "set_base_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_list_set_item_type);
    axis2_hash_set(simple_type_list_impl->methods, "get_base_type_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_list_get_item_type_name);
    axis2_hash_set(simple_type_list_impl->methods, "set_base_type_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_list_set_item_type_name);
    axis2_hash_set(simple_type_list_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_list_type);
    axis2_hash_set(simple_type_list_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_list_super_objs);                        
    
    simple_type_list_impl->sim_type_content = 
        axis2_xml_schema_simple_type_content_create(env);
        
    if(!simple_type_list_impl->sim_type_content)
    {
        axis2_xml_schema_simple_type_list_free(
            &(simple_type_list_impl->simple_type_list), env);
        return NULL;        
    }
    
    axis2_hash_set(simple_type_list_impl->ht_super, "AXIS2_XML_SCHEMA_SIMPLE_TYPE_LIST",
            AXIS2_HASH_KEY_STRING, &(simple_type_list_impl->simple_type_list));              
              
    axis2_hash_set(simple_type_list_impl->ht_super, "AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT",
            AXIS2_HASH_KEY_STRING, &(simple_type_list_impl->sim_type_content));    
            
    annotated = AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_BASE_IMPL(
        simple_type_list_impl->sim_type_content, env);            
    if(NULL != annotated)
    {            
        axis2_hash_set(simple_type_list_impl->ht_super, 
            "AXIS2_XML_SCHEMA_ANNOTATED",
            AXIS2_HASH_KEY_STRING, annotated);    
     
        axis2_hash_set(simple_type_list_impl->ht_super, 
            "AXIS2_XML_SCHEMA_OBJ",
            AXIS2_HASH_KEY_STRING,
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));    
                      
    }
    
    status = axis2_xml_schema_simple_type_content_resolve_methods(
            &(simple_type_list_impl->simple_type_list.base), env,                                  simple_type_list_impl->sim_type_content, 
            simple_type_list_impl->methods);
    
    return &(simple_type_list_impl->simple_type_list);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_list_free(void *simple_type_list,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_simple_type_list_impl_t *simple_type_list_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    simple_type_list_impl = AXIS2_INTF_TO_IMPL(simple_type_list);

    if(NULL != simple_type_list_impl->methods)
    {
        axis2_hash_free(simple_type_list_impl->methods, env);
        simple_type_list_impl->methods = NULL;
    }
    if(NULL != simple_type_list_impl->ht_super)
    {
        axis2_hash_free(simple_type_list_impl->ht_super, env);
        simple_type_list_impl->ht_super = NULL;
    }
    if(NULL != simple_type_list_impl->sim_type_content)
    {
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(
            simple_type_list_impl->sim_type_content , env);
            simple_type_list_impl->sim_type_content = NULL;
    }
    
    if(NULL != simple_type_list_impl->simple_type_list.ops)
    {
        AXIS2_FREE(env->allocator, 
            simple_type_list_impl->simple_type_list.ops);
        simple_type_list_impl->simple_type_list.ops = NULL;
    }
    if(NULL != simple_type_list_impl->simple_type_list.base.ops)
    {
        AXIS2_FREE(env->allocator, 
            simple_type_list_impl->simple_type_list.base.ops);
        simple_type_list_impl->simple_type_list.base.ops = NULL;
    }
    
    if(NULL != simple_type_list_impl)
    {
        AXIS2_FREE(env->allocator, simple_type_list_impl);
        simple_type_list_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_simple_type_content_t *AXIS2_CALL
axis2_xml_schema_simple_type_list_get_base_impl(void *simple_type_list,
                                const axis2_env_t *env)
{
    axis2_xml_schema_simple_type_list_impl_t *simple_type_list_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    simple_type_list_impl = AXIS2_INTF_TO_IMPL(simple_type_list);

    return simple_type_list_impl->sim_type_content;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_list_resolve_methods(
                    axis2_xml_schema_simple_type_list_t *simple_type_list,
                    const axis2_env_t *env,
                    axis2_xml_schema_simple_type_list_t *simple_type_list_impl,
                    axis2_hash_t *methods)
{
    axis2_xml_schema_simple_type_list_impl_t *simple_type_list_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_type_list_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    simple_type_list_impl_l = 
        (axis2_xml_schema_simple_type_list_impl_t *) simple_type_list_impl;
    
    simple_type_list->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_xml_schema_simple_type_list_ops_t));
    if(NULL != simple_type_list->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
    
    simple_type_list->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    simple_type_list->ops->get_base_impl = 
            simple_type_list_impl_l->simple_type_list.ops->get_base_impl;
            
    simple_type_list->ops->get_item_type = 
            simple_type_list_impl_l->simple_type_list.ops->get_item_type;
    
    simple_type_list->ops->set_item_type = 
            simple_type_list_impl_l->simple_type_list.ops->set_item_type;
    
    simple_type_list->ops->get_item_type_name = 
            simple_type_list_impl_l->simple_type_list.ops->get_item_type_name;
    simple_type_list->ops->set_item_type_name = 
            simple_type_list_impl_l->simple_type_list.ops->set_item_type_name;
    
    return axis2_xml_schema_simple_type_content_resolve_methods(
                &(simple_type_list->base), env, 
                simple_type_list_impl_l->sim_type_content, methods);
}
*/
axis2_xml_schema_simple_type_t* AXIS2_CALL
axis2_xml_schema_simple_type_list_get_item_type(void *simple_type_list,
                                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return 
    AXIS2_INTF_TO_IMPL(simple_type_list)->item_type;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_list_set_item_type(void *simple_type_list,
                                            const axis2_env_t *env,
                                            axis2_xml_schema_simple_type_t* item_type)
{
    axis2_xml_schema_simple_type_list_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sim_type_res_impl = AXIS2_INTF_TO_IMPL(simple_type_list);
    if(NULL != sim_type_res_impl->item_type_name)
    {
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(sim_type_res_impl->item_type, env);
        sim_type_res_impl->item_type_name = NULL;
    }
    sim_type_res_impl->item_type = item_type;
    return AXIS2_SUCCESS;
}

axis2_qname_t* AXIS2_CALL
axis2_xml_schema_simple_type_list_get_item_type_name(void *simple_type_list,
                                                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_type_list)->item_type_name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_list_set_item_type_name(void *simple_type_list,
                                                            const axis2_env_t *env,
                                                            axis2_qname_t *item_type_name)
{
    axis2_xml_schema_simple_type_list_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, item_type_name, AXIS2_FAILURE);
    
    sim_type_res_impl = AXIS2_INTF_TO_IMPL(simple_type_list);
    if(NULL != sim_type_res_impl->item_type_name)
    {
        AXIS2_QNAME_FREE(sim_type_res_impl->item_type_name, env);
        sim_type_res_impl->item_type_name = NULL;
    }  
    sim_type_res_impl->item_type_name = item_type_name;
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_simple_type_list_super_objs(void *simple_type_list,
                                            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type_list)->ht_super;
}                                            
                                            
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_simple_type_list_type(void *simple_type_list,
                                       const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type_list)->obj_type;
}

