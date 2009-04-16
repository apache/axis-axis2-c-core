
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

#include <axis2_svc.h>
#include <axis2_addr.h>
#include <axutil_property.h>
#include <axis2_module.h>
#include "../deployment/axis2_desc_builder.h"
#include <axis2_svc_skeleton.h>
#include <axutil_thread.h>
#include <axis2_core_utils.h>

struct axis2_svc
{
    axis2_svc_grp_t *parent;
    axis2_char_t *axis_svc_name;

    /** To keep last update time of the service */
    long last_update;
    axis2_char_t *filename;

    /** To store module descriptions at deploy time parsing */
    axutil_array_list_t *module_list;

    /** Service description  */
    axis2_char_t *svc_desc;

    /** wsdl file path */
    axis2_char_t *wsdl_path;

    /** service folder path */
    axis2_char_t *folder_path;

    /**
     * WSDL related stuff
     */
    axutil_hash_t *ns_map;
    /* Count of the entries in the namespace map */
    int ns_count;
    /* To keep the XML scheama either from WSDL or
     * C2WSDL(in the future)
     */
    axutil_array_list_t *schema_list;

    /**
     * A table that keeps a mapping of unique XSD names (Strings)
     * against the schema objects. This is populated in the first
     * instance the schemas are asked for and then used to serve
     * the subsequent requests
     */
    axutil_hash_t *schema_mapping_table;

    /**
     * This is where operations are kept
     */
    axutil_hash_t *op_alias_map;

    /**
     * This is where action mappings are kept
     */
    axutil_hash_t *op_action_map;

    /**
     * This is where REST mappings are kept
     */
    axutil_hash_t *op_rest_map;

    /**
     * Keeps track whether the schema locations are adjusted
     */
    axis2_bool_t schema_loc_adjusted;

    /**
     * A custom schema name prefix. if set this will be used to
     * modify the schema names
     */
    axis2_char_t *custom_schema_name_prefix;

    /**
     * A custom schema name suffix. will be attached to the
     * schema file name when the files are uniquely named.
     * A good place to add a file extension if needed
     */
    axis2_char_t *custom_schema_name_suffix;
    /* To store the target namespace for the schema */
    axis2_char_t *schema_target_ns;
    axis2_char_t *schema_target_ns_prefix;
    /* To keep the service target name space */
    axis2_char_t *target_ns;
    axis2_char_t *target_ns_prefix;
    /* Used for schema name calculations */
    int sc_calc_count;

    void *impl_class;
    axutil_qname_t *qname;
    axis2_char_t *style;
    axutil_array_list_t *engaged_module_list;

    /** Parameter container to hold service related parameters */
    struct axutil_param_container *param_container;

    /** Flow container that encapsulates the flow related data */
    struct axis2_flow_container *flow_container;

    /** Base description struct */
    axis2_desc_t *base;

	/* Mutex to avoid loading the same service dll twice */
	axutil_thread_mutex_t *mutex;
};

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create(
    const axutil_env_t * env)
{
    axis2_svc_t *svc = NULL;

    svc = (axis2_svc_t *) AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_t));
    if (!svc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
        return NULL;
    }

    svc->parent = NULL;
    svc->axis_svc_name = NULL;
    svc->filename = NULL;
    svc->svc_desc = NULL;
    svc->wsdl_path = NULL;
    svc->folder_path = NULL;
    svc->last_update = 0;
    svc->param_container = NULL;
    svc->flow_container = NULL;
    svc->op_alias_map = NULL;
    svc->op_action_map = NULL;
    svc->op_rest_map = NULL;
    svc->module_list = NULL;
    svc->ns_map = NULL;
    svc->ns_count = 0;
    svc->schema_list = NULL;
    svc->schema_mapping_table = NULL;
    svc->schema_loc_adjusted = AXIS2_FALSE;
    svc->custom_schema_name_prefix = NULL;
    svc->custom_schema_name_suffix = NULL;
    svc->schema_target_ns = NULL;
    svc->schema_target_ns_prefix = NULL;
    svc->target_ns = NULL;
    svc->target_ns_prefix = NULL;
    svc->sc_calc_count = 0;
    svc->impl_class = NULL;
    svc->qname = NULL;
    svc->style = NULL;
    svc->base = NULL;

    svc->param_container = axutil_param_container_create(env);
    if (!svc->param_container)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service param container creation failed");
        return NULL;
    }

    svc->flow_container = axis2_flow_container_create(env);
    if (!svc->flow_container)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service flow container creation failed");
        return NULL;
    }

    svc->op_alias_map = axutil_hash_make(env);
    if (!svc->op_alias_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service operation alias map creation failed");
        return NULL;
    }

    svc->op_action_map = axutil_hash_make(env);
    if (!svc->op_action_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service operation action map creation failed");
        return NULL;
    }

    svc->op_rest_map = axutil_hash_make(env);
    if (!svc->op_rest_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service operation rest map creation failed");
        return NULL;
    }

    /** Create module list of default size */
    svc->module_list = axutil_array_list_create(env, 0);
    if (!svc->module_list)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service module list creation failed");
        return NULL;
    }

    svc->schema_list = axutil_array_list_create(env,
                                                AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (!svc->schema_list)
    {
        axis2_svc_free(svc, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service schema list creation failed");
        return NULL;
    }

    svc->engaged_module_list = axutil_array_list_create(env,
                                                    AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (!svc->engaged_module_list)
    {
        axis2_svc_free(svc, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service engaged modules list creation failed");
        return NULL;
    }

    svc->schema_loc_adjusted = AXIS2_FALSE;
    if (svc->schema_target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->schema_target_ns_prefix);
        svc->schema_target_ns_prefix = NULL;
    }
    svc->schema_target_ns_prefix = axutil_strdup(env, "ns");

    if (svc->target_ns)
    {
        AXIS2_FREE(env->allocator, svc->target_ns);
        svc->target_ns = NULL;
    }
    svc->target_ns = axutil_strdup(env, "http://ws.apache.org/axis2");

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }
    svc->target_ns_prefix = axutil_strdup(env, "tns");
    svc->sc_calc_count = 0;

    svc->base = axis2_desc_create(env);
    if (!svc->base)
    {
        axis2_svc_free(svc, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Service base creation failed");
        return NULL;
    }
	svc->mutex = axutil_thread_mutex_create(env->allocator, AXIS2_THREAD_MUTEX_DEFAULT);
	if (!svc->mutex)
	{
		axis2_svc_free(svc, env);
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Service mutex creation failed");
        return NULL;
	}
    return svc;
}

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create_with_qname(
    const axutil_env_t * env,
    const axutil_qname_t * qname)
{
    axis2_svc_t *svc = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    svc = axis2_svc_create(env);
    if (!svc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Service creation failed for name %s", 
            axutil_qname_get_localpart(qname, env));
        return NULL;
    }

    status = axis2_svc_set_qname(svc, env, qname);
    if (AXIS2_FAILURE == status)
    {
        axis2_svc_free(svc, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Setting name %s to service failed", 
            axutil_qname_get_localpart(qname, env));
        return NULL;
    }

    return svc;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_svc_free(
    axis2_svc_t * svc,
    const axutil_env_t * env)
{
    if (svc->impl_class)
    {
        AXIS2_SVC_SKELETON_FREE((axis2_svc_skeleton_t *) svc->impl_class, env);
    }
    if (svc->param_container)
    {
        axutil_param_container_free(svc->param_container, env);
    }

    if (svc->flow_container)
    {
        axis2_flow_container_free(svc->flow_container, env);
    }

    if (svc->filename)
    {
        AXIS2_FREE(env->allocator, svc->filename);
        svc->filename = NULL;
    }

    if (svc->svc_desc)
    {
        AXIS2_FREE(env->allocator, svc->svc_desc);
        svc->svc_desc = NULL;
    }

    svc->parent = NULL;

    if (svc->module_list)
    {
        int i = 0;
        int size = 0;

        size = axutil_array_list_size(svc->module_list, env);
        for (i = 0; i < size; i++)
        {
            axutil_qname_t *qname = NULL;
            qname = axutil_array_list_get(svc->module_list, env, i);
            if (qname)
            {
                axutil_qname_free(qname, env);
            }
        }
        axutil_array_list_free(svc->module_list, env);
    }

    if (svc->schema_list)
    {
        axutil_array_list_free(svc->schema_list, env);
    }

    if (svc->engaged_module_list)
    {
        axutil_array_list_free(svc->engaged_module_list, env);
    }

    if (svc->axis_svc_name)
    {
        AXIS2_FREE(env->allocator, svc->axis_svc_name);
        svc->axis_svc_name = NULL;
    }

    if (svc->op_alias_map)
    {
        axutil_hash_index_t *hi = NULL;
        void *val = NULL;

        for (hi = axutil_hash_first(svc->op_alias_map, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &val);

            if (val)
            {
                if (axis2_op_is_from_module((axis2_op_t *) val, env) ==
                    AXIS2_FALSE)
                {
                    axis2_op_free((axis2_op_t *) val, env);
                }
                val = NULL;
            }
        }

        axutil_hash_free(svc->op_alias_map, env);
    }

    if (svc->op_action_map)
    {
        axutil_hash_index_t *hi = NULL;
        const void *key = NULL;

        for (hi = axutil_hash_first(svc->op_action_map, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, &key, NULL, NULL);

            if (key)
            {
                AXIS2_FREE(env->allocator, (axis2_char_t *) key);
                key = NULL;
            }
        }
        axutil_hash_free(svc->op_action_map, env);
    }

    if (svc->op_rest_map)
    {
        axis2_core_utils_free_rest_map(env, svc->op_rest_map);
    }

    if (svc->schema_target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->schema_target_ns_prefix);
        svc->schema_target_ns_prefix = NULL;
    }

    if (svc->target_ns)
    {
        AXIS2_FREE(env->allocator, svc->target_ns);
        svc->target_ns = NULL;
    }

    if (svc->wsdl_path)
    {
        AXIS2_FREE(env->allocator, svc->wsdl_path);
        svc->wsdl_path = NULL;
    }

    if (svc->folder_path)
    {
        AXIS2_FREE(env->allocator, svc->folder_path);
        svc->folder_path = NULL;
    }

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }

    if (svc->qname)
    {
        axutil_qname_free(svc->qname, env);
    }

    if (svc->style)
    {
        AXIS2_FREE(env->allocator, svc->style);
    }

    if (svc->base)
    {
        axis2_desc_free(svc->base, env);
    }
	if (svc->mutex)
	{
		axutil_thread_mutex_destroy(svc->mutex);
	}
    if (svc)
    {
        AXIS2_FREE(env->allocator, svc);
        svc = NULL;
    }
    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_op(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axis2_op_t * op)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_msg_recv_t *msg_recv = NULL;
    const axutil_qname_t *qname = NULL;
    axis2_char_t *key = NULL;
    const axis2_char_t *svcname = NULL;
    axutil_array_list_t *mappings_list = NULL;
    int size = 0;
    int j = 0;

    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    svcname = axis2_svc_get_name(svc, env);
    qname = axis2_op_get_qname(op, env);
    if (qname)
        key = axutil_qname_get_localpart(qname, env);
    mappings_list = axis2_op_get_wsamapping_list(op, env);
    /* Adding action mappings into service */
    if (mappings_list)
        size = axutil_array_list_size(mappings_list, env);
    for (j = 0; j < size; j++)
    {
        axis2_char_t *mapping = NULL;

        mapping =
            (axis2_char_t *) axutil_array_list_get(mappings_list, env, j);
        status = axis2_svc_add_mapping(svc, env, mapping, op);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Adding operation %s to service %s mapping list failed", 
                svcname, key);
            return status;
        }
    }

    status = axis2_op_set_parent(op, env, svc);
    if (AXIS2_SUCCESS != status)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Setting service %s as operation %s parent failed", svcname, key);
        return status;
    }
    msg_recv = axis2_op_get_msg_recv(op, env);
    if (msg_recv == NULL)
    {
        msg_recv = axis2_desc_builder_load_default_msg_recv(env);
        axis2_op_set_msg_recv(op, env, msg_recv);
    }
    if (key)
    {
        /* If service defines the operation, then we should not override with module level 
         * operation. Module operations are global. If any setting to be modified, those operations
         * can be defined in service */
        if(!axutil_hash_get(svc->op_alias_map, key, AXIS2_HASH_KEY_STRING))
        {
            axutil_hash_set(svc->op_alias_map, key, AXIS2_HASH_KEY_STRING, op);
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_qname(
    const axis2_svc_t * svc,
    const axutil_env_t * env,
    const axutil_qname_t * op_qname)
{
    axis2_op_t *op = NULL;
    axis2_char_t *nc_name = NULL;

    axis2_char_t *nc_tmp = NULL;
    /* This is just for the sake of comparison,
     * and must not be used to change the passed value
     */
    axis2_bool_t is_matched = AXIS2_FALSE;

    AXIS2_PARAM_CHECK(env->error, op_qname, NULL);

    nc_name = axutil_qname_get_localpart(op_qname, env);
    nc_tmp = nc_name;

    op = axutil_hash_get(svc->op_alias_map, nc_tmp, AXIS2_HASH_KEY_STRING);
    if (op)
    {
        return op;
    }
    op = axutil_hash_get(svc->op_action_map, nc_tmp, AXIS2_HASH_KEY_STRING);
    if (op)
    {
        return op;
    }
    
    if (*nc_tmp && svc->op_action_map)
    {
        axutil_hash_index_t *hi = NULL;
        const void *key = NULL;

        for (hi = axutil_hash_first(svc->op_action_map, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, &key, NULL, NULL);

            nc_tmp = nc_name;

            if (key)
            {
                axis2_char_t *search = NULL;
                axis2_bool_t match_start = AXIS2_TRUE;
                axis2_char_t *search_tmp = NULL;

                search = (axis2_char_t *)key;

                if (!axutil_strchr(search, '*'))
                {
                    if (axutil_strstr(nc_tmp, search))
                    {
                        axis2_char_t *comp_tmp = NULL;

                        comp_tmp = axutil_strstr(nc_tmp, search);
                        if (strlen(comp_tmp) == strlen (search))
                        {
                            nc_tmp = (axis2_char_t *)key;
                            is_matched = AXIS2_TRUE;
                            break;
                        }
                    }
                    continue;
                }

                if (search[0] == '*')
                {
                    search++;
                    if (!*search)
                    {
                        nc_tmp = (axis2_char_t *)key;
                        is_matched = AXIS2_TRUE;
                        break;
                    }
                    else if (axutil_strchr(search, '*'))
                    {
                        continue;
                    }
                    match_start = AXIS2_FALSE;
                }
                while (search && *search)
                {
                    int length = 0;
                    axis2_char_t *loc_tmp = NULL;

                    if (search_tmp)
                    {
                        AXIS2_FREE(env->allocator, search_tmp);
                        search_tmp = NULL;
                    }
                    loc_tmp = axutil_strchr(search, '*');
                    if (loc_tmp && *loc_tmp)
                    {
                        if (!loc_tmp[1])
                        {
                            is_matched = AXIS2_TRUE;
                            break;
                        }
                        length = (int)(loc_tmp - search);
                        /* We are sure that the difference lies within the int range */
                        search_tmp = (axis2_char_t *) (AXIS2_MALLOC (env->allocator,
                                          sizeof (axis2_char_t) * (length + 1)));
                        strncpy(search_tmp, search, length);
                        search_tmp[length] = '\0';
                    }
                    else if (axutil_strstr(nc_tmp, search))
                    {
                        axis2_char_t *comp_tmp = NULL;

                        comp_tmp = axutil_strstr(nc_tmp, search);
                        if (strlen(comp_tmp) == strlen (search))
                        {
                            nc_tmp = (axis2_char_t *)key;
                            is_matched = AXIS2_TRUE;
                            break;
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                    if (search_tmp && axutil_strstr(nc_tmp, search_tmp))
                    {
                        if (match_start && !(axutil_strncmp(nc_tmp, search, length) == 0))
                        {
                            break;
                        }
                        else if (!match_start)
                        {
                            match_start = AXIS2_TRUE;
                        }
                    }
                    else
                    {
                        break;
                    }
                    search += axutil_strlen(search_tmp) + 1;
                    nc_tmp = axutil_strstr(nc_tmp, search_tmp) + axutil_strlen(search_tmp);
                }
                if (search_tmp)
                {
                    AXIS2_FREE(env->allocator, search_tmp);
                    search_tmp = NULL;
                }
                if (is_matched || !*search)
                {
                    nc_tmp = (axis2_char_t *)key;
                    is_matched = AXIS2_TRUE;
                    break;
                }
            }
        }
    }
    if (!is_matched)
    {
        nc_tmp = nc_name;
    }

    op = axutil_hash_get(svc->op_alias_map, nc_tmp, AXIS2_HASH_KEY_STRING);
    if (op)
    {
        return op;
    }
    return (axis2_op_t *)axutil_hash_get(svc->op_action_map, nc_tmp, AXIS2_HASH_KEY_STRING);
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_svc_get_rest_op_list_with_method_and_location(
    const axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * method,
    const axis2_char_t * location)
{
    axutil_array_list_t *op_list = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *loc_str = NULL;
    axis2_char_t *loc_str_tmp = NULL;
    axis2_char_t *rindex = NULL;
    int plen;

    AXIS2_PARAM_CHECK(env->error, method, NULL);
    AXIS2_PARAM_CHECK(env->error, location, NULL);

    loc_str_tmp = (axis2_char_t *) location; /* Casted to facilitate loop */
    if (loc_str_tmp[1] == '/')
    {
        loc_str_tmp++;
    } /* ignore any '/' at the beginning */
    if (strchr(loc_str_tmp, '?'))
    {
        axis2_char_t *temp = NULL;

        temp = strchr(loc_str_tmp, '?');
        temp[0] = '\0';
    } /* ignore block after '?' */
    do
    {
        axis2_char_t *temp = NULL;
        temp = strchr(loc_str_tmp, '{');
        if (temp)
        {
            loc_str_tmp = temp;
        }
        else
        {
            loc_str_tmp += strlen(loc_str_tmp);
            break;
        }
    } while (loc_str_tmp[1] &&
             loc_str_tmp[1] == '{');

    loc_str = (axis2_char_t *) axutil_strmemdup(location, (loc_str_tmp - location), env);

    rindex = axutil_rindex(loc_str, '/');
    if (rindex && *rindex)
    {
        loc_str_tmp = axutil_string_substring_ending_at(loc_str, (int)(rindex - loc_str));
        /* We are sure that the difference lies within the int range */
    }
    else
    {
        loc_str_tmp = loc_str;
    }

    plen = axutil_strlen (method) + axutil_strlen (loc_str_tmp) + 2;
    key = (axis2_char_t *) (AXIS2_MALLOC (env->allocator,
                                          sizeof (axis2_char_t) * plen));
    sprintf (key, "%s:%s", method, loc_str_tmp);
    AXIS2_FREE (env->allocator, loc_str);
    op_list = (axutil_array_list_t *) axutil_hash_get(svc->op_rest_map,
                                                      key, AXIS2_HASH_KEY_STRING);
    AXIS2_FREE (env->allocator, key);
    return op_list;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_name(
    const axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * nc_name)
{
    AXIS2_PARAM_CHECK(env->error, nc_name, NULL);

    return (axis2_op_t *) axutil_hash_get(svc->op_alias_map, nc_name,
                                          AXIS2_HASH_KEY_STRING);
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axis2_svc_get_all_ops(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->op_alias_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_parent(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axis2_svc_grp_t * svc_grp)
{
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    svc->parent = svc_grp;
    if (svc_grp)
    {
        axis2_desc_set_parent(svc->base, env,
                              axis2_svc_grp_get_base(svc_grp, env));
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_svc_grp_t *AXIS2_CALL
axis2_svc_get_parent(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->parent;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_qname(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axutil_qname_t * qname)
{
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    if (svc->qname)
    {
        axutil_qname_free(svc->qname, env);
    }

    if (qname)
    {
        svc->qname = axutil_qname_clone((axutil_qname_t *) qname, env);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axutil_qname_t *AXIS2_CALL
axis2_svc_get_qname(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_param(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axutil_param_t * param)
{
    axis2_char_t *paramname = NULL;
    const axis2_char_t *svcname = axis2_svc_get_name(svc, env);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FAILURE);
    paramname = axutil_param_get_name(param, env);

    if (axis2_svc_is_param_locked(svc, env, axutil_param_get_name(param, env)))
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Parameter %s is locked for service %s", paramname, svcname);
        return AXIS2_FAILURE;
    }
    return axutil_param_container_add_param(svc->param_container, env,
                                            param);
}

AXIS2_EXTERN axutil_param_t *AXIS2_CALL
axis2_svc_get_param(
    const axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * name)
{
    axutil_param_t *param = NULL;
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    param = axutil_param_container_get_param(svc->param_container, env, name);
    if (!param && svc->parent)
    {
        param = axis2_svc_grp_get_param(svc->parent, env, name);
    }
    return param;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_svc_get_all_params(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return axutil_param_container_get_params(svc->param_container, env);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axutil_param_t *param = NULL;
    axis2_svc_grp_t *parent = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FALSE);

    /* Checking the locked value of parent */

    parent = axis2_svc_get_parent(svc, env);
    if (parent)
        locked = axis2_svc_grp_is_param_locked(parent, env, param_name);
    if (parent && locked)
    {
        return AXIS2_TRUE;
    }
    param = axis2_svc_get_param(svc, env, param_name);
    if (param)
    {
        ret = axutil_param_is_locked(param, env);
    }
    return ret;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_engage_module(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axis2_module_desc_t * module_desc,
    axis2_conf_t * conf)
{
    axis2_phase_resolver_t *phase_resolver = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axis2_char_t *svcname = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_svc_engage_module");

    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    svcname = axis2_svc_get_name(svc, env);
    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Creating phase resolver failed for service %s", svcname);

        return AXIS2_FAILURE;
    }

    status = axis2_phase_resolver_engage_module_to_svc(phase_resolver, env, svc, module_desc);
    if (status)
    {
        const axutil_qname_t *qname = NULL;
        status = axutil_array_list_add(svc->engaged_module_list, env, module_desc);
        qname = axis2_module_desc_get_qname(module_desc, env);
        axis2_svc_add_module_qname(svc, env, qname);
    }

    if (phase_resolver)
    {
        axis2_phase_resolver_free(phase_resolver, env);
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_svc_engage_module");

    return status;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_module_engaged(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axutil_qname_t * module_qname)
{
    int i = 0,
        size = 0;
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_svc_is_module_engaged");
    size = axutil_array_list_size(svc->engaged_module_list, env);
    for (i = 0; i < size; i++)
    {
        const axutil_qname_t *module_qname_l = NULL;
        axis2_module_desc_t *module_desc_l = NULL;

        module_desc_l =
            (axis2_module_desc_t *) axutil_array_list_get(svc->engaged_module_list,
                                                          env, i);
        module_qname_l = axis2_module_desc_get_qname(module_desc_l, env);

        if (axutil_qname_equals(module_qname, env, module_qname_l))
        {
            return AXIS2_TRUE;
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_svc_is_module_engaged");
    return AXIS2_FALSE;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_svc_get_engaged_module_list(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->engaged_module_list;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_disengage_module(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axis2_module_desc_t * module_desc,
    axis2_conf_t * conf)
{
    axis2_phase_resolver_t *phase_resolver = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axis2_char_t *svcname = NULL;

    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);
    svcname = axis2_svc_get_name(svc, env);

    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Creating phase resolver failed for service %s", svcname);
        return AXIS2_FAILURE;
    }
    status =
        axis2_phase_resolver_disengage_module_from_svc(phase_resolver, env, svc,
                                                       module_desc);

    axis2_phase_resolver_free(phase_resolver, env);

    return status;
}

/**
 * Here we extract all operations defined in module.xml and built execution 
 * chains for them by calling axis2_phase_resolver_build_execution_chains_for_module_op()
 * function. Within that function handlers of the modules defined for that 
 * operation are added to module operation chains appropriately.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_module_ops(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axis2_module_desc_t * module_desc,
    axis2_conf_t * conf)
{
    axutil_hash_t *map = NULL;
    axutil_hash_index_t *index = NULL;
    axis2_phase_resolver_t *phase_resolver = NULL;
    axis2_op_t *op_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axis2_char_t *svcname = NULL;
    axis2_char_t *modname = NULL;
    axis2_char_t *opname = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_svc_add_module_ops");
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);
    svcname = axis2_svc_get_name(svc, env);
    modname = axutil_qname_get_localpart(axis2_module_desc_get_qname(module_desc, 
        env), env);
    map = axis2_module_desc_get_all_ops(module_desc, env);
    phase_resolver = axis2_phase_resolver_create_with_config_and_svc(env, conf,
                                                                     svc);
    if (!phase_resolver)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Creating phase resolver failed for service %s", svcname);
        return AXIS2_FAILURE;
    }
    for (index = axutil_hash_first(map, env); index; index =
         axutil_hash_next(env, index))
    {
        void *v = NULL;
        axutil_hash_this(index, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        opname = axutil_qname_get_localpart(axis2_op_get_qname(op_desc, env), 
            env);
        status =
            axis2_phase_resolver_build_execution_chains_for_module_op(phase_resolver, env, op_desc);

        if (AXIS2_SUCCESS != status)
        {
            if (phase_resolver)
            {
                axis2_phase_resolver_free(phase_resolver, env);
            }
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Builidng module operation %s failed for module %s", opname, 
                modname);
            return status;
        }

        status = axis2_svc_add_op(svc, env, op_desc);
        if (AXIS2_SUCCESS != status)
        {
            if (phase_resolver)
            {
                axis2_phase_resolver_free(phase_resolver, env);
            }
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Adding operation %s to service %s failed", opname, svcname);
            return status;
        }

    }

    if (phase_resolver)
    {
        axis2_phase_resolver_free(phase_resolver, env);
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_svc_add_module_ops");
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_name(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    if (svc->qname)
    {
        return axutil_qname_get_localpart(svc->qname, env);
    }

    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_name(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * axis_svc_name)
{
    AXIS2_PARAM_CHECK(env->error, axis_svc_name, AXIS2_FAILURE);

    if (svc->axis_svc_name)
    {
        AXIS2_FREE(env->allocator, svc->axis_svc_name);
        svc->axis_svc_name = NULL;
    }
    svc->axis_svc_name = axutil_strdup(env, axis_svc_name);
    if (!svc->axis_svc_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(
    axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN long AXIS2_CALL
axis2_svc_get_last_update(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->last_update;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_file_name(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->filename;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_file_name(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * filename)
{
    AXIS2_PARAM_CHECK(env->error, filename, AXIS2_FAILURE);

    if (svc->filename)
    {
        AXIS2_FREE(env->allocator, svc->filename);
        svc->filename = NULL;
    }
    svc->filename = (axis2_char_t *) axutil_strdup(env, filename);
    if (!svc->filename)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_svc_desc(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->svc_desc;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_svc_desc(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * svc_desc)
{
    AXIS2_PARAM_CHECK(env->error, svc_desc, AXIS2_FAILURE);

    if (svc->svc_desc)
    {
        AXIS2_FREE(env->allocator, svc->svc_desc);
        svc->svc_desc = NULL;
    }
    svc->svc_desc = (axis2_char_t *) axutil_strdup(env, svc_desc);
    if (!svc->svc_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * mapping_key,
    axis2_op_t * op_desc)
{
    AXIS2_PARAM_CHECK(env->error, mapping_key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);

    axutil_hash_set(svc->op_action_map, axutil_strdup(env, mapping_key), 
            AXIS2_HASH_KEY_STRING, op_desc);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_rest_mapping(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * method,
    const axis2_char_t * location,
    axis2_op_t * op_desc)
{

    axis2_char_t *mapping_url = NULL;
    int key_len = 0;
    axis2_char_t* question_char = NULL;
    axis2_char_t* local_location_str = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    local_location_str = (axis2_char_t *) location; /* Casted to facilitate loop */

    /* skip the beginning '/' */
    if(*local_location_str == '/') 
    {
        local_location_str ++;
    }
    question_char = axutil_strchr(local_location_str, '?');
    if(question_char) 
    {
        *question_char = '\0';
    }

    key_len = axutil_strlen(method) + axutil_strlen(local_location_str) + 2;

    mapping_url = (axis2_char_t *) (AXIS2_MALLOC (env->allocator,
                                              sizeof (axis2_char_t) * key_len));
    if(!mapping_url) {
       AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
       AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "No memory. Cannot create the rest mapping url");
       return AXIS2_FAILURE;
    }

    sprintf(mapping_url, "%s:%s", method, local_location_str);

    status = axis2_core_utils_prepare_rest_mapping(env,
                    mapping_url,
                    svc->op_rest_map,
                    op_desc);

    if(mapping_url) {
        AXIS2_FREE(env->allocator, mapping_url);    
    }

    /* restore the question character */
    if(question_char) {
        *question_char = '?';
    }

    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_module_qname(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axutil_qname_t * module_qname)
{
    axutil_qname_t *qmodule_qname_l = NULL;

    AXIS2_PARAM_CHECK(env->error, module_qname, AXIS2_FAILURE);

    qmodule_qname_l = axutil_qname_clone((axutil_qname_t *) module_qname, env);
    return axutil_array_list_add(svc->module_list, env, qmodule_qname_l);
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_svc_get_all_module_qnames(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->module_list;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->target_ns;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * ns)
{
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    if (svc->target_ns)
    {
        AXIS2_FREE(env->allocator, svc->target_ns);
        svc->target_ns = NULL;
    }
    svc->target_ns = axutil_strdup(env, ns);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns_prefix(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->target_ns_prefix;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns_prefix(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * prefix)
{
    AXIS2_PARAM_CHECK(env->error, prefix, AXIS2_FAILURE);

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }
    svc->target_ns_prefix = axutil_strdup(env, prefix);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axis2_svc_get_ns_map(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->ns_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_ns_map(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axutil_hash_t * ns_map)
{
    axutil_hash_index_t *hi = NULL;

    AXIS2_PARAM_CHECK(env->error, ns_map, AXIS2_FAILURE);

    if (svc->ns_map)
    {
        for (hi = axutil_hash_first(svc->ns_map, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            void *value = NULL;
            void *key = NULL;
            axutil_hash_this(hi, (const void **) &key, NULL, (void **) &value);
            if (key)
            {
                AXIS2_FREE(env->allocator, key);
                key = NULL;
            }
            if (value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        axutil_hash_free(svc->ns_map, env);
    }
    svc->ns_map = ns_map;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axis2_svc_swap_mapping_table(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    axutil_hash_t * orig_table)
{
    axutil_hash_t *new_table = NULL;
    axutil_hash_index_t *hi = NULL;

    AXIS2_PARAM_CHECK(env->error, orig_table, NULL);

    new_table = axutil_hash_make(env);

    for (hi = axutil_hash_first(orig_table, env); env;
         hi = axutil_hash_next(env, hi))
    {
        void *value = NULL;
        void *key = NULL;

        axutil_hash_this(hi, (const void **) &key, NULL, (void **) &value);
        axutil_hash_set(new_table, value, AXIS2_HASH_KEY_STRING, key);
    }
    return new_table;
}

AXIS2_EXTERN void *AXIS2_CALL
axis2_svc_get_impl_class(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->impl_class;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_impl_class(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    void *impl_class)
{
    svc->impl_class = impl_class;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_param_container_t *AXIS2_CALL
axis2_svc_get_param_container(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->param_container;
}

AXIS2_EXTERN axis2_flow_container_t *AXIS2_CALL
axis2_svc_get_flow_container(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->flow_container;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_svc_wsdl_path(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->wsdl_path;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_svc_wsdl_path(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * wsdl_path)
{
    AXIS2_PARAM_CHECK(env->error, wsdl_path, AXIS2_FAILURE);
    svc->wsdl_path = (axis2_char_t *) axutil_strdup(env, wsdl_path);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_svc_folder_path(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->folder_path;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_svc_folder_path(
    axis2_svc_t * svc,
    const axutil_env_t * env,
    const axis2_char_t * folder_path)
{
    AXIS2_PARAM_CHECK(env->error, folder_path, AXIS2_FAILURE);
    svc->folder_path = (axis2_char_t *) axutil_strdup(env, folder_path);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_desc_t *AXIS2_CALL
axis2_svc_get_base(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->base;
}

AXIS2_EXTERN axutil_thread_mutex_t * AXIS2_CALL
axis2_svc_get_mutex(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
	return svc->mutex;
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axis2_svc_get_rest_map(
    const axis2_svc_t * svc,
    const axutil_env_t * env)
{
    return svc->op_rest_map;
}

