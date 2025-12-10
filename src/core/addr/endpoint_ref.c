/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_endpoint_ref.h>
#include <axutil_string.h>
#include <axutil_array_list.h>

struct axis2_endpoint_ref
{

    /** endpoint address */
    axis2_char_t *address;

    /** interface qname */
    axutil_qname_t *interface_qname;

    /** reference parameters */
    axutil_array_list_t *ref_param_list;

    /** meta data */
    axutil_array_list_t *metadata_list;

    /** reference parameter attribute list */
    axutil_array_list_t *ref_attribute_list;

    /** meta data attribute list */
    axutil_array_list_t *meta_attribute_list;

    /** extensible element list */
    axutil_array_list_t *extension_list;

    /** service name */
    axis2_svc_name_t *svc_name;
};

axis2_endpoint_ref_t *AXIS2_CALL
axis2_endpoint_ref_create(
    const axutil_env_t * env,
    const axis2_char_t * address)
{
    axis2_endpoint_ref_t *endpoint_ref = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    endpoint_ref = AXIS2_MALLOC(env->allocator, sizeof(axis2_endpoint_ref_t));
    if(!endpoint_ref)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    endpoint_ref->address = NULL;
    endpoint_ref->interface_qname = NULL;
    endpoint_ref->ref_param_list = NULL;
    endpoint_ref->metadata_list = NULL;
    endpoint_ref->ref_attribute_list = NULL;
    endpoint_ref->meta_attribute_list = NULL;
    endpoint_ref->extension_list = NULL;
    endpoint_ref->svc_name = NULL;

    if(address)
    {
        endpoint_ref->address = axutil_strdup(env, address);
        if(!(endpoint_ref->address))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_endpoint_ref_free(endpoint_ref, env);
            return NULL;
        }
    }

    return endpoint_ref;
}

const axis2_char_t *AXIS2_CALL
axis2_endpoint_ref_get_address(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->address;
}

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_set_address(
    axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env,
    const axis2_char_t * address)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(endpoint_ref->address)
    {
        AXIS2_FREE(env->allocator, endpoint_ref->address);
    }

    endpoint_ref->address = axutil_strdup(env, address);
    return AXIS2_SUCCESS;
}

const axutil_qname_t *AXIS2_CALL
axis2_endpoint_ref_get_interface_qname(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->interface_qname;
}

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_set_interface_qname(
    axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env,
    const axutil_qname_t * interface_qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    endpoint_ref->interface_qname = axutil_qname_clone((axutil_qname_t *)interface_qname, env);
    return AXIS2_SUCCESS;
}

axis2_svc_name_t *AXIS2_CALL
axis2_endpoint_ref_get_svc_name(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->svc_name;
}

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_set_svc_name(
    axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env,
    axis2_svc_name_t * svc_name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    endpoint_ref->svc_name = svc_name;
    return AXIS2_SUCCESS;
}

void AXIS2_CALL
axis2_endpoint_ref_free_void_arg(
    void *endpoint_ref,
    const axutil_env_t * env)
{
    axis2_endpoint_ref_t *endpoint_ref_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_l = (axis2_endpoint_ref_t *)endpoint_ref;
    axis2_endpoint_ref_free(endpoint_ref_l, env);
    return;
}

void AXIS2_CALL
axis2_endpoint_ref_free(
    axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, void);

    if(endpoint_ref->address)
    {
        AXIS2_FREE(env->allocator, endpoint_ref->address);
    }

    if(endpoint_ref->ref_param_list)
    {
        axutil_array_list_free(endpoint_ref->ref_param_list, env);
    }

    if(endpoint_ref->metadata_list)
    {
        axutil_array_list_free(endpoint_ref->metadata_list, env);
    }

    if(endpoint_ref->ref_attribute_list)
    {
        axutil_array_list_free(endpoint_ref->ref_attribute_list, env);
    }

    if(endpoint_ref->meta_attribute_list)
    {
        axutil_array_list_free(endpoint_ref->meta_attribute_list, env);
    }

    if(endpoint_ref->extension_list)
    {
        axutil_array_list_free(endpoint_ref->extension_list, env);
    }

    AXIS2_FREE(env->allocator, endpoint_ref);

    return;
}

axutil_array_list_t *AXIS2_CALL
axis2_endpoint_ref_get_ref_param_list(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->ref_param_list;
}

axutil_array_list_t *AXIS2_CALL
axis2_endpoint_ref_get_metadata_list(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->metadata_list;
}

/* HTTP/2 Pure JSON Architecture - SOAP axiom function removed
 * axis2_endpoint_ref_add_ref_param() removed as part of SOAP elimination.
 * Function used axiom_node_t which is incompatible with pure JSON design.
 */

/* HTTP/2 Pure JSON Architecture - SOAP axiom function removed
 * axis2_endpoint_ref_add_metadata() removed as part of SOAP elimination.
 * Function used axiom_node_t which is incompatible with pure JSON design.
 */

axutil_array_list_t *AXIS2_CALL
axis2_endpoint_ref_get_ref_attribute_list(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->ref_attribute_list;
}

axutil_array_list_t *AXIS2_CALL
axis2_endpoint_ref_get_metadata_attribute_list(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->meta_attribute_list;
}

axutil_array_list_t *AXIS2_CALL
axis2_endpoint_ref_get_extension_list(
    const axis2_endpoint_ref_t * endpoint_ref,
    const axutil_env_t * env)
{
    return endpoint_ref->extension_list;
}

/* HTTP/2 Pure JSON Architecture - SOAP axiom function removed
 * axis2_endpoint_ref_add_ref_attribute() removed as part of SOAP elimination.
 * Function used axiom_attribute_t which is incompatible with pure JSON design.
 */

/* HTTP/2 Pure JSON Architecture - SOAP axiom function removed
 * axis2_endpoint_ref_add_metadata_attribute() removed as part of SOAP elimination.
 * Function used axiom_attribute_t which is incompatible with pure JSON design.
 */

/* HTTP/2 Pure JSON Architecture - SOAP axiom function removed
 * axis2_endpoint_ref_add_extension() removed as part of SOAP elimination.
 * Function used axiom_node_t which is incompatible with pure JSON design.
 */
