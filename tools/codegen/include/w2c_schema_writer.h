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
  
#ifndef W2C_SCHEMA_WRITER_H
#define W2C_SCHEMA_WRITER_H

/**
 * @file w2c_schema_writer.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axutil_hash.h>
#include <w2c_schema_compiler_options.h>
#include <xml_schema_element.h>
#include <w2c_schema_writer_meta_info.h>
#include <xml_schema_complex_type.h>
#include <xml_schema_simple_type.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define W2C_SCHEMA_WRITER_XSLT_PATH "/conf/templates/"

/**
 * @defgroup w2c_schema_writer represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_schema_writer w2c_schema_writer_t;
typedef struct w2c_schema_writer_ops w2c_schema_writer_ops_t;

AXIS2_DECLARE_DATA  struct w2c_schema_writer_ops
{
   /**
    * free w2c_schema_writer.
    * @param  schema_writer pointer to schema_writer struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_schema_writer_t *schema_writer,
          const axutil_env_t *env);

    axutil_hash_t* (AXIS2_CALL *
    get_model_map)( w2c_schema_writer_t *writer,
        const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    init)( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         w2c_schema_compiler_options_t *options,
         axis2_char_t *template_name,
         axutil_hash_t *base_typemap);

    axis2_char_t* (AXIS2_CALL *
    write_element)( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         xml_schema_element_t *element,
         axutil_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info);

    axis2_char_t* (AXIS2_CALL *
    write_complex_type)( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         xml_schema_complex_type_t *type,
         axutil_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info);

    axis2_status_t (AXIS2_CALL *
    write_batch)( w2c_schema_writer_t *writer,
                 const axutil_env_t *env);

    axis2_char_t* (AXIS2_CALL *
    write_simple_type)( w2c_schema_writer_t *writer,
         const axutil_env_t *env,
         xml_schema_complex_type_t *type,
         axutil_hash_t *typemap,
         w2c_schema_writer_meta_info_t *meta_info);

    axis2_char_t* (AXIS2_CALL *
    make_fully_qualified_class_name)( w2c_schema_writer_t *writer,
        const axutil_env_t *env,
        axis2_qname_t *qname);


    axis2_status_t (AXIS2_CALL *
    register_extension_mapper_packagename)(w2c_schema_writer_t *writer,
                                        const axutil_env_t *env,
                                        axis2_char_t *package_name);

    axis2_status_t (AXIS2_CALL *
    write_extension_mapper)( w2c_schema_writer_t *writer,
                                const axutil_env_t *env,
                                axutil_hash_t *meta_info_array);


    axis2_char_t* (AXIS2_CALL *
    get_extension_mapper_package_name)(w2c_schema_writer_t *writer,
                                        const axutil_env_t *env);
 
     
   };

AXIS2_DECLARE_DATA  struct w2c_schema_writer
{
     struct w2c_schema_writer_ops *ops;
};

/** this is an interface for schema_writer */

/*************************** Function macros **********************************/

#define W2C_SCHEMA_WRITER_FREE(schema_writer, env) \
      ((schema_writer)->ops->free (schema_writer, env)) 

#define W2C_SCHEMA_WRITER_GET_MODEL_MAP(schema_writer, env) \
      ((schema_writer)->ops->get_model_map(schema_writer, env))

#define W2C_SCHEMA_WRITER_INIT(schema_writer, env, options, template_name, base_typemap) \
      ((schema_writer)->ops->init(schema_writer, env, options, template_name, base_typemap))

#define W2C_SCHEMA_WRITER_WRITE_ELEMENT(schema_writer, env, element, typemap, meta_info) \
      ((schema_writer)->ops->write_element(schema_writer, env, element, typemap, meta_info))

#define W2C_SCHEMA_WRITER_WRITE_COMPLEX_TYPE(schema_writer, env, type, typemap, meta_info) \
      ((schema_writer)->ops->write_complex_type(schema_writer, env, type, typemap, meta_info))

#define W2C_SCHEMA_WRITER_WRITE_BATCH(schema_writer, env) \
      ((schema_writer)->ops->write_batch(schema_writer, env)) 

#define W2C_SCHEMA_WRITER_WRITE_SIMPLE_TYPE(schema_writer, env, type, typemap, meta_info) \
      ((schema_writer)->ops->write_simple_type(schema_writer, env, type, typemap, meta_info))

#define W2C_SCHEMA_WRITER_MAKE_FULLY_QUALIFIED_CLASS_NAME(schema_writer, env, qname) \
      ((schema_writer)->ops->make_fully_qualified_class_name(schema_writer, env, qname))

#define W2C_SCHEMA_WRITER_REGISTER_EXTENSION_MAPPER_PACKAGENAME(schema_writer, env, package) \
      ((schema_writer)->ops->register_extension_mapper_packagename(schema_writer, env, package)) 

#define W2C_SCHEMA_WRITER_WRITE_EXTENSION_MAPPER(schema_writer, env, array) \
      ((schema_writer)->ops->write_extension_mapper(schema_writer, env, array)) 

#define W2C_SCHEMA_WRITER_GET_EXTENSION_MAPPER_PACKAGE_NAME(schema_writer, env) \
      ((schema_writer)->ops->get_extension_mapper_package_name(schema_writer, env)) 

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_SCHEMA_WRITER_H */
