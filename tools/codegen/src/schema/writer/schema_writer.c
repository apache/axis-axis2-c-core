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

#include "schema_writer_protected.h"
#include <w2c_xslt_template_processor.h>

/**
 * The following set of functions suppose to be implemented 
 * langugage indepependent manner. anyway it is currently mostly specific 
 * to 'c'
 */
AXIS2_EXTERN w2c_schema_writer_t * AXIS2_CALL
w2c_schema_writer_create (const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    writer_impl = (w2c_schema_writer_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_schema_writer_impl_t));

    if(NULL == writer_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    writer_impl-> template_loaded = AXIS2_FALSE;
    writer_impl-> wrap_classes = AXIS2_FALSE;
    writer_impl-> write_classes = AXIS2_FALSE;
    writer_impl-> package_name = NULL;
    writer_impl-> root_dir = NULL;
    writer_impl-> template_name = NULL;
    writer_impl-> global_wrapped_node = NULL;
    
    writer_impl-> model_map = axis2_hash_make(env);
    writer_impl-> names_list = axis2_hash_make(env);
    

    writer_impl-> base_type_map = NULL;
    writer_impl-> is_helper_mode = AXIS2_FALSE;
    writer_impl-> mapping_class_package = NULL;
    writer_impl-> options = NULL;



    writer_impl->uri2prefix_map = axis2_hash_make(env);
    writer_impl->prefix2uri_map = axis2_hash_make(env);
    writer_impl->last_prefix_index = 1;


    writer_impl->writer.ops =
    AXIS2_MALLOC (env->allocator, sizeof(w2c_schema_writer_ops_t));
    if(NULL == writer_impl->writer.ops)
    {
        w2c_schema_writer_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    writer_impl->writer.ops->free = w2c_schema_writer_free;
    writer_impl->writer.ops->get_model_map = w2c_schema_writer_get_model_map;
    writer_impl->writer.ops->init = w2c_schema_writer_init;
    writer_impl->writer.ops->write_element = w2c_schema_writer_write_element;
    writer_impl->writer.ops->write_complex_type = w2c_schema_writer_write_complex_type;
    writer_impl->writer.ops->write_batch = w2c_schema_writer_write_batch;
    writer_impl->writer.ops->write_simple_type = w2c_schema_writer_write_simple_type;
    writer_impl->writer.ops->make_fully_qualified_class_name = 
                         w2c_schema_writer_make_fully_qualified_class_name;
    writer_impl->writer.ops->register_extension_mapper_packagename =
                         w2c_schema_writer_register_extension_mapper_packagename;
    writer_impl->writer.ops->write_extension_mapper = 
                         w2c_schema_writer_write_extension_mapper;
    writer_impl->writer.ops->get_extension_mapper_package_name = 
                         w2c_schema_writer_get_extension_mapper_package_name;
  
    /** associate language specific functions with general function */
 
    writer_impl->writer_impl_ops =
            AXIS2_MALLOC (env->allocator, sizeof(w2c_schema_writer_impl_ops_t));
    if(NULL == writer_impl->writer_impl_ops)
    {
        w2c_schema_writer_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    writer_impl->writer_impl_ops->process = w2c_schema_writer_process;
    writer_impl->writer_impl_ops->create_out_file = w2c_schema_writer_create_out_file;
    writer_impl->writer_impl_ops->is_default_class = w2c_schema_writer_is_default_class;

    return &(writer_impl->writer);
}

axis2_status_t AXIS2_CALL 
w2c_schema_writer_free (w2c_schema_writer_t *schema_writer, 
                            const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    writer_impl = W2C_INTF_TO_IMPL(schema_writer);
    
    if( writer_impl-> package_name)
    {
        AXIS2_FREE( env-> allocator, writer_impl-> package_name);
    }
    if( writer_impl-> root_dir)
    {
        AXIS2_FREE( env-> allocator, writer_impl-> root_dir);
    }
    if( writer_impl-> template_name)
    {
        AXIS2_FREE( env-> allocator, writer_impl-> template_name);
    }
    if( writer_impl-> global_wrapped_node)
    {
        AXIOM_NODE_FREE_TREE( writer_impl-> global_wrapped_node, env);
    }
    
    if( writer_impl-> model_map)
    {
        axis2_hash_free( writer_impl-> model_map, env);
    }
    if( writer_impl-> names_list)
    {
        axis2_hash_free( writer_impl-> names_list, env);
    }
    if( writer_impl-> ns2packagename_map)
    {
        axis2_hash_free( writer_impl-> ns2packagename_map, env);
    }
    writer_impl-> is_helper_mode = AXIS2_FALSE;
    if( writer_impl-> mapping_class_package)
    {
        AXIS2_FREE( env-> allocator, writer_impl-> mapping_class_package);
    }


    if(schema_writer->ops)
    {
        AXIS2_FREE(env->allocator, schema_writer->ops);
        schema_writer->ops = NULL;
    }
    if(writer_impl)
    {
        AXIS2_FREE(env->allocator, writer_impl);
        writer_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}




axis2_hash_t* AXIS2_CALL
w2c_schema_writer_get_model_map( w2c_schema_writer_t *writer,
        const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;

	AXIS2_ENV_CHECK(env, NULL);

    writer_impl = W2C_INTF_TO_IMPL(writer);

    return writer_impl-> model_map;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_init( w2c_schema_writer_t *writer,
         const axis2_env_t *env,
         w2c_schema_compiler_options_t *options,
         axis2_char_t* template_name,
         axis2_hash_t* base_type_map)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    axis2_char_t *output = NULL;
    axis2_char_t *temp_package = NULL;
    int length = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    writer_impl = W2C_INTF_TO_IMPL(writer);

    writer_impl-> template_name = AXIS2_STRDUP( template_name, env);
    writer_impl-> base_type_map = base_type_map;

    output = W2C_SCHEMA_COMPILER_OPTIONS_GET_OUTPUT_LOCATION( options, env);
    w2c_schema_writer_init_with_file( writer_impl, env, output);
    writer_impl-> package_name = 
        W2C_SCHEMA_COMPILER_OPTIONS_GET_PACKAGE_NAME( options, env);
    writer_impl-> write_classes = W2C_SCHEMA_COMPILER_OPTIONS_GET_WRITE_OUT( options, env);
    if( !writer_impl-> write_classes)
    {
        writer_impl-> wrap_classes = AXIS2_FALSE;
    }
    else
    {
        writer_impl-> wrap_classes =
            W2C_SCHEMA_COMPILER_OPTIONS_GET_WRAP_CLASSES( options, env);
    }
    /* if the wrap mode is set then create a global document to keep the
     * wrapped
     * element models
     */
    if( !writer_impl->wrap_classes)
    {
        writer_impl-> global_wrapped_node = 
                 w2c_xslt_utils_add_child_node( env, "class", NULL);
        w2c_xslt_utils_add_attribute (env, writer_impl-> global_wrapped_node,
                "name", W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME);
        if ( writer_impl-> package_name != NULL)
        {
            length = AXIS2_STRLEN( writer_impl-> package_name);
            if ( writer_impl-> package_name[length-1] == '.' )
            {
                writer_impl-> package_name[length-1] = '\0';
                temp_package = writer_impl-> package_name;
            }
        }
        if ( temp_package == NULL )
        {
            temp_package = W2C_SCHEMA_WRITER_DEFAULT_PACKAGE;
        }
        w2c_xslt_utils_add_attribute (env, writer_impl-> global_wrapped_node,
                "package", temp_package);

    }
    writer_impl-> ns2packagename_map = 
        W2C_SCHEMA_COMPILER_OPTIONS_GET_NS2PACKAGE_MAP( options, env);
    writer_impl-> is_helper_mode = 
        W2C_SCHEMA_COMPILER_OPTIONS_GET_HELPER_MODE( options, env);
    if ( W2C_SCHEMA_COMPILER_OPTIONS_GET_MAPPER_CLASS_PACKAGE( options, env)!= NULL)
    {
        writer_impl-> mapping_class_package =
              W2C_SCHEMA_COMPILER_OPTIONS_GET_MAPPER_CLASS_PACKAGE( options, env);
    }
    writer_impl-> options = options;

    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_element( w2c_schema_writer_t *writer,
         const axis2_env_t *env,
         xml_schema_element_t *element,
         axis2_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info)
{
    axis2_qname_t *qname;
    w2c_schema_writer_impl_t *writer_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    writer_impl = W2C_INTF_TO_IMPL(writer);

    qname = XML_SCHEMA_ELEMENT_GET_QNAME( element, env);
    return W2C_SCHEMA_WRITER_PROCESS( writer_impl, env, qname, meta_info, typemap, AXIS2_TRUE);
}

axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_complex_type( w2c_schema_writer_t *writer,
         const axis2_env_t *env,
         xml_schema_complex_type_t *complex_type,
         axis2_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info)
{
    axis2_qname_t *qname = NULL;
    w2c_schema_writer_impl_t *writer_impl = NULL;
    xml_schema_type_t *type = NULL;
    axis2_hash_t *base_hash = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    writer_impl = W2C_INTF_TO_IMPL(writer);

    base_hash = XML_SCHEMA_COMPLEX_TYPE_SUPER_OBJS( complex_type, env);
    type = (xml_schema_type_t*)axis2_hash_get( base_hash, "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING);
    
    qname = XML_SCHEMA_TYPE_GET_QNAME( type, env);
    return W2C_SCHEMA_WRITER_PROCESS( writer_impl, env, qname, meta_info, typemap, AXIS2_FALSE);
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_write_batch( w2c_schema_writer_t *writer,
                 const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    axis2_char_t *temp_package = NULL;
    axis2_char_t *out = NULL;
    axis2_char_t *source_template = NULL;
    axis2_char_t *header_template = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    writer_impl = W2C_INTF_TO_IMPL(writer);
    if( writer_impl-> wrap_classes)
    {
        if ( NULL == writer_impl-> package_name)
        {
            temp_package = W2C_SCHEMA_WRITER_DEFAULT_PACKAGE;
        }
        else
        {
            temp_package = writer_impl-> package_name;
        }
        out = W2C_SCHEMA_WRITER_CREATE_OUT_FILE( writer_impl, env, 
                      temp_package, W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".c");
        /* parse with the template and create the files */
        source_template = axis2_stracat( writer_impl-> template_name, "Source.xsl", env);
        w2c_schema_writer_parse( writer_impl, env,
                      writer_impl-> global_wrapped_node, out, source_template);
        out = W2C_SCHEMA_WRITER_CREATE_OUT_FILE( writer_impl, env, 
                      temp_package, W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".h");
        /* parse with the template and create the files */
        header_template = axis2_stracat( writer_impl-> template_name, "Header.xsl", env);
        w2c_schema_writer_parse( writer_impl, env,
                      writer_impl-> global_wrapped_node, out, header_template);
        AXIS2_FREE( env-> allocator, source_template);
        AXIS2_FREE( env-> allocator, header_template);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_simple_type( w2c_schema_writer_t *writer,
         const axis2_env_t *env,
         xml_schema_complex_type_t *type,
         axis2_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info)
{
    AXIS2_ENV_CHECK(env, NULL);
    w2c_messages_print_n_log_error( env, "schema.notImplemented");
	return NULL;
}

axis2_status_t
w2c_schema_writer_init_with_file( w2c_schema_writer_impl_t *writer_impl,
         const axis2_env_t *env,
         axis2_char_t *root_dir)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if( root_dir == NULL )
    {
        root_dir = AXIS2_STRDUP(".", env);
    }

    writer_impl-> root_dir = root_dir;
    writer_impl-> names_list = axis2_hash_make(env);

    return AXIS2_SUCCESS;
}

/** this should be overrided in laguage specific writer */
axis2_char_t* AXIS2_CALL
w2c_schema_writer_make_fully_qualified_class_name( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_char_t *ns_uri = NULL;
    axis2_char_t *package_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_char_t *original_name = NULL;
    axis2_char_t *package_prefix = NULL;
    axis2_char_t *fully_qualified_class_name = NULL;
    int length = 0;
    w2c_qname2name_maker_t *qname2name_maker = NULL;
    W2C_ENGINE_CONFIGURATION_NAMEMAKER name_maker = NULL;
    w2c_schema_writer_impl_t *writer_impl = NULL;

	AXIS2_ENV_CHECK(env, NULL);

    writer_impl = W2C_INTF_TO_IMPL(writer);
    ns_uri = AXIS2_QNAME_GET_URI( qname, env);

    qname2name_maker =
        W2C_SCHEMA_COMPILER_OPTIONS_GET_QNAME2NAME_MAKER( writer_impl-> options, env);

    if ( qname2name_maker == NULL)
    {
        original_name = AXIS2_QNAME_GET_LOCALPART( qname, env);
    }
    else
    {
        original_name = 
            W2C_QNAME2NAME_MAKER_SUGGEST_NAME( qname2name_maker, env, qname);
    }
    package_name = w2c_schema_writer_get_package(writer_impl, env, ns_uri );

    name_maker = W2C_SCHEMA_COMPILER_OPTIONS_GET_NAME_MAKER_FUNC
                                            ( writer_impl-> options, env);
    class_name = w2c_schema_writer_make_unique_class_name( writer_impl, env,
            writer_impl-> names_list, original_name);
    class_name = name_maker( class_name, env);
    if( writer_impl-> wrap_classes)
    {
        package_prefix = 
            (writer_impl-> package_name == NULL)? 
                        (W2C_SCHEMA_WRITER_DEFAULT_PACKAGE "."):writer_impl-> package_name;
        package_prefix = AXIS2_STRDUP( package_prefix, env);
        package_prefix = w2c_string_add_string( package_prefix, 
                        W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, env);
    }
    else if ( writer_impl-> write_classes)
    {
        package_prefix = package_name;
    }
    if( package_prefix != NULL)
    {
        length = AXIS2_STRLEN( fully_qualified_class_name);
        fully_qualified_class_name = axis2_stracat(package_prefix,
                (package_prefix[length-1]=='.')?"":".", env);
        fully_qualified_class_name = w2c_string_add_string(fully_qualified_class_name,
                class_name, env );
        AXIS2_FREE( env-> allocator, class_name);
    }
    else
    {
        fully_qualified_class_name = class_name;
    }
    return fully_qualified_class_name;
}

axis2_char_t*
w2c_schema_writer_get_package( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_char_t *namespace_uri)
{
    axis2_char_t *base_package_name = NULL;
    axis2_char_t *package_name = NULL;

    if(  writer_impl-> ns2packagename_map)
    {
        base_package_name = (axis2_char_t*)axis2_hash_get( writer_impl-> ns2packagename_map, namespace_uri,
             AXIS2_HASH_KEY_STRING);
    }
    if ( NULL == base_package_name)
    {
        base_package_name = w2c_url_processor_make_package_name( env, namespace_uri);
    }
    if( NULL == writer_impl-> package_name)
    {
        package_name = AXIS2_STRDUP( base_package_name, env);
    }
    else
    {
        package_name = axis2_stracat(writer_impl-> package_name, base_package_name, env);
    }
    return package_name;
}

axis2_char_t*
w2c_schema_writer_process( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_qname_t *qname,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axis2_hash_t *typemap,
                        axis2_bool_t is_element)
{
    axis2_char_t *fully_qualified_class_name = NULL;
    int length = 0;
    axis2_char_t *original_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_hash_t *property_names = NULL;
    axiom_node_t *model_source_node = NULL;
    axis2_char_t *out = NULL;
    axis2_char_t *model_name = NULL;
    axis2_char_t *source_template = NULL;
    axis2_char_t *namespace_uri = NULL;

    fully_qualified_class_name = W2C_SCHEMA_WRITER_META_INFO_GET_OWN_CLASSNAME( meta_info, env);
    if( NULL == fully_qualified_class_name )
    {
        fully_qualified_class_name =
            W2C_SCHEMA_WRITER_MAKE_FULLY_QUALIFIED_CLASS_NAME( &(writer_impl->writer), env, qname);
    }
    length = AXIS2_STRLEN( fully_qualified_class_name);
    for ( ; length>=0 && fully_qualified_class_name[length]!= '.'; length -- );
    class_name = fully_qualified_class_name+length;

    original_name = w2c_string_make_key_from_qname( qname, env);
    property_names = axis2_hash_make( env);

    if( writer_impl-> template_loaded)
    {
        w2c_schema_writer_load_template( writer_impl, env);
    }
    /* for wrapped class, if it already created childs will be added */
    if( writer_impl-> wrap_classes)
    {
        writer_impl-> global_wrapped_node = w2c_schema_writer_make_element( writer_impl, env, 
           class_name, original_name, qname, is_element, meta_info, property_names, typemap); 
    }
    else
    {
        model_source_node = w2c_schema_writer_make_element( writer_impl, env, 
           class_name, original_name, qname, is_element, meta_info, property_names, typemap); 
        if(writer_impl-> write_classes)
        {
            out = W2C_SCHEMA_WRITER_CREATE_OUT_FILE( writer_impl, env, 
                          "", W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".c");
            /* parse with the template and create the files */
            source_template = axis2_stracat( writer_impl-> template_name, "Source.xsl", env);
            w2c_schema_writer_parse( writer_impl, env,
                          model_source_node, out, source_template);
        }
        namespace_uri = AXIS2_QNAME_GET_URI( qname, env);
        model_name = axis2_stracat( class_name, "|", env);
        model_name = w2c_string_add_string( model_name, namespace_uri, env);
        axis2_hash_set( writer_impl-> model_map,  model_name, AXIS2_HASH_KEY_STRING, model_source_node);
    }
    return fully_qualified_class_name;
}

/** some shorten way to add attribs */
#define ADD_ATTRIBUTE(where, key, value) \
    temp_name = name_maker( value, env);\
    w2c_xslt_utils_add_attribute( env, where, key, temp_name);\
    AXIS2_FREE( env-> allocator, temp_name);


axiom_node_t*
w2c_schema_writer_make_element( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_char_t *class_name,
                        axis2_char_t *original_name,
                        axis2_qname_t *qname,
                        axis2_bool_t is_element,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axis2_hash_t *property_names,
                        axis2_hash_t *typemap)
{
    axis2_char_t *ns_uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *caps_name = NULL;
    axis2_char_t *extension_class_name = NULL;
    axiom_node_t *model = NULL;
    axis2_char_t *temp_name = NULL;
    W2C_ENGINE_CONFIGURATION_NAMEMAKER name_maker = NULL;

    name_maker = W2C_SCHEMA_COMPILER_OPTIONS_GET_NAME_MAKER_FUNC
                                            ( writer_impl-> options, env);
    if ( model == NULL)
    {
       model =  w2c_xslt_utils_add_child_node( env, "class", NULL);
    }
    ns_uri = AXIS2_QNAME_GET_URI( qname, env);
    prefix = AXIS2_QNAME_GET_PREFIX( qname, env);

    prefix = w2c_schema_writer_get_prefix4uri( writer_impl, env, ns_uri, prefix);

    /** class_name is already name maken */
    w2c_xslt_utils_add_attribute( env, model, "name", class_name);

    caps_name = AXIS2_STRDUP( class_name, env); 
    caps_name = axis2_string_toupper( caps_name);
    w2c_xslt_utils_add_attribute( env, model, "caps-name", caps_name);
    AXIS2_FREE( env-> allocator, caps_name);
 
    w2c_xslt_utils_add_attribute( env, model, "originalName", original_name);
    ADD_ATTRIBUTE(model, "nsuri", ns_uri);
    ADD_ATTRIBUTE(model, "nsprefix", prefix);
      
    if(!writer_impl-> wrap_classes)
        w2c_xslt_utils_add_attribute( env, model, "unwrapped", "yes");
    if(!writer_impl-> write_classes)
        w2c_xslt_utils_add_attribute( env, model, "skip-write", "yes");
    if(!is_element)
        w2c_xslt_utils_add_attribute( env, model, "type", "yes");

    if(W2C_SCHEMA_WRITER_META_INFO_GET_ANONYMOUS( meta_info, env))
        w2c_xslt_utils_add_attribute( env, model, "anon", "yes");
    if(W2C_SCHEMA_WRITER_META_INFO_GET_EXTENSION( meta_info, env))
    {
        extension_class_name = W2C_SCHEMA_WRITER_META_INFO_GET_EXTENSION_CLASSNAME( meta_info, env);
        ADD_ATTRIBUTE(model, "extension", extension_class_name);
    }

    if(W2C_SCHEMA_WRITER_META_INFO_GET_CHOICE( meta_info, env))
        w2c_xslt_utils_add_attribute( env, model, "choise", "yes");
    if(W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED( meta_info, env))
    {
        w2c_xslt_utils_add_attribute( env, model, "ordered", "yes");
    }
    
    if(is_element && 
            W2C_SCHEMA_WRITER_META_INFO_IS_NILLABLE( meta_info, env, qname))
        w2c_xslt_utils_add_attribute( env, model, "nillable", "yes");

    /* populate all the information */
    w2c_schema_writer_populate_info( writer_impl, env, 
            meta_info, model, property_names, typemap, AXIS2_FALSE);
    return model;
}

axis2_status_t 
w2c_schema_writer_populate_info( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axiom_node_t *model,
                        axis2_hash_t *property_names,
                        axis2_hash_t *typemap,
                        axis2_bool_t is_inherited)
{
    w2c_schema_writer_meta_info_t *parent_meta_info = NULL;
    parent_meta_info = W2C_SCHEMA_WRITER_META_INFO_GET_PARENT( meta_info, env);
    if( parent_meta_info != NULL)
    {
        /* recursively do for all the parents */
        w2c_schema_writer_populate_info( writer_impl, env, 
            parent_meta_info, model, property_names, typemap, AXIS2_TRUE);
    }
    w2c_schema_writer_add_property_entries( writer_impl, env, 
        meta_info, model, property_names, typemap, is_inherited);
    return AXIS2_SUCCESS;
}

axis2_status_t 
w2c_schema_writer_add_property_entries( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axiom_node_t *model,
                        axis2_hash_t *property_names,
                        axis2_hash_t *typemap,
                        axis2_bool_t is_inherited)
{
    axis2_array_list_t *qnames = NULL;
    int i = 0;
    int size = 0;
    axis2_qname_t *qname = NULL;
    axiom_node_t *property = NULL;
    axis2_char_t *localpart = NULL;
    axis2_char_t *unique_name = NULL;
    axis2_char_t *nsuri = NULL;
    axis2_char_t *classname= NULL;
    axis2_char_t *caps_name = NULL;
    axis2_char_t *caps_type = NULL;
    /*axis2_bool_t is_primitive = AXIS2_FALSE;*/
    axis2_bool_t is_default = AXIS2_FALSE;
    w2c_schema_writer_meta_info_t *parent_meta_info = NULL;
    axis2_qname_t *schema_qname = NULL;
    axis2_char_t *qname_str = NULL;
    void* schema_type = NULL;
    axis2_char_t *short_type_name = NULL;
    int minoccurs = 0;
    int maxoccurs = 0;
    axis2_char_t number_str[32];
    W2C_ENGINE_CONFIGURATION_NAMEMAKER name_maker = NULL;
    w2c_qname2name_maker_t *qname2name_maker = NULL;
    axis2_char_t *original_name = NULL;
    axis2_char_t *prefix = NULL;
    
    name_maker = W2C_SCHEMA_COMPILER_OPTIONS_GET_NAME_MAKER_FUNC
                                            ( writer_impl-> options, env);
    qname2name_maker =
        W2C_SCHEMA_COMPILER_OPTIONS_GET_QNAME2NAME_MAKER( writer_impl-> options, env);

    if ( W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED( meta_info, env) )
    {
        qnames = W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED_QNAME_ARRAY( meta_info, env);
    }
    else
    {
        qnames = W2C_SCHEMA_WRITER_META_INFO_GET_QNAME_ARRAY( meta_info, env);
    }
    size = AXIS2_ARRAY_LIST_SIZE( qnames, env);
    for( i = 0; i < size; i ++)
    {
        qname = AXIS2_ARRAY_LIST_GET( qnames, env, i);
        property = w2c_xslt_utils_add_child_node( env, "property", model);
    
        if ( qname2name_maker == NULL)
        {
            localpart = AXIS2_QNAME_GET_LOCALPART( qname, env);
        }
        else
        {
            localpart = 
                W2C_QNAME2NAME_MAKER_SUGGEST_NAME( qname2name_maker, env, qname);
        }
        prefix = AXIS2_QNAME_GET_PREFIX( qname, env);
        original_name = AXIS2_QNAME_GET_LOCALPART( qname, env);
        nsuri = AXIS2_QNAME_GET_URI( qname, env);
        unique_name = w2c_schema_writer_make_unique_class_name( 
                writer_impl, env, property_names, localpart);
        parent_meta_info = W2C_SCHEMA_WRITER_META_INFO_GET_PARENT( meta_info, env);

        unique_name = name_maker( unique_name, env);
        w2c_xslt_utils_add_attribute( env, property, "name", original_name);
        w2c_xslt_utils_add_attribute( env, property, "cname", unique_name);
        w2c_xslt_utils_add_attribute( env, property, "nsuri", nsuri);
        w2c_xslt_utils_add_attribute( env, property, "prefix", prefix);

       
        classname =  
          W2C_SCHEMA_WRITER_META_INFO_GET_CLASSNAME4QNAME( meta_info, env, qname);
        if( NULL != classname )
        {
            w2c_xslt_utils_add_attribute( env, property, "type", classname);
            caps_type = AXIS2_STRDUP( classname, env);
            caps_type = axis2_string_toupper( caps_type);
            w2c_xslt_utils_add_attribute( env, property, "caps-type", caps_type);
            AXIS2_FREE( env-> allocator, caps_type);
        }

             
        caps_name = AXIS2_STRDUP( unique_name, env);

        caps_name = axis2_string_toupper( caps_name);
        w2c_xslt_utils_add_attribute( env, property, "caps-cname", caps_name);
        AXIS2_FREE( env-> allocator, caps_name);

        /*
        is_primitive = w2c_schema_writer_is_primitive( classname, env);
        if ( is_primitive)
        {
            w2c_xslt_utils_add_attribute( env, property, "primitive", "yes");
        }*/
        /* add an attribute that says the type is default */
        is_default = W2C_SCHEMA_WRITER_IS_DEFAULT_CLASS( writer_impl, env, classname);
        if ( is_default)
        {
            w2c_xslt_utils_add_attribute( env, property, "default", "yes");
        }
        qname_str = NULL;
        schema_qname = W2C_SCHEMA_WRITER_META_INFO_GET_SCHEMAQNAME4QNAME( meta_info, env, qname);
        if ( schema_qname != NULL)
        {
            qname_str = w2c_string_make_key_from_qname( schema_qname, env);
            schema_type = axis2_hash_get( typemap, qname_str, AXIS2_HASH_KEY_STRING);
        }
        if ( schema_type != NULL)
        {
            w2c_xslt_utils_add_attribute( env, property, "ours", "yes");
        }
        if ( W2C_SCHEMA_WRITER_META_INFO_GET_ATTRI_STATUS4QNAME( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "attribute", "yes");
        }
        if( W2C_SCHEMA_WRITER_META_INFO_IS_NILLABLE( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "nillable", "yes");
        }
        if( qname_str != NULL)
        {
            schema_type = axis2_hash_get(writer_impl-> base_type_map , qname_str, AXIS2_HASH_KEY_STRING);
        }
        if ( schema_qname != NULL && schema_type != NULL)
        {
            short_type_name = AXIS2_QNAME_GET_LOCALPART( schema_qname, env);
        }
        else
        {
            short_type_name = classname;
        }

        if( is_inherited)
        {
            w2c_xslt_utils_add_attribute( env, property, "inherited", "yes");
        }

        if( W2C_SCHEMA_WRITER_META_INFO_GET_ANY_STATUS4QNAME( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "any", "yes");
        }
        if( W2C_SCHEMA_WRITER_META_INFO_GET_BINARY_STATUS4QNAME( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "binary", "yes");
        }

        minoccurs = W2C_SCHEMA_WRITER_META_INFO_GET_MIN_OCCURS( meta_info, env, qname);
        sprintf(number_str, "%d", minoccurs);
        w2c_xslt_utils_add_attribute( env, property, "minOccurs", number_str);

        if ( W2C_SCHEMA_WRITER_META_INFO_GET_ARRAY_STATUS4QNAME( meta_info, env, qname) )
        {
            w2c_xslt_utils_add_attribute( env, property, "isarray", "yes");
            w2c_xslt_utils_add_attribute( env, property, "arrayBaseType", classname);
        }
        maxoccurs = W2C_SCHEMA_WRITER_META_INFO_GET_MAX_OCCURS( meta_info, env, qname);
        if ( maxoccurs == LONG_MAX )
        {
            w2c_xslt_utils_add_attribute( env, property, "unbound", "yes");
        }
        sprintf(number_str, "%d", maxoccurs);
        w2c_xslt_utils_add_attribute( env, property, "maxOccurs", number_str);
    }
    return AXIS2_SUCCESS;
}

/** this should be overidden by some language specific class */
axis2_bool_t 
w2c_schema_writer_is_default_class( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_char_t *java_class_name4element)
{
    return !(AXIS2_STRCMP( java_class_name4element, "DEFAULT_CLASS_NAME")) ||
           !(AXIS2_STRCMP( java_class_name4element, "DEFAULT_CLASS_ARRAY_NAME"));
}

axis2_char_t*
w2c_schema_writer_make_unique_class_name( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_hash_t *list_of_names,
                        axis2_char_t *xml_name)
{
    axis2_char_t *class_name = NULL;
    int present = 0;
    axis2_char_t count_str[32];
    static int count = 0;

    class_name = AXIS2_STRDUP( xml_name, env);
    present = (int)axis2_hash_get( list_of_names, xml_name, AXIS2_HASH_KEY_STRING);
    if ( present)
    {
        sprintf( count_str, "%d", count);        
        count ++;
        class_name = w2c_string_add_string( class_name, count_str, env);
    }
    else
    {
        axis2_hash_set( list_of_names, xml_name, AXIS2_HASH_KEY_STRING, (void*)1);
    }
    return class_name;
}

axis2_status_t
w2c_schema_writer_load_template( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env)
{
    if ( writer_impl-> template_name == NULL)
    {
        return AXIS2_FAILURE;
    }
    writer_impl-> template_loaded = AXIS2_TRUE;
    return AXIS2_SUCCESS;
}

axis2_char_t*
w2c_schema_writer_create_out_file( w2c_schema_writer_impl_t *writer_impl,
                    const axis2_env_t *env,
                    axis2_char_t *package_name,
                    axis2_char_t *name,
                    axis2_char_t *extension)
{
    return w2c_file_writer_create_classfile_name( env, writer_impl->root_dir,
                package_name, name, extension);
}

axis2_status_t
w2c_schema_writer_parse( w2c_schema_writer_impl_t *writer_impl,
                    const axis2_env_t *env,
                    axiom_node_t *root,
                    axis2_char_t *out_file,
                    axis2_char_t *template_name)
{
    axis2_char_t *buffer = NULL;
    axis2_char_t *full_path = NULL;
    buffer = w2c_xslt_utils_serialize(env, root);
    /*printf("%s\n", buffer);*/

    full_path = getenv("AXIS2C_HOME");
    full_path = AXIS2_STRDUP ( full_path, env);
    full_path = w2c_string_add_string(full_path, W2C_SCHEMA_WRITER_XSLT_PATH, env );
    full_path = w2c_string_add_string(full_path, template_name, env );

    w2c_xslt_template_processor_parse (env, buffer,
                         full_path,
                         out_file);
    AXIS2_FREE( env-> allocator, full_path);
    return AXIS2_SUCCESS;
}

/**
 * Get a prefix for the given namespace URI. If one has already been defined
 * in this serialization, use that. Otherwise, map the passed default prefix
 * to the URI, and return that. If a null default prefix is passed, use one
 * of the form "ns<num>"
 */

axis2_char_t*
w2c_schema_writer_get_prefix4uri( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_char_t *uri,
                        axis2_char_t *default_prefix)
{
    axis2_char_t count_str[32];
    axis2_char_t *prefix = NULL;

    if ( uri == NULL || AXIS2_STRLEN( uri) ==0 )
    {
        return NULL;
    }
    prefix = axis2_hash_get( writer_impl-> uri2prefix_map, uri, AXIS2_HASH_KEY_STRING);
    if( prefix == NULL)
    {
        if ( default_prefix == NULL || 0 == AXIS2_STRLEN( default_prefix) )
        {
            sprintf( count_str, "%d", writer_impl->last_prefix_index ++ );
            prefix = axis2_stracat( "ns", count_str, env);
        }
        else
        {
            prefix = AXIS2_STRDUP( default_prefix, env);
        }
        axis2_hash_set(writer_impl-> prefix2uri_map, prefix, AXIS2_HASH_KEY_STRING, uri);
        axis2_hash_set(writer_impl-> uri2prefix_map, uri, AXIS2_HASH_KEY_STRING, prefix);
    }
    else
    {
        prefix = AXIS2_STRDUP( prefix, env);
    }
    return prefix;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_register_extension_mapper_packagename(w2c_schema_writer_t *writer,
                                        const axis2_env_t *env,
                                        axis2_char_t *packagename)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_impl = W2C_INTF_TO_IMPL(writer);
    writer_impl-> mapping_class_package = AXIS2_STRDUP( packagename, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_write_extension_mapper( w2c_schema_writer_t *writer,
                                const axis2_env_t *env,
                                axis2_hash_t *meta_info_array)
{
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
w2c_schema_writer_get_extension_mapper_package_name(w2c_schema_writer_t *writer,
                                        const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_impl = W2C_INTF_TO_IMPL(writer);
    return writer_impl-> mapping_class_package;
}

