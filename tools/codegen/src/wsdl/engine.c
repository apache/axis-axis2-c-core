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
 
#include <w2c_engine.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <w2c_cmdline_option.h>
#include <w2c_cmdline_option_consts.h>
#include <w2c_engine_configuration.h>
#include <w2c_config_property_loader.h>
#include <w2c_class_loader.h>
#include <w2c_extension.h>
#include <w2c_emitter.h>
#include <w2c_messages.h>
#include <stdio.h>
#include <w2c_class_loader.h>
#include <axis2_dll_desc.h>

/** 
 * @brief
 */
typedef struct w2c_engine_impl
{
    w2c_engine_t engine;
    
    axis2_array_list_t *pre_extensions;
    axis2_array_list_t *post_extensions;
    axis2_array_list_t *pre_extensions_dll_desc;
    axis2_array_list_t *post_extensions_dll_desc;
    w2c_engine_configuration_t *conf;
    w2c_config_property_loader_t *prop_loader;
       
} w2c_engine_impl_t;

#define W2C_INTF_TO_IMPL(engine) \
    ((w2c_engine_impl_t *) engine)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_engine_free (w2c_engine_t *engine, 
           const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
w2c_engine_generate(w2c_engine_t *engine, 
           const axis2_env_t *env);

/* private methods */
static axis2_status_t
w2c_engine_load_extension (w2c_engine_impl_t *engine_impl,
                   const axis2_env_t *env);
static axis2_char_t*
w2c_engine_get_base_uri ( axis2_char_t *current_uri, const axis2_env_t *env);

/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_engine_t* AXIS2_CALL 
w2c_engine_create_with_parser (const axis2_env_t *env,
                           w2c_cmdline_option_parser_t *parser)
{
    w2c_engine_impl_t *engine_impl = NULL;
    axis2_hash_t *all_options = NULL;
    axis2_char_t *wsdl_uri = NULL;
    axis2_char_t *base_uri = NULL;
    w2c_cmdline_option_t *option = NULL;
    axis2_char_t *output_lang = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    engine_impl = (w2c_engine_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_engine_impl_t));

    if(NULL == engine_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    engine_impl->engine.ops = 
       AXIS2_MALLOC (env->allocator, sizeof(w2c_engine_ops_t));
    if(NULL == engine_impl->engine.ops)
    {
        w2c_engine_free(&(engine_impl->engine), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    engine_impl->engine.ops->free = w2c_engine_free;
    engine_impl->engine.ops->generate = w2c_engine_generate;

    /** initializing variables */
    engine_impl-> pre_extensions = NULL;
    engine_impl-> pre_extensions_dll_desc = NULL;
    engine_impl-> post_extensions = NULL;
    engine_impl-> post_extensions_dll_desc = NULL;

    engine_impl->prop_loader =  w2c_config_property_loader_create ( env);
    
    all_options = W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS (parser, env );
    engine_impl-> conf =  
        w2c_engine_configuration_create_with_options ( env, all_options );

    option = axis2_hash_get(all_options, 
            W2C_CMDLINE_OPTION_CONSTS_WSDL_LOCATION_URI,
            AXIS2_HASH_KEY_STRING );
    
    wsdl_uri = W2C_CMDLINE_OPTION_GET_VALUE (option, env );
    W2C_ENGINE_CONFIGURATION_SET_WSDL_URI( engine_impl->conf,
                                           env, wsdl_uri );

    base_uri = w2c_engine_get_base_uri( wsdl_uri, env );
    W2C_ENGINE_CONFIGURATION_SET_BASE_URI( engine_impl->conf,
                                           env, base_uri );

    /** engine_configuration can be changed by prop_loader as well*/   
    output_lang = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE
                     ( engine_impl-> conf, env );
    if ( output_lang == NULL || *output_lang == '\0' )
    {
        output_lang=
            W2C_CONFIG_PROPERTY_LOADER_GET_DEFAULT_LANGUAGE
                     (engine_impl->prop_loader, env);
        output_lang = AXIS2_STRDUP( output_lang, env);
        W2C_ENGINE_CONFIGURATION_SET_OUTPUT_LANGUAGE
                     ( engine_impl-> conf, env, output_lang );
        if ( NULL == output_lang )
        {
            w2c_messages_print_n_log_error( env,
                       "engine.noDefaultLang");
            return AXIS2_FAILURE;
        }
    }
    w2c_engine_load_extension ( engine_impl, env );
    return &(engine_impl->engine);
}



/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_engine_free (w2c_engine_t *engine, 
                            const axis2_env_t *env)
{
    w2c_engine_impl_t *engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    engine_impl = W2C_INTF_TO_IMPL(engine);

    /* following should be free 1 by 1 after the extension developed */
    if(engine_impl-> pre_extensions )
         AXIS2_ARRAY_LIST_FREE (engine_impl-> pre_extensions, env );
    if(engine_impl-> post_extensions )
         AXIS2_ARRAY_LIST_FREE (engine_impl-> post_extensions, env );
    if(engine_impl-> pre_extensions_dll_desc )
         AXIS2_ARRAY_LIST_FREE (engine_impl-> pre_extensions_dll_desc, env );
    if(engine_impl-> post_extensions_dll_desc )
         AXIS2_ARRAY_LIST_FREE (engine_impl-> post_extensions_dll_desc, env );
 
    if(engine_impl-> conf)
         W2C_ENGINE_CONFIGURATION_FREE (engine_impl-> conf, env);
    if(engine_impl-> prop_loader)
         W2C_CONFIG_PROPERTY_LOADER_FREE( engine_impl-> prop_loader, env );

    if(engine->ops)
    {
        AXIS2_FREE(env->allocator, engine->ops);
        engine->ops = NULL;
    }
    if(engine_impl)
    {
        AXIS2_FREE(env->allocator, engine_impl);
        engine_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
w2c_engine_generate(w2c_engine_t *engine, 
           const axis2_env_t *env)
{
    w2c_engine_impl_t *engine_impl = NULL;
    int size  = 0;
    int i = 0;
    w2c_extension_t *ext = NULL;
    w2c_emitter_t *emitter = NULL;
    axis2_hash_t *emitter_map = NULL;
    axis2_char_t *emit_name = NULL;
    axis2_char_t *output_lang = NULL;
    axis2_bool_t all_flag = AXIS2_FALSE;
    axis2_bool_t only_flag = AXIS2_FALSE;
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    engine_impl = W2C_INTF_TO_IMPL(engine);
    
    if ( engine_impl-> pre_extensions != NULL )
    {
        size = AXIS2_ARRAY_LIST_SIZE( engine_impl-> pre_extensions, env);
        for (i = 0; i < size ; i++)
        {
            ext = (w2c_extension_t*)
                       AXIS2_ARRAY_LIST_GET( engine_impl-> pre_extensions, env, i);
            W2C_EXTENSION_ENGAGE ( ext, env, engine_impl-> conf);
        }
    }
    
    emitter_map = W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_EMITTER_MAP
                    ( engine_impl-> prop_loader, env );
    output_lang = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE
                     ( engine_impl-> conf, env );
    emit_name = (axis2_char_t*)axis2_hash_get ( emitter_map,
                                            output_lang, AXIS2_HASH_KEY_STRING);
    /* the place where emitter is loading */
    emitter = w2c_class_loader_get_object_from_class_name
          ( env, emit_name , W2C_CLASS_LOADER_EMITTER_PATH, &dll_desc );
    if ( NULL == emitter)
    {
        w2c_messages_print_n_log_error( env,
                       "engine.emitterMissing");
        return AXIS2_FAILURE;
    }
    status = W2C_EMITTER_SET_CONFIG( emitter, env, engine_impl-> conf,
                                          engine_impl-> prop_loader );
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "engine.errorEmit");
        return AXIS2_FAILURE;
    }
    /* emit skel and stub */
    all_flag = W2C_ENGINE_CONFIGURATION_GET_GENERATE_ALL
                 (engine_impl-> conf, env );
    only_flag = 
         W2C_ENGINE_CONFIGURATION_GET_SERVER_SIDE ( engine_impl-> conf, env);
    if (only_flag )
    {
        W2C_EMITTER_EMIT_SKEL( emitter, env );
        if ( all_flag )
        {
            W2C_EMITTER_EMIT_STUB( emitter, env);
        }
    }
    else
    {
        W2C_EMITTER_EMIT_STUB( emitter, env);
    }
    
    w2c_class_loader_free_loaded_class(env, dll_desc);
 
    if ( engine_impl-> post_extensions != NULL )
    {
        size = AXIS2_ARRAY_LIST_SIZE( engine_impl-> post_extensions, env);
        for (i = 0; i < size ; i++)
        {
            ext = (w2c_extension_t*)
                       AXIS2_ARRAY_LIST_GET( engine_impl-> post_extensions, env, i);
            W2C_EXTENSION_ENGAGE( ext, env, engine_impl-> conf); 
        }
    }
    /** freeing used extension right here */
    if ( engine_impl-> pre_extensions != NULL )
    {
        size = AXIS2_ARRAY_LIST_SIZE( engine_impl-> pre_extensions, env);
        for (i = 0; i < size ; i++)
        {
            dll_desc = (axis2_dll_desc_t*)
              AXIS2_ARRAY_LIST_GET( engine_impl-> pre_extensions_dll_desc, env, i);
            /*w2c_class_loader_free_loaded_class(env, dll_desc);*/
        }
  
    if ( engine_impl-> post_extensions != NULL )
    {
        size = AXIS2_ARRAY_LIST_SIZE( engine_impl-> post_extensions, env);
        for (i = 0; i < size ; i++)
        {
            dll_desc = (axis2_dll_desc_t*)
              AXIS2_ARRAY_LIST_GET( engine_impl-> post_extensions_dll_desc, env, i);
            /*w2c_class_loader_free_loaded_class(env, dll_desc);*/
        }
    }  } 
    return AXIS2_SUCCESS;
}

/* private methods */
static axis2_status_t
w2c_engine_load_extension (w2c_engine_impl_t *engine_impl,
                   const axis2_env_t *env)
{
    axis2_array_list_t *extension_names = NULL;
    w2c_config_property_loader_t *prop_loader = NULL;
    axis2_char_t *class_name = NULL;
    w2c_extension_t *ext = NULL;
    axis2_dll_desc_t *dll_desc = NULL;
    int size = 0;
    int i = 0;
    
    /* load pre extension */
    prop_loader = engine_impl-> prop_loader;
    extension_names = 
      W2C_CONFIG_PROPERTY_LOADER_GET_EXTENSION_CLASS_NAMES( prop_loader, env );
    if ( extension_names != NULL)
    {
       engine_impl-> pre_extensions
           = axis2_array_list_create ( env, 10 );
       engine_impl-> pre_extensions_dll_desc
           = axis2_array_list_create ( env, 10 );
       size = AXIS2_ARRAY_LIST_SIZE ( extension_names ,env);
       for ( i = 0; i < size; i ++ )
       {
           class_name = (axis2_char_t*)AXIS2_ARRAY_LIST_GET 
                                 ( extension_names, env , i );
           ext = w2c_class_loader_get_object_from_class_name
                   ( env, class_name, W2C_CLASS_LOADER_EXTENSION_PATH, &dll_desc);
           if ( NULL!= ext )
           {
               AXIS2_ARRAY_LIST_ADD_AT ( engine_impl-> pre_extensions,
                                   env, i,(void*)ext);
               AXIS2_ARRAY_LIST_ADD_AT (engine_impl-> pre_extensions_dll_desc,
                                   env, i,(void*)dll_desc);
           }
           else
           {
               w2c_class_loader_free_loaded_class(env, dll_desc);
           }
       }
    }
    /* load post extension */
    extension_names = 
      W2C_CONFIG_PROPERTY_LOADER_GET_POST_EXTENSION_CLASS_NAMES( prop_loader, env );

    if ( extension_names != NULL)
    {
       engine_impl-> post_extensions
           = axis2_array_list_create ( env, 10 );
       engine_impl-> post_extensions_dll_desc
           = axis2_array_list_create ( env, 10 );
       size = AXIS2_ARRAY_LIST_SIZE ( extension_names ,env);
       for ( i = 0; i < size; i ++ )
       {
           class_name = (axis2_char_t*)AXIS2_ARRAY_LIST_GET 
                                 ( extension_names, env , i );
           ext = w2c_class_loader_get_object_from_class_name 
                ( env, class_name, W2C_CLASS_LOADER_EXTENSION_PATH, &dll_desc );
           if ( NULL != ext )
           {
               AXIS2_ARRAY_LIST_ADD_AT ( engine_impl-> post_extensions,
                                   env, i,ext );
               AXIS2_ARRAY_LIST_ADD_AT (engine_impl-> post_extensions_dll_desc,
                                   env, i,(void*)dll_desc);
           }
           else
           {
               w2c_class_loader_free_loaded_class(env, dll_desc);
           }
       }
    }
    return AXIS2_SUCCESS;
}

static axis2_char_t*
w2c_engine_get_base_uri ( axis2_char_t *current_uri, const axis2_env_t *env)
{
    axis2_char_t *base_uri = NULL;
    int index = 0;
    axis2_char_t *c = NULL;
    int len = 0;

    index = w2c_string_indexof_cs ( current_uri, "http://" );
    if (index != 0 )
    {
        /** uri should be a file */
        /**TODO: this doesnt work always */
        base_uri = AXIS2_STRDUP("file:/", env);
        base_uri = w2c_string_add_string ( base_uri, current_uri, env );
    }
    else
    {
        base_uri = AXIS2_STRDUP( current_uri, env );
    }
    /* retrieve the base uri */
    len = AXIS2_STRLEN ( current_uri);
    for ( c = base_uri + len-1 ; c !=base_uri && *c != '/' ; c -- );
    if (c != base_uri )
    {
        *(c+1) = '\0';
    }

    return base_uri;
}
