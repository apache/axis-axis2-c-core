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

#include <axis2_xml_schema_identity_constraint.h>
#include <axis2_xml_schema_obj_collection.h>
#include <axis2_xml_schema_xpath.h>

typedef struct axis2_xml_schema_identity_constraint_impl axis2_xml_schema_identity_constraint_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_identity_constraint_impl
{
    axis2_xml_schema_identity_constraint_t identity_constraint;
    axis2_xml_schema_annotated_t *annotated;
    axis2_hash_t *methods;
    axis2_xml_schema_obj_collection_t *fields;
    axis2_char_t *name;
    axis2_xml_schema_xpath_t *selector;
};

#define AXIS2_INTF_TO_IMPL(identity_constraint) ((axis2_xml_schema_identity_constraint_impl_t *) identity_constraint)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_identity_constraint_free(void *identity_constraint,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_base_impl(void *identity_constraint,
                                        axis2_env_t **env);

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_fields(void *identity_constraint,
                                axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_name(void *identity_constraint,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_name(void *identity_constraint,
                                axis2_env_t **env,
                                axis2_char_t *name);

axis2_xml_schema_xpath_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_selector(void *identity_constraint,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_selector(void *identity_constraint,
                                    axis2_env_t **env,
                                    axis2_xml_schema_xpath_t *selector);

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_identity_constraint_create(axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    identity_constraint_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_identity_constraint_impl_t));

    identity_constraint_impl->annotated = NULL;
    identity_constraint_impl->methods = NULL;
    identity_constraint_impl->fields = NULL;
    identity_constraint_impl->name = NULL;
    identity_constraint_impl->selector = NULL;
    identity_constraint_impl->identity_constraint.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_identity_constraint_ops_t));

    identity_constraint_impl->identity_constraint.ops->free = 
            axis2_xml_schema_identity_constraint_free;
    identity_constraint_impl->identity_constraint.ops->get_base_impl = 
            axis2_xml_schema_identity_constraint_get_base_impl;
    identity_constraint_impl->identity_constraint.ops->get_fields = 
            axis2_xml_schema_identity_constraint_get_fields;
    identity_constraint_impl->identity_constraint.ops->get_name = 
            axis2_xml_schema_identity_constraint_get_name;
    identity_constraint_impl->identity_constraint.ops->set_name = 
            axis2_xml_schema_identity_constraint_set_name;
    identity_constraint_impl->identity_constraint.ops->get_selector = 
            axis2_xml_schema_identity_constraint_get_selector;
    identity_constraint_impl->identity_constraint.ops->set_selector = 
            axis2_xml_schema_identity_constraint_set_selector;
   
    identity_constraint_impl->methods = axis2_hash_make(env);
    if(!identity_constraint_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(identity_constraint_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_identity_constraint_free);
    axis2_hash_set(identity_constraint_impl->methods, "get_fields", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_fields);
    axis2_hash_set(identity_constraint_impl->methods, "get_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_name);
    axis2_hash_set(identity_constraint_impl->methods, "set_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_set_name);
    axis2_hash_set(identity_constraint_impl->methods, "get_selector", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_selector);
    axis2_hash_set(identity_constraint_impl->methods, "set_selector", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_set_selector);
    
    identity_constraint_impl->annotated = axis2_xml_schema_annotated_create(env);
    status = axis2_xml_schema_annotated_resolve_methods(
            &(identity_constraint_impl->identity_constraint.base), env, identity_constraint_impl->annotated, 
            identity_constraint_impl->methods);
    
    return &(identity_constraint_impl->identity_constraint);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_free(void *identity_constraint,
                                    axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    identity_constraint_impl = AXIS2_INTF_TO_IMPL(identity_constraint);

    if(identity_constraint_impl->name)
    {
        AXIS2_FREE((*env)->allocator, identity_constraint_impl->name);
        identity_constraint_impl->name = NULL;
    }

    if(identity_constraint_impl->selector)
    {
        AXIS2_XML_SCHEMA_XPATH_FREE(identity_constraint_impl->selector, env);
        identity_constraint_impl->selector = NULL;
    }
    
    if(identity_constraint_impl->methods)
    {
        axis2_hash_free(identity_constraint_impl->methods, env);
        identity_constraint_impl->methods = NULL;
    }

    if(identity_constraint_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(identity_constraint_impl->annotated, env);
        identity_constraint_impl->annotated = NULL;
    }
    
    if((&(identity_constraint_impl->identity_constraint))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(identity_constraint_impl->identity_constraint))->ops);
        (&(identity_constraint_impl->identity_constraint))->ops = NULL;
    }

    if(identity_constraint_impl)
    {
        AXIS2_FREE((*env)->allocator, identity_constraint_impl);
        identity_constraint_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_base_impl(void *identity_constraint,
                                axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    identity_constraint_impl = AXIS2_INTF_TO_IMPL(identity_constraint);

    return identity_constraint_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_identity_constraint_resolve_methods(
                axis2_xml_schema_identity_constraint_t *identity_constraint,
                axis2_env_t **env,
                axis2_xml_schema_identity_constraint_t *identity_constraint_impl,
                axis2_hash_t *methods)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, identity_constraint_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    identity_constraint_impl_l = (axis2_xml_schema_identity_constraint_impl_t *) identity_constraint_impl;
    
    identity_constraint->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_identity_constraint_ops_t));
    identity_constraint->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    identity_constraint->ops->get_base_impl = 
            identity_constraint_impl_l->identity_constraint.ops->get_base_impl;
    identity_constraint->ops->get_fields = 
            identity_constraint_impl_l->identity_constraint.ops->get_fields;
    identity_constraint->ops->get_name = 
            identity_constraint_impl_l->identity_constraint.ops->get_name;
    identity_constraint->ops->set_name = 
            identity_constraint_impl_l->identity_constraint.ops->set_name;
    identity_constraint->ops->get_selector = 
            identity_constraint_impl_l->identity_constraint.ops->get_selector;
    identity_constraint->ops->set_selector = 
            identity_constraint_impl_l->identity_constraint.ops->set_selector;
    
    return axis2_xml_schema_annotated_resolve_methods(&(identity_constraint->base), 
            env, identity_constraint_impl_l->annotated, methods);
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_fields(void *identity_constraint,
                                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(identity_constraint)->fields;
}
axis2_char_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_name(void *identity_constraint,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(identity_constraint)->name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_name(void *identity_constraint,
                                axis2_env_t **env,
                                axis2_char_t *name)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    identity_constraint_impl = AXIS2_INTF_TO_IMPL(identity_constraint);

    if(identity_constraint_impl->name)
    {
        AXIS2_FREE((*env)->allocator, identity_constraint_impl->name);
        identity_constraint_impl->name = NULL;
    }
    identity_constraint_impl->name = AXIS2_STRDUP(name, env);
    if(!identity_constraint_impl->name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_xpath_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_selector(void *identity_constraint,
                                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(identity_constraint)->selector;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_selector(void *identity_constraint,
                                    axis2_env_t **env,
                                    axis2_xml_schema_xpath_t *selector)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, selector, AXIS2_FAILURE);
    identity_constraint_impl = AXIS2_INTF_TO_IMPL(identity_constraint);
    
    identity_constraint_impl->selector = selector;
    return AXIS2_SUCCESS;
}

