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

#include <woden/wsdl20/axis2_woden_documentable.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>

typedef struct axis2_woden_documentable_impl axis2_woden_documentable_impl_t;

/** 
 * @brief Documentable Struct Impl
 *	Axis2 Documentable  
 */ 
struct axis2_woden_documentable_impl
{
    axis2_woden_documentable_t documentable;
    axis2_woden_wsdl_obj_t *wsdl_obj;
    axis2_array_list_t *f_doc_elems;
};

#define INTF_TO_IMPL(documentable) ((axis2_woden_documentable_impl_t *) documentable)

axis2_status_t AXIS2_CALL 
axis2_woden_documentable_free(
        void *documentable,
        axis2_env_t **env);

axis2_woden_wsdl_obj_t *AXIS2_CALL
axis2_woden_documentable_get_base_impl(
        void *documentable,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_documentable_add_documentation_element(
        void *documentable,
        axis2_env_t **env,
        axis2_woden_documentation_element_t *documentation);

axis2_array_list_t *AXIS2_CALL
axis2_woden_documentable_get_documentation_elements(
        void *documentable,
        axis2_env_t **env);

static axis2_woden_documentable_t *
create(
        axis2_env_t **env)
{
    axis2_woden_documentable_impl_t *documentable_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentable_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_documentable_impl_t));

    documentable_impl->f_doc_elems = NULL;
    
    documentable_impl->documentable.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_documentable_ops_t));

    documentable_impl->documentable.ops->free = axis2_woden_documentable_free;
    documentable_impl->documentable.ops->get_base_impl = axis2_woden_documentable_get_base_impl;
    documentable_impl->documentable.ops->add_documentation_element = 
        axis2_woden_documentable_add_documentation_element;
    documentable_impl->documentable.ops->get_documentation_elements = 
        axis2_woden_documentable_get_documentation_elements;

    return &(documentable_impl->documentable);
}

AXIS2_DECLARE(axis2_woden_documentable_t *)
axis2_woden_documentable_create(
        axis2_env_t **env)
{
    axis2_woden_documentable_impl_t *documentable_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentable_impl = (axis2_woden_documentable_impl_t *) create(env);

    documentable_impl->wsdl_obj = axis2_woden_wsdl_obj_create(env);

    return &(documentable_impl->documentable);
}

axis2_status_t AXIS2_CALL
axis2_woden_documentable_free(
        void *documentable,
        axis2_env_t **env)
{
    axis2_woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentable_impl = INTF_TO_IMPL(documentable);

    if(documentable_impl->f_doc_elems)
    {
        AXIS2_ARRAY_LIST_FREE(documentable_impl->f_doc_elems, env);
        documentable_impl->f_doc_elems = NULL;
    }
    
    if(documentable_impl->wsdl_obj)
    {
        AXIS2_WODEN_WSDL_OBJ_FREE(documentable_impl->wsdl_obj, env);
        documentable_impl->wsdl_obj = NULL;
    }
    
    if(documentable_impl->documentable.ops)
    {
        AXIS2_FREE((*env)->allocator, documentable_impl->documentable.ops);
        documentable_impl->documentable.ops = NULL;
    }
    
    if(documentable_impl)
    {
        AXIS2_FREE((*env)->allocator, documentable_impl);
        documentable_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_wsdl_obj_t *AXIS2_CALL
axis2_woden_documentable_get_base_impl(
        void *documentable,
        axis2_env_t **env)
{
    axis2_woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentable_impl = INTF_TO_IMPL(documentable);

    return documentable_impl->wsdl_obj;
}

axis2_status_t AXIS2_CALL
axis2_woden_documentable_resolve_methods(
        axis2_woden_documentable_t *documentable,
        axis2_env_t **env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    documentable->ops = AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_ops_t));
    documentable->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    documentable->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    documentable->ops->add_documentation_element = axis2_hash_get(methods, 
            "add_documentation_element", AXIS2_HASH_KEY_STRING);
    documentable->ops->get_documentation_elements = axis2_hash_get(methods, 
            "get_documentation_elements", AXIS2_HASH_KEY_STRING);
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_documentable_add_documentation_element(
        void *documentable,
        axis2_env_t **env,
        axis2_woden_documentation_element_t *documentation)
{
    axis2_woden_documentable_impl_t *documentable_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, documentation, AXIS2_FAILURE);
    documentable_impl = INTF_TO_IMPL(documentable);
   
    if(!documentable_impl->f_doc_elems)
    {
        documentable_impl->f_doc_elems = axis2_array_list_create(env, 0);
        if(!documentable_impl->f_doc_elems)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(documentable_impl->f_doc_elems, env, 
            documentation);
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_documentable_get_documentation_elements(
        void *documentable,
        axis2_env_t **env)
{
    axis2_woden_documentable_impl_t *documentable_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentable_impl = INTF_TO_IMPL(documentable);

    return documentable_impl->f_doc_elems;
}

