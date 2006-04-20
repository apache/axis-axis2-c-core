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

#include <xml_schema/axis2_xml_schema_form.h>

typedef struct axis2_xml_schema_form_impl axis2_xml_schema_form_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_form_impl
{
    axis2_xml_schema_form_t form;
    axis2_xml_schema_enum_t *schema_enum;
    axis2_hash_t *methods;
    axis2_array_list_t *members;
};

#define INTF_TO_IMPL(form) ((axis2_xml_schema_form_impl_t *) form)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_form_free(void *form,
                        axis2_env_t **env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_form_get_base_impl(void *form,
                                axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_form_resolve_methods(
                                axis2_xml_schema_form_t *form,
                                axis2_env_t **env,
                                axis2_xml_schema_form_t *form_impl,
                                axis2_hash_t *methods);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_form_get_values(void *form,
                        axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_schema_form_t *)
axis2_xml_schema_form_create(axis2_env_t **env,
                            axis2_char_t *value)
{
    axis2_xml_schema_form_impl_t *form_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    form_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_form_impl_t));

    form_impl->schema_enum = NULL;
    form_impl->methods = NULL;
    form_impl->members = NULL;
    form_impl->form.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_form_ops_t));

    form_impl->form.ops->free = axis2_xml_schema_form_free;
    form_impl->form.ops->get_base_impl = axis2_xml_schema_form_get_base_impl;
    form_impl->form.ops->get_values = 
            axis2_xml_schema_form_get_values;
   
    if(!form_impl->members)
    {
        form_impl->members = axis2_array_list_create(env, 0);
        if(!form_impl->members)
            return NULL;
        AXIS2_ARRAY_LIST_ADD(form_impl->members, env, AXIS2_XML_SCHEMA_NONE);
        AXIS2_ARRAY_LIST_ADD(form_impl->members, env, AXIS2_XML_SCHEMA_QUALIFIED);
        AXIS2_ARRAY_LIST_ADD(form_impl->members, env, AXIS2_XML_SCHEMA_UNQUALIFIED);
    }
    form_impl->methods = axis2_hash_make(env);
    if(!form_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(form_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_form_free);
    axis2_hash_set(form_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_form_get_values);

    form_impl->schema_enum = axis2_xml_schema_enum_create(env, value);
    status = axis2_xml_schema_enum_resolve_methods(
            &(form_impl->form.base), env, form_impl->schema_enum, 
            form_impl->methods); 

    return &(form_impl->form);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_form_free(void *form,
                                    axis2_env_t **env)
{
    axis2_xml_schema_form_impl_t *form_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    form_impl = INTF_TO_IMPL(form);

    if(form_impl->members)
    {
        AXIS2_ARRAY_LIST_FREE(form_impl->members, env);
        form_impl->members = NULL;
    }
    
    if(form_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(form_impl->schema_enum, env);
        form_impl->schema_enum = NULL;
    }
    
    if((&(form_impl->form))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(form_impl->form))->ops);
        (&(form_impl->form))->ops = NULL;
    }

    if(form_impl)
    {
        AXIS2_FREE((*env)->allocator, form_impl);
        form_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_form_get_base_impl(void *form,
                                axis2_env_t **env)
{
    axis2_xml_schema_form_impl_t *form_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    form_impl = INTF_TO_IMPL(form);

    return form_impl->schema_enum;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_form_resolve_methods(
                                axis2_xml_schema_form_t *form,
                                axis2_env_t **env,
                                axis2_xml_schema_form_t *form_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_form_impl_t *form_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, form_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    form_impl_l = (axis2_xml_schema_form_impl_t *) form_impl;
    
    form->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_form_ops_t));
    form->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    form->ops->get_base_impl = 
            form_impl_l->form.ops->get_base_impl;
    form->ops->get_values = 
            form_impl_l->form.ops->get_values;
    
    return axis2_xml_schema_enum_resolve_methods(&(form->base), 
            env, form_impl_l->schema_enum, methods);
}

axis2_array_list_t* AXIS2_CALL
axis2_xml_schema_form_get_values(void *form,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return INTF_TO_IMPL(form)->members;
}

