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

#include <axis2_xml_schema_complex_content_restriction.h>

typedef struct axis2_xml_schema_complex_content_restriction_impl 
                    axis2_xml_schema_complex_content_restriction_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_complex_content_restriction_impl
{
    axis2_xml_schema_complex_content_restriction_t sim_content_res;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    axis2_xml_schema_obj_collection_t *attributes;
    
    axis2_xml_schema_obj_collection_t *facets;
    
    axis2_xml_schema_any_attribute_t *any_attribute;
    
    axis2_qname_t *base_type_name;
    
    axis2_xml_schema_simple_type_t *base_type;
};

#define AXIS2_INTF_TO_IMPL(sim_content_res) \
        ((axis2_xml_schema_complex_content_restriction_impl_t *) sim_content_res)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_complex_content_restriction_free(void *sim_content_res,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_base_impl(void *sim_content_res,
                                        axis2_env_t **env);

axis2_xml_schema_any_attribute_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_any_attribute(
                                            void *sim_content_res,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_restriction_set_any_attribute(void *sim_content_res,
                                            axis2_env_t **env,
                                            axis2_xml_schema_any_attribute_t *any_attr);

axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_attributes(void *sim_content_res,
                                                         axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_restriction_set_base_type_name(
                                          void *sim_content_res,
                                          axis2_env_t **env,
                                          axis2_qname_t *base_type_name);
                                          
axis2_qname_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_base_type_name(
                                          void *sim_content_res,
                                          axis2_env_t **env);
                                          
axis2_status_t AXIS2_CALL 
axis2_xml_schema_complex_content_restriction_set_base_type(void *sim_content_res,
                                              axis2_env_t **env,
                                              axis2_xml_schema_simple_type_t *base_type);
                                              
axis2_xml_schema_simple_type_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_base_type(void *sim_content_res,
                                              axis2_env_t **env);
                                                 
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_facets(void *sim_content_res,
                                                       axis2_env_t **env);                                          
                                          
                                          
/*****************************************************************************/                                          
AXIS2_DECLARE(axis2_xml_schema_complex_content_restriction_t *)
axis2_xml_schema_complex_content_restriction_create(axis2_env_t **env)
{
    axis2_xml_schema_complex_content_restriction_impl_t *sim_content_res_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    sim_content_res_impl = (axis2_xml_schema_complex_content_restriction_impl_t*)
        AXIS2_MALLOC((*env)->allocator, sizeof(axis2_xml_schema_complex_content_restriction_impl_t));
        
    if(!sim_content_res_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    sim_content_res_impl->annotated = NULL;
    sim_content_res_impl->methods = NULL;
    sim_content_res_impl->sim_content_res.base.ops = NULL;
    sim_content_res_impl->sim_content_res.ops = NULL;
    sim_content_res_impl->attributes = NULL;
    sim_content_res_impl->base_type_name = NULL;
    sim_content_res_impl->any_attribute = NULL;
    sim_content_res_impl->facets = NULL;
    sim_content_res_impl->base_type = NULL;
    
    sim_content_res_impl->sim_content_res.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_complex_content_restriction_ops_t));
    if(!sim_content_res_impl->sim_content_res.ops)
    {
        axis2_xml_schema_complex_content_restriction_free(
            &(sim_content_res_impl->sim_content_res), env);
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    sim_content_res_impl->sim_content_res.ops->free = 
            axis2_xml_schema_complex_content_restriction_free;
            
    sim_content_res_impl->sim_content_res.ops->get_base_impl = 
            axis2_xml_schema_complex_content_restriction_get_base_impl;
            
    sim_content_res_impl->sim_content_res.ops->get_any_attribute = 
            axis2_xml_schema_complex_content_restriction_get_any_attribute;
            
    sim_content_res_impl->sim_content_res.ops->set_any_attribute = 
            axis2_xml_schema_complex_content_restriction_set_any_attribute;
            
    sim_content_res_impl->sim_content_res.ops->get_attributes = 
            axis2_xml_schema_complex_content_restriction_get_attributes;
            
    sim_content_res_impl->sim_content_res.ops->get_base_type_name = 
            axis2_xml_schema_complex_content_restriction_get_base_type_name;
            
    sim_content_res_impl->sim_content_res.ops->set_base_type_name =
            axis2_xml_schema_complex_content_restriction_set_base_type_name;  
            
    sim_content_res_impl->sim_content_res.ops->set_base_type = 
            axis2_xml_schema_complex_content_restriction_set_base_type;
            
    sim_content_res_impl->sim_content_res.ops->get_base_type =
            axis2_xml_schema_complex_content_restriction_get_base_type;  
            
    sim_content_res_impl->sim_content_res.ops->get_facets =
            axis2_xml_schema_complex_content_restriction_get_facets;                      
                          
   
    sim_content_res_impl->methods = axis2_hash_make(env);
   
    if(!sim_content_res_impl->methods)
    {
        axis2_xml_schema_complex_content_restriction_free(
            &(sim_content_res_impl->sim_content_res), env);
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(sim_content_res_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_restriction_free);
    axis2_hash_set(sim_content_res_impl->methods, "get_any_attribute", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_restriction_get_any_attribute);
    axis2_hash_set(sim_content_res_impl->methods, "set_any_attribute", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_restriction_set_any_attribute);
    axis2_hash_set(sim_content_res_impl->methods, "get_attributes", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_restriction_get_attributes);
    axis2_hash_set(sim_content_res_impl->methods, "get_base_type_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_restriction_get_base_type_name);
    axis2_hash_set(sim_content_res_impl->methods, "set_base_type_name",
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_content_restriction_set_base_type_name);
    
    sim_content_res_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!sim_content_res_impl->annotated)
    {
        axis2_xml_schema_complex_content_restriction_free(
                &(sim_content_res_impl->sim_content_res), env);
        return NULL;
    }
    sim_content_res_impl->attributes = axis2_xml_schema_obj_collection_create(env);
    sim_content_res_impl->facets = axis2_xml_schema_obj_collection_create(env);
    if(!sim_content_res_impl->attributes || !sim_content_res_impl->facets)
    {
        axis2_xml_schema_complex_content_restriction_free(
            &(sim_content_res_impl->sim_content_res), env);
        return NULL;            
    }
    
    status = axis2_xml_schema_annotated_resolve_methods(
            &(sim_content_res_impl->sim_content_res.base), env, sim_content_res_impl->annotated, 
            sim_content_res_impl->methods);
    
    return &(sim_content_res_impl->sim_content_res);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_restriction_free(void *sim_content_res,
                                    axis2_env_t **env)
{
    axis2_xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_res_impl = AXIS2_INTF_TO_IMPL(sim_content_res);
    if(NULL != content_res_impl->any_attribute)
    {
        AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_res_impl->any_attribute, env);
        content_res_impl->any_attribute = NULL;
    }
    if(NULL != content_res_impl->attributes)
    {
        /** TODO free attributes */
    } 
    if(NULL != content_res_impl->facets)
    {
        /** TODO free facets */
    }
    if(NULL != content_res_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_res_impl->base_type_name, env);
        content_res_impl->base_type_name = NULL;
    }    
    if(NULL != content_res_impl->methods)
    {
        axis2_hash_free(content_res_impl->methods, env);
        content_res_impl->methods = NULL;
    }
    if(NULL != content_res_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(content_res_impl->annotated, env);
        content_res_impl->annotated = NULL;
    }
    if(NULL != content_res_impl->sim_content_res.base.ops)
    {
        AXIS2_FREE((*env)->allocator, content_res_impl->sim_content_res.base.ops);
        content_res_impl->sim_content_res.base.ops = NULL;  
    }
    if(NULL != content_res_impl->sim_content_res.ops)
    {
        AXIS2_FREE((*env)->allocator, content_res_impl->sim_content_res.ops);
        content_res_impl->sim_content_res.ops = NULL;
    }
 
    AXIS2_FREE((*env)->allocator, content_res_impl);
    content_res_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_base_impl(void *sim_content_res,
                                axis2_env_t **env)
{
    axis2_xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_res_impl = AXIS2_INTF_TO_IMPL(sim_content_res);
    return content_res_impl->annotated;
}



axis2_xml_schema_any_attribute_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_any_attribute(void *sim_content_res,
                                                            axis2_env_t **env)
{
    axis2_xml_schema_complex_content_restriction_impl_t *cnt_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_res_impl = AXIS2_INTF_TO_IMPL(sim_content_res);
    return cnt_res_impl->any_attribute;    
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_restriction_set_any_attribute(void *sim_content_res,
                                axis2_env_t **env,
                                axis2_xml_schema_any_attribute_t *any_attr)
{
    axis2_xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, any_attr, AXIS2_FAILURE);
    if(NULL != content_res_impl->any_attribute)
    {
        AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_res_impl->any_attribute, env);
        content_res_impl->any_attribute = NULL;
    }
        content_res_impl->any_attribute = any_attr;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_attributes(void *sim_content,
                                                        axis2_env_t **env)
{
    axis2_xml_schema_complex_content_restriction_impl_t *cnt_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_res_impl = AXIS2_INTF_TO_IMPL(sim_content);
    return cnt_res_impl->attributes;
}

axis2_qname_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_base_type_name(
                                          void *sim_content,
                                          axis2_env_t **env)
{
    axis2_xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_res_impl = AXIS2_INTF_TO_IMPL(sim_content);
    return content_res_impl->base_type_name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_restriction_set_base_type_name(
                                          void *sim_content,
                                          axis2_env_t **env,
                                          axis2_qname_t *base_type_name)
{
    axis2_xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_res_impl = AXIS2_INTF_TO_IMPL(sim_content);
    if(NULL != content_res_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_res_impl->base_type_name, env);
        content_res_impl->base_type_name = NULL;
    }
    content_res_impl->base_type_name = base_type_name;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_complex_content_restriction_set_base_type(void *sim_content_res,
                                              axis2_env_t **env,
                                              axis2_xml_schema_simple_type_t *base_type)
{
    axis2_xml_schema_complex_content_restriction_impl_t *sim_cnt_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, base_type, AXIS2_FAILURE);
    sim_cnt_res_impl = AXIS2_INTF_TO_IMPL(sim_content_res);
    if(NULL != sim_cnt_res_impl->base_type)
    {
        AXIS2_XML_SCHEMA_SIMPLE_TYPE_FREE(sim_cnt_res_impl->base_type, env);
        sim_cnt_res_impl->base_type = NULL;
    }
    sim_cnt_res_impl->base_type = base_type;
    return AXIS2_SUCCESS;
}                                              
                                              
                                              
axis2_xml_schema_simple_type_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_base_type(void *sim_content_res,
                                              axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(sim_content_res)->base_type;
}                                              

                                                 
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_content_restriction_get_facets(void *sim_content_res,
                                                       axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(sim_content_res)->facets;
}                                                       