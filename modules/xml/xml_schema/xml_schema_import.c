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

#include  <xml_schema/axis2_xml_schema_import.h>
#include  <xml_schema/axis2_xml_schema.h>

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
typedef struct axis2_xml_schema_import_impl
{
    axis2_xml_schema_import_t import;
    
    axis2_xml_schema_external_t *external;
        
    axis2_char_t *ns;
        
    axis2_hash_t *methods;

}axis2_xml_schema_import_impl_t;

#define AXIS2_INTF_TO_IMPL(import) \
        ((axis2_xml_schema_import_impl_t *) import)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_import_free(void *import,
                        axis2_env_t **env);

axis2_xml_schema_external_t *AXIS2_CALL
axis2_xml_schema_import_get_base_impl(void *import,
                                        axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_xml_schema_import_get_namespace(void *import,
                                     axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_import_set_namespace(void *import,
                                     axis2_env_t **env,
                                     axis2_char_t *ns);

/********************* end function prototypes ********************************/

AXIS2_DECLARE(axis2_xml_schema_import_t *)
axis2_xml_schema_import_create(axis2_env_t **env)
{
    axis2_xml_schema_import_impl_t *import_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    import_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_import_impl_t));
    if(!import_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    import_impl->ns = NULL;
    import_impl->external = NULL;
    import_impl->methods = NULL;
    import_impl->import.ops = NULL;
    import_impl->import.base.ops = NULL;
    
    import_impl->import.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_import_ops_t));

    if(!import_impl->import.ops)
    {
        axis2_xml_schema_import_free(&(import_impl->import), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    import_impl->import.ops->free = 
        axis2_xml_schema_import_free;
    import_impl->import.ops->get_base_impl = 
        axis2_xml_schema_import_get_base_impl;
        
    import_impl->import.ops->get_namespace = 
            axis2_xml_schema_import_get_namespace;
            
    import_impl->import.ops->set_namespace = 
            axis2_xml_schema_import_set_namespace;

    import_impl->methods = axis2_hash_make(env);
    if(!import_impl->methods)
    {
        axis2_xml_schema_import_free(&(import_impl->import), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(import_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_import_free);

    axis2_hash_set(import_impl->methods, "get_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_import_get_namespace);
    axis2_hash_set(import_impl->methods, "set_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_import_set_namespace);

    import_impl->external = axis2_xml_schema_external_create(env);
     if(!import_impl->external)
    {
        axis2_xml_schema_import_free(&(import_impl->import), env);
        return NULL;
    }
    status = axis2_xml_schema_external_resolve_methods(
            &(import_impl->import.base), env, import_impl->external, 
            import_impl->methods);

    return &(import_impl->import);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_import_free(void *import,
                                    axis2_env_t **env)
{
    axis2_xml_schema_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = AXIS2_INTF_TO_IMPL(import);

    if(NULL != import_impl->methods)
    {
        axis2_hash_free(import_impl->methods, env);
        import_impl->methods = NULL;
    }

    if(NULL != import_impl->external)
    {
        AXIS2_XML_SCHEMA_EXTERNAL_FREE(import_impl->external, env);
        import_impl->external = NULL;
    }
    
    if(NULL != import_impl->import.ops)
    {
        AXIS2_FREE((*env)->allocator, import_impl->import.ops);
        import_impl->import.ops = NULL;
    }
    if(NULL != import_impl->import.base.ops)
    {
        AXIS2_FREE((*env)->allocator, import_impl->import.base.ops);
        import_impl->import.base.ops = NULL;        
    }
    AXIS2_FREE((*env)->allocator, import_impl);
    import_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_external_t *AXIS2_CALL
axis2_xml_schema_import_get_base_impl(void *import,
                                axis2_env_t **env)
{
    axis2_xml_schema_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = AXIS2_INTF_TO_IMPL(import);

    return import_impl->external;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_import_resolve_methods(
                                axis2_xml_schema_import_t *import,
                                axis2_env_t **env,
                                axis2_xml_schema_import_t *import_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_import_impl_t *import_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, import_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    import_impl_l = (axis2_xml_schema_import_impl_t *) import_impl;
    
    import->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_import_ops_t));
            
    import->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
            
    import->ops->get_base_impl = 
        import_impl_l->import.ops->get_base_impl;
    import->ops->set_namespace = 
        import_impl_l->import.ops->set_namespace;
    import->ops->get_namespace = 
        import_impl_l->import.ops->get_namespace;
        
    return axis2_xml_schema_external_resolve_methods(&(import->base), 
            env, import_impl_l->external, methods);
}

axis2_char_t * AXIS2_CALL
axis2_xml_schema_import_get_namespace(void *import,
                                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(import)->ns;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_import_set_namespace(void *import,
                                     axis2_env_t **env,
                                     axis2_char_t *ns)
{
    axis2_xml_schema_import_impl_t *import_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = AXIS2_INTF_TO_IMPL(import);
    if(NULL != import_impl->ns)
    {
        AXIS2_FREE((*env)->allocator, ns);
        import_impl->ns = NULL;
    }
    import_impl->ns = ns;
    return AXIS2_SUCCESS;
}
