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
 
#include <w2c_engine_config_loader.h>
#include <w2c_cmdline_option_consts.h>
#include <axutil_hash.h>
#include <axutil_array_list.h>
#include <w2c_cmdline_option.h>
#include <w2c_messages.h>
#include <axutil_string.h>
#include <w2c_string.h>
#include <w2c_properties.h>

#define W2C_ENGINE_CONFIG_LOADER_OUTPUT_DEFAULT "src"

static w2c_cmdline_option_t*
w2c_engine_config_loader_load_option(const axutil_env_t *env,
                                    axis2_char_t *short_opt,axis2_char_t *long_opt,
                                     axutil_hash_t *options);
static void
w2c_engine_config_loader_free_hash(const axutil_env_t *env, axutil_hash_t *h );

AXIS2_EXTERN axis2_status_t AXIS2_CALL
w2c_engine_config_loader_load_config(
    const axutil_env_t *env,
    w2c_engine_configuration_t *conf,
    axutil_hash_t *option_map )
{
    w2c_cmdline_option_t *option = NULL;
    axis2_char_t *tmp_string= NULL;
    axis2_bool_t tmp_bool = AXIS2_FALSE;
    
    /* used in creating map */
    axis2_char_t *tag = NULL;
    axis2_char_t *cp = NULL;
    int loop_state = 0;
    axis2_char_t *tmp_key = NULL;
    axutil_hash_t *h = NULL;
    axutil_hash_index_t *hi = NULL;
    axutil_array_list_t *tmp_array = NULL;
    w2c_properties_t *props = NULL;
    int len = 0;
    
    /* set output directory */
    option = w2c_engine_config_loader_load_option( env, 
                       W2C_CMDLINE_OPTION_CONSTS_OUTPUT_LOCATION,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_OUTPUT_LOCATION, 
                       option_map);

    if (option != NULL) 
    {
         tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option , env );
    }
    else
    {
         tmp_string = axis2_strdup(env, W2C_ENGINE_CONFIG_LOADER_OUTPUT_DEFAULT);
    }
    /*TODO:  should check for the vaildity of the output location */
    W2C_ENGINE_CONFIGURATION_SET_OUTPUT_LOCATION( conf, env,
                                          tmp_string);

    /* set server side */
    tmp_bool = w2c_engine_config_loader_load_option( env, 
                       W2C_CMDLINE_OPTION_CONSTS_SERVER_SIDE_CODE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_SERVER_SIDE_CODE, 
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_SERVER_SIDE ( conf, env,tmp_bool );

    /* set service description*/
    tmp_bool = w2c_engine_config_loader_load_option( env, 
                       W2C_CMDLINE_OPTION_CONSTS_SERVICE_DESCRIPTION,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_SERVICE_DESCRIPTION, 
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_GENERATE_DEPLOYEMENT_DESCRIPTOR
                                                    (conf, env,tmp_bool );

    /* by default both sync and asyn are generated */
    W2C_ENGINE_CONFIGURATION_SET_ASYNC_ON ( conf, env, AXIS2_TRUE);
    W2C_ENGINE_CONFIGURATION_SET_SYNC_ON ( conf, env, AXIS2_TRUE);

    /* set async only flag */
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_ASYNC_ONLY,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_ASYNC_ONLY,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_SYNC_ON ( conf, env, !tmp_bool);

    /* set sync only flag - this would have the priority*/
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_SYNC_ONLY,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_SYNC_ONLY,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_ASYNC_ON ( conf, env,!tmp_bool );

    /* package option */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_PACKAGE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_PACKAGE,
                       option_map);
    
    if (option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        W2C_ENGINE_CONFIGURATION_SET_PACKAGE_NAME ( conf, env, tmp_string);
    }
 
    /* lang option */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_STUB_LANGUAGE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_STUB_LANGUAGE,
                       option_map);
    
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        W2C_ENGINE_CONFIGURATION_SET_OUTPUT_LANGUAGE( conf, env, tmp_string);
    }
    
     /* databinding option */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_DATA_BINDING_TYPE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_DATA_BINDING_TYPE,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        W2C_ENGINE_CONFIGURATION_SET_DATABINDING_TYPE( conf, env, tmp_string);
    }

    /* set unpack classes */
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_PACK_CLASSES,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_PACK_CLASSES,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_PACK_CLASSES( conf, env, tmp_bool);
 
    /* port name option */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_PORT_NAME,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_PORT_NAME,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        W2C_ENGINE_CONFIGURATION_SET_PORT_NAME( conf, env, tmp_string);
    }

    /* service name option */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_SERVICE_NAME,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_SERVICE_NAME,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        W2C_ENGINE_CONFIGURATION_SET_SERVICE_NAME( conf, env, tmp_string);
    }

    /* repository path option */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_REPOSITORY_PATH,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_REPOSITORY_PATH,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        W2C_ENGINE_CONFIGURATION_SET_REPOSITORY_PATH( conf, env, tmp_string);
    }

    /* server side code option*/
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_SERVER_SIDE_CODE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_SERVER_SIDE_CODE,
                       option_map) != NULL;
   W2C_ENGINE_CONFIGURATION_SET_SERVERSIDE_INTERFACE 
                                     ( conf, env, tmp_bool);
 
    /* generate all option*/
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_GENERATE_ALL,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_GENERATE_ALL,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_GENERATE_ALL
                                     ( conf, env, tmp_bool);

    /* convert names option*/
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_CONVERT_NAMES,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_CONVERT_NAMES,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_CONVERT_NAMES
                                     ( conf, env, tmp_bool);

    /* format code option*/
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_INDENT_CODE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_INDENT_CODE,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_INDENT_CODE
                                     ( conf, env, tmp_bool);


    /* external mapping */
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_EXTERNAL_MAPPING,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_EXTERNAL_MAPPING,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        if (tmp_string )
        {
            W2C_ENGINE_CONFIGURATION_SET_TYPE_MAPPING_FILE( conf, env, tmp_string);
        }
        else
        {
            w2c_messages_print_n_log_error(env, "options.nomappingFile");
        }
    }

    /* wsdl versioning*/
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_WSDL_VERSION,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        if (tmp_string )
        {
            W2C_ENGINE_CONFIGURATION_SET_WSDL_VERSION( conf, env, tmp_string);
        }
        else
        {
            W2C_ENGINE_CONFIGURATION_SET_WSDL_VERSION( conf, env, 
                                W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION_1);
        }
    }
  
    /* flattern files */
    tmp_bool = w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_FLATTEN_FILES,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_FLATTEN_FILES,
                       option_map) != NULL;
    W2C_ENGINE_CONFIGURATION_SET_FLATTEN_FILES ( conf, env, tmp_bool);
    
    
    /* namespace to package list*/
    option =  w2c_engine_config_loader_load_option( env,
                       W2C_CMDLINE_OPTION_CONSTS_NAME_SPACE_TO_PACKAGE,
                       W2C_CMDLINE_OPTION_CONSTS_LONG_NAME_SPACE_TO_PACKAGE,
                       option_map);
    if ( option)
    {
        tmp_string = W2C_CMDLINE_OPTION_GET_VALUE ( option, env );
        if (tmp_string)
        {
           h = axutil_hash_make (env );
           loop_state = -1; /* 1 - started, 1 - '=' found 2- end*/
           for ( cp = tmp_string; loop_state !=2 ; cp ++ )
           {
               if ( loop_state == -1 )
               {
                    tag = cp;
                    loop_state = 0;
               }
               else if (*(cp-1) == '\0' )
               {
                    tag = cp;
               }
               else if ( *cp == '=' )
               {
                    if ( loop_state != -1 && loop_state != 0 )
                    {
                        w2c_messages_print_n_log_error( env,
                                "options.wrongNamespace2Package");
                        printf("wrong namespace to package"
                                    " option-check %daxis2_char_t\n",
                                     (int)(cp - tmp_string) );
                        w2c_engine_config_loader_free_hash ( env, h);
                        break;
                    }
                    *cp ='\0';
                    tmp_key = axis2_strdup (env, tag);
                    loop_state = 1;
               }
               else if ( *cp ==',' || *cp=='\0' )
               {
                    if ( *cp == '\0' && loop_state == 0 )
                    {
                        loop_state = -1;
                        break;
                    }
                    if (  loop_state != -1 && loop_state != 1)
                    {
                        w2c_messages_print_n_log_error( env,
                                "options.wrongNamespace2Package");
                        printf("wrong namespace to package"
                                    " option-check %daxis2_char_t\n", 
                                     (int)(cp - tmp_string) );
                        w2c_engine_config_loader_free_hash ( env, h);
                        break;
                    }
                    if ( *cp =='\0')
                        loop_state = 2;
                    else 
                        loop_state = 0;
                    *cp = '\0';
                    axutil_hash_set( h, tmp_key, AXIS2_HASH_KEY_STRING,
                                    axis2_strdup (env, tag) );
               }
           }
           if (loop_state == -1)
           {
               axutil_hash_free (h ,env); /*clear created hash*/
               /* asumes option value is a filename */
               props = w2c_properties_create( env, tmp_string, '\0');
               h = W2C_PROPERTIES_GET_HASH( props, env);
               if ( h == NULL)
               {
                   w2c_messages_print_n_log_error(env, "options.noFile");
               }
               else
               {
                   /* replace arraylist in each with string */
                   for (hi = axutil_hash_first(h , env) ;
                               hi; hi = axutil_hash_next(env, hi))
                   {
                       axutil_hash_this(hi, (void*)&tmp_key, NULL, (void*)&tmp_string);
                       tmp_key = axis2_strdup(env, tmp_key);
                       tmp_string = axis2_strdup (env, tmp_string);
                       axutil_hash_set( h, tmp_key, AXIS2_HASH_KEY_STRING, tmp_string);
                   }
               }
               W2C_PROPERTIES_FREE( props, env);
           }
           W2C_ENGINE_CONFIGURATION_SET_URI2PACKAGEMAP( conf, env, h );
        }
    }
    
    /* catch extra parameters */
    h = NULL;
    for (hi = axutil_hash_first(option_map , env) ;
                hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&tmp_key,
             NULL, (void*)&tmp_array);
        if ( w2c_string_indexof_cs 
             ( tmp_key, W2C_CMDLINE_OPTION_CONSTS_EXTRA_OPTIONTYPE_PREFIX)== 0 && tmp_array)
        {
           tmp_string = (char*)axutil_array_list_get (tmp_array, env, 0 );
           if ( h == NULL )
           {
               h = axutil_hash_make ( env );   
           }
           len = axis2_strlen (W2C_CMDLINE_OPTION_CONSTS_EXTRA_OPTIONTYPE_PREFIX );
           tmp_key = axutil_string_substring_starting_at ( tmp_key, len);
           axutil_hash_set( h, tmp_key, AXIS2_HASH_KEY_STRING, tmp_string );
           axutil_array_list_free ( tmp_array, env);
        }
    }
    W2C_ENGINE_CONFIGURATION_SET_CONFIGURATION_PROPERTIES( conf, env, h );
    
    return AXIS2_SUCCESS;
}

static w2c_cmdline_option_t*
w2c_engine_config_loader_load_option(const axutil_env_t *env,
                                    axis2_char_t *short_opt,axis2_char_t *long_opt,
                                     axutil_hash_t *options_map) 
{
    w2c_cmdline_option_t *option = NULL;
    if (long_opt != NULL) 
    {
        option = (w2c_cmdline_option_t*)axutil_hash_get(options_map,
                               long_opt, AXIS2_HASH_KEY_STRING);
        if (option != NULL)
        {
            return option;
        }
    }
    /* short option gets last precedence */
    if (short_opt != NULL)
    {
        option = (w2c_cmdline_option_t*)axutil_hash_get(options_map,
                               short_opt, AXIS2_HASH_KEY_STRING);
    }

    return option;
}

static void
w2c_engine_config_loader_free_hash(const axutil_env_t *env, axutil_hash_t *h )
{
    axutil_hash_index_t *hi = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value =NULL;
    for (hi = axutil_hash_first(h, env) ;
                hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if ( value )AXIS2_FREE ( env-> allocator, value );
        if ( key )AXIS2_FREE ( env-> allocator, key);
    }
    axutil_hash_free ( h , env);
}
