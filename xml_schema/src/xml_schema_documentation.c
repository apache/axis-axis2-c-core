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

#include <xml_schema_documentation.h>
#include <xml_schema_use.h>
#include <axis2_utils.h>

typedef struct xml_schema_documentation_impl 
                xml_schema_documentation_impl_t;

/** 
 */ 
struct xml_schema_documentation_impl
{
    xml_schema_documentation_t documentation;
    
    xml_schema_obj_t *schema_obj;
    
    xml_schema_types_t obj_type;
    
    axis2_char_t *language;
    
    axis2_hash_t *ht_super;
    
    axis2_char_t *source;
    
    axiom_child_element_iterator_t *markup; 
};

#define AXIS2_INTF_TO_IMPL(documentation) \
        ((xml_schema_documentation_impl_t *) documentation)

axis2_status_t AXIS2_CALL 
xml_schema_documentation_free(
        void *documentation,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
xml_schema_documentation_super_objs(
        void *documentation,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL 
xml_schema_documentation_get_type(
        void *documentation,
        const axis2_env_t *env);

xml_schema_obj_t *AXIS2_CALL
xml_schema_documentation_get_base_impl(void *documentation,
                                const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
xml_schema_documentation_get_source(void *documentation,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_documentation_set_source(void *documentation,
                                        const axis2_env_t *env,
                                        axis2_char_t *source);

axiom_child_element_iterator_t *AXIS2_CALL
xml_schema_documentation_get_markup(void *documentation,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_documentation_set_markup(
        void *documentation,
        const axis2_env_t *env,
        axiom_child_element_iterator_t *markup);
        
axis2_status_t AXIS2_CALL
xml_schema_documentation_set_language(
        void *documentation,
        const axis2_env_t *env,
        axis2_char_t *language);
        
axis2_char_t* AXIS2_CALL
xml_schema_documentation_get_language(
        void *documentation,
        const axis2_env_t *env);        

/****************** end macros ***********************************************/

AXIS2_EXTERN xml_schema_documentation_t * AXIS2_CALL
xml_schema_documentation_create(const axis2_env_t *env)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    documentation_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_documentation_impl_t));
    if(!documentation_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    documentation_impl->schema_obj = NULL;
    documentation_impl->documentation.base.ops = NULL;
    documentation_impl->documentation.ops = NULL;
    documentation_impl->obj_type = XML_SCHEMA_APP_INFO;
    documentation_impl->ht_super = NULL;
    documentation_impl->source = NULL;
    documentation_impl->markup = NULL;
    documentation_impl->language  = NULL;
    
    documentation_impl->documentation.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_documentation_ops_t));
    
    if(!documentation_impl->documentation.ops)
    {
        xml_schema_documentation_free(&(documentation_impl->documentation), env);
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    documentation_impl->documentation.ops->free = 
        xml_schema_documentation_free;
    
    documentation_impl->documentation.ops->super_objs = 
        xml_schema_documentation_super_objs;
    
    documentation_impl->documentation.ops->get_type = 
        xml_schema_documentation_get_type;
    
    documentation_impl->documentation.ops->get_base_impl = 
        xml_schema_documentation_get_base_impl;
    
    documentation_impl->documentation.ops->get_source = 
        xml_schema_documentation_get_source;
    
    documentation_impl->documentation.ops->set_source = 
        xml_schema_documentation_set_source;
    
    documentation_impl->documentation.ops->get_markup = 
        xml_schema_documentation_get_markup;
    
    documentation_impl->documentation.ops->set_markup = 
        xml_schema_documentation_set_markup;
        
    documentation_impl->documentation.ops->set_language =
        xml_schema_documentation_set_language;
        
    documentation_impl->documentation.ops->get_language =
        xml_schema_documentation_get_language;
                   
   
    documentation_impl->schema_obj = xml_schema_obj_create(env);
    if(!documentation_impl->schema_obj) 
    {
        xml_schema_documentation_free(&(documentation_impl->documentation), env);
        return NULL;
    }

    documentation_impl->ht_super = axis2_hash_make(env);
    
    if(!documentation_impl->ht_super) 
    {
        xml_schema_documentation_free(&(documentation_impl->documentation), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentation_impl->ht_super, 
            AXIS2_STRDUP("XML_SCHEMA_DOCUMENTATION", env), 
            AXIS2_HASH_KEY_STRING, &(documentation_impl->documentation));
            
    axis2_hash_set(documentation_impl->ht_super, 
            AXIS2_STRDUP("XML_SCHEMA_OBJ", env), 
            AXIS2_HASH_KEY_STRING, documentation_impl->schema_obj);

    status = xml_schema_obj_resolve_methods(&(documentation_impl->documentation.base), 
            env, documentation_impl->schema_obj, 
            xml_schema_documentation_super_objs,
            xml_schema_documentation_get_type,
            xml_schema_documentation_free);
    return &(documentation_impl->documentation);
}

axis2_status_t AXIS2_CALL
xml_schema_documentation_free(void *documentation,
                                    const axis2_env_t *env)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = AXIS2_INTF_TO_IMPL(documentation);

    if(NULL != documentation_impl->source)
    {
        AXIS2_FREE(env->allocator, documentation_impl->source);
        documentation_impl->source = NULL;
    }

    /* TODO Free markup */
    
    if(NULL != documentation_impl->ht_super)
    {
        axis2_hash_free(documentation_impl->ht_super, env);
        documentation_impl->ht_super = NULL;
    }
    
    if(NULL != documentation_impl->schema_obj)
    {
        XML_SCHEMA_OBJ_FREE(documentation_impl->schema_obj, env);
        documentation_impl->schema_obj = NULL;
    }
    
    if(NULL != documentation_impl->documentation.ops)
    {
        AXIS2_FREE(env->allocator, documentation_impl->documentation.ops);
        documentation_impl->documentation.ops = NULL;
    }
    if(NULL != documentation_impl->documentation.base.ops)
    {
        AXIS2_FREE(env->allocator, documentation_impl->documentation.base.ops);
        documentation_impl->documentation.base.ops = NULL;    
    }
    if(NULL != documentation_impl)
    {
        AXIS2_FREE(env->allocator, documentation_impl);
        documentation_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_documentation_super_objs(
        void *documentation,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(documentation)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_documentation_get_type(
        void *documentation,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(documentation)->obj_type;
}

xml_schema_obj_t *AXIS2_CALL
xml_schema_documentation_get_base_impl(
        void *documentation,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(documentation)->schema_obj;
}

axis2_char_t *AXIS2_CALL
xml_schema_documentation_get_source(void *documentation,
                                        const axis2_env_t *env)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_APP_INFO",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  documentation_impl->source;   
    }
    
    return NULL;
}

axis2_status_t AXIS2_CALL
xml_schema_documentation_set_source(void *documentation,
                                        const axis2_env_t *env,
                                        axis2_char_t *source)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, source, AXIS2_FAILURE);
    
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_DOCUMENTATION",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  AXIS2_FAILURE;   
    }
    
    if(NULL != documentation_impl->source)
    {
        AXIS2_FREE(env->allocator, documentation_impl->source);
        documentation_impl->source = NULL;
    }

    documentation_impl->source = AXIS2_STRDUP(source, env);
    if(!documentation_impl->source)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axiom_child_element_iterator_t *AXIS2_CALL
xml_schema_documentation_get_markup(void *documentation,
                                        const axis2_env_t *env)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_DOCUMENTATION",
                AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  NULL;   
    } 
    return documentation_impl->markup;
}

axis2_status_t AXIS2_CALL
xml_schema_documentation_set_markup(void *documentation,
                                        const axis2_env_t *env,
                                        axiom_child_element_iterator_t *markup)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, markup, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_USE_SUPER_OBJS(documentation, env);
    if(NULL != ht_super)
    {
        documentation_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
            "XML_SCHEMA_DOCUMENTATION", AXIS2_HASH_KEY_STRING));
        if(NULL != documentation_impl)
            return  AXIS2_FAILURE;   
    } 
    documentation_impl->markup = markup;
    return AXIS2_SUCCESS;
}
axis2_status_t AXIS2_CALL
xml_schema_documentation_set_language(
        void *documentation,
        const axis2_env_t *env,
        axis2_char_t *language)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, language, AXIS2_FAILURE);
    
    documentation_impl = AXIS2_INTF_TO_IMPL(documentation);
    if(NULL != documentation_impl->language)
    {
        AXIS2_FREE(env->allocator, documentation_impl->language);
        documentation_impl->language = NULL;
    }
    documentation_impl->language = AXIS2_STRDUP(language, env);
    return AXIS2_SUCCESS;
}        
        
axis2_char_t* AXIS2_CALL
xml_schema_documentation_get_language(
        void *documentation,
        const axis2_env_t *env)
{
    xml_schema_documentation_impl_t *documentation_impl = NULL;
    documentation_impl = AXIS2_INTF_TO_IMPL(documentation);
    return documentation_impl->language;
}        
