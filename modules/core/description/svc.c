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
#include <axis2_property.h>
#include <axis2_module.h>
#include "../deployment/axis2_desc_builder.h"

struct axis2_svc
{
    axis2_svc_grp_t *parent;
    axis2_char_t *axis_svc_name;
    /** to keep last update time of the service */
    long last_update;
    axis2_char_t *filename;
    /** to store module descriptions at deploy time parsing */
    axis2_array_list_t *module_list;

    /* service description  */
	 axis2_char_t *svc_desc;
    /**
     * WSDL related stuff
     */
    axis2_hash_t *ns_map;
    /* count of the entries in the namespace map */
    int ns_count;
    /* to keep the XML scheama either from WSDL or
     * C2WSDL(in the future)
     */
    axis2_array_list_t *schema_list;
    /**
     * A table that keeps a mapping of unique XSD names (Strings)
     * against the schema objects. This is populated in the first
     * instance the schemas are asked for and then used to serve
     * the subsequent requests
     */
    axis2_hash_t *schema_mapping_table;
    /**
     * This is where operations are kept
     */
    axis2_hash_t *op_alias_map;
    /**
     * This is where action mappings are kept
     */
    axis2_hash_t *op_action_map;
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
    /* to store the target namespace for the schema */
    axis2_char_t *schema_target_ns;
    axis2_char_t *schema_target_ns_prefix;
    /* to keep the service target name space */
    axis2_char_t *target_ns;
    axis2_char_t *target_ns_prefix;
    /* Used for schema name calculations */
    int sc_calc_count;

    void *impl_class;
    axis2_qname_t *qname;
    axis2_char_t *style;
    axis2_array_list_t *engaged_modules;
    /** parameter container to hold service related parameters */
    struct axis2_param_container *param_container;
    /** flow container that encapsulates the flow related data */
    struct axis2_flow_container *flow_container;
};

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create(const axis2_env_t *env)
{
    axis2_svc_t *svc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc = (axis2_svc_t *)
        AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_t));
    if (! svc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->parent = NULL;
    svc->axis_svc_name = NULL;
    svc->filename = NULL;
    svc->svc_desc = NULL;
    svc->last_update = 0;
    svc->param_container = NULL;
    svc->flow_container = NULL;
    svc->op_alias_map = NULL;
    svc->op_action_map = NULL;
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

    svc->param_container = axis2_param_container_create(env);
    if (! svc->param_container)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->flow_container = axis2_flow_container_create(env);
    if (! svc->flow_container)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->op_alias_map = axis2_hash_make(env);
    if (! svc->op_alias_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** create module list of default size */
    svc->op_action_map = axis2_hash_make(env);
    if (! svc->op_action_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->module_list = axis2_array_list_create(env, 0);
    if (! svc->module_list)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->schema_list = axis2_array_list_create(env,
        AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (! svc->schema_list)
    {
        axis2_svc_free(svc, env);
        return NULL;
    }

    svc->engaged_modules = axis2_array_list_create(env,
        AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (! svc->engaged_modules)
    {
        axis2_svc_free(svc, env);
        return NULL;
    }
    
    svc->schema_loc_adjusted = AXIS2_FALSE;
    if (svc->schema_target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->schema_target_ns_prefix);
        svc->schema_target_ns_prefix = NULL;
    }
    svc->schema_target_ns_prefix = axis2_strdup("ns", env);

    if (svc->target_ns)
    {
        AXIS2_FREE(env->allocator, svc->target_ns);
        svc->target_ns = NULL;
    }
    svc->target_ns = axis2_strdup("http://ws.apache.org/axis2", env);

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }
    svc->target_ns_prefix = axis2_strdup("tns", env);
    svc->sc_calc_count = 0;

    return svc;
}

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create_with_qname(const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_svc_t *svc = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    
    svc = axis2_svc_create(env);
    if (! svc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    status = axis2_svc_set_qname(svc, env, qname);
    if (AXIS2_FAILURE == status)
    {
        axis2_svc_free(svc, env);
        return NULL;
    }

    return svc;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_free(axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (svc->param_container)
    {
        axis2_param_container_free(svc->param_container, env);
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
        AXIS2_FREE (env->allocator, svc->svc_desc);
        svc->svc_desc = NULL;
    }

    svc->parent = NULL;

    if (svc->module_list)
    {
        int i = 0;
        int size = 0;

        size = axis2_array_list_size(svc->module_list, env);
        for (i = 0; i < size; i++)
        {
            axis2_qname_t *qname = NULL;
            qname = axis2_array_list_get(svc->module_list, env, i);
            if (qname)
            {
                axis2_qname_free(qname, env);
            }
        }
        axis2_array_list_free(svc->module_list, env);
    }

    if (svc->schema_list)
    {
        axis2_array_list_free(svc->schema_list, env);
    }

    if (svc->engaged_modules)
    {
        axis2_array_list_free(svc->engaged_modules, env);
    }

    if (svc->axis_svc_name)
    {
        AXIS2_FREE(env->allocator, svc->axis_svc_name);
        svc->axis_svc_name = NULL;
    }

    if (svc->op_alias_map)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;

        for (hi = axis2_hash_first(svc->op_alias_map, env); hi;
            hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
            {
                if (axis2_op_is_from_module((axis2_op_t *)val, env) == AXIS2_FALSE )
		{
                    axis2_op_free((axis2_op_t *)val, env);
		}
                val = NULL;
            }
        }

        axis2_hash_free(svc->op_alias_map, env);
    }

    if (svc->op_action_map)
    {
        axis2_hash_index_t *hi = NULL;
        const void *key = NULL;

        for (hi = axis2_hash_first(svc->op_action_map, env); hi;
            hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, &key, NULL, NULL);

            if (key)
            {
                AXIS2_FREE(env->allocator, (axis2_char_t *)key);
                key = NULL;
            }
        }
        axis2_hash_free(svc->op_action_map, env);
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

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator,  svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }

    if (svc->qname)
    {
        axis2_qname_free(svc->qname, env);
    }

    if(svc->style)
    {
        AXIS2_FREE(env->allocator, svc->style);
    }

    if (svc)
    {
        AXIS2_FREE(env->allocator, svc);
        svc = NULL;
    }

    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_op(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_msg_recv_t *msg_recv = NULL;
    const axis2_qname_t *qname = NULL;
    axis2_char_t *key = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);

    status = axis2_op_set_parent(op, env, svc);
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }
    msg_recv = axis2_op_get_msg_recv(op, env);
    if (msg_recv == NULL)
    {
        msg_recv = axis2_desc_builder_load_default_msg_recv(env);
        axis2_op_set_msg_recv(op, env, msg_recv);
    }
    qname = axis2_op_get_qname(op, env);
    if (qname)
        key = axis2_qname_get_localpart(qname, env);
    if (key)
        axis2_hash_set(svc->op_alias_map, key, AXIS2_HASH_KEY_STRING, op);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_qname(const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname)
{
    axis2_op_t *op = NULL;
    axis2_char_t *key = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_qname, NULL);

    key = axis2_qname_get_localpart(op_qname, env);
    op = axis2_hash_get(svc->op_alias_map, key, AXIS2_HASH_KEY_STRING);
    if (! op)
    {
        op = axis2_hash_get(svc->op_action_map, key, AXIS2_HASH_KEY_STRING);
    }

    return op;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_name(const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *nc_name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, nc_name, NULL);

    return (axis2_op_t *) axis2_hash_get(svc->op_alias_map, nc_name,
        AXIS2_HASH_KEY_STRING);
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_ops(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return svc->op_alias_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_parent(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    svc->parent = svc_grp;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_svc_grp_t *AXIS2_CALL
axis2_svc_get_parent(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return svc->parent;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_qname(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    if (svc->qname)
    {
        axis2_qname_free(svc->qname, env);
    }

    if (qname)
    {
        svc->qname = axis2_qname_clone((axis2_qname_t *)qname, env);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_qname_t *AXIS2_CALL
axis2_svc_get_qname(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_param(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FAILURE);


    if (AXIS2_TRUE == axis2_svc_is_param_locked(svc, env,
        axis2_param_get_name(param, env)))
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        return axis2_param_container_add_param(svc->param_container, env, param);
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_param_t *AXIS2_CALL
axis2_svc_get_param(const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    axis2_param_t *param = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    param = axis2_param_container_get_param(svc->param_container, env, name);
    if (param == NULL && svc->parent)
    {
        param =  axis2_svc_grp_get_param(svc->parent, env, name);
    }
    return param;
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_params(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return axis2_param_container_get_params(svc->param_container, env);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_param_t *param = NULL;
    axis2_svc_grp_t *parent = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FALSE);

    /* checking the locked value of parent */

    parent = axis2_svc_get_parent(svc, env);
    if (parent)
        locked =  axis2_svc_grp_is_param_locked(parent, env, param_name);
    if (parent && AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    }
    param = axis2_svc_get_param(svc, env, param_name);
    if (param)
    {
        ret = axis2_param_is_locked(param, env);
    }
    return ret;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_engage_module(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc,
    axis2_conf_t *conf)
{
    axis2_phase_resolver_t *phase_resolver = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);
    
    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        return AXIS2_FAILURE;
    }
    status = axis2_phase_resolver_engage_module_to_svc(phase_resolver, env, svc,
        module_desc);
    if (status)
    {
        const axis2_qname_t *qname = NULL;
        status = axis2_array_list_add(svc->engaged_modules, env, module_desc);
        qname = axis2_module_desc_get_qname(module_desc, env);
        axis2_svc_add_module_qname(svc, env, qname);
    }
    if (phase_resolver)
    {
        axis2_phase_resolver_free(phase_resolver, env);
    }

    return status;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_module_engaged(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_qname_t *module_qname)
{
    int i = 0, size = 0;
    size = axis2_array_list_size(svc->engaged_modules, env);
    for (i = 0; i < size; i++)
    {
        const axis2_qname_t *module_qname_l = NULL;
        axis2_module_desc_t *module_desc_l = NULL;

        module_desc_l = (axis2_module_desc_t *) axis2_array_list_get(
            svc->engaged_modules, env, i);
        module_qname_l = axis2_module_desc_get_qname(module_desc_l, env);

        if (axis2_qname_equals(module_qname, env, module_qname_l))
        {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_module_ops(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc,
    axis2_conf_t *conf)
{
    axis2_hash_t *map = NULL;
    axis2_hash_index_t *index = NULL;
    axis2_phase_resolver_t *pr = NULL;
    axis2_op_t *op_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    map = axis2_module_desc_get_all_ops(module_desc, env);
    pr = axis2_phase_resolver_create_with_config_and_svc(env, conf, svc);

    if (!pr)
    {
        return AXIS2_FAILURE;
    }

    for (index = axis2_hash_first(map, env); index; index =
        axis2_hash_next(env, index))
    {
        axis2_array_list_t *mappings_list = NULL;
        int size = 0;
        int j = 0;
        void *v = NULL;
        axis2_hash_this(index, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        mappings_list = axis2_op_get_wsamapping_list(op_desc, env);
        /* adding WSA mapping into service */
        size = axis2_array_list_size(mappings_list, env);

        if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            if (pr)
            {
                axis2_phase_resolver_free(pr, env);
            }
            return AXIS2_FAILURE;
        }
        for (j = 0; j < size; j++)
        {
            axis2_char_t *mapping = NULL;

            mapping = (axis2_char_t *) axis2_array_list_get(mappings_list, env, j);
            status = axis2_svc_add_mapping(svc, env, mapping, op_desc);
            if (AXIS2_SUCCESS != status)
            {
                if (pr)
                {
                    axis2_phase_resolver_free(pr, env);
                }
                return status;
            }
        }

        status = axis2_phase_resolver_build_module_op(pr, env, op_desc);

        if (AXIS2_SUCCESS != status)
        {
            if (pr)
            {
                axis2_phase_resolver_free(pr, env);
            }
            return status;
        }

        status = axis2_svc_add_op(svc, env, op_desc);
        if (AXIS2_SUCCESS != status)
        {
            if (pr)
            {
                axis2_phase_resolver_free(pr, env);
            }
            return status;
        }

    }

    if (pr)
    {
        axis2_phase_resolver_free(pr, env);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_name(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    if (svc->qname)
    {
        return axis2_qname_get_localpart(svc->qname, env);
    }

    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_name(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *axis_svc_name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, axis_svc_name, AXIS2_FAILURE);

    if (svc->axis_svc_name)
    {
        AXIS2_FREE(env->allocator, svc->axis_svc_name);
        svc->axis_svc_name = NULL;
    }
    svc->axis_svc_name = axis2_strdup(axis_svc_name, env);
    if (!svc->axis_svc_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN long AXIS2_CALL
axis2_svc_get_last_update(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->last_update;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_file_name(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->filename;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_file_name(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *filename)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, filename, AXIS2_FAILURE);

    if (svc->filename)
    {
        AXIS2_FREE(env->allocator, svc->filename);
        svc->filename = NULL;
    }
    svc->filename = (axis2_char_t *) axis2_strdup(filename, env);
    if (!svc->filename)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_svc_desc(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->svc_desc;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_svc_desc(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *svc_desc)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_desc, AXIS2_FAILURE);

    if (svc->svc_desc)
    {
        AXIS2_FREE(env->allocator, svc->svc_desc);
        svc->svc_desc = NULL;
    }
    svc->svc_desc = (axis2_char_t *) axis2_strdup(svc_desc, env);
    if (!svc->svc_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *mapping_key ,
    axis2_op_t *op_desc)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, mapping_key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);

    axis2_hash_set(svc->op_action_map, axis2_strdup(mapping_key, env),
            AXIS2_HASH_KEY_STRING, op_desc);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_module_qname(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *module_qname)
{
    axis2_qname_t *qmodule_qname_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_qname, AXIS2_FAILURE);

    qmodule_qname_l = axis2_qname_clone((axis2_qname_t *)module_qname, env);
    return axis2_array_list_add(svc->module_list, env,
            qmodule_qname_l);
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_module_qnames(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return svc->module_list;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->target_ns;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    if (svc->target_ns)
    {
        AXIS2_FREE(env->allocator, svc->target_ns);
        svc->target_ns = NULL;
    }
    svc->target_ns = axis2_strdup(ns, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns_prefix(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->target_ns_prefix;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns_prefix(axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, prefix, AXIS2_FAILURE);

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }
    svc->target_ns_prefix = axis2_strdup(prefix, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_get_ns_map(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->ns_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_ns_map(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *ns_map)
{
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns_map, AXIS2_FAILURE);


    if (svc->ns_map)
    {
        for (hi = axis2_hash_first(svc->ns_map, env);  hi;
            hi = axis2_hash_next(env, hi))
        {
            void * value = NULL;
            void *key = NULL;
            axis2_hash_this(hi, (const void **)&key, NULL,
                (void **)&value);
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
        axis2_hash_free(svc->ns_map, env);
    }
    svc->ns_map = ns_map;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_swap_mapping_table(axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *orig_table)
{
    axis2_hash_t *new_table = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, orig_table, AXIS2_FAILURE);

    new_table = axis2_hash_make(env);

    for (hi = axis2_hash_first(orig_table, env);  env;
        hi = axis2_hash_next(env, hi))
    {
        void * value = NULL;
        void *key = NULL;

        axis2_hash_this(hi, (const void **)&key, NULL,
            (void **)&value);
        axis2_hash_set(new_table, value, AXIS2_HASH_KEY_STRING, key);
    }
    return new_table;
}

AXIS2_EXTERN void *AXIS2_CALL
axis2_svc_get_impl_class(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->impl_class;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_impl_class(axis2_svc_t *svc,
    const axis2_env_t *env,
    void *impl_class)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc->impl_class = impl_class;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_param_container_t *AXIS2_CALL
axis2_svc_get_param_container(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->param_container;
}

AXIS2_EXTERN axis2_flow_container_t *AXIS2_CALL
axis2_svc_get_flow_container(const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->flow_container;
}



