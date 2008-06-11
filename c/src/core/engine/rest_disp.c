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
    int param_count = 0;
    axis2_char_t ***params = NULL;
    int i = 0;

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
                        location += strlen(url_tokens[0]);
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "Checking for operation using \
REST HTTP Location fragment : %s", location);
                        op = axis2_core_utils_get_rest_op_with_method_and_location(svc, env,
                                 axis2_msg_ctx_get_rest_http_method(msg_ctx, env), location,
                                 &param_count, &params);
                        if (op)
                        {
                            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                            "Operation found using target endpoint uri fragment");
                        }
                        else
                        {
                            int i = 0;
                            int j = 0;
                            axutil_array_list_t *supported_rest_methods = NULL;
                            const axis2_char_t *http_method = NULL;
                            axis2_char_t *rest_methods[] = {AXIS2_HTTP_GET, AXIS2_HTTP_POST,
                                AXIS2_HTTP_PUT, AXIS2_HTTP_DELETE, AXIS2_HTTP_HEAD};
                            supported_rest_methods = axutil_array_list_create(env, 0);
                            http_method = axis2_msg_ctx_get_rest_http_method(msg_ctx, env);
                            if (!http_method)
                            {
                                AXIS2_LOG_WARNING (env->log, AXIS2_LOG_SI,
                                                 "unable to find http method \
for location: %s", location);
                                return NULL;
                            }

                            for (i = 0; i < 5; i++)
                            {
                                if (axutil_strcasecmp(rest_methods[i], 
                                                      http_method))
                                {
                                    if (axis2_core_utils_get_rest_op_with_method_and_location(svc, env,
                                        rest_methods[i], location, &param_count, &params))
                                    {
                                        for (j = 0; j < param_count; j++)
                                        {
                                            AXIS2_FREE (env->allocator, params[j][0]);
                                            AXIS2_FREE (env->allocator, params[j][1]);
                                            AXIS2_FREE (env->allocator, params[j]);
                                        }
                                        if (params)
                                        {
                                            AXIS2_FREE (env->allocator, params);
                                        }
                                        axutil_array_list_add(supported_rest_methods, env,
                                                              axutil_strdup(env, rest_methods[i]));
                                    }
                                }
                            }
                            axis2_msg_ctx_set_supported_rest_http_methods(msg_ctx, env, supported_rest_methods);
                        }
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
    for (i = 0; i < param_count; i++)
    {
        axiom_node_t *node = NULL;
        axiom_element_t *element = NULL;

        element = axiom_element_create(env, NULL, params[i][0],
                                       NULL, &node);
        axiom_element_set_text(element, env, params[i][1], node);
        axiom_node_add_child(body_child_node, env, node);
        AXIS2_FREE (env->allocator, params[i][0]);
        AXIS2_FREE (env->allocator, params[i][1]);
        AXIS2_FREE (env->allocator, params[i]);
    }
    if (params)
    {
        AXIS2_FREE (env->allocator, params);
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

/*
axis2_op_t *AXIS2_CALL
axis2_rest_disp_get_rest_op_with_method_and_location(
    const axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * method,
    const axis2_char_t * location,
    int * param_count,
    axis2_char_t **** params)
{
    axutil_array_list_t *op_list = NULL;
    axis2_char_t *loc_str = NULL;
    axis2_char_t *loc_str_tmp = NULL;
    axis2_char_t *rindex = NULL;
    axis2_bool_t pass_one = AXIS2_TRUE;
    axis2_bool_t loop_state = AXIS2_TRUE;
    AXIS2_PARAM_CHECK(env->error, location, NULL);
    AXIS2_PARAM_CHECK(env->error, method, NULL);
    
    loc_str = axutil_strtrim(env, location, NULL);
    if (!loc_str)
    {
        return NULL;
    }
    loc_str_tmp = loc_str;
    if (loc_str_tmp[0] == '/')
    {
        loc_str_tmp++;
    }
    if (strchr(loc_str_tmp, '?'))
    {
        axis2_char_t *temp = NULL;

        temp = strchr(loc_str_tmp, '?');
        temp[0] = '\0';
    }
    while(loop_state)
    {
        rindex = axutil_rindex(loc_str_tmp, '/');

        if (rindex && *rindex)
        {
            loc_str_tmp = axutil_string_substring_ending_at(loc_str_tmp, (int)(rindex - loc_str_tmp));
        }
        else if (pass_one)
        {
            pass_one = AXIS2_FALSE;
        }
        else
        {
            int i = 0;
            i = (int)strlen(loc_str_tmp);
            if (i == 0)
            {
                break;
            }
            loc_str_tmp[i - 1] = '\0';
        }

        if (!loc_str_tmp || !*loc_str_tmp)
        {
            break;
        }
        op_list = axis2_svc_get_rest_op_list_with_method_and_location(svc, env,
                                                                      method, loc_str_tmp);
        if (op_list && axutil_array_list_size(op_list, env) != 0)
        {
            int i = 0;
            int size = 0;

            size = axutil_array_list_size(op_list, env);
            for (i = 0; i < size; i++)
            {
                axis2_op_t *op_temp = NULL;
                axis2_char_t *op_location = NULL;
                int match_count = 0;
                axis2_char_t ***matches = NULL;
                axis2_status_t status = AXIS2_FAILURE;

                op_temp = axutil_array_list_get(op_list, env, i);
                op_location = axis2_op_get_rest_http_location(op_temp, env);
                if (!op_location)
                {
                    continue;
                }
                status = axutil_parse_rest_url_for_params(env, op_location, location,
                                                          &match_count, &matches);
                if (status == AXIS2_SUCCESS)
                {
                    *params = matches;
                    *param_count = match_count;
                    AXIS2_FREE (env->allocator, loc_str);
                    return op_temp;
                }
                else if (matches)
                {
                    for (i = 0; i < match_count; i++)
                    {
                        AXIS2_FREE (env->allocator, matches[i]);
                    }
                    AXIS2_FREE (env->allocator, matches);
                }
            }
        }
    }
    AXIS2_FREE (env->allocator, loc_str);
    return NULL;
}

*/

