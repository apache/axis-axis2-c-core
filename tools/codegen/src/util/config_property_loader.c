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
 
#include <w2c_config_property_loader.h>
#include <w2c_properties.h>
#include <axis2_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <axis2_hash.h>
#include <stdlib.h>

#define W2C_CONFIG_PROPERTY_LOADER_CODE_GEN_KEY_PREFIX "codegen.extension"
#define W2C_CONFIG_PROPERTY_LOADER_POST_CODE_GEN_KEY_PREFIX \
             "post.codegen.extension"
#define W2C_CONFIG_PROPERTY_LOADER_THIRD_PARTY_SCHEMA_KEY_PREFIX \
             "codegen.thirdparty.schema"
#define W2C_CONFIG_PROPERTY_LOADER_LANGUAGE_TYPE_KEY_PREFIX "codegen.languages"
#define W2C_CONFIG_PROPERTY_LOADER_DEFAULT_LANGUAGE_TYPE_KEY \
             "codegen.languages.default"
#define W2C_CONFIG_PROPERTY_LOADER_EMITTER_CLASS_KEY \
             "codegen.emitters"
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_FRAMEWORK_NAME_KEY \
             "codegen.databinding.frameworks"
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_FRAMEWORK_DEFAULT_NAME_KEY \
             "codegen.databinding.frameworks.default"
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_FRAMEWORK_EXTENSION_NAME_KEY \
             "codegen.databinding.extensions"
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEMPLATE_NAME_KEY \
            "codegen.databinding.supporter.template"
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEST_OBJECT_TEMPLATE_NAME_KEY \
            "codegen.databinding.testobject.template"
/* the above define should be replace to follwing pre and suf fix defines */
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEMPLATE_NAME_KEY_PREFIX \
            "codegen.databinding." 
#define W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEMPLATE_NAME_KEY_SUFFIX \
            "template"
#define W2C_CONFIG_PROPERTY_LOADER_DEFAULT_CODEGEN_CONFIG_PROPERTIES \
            "/conf/codegen-config.properties"

/* Note - Should be a non regular expression character.
 If not it should be properly escaped */
#define W2C_CONFIG_PROPERTY_LOADER_SEPARATOR_CHAR  ','


/** 
 * @brief
 */
typedef struct w2c_config_property_loader_impl
{
    w2c_config_property_loader_t config_property_loader;
    
    axis2_char_t *test_object_template_name;
    axis2_char_t *db_supporter_template_name;
    axis2_array_list_t *extension_class_names;
    axis2_array_list_t *post_extension_class_names;
    axis2_array_list_t *third_party_schema_names;
    axis2_array_list_t *language_types;
    axis2_hash_t *language_emitter_map;
    axis2_char_t *default_language;
    axis2_hash_t *language_specific_properties_map;
    axis2_array_list_t *databinding_framework_names;
    axis2_hash_t *databinding_framework_name_to_extension_map;
    axis2_char_t *default_db_framework_name;

    w2c_properties_t *prop_set;
    axis2_char_t *prop_filename;
       
} w2c_config_property_loader_impl_t;

#define W2C_INTF_TO_IMPL(config_property_loader) \
    ((w2c_config_property_loader_impl_t *) config_property_loader)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
w2c_config_property_loader_free (
    w2c_config_property_loader_t *config_property_loader, 
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_config_property_loader_set_filename (
          w2c_config_property_loader_t *config_property_loader,
          const axis2_env_t *env,
          axis2_char_t *filename);

axis2_status_t AXIS2_CALL
w2c_config_property_loader_reload (
    w2c_config_property_loader_t *config_property_loader, 
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_config_property_loader_get_test_object_template_name (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_config_property_loader_get_db_supporter_template_name (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_extension_class_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_post_extension_class_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_third_party_schema_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_language_types (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL 
w2c_config_property_loader_get_language_emitter_map (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL 
w2c_config_property_loader_get_default_language (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL 
w2c_config_property_loader_get_language_specific_properties_map (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_databinding_framework_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL 
w2c_config_property_loader_get_databinding_framework_name_to_extension_map (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL 
w2c_config_property_loader_get_default_db_framework_name (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env);

/* private methods */

static void
w2c_config_property_loader_load_values(
                      w2c_config_property_loader_impl_t *obj_impl,
                      const axis2_env_t *env);

static axis2_array_list_t*
w2c_config_property_loader_dup_array(
                      axis2_array_list_t *arr,
                      const axis2_env_t *env);

static axis2_status_t
w2c_config_property_loader_free_array(
                      axis2_array_list_t *original,
                      const axis2_env_t *env);


static axis2_status_t
w2c_config_property_loader_free_hash(
                      axis2_hash_t *original,
                      const axis2_env_t *env);

/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_config_property_loader_t * AXIS2_CALL 
w2c_config_property_loader_create (const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    axis2_char_t* axis2c_home;
    int path_len;
   
    AXIS2_ENV_CHECK(env, NULL);

    config_property_loader_impl = (w2c_config_property_loader_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_config_property_loader_impl_t));

    if(NULL == config_property_loader_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    config_property_loader_impl->config_property_loader.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_config_property_loader_ops_t));
    if(NULL == config_property_loader_impl->config_property_loader.ops)
    {
        w2c_config_property_loader_free(&(config_property_loader_impl->config_property_loader), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
 
    config_property_loader_impl->test_object_template_name = NULL;
    config_property_loader_impl->db_supporter_template_name = NULL;
    config_property_loader_impl->extension_class_names = NULL;
    config_property_loader_impl->post_extension_class_names = NULL;
    config_property_loader_impl->third_party_schema_names = NULL;
    config_property_loader_impl->language_types = NULL;
    config_property_loader_impl->language_emitter_map = NULL;
    config_property_loader_impl->default_language = NULL;
    config_property_loader_impl->language_specific_properties_map = NULL;
    config_property_loader_impl->databinding_framework_names = NULL;
    config_property_loader_impl->databinding_framework_name_to_extension_map = NULL;
    config_property_loader_impl->default_db_framework_name = NULL;
    config_property_loader_impl->prop_set = NULL;
    config_property_loader_impl->prop_filename = NULL;
     
    axis2c_home = getenv ( "AXIS2C_HOME" );
    path_len = AXIS2_STRLEN (axis2c_home ) + 
             AXIS2_STRLEN (W2C_CONFIG_PROPERTY_LOADER_DEFAULT_CODEGEN_CONFIG_PROPERTIES) +
             2;
    config_property_loader_impl-> prop_filename = AXIS2_MALLOC ( env->allocator,
                                        sizeof(axis2_char_t)*path_len );
    sprintf(config_property_loader_impl-> prop_filename, "%s%s", axis2c_home,
                           W2C_CONFIG_PROPERTY_LOADER_DEFAULT_CODEGEN_CONFIG_PROPERTIES);
    
    config_property_loader_impl-> prop_set =  w2c_properties_create( env, 
        config_property_loader_impl-> prop_filename ,
        W2C_CONFIG_PROPERTY_LOADER_SEPARATOR_CHAR );
    if ( config_property_loader_impl-> prop_set != NULL )
    {
        w2c_config_property_loader_load_values( config_property_loader_impl, env );
    }
    config_property_loader_impl->config_property_loader.ops->free = 
                      w2c_config_property_loader_free;
    config_property_loader_impl->config_property_loader.ops->set_filename= 
                      w2c_config_property_loader_set_filename;
    config_property_loader_impl->config_property_loader.ops->reload = 
                      w2c_config_property_loader_reload;
    config_property_loader_impl->config_property_loader.ops->get_test_object_template_name  =
                      w2c_config_property_loader_get_test_object_template_name ;
    config_property_loader_impl->config_property_loader.ops->get_db_supporter_template_name  =
                      w2c_config_property_loader_get_db_supporter_template_name ;
    config_property_loader_impl->config_property_loader.ops->get_extension_class_names = 
                      w2c_config_property_loader_get_extension_class_names;
    config_property_loader_impl->config_property_loader.ops->get_post_extension_class_names = 
                      w2c_config_property_loader_get_post_extension_class_names;
    config_property_loader_impl->config_property_loader.ops->get_third_party_schema_names = 
                      w2c_config_property_loader_get_third_party_schema_names;
    config_property_loader_impl->config_property_loader.ops->get_language_types  = 
                      w2c_config_property_loader_get_language_types ;
    config_property_loader_impl->config_property_loader.ops->get_language_emitter_map = 
                      w2c_config_property_loader_get_language_emitter_map;
    config_property_loader_impl->config_property_loader.ops->get_default_language = 
                      w2c_config_property_loader_get_default_language;
    config_property_loader_impl->config_property_loader.ops->get_language_specific_properties_map = 
                      w2c_config_property_loader_get_language_specific_properties_map;
    config_property_loader_impl->config_property_loader.ops->get_databinding_framework_names = 
                      w2c_config_property_loader_get_databinding_framework_names;
    config_property_loader_impl->config_property_loader.ops->get_databinding_framework_name_to_extension_map = 
                      w2c_config_property_loader_get_databinding_framework_name_to_extension_map;
    config_property_loader_impl->config_property_loader.ops->get_default_db_framework_name =
                      w2c_config_property_loader_get_default_db_framework_name;

    return &(config_property_loader_impl->config_property_loader);
}


/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_config_property_loader_free (w2c_config_property_loader_t *config_property_loader, 
                            const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    if (config_property_loader_impl-> test_object_template_name)
    {
        AXIS2_FREE (env-> allocator,
                 config_property_loader_impl-> test_object_template_name);
    }
    
    if (config_property_loader_impl-> db_supporter_template_name)
    {
        AXIS2_FREE (env-> allocator, 
             config_property_loader_impl-> db_supporter_template_name);
    }
    if (config_property_loader_impl-> extension_class_names)
    {
        w2c_config_property_loader_free_array(
                 config_property_loader_impl-> extension_class_names, env);
    }

    if (config_property_loader_impl-> post_extension_class_names)
    {
        w2c_config_property_loader_free_array(
                 config_property_loader_impl-> post_extension_class_names, env);
    }

    if (config_property_loader_impl-> third_party_schema_names)
    {
        w2c_config_property_loader_free_array(
                 config_property_loader_impl-> third_party_schema_names, env);
    }

    if (config_property_loader_impl-> language_types)
    {
        w2c_config_property_loader_free_array(
                config_property_loader_impl-> language_types, env);
    }

    if (config_property_loader_impl-> language_emitter_map)
    {
        w2c_config_property_loader_free_hash(
                config_property_loader_impl-> language_emitter_map, env);
    }

    if (config_property_loader_impl-> default_language)
    {
        AXIS2_FREE (env-> allocator, config_property_loader_impl-> default_language);
    }

    if (config_property_loader_impl-> language_specific_properties_map)
    {
        /* all the elemenets here will be freed by some other */
        axis2_hash_free (
                 config_property_loader_impl-> language_specific_properties_map, env);
    }

    if (config_property_loader_impl-> databinding_framework_names)
    {
        w2c_config_property_loader_free_array(
                 config_property_loader_impl-> databinding_framework_names, env);
    }

    if (config_property_loader_impl-> databinding_framework_name_to_extension_map)
    {
        w2c_config_property_loader_free_hash(
                 config_property_loader_impl-> databinding_framework_name_to_extension_map, env);
    }

    if (config_property_loader_impl-> default_db_framework_name)
    {
        AXIS2_FREE (env-> allocator, config_property_loader_impl-> default_db_framework_name);
    }

    if (config_property_loader_impl-> prop_set)
    {
        W2C_PROPERTIES_FREE( config_property_loader_impl-> prop_set, env);
    }

    if (config_property_loader_impl-> prop_filename)
    {
        AXIS2_FREE (env-> allocator, config_property_loader_impl-> prop_filename);
    }

    if(config_property_loader->ops)
    {
        AXIS2_FREE(env->allocator, config_property_loader->ops);
        config_property_loader->ops = NULL;
    }
    if(config_property_loader_impl)
    {
        AXIS2_FREE(env->allocator, config_property_loader_impl);
        config_property_loader_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
w2c_config_property_loader_set_filename (
          w2c_config_property_loader_t *config_property_loader,
          const axis2_env_t *env,
          axis2_char_t *filename)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
  
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    config_property_loader_impl -> prop_filename = filename;   
    w2c_config_property_loader_reload ( config_property_loader, env );
    return AXIS2_SUCCESS;
}



axis2_status_t AXIS2_CALL
w2c_config_property_loader_reload (
    w2c_config_property_loader_t *config_property_loader, 
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    if (config_property_loader_impl-> prop_set)
    {
         W2C_PROPERTIES_FREE ( config_property_loader_impl->prop_set, env );
    }
    config_property_loader_impl-> prop_set=  w2c_properties_create( env, 
        config_property_loader_impl-> prop_filename ,
        W2C_CONFIG_PROPERTY_LOADER_SEPARATOR_CHAR );
    if ( config_property_loader_impl-> prop_set != NULL )
    {
        w2c_config_property_loader_load_values( config_property_loader_impl, env );
    }
    else
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
w2c_config_property_loader_get_test_object_template_name (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> test_object_template_name;
}


axis2_char_t* AXIS2_CALL
w2c_config_property_loader_get_db_supporter_template_name (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> db_supporter_template_name;
}


axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_extension_class_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> extension_class_names;
}


axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_post_extension_class_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> post_extension_class_names;
}


axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_third_party_schema_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> third_party_schema_names;
}


axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_language_types (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl->language_types;
}


axis2_hash_t* AXIS2_CALL 
w2c_config_property_loader_get_language_emitter_map (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> language_emitter_map;
}


axis2_char_t* AXIS2_CALL 
w2c_config_property_loader_get_default_language (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> default_language;
}


axis2_hash_t* AXIS2_CALL 
w2c_config_property_loader_get_language_specific_properties_map (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> language_specific_properties_map;
}


axis2_array_list_t* AXIS2_CALL 
w2c_config_property_loader_get_databinding_framework_names (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> databinding_framework_names;
}


axis2_hash_t* AXIS2_CALL 
w2c_config_property_loader_get_databinding_framework_name_to_extension_map (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> databinding_framework_name_to_extension_map;
}

axis2_char_t* AXIS2_CALL 
w2c_config_property_loader_get_default_db_framework_name (
    w2c_config_property_loader_t *config_property_loader,
    const axis2_env_t *env)
{
    w2c_config_property_loader_impl_t *config_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    config_property_loader_impl = W2C_INTF_TO_IMPL(config_property_loader);
    
    return config_property_loader_impl-> default_db_framework_name;
}


static void
w2c_config_property_loader_load_values(
                      w2c_config_property_loader_impl_t *obj_impl,
                      const axis2_env_t *env)
{
     axis2_hash_t* prop_hash = NULL;
     axis2_hash_t* temp_hash = NULL;
     axis2_hash_t* lang_spec_hash = NULL;
     axis2_hash_index_t* hi = NULL;
     axis2_array_list_t* prop = NULL;
     axis2_array_list_t* temp_array = NULL;
     axis2_char_t* temp_buff = NULL;
     axis2_char_t* key= NULL;
     axis2_char_t* val= NULL;
     int i;
     int size;

     prop_hash = W2C_PROPERTIES_GET_HASH( obj_impl-> prop_set, env);

     /*TODO: this should be configured to choose the prophash from 
      * a command line option -codegen.config="file"
      */

     /*load the extension class names*/
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_CODE_GEN_KEY_PREFIX,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL) 
     {
         obj_impl ->extension_class_names = 
             w2c_config_property_loader_dup_array(prop, env );
     }
 
     /*load the post extension class names*/
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_POST_CODE_GEN_KEY_PREFIX,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL) 
     {
         obj_impl ->post_extension_class_names = 
             w2c_config_property_loader_dup_array(prop, env );
     }
         
     /* load the data binding framework names */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_FRAMEWORK_NAME_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL) 
     {
         obj_impl ->databinding_framework_names =
             w2c_config_property_loader_dup_array(prop, env );
     }
     
     /* populate the data binding framework name to extension name map */
     temp_array = NULL;
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_FRAMEWORK_EXTENSION_NAME_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         temp_array = 
             w2c_config_property_loader_dup_array(prop, env );
         
         size= axis2_array_list_size (obj_impl ->databinding_framework_names, env);
         if (size != axis2_array_list_size (temp_array, env) )
         {
              fprintf ( stderr, "data_binding_framework name to extension diff in size" );
              w2c_config_property_loader_free(
                       &(obj_impl->config_property_loader), env);         
              return;
         }

         temp_hash = axis2_hash_make ( env );
         obj_impl ->databinding_framework_name_to_extension_map = temp_hash;
         for (i = 0 ; i < size ;i++ )
         {
             axis2_hash_set ( temp_hash,
              AXIS2_STRDUP( axis2_array_list_get(obj_impl ->databinding_framework_names, env, i ), env),
              AXIS2_HASH_KEY_STRING,
              AXIS2_STRDUP( axis2_array_list_get(temp_array, env, i ), env));
         }
     }
     
     /*load the default framework name */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_FRAMEWORK_DEFAULT_NAME_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         obj_impl ->default_db_framework_name = 
                      AXIS2_STRDUP( axis2_array_list_get(prop, env, 0 ), env);
     }
      
     /* load the third party schema names */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_THIRD_PARTY_SCHEMA_KEY_PREFIX,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         obj_impl ->third_party_schema_names= 
                      AXIS2_STRDUP( axis2_array_list_get(prop, env, 0 ), env);
     }
       
     /* the db supporter template name */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEMPLATE_NAME_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         obj_impl ->db_supporter_template_name= 
                      AXIS2_STRDUP( axis2_array_list_get(prop, env, 0 ), env);
     }
      
     /* test object template name */
     /* TODO: this should be replce to check for all properties 
      * starts with W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEMPLATE_NAME_KEY_PREFIX
      * ends with W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEMPLATE_NAME_KEY_SUFFIX
      */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_DATA_BINDING_TEST_OBJECT_TEMPLATE_NAME_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         obj_impl ->test_object_template_name= 
                      AXIS2_STRDUP( axis2_array_list_get(prop, env, 0), env);
     }
     
      
     /* load the language types */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_LANGUAGE_TYPE_KEY_PREFIX,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         obj_impl ->language_types = 
                          w2c_config_property_loader_dup_array(prop, env );
     }
       
     /* load the language emitter map */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_EMITTER_CLASS_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         temp_array = w2c_config_property_loader_dup_array(prop, env );
         
         size= axis2_array_list_size (obj_impl->language_types , env);
         if (size != axis2_array_list_size (temp_array, env) )
         {
              fprintf ( stderr, "data_binding_framework name to extension diff in size" );
              w2c_config_property_loader_free(
                       &(obj_impl->config_property_loader), env);         
              return;
         }

         temp_hash = axis2_hash_make ( env );
         obj_impl ->language_emitter_map = temp_hash;
         for (i = 0 ; i< size ; i++ )
         {
             key = axis2_array_list_get(obj_impl ->language_types, env, i);
             val = axis2_array_list_get(temp_array, env, i );
             axis2_hash_set ( temp_hash,
              AXIS2_STRDUP(  key, env),
              AXIS2_HASH_KEY_STRING,
              AXIS2_STRDUP( val, env));
         }
     }
 
     /* load the default language */
     prop = (axis2_array_list_t*)axis2_hash_get ( prop_hash,
           W2C_CONFIG_PROPERTY_LOADER_DEFAULT_LANGUAGE_TYPE_KEY,
           AXIS2_HASH_KEY_STRING );
     if ( prop != NULL)
     {
         obj_impl ->default_language = 
                      AXIS2_STRDUP( axis2_array_list_get(prop, env, 0 ), env);
     }

     /**
      * run through the language specific properties and populate the
      * language specific property map
      */
     if ( obj_impl->language_types)
     {
         temp_hash = axis2_hash_make ( env );
         obj_impl->language_specific_properties_map = temp_hash;
         size= axis2_array_list_size (obj_impl->language_types , env);
         for (i = 0 ; i< size ; i++ )
         {
             temp_buff = axis2_array_list_get(obj_impl ->language_types, env, i );
             lang_spec_hash = axis2_hash_make ( env );
             for (hi = axis2_hash_first(prop_hash, env); hi;
                  hi = axis2_hash_next(env, hi))
             {
                axis2_hash_this(hi, (void*)&key, NULL, (void*)&prop);
                if ( w2c_string_indexof_cs ( key, temp_buff ) == 0 && 
                                    w2c_string_indexof ( key, '.') == strlen (temp_buff ))
                {
                    prop = w2c_config_property_loader_dup_array( prop, env );
                    axis2_hash_set ( lang_spec_hash, key, AXIS2_HASH_KEY_STRING, prop );
                }
             }
             axis2_hash_set (temp_hash, temp_buff, AXIS2_HASH_KEY_STRING, lang_spec_hash );
         }
     }   
}
static axis2_array_list_t*
w2c_config_property_loader_dup_array(
                    axis2_array_list_t *original,
                    const axis2_env_t *env)
{
    int i = 0;
    int size = 0;
    axis2_array_list_t* new_array = NULL;
    axis2_char_t *tmp = NULL;
    
    size = axis2_array_list_size( original, env);
    if ( 0 == size )
        return axis2_array_list_create( env, 1);

    new_array = axis2_array_list_create( env, size);
    for( i = 0; i < size; i ++ )
    {
        tmp = (axis2_char_t*)axis2_array_list_get( original, env, i);
        tmp = AXIS2_STRDUP( tmp, env);
        axis2_array_list_add_at( new_array, env, i, tmp);
    }
    return new_array;
}

/** this frees axis2_char_t[] */
static axis2_status_t
w2c_config_property_loader_free_array(
                        axis2_array_list_t *arr,
                        const axis2_env_t *env)
{
    int i = 0;
    int size = 0;
    axis2_char_t *tmp = NULL;
    
    size = axis2_array_list_size( arr, env);
    for( i = 0; i < size; i ++ )
    {
        tmp = (axis2_char_t*)axis2_array_list_get( arr, env, i);
        AXIS2_FREE( env-> allocator, tmp);
    }
    axis2_array_list_free( arr, env);
    return AXIS2_SUCCESS;
}

/** this frees (axis2_char_t, axis2_char_t) */
static axis2_status_t
w2c_config_property_loader_free_hash(
                        axis2_hash_t *h,
                        const axis2_env_t *env)
{
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axis2_hash_index_t *hi = NULL;

    for (hi = axis2_hash_first( h, env);
                        hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if (key )
        {
            AXIS2_FREE( env-> allocator, key);
        }
        if (value)
        {
            AXIS2_FREE(env-> allocator, value);
        }
    }
    axis2_hash_free( h, env);
    return AXIS2_SUCCESS;

}
