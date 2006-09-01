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

#include <w2c_schema_writer.h>
#include <w2c_schema_writer_meta_info.h>
#include <w2c_schema_compiler_options.h>
#include <w2c_messages.h>
#include <w2c_schema_property_loader.h>

/**
 * @brief
 */
#define W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME  "WrappedDatabinder"
#define W2C_SCHEMA_WRITER_DEFAULT_PACKAGE "adb"
#define W2C_SCHEMA_WRITER_EXTENSION_MAPPER_CLASSNAME "ExtensionMapper"

typedef struct w2c_schema_writer_impl
{
    w2c_schema_writer_t writer;

    axis2_char_t *template_name;
    axis2_bool_t template_loaded;
    axis2_array_list_t *names_list;
    int count;
    axis2_bool_t *wrap_classes;
    axis2_bool_t *writeClasses;
    axis2_char_t *package_name;
    axis2_char_t *root_dir;
    axis2_node_t *global_wrapped_source_node;
    axis2_node_t *global_wrapped_header_node;
    axis2_hash_t *model_map;
    
    w2c_typemapper_t *base_type_map;
    axis2_hash_t *ns2packagename_map;
    axis2_bool_t is_helper_mode;
    /**
     * package for the mapping class
     */
    axis2_char_t *mapping_class_package;

    w2c_schema_property_loader_t *loader;

} w2c_schema_writer_impl_t;

axis2_status_t AXIS2_CALL
w2c_schema_writer_free (w2c_schema_writer_t *schema_writer,
                                    const axis2_env_t *env);

axis2_char_t*AXIS2_CALL
w2c_schema_writer_get_output_location( w2c_schema_writer_t *writer,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_set_output_location( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_char_t *output_location);

axis2_char_t* AXIS2_CALL
w2c_schema_writer_get_package_name( w2c_schema_writer_t *writer,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_set_package_name( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_char_t *package_name);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_get_helper_mode( w2c_schema_writer_t *writer,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_set_elper_mode( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_bool_t helper_mode);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_get_write_out( w2c_schema_writer_t *writer,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_set_write_out( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_bool_t write_out);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_get_wrap_classes( w2c_schema_writer_t *writer,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_set_wrap_classes( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_bool_t wrap_classes);

axis2_hash_t* AXIS2_CALLL
w2c_schema_writer_get_ns2package_map( w2c_schema_writer_t *writer,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_set_ns2package_map( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_hash_t *ns2package_map);

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
    writer_impl->type = NULL;
    writer_impl->values_arr = NULL;
    writer_impl->output_location = NULL;
    writer_impl->package_name = NULL;
    writer_impl->helper_mode = AXIS2_FALSE;
    writer_impl->write_out = AXIS2_FALSE;
    writer_impl->wrap_classes = AXIS2_FALSE;
    writer_impl->ns2package_map = NULL;


    writer_impl->writer.ops =
    AXIS2_MALLOC (env->allocator, sizeof(w2c_schema_writer_ops_t));
    if(NULL == writer_impl->writer.ops)
    {
        w2c_schema_writer_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    writer_impl->writer.ops->free = w2c_schema_writer_free;
    
    writer_impl->writer.ops->get_output_location =
                            w2c_schema_writer_get_output_location;
    writer_impl->writer.ops->set_output_location =
                            w2c_schema_writer_set_output_location;
    writer_impl->compiler_option.ops->get_package_name =
                            w2c_schema_writer_get_package_name;
    writer_impl->compiler_option.ops->set_package_name =
                            w2c_schema_writer_set_package_name;
    writer_impl->compiler_option.ops->get_helper_mode =
                            w2c_schema_writer_get_helper_mode;
    writer_impl->compiler_option.ops->set_helper_mode =
                            w2c_schema_writer_set_helper_mode;
    writer_impl->compiler_option.ops->get_write_out =
                            w2c_schema_writer_get_write_out;
    writer_impl->compiler_option.ops->set_write_out =
                            w2c_schema_writer_set_write_out;
    writer_impl->compiler_option.ops->get_wrap_classes =
                            w2c_schema_writer_get_wrap_classes;
    writer_impl->compiler_option.ops->set_wrap_classes =
                            w2c_schema_writer_set_wrap_classes;
    writer_impl->compiler_option.ops->get_ns2package_map =
                            w2c_schema_writer_get_ns2package_map;
    writer_impl->compiler_option.ops->set_ns2package_map =
                            w2c_schema_writer_set_ns2package_map;

    return &(writer_impl->writer);
}

axis2_status_t AXIS2_CALL 
w2c_schema_writer_free (w2c_schema_writer_t *schema_writer, 
                            const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *schema_writer_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_writer_impl = W2C_INTF_TO_IMPL(schema_writer);
    
    if (schema_writer_impl-> output_location)
    {
        AXIS2_FREE(env-> allocator, schema_writer_impl-> output_location);
    }
    if (schema_writer_impl-> package_name)
    {
        AXIS2_FREE(env-> allocator, schema_writer_impl-> package_name);
    }
    if (schema_writer_impl-> ns2package_map)
    {
        axis2_hash_free(schema_writer_impl-> ns2package_map, env);
    }
    if(schema_writer->ops)
    {
        AXIS2_FREE(env->allocator, schema_writer->ops);
        schema_writer->ops = NULL;
    }
    if(schema_writer_impl)
    {
        AXIS2_FREE(env->allocator, schema_writer_impl);
        schema_writer_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}




axis2_hash_t* AXIS2_CALL
w2c_schema_writer_get_model_map( w2c_schema_writer_t *writer,
        const axis2_env_t *env)
{
    w2c_schema_writer_t *writer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    writer_impl = W2C_INTF_TO_IMPL(writer);

    return writer_impl-> model_map;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_init( w2c_schema_writer_t *writer,
         const axis2_env_t *env,
         w2c_schema_compler_options_t *options)
{
    w2c_schema_writer_t *writer_impl = NULL;
    axis2_char_t *output = NULL;
    axis2_bool_t write_classes = AXIS2_FALSE;
    axis2_char_t *temp_package = NULL;
    int length = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    writer_impl = W2C_INTF_TO_IMPL(writer);

    output = W2C_SCHEMA_COMPILER_OPTIONS_GET_OUTPUT_LOCATION( options, env);
    W2C_SCHEMA_WRITER_INIT_WITH_FILE( writer_impl, env, output);
    writer_impl-> package_name = 
        W2C_SCHEMA_COMPILER_OPTIONS_GET_PACKAGE_NAME( options, env);
    write_classes = W2C_SCHEMA_COMPILER_OPTIONS_GET_WRITE_OUT( options, env);
    if( !write_classes)
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
    if( !wrap_classes)
    {
        writer_impl-> global_wrapped_source_node = 
                 w2c_xslt_utils_add_child_node( env, "bean", NULL);
        w2c_xslt_utils_add_attribute (env, writer_impl-> global_wrapped_source_node,
                "name", W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME);
        if ( writer_impl-> package_name != NULL)
        {
            length = AXIS2_STRLEN( writer_impl-> package_name);
            if ( writer_impl-> package_name[length-1] == '.' )
            {
                writer_impl-> package_name[length-1] == '\0';
                temp_package = writer_impl-> package_name;
            }
        }
        if ( temp_package == NULL )
        {
            temp_package = W2C_SCHEMA_WRITER_DEFAULT_PACKAGE;
        }
        w2c_xslt_utils_add_attribute (env, writer_impl-> global_wrapped_source_node,
                "package", temp_package);

        writer_impl-> global_wrapped_header_node = 
                 w2c_xslt_utils_add_child_node( env, "bean", NULL);
        w2c_xslt_utils_add_attribute (env, writer_impl-> global_wrapped_header_node,
                "name", W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME);
        if ( writer_impl-> package_name != NULL)
        {
            length = AXIS2_STRLEN( writer_impl-> package_name);
            if ( writer_impl-> package_name[length-1] == '.' )
            {
                writer_impl-> package_name[length-1] == '\0';
                temp_package = writer_impl-> package_name;
            }
        }
        if ( temp_package == NULL )
        {
            temp_package = W2C_SCHEMA_WRITER_DEFAULT_PACKAGE;
        }
        w2c_xslt_utils_add_attribute (env, writer_impl-> global_wrapped_header_node,
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

    AXIS2_ENV_CHECK(env, NULL);

    qname = XML_SCHEMA_ELEMENT_GET_QNAME( element, env);
    return process( writer, env, qname, meta_info, typemap, AXIS2_TRUE);
}

axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_complex_type( w2c_schema_writer_t *writer,
         const axis2_env_t *env,
         xml_schema_complex_type_t *type,
         axis2_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info)
{
    axis2_qname_t *qname;

    AXIS2_ENV_CHECK(env, NULL);

    qname = XML_SCHEMA_ELEMENT_GET_QNAME( element, env);
    return process( writer, env, qname, meta_info, typemap, AXIS2_FALSE);
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_write_batch( w2c_schema_writer_t *writer,
                 const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    axis2_char_t *temp_package = NULL;
    FILE *out = NULL;

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
        out = w2c_schema_writer_create_out_file( writer_impl, env, 
                      temp_package, W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".c");
        /* parse with the template and create the files */
        w2c_schema_writer_parse( writer_impl, env,
                      writer_impl-> global_wrapped_source_node, out);
        out = w2c_schema_writer_create_out_file( writer_impl, env, 
                      temp_package, W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".h");
        /* parse with the template and create the files */
        w2c_schema_writer_parse( writer_impl, env,
                      writer_impl-> global_wrapped_header_node, out);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t*
w2c_schema_writer_write_simple_type( w2c_schema_writer_impl_t *writer_impl,
         const axis2_env_t *env,
         xml_schema_complex_type_t *type,
         axis2_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info)
{
    AXIS2_ENV_CHECK(env, NULL);
    w2c_messages_print_n_log_error( env, "schema.notImplemented");
    return AXIS2_FAILURE;
}

axis2_status_t
w2c_schema_writer_init_with_file( w2c_schema_writer_impl_t *writer_impl,
         const axis2_env_t *env,
         axis2_char_t *root_dir)
{
    AXIS2_ENV_CHECK(env, NULL);
    if( root_dir == NULL )
    {
        root_dir = AXIS2_STRDUP(".", env);
    }

    writer_impl-> root_dir = root_dir;
    writer_impl-> names_list = axis2_array_list_create(env);
    writer_impl-> template_name = 
        W2C_SCHEMA_PROPERTY_LOADER_GET_TEMPLATE_NAME( writer_impl-> loader, env);

    return AXIS2_SUCCESS;
}

/**TODO: this should be moved to laguage specific writer */
axis2_char_t*
w2c_schema_writer_make_fully_qualified_class_name( w2c_schema_writer_impl_t *writer_impl,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_char_t *ns_uri = NULL;
    axis2_char_t *base_package_name = NULL;
    axis2_char_t *package_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_char_t *original_name = NULL;
    axis2_char_t *package_prefix = NULL;
    axis2_char_t *fully_qualified_class_name = NULL;
    int length = 0;

    ns_uri = AXIS2_QNAME_GET_URI( qname, env);
    package_name = w2c_schema_writer_get_package(writer, env, ns_uri );
    class_name = w2c_schema_writer_make_unique_class_name( writer, env,
            writer_impl-> names_list, original_name);
    if( writer_impl-> wrapped_class)
    {
        package_prefix = 
            (writer_impl-> package_name == NULL)? 
                        (W2C_SCHEMA_WRITER_DEFAULT_PACKAGE "."):writer_impl-> package_name;
        package_prefix = w2c_string_add_string( package_prefix, 
                        W2C_SCHEMA_WRAPPED_DATABINDING_CLASS_NAME, env);
    }
    else if ( writer_impl-> write_classes)
    {
        package_prefix = package_name;
    }
    if( package_prefix != NULL)
    {
        length = AXIS2_STRLEN( fully_qualifed_class_name);
        fully_qualifed_class_name = axis2_stracat(package_prefix,
                (package_prefix[length-1]=='.')?"":".");
        fully_qualifed_class_name = axis2_add_string(fully_qualified_class_name,
                class_name );
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

    base_package_name = (axis2_char_t*)axis2_hash_get( writer_impl-> ns2packagename_map, qname_str,
             AXIS2_HASH_KEY_STRING);
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
    axis2_array_list_t *property_names = NULL;
    axiom_node_t *model_source_node = NULL;
    axiom_node_t *model_header_node = NULL;
    FILE *out = NULL;
    axis2_char_t *model_name = NULL;

    fully_qualified_class_name = W2C_SCHEMA_WRITER_META_INFO_GET_OWN_CLASSNAME( meta_info, env);
    if( NULL == fully_qualified_class_name )
    {
        w2c_schema_writer_make_fully_qualified_class_name( writer_impl, env, qname);
    }
    length = AXIS2_STRLEN( fully_qualified_class_name);
    for ( ; lenght>=0 && fully_qualified_class_name[length]!= '.'; length -- )
    class_name = fully_quailified_class_name+length;

    original_name = AXIS2_QNAME_TO_STRING( qname, env);
    property_names = axis2_array_list_create( env, 10);

    if( writer_impl-> templated_loaded)
    {
        w2c_schema_writer_load_template( writer_impl, env);
    }
    /* for wrapped class, if it already created childs will be added */
    if( wrapped_classes)
    {
        w2c_schema_writer_make_element( writer_impl, env, writer_impl-> global_wrapped_source_document,
           class_name, original_name, qname, is_element, meta_info, property_names, typemap); 
        w2c_schema_writer_make_element( writer_impl, env, writer_impl-> global_wrapped_header_document,
           class_name, original_name, qname, is_element, meta_info, property_names, typemap);
    }
    else
    {
        w2c_schema_writer_make_element( writer_impl, env, model_source_node,
           class_name, original_name, qname, is_element, meta_info, property_names, typemap); 
        w2c_schema_writer_make_element( writer_impl, env, model_header_node,
           class_name, original_name, qname, is_element, meta_info, property_names, typemap);
        if(write_classes)
        {
            out = w2c_schema_writer_create_out_file( writer_impl, env, 
                          "", W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".c");
            /* parse with the template and create the files */
            w2c_schema_writer_parse( writer_impl, env,
                          model_source_node, out);
            out = w2c_schema_writer_create_out_file( writer_impl, env, 
                          "", W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".h");
            /* parse with the template and create the files */
            w2c_schema_writer_parse( writer_impl, env,
                          model_header_node, out);
        }
        model_name = axis2_stracat( class_name, "|", env);
        model_name = axis2_add_string( model_name, namespace_uri, env);
        axis2_hash_set( writer_impl-> model_map,  model_source, AXIS2_HASH_KEY_STRING, model_source_node);
    }
    return fully_qualified_class_name;
}

axiom_node_t 
w2c_schema_writer_process( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axiom_node_t *model,
                        axis2_char_t *class_name,
                        axis2_char_t *original_name,
                        axis2_qname_t *qname,
                        axis2_bool_t is_element,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axis2_array_list_t *property_names,
                        axis2_hash_t *typemap)
{
    axis2_char_t *ns_uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *caps_name = NULL;

    if ( model == NULL)
    {
       model =  w2c_xslt_utils_add_child_node( env, "bean", NULL);
    }
    ns_uri = AXIS2_QNAME_GET_URI( qname, env);
    prefix = AXIS2_QNAME_GET_PREFIX( qname, env);

    prefix = w2c_schema_writer_get_prefix4uri( ns_uri, prefix);

    w2c_xslt_utils_add_attribute( env, model, "name", class_name);
    w2c_xslt_utils_add_attribute( env, model, "originalName", original_name);
    w2c_xslt_utils_add_attribute( env, model, "nsuri", ns_uri);
    w2c_xslt_utils_add_attribute( env, model, "nsprefix", ns_uri);
    
    caps_name = axis2_string_toupper( class_name);
    w2c_xslt_utils_add_attribute( env, model, "caps-name", caps_name);

    if(!writer_impl-> wrap_classes)
        w2c_xslt_utils_add_attribute( env, model, "unwrapped", "yes");
    if(!writer_impl-> skip_write)
        w2c_xslt_utils_add_attribute( env, model, "skip-write", "yes");
    if(!writer_impl-> is_element)
        w2c_xslt_utils_add_attribute( env, model, "type", "yes");

    if(W2C_SCHEMA_WRITER_META_INFO_GET_ANONYMOUS( meta_info, env))
        w2c_xslt_utils_add_attribute( env, model, "anon", "yes");
    if(W2C_SCHEMA_WRITER_META_INFO_IS_EXTENSION( meta_info, env))
        w2c_xslt_utils_add_attribute( env, model, "extension",
                W2C_SCHEMA_WRITER_META_INFO_GET_EXTENSION_CLASS_NAME( meta_info, env));

    if(W2C_SCHEMA_WRITER_META_INFO_IS_CHOICE( meta_info, env))
        w2c_xslt_utils_add_attribute( env, model, "choise", "yes");
    if(W2C_SCHEMA_WRITER_META_INFO_IS_ORDERED( meta_info, env))
        w2c_xslt_utils_add_attribute( env, model, "ordered", "yes");
    
    if(writer_impl-> is_element && 
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
                        axis2_array_list_t *property_names,
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
                        axis2_array_list_t *property_names,
                        axis2_hash_t *typemap,
                        axis2_bool_t is_inherited)
{
    axis2_array_list_t *qnames = NULL;
    int i = 0;
    int size = 0;
    axis2_qname_t *qname = NULL;
    axis2_node_t *property = NULL;
    axis2_char_t *localpart = NULL;
    axis2_char_t *unique_name = NULL;
    axis2_char_t *nsuri = NULL;
    axis2_char_t *classname= NULL;
    axis2_char_t *caps_name = NULL;
    axis2_char_t *caps_type = NULL;
    axis2_bool_t is_primitive = AXIS2_FALSE;
    axis2_bool_t is_default = AXIS2_FALSE;
    axis2_char_t *short_type_name = NULL;

    if ( W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED( meta_info, env) )
    {
        qnames = W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED_QNAMES_ARRAY( meta_info, env);
    }
    else
    {
        qnames = W2C_SCHEMA_WRITER_META_INFO_GET_QNAMES_ARRAY( meta_info, env);
    }
    size = AXIS2_ARRAY_LIST_SIZE( qnames, env);
    for( i = 0; i < size; i ++)
    {
        qname = AXIS2_ARRAY_LIST_GET( qnames, env, i);
        property = w2c_xslt_utils_add_child_node( env, "property", model);
        localpart = AXIS2_QNAME_GET_LOCALPART( qname, env);
        nsuri = AXIS2_QNAME_GET_URI( qname, env);
        unique_name = w2c_schema_writer_make_unique_struct_name( localpart, env);

        w2c_xslt_utils_add_attribute( env, property, "name", localpart);
        w2c_xslt_utils_add_attribute( env, property, "cname", unique_name);
        w2c_xslt_utils_add_attribute( env, property, "nsuri", nsuri);
       
        classname =  
          W2C_SCHEMA_WRITER_META_INFO_GET_CLASSNAME4QNAME( meta_info, env, qname);
        if( NULL == classname )
        {
            return AXIS2_FAILURE;
        }
        caps_name = AXIS2_STRDUP( name, env);
        caps_type = AXIS2_STRDUP( classname, env);

        caps_name = axis2_string_toupper( caps_name);
        caps_type = axis2_string_toupper( caps_type);
        w2c_xslt_utils_add_attribute( env, property, "caps-name", caps_name);
        w2c_xslt_utils_add_attribute( env, property, "caps-type", caps_type);

        is_primitive = w2c_schema_writer_is_primitive( classname, env);
        if ( is_primitive)
        {
            w2c_xslt_utils_add_attribute( env, property, "primitive", "yes");
        }
        /* add an attribute that says the type is default */
        is_default = w2c_schema_writer_is_default( classname, env);
        if ( is_default)
        {
            w2c_xslt_utils_add_attribute( env, property, "default", "yes");
        }
        schema_qname = W2C_SCHEMA_WRITER_META_INFO_GET_SCHEMA_QNAME4QNAME( meta_info, env, qname);
        qname_str = AXIS2_QNAME_TO_STRING( schema_qname, env);
        schema_type = axis2_hash_get( writer_impl-> typemap, qname_str, AXIS2_HASH_KEY_STRING);

        if ( schema_type != NULL)
        {
            w2c_xslt_utils_add_attribute( env, property, "ours", "yes");
        }
        if ( W2C_SCHEMA_WRITER_META_INFO_GET_ATTRI_STATUS4QNAME( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "attribute", "yes");
        }
        if( W2C_SCHEMA_WRITER_META_INFO_IS_NILLABLE( meta_info, env, name,))
        {
            w2c_xslt_utils_add_attribute( env, property, "nillable", "yes");
        }
        schema_type = axis2_hash_get( writer_impl-> base_typemap, qname_str, AXIS2_HASH_KEY_STRING);
        if ( schema_qname != NULL && schema_type != NULL)
        {
            short_type_name = AXIS2_QNAME_GET_LOCALPART( schema_qname, env);
        }
        else
        {
            short_type_qname = w2c_schema_writer_get_short_typename( schema_qname, env);
        }

        if( is_inherited)
        {
            w2c_xslt_utils_add_attribute( env, property, "inherited", "yes");
        }

        if( W2C_SCHEMA_WRITER_META_INFO_IS_GET_ANYSTATUS4QNAME( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "any", "yes");
        }
        if( W2C_SCHEMA_WRITER_META_INFO_IS_GET_BINARYSTATUS4QNAME( meta_info, env, qname))
        {
            w2c_xslt_utils_add_attribute( env, property, "binary", "yes");
        }

        minoccurs = W2C_SCHEMA_WRITER_META_INFO_IS_GET_MINOCCURS( meta_info, env, qname);
        sprintf(number_str, "%d", minoccurs);
        w2c_xslt_utils_add_attribute( env, property, "minOccurs", number_str);

        if ( W2C_SCHEMA_WRITER_META_INFO_IS_GET_ARRAY_STATUS4QNAME( meta_info, env, qname) )
        {
            w2c_xslt_utils_add_attribute( env, property, "isarray", "yes");
            w2c_xslt_utils_add_attribute( env, property, "arrayBaseType", "classname");
        }
        max_occurs = W2C_SCHEMA_WRITER_META_INFO_GET_MAX_OCCURS( meta_info, env, qname);
        if ( max_occurs == LONG_MIN )
        {
            w2c_xslt_utils_add_attribute( env, property, "unbound", "yes");
        }
        sprintf(number_str, "%d", maxoccurs);
        w2c_xslt_utils_add_attribute( env, property, "maxOccurs", number_str);
    }
    return AXIS2_SUCCESS;
} 
