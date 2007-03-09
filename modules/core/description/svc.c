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
axis2_svc_create(
    const axis2_env_t *env)
{
    axis2_svc_t *svc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc = (axis2_svc_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_t));
    if (NULL == svc)
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
    if (NULL == svc->param_container)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->flow_container = axis2_flow_container_create(env);
    if (NULL == svc->flow_container)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->op_alias_map = axis2_hash_make(env);
    if (NULL == svc->op_alias_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** create module list of default size */
    svc->op_action_map = axis2_hash_make(env);
    if (NULL == svc->op_action_map)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->module_list = axis2_array_list_create(env, 0);
    if (NULL == svc->module_list)
    {
        axis2_svc_free(svc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc->schema_list = axis2_array_list_create(env,
            AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (NULL == svc->schema_list)
    {
        axis2_svc_free(svc, env);
        return NULL;
    }

    svc->engaged_modules = axis2_array_list_create(env,
            AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (NULL == svc->engaged_modules)
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
    svc->schema_target_ns_prefix = AXIS2_STRDUP("ns", env);

    if (svc->target_ns)
    {
        AXIS2_FREE(env->allocator, svc->target_ns);
        svc->target_ns = NULL;
    }
    svc->target_ns = AXIS2_STRDUP("http://ws.apache.org/axis2", env);

    if (svc->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->target_ns_prefix);
        svc->target_ns_prefix = NULL;
    }
    svc->target_ns_prefix = AXIS2_STRDUP("tns", env);
    svc->sc_calc_count = 0;

    return svc;
}

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create_with_qname(
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_svc_t *svc = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    
    svc = axis2_svc_create(env);
    if (NULL == svc)
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
axis2_svc_free(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (svc->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(svc->param_container, env);
        svc->param_container = NULL;
    }

    if (svc->flow_container)
    {
        axis2_flow_container_free(svc->flow_container, env);
        svc->flow_container = NULL;
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
                AXIS2_QNAME_FREE(qname, env);
                qname = NULL;
            }
        }
        axis2_array_list_free(svc->module_list, env);
        svc->module_list = NULL;
    }

    if (svc->schema_list)
    {
        axis2_array_list_free(svc->schema_list, env);
        svc->schema_list = NULL;
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
                    axis2_op_free((axis2_op_t *)val, env);
                val = NULL;
            }
        }

        axis2_hash_free(svc->op_alias_map, env);
        svc->op_alias_map = NULL;
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
        svc->op_action_map = NULL;
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
axis2_svc_add_op(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    axis2_status_t status = AXIS2_FAILURE;
    /*axis2_array_list_t *modules = NULL;
    int i = 0, size = 0;*/
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
    /*modules = axis2_svc_get_all_engaged_modules(svc, env);
    if (modules)
        size = axis2_array_list_size(modules, env);
    for (i = 0; i < size; i++)
    {
        axis2_module_desc_t *module_desc = NULL;
        axis2_module_t *module_impl = NULL;
        axis2_svc_grp_t *parent = NULL;
        axis2_conf_t *conf = NULL;

        module_desc = (axis2_module_desc_t *) axis2_array_list_get(modules, env, i);
        parent = (axis2_svc_grp_t *) axis2_svc_get_parent(svc, env);
        if (parent)
        {
            conf = (axis2_conf_t *) AXIS2_SVC_GRP_GET_PARENT(parent, env);
        }
        if (module_desc)
            module_impl = axis2_module_desc_get_module(module_desc, env);
        if (module_impl)
        {
        }
        status = axis2_op_engage_module(op, env, module_desc, conf);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Module already engaged to operation");
        }
    }*/
    msg_recv = axis2_op_get_msg_recv(op, env);
    if (msg_recv == NULL)
    {
        msg_recv = axis2_desc_builder_load_default_msg_recv(env);
        axis2_op_set_msg_recv(op, env, msg_recv);
    }
    qname = axis2_op_get_qname(op, env);
    if (qname)
        key = AXIS2_QNAME_GET_LOCALPART(qname, env);
    if (key)
        axis2_hash_set(svc->op_alias_map, key, AXIS2_HASH_KEY_STRING, op);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_qname(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname)
{
    axis2_op_t *op = NULL;
    axis2_char_t *key = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_qname, NULL);

    key = AXIS2_QNAME_GET_LOCALPART(op_qname, env);
    op = axis2_hash_get(svc->op_alias_map, key, AXIS2_HASH_KEY_STRING);
    if (NULL == op)
        op = axis2_hash_get(svc->op_action_map, key, AXIS2_HASH_KEY_STRING);

    return op;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *nc_name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, nc_name, NULL);

    return (axis2_op_t *) axis2_hash_get(svc->op_alias_map, nc_name,
            AXIS2_HASH_KEY_STRING);
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_ops(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return svc->op_alias_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_parent(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    svc->parent = svc_grp;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_svc_grp_t *AXIS2_CALL
axis2_svc_get_parent(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return svc->parent;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_qname(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    if (svc->qname)
    {
        axis2_qname_free(svc->qname, env);
        svc->qname = NULL;
    }

    if (qname)
    {
        svc->qname = axis2_qname_clone((axis2_qname_t *)qname, env);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_qname_t *AXIS2_CALL
axis2_svc_get_qname(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_param(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FAILURE);


    if (AXIS2_TRUE == axis2_svc_is_param_locked(svc, env,
            AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(svc->param_container, env, param);
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_param_t *AXIS2_CALL
axis2_svc_get_param(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    axis2_param_t *param = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    param = AXIS2_PARAM_CONTAINER_GET_PARAM(svc->param_container, env, name);
    if (param == NULL && svc->parent)
    {
        param = AXIS2_SVC_GRP_GET_PARAM(svc->parent, env, name);
    }
    return param;
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_params(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(svc->param_container, env);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked(
    axis2_svc_t *svc,
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
        locked = AXIS2_SVC_GRP_IS_PARAM_LOCKED(parent, env, param_name);
    if (parent && AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    }
    param = axis2_svc_get_param(svc, env, param_name);
    if (param)
    {
        ret = AXIS2_PARAM_IS_LOCKED(param, env);
    }
    return ret;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_engage_module(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc,
    axis2_conf_t *conf)
{
    /*axis2_module_desc_t *modu = NULL;*/
    /*axis2_array_list_t *collection_module = NULL;*/
    axis2_phase_resolver_t *phase_resolver = NULL;
    /*int i = 0;*/
    axis2_status_t status = AXIS2_FAILURE;
    /*int size = 0;
    axis2_property_t *property = NULL;*/

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);
    

    /*property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
                    env, AXIS2_MODULEREF_KEY);
    if (property)
        collection_module = (axis2_array_list_t *)
                AXIS2_PROPERTY_GET_VALUE(property, env);
    if (!collection_module)
    {
        return AXIS2_FAILURE;
    }
    size = axis2_array_list_size(collection_module, env);
    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE(env->error);
    }
    for (i = 0; i < size; i++)
    {
        modu = (axis2_module_desc_t *) axis2_array_list_get(collection_module,
                env, i);
        if (axis2_qname_equals(axis2_module_desc_get_qname(modu, env), env,
                axis2_module_desc_get_qname(module_desc, env)))
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

    }*/

    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, env, svc,
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
        AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
        phase_resolver = NULL;
    }

    return status;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_module_engaged(
    axis2_svc_t *svc,
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
axis2_svc_add_module_ops(
    axis2_svc_t *svc,
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
                AXIS2_PHASE_RESOLVER_FREE(pr, env);
                pr = NULL;
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
                    AXIS2_PHASE_RESOLVER_FREE(pr, env);
                    pr = NULL;
                }
                return status;
            }
        }

        status = AXIS2_PHASE_RESOLVER_BUILD_MODULE_OP(pr, env, op_desc);

        if (AXIS2_SUCCESS != status)
        {
            if (pr)
            {
                AXIS2_PHASE_RESOLVER_FREE(pr, env);
                pr = NULL;
            }
            return status;
        }

        status = axis2_svc_add_op(svc, env, op_desc);
        if (AXIS2_SUCCESS != status)
        {
            if (pr)
            {
                AXIS2_PHASE_RESOLVER_FREE(pr, env);
                pr = NULL;
            }
            return status;
        }

    }

    if (pr)
    {
        AXIS2_PHASE_RESOLVER_FREE(pr, env);
        pr = NULL;
    }
    return AXIS2_SUCCESS;
}

/*AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc)
{
    axis2_svc_t *svc = NULL;
    const axis2_qname_t *module_qname = NULL;
    int i = 0;
    int size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    svc = AXIS2_INTF_TO_IMPL(svc);
    if (!svc->engaged_modules)
    {
        return AXIS2_FAILURE;
    }
    module_qname = axis2_module_desc_get_qname(module_desc, env);
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
            return AXIS2_SUCCESS;
        }
    }
    return axis2_array_list_add(svc->engaged_modules, env, module_desc);
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_engaged_modules(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_svc_t *svc = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    svc = AXIS2_INTF_TO_IMPL(svc);

    return svc->engaged_modules;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_style(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *style)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, style, AXIS2_FAILURE);

    if (svc->style)
    {
        AXIS2_FREE(env->allocator, svc->style);
        svc->style = NULL;
    }

    svc->style = AXIS2_STRDUP(style, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_style(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->style;
}

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_svc_get_in_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                svc->wsdl_svc->wsdl_component, env, AXIS2_IN_FLOW_KEY);
    if (property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_in_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *in_flow)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_flow, AXIS2_FAILURE);

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, in_flow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_IN_FLOW_KEY, property);
}

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_svc_get_out_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                svc->wsdl_svc->wsdl_component, env, AXIS2_OUT_FLOW_KEY);
    if (property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_out_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *out_flow)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_flow, AXIS2_FAILURE);

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, out_flow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_OUT_FLOW_KEY, property);
}

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_svc_get_fault_in_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                svc->wsdl_svc->wsdl_component, env, AXIS2_IN_FAULTFLOW_KEY);
    if (property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_fault_in_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *fault_flow)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_flow, AXIS2_FAILURE);

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, fault_flow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_IN_FAULTFLOW_KEY, property);
}

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_svc_get_fault_out_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_property_t *property = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                svc->wsdl_svc->wsdl_component, env, AXIS2_OUT_FAULTFLOW_KEY);
    if (property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_fault_out_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *fault_flow)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_flow, AXIS2_FAILURE);

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, fault_flow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_OUT_FAULTFLOW_KEY, property);
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_by_soap_action(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *soap_action)
{
    axis2_hash_t *endpoints = NULL;
    axis2_hash_index_t *index = NULL;
    void *value = NULL;
    axis2_wsdl_endpoint_t *endpoint = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_action, NULL);

    if (0 == AXIS2_STRCMP(soap_action, ""))
        return NULL;

    endpoints = axis2_svc_get_all_endpoints(svc, env);
    if (!endpoints)
    {
        return NULL;
    }
    index = axis2_hash_first(endpoints, env);
    if (index)
    {
        axis2_qname_t *endpoint_qname = NULL;

        axis2_hash_this(index, NULL, NULL, &value);
        endpoint = (axis2_wsdl_endpoint_t *) value;
        endpoint_qname = AXIS2_WSDL_ENDPOINT_GET_NAME(endpoint, env);
        return axis2_svc_get_op_by_soap_action_and_endpoint(svc, env,
                soap_action, endpoint_qname);
    }

    return NULL;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_svc_get_op_by_soap_action_and_endpoint(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *soap_action,
    const axis2_qname_t *endpoint)
{
    axis2_hash_t *binding_ops = NULL;
    int count = 0;
    axis2_hash_index_t *index_i = NULL;
    axis2_wsdl_binding_op_t *binding_op = NULL;
    axis2_wsdl_soap_op_t *element = NULL;
    axis2_op_t *op = NULL;
    axis2_linked_list_t *extensiblity_elements = NULL;
    axis2_qname_t *type_1 = NULL;
    axis2_wsdl_binding_t *wsdl_binding = NULL;
    axis2_wsdl_endpoint_t *wsdl_endpoint = NULL;

    type_1 = axis2_qname_create(env, "op", AXIOM_SOAP_11_OP, NULL);
    wsdl_endpoint = axis2_svc_get_endpoint(svc, env, endpoint);
    wsdl_binding = AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, env);
    binding_ops = AXIS2_WSDL_BINDING_GET_BINDING_OPS(wsdl_binding, env);
    if (NULL == binding_ops)
    {
        return NULL;
    }
    index_i = axis2_hash_first(binding_ops, env);
    while (index_i)
    {
        int j = 0;
        int size = 0;
        const void *k = NULL;
        axis2_status_t status = AXIS2_FAILURE;

        axis2_hash_this(index_i, &k, NULL, NULL);
        binding_op = (axis2_wsdl_binding_op_t *) k;
        extensiblity_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
                    binding_op->extensible_component->wsdl_component, env);

        size = axis2_linked_list_size(extensiblity_elements, env);
        status = AXIS2_ERROR_GET_STATUS_CODE(env->error);
        if (AXIS2_SUCCESS != status)
        {
            return NULL;
        }
        while (j < size)
        {
            axis2_qname_t *type_2 = NULL;

            element = (axis2_wsdl_soap_op_t *) axis2_linked_list_get(
                        extensiblity_elements, env, j);

            type_2 = AXIS2_WSDL_EXTENSIBLE_ELEMENT_GET_TYPE(element->
                    extensible_element, env);
            if (axis2_qname_equals(type_2, env, type_1))
            {
                if (0 == AXIS2_STRCMP(AXIS2_WSDL_SOAP_OP_GET_SOAP_ACTION(
                            element, env), soap_action))
                {
                    op = (axis2_op_t *)
                            AXIS2_WSDL_BINDING_OP_GET_OP(binding_op,
                                    env);
                    count++;
                }

            }
            AXIS2_QNAME_FREE(type_2, env);
            type_2 = NULL;
            j++;
        }
        index_i = axis2_hash_next(env, index_i);
    }
    if (1 == count)
    {
        return op;
    }
    AXIS2_QNAME_FREE(type_1, env);
    type_1 = NULL;
    return NULL;
} */

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    if (svc->qname)
    {
        return AXIS2_QNAME_GET_LOCALPART(svc->qname, env);
    }

    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_name(
    axis2_svc_t *svc,
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
    svc->axis_svc_name = AXIS2_STRDUP(axis_svc_name, env);
    if (!svc->axis_svc_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN long AXIS2_CALL
axis2_svc_get_last_update(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->last_update;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_file_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->filename;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_file_name(
    axis2_svc_t *svc,
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
    svc->filename = (axis2_char_t *) AXIS2_STRDUP(filename, env);
    if (!svc->filename)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_svc_desc(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return svc->svc_desc;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_svc_desc(
    axis2_svc_t *svc,
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
    svc->svc_desc = (axis2_char_t *) AXIS2_STRDUP(svc_desc, env);
    if (!svc->svc_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/*AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_endpoints(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_WSDL_SVC_GET_ENDPOINTS(svc->wsdl_svc, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_all_endpoints(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *endpoints)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINTS(svc->wsdl_svc, env, endpoints);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_wsdl_endpoint_t *endpoint)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINT(svc->wsdl_svc, env, endpoint);
}

AXIS2_EXTERN axis2_wsdl_endpoint_t *AXIS2_CALL
axis2_svc_get_endpoint(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    return AXIS2_WSDL_SVC_GET_ENDPOINT(svc->wsdl_svc, env, qname);
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_namespace(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_WSDL_SVC_GET_NAMESPACE(svc->wsdl_svc, env);
}
*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *mapping_key ,
    axis2_op_t *op_desc)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, mapping_key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);

    axis2_hash_set(svc->op_action_map, AXIS2_STRDUP(mapping_key, env),
            AXIS2_HASH_KEY_STRING, op_desc);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_module_qname(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *module_qname)
{
    axis2_qname_t *qmodule_qname_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_qname, AXIS2_FAILURE);

    qmodule_qname_l = AXIS2_QNAME_CLONE((axis2_qname_t *)module_qname, env);
    return axis2_array_list_add(svc->module_list, env,
            qmodule_qname_l);
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_module_qnames(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return svc->module_list;
}

/*AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_svc_is_schema_location_adjusted(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return svc->schema_loc_adjusted;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_schema_location_adjusted(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_bool_t adjusted)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc->schema_loc_adjusted = adjusted;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_get_schema_mapping_table(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->schema_mapping_table;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_schema_mapping_table(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *table)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, table, AXIS2_FAILURE);

    if (svc->schema_mapping_table)
    {
        axis2_hash_free(svc->schema_mapping_table, env);
        svc->schema_mapping_table = NULL;
    }
    svc->schema_mapping_table = table;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_custom_schema_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->custom_schema_name_prefix;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_custom_schema_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (svc->custom_schema_name_prefix)
    {
        AXIS2_FREE(env->allocator, svc->custom_schema_name_prefix);
        svc->custom_schema_name_prefix = NULL;
    }
    svc->custom_schema_name_prefix = AXIS2_STRDUP(prefix, env);
    if (NULL == svc->custom_schema_name_prefix)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_custom_schema_suffix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->custom_schema_name_suffix;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_custom_schema_suffix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *suffix)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (svc->custom_schema_name_suffix)
    {
        AXIS2_FREE(env->allocator, svc->custom_schema_name_suffix);
        svc->custom_schema_name_suffix = NULL;
    }
    svc->custom_schema_name_suffix = AXIS2_STRDUP(suffix, env);
    if (NULL == svc->custom_schema_name_suffix)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_print_schema(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_stream_t *out_stream)
{
    int size = 0;
    int i = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (NULL == svc->schema_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_EMPTY_SCHEMA_LIST,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    size = axis2_array_list_size(svc->schema_list, env);
    for (i = 0; i < size; i++)
    {
        xml_schema_t *schema = NULL;
        axis2_char_t *buffer = NULL;
        schema = axis2_array_list_get(svc->schema_list, env, i);
        buffer = XML_SCHEMA_SERIALIZE(schema, env);
        if (buffer)
        {
            AXIS2_STREAM_WRITE(out_stream, env, buffer, AXIS2_STRLEN(buffer));
        }
        AXIS2_FREE(env->allocator, buffer);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN xml_schema_t *AXIS2_CALL
axis2_svc_get_schema(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    int index)
{
    AXIS2_ENV_CHECK(env, NULL);
    return axis2_svc_add_all_namespaces((axis2_svc_t *) svc, env, index);
}

AXIS2_EXTERN xml_schema_t *AXIS2_CALL
axis2_svc_add_all_namespaces(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    int index)
{
    xml_schema_t *schema = NULL;
    axis2_hash_t *prefix_map = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (NULL == svc->schema_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_EMPTY_SCHEMA_LIST,
                AXIS2_FAILURE);
        return NULL;
    }
    schema = (xml_schema_t *)axis2_array_list_get(svc->schema_list, env, index);
    prefix_map = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(schema, env);
    if (NULL == prefix_map)
    {
        return NULL;
    }
    for (hi = axis2_hash_first(svc->ns_map, env);  hi;
            hi = axis2_hash_next(env, hi))
    {
        void * value = NULL;
        void *key = NULL;
        axis2_hash_this(hi, (const void **)&key, NULL, (void **)&value);
        if (value)
        {
            axis2_hash_set(prefix_map, key, AXIS2_HASH_KEY_STRING, value);
        }
    }
    return schema;
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_schemas(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->schema_list;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_schema(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);

    if (NULL == svc->schema_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_EMPTY_SCHEMA_LIST,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    axis2_array_list_add(svc->schema_list, env, schema);
    return axis2_svc_add_schema_ns(svc, env, XML_SCHEMA_GET_TARGET_NAMESPACE(
                schema, env));
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_all_schemas(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_array_list_t *schemas)
{
    int i = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schemas, AXIS2_FAILURE);

    for (i = 0; i < axis2_array_list_size(schemas, env); i++)
    {
        xml_schema_t *schema = axis2_array_list_get(schemas, env, i);
        axis2_svc_add_schema(svc, env, schema);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_schema_target_ns(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->schema_target_ns;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_schema_target_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    if (svc->schema_target_ns)
    {
        AXIS2_FREE(env->allocator, svc->schema_target_ns);
        svc->schema_target_ns = NULL;
    }
    svc->schema_target_ns = AXIS2_STRDUP(ns, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_schema_target_ns_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->schema_target_ns_prefix;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_schema_target_ns_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, prefix, AXIS2_FAILURE);

    if (svc->schema_target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc->schema_target_ns_prefix);
        svc->schema_target_ns_prefix = NULL;
    }
    svc->schema_target_ns_prefix = AXIS2_STRDUP(prefix, env);
    return AXIS2_SUCCESS;
}*/

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->target_ns;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns(
    axis2_svc_t *svc,
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
    svc->target_ns = AXIS2_STRDUP(ns, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->target_ns_prefix;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns_prefix(
    axis2_svc_t *svc,
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
    svc->target_ns_prefix = AXIS2_STRDUP(prefix, env);
    return AXIS2_SUCCESS;
}

/*
AXIS2_EXTERN xml_schema_element_t *AXIS2_CALL
axis2_svc_get_schema_element(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    int i = 0;

    AXIS2_ENV_CHECK(env, NULL);

    for (i = 0; i < axis2_array_list_size(svc->schema_list, env); i++)
    {
        xml_schema_t *schema = NULL;
        xml_schema_element_t *schema_element = NULL;
        schema = axis2_array_list_get(svc->schema_list, env, i);
        if (schema)
        {
            schema_element = XML_SCHEMA_GET_ELEMENT_BY_QNAME(schema, env,
                    qname);
            if (schema_element)
            {
                return schema_element;
            }
        }
    }
    return NULL;
}*/

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_get_ns_map(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->ns_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_ns_map(
    axis2_svc_t *svc,
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

/*AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_add_schema_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns)
{
    axis2_bool_t found = AXIS2_FALSE;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    if (svc->ns_map && axis2_hash_count(svc->ns_map) > 0)
    {
        for (hi = axis2_hash_first(svc->ns_map, env);  hi;
                hi = axis2_hash_next(env, hi))
        {
            void * value = NULL;
            void *key = NULL;
            axis2_hash_this(hi, (const void **)&key, NULL,
                    (void **)&value);
            if (0 == AXIS2_STRCMP((axis2_char_t *)value, ns))
            {
                found = AXIS2_TRUE;
                break;
            }
        }
    }
    if (NULL == svc->ns_map)
    {
        svc->ns_map = axis2_hash_make(env);
        svc->ns_map = NULL;
    }
    if (AXIS2_FALSE == found)
    {
        axis2_char_t str_count[32];
        axis2_char_t *tmp_val = NULL;
        axis2_char_t *key = NULL;

        tmp_val = AXIS2_STRDUP(ns, env);
        sprintf(str_count, "%d", svc->ns_count);
        key = AXIS2_STRACAT("ns", str_count, env);
        axis2_hash_set(svc->ns_map, key, AXIS2_HASH_KEY_STRING, tmp_val);
        svc->ns_count++;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_populate_schema_mappings(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_array_list_t *schemas = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    schemas = axis2_svc_get_all_schemas(svc, env);
    if (NULL == schemas)
    {
        return AXIS2_FAILURE;
    }
    if (AXIS2_FALSE == axis2_svc_is_schema_location_adjusted(svc, env))
    {
        int count = 0;
        int i = 0;
        axis2_hash_t *name_table = axis2_hash_make(env);
        axis2_hash_t *schema_table = NULL;

        count = axis2_array_list_size(schemas, env);
        for (i = 0; i < count; i++)
        {
            xml_schema_t *schema = axis2_array_list_get(schemas, env, i);
            axis2_svc_calculate_schema_names(svc, env, schema, name_table);
        }
        for (i = 0; i < count; i++)
        {
            xml_schema_t *schema = axis2_array_list_get(schemas, env, i);
            axis2_svc_adjust_schema_names(svc, env, schema, name_table);
        }
        schema_table = axis2_svc_swap_mapping_table(svc, env, name_table);
        axis2_svc_set_schema_mapping_table(svc, env, schema_table);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_calculate_schema_names(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axis2_hash_t *name_table)
{
    xml_schema_obj_collection_t *collection = NULL;
    xml_schema_obj_t *schema_obj = NULL;
    int count = 0;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name_table, AXIS2_FAILURE);

    collection = XML_SCHEMA_GET_INCLUDES(schema, env);
    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT(collection, env);

    for (i = 0; i < count; i++)
    {
        schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(collection, env, i);
        if (XML_SCHEMA_EXTERNAL == XML_SCHEMA_OBJ_GET_TYPE(schema_obj, env))
        {
            xml_schema_external_t *schema_external = NULL;
            xml_schema_t *schema1 = NULL;
            schema_external = (xml_schema_external_t *)schema_obj;
            schema1 = XML_SCHEMA_EXTERNAL_GET_SCHEMA(schema_external, env);
            if (schema1)
            {
                axis2_char_t *name = NULL;
                axis2_char_t str_count[32];
                sprintf(str_count, "%d", svc->sc_calc_count++);
                axis2_svc_calculate_schema_names(svc, env, schema1, name_table);
                name = axis2_strcat(env, "xsd", str_count,
                        svc->custom_schema_name_suffix, NULL);
                axis2_hash_set(name_table, schema1, AXIS2_HASH_KEY_STRING,
                        name);
            }
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_adjust_schema_names(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axis2_hash_t *names)
{
    xml_schema_obj_collection_t *collection = NULL;
    xml_schema_obj_t *schema_obj = NULL;
    int count = 0;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, names, AXIS2_FAILURE);

    collection = XML_SCHEMA_GET_INCLUDES(schema, env);
    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT(collection, env);
    for (i = 0; i < count; i++)
    {
        schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(collection, env, i);
        if (XML_SCHEMA_EXTERNAL == XML_SCHEMA_OBJ_GET_TYPE(schema_obj, env))
        {
            xml_schema_external_t *schema_external = NULL;
            xml_schema_t *schema1 = NULL;
            schema_external = (xml_schema_external_t *)schema_obj;
            schema1 = XML_SCHEMA_EXTERNAL_GET_SCHEMA(schema_external, env);
            if (schema1)
            {
                axis2_char_t *name = NULL;
                axis2_char_t *value = NULL;

                axis2_svc_adjust_schema_names(svc, env, schema1, names);
                value = (axis2_char_t *) axis2_hash_get(names, schema1,
                        AXIS2_HASH_KEY_STRING);
                if (value)
                {
                    if (svc->custom_schema_name_prefix)
                    {
                        name = axis2_strcat(env,
                                svc->custom_schema_name_prefix, value, NULL);
                    }
                    else
                    {
                        name = axis2_strcat(env, "?xsd=", value, NULL);
                    }
                    XML_SCHEMA_EXTERNAL_SET_SCHEMA_LOCATION(schema_external, env,
                            name);
                }
            }
        }
    }
    return AXIS2_SUCCESS;
}*/

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_svc_swap_mapping_table(
    axis2_svc_t *svc,
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
axis2_svc_get_impl_class(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return svc->impl_class;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_set_impl_class(
    axis2_svc_t *svc,
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


