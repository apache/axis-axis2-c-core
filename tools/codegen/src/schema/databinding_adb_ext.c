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
 
#include <w2c_extension.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>
#include <xml_schema.h>
#include <stdio.h>

#include <woden_resolver.h>
#include <woden_desc.h>
#include <woden_wsdl10_desc.h>
#include <woden_types.h>
#include <woden_schema.h>

#include <axiom_document.h>
#include <axiom_stax_builder.h>
#include <axiom_xml_reader.h>

#include <w2c_schema_compiler.h>
#include <w2c_schema_compiler_options.h>

typedef struct w2c_databinding_adb_ext_impl
{
    w2c_extension_t extension;

} w2c_databinding_adb_ext_impl_t;

#define W2C_INTF_TO_IMPL(extension) \
        ((w2c_databinding_adb_ext_impl_t*) extension)

#define W2C_DATABINDING_DEFAULT_EXT_FILENAME "c.default.typemap.xml"

axis2_status_t
w2c_databinding_adb_ext_invoke( const axis2_env_t * env,
        w2c_engine_configuration_t *conf);

axis2_status_t
populate_default_options(
           const axis2_env_t *env,
           w2c_schema_compiler_options_t *options,
           w2c_engine_configuration_t *config);


axiom_document_t*
get_root_element_from_filename(
        const axis2_env_t *env,
        axis2_char_t *filename);

/************************* implmentations ********************************/
axis2_status_t AXIS2_CALL
w2c_databinding_adb_ext_free(w2c_extension_t *extension,
       const axis2_env_t *env)
{
    w2c_databinding_adb_ext_impl_t *extension_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    extension_impl = W2C_INTF_TO_IMPL(extension);

    if(extension->ops)
    {
        AXIS2_FREE(env->allocator, extension->ops);
    }
    if(extension_impl)
    {
        AXIS2_FREE(env->allocator, extension_impl);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_databinding_adb_ext_engage(w2c_extension_t *extension,
       const axis2_env_t *env,
       w2c_engine_configuration_t *conf)
{
    w2c_databinding_adb_ext_impl_t *extension_impl = NULL;
    w2c_typemapper_t *current_typemapper = NULL;
    axis2_char_t *db_type = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    extension_impl = W2C_INTF_TO_IMPL(extension);

    db_type = W2C_ENGINE_CONFIGURATION_GET_DATABINDING_TYPE( conf, env);

    /* this executes only with following configuration */
    if ( !AXIS2_STRCMP( db_type, "adb" ) )
    {
        current_typemapper = W2C_ENGINE_CONFIGURATION_GET_TYPEMAPPER( conf, env);
        status = w2c_databinding_adb_ext_invoke( env, conf);
        if ( AXIS2_SUCCESS == status )
        {
            if ( current_typemapper != NULL )
            {
                W2C_TYPEMAPPER_FREE( current_typemapper, env );
            }
        }
        else
        {
            if ( current_typemapper == NULL)
            {
                current_typemapper = w2c_typemapper_create_from_file( env,
                                         W2C_DATABINDING_DEFAULT_EXT_FILENAME );
                W2C_ENGINE_CONFIGURATION_SET_TYPEMAPPER( conf, env, current_typemapper);
            }
        }
    }
    return status;
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_extension_t **inst,
                   const axis2_env_t *env)
{
    w2c_databinding_adb_ext_impl_t *extension_impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    extension_impl = (w2c_databinding_adb_ext_impl_t*)AXIS2_MALLOC( env-> allocator, 
                                          sizeof(w2c_databinding_adb_ext_impl_t) );
 
    if(NULL == extension_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }
    
    extension_impl->extension.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_extension_ops_t));
    if(NULL == extension_impl->extension.ops)
    {
        w2c_databinding_adb_ext_free(&(extension_impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }   
    extension_impl->extension.ops->free = 
                      w2c_databinding_adb_ext_free;
    extension_impl->extension.ops->engage = 
                      w2c_databinding_adb_ext_engage;

    *inst = &(extension_impl->extension);

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(w2c_extension_t *inst,
                      const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    if (inst)
    {
        status = W2C_EXTENSION_FREE(inst, env);
    }
    return status;
}

/** the following part will handle schema compiler */

axis2_status_t
w2c_databinding_adb_ext_invoke( const axis2_env_t *env,
        w2c_engine_configuration_t *conf)
{
    axis2_char_t *doc_base_uri = NULL;
    axis2_char_t *wsdl_name = NULL;
    axis2_array_list_t *list = NULL;
    axis2_array_list_t *schema_list = NULL;
    xml_schema_t *schema = NULL;
    int count = 0;
    int i = 0;
    axiom_document_t *om_doc = NULL;
    int non_null_schemas = 0;

    woden_resolver_t *resolver = NULL;
    void *desc = NULL;
    woden_desc_t *desc_ele = NULL;
    woden_wsdl10_desc_t *desc_ele10 = NULL;
    woden_types_t *types_ele = NULL;
    woden_schema_t *woden_schema = NULL;

    w2c_schema_compiler_options_t *options = NULL;
    w2c_schema_compiler_t *compiler = NULL;
    w2c_typemapper_t *processed_mapper = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    wsdl_name =
       W2C_ENGINE_CONFIGURATION_GET_WSDL_URI( conf, env);

    doc_base_uri = W2C_ENGINE_CONFIGURATION_GET_BASE_URI ( conf, env);

    om_doc = get_root_element_from_filename(env, wsdl_name);
    if ( NULL == om_doc )
    {
        return AXIS2_FAILURE;
    }

    resolver = woden_resolver_create(env);
    desc = (void *)WODEN_RESOLVER_READ(resolver , env, om_doc,
                                                            doc_base_uri);
    if (WODEN_RESOLVER_GET_SPEC( resolver, env )
                                            == WODEN_WSDL20 )
    {
        desc_ele = woden_desc_to_desc_element( desc, env);
        types_ele = WODEN_DESC_ELEMENT_GET_TYPES_ELEMENT( desc_ele, env);
    }
    else if (WODEN_RESOLVER_GET_SPEC( resolver, env )
                                            == WODEN_WSDL10 )
    {
        desc_ele10 = woden_wsdl10_desc_to_desc_element( desc, env);
        types_ele = WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT( desc_ele10, env); 
    }
    else
    {
        AXIOM_DOCUMENT_FREE( om_doc, env);
        return AXIS2_FAILURE;
    }
    list = WODEN_TYPES_ELEMENT_GET_SCHEMAS( types_ele, env);

    schema_list = axis2_array_list_create( env, 10);
    count = AXIS2_ARRAY_LIST_SIZE( list, env);
    for ( i = 0, non_null_schemas =0; i < count; i ++ )
    {
        woden_schema = ( woden_schema_t*)AXIS2_ARRAY_LIST_GET( list, env, i);

        schema =  WODEN_SCHEMA_GET_SCHEMA_DEF( woden_schema, env);
        if ( schema != NULL)
        {
            non_null_schemas ++;
            AXIS2_ARRAY_LIST_ADD( schema_list, env, schema);
        }
    }
    if( non_null_schemas == 0 )
    {
        status = AXIS2_FAILURE;
    }
    else
    {
        options = w2c_schema_compiler_options_create( env);
        populate_default_options( env, options, conf);
        
        compiler = w2c_schema_compiler_create( env, options);
        processed_mapper = W2C_SCHEMA_COMPILER_COMPILE_SCHEMA_LIST( compiler, env, schema_list);
    
        /** currently we support only for this mode */
        if( W2C_SCHEMA_COMPILER_OPTIONS_GET_WRITE_OUT( options, env) )
        {
            if( processed_mapper != NULL)
            {
                W2C_ENGINE_CONFIGURATION_SET_TYPEMAPPER( conf, env, processed_mapper);
            }
            else
            {
                status = AXIS2_FAILURE;
            }
        }
        else
        {
            status = AXIS2_FAILURE;
        }
    }
    if ( desc )
    {
        if (WODEN_RESOLVER_GET_SPEC( resolver, env )
                                                 == WODEN_WSDL10 )
        {
             WODEN_WSDL10_DESC_FREE ( desc, env);
        }
        else if (WODEN_RESOLVER_GET_SPEC( resolver, env)
                                                 == WODEN_WSDL20 )
        {
            WODEN_DESC_FREE ( desc, env);
        }
    }
    WODEN_RESOLVER_FREE( resolver, env);
    AXIOM_DOCUMENT_FREE( om_doc, env);

    return status;
}

axiom_document_t*
get_root_element_from_filename(
        const axis2_env_t *env,
        axis2_char_t *filename)
{
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;

    reader = axiom_xml_reader_create_for_file(env, filename, NULL);
    if ( NULL == reader )
    {
       return NULL;
    }
    om_builder = axiom_stax_builder_create(env, reader);
    doc = axiom_document_create(env, NULL, om_builder);
    AXIOM_DOCUMENT_BUILD_ALL(doc, env);

    return doc;
}


axis2_status_t
populate_default_options(
           const axis2_env_t *env,
           w2c_schema_compiler_options_t *options,
           w2c_engine_configuration_t *config)
{
    axis2_char_t *output_location = NULL;
    axis2_bool_t pack_classes = AXIS2_FALSE;
    axis2_char_t *language = NULL;

    W2C_ENGINE_CONFIGURATION_NAMEMAKER name_maker_func = NULL;
    w2c_qname2name_maker_t *qname2name_maker = NULL;

    output_location = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LOCATION( config, env);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_OUTPUT_LOCATION( options, env, output_location);

    /* packing classes is not yet implmemented*/
    pack_classes = W2C_ENGINE_CONFIGURATION_GET_PACK_CLASSES( config, env);
   
    W2C_SCHEMA_COMPILER_OPTIONS_SET_WRAP_CLASSES( options, env, pack_classes);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_WRITE_OUT( options, env, !pack_classes);

    language = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE( config, env);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_LANGUAGE( options, env, language); 
  
    name_maker_func = W2C_ENGINE_CONFIGURATION_GET_NAMEMAKER_FUNC( config, env); 
    W2C_SCHEMA_COMPILER_OPTIONS_SET_NAME_MAKER_FUNC( options, env, name_maker_func); 

    qname2name_maker = W2C_ENGINE_CONFIGURATION_GET_QNAME2NAME( config, env);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_QNAME2NAME_MAKER( options, env, qname2name_maker);
    return AXIS2_SUCCESS;
}
