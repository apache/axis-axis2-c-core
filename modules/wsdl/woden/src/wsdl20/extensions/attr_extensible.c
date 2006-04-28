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
 
#include <woden/axis2_woden_attr_extensible.h>
#include <woden/axis2_woden_xml_attr.h>
#include <axis2_url.h>
#include <axis2_hash.h>

typedef struct axis2_woden_attr_extensible_impl axis2_woden_attr_extensible_impl_t;

/** 
 * @brief Attribute Extensible Struct Impl
 *	Axis2 Attribute Extensible  
 */ 
struct axis2_woden_attr_extensible_impl
{
    axis2_woden_attr_extensible_t extensible;
    axis2_hash_t *f_ext_attrs;
    axis2_array_list_t *temp_attrs;
};

#define INTF_TO_IMPL(extensible) \
    ((axis2_woden_attr_extensible_impl_t *) extensible)

axis2_status_t AXIS2_CALL 
axis2_woden_attr_extensible_free(void *extensible,
                axis2_env_t **envv);

axis2_status_t AXIS2_CALL 
axis2_woden_attr_extensible_set_ext_attr(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_qname_t *attr_type,
                                    axis2_woden_xml_attr_t *attr); 

axis2_woden_xml_attr_t *AXIS2_CALL 
axis2_woden_attr_extensible_get_ext_attr(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_qname_t *attr_type); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_attr_extensible_get_ext_attrs(void *extensible,
                                                axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_attr_extensible_get_ext_attrs_for_namespace(void *extensible,
                                                        axis2_env_t **env,
                                                        axis2_url_t *namespc);

axis2_bool_t AXIS2_CALL 
axis2_woden_attr_extensible_has_ext_attrs_for_namespace(
                                                       void *extensible,
                                                       axis2_env_t **env,
                                                       axis2_url_t *namespc);



AXIS2_DECLARE(axis2_woden_attr_extensible_t *)
axis2_woden_attr_extensible_create(axis2_env_t **env)
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_attr_extensible_impl_t));

    extensible_impl->f_ext_attrs = NULL;
    extensible_impl->temp_attrs = NULL;

    extensible_impl->extensible.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_attr_extensible_ops_t)); 
    
    extensible_impl->extensible.ops->free = 
        axis2_woden_attr_extensible_free;
    extensible_impl->extensible.ops->set_ext_attr = 
        axis2_woden_attr_extensible_set_ext_attr;
    extensible_impl->extensible.ops->get_ext_attr = 
        axis2_woden_attr_extensible_get_ext_attr;
    extensible_impl->extensible.ops->get_ext_attrs = 
        axis2_woden_attr_extensible_get_ext_attrs;
    extensible_impl->extensible.ops->get_ext_attrs_for_namespace = 
        axis2_woden_attr_extensible_get_ext_attrs_for_namespace;
    extensible_impl->extensible.ops->has_ext_attrs_for_namespace = 
        axis2_woden_attr_extensible_has_ext_attrs_for_namespace;
    
    return &(extensible_impl->extensible);
}

axis2_status_t AXIS2_CALL
axis2_woden_attr_extensible_free(void *extensible,
                axis2_env_t **env)
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    if(extensible_impl->f_ext_attrs)
    {
        axis2_hash_free(extensible_impl->f_ext_attrs, env);
        extensible_impl->f_ext_attrs = NULL;
    }
    
    if(extensible_impl->temp_attrs)
    {
        int size = 0, i = 0;
        size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->temp_attrs, env);
        for(i = 0; i < size; i++)
        {
            void *ext_el = NULL;

            ext_el = AXIS2_ARRAY_LIST_GET(extensible_impl->temp_attrs, env, i);
            AXIS2_WODEN_XML_ATTR_FREE(ext_el, env);
        }
        AXIS2_ARRAY_LIST_FREE(extensible_impl->temp_attrs, env);
        extensible_impl->temp_attrs = NULL;
    }

    if((&(extensible_impl->extensible))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(extensible_impl->extensible))->ops);
        (&(extensible_impl->extensible))->ops = NULL;
    }

    if(extensible_impl)
    {
        AXIS2_FREE((*env)->allocator, extensible_impl);
        extensible_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_attr_extensible_resolve_methods(axis2_woden_attr_extensible_t *extensible,
                                axis2_env_t **env,
                                axis2_woden_attr_extensible_t *extensible_impl,
                                axis2_hash_t *methods)
{
    axis2_woden_attr_extensible_impl_t *extensible_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, extensible_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    extensible_impl_l = (axis2_woden_attr_extensible_impl_t *) extensible_impl;
    
    extensible->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    extensible->ops->free = axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    extensible->ops->set_ext_attr = 
        extensible_impl_l->extensible.ops->set_ext_attr;
    extensible->ops->get_ext_attr = 
        extensible_impl_l->extensible.ops->get_ext_attr;
    extensible->ops->get_ext_attrs = 
        extensible_impl_l->extensible.ops->get_ext_attrs;
    extensible->ops->get_ext_attrs_for_namespace = 
        extensible_impl_l->extensible.ops->get_ext_attrs_for_namespace;
    extensible->ops->has_ext_attrs_for_namespace = 
        extensible_impl_l->extensible.ops->has_ext_attrs_for_namespace;

    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL 
axis2_woden_attr_extensible_set_ext_attr(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_qname_t *attr_type,
                                    axis2_woden_xml_attr_t *attr)
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;
    axis2_char_t *str_attr_type = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_type, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    str_attr_type = AXIS2_QNAME_TO_STRING(attr_type, env);
    if(attr)
        axis2_hash_set(extensible_impl->f_ext_attrs, str_attr_type, 
                AXIS2_HASH_KEY_STRING, attr);
    else
        axis2_hash_set(extensible_impl->f_ext_attrs, str_attr_type, 
                AXIS2_HASH_KEY_STRING, NULL);
    return AXIS2_SUCCESS;
}

axis2_woden_xml_attr_t *AXIS2_CALL 
axis2_woden_attr_extensible_get_ext_attr(void *extensible,
                                            axis2_env_t **env,
                                            axis2_qname_t *attr_type) 
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;
    axis2_char_t *str_attr_type = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_type, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    str_attr_type = AXIS2_QNAME_TO_STRING(attr_type, env);
    return (axis2_woden_xml_attr_t *)axis2_hash_get(extensible_impl->f_ext_attrs, 
            str_attr_type, AXIS2_HASH_KEY_STRING);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_attr_extensible_get_ext_attrs(void *extensible,
                                                axis2_env_t **env) 
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;
    axis2_hash_index_t *index = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
 
    if(extensible_impl->temp_attrs)
    {
        int size = 0, i = 0;
        size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->temp_attrs, env);
        for(i = 0; i < size; i++)
        {
            void *ext_el = NULL;

            ext_el = AXIS2_ARRAY_LIST_GET(extensible_impl->temp_attrs, env, i);
            AXIS2_WODEN_XML_ATTR_FREE(ext_el, env);
        }
        AXIS2_ARRAY_LIST_FREE(extensible_impl->temp_attrs, env);
        extensible_impl->temp_attrs = NULL;
    }
    extensible_impl->temp_attrs = axis2_array_list_create(env, 0);
    for (index = axis2_hash_first (extensible_impl->f_ext_attrs, env); index; 
            index = axis2_hash_next (env, index))
    {
        void *value = NULL;
        
        axis2_hash_this (index, NULL, NULL, &value);
        AXIS2_ARRAY_LIST_ADD(extensible_impl->temp_attrs, env, value);
    }   
    return extensible_impl->temp_attrs;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_attr_extensible_get_ext_attrs_for_namespace(void *extensible,
                                                        axis2_env_t **env,
                                                        axis2_url_t *namespc) 
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;
    axis2_char_t *str_namespc = NULL;
    axis2_hash_index_t *index = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    str_namespc = AXIS2_URL_TO_EXTERNAL_FORM(namespc, env);
    if(extensible_impl->temp_attrs)
    {
        int size = 0, i = 0;
        size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->temp_attrs, env);
        for(i = 0; i < size; i++)
        {
            void *ext_el = NULL;

            ext_el = AXIS2_ARRAY_LIST_GET(extensible_impl->temp_attrs, env, i);
            AXIS2_WODEN_XML_ATTR_FREE(ext_el, env);
        }
        AXIS2_ARRAY_LIST_FREE(extensible_impl->temp_attrs, env);
        extensible_impl->temp_attrs = NULL;
    }

    extensible_impl->temp_attrs = axis2_array_list_create(env, 0);
    for (index = axis2_hash_first (extensible_impl->f_ext_attrs, env); index; 
            index = axis2_hash_next (env, index))
    {
        void *value = NULL;
        
        axis2_hash_this (index, NULL, NULL, &value);
        AXIS2_ARRAY_LIST_ADD(extensible_impl->temp_attrs, env, value);
    }

    return extensible_impl->temp_attrs;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_attr_extensible_has_ext_attrs_for_namespace(void *extensible,
                                                            axis2_env_t **env,
                                                            axis2_url_t *namespc)
{
    axis2_woden_attr_extensible_impl_t *extensible_impl = NULL;
    axis2_bool_t result = AXIS2_FALSE;
    axis2_char_t *str_namespc = NULL;
    axis2_hash_index_t *index = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    str_namespc = AXIS2_URL_TO_EXTERNAL_FORM(namespc, env);
    for (index = axis2_hash_first (extensible_impl->f_ext_attrs, env); index; 
            index = axis2_hash_next (env, index))
    {
        void *v = NULL;
        axis2_qname_t *key = NULL;
        axis2_char_t *str_ns = NULL;
        
        axis2_hash_this (index, &v, NULL, NULL);
        key = (axis2_qname_t *) v;
        str_ns = AXIS2_QNAME_GET_URI(key, env);
        if(0 == AXIS2_STRCMP(str_ns, str_namespc))
        {
            result = AXIS2_TRUE;
            break;
        }
    }
    return result;
}


