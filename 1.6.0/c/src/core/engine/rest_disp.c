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

#include <axis2_disp.h>
#include <axis2_handler_desc.h>
#include <axutil_string.h>
#include <axis2_relates_to.h>
#include <axis2_svc.h>
#include <axis2_const.h>
#include <axis2_conf_ctx.h>
#include <axis2_addr.h>
#include <axutil_utils.h>
#include <axiom_soap_builder.h>
#include <axiom_soap_body.h>
#include <axiom_soap_const.h>
#include <axis2_http_transport.h>
#include <axis2_core_utils.h>


const axis2_char_t *AXIS2_REST_DISP_NAME = "rest_dispatcher";

axis2_status_t AXIS2_CALL axis2_rest_disp_invoke(
    axis2_handler_t * handler,
    const axutil_env_t * env,
    struct axis2_msg_ctx *msg_ctx);

axis2_svc_t *AXIS2_CALL axis2_rest_disp_find_svc(
    axis2_msg_ctx_t * msg_ctx,
    const axutil_env_t * env);

axis2_op_t *AXIS2_CALL axis2_rest_disp_find_op(
    axis2_msg_ctx_t * msg_ctx,
    const axutil_env_t * env,
    axis2_svc_t * svc);

axis2_op_t *AXIS2_CALL
axis2_rest_disp_get_rest_op_with_method_and_location(
    const axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * method,
    const axis2_char_t * location,
    int * param_count,
    axis2_char_t **** params);

AXIS2_EXTERN axis2_disp_t *AXIS2_CALL
axis2_rest_disp_create(
    const axutil_env_t * env)
{
    axis2_disp_t *disp = NULL;
    axis2_handler_t *handler = NULL;
    axutil_string_t *name = NULL;

    name = axutil_string_create_const(env,
                                      (axis2_char_t **) &
                                      AXIS2_REST_DISP_NAME);

    disp = axis2_disp_create(env, name);
    if (!disp)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    handler = axis2_disp_get_base(disp, env);
    if (!handler)
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
        return NULL;
    }

    axis2_handler_set_invoke(handler, env, axis2_rest_disp_invoke);

    axutil_string_free(name, env);

    return disp;
}

axis2_svc_t *AXIS2_CALL
axis2_rest_disp_find_svc(
    axis2_msg_ctx_t * msg_ctx,
    const axutil_env_t * env)
{
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_svc_t *svc = NULL;

    if (!axis2_msg_ctx_get_doing_rest(msg_ctx, env))
        return NULL;

    endpoint_ref = axis2_msg_ctx_get_to(msg_ctx, env);

    if (endpoint_ref)
    {
        const axis2_char_t *address = NULL;

        address = axis2_endpoint_ref_get_address(endpoint_ref, env);
        if (address)
        {
            axis2_char_t **url_tokens = NULL;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "Checking for service using target endpoint address : %s",
                            address);

            url_tokens = axutil_parse_request_url_for_svc_and_op(env, address);

            if (url_tokens)
            {
                if (url_tokens[0])
                {
                    axis2_conf_ctx_t *conf_ctx = NULL;

                    conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
                    if (conf_ctx)
                    {
                        axis2_conf_t *conf = NULL;
                        conf = axis2_conf_ctx_get_conf(conf_ctx, env);
                        if (conf)
                        {
                            svc = axis2_conf_get_svc(conf, env, url_tokens[0]);
                            if (svc)
                                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                                "Service found using target endpoint address");
                        }
                    }
                    AXIS2_FREE(env->allocator, url_tokens[0]);

                    if (url_tokens[1])
                    {
                        AXIS2_FREE(env->allocator, url_tokens[1]);
                    }

                }
                AXIS2_FREE(env->allocator, url_tokens);
                url_tokens = NULL;
            }
        }
    }

    return svc;
}

axis2_op_t *AXIS2_CALL
axis2_rest_disp_find_op(
    axis2_msg_ctx_t * msg_ctx,
    const axutil_env_t * env,
    axis2_svc_t * svc)
{
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_op_t *op = NULL;
    axiom_soap_envelope_t *soap_env = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_element_t *body_child = NULL;
    axiom_node_t *body_child_node = NULL;
    axiom_node_t *body_element_node = NULL;
    axis2_bool_t soap_env_exists = AXIS2_TRUE;
    int i = 0;

    axutil_array_list_t *param_keys = NULL;
    axutil_array_list_t *param_values = NULL;
    

    AXIS2_PARAM_CHECK(env->error, svc, NULL);

    if (!axis2_msg_ctx_get_doing_rest(msg_ctx, env))
        return NULL;

    endpoint_ref = axis2_msg_ctx_get_to(msg_ctx, env);

    if (endpoint_ref)
    {
        const axis2_char_t *address = NULL;

        address = axis2_endpoint_ref_get_address(endpoint_ref, env);
        if (address)
        {
            axis2_char_t **url_tokens = NULL;

            url_tokens = axutil_parse_request_url_for_svc_and_op(env, address);

            if (url_tokens)
            {
                if (url_tokens[0])
                {
                    axis2_char_t *location = NULL;

                    location = strstr(address, url_tokens[0]);
                    if (location)
                    {
                        const axis2_char_t *method = NULL;
                        
                        location += strlen(url_tokens[0]);
                        param_keys = axutil_array_list_create(env, 10);
                        if(!param_keys)
                        {
                            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "No memory. Cannot create the live rest parameter maps");
                            return NULL;
                        }
                        param_values = axutil_array_list_create(env, 10);
     
                        if(!param_values)
                        {
                            axutil_array_list_free(param_keys, env);
                            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "No memory. Cannot create the live rest parameter maps");
                            return NULL;
                        }
                        method = axis2_msg_ctx_get_rest_http_method(msg_ctx, env);
                        op = axis2_core_utils_get_rest_op_with_method_and_location(svc,
                                   env, method, location, param_keys, param_values);
                    }
                }
                if (url_tokens[0])
                    AXIS2_FREE(env->allocator, url_tokens[0]);
                if (url_tokens[1])
                    AXIS2_FREE(env->allocator, url_tokens[1]);
                AXIS2_FREE(env->allocator, url_tokens);
            }
        }
    }

    if (!op)
    {
        if(param_keys)
        {
            for (i = 0; i < axutil_array_list_size(param_keys, env); i++)
            {
                void *value = axutil_array_list_get(param_keys, env, i);
                AXIS2_FREE(env->allocator, value);
            }
            axutil_array_list_free(param_keys, env);
        }
        if(param_values)
        {
            for (i = 0; i < axutil_array_list_size(param_values, env); i++)
            {
                void *value = axutil_array_list_get(param_values, env, i);
                AXIS2_FREE(env->allocator, value);
            }
            axutil_array_list_free(param_values, env);
        }
        return NULL;
    }

    soap_env = axis2_msg_ctx_get_soap_envelope(msg_ctx, env);

    if (!soap_env)
    {
        soap_env_exists = AXIS2_FALSE;
        soap_env = axiom_soap_envelope_create_default_soap_envelope(env,
                                                                    AXIOM_SOAP11);
    }
    if (soap_env)
    {
        soap_body = axiom_soap_envelope_get_body(soap_env, env);
    }
    if (!soap_body)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                        AXIS2_FAILURE);

        if(param_keys)
        {
            for (i = 0; i < axutil_array_list_size(param_keys, env); i++)
            {
                void *value = axutil_array_list_get(param_keys, env, i);
                AXIS2_FREE(env->allocator, value);
            }
            axutil_array_list_free(param_keys, env);
        }
        if(param_values)
        {
            for (i = 0; i < axutil_array_list_size(param_values, env); i++)
            {
                void *value = axutil_array_list_get(param_values, env, i);
                AXIS2_FREE(env->allocator, value);
            }
            axutil_array_list_free(param_values, env);
        }
        return NULL;
    }

    body_element_node = axiom_soap_body_get_base_node(soap_body, env);

    if (body_element_node)
    {
        body_child_node = axiom_node_get_first_child(body_element_node, env);
    }

    if (!body_child_node)
    {
        body_child = axiom_element_create_with_qname(env, NULL,
                                                     axis2_op_get_qname(
                                                      op, env),
                                                     &body_child_node);
        axiom_soap_body_add_child(soap_body, env, body_child_node);
    }
   
    if(param_keys && param_values) {
        for (i = 0; i < axutil_array_list_size(param_keys, env); i++)
        {
            axis2_char_t *param_key = NULL;
            axis2_char_t *param_value = NULL;

            axiom_node_t *node = NULL;
            axiom_element_t *element = NULL;

            param_key = axutil_array_list_get(param_keys, env, i);
            param_value = axutil_array_list_get(param_values, env, i);

            element = axiom_element_create(env, NULL, param_key,
                                           NULL, &node);
            axiom_element_set_text(element, env, param_value, node);
            axiom_node_add_child(body_child_node, env, node);

            AXIS2_FREE(env->allocator, param_key);
            AXIS2_FREE(env->allocator, param_value);
        }

        axutil_array_list_free(param_keys, env);
        axutil_array_list_free(param_values, env);
    }

    if (!soap_env_exists)
    {
        axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_env);
    }

    return op;
}

axis2_status_t AXIS2_CALL
axis2_rest_disp_invoke(
    axis2_handler_t * handler,
    const axutil_env_t * env,
    struct axis2_msg_ctx * msg_ctx)
{
    axis2_msg_ctx_set_find_svc(msg_ctx, env, axis2_rest_disp_find_svc);
    axis2_msg_ctx_set_find_op(msg_ctx, env, axis2_rest_disp_find_op);
    return axis2_disp_find_svc_and_op(handler, env, msg_ctx);
}

