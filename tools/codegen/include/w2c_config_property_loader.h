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
  
#ifndef W2C_CONFIG_PROPERTY_LOADER_H
#define W2C_CONFIG_PROPERTY_LOADER_H

/**
 * @file w2c_config_property_loader.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_config_property_loader represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_config_property_loader w2c_config_property_loader_t;
typedef struct w2c_config_property_loader_ops w2c_config_property_loader_ops_t;

AXIS2_DECLARE_DATA  struct w2c_config_property_loader_ops
{
   /**
    * free w2c_config_property_loader.
    * @param  config_property_loader pointer to config_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_config_property_loader_t *config_property_loader,
         const axis2_env_t *env);

   /**
    * change the filename of the loader
    * just sets the filame should call reload to reload the properties.
    * @param  config_property_loader pointer to config_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_filename)(w2c_config_property_loader_t *config_property_loader,
         const axis2_env_t *env, axis2_char_t *filename);

    /**
    * reload the properties.
    * @param  config_property_loader pointer to config_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    reload )(w2c_config_property_loader_t *config_property_loader,
         const axis2_env_t *env);
    
    /**
     * Gets the test object support template. This is used in the
     * generated test class.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_test_object_template_name )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the databinder template name. This is the template that has the
     * logic for creating the databind supporters.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL*
    get_db_supporter_template_name )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    /**
     * Gets the extension class names.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_array_list_t*.
     */
    axis2_array_list_t* (AXIS2_CALL* 
    get_extension_class_names )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
     /**
     * Gets the post extension class names.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_array_list_t*.
     */
    axis2_array_list_t* (AXIS2_CALL* 
    get_post_extension_class_names )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the third party schema names list.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_array_list_t*.
     */
    axis2_array_list_t* (AXIS2_CALL* 
    get_third_party_schema_names )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the language type names.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_array_list_t*.
     */
    axis2_array_list_t* (AXIS2_CALL* 
    get_language_types )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the emitter names map keys with the language name.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_hash_t*.
     */
    axis2_hash_t* (AXIS2_CALL* 
    get_language_emitter_map )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the default language name.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_default_language )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the language specific properties.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_hash_t*.
     */
    axis2_hash_t* (AXIS2_CALL* 
    get_language_specific_properties_map )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the databinding framework names.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_array_list_t*.
     */
    axis2_array_list_t* (AXIS2_CALL* 
    get_databinding_framework_names )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the extensions map for the databinding frameworks.
     * The entries are keys by the framework name.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_hash_t*.
     */
    axis2_hash_t* (AXIS2_CALL* 
    get_databinding_framework_name_to_extension_map )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
    
    /**
     * Gets the default DB framwork name.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_default_db_framework_name )(
        w2c_config_property_loader_t *config_property_loader,
        const axis2_env_t *env);
};

AXIS2_DECLARE_DATA  struct w2c_config_property_loader
{
     struct w2c_config_property_loader_ops *ops;
};

/**
 * create a config_property_loader obj
 * @param env Environment. MUST NOT be NULL
 * @return newly created config_property_loader obj
 */
AXIS2_EXTERN w2c_config_property_loader_t* AXIS2_CALL
w2c_config_property_loader_create( const axis2_env_t *env);

/*************************** Function macros **********************************/

#define W2C_CONFIG_PROPERTY_LOADER_FREE(config_property_loader, env) \
      ((config_property_loader)->ops->free (config_property_loader, env)) 

#define W2C_CONFIG_PROPERTY_LOADER_SET_FILENAME(config_property_loader, env, filename) \
      ((config_property_loader)->ops->set_filename (config_property_loader, env, filename)) 

#define W2C_CONFIG_PROPERTY_LOADER_RELOAD(config_property_loader, env) \
      ((config_property_loader)->ops->reload (config_property_loader, env)) 

#define W2C_CONFIG_PROPERTY_LOADER_GET_TEST_OBJECT_TEMPLATE_NAME (config_property_loader, env) \
      ((config_property_loader)->ops->get_test_object_template_name  (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_DB_SUPPORTER_TEMPLATE_NAME (config_property_loader, env) \
      ((config_property_loader)->ops->get_db_supporter_template_name  (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_EXTENSION_CLASS_NAMES(config_property_loader, env) \
      ((config_property_loader)->ops->get_extension_class_names (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_POST_EXTENSION_CLASS_NAMES(config_property_loader, env) \
      ((config_property_loader)->ops->get_post_extension_class_names (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_THIRD_PARTY_SCHEMA_NAMES(config_property_loader, env) \
      ((config_property_loader)->ops->get_third_party_schema_names (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_TYPES (config_property_loader, env) \
      ((config_property_loader)->ops->get_language_types  (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_EMITTER_MAP(config_property_loader, env) \
      ((config_property_loader)->ops->get_language_emitter_map (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_DEFAULT_LANGUAGE(config_property_loader, env) \
      ((config_property_loader)->ops->get_default_language (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_SPECIFIC_PROPERTIES_MAP(config_property_loader, env) \
      ((config_property_loader)->ops->get_language_specific_properties_map (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_DATABINDING_FARMEWORK_NAMES(config_property_loader, env) \
      ((config_property_loader)->ops->get_databinding_framework_names (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_DATABIDNING_FRAMEWORK_NAME_TO_EXTENSION_MAP(config_property_loader, env) \
      ((config_property_loader)->ops->get_databinding_framework_name_to_extension_map (config_property_loader, env))

#define W2C_CONFIG_PROPERTY_LOADER_GET_DEFAULT_DB_FRMAEWORK_NAME(config_property_loader, env) \
      ((config_property_loader)->ops->get_default_db_framework_name (config_property_loader, env))




/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_CONFIG_PROPERTY_LOADER_H */
