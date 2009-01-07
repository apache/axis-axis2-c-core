
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



/* internal structure to keep the rest map in a multi level hash */
typedef struct {
    /* the structure will keep as many as following fields */

    /* if the mapped value is directly the operation */
    axis2_op_t *op_desc;

    /* if the mapped value is a constant, this keeps a hash map of
    possible constants => corrosponding map_internal structure */
    axutil_hash_t *consts_map;

    /* if the mapped value is a param, this keeps a hash map of
    possible param_values => corrosponding_map_internal structre */
    axutil_hash_t *params_map;

} axutil_core_utils_map_internal_t;

/* some functions to use internally in handling rest map */
/* infer op from the live url */
axis2_op_t *
axis2_core_utils_infer_op_from_parent_rest_map(
	const axutil_env_t *env,
    axutil_hash_t *rest_map,
	axis2_char_t *live_url,
	axutil_array_list_t *param_keys,
	axutil_array_list_t *param_values);

/* build the restmap recursively - internal function*/
axis2_status_t AXIS2_CALL
axis2_core_utils_internal_build_rest_map_recursively(
	const axutil_env_t * env,
	axis2_char_t * url,
    axutil_core_utils_map_internal_t *mapping_struct,
    axis2_op_t *op_desc);

/* infer op from the live url recursively */
axis2_op_t *AXIS2_CALL
axis2_core_utils_internal_infer_op_from_rest_map_recursively(
	const axutil_env_t *env,
    const axutil_core_utils_map_internal_t *parent_mapping_struct,
	axis2_char_t *live_url,
	axutil_array_list_t *param_keys,
	axutil_array_list_t *param_values);

/* match a pattern with a url component */
axis2_status_t
axis2_core_utils_match_url_component_with_pattern(const axutil_env_t *env,
             axis2_char_t *pattern,
             axis2_char_t *url_component, 
             axutil_array_list_t *param_keys, 
             axutil_array_list_t *param_values);

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
   
    /* we can create with default Relates to namespace. 
    Actual namespace based on addressing version will be created in addressing out handler */
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


/* build the rest map - external function */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_core_utils_prepare_rest_mapping (
	const axutil_env_t * env,
	axis2_char_t * url,
    axutil_hash_t *rest_map,
    axis2_op_t *op_desc)
{
    
    
    axis2_char_t *first_delimitter = NULL;
    axis2_char_t *next_level_url = NULL;
    axis2_char_t *mapping_key = NULL;
    axutil_core_utils_map_internal_t *mapping_struct =  NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_char_t *bracket_start = NULL;

    first_delimitter = axutil_strchr(url, '/');
   
    if(first_delimitter)
    {
        /* if there is another recursive level,
           this will get the url of that level */
        next_level_url = first_delimitter + 1;
        *first_delimitter = '\0';
    }

    if((bracket_start = axutil_strchr(url, '{')))
    {
        /* we support multiple param per url component,
           but we validate only one param */
        if(axutil_strchr(bracket_start, '}'))
        {
            /* this is validated */
        }

        else
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Invalid URL Format, no closing bracket in declaring parameters");
            return AXIS2_FAILURE;
        }
    }

    
    /* only constants are allowed in this level, so here url become the mapping_key */
    mapping_key = url;

    if(*mapping_key == '\0') /* empty mapping key */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid URL Format: empty mapping key");
        return AXIS2_FAILURE;
    }


    /* retrieve or create the mapping structure for the key*/
    mapping_struct = axutil_hash_get(rest_map,
                            mapping_key, AXIS2_HASH_KEY_STRING);
    if(!mapping_struct)
    {
        mapping_struct = (axutil_core_utils_map_internal_t*)AXIS2_MALLOC (env->allocator,
                                       sizeof(axutil_core_utils_map_internal_t));
        if(!mapping_struct) {
           AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
           AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "No memory. Cannot create internal rest mapping structure");
           return AXIS2_FAILURE;

        }
        memset(mapping_struct, 0, sizeof(axutil_core_utils_map_internal_t));
        
        mapping_key = axutil_strdup(env, mapping_key);
        axutil_hash_set(rest_map, mapping_key, AXIS2_HASH_KEY_STRING, mapping_struct);
    }

    if(!next_level_url) {
        /* if no next level url, put the op_desc in right this level */
        if(mapping_struct->op_desc) {
           AXIS2_ERROR_SET(env->error, AXIS2_ERROR_DUPLICATE_URL_REST_MAPPING, AXIS2_FAILURE);
           AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Duplicate URL Mapping found");
           return AXIS2_FAILURE;
        }
        mapping_struct->op_desc = op_desc;
    }
    else {
        /* we have to build the map_internal structure recursively */
        status = axis2_core_utils_internal_build_rest_map_recursively(env,
                            next_level_url,
                            mapping_struct,
                            op_desc);
    }

    return status;

}

/* build the restmap recursively - internal function*/
axis2_status_t AXIS2_CALL
axis2_core_utils_internal_build_rest_map_recursively(
	const axutil_env_t * env,
	axis2_char_t * url,
    axutil_core_utils_map_internal_t *parent_mapping_struct,
    axis2_op_t *op_desc) {

 /* Here url is expected to be in the form
    {student}/marks/{subject} or
    marks/{subject} 
    */

    axis2_char_t *first_delimitter = NULL;
    axis2_char_t *next_level_url = NULL;
    axis2_char_t *mapping_key = NULL;
    axutil_core_utils_map_internal_t *mapping_struct =  NULL;
    axutil_hash_t *cur_level_rest_map = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_char_t *bracket_start = NULL;

    axis2_bool_t is_key_a_param = AXIS2_FALSE;

    first_delimitter = axutil_strchr(url, '/');
   
    if(first_delimitter)
    {
        /* if there is another recurisive level,
           this will get the url of that level */
        next_level_url = first_delimitter + 1;
        *first_delimitter = '\0';
    }

    if((bracket_start = axutil_strchr(url, '{')))
    {
        /* we support multiple param per url component,
           but we validate only one param */
        if(axutil_strchr(bracket_start, '}'))
        {
            is_key_a_param = AXIS2_TRUE;
        }

        else
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Invalid URL Format, no closing bracket in declaring parameters");
            return AXIS2_FAILURE;
        }
    }
    
    /* here the url become the mapping_key */
    mapping_key = url;

    if(*mapping_key == '\0') /* empty mappng key */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid URL Format: empty mapping key");
        return AXIS2_FAILURE;
    }

    if(is_key_a_param) {
        /* set the rest map as the params_map */
        if(parent_mapping_struct->params_map == NULL) {
            parent_mapping_struct->params_map = axutil_hash_make(env);
            if(!parent_mapping_struct->params_map) {
               AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
               AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");
               return AXIS2_FAILURE;
            }
        }
        cur_level_rest_map = parent_mapping_struct->params_map;
    }
    else {
        /* set the rest map as the consts_map */
        if(parent_mapping_struct->consts_map == NULL) {
            parent_mapping_struct->consts_map = axutil_hash_make(env);
            if(!parent_mapping_struct->consts_map) {
               AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
               AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");
               return AXIS2_FAILURE;
            }
        }
        cur_level_rest_map = parent_mapping_struct->consts_map;
    }

    /* retrieve or create the maping structure for the key*/
    mapping_struct = axutil_hash_get(cur_level_rest_map,
                            mapping_key, AXIS2_HASH_KEY_STRING);
    if(!mapping_struct) {
        mapping_struct = (axutil_core_utils_map_internal_t*)AXIS2_MALLOC (env->allocator,
                                       sizeof(axutil_core_utils_map_internal_t));
        if(!mapping_struct) {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "No memory. Cannot create internal rest mapping structure");
            return AXIS2_FAILURE;

        }
        memset(mapping_struct, 0, sizeof(axutil_core_utils_map_internal_t));
        
        mapping_key = axutil_strdup(env, mapping_key);
        axutil_hash_set(cur_level_rest_map, mapping_key, AXIS2_HASH_KEY_STRING, mapping_struct);
    }

    if(!next_level_url) {
        /* if no next level url, put the op_desc in right this level */
        if(mapping_struct->op_desc) {
           AXIS2_ERROR_SET(env->error, AXIS2_ERROR_DUPLICATE_URL_REST_MAPPING, AXIS2_FAILURE);
           AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Duplicate URL Mapping found");
           return AXIS2_FAILURE;
        }
        mapping_struct->op_desc = op_desc;
    }
    else {
        /* we have to build the map_internal structure recursively */
        status = axis2_core_utils_internal_build_rest_map_recursively(env,
                            next_level_url,
                            mapping_struct,
                            op_desc);
    }

    return status;
}


/* free the rest map recursively */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_core_utils_free_rest_map (
	const axutil_env_t *env,
    axutil_hash_t *rest_map) {

    axutil_hash_index_t *hi = NULL;
    const void *key = NULL;
    void *val = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    for (hi = axutil_hash_first(rest_map, env); hi;
         hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, &key, NULL, &val);

        if (val)
        {
            axutil_core_utils_map_internal_t *mapping_struct = NULL;
            mapping_struct = (axutil_core_utils_map_internal_t*)val;
            
            /* freeing the consts_map and params_map */
            if(mapping_struct->consts_map) {
                axis2_core_utils_free_rest_map(env, mapping_struct->consts_map);
            }
            
            if(mapping_struct->params_map) {
                axis2_core_utils_free_rest_map(env, mapping_struct->params_map);
            }  
            AXIS2_FREE(env->allocator, mapping_struct);
        }

        if (key)
        {
            AXIS2_FREE(env->allocator, (axis2_char_t *) key);
            key = NULL;
        }
    }
    axutil_hash_free(rest_map, env);
    return status;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_core_utils_get_rest_op_with_method_and_location(axis2_svc_t *svc,
                    const axutil_env_t *env, 
                    const axis2_char_t *method, 
                    const axis2_char_t *location,
                    axutil_array_list_t *param_keys,
                    axutil_array_list_t *param_values) 
{
    axis2_char_t *addition_params_str = NULL;
    axis2_char_t *adjusted_local_url = NULL;

    axis2_char_t *live_mapping_url = NULL;
    axis2_char_t *local_url = NULL;

    axis2_op_t *op = NULL;
    
    int key_len = 0;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Checking for operation using "
                    "REST HTTP Location fragment : %s", location);

    /* we are creating a dup of the location */
    local_url = (axis2_char_t*)axutil_strdup(env, location);
    if(!local_url) {
       AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
       AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "No memory. Cannot create the live rest mapping url");
       return NULL;
    }
    
    /* checking the existence of the addition parameters
       after the question mark '?' */
    addition_params_str = strchr(local_url, '?');
    if(addition_params_str)
    {
        *addition_params_str = '\0';
        addition_params_str ++;
    }
    
    /* if the first character is '/' ignore that */
    if(*local_url == '/')
    {
        adjusted_local_url = local_url + 1;
    }
    else
    {
        adjusted_local_url = local_url;
    }
    
    /* now create the mapping url */
    key_len = axutil_strlen(method) + axutil_strlen(adjusted_local_url) + 2;
    
    live_mapping_url = (axis2_char_t *) (AXIS2_MALLOC (env->allocator,
                                              sizeof (axis2_char_t) * key_len));
    
    if(!live_mapping_url)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "No memory. Cannot create the live rest mapping url");
        AXIS2_FREE(env->allocator, local_url);
        return NULL;
    }

    sprintf(live_mapping_url, "%s:%s", method, adjusted_local_url);


    op = axis2_core_utils_infer_op_from_parent_rest_map(
                env,
                axis2_svc_get_rest_map(svc, env),
                live_mapping_url,
                param_keys,
                param_values);
    
    
    if (op)
    {
        axis2_char_t *params_str;

        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                        "Operation found using target endpoint uri fragment");

        /* here we are going to extract out the additional parameters 
         * put after '?' mark */
        params_str =  addition_params_str;
        while(params_str && *params_str != '\0')
        {
            axis2_char_t *next_params_str = NULL;
            axis2_char_t *key_value_seperator = NULL;

            axis2_char_t *key = NULL;
            axis2_char_t *value= NULL;

            
            /* we take one parameter pair to the params_str */
            next_params_str = strchr(params_str, '&');
            if(next_params_str) 
            {
                *next_params_str = '\0';
            }
            
            key_value_seperator = strchr(params_str, '=');
            if(key_value_seperator) 
            {
                /* devide the key value pair */
                *key_value_seperator = '\0';
                key = params_str;
                value = key_value_seperator + 1;
            }
            else {
                /* there is no '=' symbol, that mean only the key exist */
                key = params_str;
            }
            if(key) {
                key = axutil_strdup(env, key);
                axutil_array_list_add(param_keys, env, key);
            }
            if(value) {
                value = axutil_strdup(env, value);
                axutil_array_list_add(param_values, env, value);
            }

            if(next_params_str)
            {
                /* if there was an '&' character then */
                params_str = next_params_str + 1;
            }
            else {
                params_str = NULL; /* just to end the loop */
            }
        }
    }

    if (live_mapping_url)
    {
        AXIS2_FREE (env->allocator, live_mapping_url);
    }
    if(local_url)
    {
        AXIS2_FREE(env->allocator, local_url);
    }
    return op;
}


/* infer op from the live url */
axis2_op_t *
axis2_core_utils_infer_op_from_parent_rest_map(
	const axutil_env_t *env,
    axutil_hash_t *rest_map,
	axis2_char_t *live_url,
	axutil_array_list_t *param_keys,
	axutil_array_list_t *param_values) 
{
    axis2_char_t *first_delimitter = NULL;
    axis2_char_t *next_level_url = NULL;
    axis2_char_t *url_component = NULL;

    axis2_op_t *op_desc = NULL;
    axutil_core_utils_map_internal_t *mapping_struct =  NULL;

    first_delimitter = axutil_strchr(live_url, '/');
   
    if(first_delimitter)
    {
        /* if there is another recursive level,
           this will get the url of that level */
        next_level_url = first_delimitter + 1;
        *first_delimitter = '\0';
    }

    /* so live url is the url_component */
    url_component = live_url;

    /* check it in the hash map */
    mapping_struct = (axutil_core_utils_map_internal_t*)axutil_hash_get(rest_map,
                            url_component, AXIS2_HASH_KEY_STRING);
    
    if(mapping_struct)
    {
        if(!next_level_url)
        {
            /* if no level exists, find it here */
            op_desc = mapping_struct->op_desc;
        }
        else 
        {

            op_desc = axis2_core_utils_internal_infer_op_from_rest_map_recursively(
                        env,
                        mapping_struct,
                        next_level_url,
                        param_keys,
                        param_values);
        }
    }
    if(!op_desc)
    {
        /* if the url is not mapped to the given constant url 
         * we have to match it with the url pattern */

        axutil_hash_index_t *hi = NULL;
        const void *key = NULL;
        void *val = NULL;
        axis2_status_t matched_status = AXIS2_FAILURE;

        for (hi = axutil_hash_first(rest_map, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, &key, NULL, &val);
            
            if(key == url_component)
            {
                continue; /* skip the already checked key */
            }
            if (key && val)
            {
                axis2_char_t *hash_key = (axis2_char_t*)key;
                axis2_char_t *dup_url_component = NULL;
                axis2_char_t *dup_pattern = NULL;

                /* temporary param keys and values for each entry */
                axutil_array_list_t *tmp_param_keys = NULL;
                axutil_array_list_t *tmp_param_values = NULL;


                tmp_param_keys = axutil_array_list_create(env, 10);
                if(!tmp_param_keys)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Cannot create internal rest mapping structure");

                    return NULL;
                }
                tmp_param_values = axutil_array_list_create(env, 10);
                if(!tmp_param_values)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Cannot create internal rest mapping structure");
                    axutil_array_list_free(tmp_param_keys, env);
                    return NULL;
                }

                dup_url_component = axutil_strdup(env, url_component);
                if(!dup_url_component)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Cannot create internal rest mapping structure");
                    axutil_array_list_free(tmp_param_keys, env);
                    axutil_array_list_free(tmp_param_values, env);
                    return NULL;
                }
                dup_pattern = axutil_strdup(env, hash_key);
                if(!dup_pattern)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Cannot create internal rest mapping structure");
                    axutil_array_list_free(tmp_param_keys, env);
                    axutil_array_list_free(tmp_param_values, env);
                    AXIS2_FREE(env->allocator, dup_url_component);
                    return NULL;
                }

                matched_status = axis2_core_utils_match_url_component_with_pattern(env,
                                 dup_pattern, dup_url_component, tmp_param_keys, tmp_param_values);

                AXIS2_FREE(env->allocator, dup_url_component);
                AXIS2_FREE(env->allocator, dup_pattern);

                if(matched_status == AXIS2_SUCCESS && val)
                {
                    mapping_struct = (axutil_core_utils_map_internal_t*)val;

                    if(!next_level_url)
                    {
                        op_desc = mapping_struct->op_desc;
                    }
                    else
                    {

                        op_desc = axis2_core_utils_internal_infer_op_from_rest_map_recursively(
                                    env,
                                    mapping_struct,
                                    next_level_url,
                                    tmp_param_keys,
                                    tmp_param_values);
                    }
                    if(op_desc)
                    {
                        /* we are done, the url is matched with a pattern */
                        /* but before leaving should merge the param arrays */

                        int i = 0;
                        void *param_key = NULL;
                        void *param_value = NULL;

                        for(i = 0; i < axutil_array_list_size(tmp_param_keys, env); i ++)
                        {
                            /* size(tmp_param_keys) == size(tmp_param_values) */
                            param_key = axutil_array_list_get(tmp_param_keys, env, i);
                            param_value = axutil_array_list_get(tmp_param_values, env, i);

                            /* add it to original array */
                            axutil_array_list_add(param_keys, env, param_key);
                            axutil_array_list_add(param_values, env, param_value);

                        }
                        /* since of is found, no more searches needed */
                        break;
                    }
                }
                /* freeing the temporary arrays */
                axutil_array_list_free(tmp_param_keys, env);
                axutil_array_list_free(tmp_param_values, env);
            }
        }
    }

    if(!op_desc)
    {
        /* no more to look up */
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "REST maping structure is NULL for the accessed URL");
        return NULL;
    }

    return op_desc;
}

/* infer op from the live url recursively */
axis2_op_t *AXIS2_CALL
axis2_core_utils_internal_infer_op_from_rest_map_recursively(
	const axutil_env_t *env,
    const axutil_core_utils_map_internal_t *parent_mapping_struct,
	axis2_char_t *live_url,
	axutil_array_list_t *param_keys,
	axutil_array_list_t *param_values)
{

    axis2_char_t *first_delimitter = NULL;
    axis2_char_t *next_level_url = NULL;
    axis2_char_t *url_component = NULL;

    axis2_op_t *op_desc = NULL;
    axutil_core_utils_map_internal_t *child_mapping_struct = NULL;
   
    axutil_hash_index_t *hi = NULL;
    const void *key = NULL;
    void *val = NULL;

    
    first_delimitter = axutil_strchr(live_url, '/');
   
    if(first_delimitter)
    {
        /* if there is another recurisive level,
           this will get the url of that level */
        next_level_url = first_delimitter + 1;
        *first_delimitter = '\0';
    }

    /* so live url is the url_component */
    url_component = live_url;
    
    /* first check the url component in the constants array list */
    if(parent_mapping_struct->consts_map) 
    {
        child_mapping_struct = axutil_hash_get(parent_mapping_struct->consts_map,
                            url_component, AXIS2_HASH_KEY_STRING);
    }
    
    /* if the url component exists in the consts_map, go through it inside */

    if(child_mapping_struct)
    {
        if(!next_level_url)
        {
            /* there is no another level, so the op should be here */

            op_desc = child_mapping_struct->op_desc;
            
            if(!op_desc) {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                    "The operation descriptor not found constant given in the url");
            }
            /* rather than returning NULL we continue to search params_map */
            
        }
        else {

            op_desc = axis2_core_utils_internal_infer_op_from_rest_map_recursively(
                        env,
                        child_mapping_struct,
                        next_level_url,
                        param_keys,
                        param_values);
        }
    }

    if(op_desc)
    {
        /* if the op for the accessed url found, no further searching is needed */
        return op_desc;
    }

    /* if it is not found in the consts_map we have to assume it is in a params_map */

    if(!parent_mapping_struct->params_map)
    {
        /* wrong operation, abort to continue to let calling function to check other operations */
        if(first_delimitter)
        {
            /* restore the delimmiters */
            *first_delimitter = '/';
        }
        return NULL;
    }

    for (hi = axutil_hash_first(parent_mapping_struct->params_map, env); hi;
         hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, &key, NULL, &val);

        if (key && val)
        {
            int i = 0;
            axis2_char_t *hash_key = (axis2_char_t*)key;
            axis2_status_t matched_status = AXIS2_SUCCESS;
            axis2_char_t *dup_url_component = NULL;
            axis2_char_t *dup_pattern = NULL;

            /* temporary param keys and values for each entry */
            axutil_array_list_t *tmp_param_keys;
            axutil_array_list_t *tmp_param_values;


            tmp_param_keys = axutil_array_list_create(env, 10);
            if(!tmp_param_keys)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");

                return NULL;
            }
            tmp_param_values = axutil_array_list_create(env, 10);
            if(!tmp_param_values)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");
                axutil_array_list_free(tmp_param_keys, env);
                return NULL;
            }

            dup_url_component = axutil_strdup(env, url_component);
            if(!dup_url_component)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");
                axutil_array_list_free(tmp_param_keys, env);
                axutil_array_list_free(tmp_param_values, env);
                return NULL;
            }
            dup_pattern = axutil_strdup(env, hash_key);
            if(!dup_pattern)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");
                axutil_array_list_free(tmp_param_keys, env);
                axutil_array_list_free(tmp_param_values, env);
                AXIS2_FREE(env->allocator, dup_url_component);
                return NULL;
            }

            matched_status = axis2_core_utils_match_url_component_with_pattern(env,
                             dup_pattern, dup_url_component, tmp_param_keys, tmp_param_values);

            AXIS2_FREE(env->allocator, dup_url_component);
            AXIS2_FREE(env->allocator, dup_pattern);

            if(matched_status == AXIS2_SUCCESS)
            {
                child_mapping_struct = (axutil_core_utils_map_internal_t*)val;

                if(!next_level_url)
                {
                    /* there is no another level, so the op should be here */
                    op_desc = child_mapping_struct->op_desc;
                    
                }
                else {
                    
                    /* if there is next level, we should check that level too */
                    op_desc = axis2_core_utils_internal_infer_op_from_rest_map_recursively(
                                env,
                                child_mapping_struct,
                                next_level_url,
                                tmp_param_keys,
                                tmp_param_values);
                }

                if(op_desc) {
                    /* the operation is found */
                    /* but before leaving should merge the param arrays */

                    int i = 0;
                    void *param_key = NULL;
                    void *param_value = NULL;
                    for(i = 0; i < axutil_array_list_size(tmp_param_keys, env); i ++)
                    {
                        /* size(tmp_param_keys) == size(tmp_param_values) */
                        param_key = axutil_array_list_get(tmp_param_keys, env, i);
                        param_value = axutil_array_list_get(tmp_param_values, env, i);

                        /* add it to original array */
                        axutil_array_list_add(param_keys, env, param_key);
                        axutil_array_list_add(param_values, env, param_value);

                    }
                    
                    /* freeing the temporary arrays */
                    axutil_array_list_free(tmp_param_keys, env);
                    axutil_array_list_free(tmp_param_values, env);
                    /* since of is found, no more searches needed */
                    break;
                }
            }

            /* if we come here => op is not yet found */
            /* just freeing the temp key and value arrays */
            for(i = 0; i < axutil_array_list_size(tmp_param_keys, env); i ++) {
                void *value = axutil_array_list_get(tmp_param_keys, env, i);
                if(value) {
                    AXIS2_FREE(env->allocator, value);
                }
            }
            for(i = 0; i < axutil_array_list_size(tmp_param_values, env); i ++) {
                void *value = axutil_array_list_get(tmp_param_values, env, i);
                if(value) {
                    AXIS2_FREE(env->allocator, value);
                }
            }
            axutil_array_list_free(tmp_param_keys, env);
            axutil_array_list_free(tmp_param_values, env);
        }
    }

    if(!op_desc) {
        /* this is not an error, since the calling function
            may find another opertion match with the url */
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                    "The operation descriptor not found for the accessed URL");

        if(first_delimitter)
        {
            /* restore the delimmiters */
            *first_delimitter = '/';
        }
    }
    return op_desc;
}


/* match a pattern with a url component */
axis2_status_t
axis2_core_utils_match_url_component_with_pattern(const axutil_env_t *env,
             axis2_char_t *pattern,
             axis2_char_t *url_component, 
             axutil_array_list_t *param_keys, 
             axutil_array_list_t *param_values)
{
    axutil_array_list_t *const_components = NULL;
    axis2_char_t *c = NULL;
    axis2_char_t *url_c = NULL;
    axis2_char_t *pattern_c = NULL;
    axis2_char_t *const_part = NULL;
    axis2_char_t *param_part = NULL;
    axis2_char_t *param_value = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    /* here the state can have following values
       0 - inside a constant
       1 - inside a param 
     */
    int loop_state = 0;
    int i = 0;
    int pattern_ending_with_param = 0;

    /* the constant that undergoing matching */
    int matching_constant_index = 0;
    axis2_char_t *matching_constant = NULL;


    /* dividing the pattern to consts */
    const_components = axutil_array_list_create(env, 10);
    if(!const_components)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "No memory. Cannot create internal rest mapping structure");
    }
    /* check whether the pattern ending with a param */
    if(*(pattern + axutil_strlen(pattern) -1) == '}')
    {
        pattern_ending_with_param = 1;
    }

    const_part = pattern;
    /* a parse to fil the const array and key array */
    for(c = pattern; c && *c != '\0'; c ++)
    {
        if(loop_state == 0) 
        {
            /* inside a constant */
            if(*c == '{')
            {
                if(const_part == c) 
                {
                    /* no const part */
                }
                else 
                {
                    /* add the constant */
                    *c = '\0';
                    const_part = axutil_strdup(env, const_part);
                    if(!const_part)
                    {
                        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "No memory. Cannot create internal rest mapping structure");

                        status = AXIS2_FAILURE;
                        break;

                    }
                    axutil_array_list_add(const_components, env, const_part);

                }
                param_part = c + 1; /* start the param */
                loop_state = 1; /* moving to the param from next iteration */
            }
            else if(*c == '}')
            {
                /* invalid state */
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing the url for %s", url_component);

                status = AXIS2_FAILURE;
                break;
            }
        }
        else
        {
            /* inside a param */
            if(*c == '}')
            {
                if(*(c + 1) == '{') /* you can not have two params without a constant in between */
                {
                    /* invalid state */
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Error in parsing the url for %s, Please put constant between 2 parameters",
                        url_component);

                    status = AXIS2_FAILURE;
                    break;
                }
                if(param_part == c) 
                {
                    /* no param part */
                }
                else
                {
                    /* add the param */
                    *c = '\0';
                    param_part = axutil_strdup(env, param_part);

                    if(param_part == NULL)
                    {
                        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "No memory. Cannot create internal rest mapping structure");
                        status = AXIS2_FAILURE;
                        break;
                    }
                    axutil_array_list_add(param_keys, env, param_part);

                    const_part = c + 1; /* start the const */
                }
                loop_state = 0; /* moving to the const from next iteration */
            }
            else if(*c == '{')
            {
                /* invalid state */
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing the url for %s", url_component);

                status = AXIS2_FAILURE;
                break;

            }
        }
    }
    /* loop should stop in state 0 */
    if(loop_state != 0)
    {
        status = AXIS2_FAILURE;
    }

    if(const_part != c) {
        /* add the tailing const */
        const_part = axutil_strdup(env, const_part);
        if(!const_part) {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "No memory. Cannot create internal rest mapping structure");
            status = AXIS2_FAILURE;
        }
        axutil_array_list_add(const_components, env, const_part);
    }

    if(axutil_array_list_size(const_components, env) == 0 && status == AXIS2_SUCCESS) {
        /* no constants mean, the url componenent itself is the value */
        url_component = axutil_strdup(env, url_component);
        if(url_component)
        {
            axutil_array_list_add(param_values, env, url_component);

            /* free the empty const array */
            
            axutil_array_list_free(const_components, env);
            return AXIS2_SUCCESS;
        }
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "No memory. Cannot create internal rest mapping structure");
        status = AXIS2_FAILURE;
       
    }
    
    if(status == AXIS2_FAILURE)
    {
        /* invalid state */
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error in parsing the url for %s", url_component);

        /* free the const array */
        for(i = 0; i < axutil_array_list_size(const_components, env); i ++)
        {
            void *value;
            value = axutil_array_list_get(const_components, env, i);

            AXIS2_FREE(env->allocator, value);
        }

        axutil_array_list_free(const_components, env);
       
        return status;
    }

    /* we are tracking the loop_state here too - this is useful only to track start*/
    /* we are using the param_value part to track the matching param value */
    if(*pattern != '{') {
        /* starting_with_constant  */
        loop_state = 0;
        param_value = NULL;
    }
    else {
        /* starting_with_param  */
        loop_state = 1;
        param_value = url_component;
        
    }

    matching_constant_index = 0;
    matching_constant = axutil_array_list_get(const_components, env, 0);

    /* now parse the url component */
    for(url_c = url_component;
        *url_c != '\0' && status == AXIS2_SUCCESS && matching_constant != NULL; url_c ++ )
    {
        axis2_char_t *tmp_url_c = url_c;
        pattern_c = matching_constant;
        
        while(*tmp_url_c == *pattern_c && *tmp_url_c != '\0' && *pattern_c != '\0')
        {
            tmp_url_c ++;
            pattern_c ++;
        }

        if(*pattern_c == '\0')
        {
            /* we finised matching the constant pattern successfuly*/
            if(loop_state == 0) 
            {
                /* loop_state => we expected there is a constant */
            }
            else {
                /* we expected a param, but the constant is found => 
                   url_c should mark the end of the param */
                if(param_value == NULL) {
                    /* unexpected invalid state */
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_URL_FORMAT, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Error in parsing the url for %s", url_component);
                    status = AXIS2_FAILURE;
                }
                *url_c = '\0';
                param_value = axutil_strdup(env, param_value);

                if(param_value == NULL)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "No memory. Cannot create internal rest mapping structure");
                    status = AXIS2_FAILURE;
                    break;
                }
                axutil_array_list_add(param_values, env, param_value);

            }
            /* next the param part is starting */
            param_value = tmp_url_c; 

            loop_state = 1; /* the end of the constant expects, start of a variable */

            /* so we found one constant, go for the other */
            matching_constant_index ++;
            matching_constant = axutil_array_list_get(const_components,
                    env, matching_constant_index);

            tmp_url_c --;
            /* increment the url_c to tmp_url_c */
            url_c = tmp_url_c;
        }
        else
        {
            /* pattern not matched */
            if(loop_state == 0)
            {
                /* we are expected this to be a constant, but it has not happend
                 * mean: the pattern match failed 
                 */
                status = AXIS2_FAILURE;
                break;
            }
        }
       
    }

    if(matching_constant_index !=  axutil_array_list_size(const_components, env))
    {
        status = AXIS2_FAILURE;
    }

    if(pattern_ending_with_param)
    {
        if(param_value)
        {
            param_value = axutil_strdup(env, param_value);

            if(param_value == NULL)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Cannot create internal rest mapping structure");
                status = AXIS2_FAILURE;
            }
            else {
                axutil_array_list_add(param_values, env, param_value);
            }
        }
    }
    else if(*url_c != '\0')
    {
        /* here the pattern ending is a constant (not a param), and matches all are already made
         * but some url part left => this is a not mach */
        status = AXIS2_FAILURE;
    }

    /* finally freeing the const array */
    for(i = 0; i < axutil_array_list_size(const_components, env); i ++)
    {
        void *value;
        value = axutil_array_list_get(const_components, env, i);

        AXIS2_FREE(env->allocator, value);
    }
    axutil_array_list_free(const_components, env);

    return status;
}
