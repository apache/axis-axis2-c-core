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

#include "schema_writer_protected.h"
#include <w2c_xslt_template_processor.h>

/**
 * @brief
 */
#define W2C_SCHEMA_C_WRITER_PREFIX "axis2_"
#define W2C_SCHEMA_C_WRITER_SUFFIX ""

#define W2C_SCHEMA_C_DEFAULT_CLASS "axiom_node_t*"

/******************* C language specific functions ****************************/

axis2_char_t* AXIS2_CALL
w2c_schema_c_writer_make_fully_qualified_class_name( w2c_schema_writer_t *writer,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_char_t *fully_qualified_class_name = NULL;
    w2c_schema_writer_impl_t *writer_impl = NULL;
    W2C_ENGINE_CONFIGURATION_NAMEMAKER name_maker = NULL;
    w2c_qname2name_maker_t *qname2name_maker = NULL;
    axis2_qname_t *temp_qname = NULL;
    axis2_char_t *temp_name = NULL;
    axis2_char_t *temp_uri = NULL;
    axis2_char_t *temp_prefix = NULL;

	AXIS2_ENV_CHECK(env, NULL);

    writer_impl = W2C_INTF_TO_IMPL(writer);
   
    /** verify unique names */ 
    temp_name = AXIS2_QNAME_GET_LOCALPART( qname, env);
    temp_uri = AXIS2_QNAME_GET_URI( qname, env);
    temp_prefix = AXIS2_QNAME_GET_PREFIX( qname, env);
    temp_name = w2c_schema_writer_make_unique_class_name( writer_impl, env,
                                 writer_impl-> names_list, temp_name);
    temp_qname = axis2_qname_create( env, temp_name, temp_uri, temp_prefix);

    qname2name_maker =
        W2C_SCHEMA_COMPILER_OPTIONS_GET_QNAME2NAME_MAKER( writer_impl-> options, env);
    if ( qname2name_maker == NULL)
    {
        fully_qualified_class_name = AXIS2_QNAME_GET_LOCALPART( temp_qname, env);
    }
    else
    {
        fully_qualified_class_name = 
            W2C_QNAME2NAME_MAKER_SUGGEST_NAME( qname2name_maker, env, temp_qname);
    }

    fully_qualified_class_name = AXIS2_STRDUP( fully_qualified_class_name, env);
    
    name_maker = W2C_SCHEMA_COMPILER_OPTIONS_GET_NAME_MAKER_FUNC
                                            ( writer_impl-> options, env);
    fully_qualified_class_name = name_maker( fully_qualified_class_name, env);
    return fully_qualified_class_name;
}

axis2_status_t AXIS2_CALL
w2c_schema_c_writer_write_batch( w2c_schema_writer_t *writer,
                 const axis2_env_t *env)
{
    w2c_schema_writer_impl_t *writer_impl = NULL;
    axis2_char_t *out = NULL;
    axis2_char_t *source_template = NULL;
    axis2_char_t *header_template = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    writer_impl = W2C_INTF_TO_IMPL(writer);
    if( writer_impl-> wrap_classes)
    {
        out = W2C_SCHEMA_WRITER_CREATE_OUT_FILE( writer_impl, env,
                      NULL, W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".c");
        /* parse with the template and create the files */
        source_template = axis2_stracat( writer_impl-> template_name, "Source.xsl", env);
        w2c_schema_writer_parse( writer_impl, env,
                      writer_impl-> global_wrapped_node, out, source_template);
        out = W2C_SCHEMA_WRITER_CREATE_OUT_FILE( writer_impl, env,
                      NULL, W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME, ".h");
        /* parse with the template and create the files */
        header_template = axis2_stracat( writer_impl-> template_name, "Header.xsl", env);
        w2c_schema_writer_parse( writer_impl, env,
                      writer_impl-> global_wrapped_node, out, header_template);
        AXIS2_FREE( env-> allocator, source_template);
        AXIS2_FREE( env-> allocator, header_template);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t*
w2c_schema_c_writer_process( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_qname_t *qname,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axis2_hash_t *typemap,
                        axis2_bool_t is_element)
{
    axis2_char_t *original_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_hash_t *property_names = NULL;
    axiom_node_t *model_source_node = NULL;
    axis2_char_t *out = NULL;
    axis2_char_t *source_template = NULL;
    axis2_char_t *header_template = NULL;
    axis2_char_t *model_name = NULL;
    axis2_char_t *namespace_uri = NULL;

    original_name = AXIS2_QNAME_GET_LOCALPART( qname, env);
    original_name = AXIS2_STRDUP( original_name, env);
    class_name = W2C_SCHEMA_WRITER_META_INFO_GET_OWN_CLASSNAME( meta_info, env);
    if( NULL == class_name )
    {
        class_name =
            W2C_SCHEMA_WRITER_MAKE_FULLY_QUALIFIED_CLASS_NAME( &(writer_impl->writer), env, qname);
    }

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
                          "", class_name, ".c");
            /* parse with the template and create the files */
            source_template = axis2_stracat( writer_impl-> template_name, "Source.xsl", env);
            w2c_schema_writer_parse( writer_impl, env,
                          model_source_node, out, source_template);
            out = W2C_SCHEMA_WRITER_CREATE_OUT_FILE( writer_impl, env, 
                          "", class_name, ".h"); 
            /* parse with the template and create the files */
            header_template = axis2_stracat( writer_impl-> template_name, "Header.xsl", env);
            w2c_schema_writer_parse( writer_impl, env,
                          model_source_node, out, header_template);
        }
        namespace_uri = AXIS2_QNAME_GET_URI( qname, env);
        model_name = axis2_stracat( class_name, "|", env);
        model_name = w2c_string_add_string( model_name, namespace_uri, env);
        axis2_hash_set( writer_impl-> model_map,  model_name, AXIS2_HASH_KEY_STRING, model_source_node);
    }
    AXIS2_FREE( env->allocator, original_name);
    return class_name;
}

axis2_char_t*
w2c_schema_c_writer_create_out_file( w2c_schema_writer_impl_t *writer_impl,
                    const axis2_env_t *env,
                    axis2_char_t *package_name, /* here it is NULL */
                    axis2_char_t *name,
                    axis2_char_t *extension)
{
    axis2_char_t *filename = NULL;
    axis2_char_t *fullpath = NULL;

    filename = axis2_stracat( W2C_SCHEMA_C_WRITER_PREFIX, name, env);
    fullpath = w2c_file_writer_create_classfile_name( env, writer_impl->root_dir,
                package_name, filename, extension);
    AXIS2_FREE( env-> allocator, filename);
    return fullpath;
}

axis2_bool_t 
w2c_schema_c_writer_is_default_class( w2c_schema_writer_impl_t *writer_impl,
                        const axis2_env_t *env,
                        axis2_char_t *c_class_name4element)
{
    return !(AXIS2_STRCMP( c_class_name4element, W2C_SCHEMA_C_DEFAULT_CLASS));
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_schema_writer_t **inst,
                   const axis2_env_t *env)
{
    w2c_schema_writer_t *writer = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);

    writer = w2c_schema_writer_create(env);
    writer->ops->make_fully_qualified_class_name = 
                         w2c_schema_c_writer_make_fully_qualified_class_name;
    writer->ops->write_batch= 
                         w2c_schema_c_writer_write_batch;
    W2C_INTF_TO_IMPL(writer)->writer_impl_ops->process = w2c_schema_c_writer_process;
    W2C_INTF_TO_IMPL(writer)->writer_impl_ops->create_out_file =
                                         w2c_schema_c_writer_create_out_file;
    W2C_INTF_TO_IMPL(writer)->writer_impl_ops->is_default_class =
                                         w2c_schema_c_writer_is_default_class;
    if(!writer)
    {
        return AXIS2_FAILURE;
    }
    *inst =  writer;
    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(w2c_schema_writer_t *inst,
                      const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    if (inst)
    {
        status = W2C_SCHEMA_WRITER_FREE(inst, env);
    }
    return status;
}

