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

#include <axis2_svc.h>
#include <axis2_addr.h>
#include <axis2_property.h>
#include <xml_schema_obj_collection.h>
#include <axis2_module.h>
#include "../deployment/axis2_desc_builder.h"

typedef struct axis2_svc_impl axis2_svc_impl_t;

struct axis2_svc_impl
{
    axis2_svc_t svc;
    axis2_svc_grp_t *parent;
    axis2_char_t *axis_svc_name;
    /** to keep last update time of the service */
    long last_update;
    axis2_char_t *filename;
    /** to store module descriptions at deploy time parsing */
    axis2_array_list_t *module_list;
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
};

#define AXIS2_INTF_TO_IMPL(svc) ((axis2_svc_impl_t *)svc)

axis2_status_t AXIS2_CALL
axis2_svc_free(
    axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_add_op(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_op_t *op);

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_qname(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *op_name);

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *op_name);

axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_ops(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_parent(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t *AXIS2_CALL
axis2_svc_get_parent(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_qname(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname);

const axis2_qname_t *AXIS2_CALL
axis2_svc_get_qname(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_add_param(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_param_t *param);

axis2_param_t *AXIS2_CALL
axis2_svc_get_param(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_params(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_wsdl_interface(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_wsdl_interface_t *svc_interface);

axis2_wsdl_interface_t *AXIS2_CALL
axis2_svc_get_wsdl_interface(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_engage_module(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc,
    axis2_conf_t *conf);

axis2_status_t AXIS2_CALL
axis2_svc_add_module_ops(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module,
    axis2_conf_t *conf);

axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_name);

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_engaged_modules(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

void *AXIS2_CALL
axis2_svc_get_wsdl_op(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *op_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_style(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *style);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_style(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_flow_t *AXIS2_CALL
axis2_svc_get_in_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_in_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *in_flow);

axis2_flow_t *AXIS2_CALL
axis2_svc_get_out_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_out_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *out_flow);

axis2_flow_t *AXIS2_CALL
axis2_svc_get_fault_in_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_in_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *fault_flow);

axis2_flow_t *AXIS2_CALL
axis2_svc_get_fault_out_flow(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_out_flow(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_flow_t *fault_flow);

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_by_soap_action(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *soap_action);

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_by_soap_action_and_endpoint(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *soap_action,
    const axis2_qname_t *endpoint);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_name(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *axis_svc_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(
    axis2_svc_t *svc,
    const axis2_env_t *env);

long AXIS2_CALL
axis2_svc_get_last_update(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_file_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_file_name(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *filename);

axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_endpoints(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_all_endpoints(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *endpoints);

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_wsdl_endpoint_t *endpoint);

axis2_wsdl_endpoint_t *AXIS2_CALL
axis2_svc_get_endpoint(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_namespace(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *mapping_key ,
    axis2_op_t *op_desc);

axis2_status_t AXIS2_CALL
axis2_svc_add_module_qname(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *module_qname);

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_module_qnames(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_svc_is_schema_location_adjusted(
    axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_location_adjusted(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_bool_t adjusted);

axis2_hash_t *AXIS2_CALL
axis2_svc_get_schema_mapping_table(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_mapping_table(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *table);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_custom_schema_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_custom_schema_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_custom_schema_suffix(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_custom_schema_suffix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *suffix);

axis2_status_t AXIS2_CALL
axis2_svc_print_schema(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_stream_t *out_stream);

xml_schema_t *AXIS2_CALL
axis2_svc_get_schema(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    int index);

xml_schema_t *AXIS2_CALL
axis2_svc_add_all_namespaces(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    int index);

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_schemas(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_add_schema(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema);

axis2_status_t AXIS2_CALL
axis2_svc_add_all_schemas(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_array_list_t *schemas);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_schema_target_ns(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_target_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_schema_target_ns_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_target_ns_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns);

const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix);

xml_schema_element_t *AXIS2_CALL
axis2_svc_get_schema_element(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname);

axis2_hash_t *AXIS2_CALL
axis2_svc_get_ns_map(
    const axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_set_ns_map(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *ns_map);

axis2_status_t AXIS2_CALL
axis2_svc_add_schema_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns);

axis2_status_t AXIS2_CALL
axis2_svc_populate_schema_mappings(
    axis2_svc_t *svc,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_calculate_schema_names(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axis2_hash_t *names);

axis2_status_t AXIS2_CALL
axis2_svc_adjust_schema_names(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axis2_hash_t *names);

axis2_hash_t *AXIS2_CALL
axis2_svc_swap_mapping_table(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *orig_table);

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create(
    const axis2_env_t *env)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_array_list_t *array_list_l = NULL;
    axis2_param_container_t *param_container_l = NULL;
    axis2_wsdl_interface_t *wsdl_interface_l = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_impl = (axis2_svc_impl_t *)
            AXIS2_MALLOC (env->allocator, sizeof(axis2_svc_impl_t));
    if (NULL == svc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_impl->parent = NULL;
    svc_impl->axis_svc_name = NULL;
    svc_impl->filename = NULL;
    svc_impl->last_update = 0;
    svc_impl->svc.param_container = NULL;
    svc_impl->svc.flow_container = NULL;
    svc_impl->svc.wsdl_svc = NULL;
    svc_impl->op_alias_map = NULL;
    svc_impl->op_action_map = NULL;
    svc_impl->module_list = NULL;
    svc_impl->ns_map = NULL;
    svc_impl->ns_count = 0;
    svc_impl->schema_list = NULL;
    svc_impl->schema_mapping_table = NULL;
    svc_impl->schema_loc_adjusted = AXIS2_FALSE;
    svc_impl->custom_schema_name_prefix = NULL;
    svc_impl->custom_schema_name_suffix = NULL;
    svc_impl->schema_target_ns = NULL;
    svc_impl->schema_target_ns_prefix = NULL;
    svc_impl->target_ns = NULL;
    svc_impl->target_ns_prefix = NULL;
    svc_impl->sc_calc_count = 0;

    svc_impl->svc.param_container = axis2_param_container_create(env);
    if (NULL == svc_impl->svc.param_container)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_impl->svc.flow_container = axis2_flow_container_create(env);
    if (NULL == svc_impl->svc.flow_container)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_impl->svc.wsdl_svc = axis2_wsdl_svc_create(env);
    if (NULL == svc_impl->svc.wsdl_svc)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_impl->op_alias_map = axis2_hash_make (env);
    if (NULL == svc_impl->op_alias_map)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** create module list of default size */
    svc_impl->op_action_map = axis2_hash_make (env);            
    if(NULL == svc_impl->op_action_map)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);      
        return NULL;
    }

    svc_impl->module_list = axis2_array_list_create(env, 0);
    if (NULL == svc_impl->module_list)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_impl->svc.ops = AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_ops_t));
    if (NULL == svc_impl->svc.ops)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_interface_l = axis2_wsdl_interface_create(env);
    if (NULL == wsdl_interface_l)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    status = axis2_svc_set_wsdl_interface(&(svc_impl->svc), env, wsdl_interface_l);
    if (AXIS2_FAILURE == status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }

    param_container_l = axis2_param_container_create(env);
    if (NULL == param_container_l)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env,
            axis2_param_container_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, param_container_l);
    status = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
            wsdl_component, env, AXIS2_PARAMETER_KEY, property);
    if (AXIS2_SUCCESS != status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }
    array_list_l = axis2_array_list_create(env, 0);
    if (NULL == array_list_l)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env,
            axis2_array_list_free_void_arg);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_SESSION);
    AXIS2_PROPERTY_SET_VALUE(property, env, array_list_l);

    status = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
            wsdl_component, env, AXIS2_MODULEREF_KEY, property);
    if (AXIS2_SUCCESS != status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }

    svc_impl->schema_list = axis2_array_list_create(env,
            AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);
    if (NULL == svc_impl->schema_list)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }

    svc_impl->schema_loc_adjusted = AXIS2_FALSE;
    if(NULL != svc_impl->schema_target_ns_prefix )
    {
        AXIS2_FREE(env->allocator, svc_impl->schema_target_ns_prefix);
        svc_impl->schema_target_ns_prefix = NULL;
    }
    svc_impl->schema_target_ns_prefix = AXIS2_STRDUP("ns", env);
    
    if(NULL != svc_impl->target_ns )
    {
        AXIS2_FREE(env->allocator, svc_impl->target_ns);
        svc_impl->target_ns = NULL;
    }
    svc_impl->target_ns = AXIS2_STRDUP("http://ws.apache.org/axis2", env);

    if(NULL != svc_impl->target_ns_prefix )
    {
        AXIS2_FREE(env->allocator, svc_impl->target_ns_prefix);
        svc_impl->target_ns_prefix = NULL;
    }
    svc_impl->target_ns_prefix = AXIS2_STRDUP("tns", env);
    svc_impl->sc_calc_count = 0;

    svc_impl->svc.ops->free = axis2_svc_free;
    svc_impl->svc.ops->add_op = axis2_svc_add_op;
    svc_impl->svc.ops->get_op_with_qname = axis2_svc_get_op_with_qname;
    svc_impl->svc.ops->get_op_with_name = axis2_svc_get_op_with_name;
    svc_impl->svc.ops->get_all_ops = axis2_svc_get_all_ops;
    svc_impl->svc.ops->set_parent = axis2_svc_set_parent;
    svc_impl->svc.ops->get_parent = axis2_svc_get_parent;
    svc_impl->svc.ops->set_qname = axis2_svc_set_qname;
    svc_impl->svc.ops->get_qname = axis2_svc_get_qname;
    svc_impl->svc.ops->add_param = axis2_svc_add_param;
    svc_impl->svc.ops->get_param = axis2_svc_get_param;
    svc_impl->svc.ops->get_all_params = axis2_svc_get_all_params;
    svc_impl->svc.ops->is_param_locked = axis2_svc_is_param_locked;
    svc_impl->svc.ops->set_wsdl_interface = axis2_svc_set_wsdl_interface;
    svc_impl->svc.ops->get_wsdl_interface = axis2_svc_get_wsdl_interface;
    svc_impl->svc.ops->engage_module = axis2_svc_engage_module;
    svc_impl->svc.ops->add_module_ops = axis2_svc_add_module_ops;
    svc_impl->svc.ops->add_to_engaged_module_list =
        axis2_svc_add_to_engaged_module_list;
    svc_impl->svc.ops->get_all_engaged_modules = axis2_svc_get_all_engaged_modules;
    svc_impl->svc.ops->get_wsdl_op = axis2_svc_get_wsdl_op;
    svc_impl->svc.ops->set_style = axis2_svc_set_style;
    svc_impl->svc.ops->get_style = axis2_svc_get_style;
    svc_impl->svc.ops->get_in_flow = axis2_svc_get_in_flow;
    svc_impl->svc.ops->set_in_flow = axis2_svc_set_in_flow;
    svc_impl->svc.ops->get_out_flow = axis2_svc_get_out_flow;
    svc_impl->svc.ops->set_out_flow = axis2_svc_set_out_flow;
    svc_impl->svc.ops->get_fault_in_flow = axis2_svc_get_fault_in_flow;
    svc_impl->svc.ops->set_fault_in_flow = axis2_svc_set_fault_in_flow;
    svc_impl->svc.ops->get_fault_out_flow = axis2_svc_get_fault_out_flow;
    svc_impl->svc.ops->set_fault_out_flow = axis2_svc_set_fault_out_flow;
    svc_impl->svc.ops->get_op_by_soap_action = axis2_svc_get_op_by_soap_action;
    svc_impl->svc.ops->get_op_by_soap_action_and_endpoint =
        axis2_svc_get_op_by_soap_action_and_endpoint;
    svc_impl->svc.ops->get_name = axis2_svc_get_name;
    svc_impl->svc.ops->set_name = axis2_svc_set_name;
    svc_impl->svc.ops->set_last_update = axis2_svc_set_last_update;
    svc_impl->svc.ops->get_last_update = axis2_svc_get_last_update;
    svc_impl->svc.ops->get_file_name = axis2_svc_get_file_name;
    svc_impl->svc.ops->set_file_name = axis2_svc_set_file_name;
    svc_impl->svc.ops->get_all_endpoints = axis2_svc_get_all_endpoints;
    svc_impl->svc.ops->set_all_endpoints = axis2_svc_set_all_endpoints;
    svc_impl->svc.ops->set_endpoint = axis2_svc_set_endpoint;
    svc_impl->svc.ops->get_endpoint = axis2_svc_get_endpoint;
    svc_impl->svc.ops->get_namespace = axis2_svc_get_namespace;
    svc_impl->svc.ops->add_mapping = axis2_svc_add_mapping;
    svc_impl->svc.ops->add_module_qname = axis2_svc_add_module_qname;
    svc_impl->svc.ops->get_all_module_qnames = axis2_svc_get_all_module_qnames;

    svc_impl->svc.ops->is_schema_location_adjusted =
        axis2_svc_is_schema_location_adjusted;
    svc_impl->svc.ops->set_schema_location_adjusted =
        axis2_svc_set_schema_location_adjusted;
    svc_impl->svc.ops->get_schema_mapping_table =
        axis2_svc_get_schema_mapping_table;
    svc_impl->svc.ops->set_schema_mapping_table =
        axis2_svc_set_schema_mapping_table;
    svc_impl->svc.ops->get_custom_schema_prefix =
        axis2_svc_get_custom_schema_prefix;
    svc_impl->svc.ops->set_custom_schema_prefix =
        axis2_svc_set_custom_schema_prefix;
    svc_impl->svc.ops->get_custom_schema_suffix =
        axis2_svc_get_custom_schema_suffix;
    svc_impl->svc.ops->set_custom_schema_suffix =
        axis2_svc_set_custom_schema_suffix;
    svc_impl->svc.ops->print_schema = axis2_svc_print_schema;
    svc_impl->svc.ops->get_schema = axis2_svc_get_schema;
    svc_impl->svc.ops->add_all_namespaces = axis2_svc_add_all_namespaces;
    svc_impl->svc.ops->get_all_schemas = axis2_svc_get_all_schemas;
    svc_impl->svc.ops->add_schema = axis2_svc_add_schema;
    svc_impl->svc.ops->add_all_schemas = axis2_svc_add_all_schemas;
    svc_impl->svc.ops->get_schema_target_ns = axis2_svc_get_schema_target_ns;
    svc_impl->svc.ops->set_schema_target_ns = axis2_svc_set_schema_target_ns;
    svc_impl->svc.ops->get_schema_target_ns_prefix =
        axis2_svc_get_schema_target_ns_prefix;
    svc_impl->svc.ops->set_schema_target_ns_prefix =
        axis2_svc_set_schema_target_ns_prefix;
    svc_impl->svc.ops->get_target_ns = axis2_svc_get_target_ns;
    svc_impl->svc.ops->set_target_ns = axis2_svc_set_target_ns;
    svc_impl->svc.ops->get_target_ns_prefix = axis2_svc_get_target_ns_prefix;
    svc_impl->svc.ops->set_target_ns_prefix = axis2_svc_set_target_ns_prefix;
    svc_impl->svc.ops->get_schema_element = axis2_svc_get_schema_element;
    svc_impl->svc.ops->get_ns_map = axis2_svc_get_ns_map;
    svc_impl->svc.ops->set_ns_map = axis2_svc_set_ns_map;
    svc_impl->svc.ops->populate_schema_mappings =
        axis2_svc_populate_schema_mappings;

    return &(svc_impl->svc);
}

axis2_svc_t *AXIS2_CALL
axis2_svc_create_with_qname(
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
    if (NULL == svc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    status = axis2_svc_set_qname(&(svc_impl->svc), env, qname);
    if (AXIS2_FAILURE == status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }

    return &(svc_impl->svc);
}


AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_svc_create_with_wsdl_svc(
    const axis2_env_t *env,
    axis2_wsdl_svc_t *wsdl_svc)
{
    axis2_svc_impl_t *svc_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, wsdl_svc, NULL);

    svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));

    if (NULL == svc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (NULL != svc_impl->svc.wsdl_svc)
    {
        AXIS2_WSDL_SVC_FREE(svc_impl->svc.wsdl_svc, env);
    }

    svc_impl->svc.wsdl_svc = wsdl_svc;

    return &(svc_impl->svc);
}

axis2_status_t AXIS2_CALL
axis2_svc_free(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    if (NULL != svc->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(svc->param_container, env);
        svc->param_container = NULL;
    }

    if (NULL != svc->flow_container)
    {
        AXIS2_FLOW_CONTAINER_FREE(svc->flow_container, env);
        svc->flow_container = NULL;
    }

    if (NULL != svc->wsdl_svc)
    {
        AXIS2_WSDL_SVC_FREE(svc->wsdl_svc, env);
        svc->wsdl_svc = NULL;
    }

    if (svc_impl->filename)
    {
        AXIS2_FREE(env->allocator, svc_impl->filename);
        svc_impl->filename = NULL;
    }

    svc_impl->parent = NULL;

    if (NULL != svc_impl->module_list)
    {
        int i = 0;
        int size = 0;

        size = AXIS2_ARRAY_LIST_SIZE(svc_impl->module_list, env);
        for (i = 0; i < size; i++)
        {
            axis2_qname_t *qname = NULL;
            qname = AXIS2_ARRAY_LIST_GET(svc_impl->module_list, env, i);
            if (qname)
            {
                AXIS2_QNAME_FREE (qname, env);
                qname = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(svc_impl->module_list, env);
        svc_impl->module_list = NULL;
    }

    if(NULL != svc_impl->schema_list)
    {
        AXIS2_ARRAY_LIST_FREE(svc_impl->schema_list, env);
        svc_impl->schema_list = NULL;
    }

    if (NULL != svc_impl->axis_svc_name)
    {
        AXIS2_FREE(env->allocator, svc_impl->axis_svc_name);
        svc_impl->axis_svc_name = NULL;
    }

    if(NULL != svc_impl->op_alias_map)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        
        for (hi = axis2_hash_first(svc_impl->op_alias_map, env); hi;
            hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
            {
                axis2_op_free(val, env);
                val = NULL;
            }
        }

        axis2_hash_free(svc_impl->op_alias_map, env);
        svc_impl->op_alias_map = NULL;
    }

    if(NULL != svc_impl->op_action_map)
    {
        axis2_hash_free(svc_impl->op_action_map, env);
        svc_impl->op_action_map = NULL;
    }

    if(NULL != svc_impl->schema_target_ns_prefix )
    {
        AXIS2_FREE(env->allocator, svc_impl->schema_target_ns_prefix);
        svc_impl->schema_target_ns_prefix = NULL;
    }

    if(NULL != svc_impl->target_ns )
    {
        AXIS2_FREE(env->allocator, svc_impl->target_ns);
        svc_impl->target_ns = NULL;
    }

    if(NULL != svc_impl->target_ns_prefix )
    {
        AXIS2_FREE(env->allocator,  svc_impl->target_ns_prefix);
        svc_impl->target_ns_prefix = NULL;
    }


    if (NULL != svc_impl->svc.ops)
    {
        AXIS2_FREE(env->allocator, svc_impl->svc.ops);
        svc_impl->svc.ops = NULL;
    }

    if (svc_impl)
    {
        AXIS2_FREE(env->allocator, svc_impl);
        svc_impl = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_add_op(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *modules = NULL;
    int i = 0, size = 0;
    axis2_msg_recv_t *msg_recv = NULL;
    const axis2_qname_t *qname = NULL;
    axis2_char_t *key = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    status = AXIS2_OP_SET_PARENT(op, env, svc);
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }
    modules = axis2_svc_get_all_engaged_modules(svc, env);
    if (modules)
        size = AXIS2_ARRAY_LIST_SIZE(modules, env);
    for (i = 0; i < size; i++)
    {
        axis2_module_desc_t *module_desc = NULL;
        axis2_module_t *module_impl = NULL;
        axis2_svc_grp_t *parent = NULL;
        axis2_conf_t *conf = NULL;

        module_desc = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(modules, env, i);
        parent = (axis2_svc_grp_t *) axis2_svc_get_parent(svc, env);
        if (parent)
        {
            conf = (axis2_conf_t *) AXIS2_SVC_GRP_GET_PARENT(parent, env);
        }
        if (module_desc)
            module_impl = AXIS2_MODULE_DESC_GET_MODULE(module_desc, env);
        if (module_impl)
        {
            /* TODO: Notify module for service engagement */
            /*AXIS2_MODULE_ENGAGE_NOTIFY(module_impl, env, op); */
        }
        status = AXIS2_OP_ENGAGE_MODULE(op, env, module_desc, conf);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Module already engaged to operation");
        }
    }
    msg_recv = AXIS2_OP_GET_MSG_RECV(op, env);
    if (msg_recv == NULL)
    {
        msg_recv = axis2_desc_builder_load_default_msg_recv(env);
        AXIS2_OP_SET_MSG_RECV(op, env, msg_recv);
    }
    qname = AXIS2_OP_GET_QNAME(op, env);
    if (qname)
        key = AXIS2_QNAME_GET_LOCALPART(qname, env);
    if (key)
        axis2_hash_set(svc_impl->op_alias_map, key, AXIS2_HASH_KEY_STRING, op);
    return AXIS2_SUCCESS;
}

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_qname(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_op_t *op = NULL;
    axis2_char_t *key = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_qname, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    key = AXIS2_QNAME_GET_LOCALPART(op_qname, env);
    op = axis2_hash_get(svc_impl->op_alias_map, key, AXIS2_HASH_KEY_STRING);
    if(NULL == op)
        op = axis2_hash_get(svc_impl->op_action_map, key, AXIS2_HASH_KEY_STRING);

    return op;
}

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_with_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *nc_name)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, nc_name, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    return (axis2_op_t *) axis2_hash_get(svc_impl->op_alias_map, nc_name,
            AXIS2_HASH_KEY_STRING);
}

axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_ops(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    return svc_impl->op_alias_map;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_parent(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    svc_impl->parent = svc_grp;

    return AXIS2_SUCCESS;
}

axis2_svc_grp_t *AXIS2_CALL
axis2_svc_get_parent(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(svc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_qname(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);

    return AXIS2_WSDL_SVC_SET_QNAME(svc->wsdl_svc, env, qname);
}

const axis2_qname_t *AXIS2_CALL
axis2_svc_get_qname(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_WSDL_SVC_GET_QNAME(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_param (
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

axis2_param_t *AXIS2_CALL
axis2_svc_get_param(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    axis2_param_t *param = NULL;
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    param = AXIS2_PARAM_CONTAINER_GET_PARAM(svc->param_container, env, name);
    if (param == NULL && svc_impl->parent)
    {
        param = AXIS2_SVC_GRP_GET_PARAM(svc_impl->parent, env, name);
    }
    return param;
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_params(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(svc->param_container, env);
}

axis2_bool_t AXIS2_CALL
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

axis2_status_t AXIS2_CALL
axis2_svc_set_wsdl_interface(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_wsdl_interface_t *svc_interface)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_interface, AXIS2_FAILURE);

    return AXIS2_WSDL_SVC_SET_SVC_INTERFACE(svc->wsdl_svc, env, svc_interface);
}

axis2_wsdl_interface_t *AXIS2_CALL
axis2_svc_get_wsdl_interface(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_WSDL_SVC_GET_SVC_INTERFACE(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_engage_module(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc,
    axis2_conf_t *conf)
{
    axis2_module_desc_t *modu = NULL;
    axis2_array_list_t *collection_module = NULL;
    axis2_phase_resolver_t *phase_resolver = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    int size = 0;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
                    env, AXIS2_MODULEREF_KEY);
    if (property)
        collection_module = (axis2_array_list_t *)
                AXIS2_PROPERTY_GET_VALUE(property, env);
    if (!collection_module)
    {
        return AXIS2_FAILURE;
    }
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE(env->error);
    }
    for (i = 0; i < size; i++)
    {
        modu = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(collection_module,
                env, i);
        if (AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_QNAME(modu, env), env,
                AXIS2_MODULE_DESC_GET_QNAME(module_desc, env)))
        {
            /* module has already been engaged on the service. */
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

    }

    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, env, svc,
            module_desc);
    if (status)
    {
        status = AXIS2_ARRAY_LIST_ADD(collection_module, env, module_desc);
    }
    if (phase_resolver)
    {
        AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
        phase_resolver = NULL;
    }

    return status;
}

axis2_status_t AXIS2_CALL
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
    axis2_array_list_t *params = NULL;
    axis2_param_t *param = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    map = AXIS2_MODULE_DESC_GET_ALL_OPS(module_desc, env);
    pr = axis2_phase_resolver_create_with_config_and_svc(env, conf, svc);

    if (!pr)
    {
        return AXIS2_FAILURE;
    }

    for (index = axis2_hash_first (map, env); index; index =
                axis2_hash_next (env, index))
    {
        int size = 0;
        int j = 0;
        void *v = NULL;
        axis2_hash_this (index, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        params = AXIS2_OP_GET_ALL_PARAMS(op_desc, env);
        /* adding WSA mapping into service */
        size = AXIS2_ARRAY_LIST_SIZE(params, env);

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
            axis2_char_t *key = NULL;

            param = (axis2_param_t *) AXIS2_ARRAY_LIST_GET(params, env, j);
            if (0 == AXIS2_STRCMP( AXIS2_PARAM_GET_NAME(param, env),
                    AXIS2_WSA_MAPPING))
            {
                key = (axis2_char_t *) AXIS2_PARAM_GET_VALUE(param, env);
                status = axis2_svc_add_mapping(svc, env, key, op_desc);
                if (AXIS2_SUCCESS != status)
                {
                    if (pr)
                    {
                        AXIS2_PHASE_RESOLVER_FREE(pr, env);
                        pr = NULL;
                    }
                    return status;
                }
                break;
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

axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_module_desc_t *module_name)
{
    axis2_array_list_t *collection_module = NULL;
    axis2_module_desc_t *module_desc = NULL;
    int i = 0;
    int size = 0;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FAILURE);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
                    wsdl_component, env, AXIS2_MODULEREF_KEY);
    if (property)
        collection_module = (axis2_array_list_t *) AXIS2_PROPERTY_GET_VALUE(
                    property, env);
    if (!collection_module)
    {
        return AXIS2_FAILURE;
    }

    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);

    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE(env->error);
    }

    for (i = 0; i < size; i++)
    {
        const axis2_qname_t *module_d_name = NULL;
        const axis2_qname_t *module_d_name_l = NULL;

        module_desc = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(
                    collection_module, env, i);
        module_d_name = AXIS2_MODULE_DESC_GET_QNAME(module_desc, env);
        module_d_name_l = AXIS2_MODULE_DESC_GET_QNAME(module_name, env);

        if (AXIS2_QNAME_EQUALS(module_d_name, env, module_d_name_l))
        {
            /* module is already engaged, so return */
            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, module_name);
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_engaged_modules(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                svc->wsdl_svc->wsdl_component, env, AXIS2_MODULEREF_KEY);
    if (property)
        return (axis2_array_list_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

void *AXIS2_CALL
axis2_svc_get_wsdl_op(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *op_name)
{
    axis2_wsdl_interface_t *svc_interface = NULL;
    axis2_char_t *op_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_name, NULL);

    op_str = AXIS2_QNAME_GET_LOCALPART(op_name, env);
    svc_interface = axis2_svc_get_wsdl_interface(svc, env);
    if (svc_interface)
    {
        return AXIS2_WSDL_INTERFACE_GET_OP(svc_interface, env, op_str) ;
    }
    else
        return NULL;

}

axis2_status_t AXIS2_CALL
axis2_svc_set_style(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *style)
{
    axis2_char_t *style_l = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, style, AXIS2_FAILURE);

    style_l = AXIS2_STRDUP(style, env);
    if (!style_l)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    property = axis2_property_create(env);

    AXIS2_PROPERTY_SET_VALUE(property, env, style_l);

    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_STYLE_KEY, property);
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_style(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                svc->wsdl_svc->wsdl_component, env, AXIS2_STYLE_KEY);
    if (property)
        return (axis2_char_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

axis2_flow_t *AXIS2_CALL
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

axis2_status_t AXIS2_CALL
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

axis2_flow_t *AXIS2_CALL
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

axis2_status_t AXIS2_CALL
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

axis2_flow_t *AXIS2_CALL
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

axis2_status_t AXIS2_CALL
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

axis2_flow_t *AXIS2_CALL
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

axis2_status_t AXIS2_CALL
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

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_by_soap_action(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *soap_action)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *endpoints = NULL;
    axis2_hash_index_t *index = NULL;
    void *value = NULL;
    axis2_wsdl_endpoint_t *endpoint = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_action, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    if (0 == AXIS2_STRCMP(soap_action, ""))
        return NULL;

    endpoints = axis2_svc_get_all_endpoints(svc, env);
    if (!endpoints)
    {
        return NULL;
    }
    index = axis2_hash_first (endpoints, env);
    if (NULL != index)
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

axis2_op_t *AXIS2_CALL
axis2_svc_get_op_by_soap_action_and_endpoint(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *soap_action,
    const axis2_qname_t *endpoint)
{
    axis2_svc_impl_t *svc_impl = NULL;
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

    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    type_1 = axis2_qname_create(env, "op", AXIOM_SOAP_11_OP, NULL);
    wsdl_endpoint = axis2_svc_get_endpoint(svc, env, endpoint);
    wsdl_binding = AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, env);
    binding_ops = AXIS2_WSDL_BINDING_GET_BINDING_OPS(wsdl_binding, env);
    if (NULL == binding_ops)
    {
        return NULL;
    }
    index_i = axis2_hash_first (binding_ops, env);
    while (NULL != index_i)
    {
        int j = 0;
        int size = 0;
        const void *k = NULL;
        axis2_status_t status = AXIS2_FAILURE;

        axis2_hash_this (index_i, &k, NULL, NULL);
        binding_op = (axis2_wsdl_binding_op_t *) k;
        extensiblity_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
                    binding_op->extensible_component->wsdl_component, env);

        size = AXIS2_LINKED_LIST_SIZE(extensiblity_elements, env);
        status = AXIS2_ERROR_GET_STATUS_CODE(env->error);
        if (AXIS2_SUCCESS != status)
        {
            return NULL;
        }
        while (j < size)
        {
            axis2_qname_t *type_2 = NULL;

            element = (axis2_wsdl_soap_op_t *) AXIS2_LINKED_LIST_GET(
                        extensiblity_elements, env, j);

            type_2 = AXIS2_WSDL_EXTENSIBLE_ELEMENT_GET_TYPE(element->
                    extensible_element, env);
            if (AXIS2_QNAME_EQUALS(type_2, env, type_1))
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
        index_i = axis2_hash_next (env, index_i);
    }
    if (1 == count)
    {
        return op;
    }
    AXIS2_QNAME_FREE(type_1, env);
    type_1 = NULL;
    return NULL;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_svc_impl_t *svc_impl = NULL;
    const axis2_qname_t *qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    if (svc_impl->axis_svc_name)
        return svc_impl->axis_svc_name;

    qname = AXIS2_WSDL_SVC_GET_QNAME(svc->wsdl_svc, env);
    if (qname)
    {
        return AXIS2_QNAME_GET_LOCALPART(qname, env);
    }

    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_name(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *axis_svc_name)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, axis_svc_name, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    if (svc_impl->axis_svc_name)
    {
        AXIS2_FREE(env->allocator, svc_impl->axis_svc_name);
        svc_impl->axis_svc_name = NULL;
    }
    svc_impl->axis_svc_name = AXIS2_STRDUP(axis_svc_name, env);
    if (!svc_impl->axis_svc_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
axis2_svc_get_last_update(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(svc)->last_update;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_file_name(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(svc)->filename;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_file_name(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *filename)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, filename, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (svc_impl->filename)
    {
        AXIS2_FREE(env->allocator, svc_impl->filename);
        svc_impl->filename = NULL;
    }
    svc_impl->filename = (axis2_char_t *) AXIS2_STRDUP(filename, env);
    if (!svc_impl->filename)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_svc_get_all_endpoints(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_WSDL_SVC_GET_ENDPOINTS(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_all_endpoints(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *endpoints)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINTS(svc->wsdl_svc, env, endpoints);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_wsdl_endpoint_t *endpoint)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINT(svc->wsdl_svc, env, endpoint);
}

axis2_wsdl_endpoint_t *AXIS2_CALL
axis2_svc_get_endpoint(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    return AXIS2_WSDL_SVC_GET_ENDPOINT(svc->wsdl_svc, env, qname);
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_namespace(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    return AXIS2_WSDL_SVC_GET_NAMESPACE(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *mapping_key ,
    axis2_op_t *op_desc)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, mapping_key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    axis2_hash_set(svc_impl->op_action_map, mapping_key,
        AXIS2_HASH_KEY_STRING, op_desc);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_module_qname(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *module_qname)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_qname_t *qmodule_qname_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_qname, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    qmodule_qname_l = AXIS2_QNAME_CLONE(module_qname, env);
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(svc)->module_list, env,
            qmodule_qname_l);
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_module_qnames(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(svc)->module_list;
}

axis2_bool_t AXIS2_CALL
axis2_svc_is_schema_location_adjusted(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(svc)->schema_loc_adjusted;
}


axis2_status_t AXIS2_CALL
axis2_svc_set_schema_location_adjusted(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_bool_t adjusted)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(svc)->schema_loc_adjusted = adjusted;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_svc_get_schema_mapping_table(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->schema_mapping_table;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_mapping_table(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *table)
{
    axis2_svc_impl_t *svc_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, table, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->schema_mapping_table)
    {
        axis2_hash_free(svc_impl->schema_mapping_table, env);
        svc_impl->schema_mapping_table = NULL;
    }
    svc_impl->schema_mapping_table = table;
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_custom_schema_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->custom_schema_name_prefix;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_custom_schema_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix)
{
    axis2_svc_impl_t *svc_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->custom_schema_name_prefix)
    {
        AXIS2_FREE(env->allocator, svc_impl->custom_schema_name_prefix);
        svc_impl->custom_schema_name_prefix = NULL;
    }
    svc_impl->custom_schema_name_prefix = AXIS2_STRDUP(prefix, env);
    if (NULL == svc_impl->custom_schema_name_prefix)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


const axis2_char_t *AXIS2_CALL
axis2_svc_get_custom_schema_suffix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->custom_schema_name_suffix;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_custom_schema_suffix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *suffix)
{
    axis2_svc_impl_t *svc_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->custom_schema_name_suffix)
    {
        AXIS2_FREE(env->allocator, svc_impl->custom_schema_name_suffix);
        svc_impl->custom_schema_name_suffix = NULL;
    }
    svc_impl->custom_schema_name_suffix = AXIS2_STRDUP(suffix, env);
    if (NULL == svc_impl->custom_schema_name_suffix)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_print_schema(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_stream_t *out_stream)
{
    axis2_svc_impl_t *svc_impl = NULL;
    int size = 0;
    int i = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL == svc_impl->schema_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_EMPTY_SCHEMA_LIST,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    size = AXIS2_ARRAY_LIST_SIZE(svc_impl->schema_list, env);
    for (i = 0; i < size; i++)
    {
        xml_schema_t *schema = NULL;
        axis2_char_t *buffer = NULL;
        schema = AXIS2_ARRAY_LIST_GET(svc_impl->schema_list, env, i);
        buffer = XML_SCHEMA_SERIALIZE(schema, env);
        if (NULL != buffer)
        {
            AXIS2_STREAM_WRITE(out_stream, env, buffer, AXIS2_STRLEN(buffer));
        }
        AXIS2_FREE(env->allocator, buffer);
    }
    return AXIS2_SUCCESS;
}

xml_schema_t *AXIS2_CALL
axis2_svc_get_schema(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    int index)
{
    AXIS2_ENV_CHECK(env, NULL);
    return axis2_svc_add_all_namespaces((axis2_svc_t *) svc, env, index);
}

xml_schema_t *AXIS2_CALL
axis2_svc_add_all_namespaces(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    int index)
{
    xml_schema_t *schema = NULL;
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *prefix_map = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL == svc_impl->schema_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_EMPTY_SCHEMA_LIST,
                AXIS2_FAILURE);
        return NULL;
    }
    schema = (xml_schema_t *)AXIS2_ARRAY_LIST_GET(svc_impl->schema_list, env, index);
    prefix_map = XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(schema, env);
    if (NULL == prefix_map)
    {
        return NULL;
    }
    for (hi = axis2_hash_first (svc_impl->ns_map, env); NULL != hi;
            hi = axis2_hash_next (env, hi))
    {
        void * value = NULL;
        void *key = NULL;
        axis2_hash_this (hi, (const void **)&key, NULL, (void **)&value);
        if (NULL != value)
        {
            axis2_hash_set(prefix_map, key, AXIS2_HASH_KEY_STRING, value);
        }
    }
    return schema;
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_all_schemas(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->schema_list;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_schema(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL == svc_impl->schema_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_EMPTY_SCHEMA_LIST,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_ARRAY_LIST_ADD(svc_impl->schema_list, env, schema);
    return axis2_svc_add_schema_ns(svc, env, XML_SCHEMA_GET_TARGET_NAMESPACE(
                schema, env));
}

axis2_status_t AXIS2_CALL
axis2_svc_add_all_schemas(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_array_list_t *schemas)
{
    int i = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schemas, AXIS2_FAILURE);

    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(schemas, env); i++)
    {
        xml_schema_t *schema = AXIS2_ARRAY_LIST_GET(schemas, env, i);
        axis2_svc_add_schema(svc, env, schema);
    }
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_schema_target_ns(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->schema_target_ns;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_target_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->schema_target_ns)
    {
        AXIS2_FREE(env->allocator, svc_impl->schema_target_ns);
        svc_impl->schema_target_ns = NULL;
    }
    svc_impl->schema_target_ns = AXIS2_STRDUP(ns, env);
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_schema_target_ns_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->schema_target_ns_prefix;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_schema_target_ns_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, prefix, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->schema_target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc_impl->schema_target_ns_prefix);
        svc_impl->schema_target_ns_prefix = NULL;
    }
    svc_impl->schema_target_ns_prefix = AXIS2_STRDUP(prefix, env);
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->target_ns;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->target_ns)
    {
        AXIS2_FREE(env->allocator, svc_impl->target_ns);
        svc_impl->target_ns = NULL;
    }
    svc_impl->target_ns = AXIS2_STRDUP(ns, env);
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_svc_get_target_ns_prefix(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->target_ns_prefix;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_target_ns_prefix(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *prefix)
{
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, prefix, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->target_ns_prefix)
    {
        AXIS2_FREE(env->allocator, svc_impl->target_ns_prefix);
        svc_impl->target_ns_prefix = NULL;
    }
    svc_impl->target_ns_prefix = AXIS2_STRDUP(prefix, env);
    return AXIS2_SUCCESS;
}

xml_schema_element_t *AXIS2_CALL
axis2_svc_get_schema_element(
    const axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_svc_impl_t *svc_impl = NULL;
    int i = 0;

    AXIS2_ENV_CHECK(env, NULL);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(svc_impl->schema_list, env); i++)
    {
        xml_schema_t *schema = NULL;
        xml_schema_element_t *schema_element = NULL;
        schema = AXIS2_ARRAY_LIST_GET(svc_impl->schema_list, env, i);
        if (NULL != schema)
        {
            schema_element = XML_SCHEMA_GET_ELEMENT_BY_QNAME(schema, env,
                    qname);
            if (NULL != schema_element)
            {
                return schema_element;
            }
        }
    }
    return NULL;
}

axis2_hash_t *AXIS2_CALL
axis2_svc_get_ns_map(
    const axis2_svc_t *svc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc)->ns_map;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_ns_map(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    axis2_hash_t *ns_map)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns_map, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    if (NULL != svc_impl->ns_map)
    {
        for (hi = axis2_hash_first (svc_impl->ns_map, env); NULL != hi;
                hi = axis2_hash_next (env, hi))
        {
            void * value = NULL;
            void *key = NULL;
            axis2_hash_this (hi, (const void **)&key, NULL,
                    (void **)&value);
            if (NULL != key)
            {
                AXIS2_FREE(env->allocator, key);
                key = NULL;
            }
            if (NULL != value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        axis2_hash_free(svc_impl->ns_map, env);
    }
    svc_impl->ns_map = ns_map;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_schema_ns(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    const axis2_char_t *ns)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_bool_t found = AXIS2_FALSE;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if (NULL != svc_impl->ns_map && axis2_hash_count(svc_impl->ns_map) > 0)
    {
        for (hi = axis2_hash_first (svc_impl->ns_map, env); NULL != hi;
                hi = axis2_hash_next (env, hi))
        {
            void * value = NULL;
            void *key = NULL;
            axis2_hash_this (hi, (const void **)&key, NULL,
                    (void **)&value);
            if (0 == AXIS2_STRCMP((axis2_char_t *)value, ns))
            {
                found = AXIS2_TRUE;
                break;
            }
        }
    }
    if (NULL == svc_impl->ns_map)
    {
        svc_impl->ns_map = axis2_hash_make(env);
        svc_impl->ns_map = NULL;
    }
    if (AXIS2_FALSE == found)
    {
        axis2_char_t str_count[32];
        axis2_char_t *tmp_val = NULL;
        axis2_char_t *key = NULL;

        tmp_val = AXIS2_STRDUP(ns, env);
        sprintf(str_count, "%d", svc_impl->ns_count);
        key = AXIS2_STRACAT("ns", str_count, env);
        axis2_hash_set(svc_impl->ns_map, key, AXIS2_HASH_KEY_STRING, tmp_val);
        svc_impl->ns_count++;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_populate_schema_mappings(
    axis2_svc_t *svc,
    const axis2_env_t *env)
{
    axis2_array_list_t *schemas = NULL;
    axis2_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
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

        count = AXIS2_ARRAY_LIST_SIZE(schemas, env);
        /* calculate unique names for the schemas */
        for (i = 0; i < count; i++)
        {
            xml_schema_t *schema = AXIS2_ARRAY_LIST_GET(schemas, env, i);
            axis2_svc_calculate_schema_names(svc, env, schema, name_table);
        }
        /* adjust the schema locations as per the calculated names */
        for (i = 0; i < count; i++)
        {
            xml_schema_t *schema = AXIS2_ARRAY_LIST_GET(schemas, env, i);
            axis2_svc_adjust_schema_names(svc, env, schema, name_table);
        }
        /* reverse the name table so that there is a mapping from the
         * name to the schema
         */
        schema_table = axis2_svc_swap_mapping_table(svc, env, name_table);
        axis2_svc_set_schema_mapping_table(svc, env, schema_table);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_calculate_schema_names(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axis2_hash_t *name_table)
{
    axis2_svc_impl_t *svc_impl = NULL;
    xml_schema_obj_collection_t *collection = NULL;
    xml_schema_obj_t *schema_obj = NULL;
    int count = 0;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name_table, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
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
            if (NULL != schema1)
            {
                axis2_char_t *name = NULL;
                axis2_char_t str_count[32];
                sprintf(str_count, "%d", svc_impl->sc_calc_count++);
                axis2_svc_calculate_schema_names(svc, env, schema1, name_table);
                name = axis2_strcat(env, "xsd", str_count,
                        svc_impl->custom_schema_name_suffix, NULL);
                axis2_hash_set(name_table, schema1, AXIS2_HASH_KEY_STRING,
                        name);
            }
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_adjust_schema_names(
    axis2_svc_t *svc,
    const axis2_env_t *env,
    xml_schema_t *schema,
    axis2_hash_t *names)
{
    axis2_svc_impl_t *svc_impl = NULL;
    xml_schema_obj_collection_t *collection = NULL;
    xml_schema_obj_t *schema_obj = NULL;
    int count = 0;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, names, AXIS2_FAILURE);

    svc_impl = AXIS2_INTF_TO_IMPL(svc);
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
            if (NULL != schema1)
            {
                axis2_char_t *name = NULL;
                axis2_char_t *value = NULL;

                axis2_svc_adjust_schema_names(svc, env, schema1, names);
                value = (axis2_char_t *) axis2_hash_get(names, schema1,
                        AXIS2_HASH_KEY_STRING);
                if (NULL != value)
                {
                    if (NULL != svc_impl->custom_schema_name_prefix)
                    {
                        name = axis2_strcat(env,
                                svc_impl->custom_schema_name_prefix, value, NULL);
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
}

axis2_hash_t *AXIS2_CALL
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

    for (hi = axis2_hash_first(orig_table, env); NULL != env;
            hi = axis2_hash_next(env, hi))
    {
        void * value = NULL;
        void *key = NULL;

        axis2_hash_this (hi, (const void **)&key, NULL,
                (void **)&value);
        axis2_hash_set(new_table, value, AXIS2_HASH_KEY_STRING, key);
    }
    return new_table;
}

