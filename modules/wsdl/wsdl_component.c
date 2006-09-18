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

#include <axis2_wsdl_component.h>
#include <axis2_property.h>

/**
 * @brief Wsdl Component struct impl
 *   Axis2 Wsdl Component impl
 */
typedef struct axis2_wsdl_component_impl
{
    axis2_wsdl_component_t wsdl_component;
    axis2_hash_t *component_properties;
    /**
     * Field Namespace Qualified elements that can be sticked in the component.
     */
    axis2_linked_list_t *elements;

    /**
     * Field Namespace Qualified attrebutes that can be sticked int eh
     * component.
     */
    axis2_linked_list_t *attributes;

}
axis2_wsdl_component_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_component) \
        ((axis2_wsdl_component_impl_t *)(wsdl_component))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_component_free(axis2_wsdl_component_t *wsdl_component,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_properties(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env,
    axis2_hash_t *properties);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_component_get_component_properties(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_property(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env,
    const axis2_char_t *key,
    void *value);

void * AXIS2_CALL
axis2_wsdl_component_get_component_property(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env,
    const axis2_char_t *key);



axis2_status_t AXIS2_CALL
axis2_wsdl_component_add_extensibility_element(axis2_wsdl_component_t *wsdl_component,
        const axis2_env_t *env,
        void *element);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_component_get_extensibility_elements(axis2_wsdl_component_t *wsdl_component,
        const axis2_env_t *env);

/***************************** End of function headers ************************/

axis2_wsdl_component_t * AXIS2_CALL
axis2_wsdl_component_create(const axis2_env_t *env)
{
    axis2_wsdl_component_impl_t *wsdl_component_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    wsdl_component_impl = (axis2_wsdl_component_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_wsdl_component_impl_t));

    if (NULL == wsdl_component_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_component_impl->component_properties = NULL;
    wsdl_component_impl->elements = NULL;
    wsdl_component_impl->attributes = NULL;

    wsdl_component_impl->component_properties = axis2_hash_make(env);
    if (NULL == wsdl_component_impl->component_properties)
    {
        axis2_wsdl_component_free(&(wsdl_component_impl->wsdl_component), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_component_impl->wsdl_component.ops = (axis2_wsdl_component_ops_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_component_ops_t));

    if (NULL == wsdl_component_impl->wsdl_component.ops)
    {
        axis2_wsdl_component_free(&(wsdl_component_impl->wsdl_component), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_component_impl->wsdl_component.ops->free = axis2_wsdl_component_free;
    wsdl_component_impl->wsdl_component.ops->set_component_properties =
        axis2_wsdl_component_set_component_properties;
    wsdl_component_impl->wsdl_component.ops->get_component_properties =
        axis2_wsdl_component_get_component_properties;
    wsdl_component_impl->wsdl_component.ops->set_component_property =
        axis2_wsdl_component_set_component_property;
    wsdl_component_impl->wsdl_component.ops->get_component_property =
        axis2_wsdl_component_get_component_property;
    wsdl_component_impl->wsdl_component.ops->add_extensibility_element =
        axis2_wsdl_component_add_extensibility_element;
    wsdl_component_impl->wsdl_component.ops->get_extensibility_elements =
        axis2_wsdl_component_get_extensibility_elements;

    return &(wsdl_component_impl->wsdl_component);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_component_free(axis2_wsdl_component_t *wsdl_component,
        const axis2_env_t *env)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);

    if (NULL != wsdl_component->ops)
    {
        AXIS2_FREE(env->allocator, wsdl_component->ops);
        wsdl_component->ops = NULL;
    }

    if (NULL != component_impl->component_properties)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        const void *key = NULL;
        for (hi = axis2_hash_first(component_impl->component_properties, env);
                hi; hi = axis2_hash_next(env, hi))
        {
            axis2_property_t *property = NULL;

            axis2_hash_this(hi, &key, NULL, &val);
            property = (axis2_property_t *) val;

            if (property)
            {
                AXIS2_PROPERTY_FREE(property, env);
                property = NULL;
            }

            val = NULL;
            key = NULL;
        }
        axis2_hash_free(component_impl->component_properties, env);
        component_impl->component_properties = NULL;
    }

    if (NULL != component_impl->attributes)
    {
        AXIS2_LINKED_LIST_FREE(component_impl->attributes, env);
        component_impl->attributes = NULL;
    }

    if (NULL != component_impl->elements)
    {
        AXIS2_LINKED_LIST_FREE(component_impl->elements, env);
        component_impl->elements = NULL;
    }

    AXIS2_FREE(env->allocator, component_impl);
    component_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_properties(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env,
    axis2_hash_t *properties)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, properties, AXIS2_FAILURE);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);

    if (component_impl->component_properties)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        const void *key = NULL;
        for (hi = axis2_hash_first(component_impl->component_properties, env);
                hi; hi = axis2_hash_next(env, hi))
        {
            axis2_property_t *property;

            axis2_hash_this(hi, &key, NULL, &val);
            property = (axis2_property_t *) val;
            if (property)
            {
                AXIS2_PROPERTY_FREE(property, env);
                property = NULL;
            }
            val = NULL;
            key = NULL;
        }
        axis2_hash_free(component_impl->component_properties, env);
        component_impl->component_properties = NULL;
    }
    component_impl->component_properties = properties;
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_component_get_component_properties(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_component)->component_properties;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_property(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env,
    const axis2_char_t *key,
    void *value)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);

    axis2_hash_set(component_impl->component_properties, key,
            AXIS2_HASH_KEY_STRING, value);

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_wsdl_component_get_component_property(
    axis2_wsdl_component_t *wsdl_component,
    const axis2_env_t *env,
    const axis2_char_t *key)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, key, NULL);

    return (void *)(axis2_hash_get(AXIS2_INTF_TO_IMPL(wsdl_component)->
            component_properties, key, AXIS2_HASH_KEY_STRING));
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_add_extensibility_element(axis2_wsdl_component_t *wsdl_component,
        const axis2_env_t *env,
        void *element)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);

    if (NULL == component_impl->elements)
    {
        component_impl->elements = axis2_linked_list_create(env);
        if (!component_impl->elements)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_LINKED_LIST_ADD(component_impl->elements, env, element);
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_component_get_extensibility_elements(axis2_wsdl_component_t *wsdl_component,
        const axis2_env_t *env)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);

    return component_impl->elements;
}

/**
 * Returns a <code>List</code> of ExtensibleAttributes of this component.
 *
 * @return <code>List</code>
 */
/*axis2_array_list_t *AXIS2_CALL
axis2_wsdl_component_get_extensibility_attributes(axis2_wsdl_component_t *wsdl_component,
                                                    const axis2_env_t *env)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);

    if (NULL == component_impl->attributes)
    {
        component_impl->attributes = axis2_linked_list_create(env);
    }
    return component_impl->attributes;
}
*/
