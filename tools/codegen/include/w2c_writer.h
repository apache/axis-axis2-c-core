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
  
#ifndef W2C_WRITER_H
#define W2C_WRITER_H

/**
 * @file w2c_writer.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axiom.h>
#include <w2c_config_property_loader.h>
#include <w2c_engine_configuration.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define W2C_WRITER_XSLT_PATH "/conf/templates/"

/**
 * @defgroup w2c_writer represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_writer w2c_writer_t;
typedef struct w2c_writer_ops w2c_writer_ops_t;

AXIS2_DECLARE_DATA  struct w2c_writer_ops
{
   /**
    * free w2c_writer.
    * @param  writer pointer to writer struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_writer_t *writer,
          const axis2_env_t *env);
  
   /**
    * create the file to output.
    * @param  writer pointer to writer struct
    * @param  env Environment. MUST NOT be NULL
    * @param package_name package name to derive the directory name
    * @param file_name name of the ouptut file
    * @return full name of the created file
    */  
    axis2_char_t* (AXIS2_CALL *
    create_out_file)(w2c_writer_t *writer,
          const axis2_env_t *env,
          axis2_char_t *package_name,
          axis2_char_t *file_name);
   
   /**
    * write the output from parsing a given name with xslt.
    * @param  writer pointer to writer struct
    * @param  env Environment. MUST NOT be NULL
    * @param  root node which used to parse the xslt
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */  
    axis2_status_t (AXIS2_CALL *
    parse)(w2c_writer_t *writer,
          const axis2_env_t *env,
          axiom_node_t *root);
 
   /**
    * initialize writer with configurations.
    * @param  writer pointer to writer struct
    * @param  env Environment. MUST NOT be NULL
    * @param  loader loaded configuration properties
    * @param  config confgurations based on user options
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    initialize)(w2c_writer_t *writer,
          const axis2_env_t *env,
          w2c_config_property_loader_t *loader,
          w2c_engine_configuration_t *config);

};

AXIS2_DECLARE_DATA  struct w2c_writer
{
     struct w2c_writer_ops *ops;
};

/** this is an interface for writer */

/*************************** Function macros **********************************/

#define W2C_WRITER_FREE(writer, env) \
      ((writer)->ops->free (writer, env)) 

#define W2C_WRITER_CREATE_OUT_FILE(writer, env, package, file) \
      ((writer)->ops->create_out_file(writer, env, package, file ))

#define W2C_WRITER_PARSE(writer, env, root) \
      ((writer)->ops->parse(writer, env, root ))

#define W2C_WRITER_INITIALIZE(writer, env, loader, config) \
      ((writer)->ops->initialize(writer, env, loader, config))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_WRITER_H */
