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

#include <w2c_schema_writer.h>
#include <w2c_schema_writer_meta_info.h>
#include <w2c_schema_compiler_options.h>
#include <w2c_messages.h>
#include <axiom_node.h>
#include <w2c_xslt_utils.h>
#include <w2c_string.h>
#include <w2c_url_processor.h>
#include <w2c_typemapper.h>
#include <limits.h>
#include <stdlib.h>
#include <w2c_file_writer.h>

/**
 * @brief
 */
#define W2C_SCHEMA_WRITER_WRAPPED_DATABINDING_CLASS_NAME  "WrappedDatabinder"
#define W2C_SCHEMA_WRITER_DEFAULT_PACKAGE "adb"
#define W2C_SCHEMA_WRITER_EXTENSION_MAPPER_CLASSNAME "ExtensionMapper"

typedef struct w2c_schema_writer_impl w2c_schema_writer_impl_t;
typedef struct w2c_schema_writer_impl_ops w2c_schema_writer_impl_ops_t;

struct w2c_schema_writer_impl
{
    w2c_schema_writer_t writer;
    w2c_schema_writer_impl_ops_t *writer_impl_ops;

    axis2_char_t *template_name;
    axis2_bool_t template_loaded;
    axutil_hash_t *names_list;
    axis2_bool_t wrap_classes;
    axis2_bool_t write_classes;
    axis2_char_t *package_name;
    axis2_char_t *root_dir;
    axiom_node_t *global_wrapped_node;
    axutil_hash_t *model_map;
    
    axutil_hash_t *base_type_map;
    axutil_hash_t *ns2packagename_map;
    axis2_bool_t is_helper_mode;
    /**
     * package for the mapping class
     */
    axis2_char_t *mapping_class_package;

    axutil_hash_t *uri2prefix_map;
    axutil_hash_t *prefix2uri_map;
    int last_prefix_index;
    w2c_schema_compiler_options_t *options;

    /** to hold language specific data */
    void *language_specific_extension;

};

/** langauge dependent operations will be added here */
struct w2c_schema_writer_impl_ops
{
    axis2_char_t*
    (*process)( w2c_schema_writer_impl_t *writer_impl,
                                    const axutil_env_t *env,
                                    axis2_qname_t *qname,
                                    w2c_schema_writer_meta_info_t *meta_info,
                                    axutil_hash_t *typemap,
                                    axis2_bool_t is_element);
    axis2_char_t*
    (*create_out_file)( w2c_schema_writer_impl_t *writer_impl,
                            const axutil_env_t *env,
                            axis2_char_t *package_name,
                            axis2_char_t *name,
                            axis2_char_t *extension);
    
    axis2_bool_t
    (*is_default_class)( w2c_schema_writer_impl_t *writer_impl,
                            const axutil_env_t *env,
                            axis2_char_t *name );
                            
};

/** macros to call language dependent operations */
#define W2C_SCHEMA_WRITER_PROCESS(writer_impl, env, qname, meta_info, typemap, is_element) \
    ((writer_impl)->writer_impl_ops->process(writer_impl, env, qname, meta_info, typemap, is_element))

#define W2C_SCHEMA_WRITER_CREATE_OUT_FILE(writer_impl, env, package_name, name, extension) \
    ((writer_impl)->writer_impl_ops->create_out_file(writer_impl, env, package_name, name, extension))

#define W2C_SCHEMA_WRITER_IS_DEFAULT_CLASS(writer_impl, env, name) \
    ((writer_impl)->writer_impl_ops->is_default_class(writer_impl, env, name))

/** return implmentation from the interface */
#define W2C_INTF_TO_IMPL(schema_writer) \
        ((w2c_schema_writer_impl_t *) schema_writer)

/* 
 * common set of function set which can be used by any language is 
 * listed below, implementations for these are in schema_writer.c
 */

axis2_status_t AXIS2_CALL
w2c_schema_writer_free (w2c_schema_writer_t *schema_writer,
                                    const axutil_env_t *env);

axutil_hash_t* AXIS2_CALL
w2c_schema_writer_get_model_map( w2c_schema_writer_t *writer,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_init( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         w2c_schema_compiler_options_t *options,
         axis2_char_t *template_name,
         axutil_hash_t *base_type_map);

axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_element( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         xml_schema_element_t *element,
         axutil_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info);

axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_complex_type( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         xml_schema_complex_type_t *type,
         axutil_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info);

axis2_status_t AXIS2_CALL
w2c_schema_writer_write_batch( w2c_schema_writer_t *writer,
                 const axutil_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_schema_writer_write_simple_type( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         xml_schema_complex_type_t *type,
         axutil_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info);

axis2_char_t* AXIS2_CALL
w2c_schema_writer_make_fully_qualified_class_name( w2c_schema_writer_t *writer,
        const axutil_env_t *env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
w2c_schema_writer_register_extension_mapper_packagename(w2c_schema_writer_t *writer,
                                        const axutil_env_t *env,
                                        axis2_char_t *package_name);

axis2_status_t AXIS2_CALL
w2c_schema_writer_write_extension_mapper( w2c_schema_writer_t *writer,
                                const axutil_env_t *env,
                                axutil_hash_t *meta_info_array);


axis2_char_t* AXIS2_CALL
w2c_schema_writer_get_extension_mapper_package_name(w2c_schema_writer_t *writer_,
                                        const axutil_env_t *env);

/** protected methods */

axis2_status_t
w2c_schema_writer_init_with_file( w2c_schema_writer_impl_t *writer_impl,
         const axutil_env_t *env,
         axis2_char_t *root_dir);


axis2_char_t*
w2c_schema_writer_get_package( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        axis2_char_t *namespace_uri);

axis2_char_t*
w2c_schema_writer_process( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        axis2_qname_t *qname,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axutil_hash_t *typemap,
                        axis2_bool_t is_element);

axiom_node_t*
w2c_schema_writer_make_element( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        axis2_char_t *class_name,
                        axis2_char_t *original_name,
                        axis2_qname_t *qname,
                        axis2_bool_t is_element,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axutil_hash_t *property_names,
                        axutil_hash_t *typemap);

axis2_status_t 
w2c_schema_writer_populate_info( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axiom_node_t *model,
                        axutil_hash_t *property_names,
                        axutil_hash_t *typemap,
                        axis2_bool_t is_inherited);

axis2_status_t 
w2c_schema_writer_add_property_entries( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        w2c_schema_writer_meta_info_t *meta_info,
                        axiom_node_t *model,
                        axutil_hash_t *property_names,
                        axutil_hash_t *typemap,
                        axis2_bool_t is_inherited);

axis2_bool_t 
w2c_schema_writer_is_default_class( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        axis2_char_t *java_class_name4element);


axis2_char_t*
w2c_schema_writer_make_unique_class_name( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        axutil_hash_t *list_of_names,
                        axis2_char_t *xml_name);

axis2_status_t
w2c_schema_writer_load_template( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env);

axis2_char_t*
w2c_schema_writer_create_out_file( w2c_schema_writer_impl_t *writer_impl,
                    const axutil_env_t *env,
                    axis2_char_t *package_name,
                    axis2_char_t *name,
                    axis2_char_t *extension);


axis2_status_t
w2c_schema_writer_parse( w2c_schema_writer_impl_t *writer_impl,
                    const axutil_env_t *env,
                    axiom_node_t *root,
                    axis2_char_t *out_file,
                    axis2_char_t *tempalte_name);

axis2_char_t*
w2c_schema_writer_get_prefix4uri( w2c_schema_writer_impl_t *writer_impl,
                        const axutil_env_t *env,
                        axis2_char_t *uri,
                        axis2_char_t *default_prefix);

/** create function prototype */
AXIS2_EXTERN w2c_schema_writer_t * AXIS2_CALL
w2c_schema_writer_create (const axutil_env_t *env);
