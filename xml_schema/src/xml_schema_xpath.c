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

#include <xml_schema/axis2_xml_schema_xpath.h>

typedef struct axis2_xml_schema_xpath_impl axis2_xml_schema_xpath_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct axis2_xml_schema_xpath_impl
{
    axis2_xml_schema_xpath_t xpath;
    axis2_xml_schema_annotated_t *annotated;
    axis2_hash_t *methods;
    axis2_char_t *x_path;
    
    axis2_hash_t *ht_super;
    
    axis2_xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(xpath) ((axis2_xml_schema_xpath_impl_t *) xpath)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_xpath_free(void *xpath,
                        const axis2_env_t *env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_xpath_get_base_impl(void *xpath,
                                        const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_xpath_type(void *xpath,
                            const axis2_env_t *env);
                                        
axis2_hash_t *AXIS2_CALL
axis2_xml_schema_xpath_super_objs(void *xpath,
                                  const axis2_env_t *env);
                                                                                
axis2_char_t *AXIS2_CALL
axis2_xml_schema_xpath_get_xpath(void *xpath,
                                 const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_xpath_set_xpath(void *xpath,
                                 const axis2_env_t *env,
                                 axis2_char_t *x_path);

AXIS2_EXTERN axis2_xml_schema_xpath_t * AXIS2_CALL
axis2_xml_schema_xpath_create(const axis2_env_t *env)
{
    axis2_xml_schema_xpath_impl_t *xpath_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    xpath_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_xpath_impl_t));

    xpath_impl->annotated = NULL;
    xpath_impl->methods = NULL;
    xpath_impl->x_path = NULL;
    xpath_impl->ht_super = NULL;
    xpath_impl->obj_type = AXIS2_XML_SCHEMA_XPATH;
    xpath_impl->xpath.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_xpath_ops_t));

    xpath_impl->xpath.ops->free = axis2_xml_schema_xpath_free;
    xpath_impl->xpath.ops->get_base_impl = 
            axis2_xml_schema_xpath_get_base_impl;
    xpath_impl->xpath.ops->get_xpath = 
            axis2_xml_schema_xpath_get_xpath;
    xpath_impl->xpath.ops->set_xpath = 
            axis2_xml_schema_xpath_set_xpath;
    xpath_impl->xpath.ops->type =
            axis2_xml_schema_xpath_type; 
    xpath_impl->xpath.ops->super_objs =
            axis2_xml_schema_xpath_super_objs;                       
   
    xpath_impl->methods = axis2_hash_make(env);
    if(!xpath_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(xpath_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_xpath_free);
    axis2_hash_set(xpath_impl->methods, "get_xpath", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_xpath_get_xpath);
    axis2_hash_set(xpath_impl->methods, "set_xpath", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_xpath_set_xpath);
    
    xpath_impl->annotated = axis2_xml_schema_annotated_create(env);
    
    xpath_impl->ht_super = axis2_hash_make(env);
    if(!xpath_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(xpath_impl->ht_super, "AXIS2_XML_SCHEMA_XPATH", AXIS2_HASH_KEY_STRING,
        &(xpath_impl->xpath));
    axis2_hash_set(xpath_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", AXIS2_HASH_KEY_STRING,
        xpath_impl->annotated);        
    axis2_hash_set(xpath_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING,
        AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(xpath_impl->annotated, env));

    status = axis2_xml_schema_annotated_resolve_methods(
            &(xpath_impl->xpath.base), env, xpath_impl->annotated, 
            xpath_impl->methods);
    
    return &(xpath_impl->xpath);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_xpath_free(void *xpath,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_xpath_impl_t *xpath_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    xpath_impl = AXIS2_INTF_TO_IMPL(xpath);

    if(xpath_impl->x_path)
    {
        AXIS2_FREE(env->allocator, xpath_impl->x_path);
        xpath_impl->x_path = NULL;
    }
    if(xpath_impl->ht_super)
    {
        axis2_hash_free(xpath_impl->ht_super, env);
        xpath_impl->ht_super = NULL;
    }

    if(xpath_impl->methods)
    {
        axis2_hash_free(xpath_impl->methods, env);
        xpath_impl->methods = NULL;
    }

    if(xpath_impl->annotated)
    {
        AXIS2_XML_SCHEMA_OBJ_FREE(xpath_impl->annotated, env);
        xpath_impl->annotated = NULL;
    }
    
    if((&(xpath_impl->xpath))->ops)
    {
        AXIS2_FREE(env->allocator, (&(xpath_impl->xpath))->ops);
        (&(xpath_impl->xpath))->ops = NULL;
    }

    if(xpath_impl)
    {
        AXIS2_FREE(env->allocator, xpath_impl);
        xpath_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_xpath_get_base_impl(void *xpath,
                                const axis2_env_t *env)
{
    axis2_xml_schema_xpath_impl_t *xpath_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    xpath_impl = AXIS2_INTF_TO_IMPL(xpath);

    return xpath_impl->annotated;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_xpath_resolve_methods(
                                axis2_xml_schema_xpath_t *xpath,
                                const axis2_env_t *env,
                                axis2_xml_schema_xpath_t *xpath_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_xpath_impl_t *xpath_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    xpath_impl_l = (axis2_xml_schema_xpath_impl_t *) xpath_impl;
    
    xpath->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_xml_schema_xpath_ops_t));
    xpath->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    xpath->ops->get_base_impl = 
            xpath_impl_l->xpath.ops->get_base_impl;
    xpath->ops->get_xpath = 
            xpath_impl_l->xpath.ops->get_xpath;
    xpath->ops->set_xpath = 
            xpath_impl_l->xpath.ops->set_xpath;
    
    return axis2_xml_schema_annotated_resolve_methods(&(xpath->base), 
            env, xpath_impl_l->annotated, methods);
}
*/

axis2_char_t *AXIS2_CALL
axis2_xml_schema_xpath_get_xpath(void *xpath,
                                 const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(xpath)->x_path;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_xpath_set_xpath(void *xpath,
                                 const axis2_env_t *env,
                                 axis2_char_t *x_path)
{
    axis2_xml_schema_xpath_impl_t *xpath_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath, AXIS2_FAILURE);
    xpath_impl = AXIS2_INTF_TO_IMPL(xpath);
    
    if(xpath_impl->x_path)
    {
        AXIS2_FREE(env->allocator, xpath_impl->x_path);
        xpath_impl->x_path = NULL;
    }
    xpath_impl->x_path = AXIS2_STRDUP(x_path, env);
    if(!xpath_impl->x_path)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_xpath_type(void *xpath,
                            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(xpath)->obj_type;
}
                                        
axis2_hash_t *AXIS2_CALL
axis2_xml_schema_xpath_super_objs(void *xpath,
                                  const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(xpath)->ht_super;
}
