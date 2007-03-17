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
 
#include <w2c_engine_configuration.h>
#include <w2c_engine_config_loader.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <w2c_qname2name_maker.h>

/** 
 * @brief
 */
typedef struct w2c_engine_configuration_impl
{
    w2c_engine_configuration_t engine_configuration;

    /** Base URI */
    axis2_char_t *base_uri;
    axis2_char_t *wsdl_uri;
    /** path to the repository - used for evaluating policy */
    axis2_char_t *repository_path;
    /** Mapping file including the qname to type map */
    axis2_char_t *type_mapping_file;
    axis2_hash_t *uri2packagemap;
    axis2_bool_t params_wrapped;

    axis2_char_t *output_location;

    /* get the defaults for these from the property file */
    axis2_char_t *output_language;
    axis2_char_t *databinding_type;
    axis2_bool_t advanced_codegen_enabled;

    axis2_bool_t async_on;
    axis2_bool_t sync_on;
    axis2_bool_t server_side;
    axis2_bool_t generate_deployement_descriptor; 
    axis2_bool_t write_testcase;
    axis2_bool_t write_message_receiver;
    axis2_char_t *package_name;

    /**
     * Default packClasses is true, which means the classes generated
     * by default are wrapped. The effect of this setting will be controlled
     * to some extent, by the other settings as well.
     */
    axis2_bool_t pack_classes;
    axis2_bool_t generate_all;

    /* user selected portname */
    axis2_char_t *port_name;
    /* user selected servicename */
    axis2_char_t *service_name;

    /*option to generate server side interface or not*/
    axis2_bool_t serverside_interface;

    /**
     * A hashmap to hang the property objects
     */
    axis2_hash_t *policy_map;

    /*
    * A hashmap of properties that may be populated on the way. extensions can populate it
    * This can be used to keep non specific information
    */
    axis2_hash_t *configuration_properties;
   
    axis2_char_t *wsdl_version;
    axis2_bool_t flatten_files;

    w2c_typemapper_t *typemapper;

    /** c specific names */
    axis2_bool_t convert_names;
    axis2_bool_t indent_code;
    axis2_array_list_t *filenames;

    w2c_qname2name_maker_t *qname2name;
    W2C_ENGINE_CONFIGURATION_NAMEMAKER namemaker_func; 
       
} w2c_engine_configuration_impl_t;

#define W2C_INTF_TO_IMPL(engine_configuration) \
    ((w2c_engine_configuration_impl_t *) engine_configuration)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_engine_configuration_free (w2c_engine_configuration_t *engine_configuration, 
           const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_base_uri (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_wsdl_uri (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_repository_path (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_type_mapping_file (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_hash_t* AXIS2_CALL
w2c_engine_configuration_get_uri2packagemap (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_params_wrapped (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_output_location (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_output_language (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_databinding_type (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_advanced_codegen_enabled (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_async_on (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_sync_on (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_server_side (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_generate_deployement_descriptor (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_write_testcase (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_write_message_receiver (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_package_name (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_pack_classes (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_generate_all (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_port_name (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_service_name (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_serverside_interface (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_hash_t* AXIS2_CALL
w2c_engine_configuration_get_policy_map (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_hash_t* AXIS2_CALL
w2c_engine_configuration_get_configuration_properties (
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_wsdl_version(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_flatten_files(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

w2c_typemapper_t* AXIS2_CALL
w2c_engine_configuration_get_typemapper(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_convert_names(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

W2C_ENGINE_CONFIGURATION_NAMEMAKER AXIS2_CALL
w2c_engine_configuration_get_namemaker_func(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

w2c_qname2name_maker_t* AXIS2_CALL
w2c_engine_configuration_get_qname2name(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_indent_code(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env );



/** setter methods */

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_base_uri (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* base_uri );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_wsdl_uri (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* wsdl_uri );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_repository_path (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* repository_path );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_type_mapping_file (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* type_mapping_file );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_uri2packagemap (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_hash_t* uri2packagemap );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_params_wrapped (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t params_wrapped );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_output_location (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* output_location );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_output_language (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* output_language );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_databinding_type (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* databinding_type );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_advanced_codegen_enabled (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t advanced_codegen_enabled );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_async_on (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t async_on );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_sync_on (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t sync_on );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_server_side (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t server_side );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_generate_deployement_descriptor (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t generate_deployement_descriptor );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_write_testcase (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t write_testcase );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_write_message_receiver (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t write_message_receiver );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_package_name (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* package_name );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_pack_classes (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t pack_classes );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_generate_all (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t generate_all );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_port_name (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* port_name );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_service_name (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* service_name );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_serverside_interface (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t serverside_interface );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_policy_map (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_hash_t* policy_map );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_configuration_properties (
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_hash_t* configuration_properties );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_wsdl_version(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t* wsdl_version );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_flatten_files(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t flatten_files );

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_typemapper(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           w2c_typemapper_t *typemapper);

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_convert_names(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t convert_names);

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_namemaker_func(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           W2C_ENGINE_CONFIGURATION_NAMEMAKER namemaker_func);

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_qname2name(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           w2c_qname2name_maker_t *qname2name);

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_indent_code(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t indent_code);

axis2_status_t AXIS2_CALL
w2c_engine_configuration_add_filename(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t *filename);

axis2_array_list_t *AXIS2_CALL
w2c_engine_configuration_get_filenames(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env);



/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_engine_configuration_t * AXIS2_CALL 
w2c_engine_configuration_create (const axis2_env_t *env)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    engine_configuration_impl = (w2c_engine_configuration_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_engine_configuration_impl_t));

    if(NULL == engine_configuration_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
 
    engine_configuration_impl->base_uri = NULL;
    engine_configuration_impl->wsdl_uri = NULL;
    engine_configuration_impl->repository_path = NULL;
    engine_configuration_impl->type_mapping_file = NULL;
    engine_configuration_impl->output_location = NULL;
    engine_configuration_impl->output_language = NULL;
    engine_configuration_impl->databinding_type = NULL;
    engine_configuration_impl->package_name = NULL;
    engine_configuration_impl->port_name = NULL;
    engine_configuration_impl->service_name = NULL;
    engine_configuration_impl->policy_map = NULL;
    engine_configuration_impl->configuration_properties = NULL;
    engine_configuration_impl->uri2packagemap = NULL;
    engine_configuration_impl->wsdl_version = NULL;
    engine_configuration_impl->typemapper = NULL;
    engine_configuration_impl->qname2name = NULL;
    engine_configuration_impl->namemaker_func = NULL; 
   
    engine_configuration_impl->serverside_interface = AXIS2_FALSE;
    engine_configuration_impl->pack_classes = AXIS2_FALSE;
    engine_configuration_impl->generate_all = AXIS2_FALSE;
    engine_configuration_impl->advanced_codegen_enabled = AXIS2_FALSE;
    engine_configuration_impl->async_on = AXIS2_FALSE;
    engine_configuration_impl->sync_on = AXIS2_FALSE;
    engine_configuration_impl->server_side = AXIS2_FALSE;
    engine_configuration_impl->generate_deployement_descriptor = AXIS2_FALSE;
    engine_configuration_impl->write_testcase = AXIS2_FALSE;
    engine_configuration_impl->write_message_receiver = AXIS2_FALSE;
    engine_configuration_impl->params_wrapped = AXIS2_FALSE;
    engine_configuration_impl->flatten_files = AXIS2_FALSE;
    engine_configuration_impl->convert_names = AXIS2_FALSE;
    engine_configuration_impl->indent_code = AXIS2_FALSE;

    engine_configuration_impl->filenames = axis2_array_list_create(env, 10);

    engine_configuration_impl->engine_configuration.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_engine_configuration_ops_t));
    if(NULL == engine_configuration_impl->engine_configuration.ops)
    {
        w2c_engine_configuration_free(&(engine_configuration_impl->engine_configuration), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    engine_configuration_impl->engine_configuration.ops->free = w2c_engine_configuration_free;
    engine_configuration_impl->engine_configuration.ops->get_base_uri =
                                                w2c_engine_configuration_get_base_uri;
    engine_configuration_impl->engine_configuration.ops->set_base_uri =
                                                w2c_engine_configuration_set_base_uri;
    engine_configuration_impl->engine_configuration.ops->get_wsdl_uri =
                                                w2c_engine_configuration_get_wsdl_uri;
    engine_configuration_impl->engine_configuration.ops->set_wsdl_uri =
                                                w2c_engine_configuration_set_wsdl_uri;
    engine_configuration_impl->engine_configuration.ops->get_repository_path =
                                                w2c_engine_configuration_get_repository_path;
    engine_configuration_impl->engine_configuration.ops->set_repository_path =
                                                w2c_engine_configuration_set_repository_path;
    engine_configuration_impl->engine_configuration.ops->get_type_mapping_file =
                                                w2c_engine_configuration_get_type_mapping_file;
    engine_configuration_impl->engine_configuration.ops->set_type_mapping_file =
                                                w2c_engine_configuration_set_type_mapping_file;
    engine_configuration_impl->engine_configuration.ops->get_uri2packagemap =
                                                w2c_engine_configuration_get_uri2packagemap;
    engine_configuration_impl->engine_configuration.ops->set_uri2packagemap =
                                                w2c_engine_configuration_set_uri2packagemap;
    engine_configuration_impl->engine_configuration.ops->get_params_wrapped =
                                                w2c_engine_configuration_get_params_wrapped;
    engine_configuration_impl->engine_configuration.ops->set_params_wrapped =
                                                w2c_engine_configuration_set_params_wrapped;
    engine_configuration_impl->engine_configuration.ops->get_output_location =
                                                w2c_engine_configuration_get_output_location;
    engine_configuration_impl->engine_configuration.ops->set_output_location =
                                                w2c_engine_configuration_set_output_location;
    engine_configuration_impl->engine_configuration.ops->get_output_language =
                                                w2c_engine_configuration_get_output_language;
    engine_configuration_impl->engine_configuration.ops->set_output_language =
                                                w2c_engine_configuration_set_output_language;
    engine_configuration_impl->engine_configuration.ops->get_databinding_type =
                                                w2c_engine_configuration_get_databinding_type;
    engine_configuration_impl->engine_configuration.ops->set_databinding_type =
                                                w2c_engine_configuration_set_databinding_type;
    engine_configuration_impl->engine_configuration.ops->get_advanced_codegen_enabled =
                                                w2c_engine_configuration_get_advanced_codegen_enabled;
    engine_configuration_impl->engine_configuration.ops->set_advanced_codegen_enabled =
                                                w2c_engine_configuration_set_advanced_codegen_enabled;
    engine_configuration_impl->engine_configuration.ops->get_async_on =
                                                w2c_engine_configuration_get_async_on;
    engine_configuration_impl->engine_configuration.ops->set_async_on =
                                                w2c_engine_configuration_set_async_on;
    engine_configuration_impl->engine_configuration.ops->get_sync_on =
                                                w2c_engine_configuration_get_sync_on;
    engine_configuration_impl->engine_configuration.ops->set_sync_on =
                                                w2c_engine_configuration_set_sync_on;
    engine_configuration_impl->engine_configuration.ops->get_server_side =
                                                w2c_engine_configuration_get_server_side;
    engine_configuration_impl->engine_configuration.ops->set_server_side =
                                                w2c_engine_configuration_set_server_side;
    engine_configuration_impl->engine_configuration.ops->get_generate_deployement_descriptor =
                                                w2c_engine_configuration_get_generate_deployement_descriptor; 
    engine_configuration_impl->engine_configuration.ops->set_generate_deployement_descriptor =
                                                w2c_engine_configuration_set_generate_deployement_descriptor; 
    engine_configuration_impl->engine_configuration.ops->get_write_testcase =
                                                w2c_engine_configuration_get_write_testcase;
    engine_configuration_impl->engine_configuration.ops->set_write_testcase =
                                                w2c_engine_configuration_set_write_testcase;
    engine_configuration_impl->engine_configuration.ops->get_write_message_receiver =
                                                w2c_engine_configuration_get_write_message_receiver;
    engine_configuration_impl->engine_configuration.ops->set_write_message_receiver =
                                                w2c_engine_configuration_set_write_message_receiver;
    engine_configuration_impl->engine_configuration.ops->get_package_name =
                                                w2c_engine_configuration_get_package_name;
    engine_configuration_impl->engine_configuration.ops->set_package_name =
                                                w2c_engine_configuration_set_package_name;
    engine_configuration_impl->engine_configuration.ops->get_pack_classes =
                                                w2c_engine_configuration_get_pack_classes;
    engine_configuration_impl->engine_configuration.ops->set_pack_classes =
                                                w2c_engine_configuration_set_pack_classes;
    engine_configuration_impl->engine_configuration.ops->get_generate_all =
                                                w2c_engine_configuration_get_generate_all;
    engine_configuration_impl->engine_configuration.ops->set_generate_all =
                                                w2c_engine_configuration_set_generate_all;
    engine_configuration_impl->engine_configuration.ops->get_port_name =
                                                w2c_engine_configuration_get_port_name;
    engine_configuration_impl->engine_configuration.ops->set_port_name =
                                                w2c_engine_configuration_set_port_name;
    engine_configuration_impl->engine_configuration.ops->get_service_name =
                                                w2c_engine_configuration_get_service_name;
    engine_configuration_impl->engine_configuration.ops->set_service_name =
                                                w2c_engine_configuration_set_service_name;
    engine_configuration_impl->engine_configuration.ops->get_serverside_interface =
                                                w2c_engine_configuration_get_serverside_interface;
    engine_configuration_impl->engine_configuration.ops->set_serverside_interface =
                                                w2c_engine_configuration_set_serverside_interface;
    engine_configuration_impl->engine_configuration.ops->get_policy_map =
                                                w2c_engine_configuration_get_policy_map;
    engine_configuration_impl->engine_configuration.ops->set_policy_map =
                                                w2c_engine_configuration_set_policy_map;
    engine_configuration_impl->engine_configuration.ops->get_configuration_properties =
                                                w2c_engine_configuration_get_configuration_properties;
    engine_configuration_impl->engine_configuration.ops->set_configuration_properties =
                                                w2c_engine_configuration_set_configuration_properties;
    engine_configuration_impl->engine_configuration.ops->get_wsdl_version=
                                                w2c_engine_configuration_get_wsdl_version;
    engine_configuration_impl->engine_configuration.ops->set_wsdl_version=
                                                w2c_engine_configuration_set_wsdl_version;
    engine_configuration_impl->engine_configuration.ops->get_flatten_files=
                                                w2c_engine_configuration_get_flatten_files;
    engine_configuration_impl->engine_configuration.ops->set_flatten_files=
                                                w2c_engine_configuration_set_flatten_files;
    engine_configuration_impl->engine_configuration.ops->get_typemapper=
                                                w2c_engine_configuration_get_typemapper;
    engine_configuration_impl->engine_configuration.ops->set_typemapper=
                                                w2c_engine_configuration_set_typemapper;
    engine_configuration_impl->engine_configuration.ops->get_convert_names=
                                                w2c_engine_configuration_get_convert_names;
    engine_configuration_impl->engine_configuration.ops->set_convert_names=
                                                w2c_engine_configuration_set_convert_names;
    engine_configuration_impl->engine_configuration.ops->get_namemaker_func=
                                                w2c_engine_configuration_get_namemaker_func;
    engine_configuration_impl->engine_configuration.ops->set_namemaker_func=
                                                w2c_engine_configuration_set_namemaker_func;
    engine_configuration_impl->engine_configuration.ops->get_qname2name =
                                                w2c_engine_configuration_get_qname2name;
    engine_configuration_impl->engine_configuration.ops->set_qname2name =
                                                w2c_engine_configuration_set_qname2name;
    engine_configuration_impl->engine_configuration.ops->get_indent_code =
                                                w2c_engine_configuration_get_indent_code;
    engine_configuration_impl->engine_configuration.ops->set_indent_code =
                                                w2c_engine_configuration_set_indent_code;
    engine_configuration_impl->engine_configuration.ops->add_filename =
                                                w2c_engine_configuration_add_filename;
    engine_configuration_impl->engine_configuration.ops->get_filenames=
                                                w2c_engine_configuration_get_filenames;

    return &(engine_configuration_impl->engine_configuration);
}

AXIS2_EXTERN w2c_engine_configuration_t * AXIS2_CALL 
w2c_engine_configuration_create_with_options (const axis2_env_t *env,
                     axis2_hash_t* options)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    engine_configuration_impl = (w2c_engine_configuration_impl_t *) 
                     w2c_engine_configuration_create ( env);

    if(NULL == engine_configuration_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    w2c_engine_config_loader_load_config (env, 
            &(engine_configuration_impl-> engine_configuration),
            options );

    return &(engine_configuration_impl->engine_configuration);
}
/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_engine_configuration_free (w2c_engine_configuration_t *engine_configuration, 
                            const axis2_env_t *env)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration);
    
    if ( engine_configuration_impl->filenames)
    {
        axis2_array_list_free( engine_configuration_impl->filenames, env);
    }
    if ( engine_configuration_impl->base_uri )
    {
		/*
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->base_uri );
		*/
	}
    if ( engine_configuration_impl->wsdl_uri )
    {
		/*
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->wsdl_uri );
		*/
    }
    if ( engine_configuration_impl->repository_path )
    {
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->repository_path );
    }
    if ( engine_configuration_impl->output_language )
    {
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->output_language );
    }
    if ( engine_configuration_impl->databinding_type )
    {
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->databinding_type );
    }
    if ( engine_configuration_impl->package_name )
    {
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->package_name );
    }
    if ( engine_configuration_impl->port_name )
    {
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->port_name );
    }
    if ( engine_configuration_impl->service_name )
    {
        AXIS2_FREE ( env-> allocator, engine_configuration_impl->service_name );
    }
    if ( engine_configuration_impl->typemapper)
    {
        W2C_TYPEMAPPER_FREE ( engine_configuration_impl->typemapper, env );
    }
    
    axis2_hash_free (engine_configuration_impl->uri2packagemap, env );
    axis2_hash_free (engine_configuration_impl->policy_map, env );


    if(engine_configuration->ops)
    {
        AXIS2_FREE(env->allocator, engine_configuration->ops);
        engine_configuration->ops = NULL;
    }
    if(engine_configuration_impl)
    {
        AXIS2_FREE(env->allocator, engine_configuration_impl);
        engine_configuration_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_base_uri (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> base_uri;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_wsdl_uri (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> wsdl_uri;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_repository_path (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> repository_path;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_type_mapping_file (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> type_mapping_file;
}

axis2_hash_t* AXIS2_CALL
w2c_engine_configuration_get_uri2packagemap (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> uri2packagemap;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_params_wrapped (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> params_wrapped;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_output_location (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> output_location;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_output_language (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> output_language;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_databinding_type (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> databinding_type;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_advanced_codegen_enabled (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> advanced_codegen_enabled;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_async_on (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> async_on;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_sync_on (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> sync_on;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_server_side (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> server_side;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_generate_deployement_descriptor (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> generate_deployement_descriptor;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_write_testcase (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> write_testcase;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_write_message_receiver (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> write_message_receiver;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_package_name (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> package_name;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_pack_classes (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> pack_classes;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_generate_all (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> generate_all;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_port_name (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> port_name;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_service_name (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> service_name;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_serverside_interface (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> serverside_interface;
}

axis2_hash_t* AXIS2_CALL
w2c_engine_configuration_get_policy_map (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> policy_map;
}

axis2_hash_t* AXIS2_CALL
w2c_engine_configuration_get_configuration_properties (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> configuration_properties;
}

axis2_char_t* AXIS2_CALL
w2c_engine_configuration_get_wsdl_version (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> wsdl_version;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_flatten_files (
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> flatten_files;
}

w2c_typemapper_t* AXIS2_CALL
w2c_engine_configuration_get_typemapper(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> typemapper;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_convert_names(
          w2c_engine_configuration_t  *engine_configuration,
          const axis2_env_t *env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE );
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> convert_names;
}

W2C_ENGINE_CONFIGURATION_NAMEMAKER AXIS2_CALL
w2c_engine_configuration_get_namemaker_func(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> namemaker_func;
}

w2c_qname2name_maker_t *AXIS2_CALL
w2c_engine_configuration_get_qname2name(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, NULL);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> qname2name;
}

axis2_bool_t AXIS2_CALL
w2c_engine_configuration_get_indent_code(
          w2c_engine_configuration_t * engine_configuration,
          const axis2_env_t* env )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FALSE);
    
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );
   
    return engine_configuration_impl-> indent_code;
}

/** setter methods */

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_base_uri (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *base_uri )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->base_uri = axis2_strdup( base_uri, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_wsdl_uri (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *wsdl_uri )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->wsdl_uri = axis2_strdup( wsdl_uri, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_repository_path (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *repository_path )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->repository_path = axis2_strdup( repository_path, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_type_mapping_file (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *type_mapping_file )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->type_mapping_file = axis2_strdup( type_mapping_file, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_uri2packagemap (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_hash_t *uri2packagemap )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->uri2packagemap = uri2packagemap;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_params_wrapped (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t params_wrapped )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->params_wrapped = params_wrapped;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_output_location (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *output_location )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->output_location = output_location;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_output_language (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *output_language )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->output_language = axis2_strdup( output_language, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_databinding_type (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *databinding_type )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->databinding_type = axis2_strdup( databinding_type, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_advanced_codegen_enabled (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t advanced_codegen_enabled )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->advanced_codegen_enabled = advanced_codegen_enabled;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_async_on (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t async_on )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->async_on = async_on;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_sync_on (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t sync_on )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->sync_on = sync_on;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_server_side (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t server_side )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->server_side = server_side;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_generate_deployement_descriptor (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t generate_deployement_descriptor )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->generate_deployement_descriptor = generate_deployement_descriptor;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_write_testcase (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t write_testcase )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->write_testcase = write_testcase;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_write_message_receiver (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t write_message_receiver )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->write_message_receiver = write_message_receiver;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_package_name (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *package_name )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->package_name = axis2_strdup( package_name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_pack_classes (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t pack_classes )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->pack_classes = pack_classes;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_generate_all (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t generate_all )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->generate_all = generate_all;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_port_name (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *port_name )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->port_name = axis2_strdup( port_name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_service_name (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *service_name )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->service_name = axis2_strdup( service_name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_serverside_interface (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t serverside_interface )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->serverside_interface = serverside_interface;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_policy_map (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_hash_t *policy_map )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->policy_map = policy_map;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_configuration_properties (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_hash_t *configuration_properties )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->configuration_properties = configuration_properties;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_wsdl_version (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_char_t *wsdl_version )
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->wsdl_version = axis2_strdup( wsdl_version, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_flatten_files (
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t flatten_files)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->flatten_files = flatten_files;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_typemapper(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           w2c_typemapper_t *typemapper)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->typemapper = typemapper;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_convert_names(
           w2c_engine_configuration_t *engine_configuration,
           const axis2_env_t *env,
           axis2_bool_t convert_names)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->convert_names = convert_names;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_namemaker_func(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           W2C_ENGINE_CONFIGURATION_NAMEMAKER namemaker_func)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->namemaker_func = namemaker_func;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_qname2name(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           w2c_qname2name_maker_t *qname2name)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->qname2name = qname2name;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_engine_configuration_set_indent_code(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_bool_t indent_code)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    engine_configuration_impl->indent_code = indent_code;
    return AXIS2_SUCCESS;
}

/** methods other than getters and setters */
axis2_status_t AXIS2_CALL
w2c_engine_configuration_add_filename(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env,
           axis2_char_t *filename)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    axis2_array_list_add( engine_configuration_impl->filenames, env, filename);
    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
w2c_engine_configuration_get_filenames(
           w2c_engine_configuration_t* engine_configuration,
           const axis2_env_t* env)
{
    w2c_engine_configuration_impl_t *engine_configuration_impl = NULL;
    
	AXIS2_ENV_CHECK (env, NULL);
   
    engine_configuration_impl = W2C_INTF_TO_IMPL(engine_configuration );

    return engine_configuration_impl->filenames;
}
