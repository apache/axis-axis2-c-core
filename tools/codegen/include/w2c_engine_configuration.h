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
  
#ifndef W2C_ENGINE_CONFIGURATION_H
#define W2C_ENGINE_CONFIGURATION_H

/**
 * @file w2c_engine_configuration.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axutil_array_list.h>
#include <axis2_hash.h>
#include <w2c_typemapper.h>
#include <w2c_qname2name_maker.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_engine_configuration represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef axis2_char_t* (*W2C_ENGINE_CONFIGURATION_NAMEMAKER)
                  ( axis2_char_t *name, const axis2_env_t *env);
    
typedef struct w2c_engine_configuration w2c_engine_configuration_t;
typedef struct w2c_engine_configuration_ops w2c_engine_configuration_ops_t;

AXIS2_DECLARE_DATA  struct w2c_engine_configuration_ops
{
   /**
    * free w2c_engine_configuration.
    * @param  engine_configuration pointer to engine_configuration struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL * 
    free)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve base_uri.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return base_uri
     */
    axis2_char_t* (AXIS2_CALL * 
    get_base_uri)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve wsdl_uri.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return wsdl_uri
     */
    axis2_char_t* (AXIS2_CALL * 
    get_wsdl_uri)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve repository_path.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return repository_path
     */
    axis2_char_t* (AXIS2_CALL * 
    get_repository_path)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve type_mapping_file.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return type_mapping_file
     */
    axis2_char_t* (AXIS2_CALL * 
    get_type_mapping_file)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve uri2packagemap.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return uri2packagemap
     */
    axis2_hash_t* (AXIS2_CALL *
    get_uri2packagemap)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve params_wrapped.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return params_wrapped
     */
    axis2_bool_t (AXIS2_CALL *
    get_params_wrapped)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve output_location.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return output_location
     */
    axis2_char_t* (AXIS2_CALL *
    get_output_location)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve output_language.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return output_language
     */
    axis2_char_t* (AXIS2_CALL *
    get_output_language)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve databinding_type.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return databinding_type
     */
    axis2_char_t* (AXIS2_CALL *
    get_databinding_type)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve advanced_codegen_enabled.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return advanced_codegen_enabled
     */
    axis2_bool_t (AXIS2_CALL *
    get_advanced_codegen_enabled)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve async_on.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return async_on
     */
    axis2_bool_t (AXIS2_CALL *
    get_async_on)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve sync_on.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return sync_on
     */
    axis2_bool_t (AXIS2_CALL *
    get_sync_on)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve server_side.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return server_side
     */
    axis2_bool_t (AXIS2_CALL *
    get_server_side)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve generate_deployement_descriptor.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return generate_deployement_descriptor
     */
    axis2_bool_t (AXIS2_CALL *
    get_generate_deployement_descriptor)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve write_testcase.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return write_testcase
     */
    axis2_bool_t (AXIS2_CALL *
    get_write_testcase)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve write_message_receiver.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return write_message_receiver
     */
    axis2_bool_t (AXIS2_CALL *
    get_write_message_receiver)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve package_name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return package_name
     */
    axis2_char_t* (AXIS2_CALL *
    get_package_name)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve port_name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return port_name
     */
    axis2_char_t* (AXIS2_CALL *
    get_port_name)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve pack_classes.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return pack_classes
     */
    axis2_bool_t (AXIS2_CALL *
    get_pack_classes)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve generate_all.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return generate_all
     */
    axis2_bool_t (AXIS2_CALL *
    get_generate_all)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve service_name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return service_name
     */
    axis2_char_t* (AXIS2_CALL *
    get_service_name)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve serverside_interface.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return serverside_interface
     */
    axis2_bool_t (AXIS2_CALL *
    get_serverside_interface)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve policy_map.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return policy_map
     */
    axis2_hash_t* (AXIS2_CALL *
    get_policy_map)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve configuration_properties.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return configuration_properties
     */
    axis2_hash_t* (AXIS2_CALL *
    get_configuration_properties)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve wsdl_version.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return wsdl_version
     */
    axis2_char_t* (AXIS2_CALL *
    get_wsdl_version)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve flatten_files.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return flatten_files
     */
    axis2_bool_t (AXIS2_CALL *
    get_flatten_files)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );
  
   /**
    * retrieve typemapper.
    * @param engine_configuration pointer to engine configuration struct
    * @param env Environment. MUST NOT be NULL 
    * @return typemapper
    */
    w2c_typemapper_t* (AXIS2_CALL *
    get_typemapper)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve convert_names.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return convert_names
     */
    axis2_bool_t (AXIS2_CALL *
    get_convert_names)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );

    /**
     * retrieve namemaker_func.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return namemaker_func 
     */
    W2C_ENGINE_CONFIGURATION_NAMEMAKER (AXIS2_CALL *
    get_namemaker_func)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );


    /**
     * retrieve qname2name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return qname2name_func 
     */
    w2c_qname2name_maker_t* (AXIS2_CALL *
    get_qname2name)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env );


    /**
     * set base_uri.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param base_uri value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_base_uri ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *base_uri );
   
   /**
    * set wsdl_uri.
    * @param engine_configuration pointer to engine configuration struct 
    * @param env Environment. MUST NOT be NULL
    * @param wsdl_uri value to set
    * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
    */
    axis2_status_t (AXIS2_CALL* 
    set_wsdl_uri ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *wsdl_uri );

    /**
     * set repository_path.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param repository_path value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_repository_path ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *repository_path );

    /**
     * set type_mapping_file.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param type_mapping_file value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_type_mapping_file ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *type_mapping_file );

    /**
     * set uri2packagemap.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param uri2packagemap value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_uri2packagemap ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_hash_t *uri2packagemap );

    /**
     * set params_wrapped.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param params_wrapped value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_params_wrapped ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t params_wrapped );

    /**
     * set output_location.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param output_location value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_output_location ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *output_location );

    /**
     * set output_language.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param output_language value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_output_language ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *output_language );

    /**
     * set databinding_type.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param databinding_type value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_databinding_type ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *databinding_type );

    /**
     * set advanced_codegen_enabled.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param advanced_codegen_enabled value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_advanced_codegen_enabled ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t advanced_codegen_enabled );

    /**
     * set async_on.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param async_on value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_async_on ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t async_on );

    /**
     * set sync_on.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param sync_on value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_sync_on ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t sync_on );

    /**
     * set server_side.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param server_side value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_server_side ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t server_side );

    /**
     * set generate_deployement_descriptor.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param generate_deployement_descriptor value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_generate_deployement_descriptor ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t generate_deployement_descriptor );

    /**
     * set write_testcase.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param write_testcase value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_write_testcase ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t write_testcase );

    /**
     * set write_message_receiver.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param write_message_receiver value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_write_message_receiver ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t writeMessageReceiver );

    /**
     * set package_name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param package_name value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_package_name ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *package_name );

    /**
     * set port_nam.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param port_nam value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_port_name) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *package_name );

    /**
     * set pack_classes.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param pack_classes value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_pack_classes ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t packClasses );

    /**
     * set generate_all.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param generate_all value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_generate_all ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t generate_all );

    /**
     * set service_name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param service_name value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_service_name ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *service_name );

    /**
     * set serverside_interface.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param serverside_interface value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_serverside_interface ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t serverSideInterface );

    /**
     * set policy_map.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param policy_map value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_policy_map ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_hash_t *policy_map );

    /**
     * set configuration_properties.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param configuration_properties value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_configuration_properties ) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_hash_t *configuration_properties );

    /**
     * set wsdl_versio.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param wsdl_versio value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_wsdl_version) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *wsdl_version);

    /**
     * set flatten_file.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param flatten_file value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_flatten_files) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t flatten_files);
 
   /**
    * set typemappe.
    * @param engine_configuration pointer to engine configuration struct 
    * @param env Environment. MUST NOT be NULL
    * @param typemappe value to set
    * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
    */
    axis2_status_t (AXIS2_CALL *
    set_typemapper)(w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env,
                w2c_typemapper_t *typemapper );

    /**
     * set convert_name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param convert_name value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_convert_names) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t convert_names);

    /**
     * set namemaker function.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param namemaker_func value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_namemaker_func) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                W2C_ENGINE_CONFIGURATION_NAMEMAKER name_maker_func);


    /**
     * set qname2name.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param qname2name_value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_qname2name) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                w2c_qname2name_maker_t *qname2name);

    /**
     * set format code option.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @param indent_code value to set
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    set_indent_code) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_bool_t indent_code);

    /**
     * get whether format the code.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_bool_t (AXIS2_CALL* 
    get_indent_code) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env); 

    /**
     * get arraylist of generated filenames.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return array list of generated filenames.
     */
    axutil_array_list_t* (AXIS2_CALL* 
    get_filenames) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env); 


    /**
     * add generating filename.
     * @param engine_configuration pointer to engine configuration struct
     * @param env Environment. MUST NOT be NULL
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure
     */
    axis2_status_t (AXIS2_CALL* 
    add_filename) ( w2c_engine_configuration_t *engine_configuration,
                const axis2_env_t *env, 
                axis2_char_t *filename);

};

AXIS2_DECLARE_DATA  struct w2c_engine_configuration
{
     struct w2c_engine_configuration_ops* ops;
};

/**
 * create a engine_configuration obj
 * @param env Environment. MUST NOT be NULL
 * @return newly created engine_configuration obj
 */
AXIS2_EXTERN w2c_engine_configuration_t* AXIS2_CALL
w2c_engine_configuration_create( const axis2_env_t *env );

/**
 * create a engine_configuration obj with option map
 * @param env Environment. MUST NOT be NULL
 * @param options option map
 * @return newly created engine_configuration obj
 */
AXIS2_EXTERN w2c_engine_configuration_t* AXIS2_CALL
w2c_engine_configuration_create_with_options( const axis2_env_t *env,
                             axis2_hash_t *options );


/*************************** Function macros **********************************/

#define W2C_ENGINE_CONFIGURATION_FREE(engine_configuration, env) \
    ((engine_configuration)->ops->free (engine_configuration, env)) 

#define W2C_ENGINE_CONFIGURATION_GET_BASE_URI(engine_configuration, env) \
    ((engine_configuration)->ops->get_base_uri (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_BASE_URI(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_base_uri (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_WSDL_URI(engine_configuration, env) \
    ((engine_configuration)->ops->get_wsdl_uri (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_WSDL_URI(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_wsdl_uri (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_REPOSITORY_PATH(engine_configuration, env) \
    ((engine_configuration)->ops->get_repository_path (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_REPOSITORY_PATH(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_repository_path (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_TYPE_MAPPING_FILE(engine_configuration, env) \
    ((engine_configuration)->ops->get_type_mapping_file (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_TYPE_MAPPING_FILE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_type_mapping_file (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_URI2PACKAGEMAP(engine_configuration, env) \
    ((engine_configuration)->ops->get_uri2packagemap (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_URI2PACKAGEMAP(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_uri2packagemap (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_PARAMS_WRAPPED(engine_configuration, env) \
    ((engine_configuration)->ops->get_params_wrapped (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_PARAMS_WRAPPED(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_params_wrapped (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LOCATION(engine_configuration, env) \
    ((engine_configuration)->ops->get_output_location (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_OUTPUT_LOCATION(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_output_location (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE(engine_configuration, env) \
    ((engine_configuration)->ops->get_output_language (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_OUTPUT_LANGUAGE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_output_language (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_DATABINDING_TYPE(engine_configuration, env) \
    ((engine_configuration)->ops->get_databinding_type (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_DATABINDING_TYPE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_databinding_type (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_ADVANCED_CODEGEN_ENABLED(engine_configuration, env) \
    ((engine_configuration)->ops->get_advanced_codegen_enabled (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_ADVANCED_CODEGEN_ENABLED(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_advanced_codegen_enabled (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_ASYNC_ON(engine_configuration, env) \
    ((engine_configuration)->ops->get_async_on (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_ASYNC_ON(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_async_on (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_SYNC_ON(engine_configuration, env) \
    ((engine_configuration)->ops->get_sync_on (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_SYNC_ON(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_sync_on (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_SERVER_SIDE(engine_configuration, env) \
    ((engine_configuration)->ops->get_server_side (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_SERVER_SIDE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_server_side (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_GENERATE_DEPLOYEMENT_DESCRIPTOR(engine_configuration, env) \
    ((engine_configuration)->ops->get_generate_deployement_descriptor (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_GENERATE_DEPLOYEMENT_DESCRIPTOR(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_generate_deployement_descriptor (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_WRITE_TESTCASE(engine_configuration, env) \
    ((engine_configuration)->ops->get_write_testcase (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_WRITE_TESTCASE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_write_testcase (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_WRITE_MESSAGE_RECEIVER(engine_configuration, env) \
    ((engine_configuration)->ops->get_write_message_receiver (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_WRITE_MESSAGE_RECEIVER(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_write_message_receiver (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_PACKAGE_NAME(engine_configuration, env) \
    ((engine_configuration)->ops->get_package_name (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_PACKAGE_NAME(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_package_name (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_PACK_CLASSES(engine_configuration, env) \
    ((engine_configuration)->ops->get_pack_classes (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_PACK_CLASSES(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_pack_classes (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_GENERATE_ALL(engine_configuration, env) \
    ((engine_configuration)->ops->get_generate_all (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_GENERATE_ALL(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_generate_all (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_PORT_NAME(engine_configuration, env) \
    ((engine_configuration)->ops->get_port_name (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_PORT_NAME(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_port_name (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_SERVICE_NAME(engine_configuration, env) \
    ((engine_configuration)->ops->get_service_name (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_SERVICE_NAME(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_service_name (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_SERVERSIDE_INTERFACE(engine_configuration, env) \
    ((engine_configuration)->ops->get_serverside_interface (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_SERVERSIDE_INTERFACE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_serverside_interface (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_POLICY_MAP(engine_configuration, env) \
    ((engine_configuration)->ops->get_policy_map (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_POLICY_MAP(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_policy_map (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_CONFIGURATION_PROPERTIES(engine_configuration, env) \
    ((engine_configuration)->ops->get_configuration_properties (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_CONFIGURATION_PROPERTIES(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_configuration_properties (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_WSDL_VERSION(engine_configuration, env) \
    ((engine_configuration)->ops->get_wsdl_version (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_WSDL_VERSION(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_wsdl_version (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_FLATTEN_FILES(engine_configuration, env) \
    ((engine_configuration)->ops->get_flatten_files (engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_FLATTEN_FILES(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_flatten_files (engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_TYPEMAPPER(engine_configuration, env) \
    ((engine_configuration)->ops->get_typemapper(engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_TYPEMAPPER(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_typemapper(engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_CONVERT_NAMES(engine_configuration, env) \
    ((engine_configuration)->ops->get_convert_names(engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_CONVERT_NAMES(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_convert_names(engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_QNAME2NAME(engine_configuration, env) \
    ((engine_configuration)->ops->get_qname2name(engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_QNAME2NAME(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_qname2name(engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_NAMEMAKER_FUNC(engine_configuration, env) \
    ((engine_configuration)->ops->get_namemaker_func(engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_NAMEMAKER_FUNC(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_namemaker_func(engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_INDENT_CODE(engine_configuration, env) \
    ((engine_configuration)->ops->get_indent_code(engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_SET_INDENT_CODE(engine_configuration, env, val) \
    ((engine_configuration)->ops->set_indent_code(engine_configuration, env, val))

#define W2C_ENGINE_CONFIGURATION_GET_FILENAMES(engine_configuration, env) \
    ((engine_configuration)->ops->get_filenames(engine_configuration, env ))

#define W2C_ENGINE_CONFIGURATION_ADD_FILENAME(engine_configuration, env, val) \
    ((engine_configuration)->ops->add_filename(engine_configuration, env, val))



/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_ENGINE_CONFIGURATION_H */
