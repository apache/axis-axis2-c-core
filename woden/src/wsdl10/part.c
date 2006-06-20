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

#include <woden_wsdl_obj.h>
#include <woden_wsdl10_part.h>
#include <woden_types.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_msg_label.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>
#include <woden_element_decl.h>

typedef struct woden_wsdl10_part_impl woden_wsdl10_part_impl_t;

/** 
 * @brief Interface Message Reference Struct Impl
 *   Axis2 Interface Message Reference  
 */ 
struct woden_wsdl10_part_impl
{
    woden_wsdl10_part_t part;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;

    axis2_qname_t *f_qname;
    void *f_element_decl;
    axis2_qname_t *f_element_qname;
    void *f_types;
};

#define INTF_TO_IMPL(part) ((woden_wsdl10_part_impl_t *) part)

axis2_status_t AXIS2_CALL 
woden_wsdl10_part_free(
        void *part,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
woden_wsdl10_part_super_objs(
        void *part,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_wsdl10_part_type(
        void *part,
        const axis2_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_part_get_base_impl(
        void *part,
        const axis2_env_t *env);

/* ************************************************************
 *  Interface Message Reference  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
woden_wsdl10_part_get_qname(
        void *part,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_part_get_element_declaration(
        void *part,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_part_to_element(
        void *part,
        const axis2_env_t *env);

/* ************************************************************
 *  Interface Message Reference Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_qname(
        void *part,
        const axis2_env_t *env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_element_qname(
        void *part,
        const axis2_env_t *env,
        axis2_qname_t *element);

axis2_qname_t *AXIS2_CALL
woden_wsdl10_part_get_element_qname(
        void *part,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_part_get_element(
        void *part,
        const axis2_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_element_declaration(
        void *part,
        const axis2_env_t *env,
        void *element);

axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_types(
        void *part,
        const axis2_env_t *env,
        void *types);


static woden_wsdl10_part_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_wsdl10_part_free_ops(
        void *part,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_part_element(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.part_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_wsdl10_part_element_ops_t));
    woden_wsdl10_part_element_resolve_methods(&(part_impl->part.base.
            part_element), env, part_impl->methods);
    return part;
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_nested_configurable(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.nested_configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(part_impl->part.base.
            nested_configurable), env, part_impl->nested_configurable, 
            part_impl->methods);
    return part;
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_configurable(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            part_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(part_impl->part.base.
            nested_configurable.base.configurable), env, configurable, part_impl->methods);
    return part;
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_nested_element(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.part_element.base.nested_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(part_impl->part.base.
            part_element.base.nested_element), env, part_impl->methods);
    return part;
}


AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_configurable_element(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.part_element.base.configurable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(part_impl->part.base.
            part_element.base.configurable_element), env, part_impl->methods);
    return part;
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_documentable_element(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.part_element.base.documentable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(part_impl->part.base.
            part_element.base.documentable_element), env, 
            part_impl->methods);
    return part;
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_documentable(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.nested_configurable.base.
        configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(part_impl->part.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            part_impl->methods);
    return part;
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_attr_extensible(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.part_element.base.documentable_element.
        wsdl_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(part_impl->part.base.
            part_element.base.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, part_impl->methods);
    return part;
}


AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_element_extensible(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!part)
    {
        part_impl = (woden_wsdl10_part_impl_t *) create(env);
    }
    else
        part_impl = (woden_wsdl10_part_impl_t *) part;

    woden_wsdl10_part_free_ops(part, env);

    part_impl->part.base.part_element.base.documentable_element.
        wsdl_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(part_impl->part.base.
            part_element.base.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, part_impl->methods);
    return part;
}


/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_part_t *
create(const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    part_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_wsdl10_part_impl_t));

    part_impl->obj_type= WODEN_WSDL10_PART;
    part_impl->super = NULL;
    part_impl->methods = NULL;
    part_impl->f_qname = NULL;
    part_impl->f_element_decl = NULL;
    part_impl->f_types = NULL;
    part_impl->f_element_qname = NULL;
    
    part_impl->part.base.part_element.ops = NULL;
    part_impl->part.base.nested_configurable.ops = NULL;
    part_impl->part.base.nested_configurable.base.configurable.ops = 
            NULL;
    part_impl->part.base.part_element.base.
        nested_element.ops = NULL;
    part_impl->part.base.part_element.base.
        configurable_element.ops = NULL;
    part_impl->part.base.part_element.base.
        documentable_element.ops = NULL;
    part_impl->part.base.nested_configurable.base.configurable.base.
        documentable.ops = NULL;
    part_impl->part.base.part_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    part_impl->part.base.part_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;


    part_impl->part.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_wsdl10_part_ops_t));

    part_impl->part.ops->free = woden_wsdl10_part_free;
    part_impl->part.ops->super_objs = woden_wsdl10_part_super_objs;
    part_impl->part.ops->type = woden_wsdl10_part_type;
    part_impl->part.ops->get_base_impl = woden_wsdl10_part_get_base_impl;
    
    part_impl->part.ops->get_qname = 
        woden_wsdl10_part_get_qname;
    part_impl->part.ops->get_element_declaration = 
        woden_wsdl10_part_get_element_declaration;
    part_impl->part.ops->to_element = 
        woden_wsdl10_part_to_element;
    part_impl->part.ops->set_element_declaration = 
        woden_wsdl10_part_set_element_declaration;
    part_impl->part.ops->set_types = 
        woden_wsdl10_part_set_types;
 
    part_impl->methods = axis2_hash_make(env);
    if(!part_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(part_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_free);
    axis2_hash_set(part_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, woden_wsdl10_part_super_objs);
    axis2_hash_set(part_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_wsdl10_part_type);

    axis2_hash_set(part_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_get_qname);
    axis2_hash_set(part_impl->methods, "get_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_get_element_declaration);
    axis2_hash_set(part_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_to_element);
    axis2_hash_set(part_impl->methods, "set_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_set_element_qname);
    axis2_hash_set(part_impl->methods, "get_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_get_element_qname);
    axis2_hash_set(part_impl->methods, "get_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_get_element);
    axis2_hash_set(part_impl->methods, "set_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_set_qname);
    axis2_hash_set(part_impl->methods, "set_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_set_element_declaration);
    axis2_hash_set(part_impl->methods, "set_types", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl10_part_set_types);

    return &(part_impl->part);
}

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_create(const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    part_impl = (woden_wsdl10_part_impl_t *) create(env);

    part_impl->nested_configurable = woden_nested_configurable_create(env);

    part_impl->super = axis2_hash_make(env);
    if(!part_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(part_impl->super, "WODEN_WSDL10_PART", 
            AXIS2_HASH_KEY_STRING, 
            &(part_impl->part));
    axis2_hash_set(part_impl->super, "WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            part_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            part_impl->nested_configurable, env);
    axis2_hash_set(part_impl->super, "WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(part_impl->part);
}

static axis2_status_t
woden_wsdl10_part_free_ops(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    part_impl = INTF_TO_IMPL(part);

    if(part_impl->part.base.part_element.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                part_element.ops);
        part_impl->part.base.part_element.ops = NULL;
    }

    if(part_impl->part.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                nested_configurable.ops);
        part_impl->part.base.nested_configurable.ops = 
            NULL;
    }
    
    if(part_impl->part.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                nested_configurable.base.configurable.ops);
        part_impl->part.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
   
    if(part_impl->part.base.part_element.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                part_element.base.nested_element.ops);
        part_impl->part.base.part_element.base.
            nested_element.ops = NULL;
    }
 
    if(part_impl->part.base.part_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                part_element.base.configurable_element.ops);
        part_impl->part.base.part_element.base.
            configurable_element.ops = NULL;
    }
    
    if(part_impl->part.base.part_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                part_element.base.documentable_element.ops);
        part_impl->part.base.part_element.base.
            documentable_element.ops = NULL;
    }
     
    if(part_impl->part.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                nested_configurable.base.configurable.base.documentable.ops);
        part_impl->part.base.nested_configurable.base.
            configurable.base.documentable.ops = NULL;
    }
      
    if(part_impl->part.base.part_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                part_element.base.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        part_impl->part.base.part_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }
      
    if(part_impl->part.base.part_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, part_impl->part.base.
                part_element.base.documentable_element.wsdl_element.base.
                element_extensible.ops);
        part_impl->part.base.part_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }
      
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_part_free(void *part,
                        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    part_impl = INTF_TO_IMPL(part);

    if(part_impl->f_qname)
    {
        AXIS2_QNAME_FREE(part_impl->f_qname, env);
        part_impl->f_qname = NULL;
    }

    if(part_impl->f_element_decl)
    {
        WODEN_ELEMENT_DECL_FREE(part_impl->f_element_decl, env);
        part_impl->f_element_decl = NULL;
    }

    if(part_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(part_impl->f_element_qname, env);
        part_impl->f_element_qname = NULL;
    }

    if(part_impl->f_types)
    {
        WODEN_TYPES_FREE(part_impl->f_types, env);
        part_impl->f_types = NULL;
    }

    if(part_impl->super)
    {
        axis2_hash_free(part_impl->super, env);
        part_impl->super = NULL;
    }
    
    if(part_impl->methods)
    {
        axis2_hash_free(part_impl->methods, env);
        part_impl->methods = NULL;
    }

    if(part_impl->nested_configurable)
    {
        WODEN_NESTED_CONFIGURABLE_FREE(part_impl->
                nested_configurable, env);
        part_impl->nested_configurable = NULL;
    }

    woden_wsdl10_part_free_ops(part, env);

    if((&(part_impl->part))->ops)
    {
        AXIS2_FREE(env->allocator, (&(part_impl->part))->ops);
        (&(part_impl->part))->ops = NULL;
    }
    
    if(part_impl)
    {
        AXIS2_FREE(env->allocator, part_impl);
        part_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_wsdl10_part_super_objs(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    part_impl = INTF_TO_IMPL(part);

    return part_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_part_type(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    part_impl = INTF_TO_IMPL(part);

    return part_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_part_get_base_impl(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    part_impl = INTF_TO_IMPL(part);

    return part_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_part_resolve_methods(
        woden_wsdl10_part_t *part,
        const axis2_env_t *env,
        woden_wsdl10_part_t *part_impl,
        axis2_hash_t *methods)
{
    woden_wsdl10_part_impl_t *part_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    part_impl_l = INTF_TO_IMPL(part_impl);
    
    part->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    part->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    part->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    part->ops->get_qname = axis2_hash_get(methods, 
            "get_qname", AXIS2_HASH_KEY_STRING);
    if(!part->ops->get_qname && part_impl_l)
            part->ops->get_qname = 
            part_impl_l->part.ops->get_qname;
    
    part->ops->get_element_declaration = axis2_hash_get(methods, 
            "get_element_declaration", AXIS2_HASH_KEY_STRING);
    if(!part->ops->get_element_declaration && part_impl_l)
            part->ops->get_element_declaration = 
            part_impl_l->part.ops->get_element_declaration;
    
    part->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!part->ops->to_element && part_impl_l)
            part->ops->to_element = 
            part_impl_l->part.ops->to_element;
    
    part->ops->set_element_declaration = axis2_hash_get(methods, 
            "set_element_declaration", AXIS2_HASH_KEY_STRING);
    if(!part->ops->set_element_declaration && part_impl_l)
            part->ops->set_element_declaration = 
            part_impl_l->part.ops->set_element_declaration;
    
    part->ops->set_types = axis2_hash_get(methods, 
            "set_types", AXIS2_HASH_KEY_STRING);
    if(!part->ops->set_types && part_impl_l)
            part->ops->set_types = 
            part_impl_l->part.ops->set_types;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Message Reference  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
woden_wsdl10_part_get_qname(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    return part_impl->f_qname;
}

void *AXIS2_CALL
woden_wsdl10_part_get_element_declaration(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));
    
    return part_impl->f_element_decl;
}

void *AXIS2_CALL
woden_wsdl10_part_to_element(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));
    
    return &(part_impl->part);
}

/* ************************************************************
 *  Interface Message Reference Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_qname(
        void *part,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    if(!part_impl->f_qname)
    {
        AXIS2_QNAME_FREE(part_impl->f_qname, env);
        part_impl->f_qname = NULL;
    }
    part_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);


    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_element_qname(
        void *part,
        const axis2_env_t *env,
        axis2_qname_t *element)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    if(part_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(part_impl->f_element_qname, env);
    }
    part_impl->f_element_qname = AXIS2_QNAME_CLONE(element, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_wsdl10_part_get_element_qname(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    return part_impl->f_element_qname;
}

void *AXIS2_CALL
woden_wsdl10_part_get_element(
        void *part,
        const axis2_env_t *env)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;
    xml_schema_element_t *xse = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    if(part_impl->f_types)
    {
        xse = (xml_schema_element_t *) 
            WODEN_TYPES_GET_ELEMENT_DECLARATION(
            part_impl->f_types, env, part_impl->
            f_element_qname);
    }

    return xse;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_element_declaration(
        void *part,
        const axis2_env_t *env,
        void *element)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    if(!part_impl->f_element_decl)
    {
        /* TODO */
    }
    part_impl->f_element_decl = element;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_part_set_types(
        void *part,
        const axis2_env_t *env,
        void *types)
{
    woden_wsdl10_part_impl_t *part_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, types, AXIS2_FAILURE);
    super = WODEN_WSDL10_PART_SUPER_OBJS(part, env);
    part_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL10_PART", AXIS2_HASH_KEY_STRING));

    if(!part_impl->f_types)
    {
        /* TODO */
    }
    part_impl->f_types = types;
    
    return AXIS2_SUCCESS;
}

