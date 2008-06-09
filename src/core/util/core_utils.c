
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
#include <axutil_string.h>
#include <axis2_conf.h>
#include <axis2_relates_to.h>
#include <axis2_addr.h>
#include <axis2_http_transport.h>
#include <axutil_uuid_gen.h>
#include <axutil_property.h>
#include <axis2_conf_ctx.h>

AXIS2_EXTERN axis2_msg_ctx_t *AXIS2_CALL
axis2_core_utils_create_out_msg_ctx(
    const axutil_env_t * env,
    axis2_msg_ctx_t * in_msg_ctx)
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
    axis2_char_t *msg_uuid = NULL;
    axutil_stream_t *out_stream = NULL;
    axutil_param_t *expose_headers_param = NULL;
    axis2_bool_t expose_headers = AXIS2_FALSE;

    AXIS2_PARAM_CHECK(env->error, in_msg_ctx, NULL);

    conf_ctx = axis2_msg_ctx_get_conf_ctx(in_msg_ctx, env);
    transport_in = axis2_msg_ctx_get_transport_in_desc(in_msg_ctx, env);
    transport_out = axis2_msg_ctx_get_transport_out_desc(in_msg_ctx, env);

    new_msg_ctx =
        axis2_msg_ctx_create(env, conf_ctx, transport_in, transport_out);
    if (!new_msg_ctx)
    {
        return NULL;
    }

    if (transport_in)
    {
        expose_headers_param =
            axutil_param_container_get_param(
                 axis2_transport_in_desc_param_container(transport_in, env), env,
                 AXIS2_EXPOSE_HEADERS);
    }
    if (expose_headers_param)
    {
        axis2_char_t *expose_headers_value = NULL;
        expose_headers_value = axutil_param_get_value(expose_headers_param, env);
        if (expose_headers_value && 0 == axutil_strcasecmp (expose_headers_value, AXIS2_VALUE_TRUE))
        {
            expose_headers = AXIS2_TRUE;
        }
    }
    if (expose_headers)
    {
        axis2_msg_ctx_set_transport_headers(new_msg_ctx, env,
                axis2_msg_ctx_extract_transport_headers(in_msg_ctx, env));
    }
    axis2_msg_ctx_set_http_accept_record_list(new_msg_ctx, env,
        axis2_msg_ctx_extract_http_accept_record_list(in_msg_ctx, env));
    axis2_msg_ctx_set_http_accept_charset_record_list(new_msg_ctx, env,
        axis2_msg_ctx_extract_http_accept_charset_record_list(in_msg_ctx, env));
    axis2_msg_ctx_set_http_accept_language_record_list(new_msg_ctx, env,
        axis2_msg_ctx_extract_http_accept_language_record_list(in_msg_ctx, env));

    old_msg_info_headers = axis2_msg_ctx_get_msg_info_headers(in_msg_ctx, env);
    if (!old_msg_info_headers)
    {
        return NULL;
    }
    msg_info_headers = axis2_msg_ctx_get_msg_info_headers(new_msg_ctx, env);
    if (!msg_info_headers)
    {
        /* if there is no msg info header in ew msg ctx, then create one */
        msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
        if (!msg_info_headers)
            return NULL;
        axis2_msg_ctx_set_msg_info_headers(new_msg_ctx, env, msg_info_headers);
    }

    msg_uuid = axutil_uuid_gen(env);
    axis2_msg_info_headers_set_message_id(msg_info_headers, env, msg_uuid);
    if (msg_uuid)
    {
        AXIS2_FREE(env->allocator, msg_uuid);
        msg_uuid = NULL;
    }
    reply_to = axis2_msg_info_headers_get_reply_to(old_msg_info_headers, env);
    axis2_msg_info_headers_set_to(msg_info_headers, env, reply_to);

    fault_to = axis2_msg_info_headers_get_fault_to(old_msg_info_headers, env);
    axis2_msg_info_headers_set_fault_to(msg_info_headers, env, fault_to);

    to = axis2_msg_info_headers_get_to(old_msg_info_headers, env);
    axis2_msg_info_headers_set_from(msg_info_headers, env, to);

    msg_id = axis2_msg_info_headers_get_message_id(old_msg_info_headers, env);
    relates_to = axis2_relates_to_create(env, msg_id,
                                         AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE);
    axis2_msg_info_headers_set_relates_to(msg_info_headers, env, relates_to);

    action = axis2_msg_info_headers_get_action(old_msg_info_headers, env);
    axis2_msg_info_headers_set_action(msg_info_headers, env, action);

    op_ctx = axis2_msg_ctx_get_op_ctx(in_msg_ctx, env);
    axis2_msg_ctx_set_op_ctx(new_msg_ctx, env, op_ctx);

    svc_ctx = axis2_msg_ctx_get_svc_ctx(in_msg_ctx, env);
    axis2_msg_ctx_set_svc_ctx(new_msg_ctx, env, svc_ctx);

    ctx = axis2_msg_ctx_get_base(in_msg_ctx, env);
    if (ctx)
    {
        axis2_ctx_t *new_ctx = axis2_msg_ctx_get_base(new_msg_ctx, env);
        if (new_ctx)
        {
            axis2_ctx_set_property_map(new_ctx, env,
                                       axis2_ctx_get_property_map(ctx, env));
        }
    }

    out_stream = axis2_msg_ctx_get_transport_out_stream(in_msg_ctx, env);
    axis2_msg_ctx_set_transport_out_stream(new_msg_ctx, env, out_stream);
    axis2_msg_ctx_set_out_transport_info(new_msg_ctx, env,
         axis2_msg_ctx_get_out_transport_info
         (in_msg_ctx, env));

    /* Setting the charater set encoding */
    doing_rest = axis2_msg_ctx_get_doing_rest(in_msg_ctx, env);
    axis2_msg_ctx_set_doing_rest(new_msg_ctx, env, doing_rest);

    doing_mtom = axis2_msg_ctx_get_doing_mtom(in_msg_ctx, env);
    axis2_msg_ctx_set_doing_mtom(new_msg_ctx, env, doing_mtom);

    server_side = axis2_msg_ctx_get_server_side(in_msg_ctx, env);
    axis2_msg_ctx_set_server_side(new_msg_ctx, env, server_side);

    svc_grp_ctx = axis2_msg_ctx_get_svc_grp_ctx(in_msg_ctx, env);
    axis2_msg_ctx_set_svc_grp_ctx(new_msg_ctx, env, svc_grp_ctx);

    axis2_msg_ctx_set_is_soap_11(new_msg_ctx, env,
                                 axis2_msg_ctx_get_is_soap_11(in_msg_ctx, env));
    axis2_msg_ctx_set_keep_alive(new_msg_ctx, env,
                                 axis2_msg_ctx_is_keep_alive(in_msg_ctx, env));

    axis2_msg_ctx_set_charset_encoding(new_msg_ctx, env,
                                       axis2_msg_ctx_get_charset_encoding
                                       (in_msg_ctx, env));

    return new_msg_ctx;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_core_utils_reset_out_msg_ctx(
    const axutil_env_t * env,
    axis2_msg_ctx_t * out_msg_ctx)
{
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    if (!out_msg_ctx)
        return;

    msg_info_headers = axis2_msg_ctx_get_msg_info_headers(out_msg_ctx, env);
    if (msg_info_headers)
    {
        axis2_msg_info_headers_set_to(msg_info_headers, env, NULL);
        axis2_msg_info_headers_set_fault_to(msg_info_headers, env, NULL);
        axis2_msg_info_headers_set_from(msg_info_headers, env, NULL);
        axis2_msg_info_headers_set_reply_to(msg_info_headers, env, NULL);
    }

    axis2_msg_ctx_set_op_ctx(out_msg_ctx, env, NULL);
    axis2_msg_ctx_set_svc_ctx(out_msg_ctx, env, NULL);
    axis2_msg_ctx_reset_transport_out_stream(out_msg_ctx, env);
    axis2_msg_ctx_reset_out_transport_info(out_msg_ctx, env);
    axis2_msg_ctx_set_svc_grp_ctx(out_msg_ctx, env, NULL);

    return;
}

AXIS2_EXTERN axutil_qname_t *AXIS2_CALL
axis2_core_utils_get_module_qname(
    const axutil_env_t * env,
    const axis2_char_t * name,
    const axis2_char_t * version)
{
    axutil_qname_t *ret_qname = NULL;
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    if (version && 0 != axutil_strlen(version))
    {
        axis2_char_t *mod_name1 = NULL;
        axis2_char_t *mod_name = NULL;
        mod_name1 = axutil_stracat(env, name, "-");
        if (!mod_name1)
        {
            return NULL;
        }
        mod_name = axutil_stracat(env, mod_name1, version);
        if (!mod_name)
        {
            AXIS2_FREE(env->allocator, mod_name1);
            mod_name1 = NULL;
            return NULL;
        }
        ret_qname = axutil_qname_create(env, mod_name, NULL, NULL);
        AXIS2_FREE(env->allocator, mod_name);
        AXIS2_FREE(env->allocator, mod_name1);
        return ret_qname;
    }
    ret_qname = axutil_qname_create(env, name, NULL, NULL);
    return ret_qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_core_utils_calculate_default_module_version(
    const axutil_env_t * env,
    axutil_hash_t * modules_map,
    axis2_conf_t * axis_conf)
{
    axutil_hash_t *default_modules = NULL;
    axutil_hash_index_t *hi = NULL;
    void *val = NULL;

    AXIS2_PARAM_CHECK(env->error, modules_map, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, axis_conf, AXIS2_FAILURE);

    default_modules = axutil_hash_make(env);
    if (!default_modules)
    {
        return AXIS2_FAILURE;
    }
    for (hi = axutil_hash_first(modules_map, env); hi;
         hi = axutil_hash_next(env, hi))
    {
        axis2_module_desc_t *mod_desc = NULL;

        axutil_hash_this(hi, NULL, NULL, &val);
        mod_desc = (axis2_module_desc_t *) val;
        if (mod_desc)
        {
            const axutil_qname_t *module_qname = NULL;
            module_qname = axis2_module_desc_get_qname(mod_desc, env);
            if (module_qname)
            {
                axis2_char_t *mod_name_with_ver = NULL;
                mod_name_with_ver =
                    axutil_qname_get_localpart(module_qname, env);
                if (mod_name_with_ver)
                {
                    axis2_char_t *module_name_str = NULL;
                    axis2_char_t *module_ver_str = NULL;
                    axis2_char_t *current_def_ver = NULL;

                    module_name_str = axis2_core_utils_get_module_name(env,
                                                                       mod_name_with_ver);
                    if (!module_name_str)
                    {
                        return AXIS2_FAILURE;
                    }
                    module_ver_str = axis2_core_utils_get_module_version(env,
                                                                         mod_name_with_ver);
                    current_def_ver = axutil_hash_get(default_modules,
                                                      module_name_str,
                                                      AXIS2_HASH_KEY_STRING);
                    if (current_def_ver)
                    {
                        if (module_ver_str && AXIS2_TRUE ==
                            axis2_core_utils_is_latest_mod_ver(env,
                                                               module_ver_str,
                                                               current_def_ver))
                        {
                            axutil_hash_set(default_modules, module_name_str,
                                            AXIS2_HASH_KEY_STRING,
                                            module_ver_str);
                        }
                        else
                        {
                            if (module_name_str)
                            {
                                AXIS2_FREE(env->allocator, module_name_str);
                            }
                            if (module_ver_str)
                            {
                                AXIS2_FREE(env->allocator, module_ver_str);
                            }
                        }
                    }
                    else
                    {
                        axutil_hash_set(default_modules, module_name_str,
                                        AXIS2_HASH_KEY_STRING, module_ver_str);
                    }

                    if (module_name_str)
                    {
                        AXIS2_FREE(env->allocator, module_name_str);
                    }
                }
            }
        }
        val = NULL;
    }

    hi = NULL;
    val = NULL;
    for (hi = axutil_hash_first(default_modules, env); hi;
         hi = axutil_hash_next(env, hi))
    {
        void *key_string = NULL;
        axutil_hash_this(hi, (const void **) &key_string, NULL, &val);
        if (key_string && NULL != val)
        {
            axis2_conf_add_default_module_version(axis_conf, env,
                                                  (axis2_char_t *) key_string,
                                                  (axis2_char_t *) val);
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "Added default module"
                            " version : %s for module : %s",
                            (axis2_char_t *) val, (axis2_char_t *) key_string);
        }
    }

    if (default_modules)
    {
        axutil_hash_free(default_modules, env);
        default_modules = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_core_utils_get_module_name(
    const axutil_env_t * env,
    axis2_char_t * module_name)
{
    axis2_char_t version_seperator = '-';
    axis2_char_t *name = NULL;
    axis2_char_t *version_sep_loc = NULL;

    AXIS2_PARAM_CHECK(env->error, module_name, NULL);

    name = axutil_strdup(env, module_name);
    if (!name)
    {
        return NULL;
    }
    version_sep_loc = axutil_rindex(name, version_seperator);
    if (version_sep_loc)
    {
        *version_sep_loc = '\0';
    }
    return name;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_core_utils_get_module_version(
    const axutil_env_t * env,
    axis2_char_t * module_name)
{
    axis2_char_t version_seperator = '-';
    axis2_char_t *version_sep_loc = NULL;

    AXIS2_PARAM_CHECK(env->error, module_name, NULL);

    version_sep_loc = axutil_rindex(module_name, version_seperator);
    if (version_sep_loc)
    {
        return axutil_strdup(env, version_sep_loc + sizeof(axis2_char_t));
    }
    return NULL;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_core_utils_is_latest_mod_ver(
    const axutil_env_t * env,
    axis2_char_t * module_ver,
    axis2_char_t * current_def_ver)
{
    double cur_ver = 0.0;
    double mod_ver = 0.0;
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


axis2_op_t *AXIS2_CALL
axis2_core_utils_get_rest_op_with_method_and_location(
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
			/* We are sure that the difference lies within the int range */
			}
		else if (pass_one)
			{
			pass_one = AXIS2_FALSE;
			}
		else
			{
			int i = 0;
			i = (int)strlen(loc_str_tmp);
			/* We are sure that the difference lies within the int range */
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


