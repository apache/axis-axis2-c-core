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

#include <axis2_wsdl_extensible_component.h>

/**
 * @brief Wsdl extensible component struct impl
 *   Wsdl extensible component
 */
typedef struct axis2_wsdl_extensible_component_impl
{
    axis2_wsdl_extensible_component_t extensible_component;

    /**
     * Field features
     */
    axis2_linked_list_t *features;

    /**
     * Field properties
     */
    axis2_linked_list_t *properties;

}
axis2_wsdl_extensible_component_impl_t;

#define AXIS2_INTF_TO_IMPL(extensible_component) \
      ((axis2_wsdl_extensible_component_impl_t *)extensible_component)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_free(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_feature(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env,
    axis2_wsdl_feature_t *wsdl_feature);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_features(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_property(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env,
    axis2_wsdl_property_t *wsdl_property);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_properties(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env);

/************************** End of function prototypes ************************/

axis2_wsdl_extensible_component_t * AXIS2_CALL
axis2_wsdl_extensible_component_create(const axis2_env_t *env)
{
    axis2_wsdl_extensible_component_impl_t *extensible_component_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    extensible_component_impl = (axis2_wsdl_extensible_component_impl_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_extensible_component_impl_t));


    if (NULL == extensible_component_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    extensible_component_impl->extensible_component.ops = NULL;
    extensible_component_impl->properties = NULL;
    extensible_component_impl->features = NULL;
    extensible_component_impl->extensible_component.wsdl_component = NULL;

    extensible_component_impl->features = axis2_linked_list_create(env);
    if (NULL == extensible_component_impl->features)
    {
        axis2_wsdl_extensible_component_free(&(extensible_component_impl->
                extensible_component), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    extensible_component_impl->properties = axis2_linked_list_create(env);
    if (NULL == extensible_component_impl->properties)
    {
        axis2_wsdl_extensible_component_free(&(extensible_component_impl->
                extensible_component), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    extensible_component_impl->extensible_component.wsdl_component =
        axis2_wsdl_component_create(env);
    if (NULL == extensible_component_impl->extensible_component.wsdl_component)
    {
        axis2_wsdl_extensible_component_free(&(extensible_component_impl->
                extensible_component), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    extensible_component_impl->extensible_component.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_extensible_component_ops_t));
    if (NULL == extensible_component_impl->extensible_component.ops)
    {
        axis2_wsdl_extensible_component_free(&(extensible_component_impl->
                extensible_component), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    extensible_component_impl->extensible_component.ops->free =
        axis2_wsdl_extensible_component_free;
    extensible_component_impl->extensible_component.ops->add_feature =
        axis2_wsdl_extensible_component_add_feature;
    extensible_component_impl->extensible_component.ops->get_features =
        axis2_wsdl_extensible_component_get_features;
    extensible_component_impl->extensible_component.ops->add_property =
        axis2_wsdl_extensible_component_add_property;
    extensible_component_impl->extensible_component.ops->get_properties =
        axis2_wsdl_extensible_component_get_properties;


    return &(extensible_component_impl->extensible_component);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_free(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env)
{
    axis2_wsdl_extensible_component_impl_t *extensible_component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    extensible_component_impl = AXIS2_INTF_TO_IMPL(extensible_component);

    if (extensible_component->ops)
    {
        AXIS2_FREE(env->allocator, extensible_component->ops);
        extensible_component->ops = NULL;
    }

    if (extensible_component_impl->features)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(extensible_component_impl->features, env); i++)
        {
            axis2_wsdl_feature_t *feature = NULL;
            feature = AXIS2_LINKED_LIST_GET(extensible_component_impl->features, env, i);

            feature = (axis2_wsdl_feature_t *) val;
            if (feature)
                AXIS2_WSDL_FEATURE_FREE(feature, env);

            val = NULL;
            feature = NULL;

        }
        AXIS2_LINKED_LIST_FREE(extensible_component_impl->features, env);
        extensible_component_impl->features = NULL;
    }

    if (extensible_component_impl->properties)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(extensible_component_impl->properties, env); i++)
        {
            axis2_wsdl_property_t *property = NULL;
            property = AXIS2_LINKED_LIST_GET(extensible_component_impl->properties, env, i);

            property = (axis2_wsdl_property_t *) val;
            if (property)
                AXIS2_WSDL_PROPERTY_FREE(property, env);

            val = NULL;
            property = NULL;

        }
        AXIS2_LINKED_LIST_FREE(extensible_component_impl->properties, env);
        extensible_component_impl->properties = NULL;
    }

    if (extensible_component->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(extensible_component->wsdl_component, env);
        extensible_component->wsdl_component = NULL;
    }

    if (extensible_component_impl)
        AXIS2_FREE(env->allocator, extensible_component_impl);
    extensible_component_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_feature(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env,
    axis2_wsdl_feature_t *wsdl_feature)
{
    axis2_wsdl_extensible_component_impl_t *extensible_component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, wsdl_feature, AXIS2_FAILURE);

    extensible_component_impl = AXIS2_INTF_TO_IMPL(extensible_component);

    if (NULL == extensible_component_impl->features)
    {
        extensible_component_impl->features = axis2_linked_list_create(env);
        if (!extensible_component_impl->features)
            return AXIS2_FAILURE;
    }

    return AXIS2_LINKED_LIST_ADD(extensible_component_impl->features, env, wsdl_feature);
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_features(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(extensible_component)->features;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_property(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env,
    axis2_wsdl_property_t *wsdl_property)
{
    axis2_wsdl_extensible_component_impl_t *extensible_component_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, wsdl_property, AXIS2_FAILURE);

    extensible_component_impl = AXIS2_INTF_TO_IMPL(extensible_component);

    if (NULL == extensible_component_impl->properties)
    {
        extensible_component_impl->properties = axis2_linked_list_create(env);
        if (!extensible_component_impl->properties)
            return AXIS2_FAILURE;
    }

    return  AXIS2_LINKED_LIST_ADD(extensible_component_impl->features, env, wsdl_property);
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_properties(
    axis2_wsdl_extensible_component_t *extensible_component,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(extensible_component)->properties;
}
