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

#include <xml_schema/axis2_xml_schema_documentation.h>
#include <xml_schema/axis2_xml_schema_use.h>

typedef struct axis2_xml_schema_documentation_impl 
                axis2_xml_schema_documentation_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_documentation_impl
{
    axis2_xml_schema_documentation_t documentation;
    
    axis2_xml_schema_obj_t *schema_obj;
    
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;

    axis2_char_t *source;
    
    void *markup; /* TODO Replace (void *) with node list */
};

#define AXIS2_INTF_TO_IMPL(documentation) \
        ((axis2_xml_schema_documentation_impl_t *) documentation)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_documentation_free(
        void *documentation,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_documentation_super_objs(
        void *documentation,
        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_documentation_type(
        void *documentation,
        axis2_env_t **env);

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_documentation_get_base_impl(void *documentation,
                                axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_documentation_get_source(void *documentation,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_documentation_set_source(void *documentation,
                                        axis2_env_t **env,
                                        axis2_char_t *source);

/* TODO replace (void *) mark up with node list */
void *AXIS2_CALL
axis2_xml_schema_documentation_get_markup(void *documentation,
                                        axis2_env_t **env);

/* TODO replace (void *) mark up with node list */
axis2_status_t AXIS2_CALL
axis2_xml_schema_documentation_set_markup(void *documentation,
                                        axis2_env_t **env,
                                        void *markup);
/****************** end macros ***********************************************/

AXIS2_DECLARE(axis2_xml_schema_documentation_t *)
axis2_xml_schema_documentation_create(axis2_env_t **env)
{
    axis2_xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    documentation_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_documentation_impl_t));
    if(!documentation_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    documentation_impl->schema_obj = NULL;
    documentation_impl->documentation.base.ops = NULL;
    documentation_impl->documentation.ops = NULL;
    documentation_impl->obj_type = AXIS2_XML_SCHEMA_APP_INFO;
    documentation_impl->ht_super = NULL;
    documentation_impl->methods = NULL;
    documentation_impl->source = NULL;
    documentation_impl->markup = NULL;
    
    documentation_impl->documentation.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_documentation_ops_t));
    
    if(!documentation_impl->documentation.ops)
    {
        axis2_xml_schema_documentation_free(&(documentation_impl->documentation), env);
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    documentation_impl->documentation.ops->free = 
        axis2_xml_schema_documentation_free;
    
    documentation_impl->documentation.ops->super_objs = 
        axis2_xml_schema_documentation_super_objs;
    
    documentation_impl->documentation.ops->type = 
        axis2_xml_schema_documentation_type;
    
    documentation_impl->documentation.ops->get_base_impl = 
        axis2_xml_schema_documentation_get_base_impl;
    
    documentation_impl->documentation.ops->get_source = 
        axis2_xml_schema_documentation_get_source;
    
    documentation_impl->documentation.ops->set_source = 
        axis2_xml_schema_documentation_set_source;
    
    documentation_impl->documentation.ops->get_markup = 
        axis2_xml_schema_documentation_get_markup;
    
    documentation_impl->documentation.ops->set_markup = 
        axis2_xml_schema_documentation_set_markup;
   
    documentation_impl->methods = axis2_hash_make(env);
    if(!documentation_impl->methods) 
    {
        axis2_xml_schema_documentation_free(&(documentation_impl->documentation), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentation_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_free);
    
    axis2_hash_set(documentation_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_super_objs);
    
    axis2_hash_set(documentation_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_type);
    
    axis2_hash_set(documentation_impl->methods, "get_source", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_get_source);
    
    axis2_hash_set(documentation_impl->methods, "set_source", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_set_source);
    
    axis2_hash_set(documentation_impl->methods, "get_markup", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_get_markup);
    
    axis2_hash_set(documentation_impl->methods, "set_markup", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_documentation_set_markup);

    documentation_impl->schema_obj = axis2_xml_schema_obj_create(env);
    if(!documentation_impl->schema_obj) 
    {
        axis2_xml_schema_documentation_free(&(documentation_impl->documentation), env);
        return NULL;
    }

    documentation_impl->ht_super = axis2_hash_make(env);
    
    if(!documentation_impl->ht_super) 
    {
        axis2_xml_schema_documentation_free(&(documentation_impl->documentation), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentation_impl->ht_super, "AXIS2_XML_SCHEMA_DOCUMENTATION", 
            AXIS2_HASH_KEY_STRING, &(documentation_impl->documentation));
            
    axis2_hash_set(documentation_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, documentation_impl->schema_obj);

    status = axis2_xml_schema_obj_resolve_methods(&(documentation_impl->documentation.base), 
            env, documentation_impl->schema_obj, documentation_impl->methods);
    return &(documentation_impl->documentation);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_documentation_free(void *documentation,
                                    axis2_env_t **env)
{
    axis2_xml_schema_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = AXIS2_INTF_TO_IMPL(documentation);

    if(NULL != documentation_impl->source)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->source);
        documentation_impl->source = NULL;
    }

    /* TODO Free markup */
    
    if(NULL != documentation_impl->ht_super)
    {
        axis2_hash_free(documentation_impl->ht_super, env);
        documentation_impl->ht_super = NULL;
    }
    
    if(NULL != documentation_impl->methods)
    {
        axis2_hash_free(documentation_impl->methods, env);
        documentation_impl->methods = NULL;
    }

    if(NULL != documentation_impl->schema_obj)
    {
        AXIS2_XML_SCHEMA_OBJ_FREE(documentation_impl->schema_obj, env);
        documentation_impl->schema_obj = NULL;
    }
    
    if(NULL != documentation_impl->documentation.ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.ops);
        documentation_impl->documentation.ops = NULL;
    }
    if(NULL != documentation_impl->documentation.base.ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.base.ops);
        documentation_impl->documentation.base.ops = NULL;    
    }
    if(NULL != documentation_impl)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl);
        documentation_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_documentation_super_objs(
        void *documentation,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(documentation)->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_documentation_type(
        void *documentation,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(documentation)->obj_type;
}

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_documentation_get_base_impl(
        void *documentation,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(documentation)->schema_obj;
}

axis2_char_t *AXIS2_CALL
axis2_xml_schema_documentation_get_source(void *documentation,
                                        axis2_env_t **env)
{
    axis2_xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  documentation_impl->source;   
    }
    
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_documentation_set_source(void *documentation,
                                        axis2_env_t **env,
                                        axis2_char_t *source)
{
    axis2_xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, source, AXIS2_FAILURE);
    
    ht_super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  AXIS2_FAILURE;   
    }
    
    if(NULL != documentation_impl->source)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->source);
        documentation_impl->source = NULL;
    }

    documentation_impl->source = AXIS2_STRDUP(source, env);
    if(!documentation_impl->source)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* TODO replace (void *) mark up with node list */
void *AXIS2_CALL
axis2_xml_schema_documentation_get_markup(void *documentation,
                                        axis2_env_t **env)
{
    axis2_xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  NULL;   
    } 
    return documentation_impl->markup;
}

/* TODO replace (void *) mark up with node list */
axis2_status_t AXIS2_CALL
axis2_xml_schema_documentation_set_markup(void *documentation,
                                        axis2_env_t **env,
                                        void *markup)
{
    axis2_xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, markup, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  AXIS2_FAILURE;   
    } 
    documentation_impl->markup = markup;
    return AXIS2_SUCCESS;
}
