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

#include <axis2_core_utils.h>
#include <axis2_string.h>
#include <axis2_conf.h>
#include <axis2_relates_to.h>
#include <axis2_addr.h>
#include <axis2_http_transport.h>
#include <axis2_uuid_gen.h>
#include <axis2_property.h>
#include <axis2_conf_ctx.h>

AXIS2_EXTERN axis2_msg_ctx_t * AXIS2_CALL
axis2_core_utils_create_out_msg_ctx(
    const axis2_env_t *env,
    axis2_msg_ctx_t *in_msg_ctx)
{
    axis2_ctx_t *ctx = NULL;
    axis2_msg_ctx_t *new_msg_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_transport_in_desc_t *transport_in = NULL;
    axis2_transport_out_desc_t *transport_out = NULL;
    axis2_msg_info_headers_t *old_msg_info_headers = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *to = NULL;
    const axis2_char_t *msg_id = NULL;
    axis2_relates_to_t *relates_to = NULL;
    const axis2_char_t *action = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_bool_t doing_rest = AXIS2_FALSE;
    axis2_bool_t doing_mtom = AXIS2_FALSE;
    axis2_bool_t server_side = AXIS2_FALSE;
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
    /*axis2_property_t *property = NULL;*/
    axis2_char_t *msg_uuid = NULL;

    AXIS2_PARAM_CHECK(env->error, in_msg_ctx, NULL);

    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(in_msg_ctx, env);
    transport_in = AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(in_msg_ctx, env);
    transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(in_msg_ctx, env);

    new_msg_ctx = axis2_msg_ctx_create(env, conf_ctx, transport_in, transport_out);
    if (!new_msg_ctx)
    {
        return NULL;
    }
    old_msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(in_msg_ctx, env);
    if (!old_msg_info_headers)
    {
        return NULL;
    }
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(new_msg_ctx, env);
    if (!msg_info_headers)
    {
        /* if there is no msg info header in ew msg ctx, then create one */
        msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
        if (!msg_info_headers)
            return NULL;
        AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(new_msg_ctx, env, msg_info_headers);
    }

    msg_uuid =  axis2_uuid_gen(env);
    AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, msg_uuid);
    if (msg_uuid)
    {
        AXIS2_FREE(env->allocator, msg_uuid);
        msg_uuid = NULL;
    }
    reply_to = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, reply_to);
    /*if (reply_to)
    {
        axis2_endpoint_ref_t *epr = NULL;
        axis2_char_t *address = NULL;

        address = AXIS2_ENDPOINT_REF_GET_ADDRESS (endpoint_ref, env);
        if (address)
        {
            epr = axis2_endpoint_ref_create (env, address);
            AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, epr);
        }
    }*/


    fault_to = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, fault_to);

    to = AXIS2_MSG_INFO_HEADERS_GET_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, to);

    msg_id = AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(old_msg_info_headers, env);
    relates_to = axis2_relates_to_create(env, msg_id,
            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE);
    AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to);

    action = AXIS2_MSG_INFO_HEADERS_GET_ACTION(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, action);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_OP_CTX(new_msg_ctx, env, op_ctx);

    svc_ctx = AXIS2_MSG_CTX_GET_SVC_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SVC_CTX(new_msg_ctx, env, svc_ctx);

    ctx = AXIS2_MSG_CTX_GET_BASE(in_msg_ctx, env);
    if (ctx)
    {
        axis2_ctx_t *new_ctx = AXIS2_MSG_CTX_GET_BASE(new_msg_ctx, env);
        if (new_ctx)
        {
            axis2_ctx_set_non_persistent_map(new_ctx, env,
                axis2_ctx_get_non_persistent_map(ctx, env));
        }
    }

    /*property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env,
            AXIS2_TRANSPORT_OUT, AXIS2_FALSE);
    if (property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_TRANSPORT_OUT, property,
                AXIS2_FALSE);
        property = NULL;
    }


    property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env,
            AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_FALSE);
    if (property)
    {
        axis2_char_t *temp = AXIS2_PROPERTY_GET_VALUE(property, env);
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO,
            property, AXIS2_FALSE);
        property = NULL;
    }*/

    /* Setting the charater set encoding */

    /*property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env,
            AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
    if (property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env,
                AXIS2_CHARACTER_SET_ENCODING, property, AXIS2_FALSE);
        property = NULL;
    }

    property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env,
            AXIS2_WSA_VERSION, AXIS2_FALSE);
    if (property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env,
                AXIS2_WSA_VERSION, property, AXIS2_FALSE);
        property = NULL;
    }*/

    doing_rest = AXIS2_MSG_CTX_GET_DOING_REST(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_DOING_REST(new_msg_ctx, env, doing_rest);

    doing_mtom = AXIS2_MSG_CTX_GET_DOING_MTOM(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_DOING_MTOM(new_msg_ctx, env, doing_mtom);

    server_side = AXIS2_MSG_CTX_GET_SERVER_SIDE(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SERVER_SIDE(new_msg_ctx, env, server_side);

    svc_grp_ctx = AXIS2_MSG_CTX_GET_SVC_GRP_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SVC_GRP_CTX(new_msg_ctx, env, svc_grp_ctx);

    AXIS2_MSG_CTX_SET_IS_SOAP_11(new_msg_ctx, env,
            AXIS2_MSG_CTX_GET_IS_SOAP_11(in_msg_ctx, env));
    AXIS2_MSG_CTX_SET_KEEP_ALIVE(new_msg_ctx, env,
            AXIS2_MSG_CTX_IS_KEEP_ALIVE(in_msg_ctx, env));

    axis2_msg_ctx_set_charset_encoding(new_msg_ctx, env, 
        axis2_msg_ctx_get_charset_encoding(in_msg_ctx, env));

    return new_msg_ctx;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_core_utils_reset_out_msg_ctx(const axis2_env_t *env,
        axis2_msg_ctx_t *out_msg_ctx)
{
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    if (!out_msg_ctx)
        return;

    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(out_msg_ctx, env);
    if (msg_info_headers)
    {
        AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, NULL);
        AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, NULL);
        AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, NULL);
        AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, NULL);
    }

    AXIS2_MSG_CTX_SET_OP_CTX(out_msg_ctx, env, NULL);
    AXIS2_MSG_CTX_SET_SVC_CTX(out_msg_ctx, env, NULL);
    /*AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_TRANSPORT_OUT, NULL,
            AXIS2_FALSE);
    AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO,
            NULL, AXIS2_FALSE);
    AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING,
            NULL, AXIS2_FALSE);
    AXIS2_MSG_CTX_SET_PROPERTY(out_msg_ctx, env, AXIS2_WSA_VERSION, NULL,
            AXIS2_FALSE);*/

    AXIS2_MSG_CTX_SET_SVC_GRP_CTX(out_msg_ctx, env, NULL);

    return;
}

AXIS2_EXTERN axis2_qname_t* AXIS2_CALL
axis2_core_utils_get_module_qname(const axis2_env_t *env,
        const axis2_char_t *name,
        const axis2_char_t *version)
{
    axis2_qname_t *ret_qname = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    if (version  && 0 != AXIS2_STRLEN(version))
    {
        axis2_char_t * mod_name1 = NULL;
        axis2_char_t * mod_name = NULL;
        mod_name1 = AXIS2_STRACAT(name, "-", env);
        if (NULL == mod_name1)
        {
            return NULL;
        }
        mod_name = AXIS2_STRACAT(mod_name1, version, env);
        if (NULL == mod_name)
        {
            AXIS2_FREE(env->allocator, mod_name1);
            mod_name1 = NULL;
            return NULL;
        }
        ret_qname = axis2_qname_create(env, mod_name, NULL, NULL);
        AXIS2_FREE(env->allocator, mod_name);
        AXIS2_FREE(env->allocator, mod_name1);
        return ret_qname;
    }
    ret_qname = axis2_qname_create(env, name, NULL, NULL);
    return ret_qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_core_utils_calculate_default_module_version(const axis2_env_t *env,
        axis2_hash_t *modules_map, axis2_conf_t *axis_conf)
{
    axis2_hash_t *default_modules = NULL;
    axis2_hash_index_t *hi = NULL;
    void *val = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, modules_map, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, axis_conf, AXIS2_FAILURE);

    default_modules = axis2_hash_make(env);
    if (NULL == default_modules)
    {
        return AXIS2_FAILURE;
    }
    for (hi = axis2_hash_first(modules_map, env);  hi;
            hi = axis2_hash_next(env, hi))
    {
        axis2_module_desc_t *mod_desc = NULL;

        axis2_hash_this(hi, NULL, NULL, &val);
        mod_desc = (axis2_module_desc_t *) val;
        if (mod_desc)
        {
            const axis2_qname_t *module_qname = NULL;
            module_qname = AXIS2_MODULE_DESC_GET_QNAME(mod_desc, env);
            if (module_qname)
            {
                axis2_char_t *mod_name_with_ver = NULL;
                mod_name_with_ver = AXIS2_QNAME_GET_LOCALPART(module_qname, env);
                if (mod_name_with_ver)
                {
                    axis2_char_t *module_name_str = NULL;
                    axis2_char_t *module_ver_str = NULL;
                    axis2_char_t *current_def_ver = NULL;

                    module_name_str = axis2_core_utils_get_module_name(env,
                            mod_name_with_ver);
                    if (NULL == module_name_str)
                    {
                        return AXIS2_FAILURE;
                    }
                    module_ver_str = axis2_core_utils_get_module_version(env,
                            mod_name_with_ver);
                    current_def_ver = axis2_hash_get(default_modules,
                            module_name_str, AXIS2_HASH_KEY_STRING);
                    if (current_def_ver)
                    {
                        if (module_ver_str && AXIS2_TRUE ==
                                axis2_core_utils_is_latest_mod_ver(env,
                                        module_ver_str, current_def_ver))
                        {
                            axis2_hash_set(default_modules, module_name_str,
                                    AXIS2_HASH_KEY_STRING, module_ver_str);
                        }
                        else
                        {
                            if (module_name_str)
                            {
                                AXIS2_FREE(env->allocator, module_name_str);
                                module_name_str = NULL;
                            }
                            if (module_ver_str)
                            {
                                AXIS2_FREE(env->allocator, module_ver_str);
                                module_ver_str = NULL;
                            }
                        }
                    }
                    else
                    {
                        axis2_hash_set(default_modules, module_name_str,
                                AXIS2_HASH_KEY_STRING, module_ver_str);
                    }

                    if  (module_name_str)
                    {
                        AXIS2_FREE(env->allocator, module_name_str);
                        module_name_str = NULL;
                    }
                }
            }
        }
        val = NULL;
    }

    hi = NULL;
    val = NULL;
    for (hi = axis2_hash_first(default_modules, env);  hi;
            hi = axis2_hash_next(env, hi))
    {
        void *key_string = NULL;
        axis2_hash_this(hi, (const void **)&key_string, NULL, &val);
        if (key_string && NULL != val)
        {
            AXIS2_CONF_ADD_DEFAULT_MODULE_VERSION(axis_conf, env,
                    (axis2_char_t *)key_string, (axis2_char_t *)val);
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Added default module"
                    " version : %s for module : %s",
                    (axis2_char_t *)val, (axis2_char_t *)key_string);
        }
    }

    if (default_modules)
    {
        axis2_hash_free(default_modules, env);
        default_modules = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_core_utils_get_module_name(const axis2_env_t *env, axis2_char_t *module_name)
{
    axis2_char_t version_seperator = '-';
    axis2_char_t *name = NULL;
    axis2_char_t *version_sep_loc = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, module_name, NULL);

    name = AXIS2_STRDUP(module_name, env);
    if (NULL == name)
    {
        return NULL;
    }
    version_sep_loc = AXIS2_RINDEX(name, version_seperator);
    if (version_sep_loc)
    {
        *version_sep_loc = '\0';
    }
    return name;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_core_utils_get_module_version(const axis2_env_t *env, axis2_char_t *module_name)
{
    axis2_char_t version_seperator = '-';
    axis2_char_t *version_sep_loc = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, module_name, NULL);

    version_sep_loc = AXIS2_RINDEX(module_name, version_seperator);
    if (version_sep_loc)
    {
        return AXIS2_STRDUP(version_sep_loc + sizeof(axis2_char_t), env);
    }
    return NULL;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_core_utils_is_latest_mod_ver(const axis2_env_t *env,
        axis2_char_t *module_ver, axis2_char_t *current_def_ver)
{
    double cur_ver = 0.0;
    double mod_ver = 0.0;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, module_ver, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, current_def_ver, AXIS2_FALSE);

    cur_ver = atof(current_def_ver);
    mod_ver = atof(module_ver);

    if (mod_ver > cur_ver)
    {
        return AXIS2_TRUE;
    }
    return AXIS2_FAILURE;
}
