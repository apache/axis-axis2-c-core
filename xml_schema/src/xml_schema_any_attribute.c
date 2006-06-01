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

#include <xml_schema/axis2_xml_schema_any_attribute.h>
#include <axis2_utils.h>


typedef struct axis2_xml_schema_any_attribute_impl 
                axis2_xml_schema_any_attribute_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_any_attribute_impl
{
    axis2_xml_schema_any_attribute_t any_attr;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    axis2_xml_schema_content_processing_t *process_content;
    /**
     * Namespaces containing the elements that can be used.
     */
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t* ht_super;
         
    axis2_char_t *ns;
};

#define AXIS2_INTF_TO_IMPL(any_attr) \
        ((axis2_xml_schema_any_attribute_impl_t *) any_attr)

/******************* function prototypes **********************************/
axis2_status_t AXIS2_CALL 
axis2_xml_schema_any_attribute_free(void *any_attr,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_any_attribute_get_base_impl(void *any_attr,
                                    axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_any_attribute_type(void *any_attr,
                                    axis2_env_t **env);

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_any_attribute_super_objs(void *any_attr,
                                        axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_any_attribute_get_namespace(void *any_attr,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_set_namespace(void *any_attr,
                                            axis2_env_t **env,
                                            axis2_char_t *ns);

axis2_xml_schema_content_processing_t *AXIS2_CALL
axis2_xml_schema_any_attribute_get_process_content(void *any_attr,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_set_process_content(void *any_attr,
                        axis2_env_t **env,
                        axis2_xml_schema_content_processing_t *process_content);
                                                
 /************************ end function prototypes ******************************/
 
AXIS2_EXTERN axis2_xml_schema_any_attribute_t * AXIS2_CALL
axis2_xml_schema_any_attribute_create(axis2_env_t **env)
{
    axis2_xml_schema_any_attribute_impl_t *any_attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    any_attr_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_any_attribute_impl_t));
    if(!any_attr_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    any_attr_impl->any_attr.base.ops = NULL;
    any_attr_impl->any_attr.ops = NULL;
    any_attr_impl->ns = NULL;
    any_attr_impl->annotated = NULL;
    any_attr_impl->process_content = NULL;
    any_attr_impl->methods = NULL;
    any_attr_impl->ht_super = NULL;
    any_attr_impl->obj_type = AXIS2_XML_SCHEMA_ANY_ATTRIBUTE;
    any_attr_impl->any_attr.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_any_attribute_ops_t));

    if(!any_attr_impl->any_attr.ops)
    {
        axis2_xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        AXIS2_ERROR_SET((*env)->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    any_attr_impl->process_content = axis2_xml_schema_content_processing_create(env, 
            "none");
    if(!any_attr_impl->process_content)
    {
        axis2_xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        return NULL;
    }            
    any_attr_impl->any_attr.ops->free = 
        axis2_xml_schema_any_attribute_free;
        
    any_attr_impl->any_attr.ops->type =
        axis2_xml_schema_any_attribute_type;
    
    any_attr_impl->any_attr.ops->super_objs =
        axis2_xml_schema_any_attribute_super_objs;                
    
    any_attr_impl->any_attr.ops->get_base_impl = 
        axis2_xml_schema_any_attribute_get_base_impl;
    
    any_attr_impl->any_attr.ops->get_namespace = 
        axis2_xml_schema_any_attribute_get_namespace;
    
    any_attr_impl->any_attr.ops->set_namespace = 
        axis2_xml_schema_any_attribute_set_namespace;
    
    any_attr_impl->any_attr.ops->get_process_content = 
        axis2_xml_schema_any_attribute_get_process_content;
    
    any_attr_impl->any_attr.ops->set_process_content = 
        axis2_xml_schema_any_attribute_set_process_content;

    any_attr_impl->methods = axis2_hash_make(env);
    if(!any_attr_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(any_attr_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_any_attribute_free);
    axis2_hash_set(any_attr_impl->methods, "type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_any_attribute_type);
    axis2_hash_set(any_attr_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_any_attribute_super_objs);            
    axis2_hash_set(any_attr_impl->methods, "get_namespace", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_any_attribute_get_namespace);
    axis2_hash_set(any_attr_impl->methods, "set_namespace", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_any_attribute_set_namespace);
    axis2_hash_set(any_attr_impl->methods, "get_process_content", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_any_attribute_get_process_content);
    axis2_hash_set(any_attr_impl->methods, "set_process_content", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_any_attribute_set_process_content);
    
    any_attr_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!any_attr_impl->methods)
    {
        axis2_xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    any_attr_impl->ht_super = axis2_hash_make(env);
    if(!any_attr_impl->methods)
    {
        axis2_xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(any_attr_impl->ht_super, "AXIS2_XML_SCHEMA_ANY_ATTRIBUTE", 
            AXIS2_HASH_KEY_STRING, &(any_attr_impl->any_attr));
            
    axis2_hash_set(any_attr_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, any_attr_impl->annotated);

    axis2_hash_set(any_attr_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(any_attr_impl->annotated, env));
                            
    status = axis2_xml_schema_annotated_resolve_methods(
            &(any_attr_impl->any_attr.base), env, any_attr_impl->annotated, 
            any_attr_impl->methods); 
    return &(any_attr_impl->any_attr);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_free(void *any_attr,
                            axis2_env_t **env)
{
    axis2_xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);

    if(NULL != any_attr_impl->ns)
    {
        AXIS2_FREE((*env)->allocator, any_attr_impl->ns);
        any_attr_impl->ns = NULL;
    }
    if(NULL != any_attr_impl->ht_super)
    {
        axis2_hash_free(any_attr_impl->ht_super, env);
        any_attr_impl->ht_super = NULL;    
    }
    if(NULL != any_attr_impl->process_content)
    {
        AXIS2_XML_SCHEMA_CONTENT_PROCESSING_FREE(any_attr_impl->process_content, env);
        any_attr_impl->process_content = NULL;
    }
 
    if(NULL != any_attr_impl->methods)
    {
        axis2_hash_free(any_attr_impl->methods, env);
        any_attr_impl->methods = NULL;
    }
   
    if(NULL != any_attr_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(any_attr_impl->annotated, env);
        any_attr_impl->annotated = NULL;
    }
    
    if(NULL != any_attr_impl->any_attr.ops)
    {
        AXIS2_FREE((*env)->allocator, any_attr_impl->any_attr.ops);
        any_attr_impl->any_attr.ops = NULL;
    }
    if(NULL != any_attr_impl->any_attr.base.ops)
    {
        AXIS2_FREE((*env)->allocator, any_attr_impl->any_attr.base.ops);
        any_attr_impl->any_attr.base.ops = NULL;    
    }

    AXIS2_FREE((*env)->allocator, any_attr_impl);
    any_attr_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_any_attribute_get_base_impl(void *any_attr,
                                axis2_env_t **env)
{
    axis2_xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);
    return any_attr_impl->annotated;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_resolve_methods(
                                axis2_xml_schema_any_attribute_t *any_attr,
                                axis2_env_t **env,
                                axis2_xml_schema_any_attribute_t *any_attr_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_any_attribute_impl_t *any_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, any_attr_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    any_impl_l = (axis2_xml_schema_any_attribute_impl_t *) any_attr_impl;
    
    any_attr->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_any_attribute_ops_t));
    if(NULL != any_attr->ops)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
            
    any_attr->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    any_attr->ops->get_base_impl = 
            any_impl_l->any_attr.ops->get_base_impl;
    any_attr->ops->get_namespace = 
            any_impl_l->any_attr.ops->get_namespace;
    any_attr->ops->set_namespace = 
            any_impl_l->any_attr.ops->set_namespace;
    any_attr->ops->get_process_content = 
            any_impl_l->any_attr.ops->get_process_content;
    any_attr->ops->set_process_content = 
            any_impl_l->any_attr.ops->set_process_content;
    
    return axis2_xml_schema_annotated_resolve_methods(&(any_attr->base), 
            env, any_impl_l->annotated, methods);
}
*/
axis2_char_t *AXIS2_CALL
axis2_xml_schema_any_attribute_get_namespace(void *any_attr,
                                    axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->ns;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_set_namespace(void *any_attr,
                                            axis2_env_t **env,
                                            axis2_char_t *namespc)
{
    axis2_xml_schema_any_attribute_impl_t *any_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);
    
    if(!any_attr_impl->ns)
    {
        AXIS2_FREE((*env)->allocator, any_attr_impl->ns);
        any_attr_impl->ns = NULL;
    }
    any_attr_impl->ns = AXIS2_STRDUP(any_attr_impl->ns, env);
    return AXIS2_SUCCESS;
}

axis2_xml_schema_content_processing_t *AXIS2_CALL
axis2_xml_schema_any_attribute_get_process_content(void *any_attr,
                                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->process_content;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_set_process_content(void *any_attr,
                                            axis2_env_t **env,
                                            axis2_xml_schema_content_processing_t *
                                                process_content)
{
    axis2_xml_schema_any_attribute_impl_t *any_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);
    
    if(!any_attr_impl->process_content)
    {
        AXIS2_XML_SCHEMA_CONTENT_PROCESSING_FREE(any_attr_impl->process_content, env);
        any_attr_impl->process_content = NULL;
    }
    any_attr_impl->process_content = process_content;
    return AXIS2_SUCCESS;
}


axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_any_attribute_type(void *any_attr,
                                    axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->obj_type;    
}                                    

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_any_attribute_super_objs(void *any_attr,
                                        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->ht_super;
}                                        