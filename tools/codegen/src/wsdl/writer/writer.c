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
 
#include "writer_protected.h"
#include <axis2_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <w2c_engine_configuration.h>
#include <w2c_properties.h>
#include <stdlib.h>
#include <w2c_file_writer.h>
#include <w2c_xslt_utils.h>
#include <w2c_xslt_template_processor.h>


w2c_writer_impl_t * AXIS2_CALL 
w2c_writer_create_impl (const axis2_env_t *env)
{
    w2c_writer_impl_t *writer_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    writer_impl = (w2c_writer_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_writer_impl_t));

    if(NULL == writer_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    writer_impl->writer.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_writer_ops_t));
    if(NULL == writer_impl->writer.ops)
    {
        w2c_writer_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    writer_impl->self_name = W2C_WRITER_CLASS;
    writer_impl->xslt_filename = NULL;
    writer_impl->out_filename = NULL;
    writer_impl->language = NULL;

    writer_impl->writer.ops->free = w2c_writer_free;
    writer_impl->writer.ops->create_out_file = w2c_writer_create_out_file;
    writer_impl->writer.ops->parse = w2c_writer_parse;
    writer_impl->writer.ops->initialize = w2c_writer_initialize;

    return writer_impl;
}


/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
w2c_writer_free(w2c_writer_t *writer,
       const axis2_env_t *env)
{
    w2c_writer_impl_t *writer_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    writer_impl = W2C_INTF_TO_IMPL(writer);
    
    if( writer_impl->xslt_filename )
       AXIS2_FREE (env->allocator, writer_impl-> xslt_filename);
    if( writer_impl->out_filename )
       AXIS2_FREE (env->allocator, writer_impl-> out_filename);
    if( writer_impl->language)
       AXIS2_FREE (env->allocator, writer_impl-> language);

    if(writer->ops)
    {
        AXIS2_FREE(env->allocator, writer->ops);
        writer->ops = NULL;
    }
    if(writer_impl)
    {
        AXIS2_FREE(env->allocator, writer_impl);
        writer_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
w2c_writer_create_out_file(w2c_writer_t *writer,
       const axis2_env_t *env,
       axis2_char_t* package_name,
       axis2_char_t* file_name)
{
    w2c_writer_impl_t* writer_impl = NULL;
    axis2_char_t* ext_name = NULL;
    axis2_char_t* output_locat = NULL;

	AXIS2_ENV_CHECK(env, NULL);
    writer_impl = W2C_INTF_TO_IMPL(writer);

    ext_name = w2c_writer_get_extension ( writer_impl, env );
    output_locat = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LOCATION (
                             writer_impl-> config, env);
    writer_impl ->out_filename = 
              w2c_file_writer_create_classfile_name (env, output_locat,
                               package_name,
                               file_name,
                               ext_name );
    return writer_impl ->out_filename;                      
}


axis2_status_t AXIS2_CALL
w2c_writer_parse(w2c_writer_t *writer,
       const axis2_env_t *env,
       axiom_node_t* root)
{
    w2c_writer_impl_t *writer_impl = NULL;
    axis2_char_t* buffer = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_impl = W2C_INTF_TO_IMPL(writer);

    buffer = w2c_xslt_utils_serialize(env, root);
    
    w2c_xslt_template_processor_parse (env, buffer, 
                         writer_impl-> xslt_filename,
                         writer_impl-> out_filename);
    AXIS2_FREE ( env-> allocator, buffer );
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_writer_initialize(w2c_writer_t *writer,
               const axis2_env_t *env,
               w2c_config_property_loader_t* loader,
               w2c_engine_configuration_t* config)
{
    w2c_writer_impl_t *writer_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_impl = W2C_INTF_TO_IMPL(writer);
    writer_impl-> config = config;
    writer_impl-> loader = loader;

    w2c_writer_set_language (writer_impl, env);
    status = w2c_writer_load_template (writer_impl, env);

    return status;
}

/**
 * implementation for protected functions
 */
axis2_char_t*
w2c_writer_get_extension( w2c_writer_impl_t *writer_impl,
             const axis2_env_t *env)
{
    axis2_hash_t *lang_map = NULL;
    axis2_hash_t *lang_speci_map = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axis2_hash_index_t *hi = NULL;
    int all_length = 0;
    int suf_length = 0;
    int index = 0;
    axis2_char_t *ext_name = NULL;
    axis2_array_list_t *arr_list = NULL;
  
    lang_map = W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_SPECIFIC_PROPERTIES_MAP
                           ( writer_impl->loader, env );
    
    lang_speci_map = axis2_hash_get( lang_map,
                        writer_impl-> language, AXIS2_HASH_KEY_STRING);
    
    for (hi = axis2_hash_first(lang_speci_map, env) ;
                hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&arr_list);
        if ( key && arr_list)
        {
            suf_length = axis2_strlen( W2C_WRITER_EXTENSION_SUFFIX );
            all_length = axis2_strlen( key);
            index = w2c_string_indexof_cs
                                     (key, W2C_WRITER_EXTENSION_SUFFIX );
            if ( index + suf_length == all_length )/* this meen its a suf*/
            {
                 value = axis2_array_list_get( arr_list, env, 0);
                 ext_name = axis2_strdup (".", env );
                 ext_name = w2c_string_add_string ( ext_name, value, env);
                 break;
            }
        }
    }
    return ext_name;
}

axis2_char_t *
w2c_writer_find_template ( w2c_writer_impl_t *writer_impl,
                           const axis2_env_t *env,
                           axis2_hash_t *lang_speci_map)
{
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axis2_hash_index_t *hi = NULL;
    int all_length = 0;
    int suf_length = 0;
    int index = 0;
    axis2_char_t *tmpl_name = NULL;
    axis2_array_list_t *arr_list = NULL;
  
    for (hi = axis2_hash_first(lang_speci_map, env) ;
                hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&arr_list);
        if ( key && arr_list)
        {
            suf_length = axis2_strlen( W2C_WRITER_TEMPLATE_SUFFIX );
            all_length = axis2_strlen(key );
            index = w2c_string_indexof_cs
                                     (key, W2C_WRITER_TEMPLATE_SUFFIX );
            if ( index + suf_length == all_length )/* this meen its a suf*/
            {
                 value = axis2_array_list_get ( arr_list, env, 0 );
                 index = w2c_string_indexof_cs
                                      (value, writer_impl-> self_name );
                 if (0 == index) /* prefix matches for the first entry*/
                 {
                     value = axis2_array_list_get ( arr_list, env, 1);
                     tmpl_name = axis2_strdup ("", env );
                     tmpl_name = w2c_string_add_string ( tmpl_name, value, env);
                     break;
                 }
            }
        }
    }
    return tmpl_name;
}

axis2_status_t
w2c_writer_set_self_name( w2c_writer_impl_t* writer_impl,
             const axis2_env_t* env,
             axis2_char_t* self_name)
{
    writer_impl -> self_name = axis2_strdup(self_name, env);
    return AXIS2_SUCCESS;
}


axis2_status_t
w2c_writer_set_language(w2c_writer_impl_t *writer_impl,
       const axis2_env_t *env)
       
{
    axis2_char_t* lang = NULL;
    lang = 
       W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE ( writer_impl-> config, env);
    if (NULL == lang )
    {
        lang = 
            W2C_CONFIG_PROPERTY_LOADER_GET_DEFAULT_LANGUAGE(writer_impl->loader, env);
        if ( NULL == lang )
            return AXIS2_FAILURE;
    }
    writer_impl-> language = axis2_strdup(lang, env);
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_writer_load_template(w2c_writer_impl_t *writer_impl,
       const axis2_env_t *env)
{
    axis2_hash_t* lang_map = NULL;
    axis2_hash_t* lang_speci_map = NULL;
    axis2_char_t* file_path = NULL;
    axis2_char_t* tmpl_name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if ( NULL == writer_impl-> language )
          return AXIS2_FAILURE;


    file_path = getenv("AXIS2C_HOME");
    file_path = axis2_strdup ( file_path, env);
    if ( NULL == file_path)
    {
        return AXIS2_FAILURE;
    }
 
    lang_map = W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_SPECIFIC_PROPERTIES_MAP
                           ( writer_impl->loader, env );
    
    lang_speci_map = axis2_hash_get( lang_map,
                        writer_impl-> language, AXIS2_HASH_KEY_STRING);
    
    tmpl_name = w2c_writer_find_template( writer_impl, env, lang_speci_map);
    if ( NULL == tmpl_name || *tmpl_name =='\0')
    {
        AXIS2_FREE ( env-> allocator, file_path );
        return AXIS2_FAILURE;
    }
    file_path = w2c_string_add_string(file_path, W2C_WRITER_XSLT_PATH, env );
    file_path = w2c_string_add_string(file_path, tmpl_name, env );    
    writer_impl-> xslt_filename = file_path;
    return AXIS2_SUCCESS;
}

