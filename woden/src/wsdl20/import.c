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

#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_import.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/util/axis2_woden_generic_obj.h>

typedef struct axis2_woden_import_impl axis2_woden_import_impl_t;

/** 
 * @brief Feature Struct Impl
 *	Axis2 Feature  
 */ 
struct axis2_woden_import_impl
{
    axis2_woden_import_t import;
    axis2_woden_obj_types_t obj_type;
    axis2_woden_wsdl_ref_t *wsdl_ref;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    
    axis2_url_t *f_namespc;
};

#define INTF_TO_IMPL(import) ((axis2_woden_import_impl_t *) import)

axis2_status_t AXIS2_CALL 
axis2_woden_import_free(
        void *import,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_import_super_objs(
        void *import,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_import_type(
        void *import,
        axis2_env_t **env);

axis2_woden_wsdl_ref_t *AXIS2_CALL
axis2_woden_import_get_base_impl(
        void *import,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_import_set_namespace(
        void *import,
        axis2_env_t **env,
        axis2_url_t *ns_uri);

axis2_url_t *AXIS2_CALL
axis2_woden_import_get_namespace(
        void *import,
        axis2_env_t **env);

static axis2_woden_import_t *
create(axis2_env_t **env);

static axis2_status_t
axis2_woden_import_free_ops(
        void *import,
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_import_t *)
axis2_woden_import_to_import_element(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!import)
    {
        import_impl = (axis2_woden_import_impl_t *) create(env);
    }
    else
        import_impl = (axis2_woden_import_impl_t *) import;

    axis2_woden_import_free_ops(import, env);

    import_impl->import.base.import_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_import_element_ops_t));
    axis2_woden_import_element_resolve_methods(&(import_impl->import.base.
            import_element), env, import_impl->methods);
    return import;
}

AXIS2_DECLARE(axis2_woden_import_t *)
axis2_woden_import_to_wsdl_ref(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!import)
    {
        import_impl = (axis2_woden_import_impl_t *) create(env);
    }
    else
        import_impl = (axis2_woden_import_impl_t *) import;
    axis2_woden_import_free_ops(import, env);

    import_impl->import.base.wsdl_ref.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_wsdl_ref_ops_t));
    axis2_woden_documentable_resolve_methods(&(import_impl->import.base.
            wsdl_ref), env, import_impl->wsdl_ref, import_impl->methods);
    return import;
}

AXIS2_DECLARE(axis2_woden_import_t *)
axis2_woden_import_to_attr_extensible(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!import)
    {
        import_impl = (axis2_woden_import_impl_t *) create(env);
    }
    else
        import_impl = (axis2_woden_import_impl_t *) import;
    axis2_woden_import_free_ops(import, env);

    import_impl->import.base.wsdl_ref.wsdl_element.base.
            attr_extensible.ops =
            AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(import_impl->import.base.
            wsdl_ref.wsdl_element.base.attr_extensible), 
            env, NULL, import_impl->methods);
    return import;

}

AXIS2_DECLARE(axis2_woden_import_t *)
axis2_woden_import_to_element_extensible(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!import)
    {
        import_impl = (axis2_woden_import_impl_t *) create(env);
    }
    else
        import_impl = (axis2_woden_import_impl_t *) import;
    axis2_woden_import_free_ops(import, env);
    import_impl->import.base.wsdl_ref.wsdl_element.
                base.element_extensible.ops = AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(import_impl->import.base.
            wsdl_ref.wsdl_element.base.element_extensible), 
            env, NULL, import_impl->methods);
    return import;

}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_import_t *
create(axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_import_impl_t));

    import_impl->obj_type= AXIS2_WODEN_IMPORT;
    import_impl->super = NULL;
    import_impl->methods = NULL;
    import_impl->f_namespc = NULL;
    
    import_impl->import.base.import_element.ops = NULL;
    import_impl->import.base.wsdl_ref.ops = NULL;
    import_impl->import.base.wsdl_ref.wsdl_element.base.attr_extensible.ops = NULL;
    import_impl->import.base.wsdl_ref.wsdl_element.base.element_extensible.ops = NULL;
    
    import_impl->import.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_import_ops_t));

    import_impl->import.ops->free = axis2_woden_import_free;
    import_impl->import.ops->super_objs = axis2_woden_import_super_objs;
    import_impl->import.ops->type = axis2_woden_import_type;
    import_impl->import.ops->get_base_impl = axis2_woden_import_get_base_impl;

    import_impl->import.ops->get_namespace = axis2_woden_import_get_namespace;
    import_impl->import.ops->set_namespace = axis2_woden_import_set_namespace;
 
    import_impl->methods = axis2_hash_make(env);
    if(!import_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(import_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_import_free);
    axis2_hash_set(import_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_woden_import_super_objs);
    axis2_hash_set(import_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_import_type);

    axis2_hash_set(import_impl->methods, "set_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_import_set_namespace);
    axis2_hash_set(import_impl->methods, "get_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_import_get_namespace);

    return &(import_impl->import);
}

AXIS2_DECLARE(axis2_woden_import_t *)
axis2_woden_import_create(axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = (axis2_woden_import_impl_t *) create(env);

    import_impl->wsdl_ref = axis2_woden_wsdl_ref_create(env);

    import_impl->super = axis2_hash_make(env);
    if(!import_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(import_impl->super, "AXIS2_WODEN_IMPORT", 
            AXIS2_HASH_KEY_STRING, &(import_impl->import));
    axis2_hash_set(import_impl->super, "AXIS2_WODEN_WSDL_REF", 
            AXIS2_HASH_KEY_STRING, import_impl->wsdl_ref);
    
    return &(import_impl->import);
}

static axis2_status_t
axis2_woden_import_free_ops(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    if(import_impl->import.base.import_element.ops)
    {
        AXIS2_FREE((*env)->allocator, import_impl->import.base.
                import_element.ops);
        import_impl->import.base.import_element.ops = NULL;
    }

    if(import_impl->import.base.wsdl_ref.ops)
    {
        AXIS2_FREE((*env)->allocator, import_impl->import.base.wsdl_ref.ops);
        import_impl->import.base.wsdl_ref.ops = NULL;
    }

    if(import_impl->import.base.wsdl_ref.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, import_impl->import.base.wsdl_ref.
                wsdl_element.base.attr_extensible.ops);
        import_impl->import.base.wsdl_ref.wsdl_element.
                base.attr_extensible.ops = NULL;
    }

    if(import_impl->import.base.wsdl_ref.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, import_impl->import.base.wsdl_ref.
                wsdl_element.base.element_extensible.ops);
        import_impl->import.base.wsdl_ref.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_import_free(void *import,
                        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    if(import_impl->f_namespc)
    {
        AXIS2_URL_FREE(import_impl->f_namespc, env);
        import_impl->f_namespc = NULL;
    }
   
    if(import_impl->super)
    {
        axis2_hash_free(import_impl->super, env);
        import_impl->super = NULL;
    }
    
    if(import_impl->methods)
    {
        axis2_hash_free(import_impl->methods, env);
        import_impl->methods = NULL;
    }

    if(import_impl->wsdl_ref)
    {
        AXIS2_WODEN_WSDL_REF_FREE(import_impl->wsdl_ref, env);
        import_impl->wsdl_ref = NULL;
    }

    axis2_woden_import_free_ops(import, env);

    if((&(import_impl->import))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(import_impl->import))->ops);
        (&(import_impl->import))->ops = NULL;
    }
    
    if(import_impl)
    {
        AXIS2_FREE((*env)->allocator, import_impl);
        import_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_import_super_objs(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    return import_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_import_type(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = INTF_TO_IMPL(import);

    return import_impl->obj_type;
}

axis2_woden_wsdl_ref_t *AXIS2_CALL
axis2_woden_import_get_base_impl(void *import,
                                axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = INTF_TO_IMPL(import);

    return import_impl->wsdl_ref;
}

axis2_status_t AXIS2_CALL
axis2_woden_import_resolve_methods(
        axis2_woden_import_t *import,
        axis2_env_t **env,
        axis2_woden_import_t *import_impl,
        axis2_hash_t *methods)
{
    axis2_woden_import_impl_t *import_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    import_impl_l = INTF_TO_IMPL(import_impl);
    
    import->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    import->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    import->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    import->ops->get_namespace = axis2_hash_get(methods, 
            "get_namespace", AXIS2_HASH_KEY_STRING);
    if(!import->ops->get_namespace && import_impl_l)
            import->ops->get_namespace = 
            import_impl_l->import.ops->get_namespace;

    import->ops->set_namespace = axis2_hash_get(methods, 
            "set_namespace", AXIS2_HASH_KEY_STRING);
    if(!import->ops->set_namespace && import_impl_l)
            import->ops->set_namespace = 
            import_impl_l->import.ops->set_namespace;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_import_set_namespace(
        void *import,
        axis2_env_t **env,
        axis2_url_t *ns_uri)
{
    axis2_woden_import_impl_t *import_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ns_uri, AXIS2_FAILURE);
    super = AXIS2_WODEN_IMPORT_SUPER_OBJS(import, env);
    import_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_IMPORT", AXIS2_HASH_KEY_STRING));

    if(import_impl->f_namespc)
    {
        AXIS2_URI_FREE(import_impl->f_namespc, env);
        import_impl->f_namespc = NULL;
    }
    import_impl->f_namespc = AXIS2_URL_CLONE(ns_uri, env);

    return AXIS2_SUCCESS;
}

axis2_url_t *AXIS2_CALL
axis2_woden_import_get_namespace(
        void *import,
        axis2_env_t **env)
{
    axis2_woden_import_impl_t *import_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_IMPORT_SUPER_OBJS(import, env);
    import_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_IMPORT", AXIS2_HASH_KEY_STRING));

    return import_impl->f_namespc;
}

