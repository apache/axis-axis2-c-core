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
  
#ifndef W2C_SCHEMA_PROPERTY_LOADER_H
#define W2C_SCHEMA_PROPERTY_LOADER_H

/**
 * @file w2c_schema_property_loader.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <w2c_schema_writer.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_schema_property_loader represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */
typedef struct w2c_schema_property_loader w2c_schema_property_loader_t;
typedef struct w2c_schema_property_loader_ops w2c_schema_property_loader_ops_t;

AXIS2_DECLARE_DATA  struct w2c_schema_property_loader_ops
{
   /**
    * free w2c_schema_property_loader.
    * @param  schema_property_loader pointer to schema_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_schema_property_loader_t *schema_property_loader,
         const axutil_env_t *env);

   /**
    * change the filename of the loader
    * just sets the filame should call reload to reload the properties.
    * @param  schema_property_loader pointer to schema_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @param  filename filename of the property file. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_filename)(w2c_schema_property_loader_t *schema_property_loader,
         const axutil_env_t *env, axis2_char_t *filename);

   /**
    * change the language of the loader
    * just sets the language to call reload the properties.
    * @param  schema_property_loader pointer to schema_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @param  lang language to be loaded
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_language)(w2c_schema_property_loader_t *schema_property_loader,
         const axutil_env_t *env, axis2_char_t *lang);


    /**
    * reload the properties.
    * @param  schema_property_loader pointer to schema_property_loader struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    reload )(w2c_schema_property_loader_t *schema_property_loader,
         const axutil_env_t *env);
    
    /**
     * retrieve schema template name.
     * @param  env Environment. MUST NOT be NULL
     * @param lang language to get the template name
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_template_name)(
        w2c_schema_property_loader_t *schema_property_loader,
        const axutil_env_t *env);
    
    /**
     * retrieve schema writer instance.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    w2c_schema_writer_t* (AXIS2_CALL*
    get_writer_instance)(
        w2c_schema_property_loader_t *schema_property_loader,
        const axutil_env_t *env);

    /**
     * retrieve typemapper name.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns typemapper_name.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_typemapper_name)(
        w2c_schema_property_loader_t *schema_property_loader,
        const axutil_env_t *env);

    /**
     * retrieve default class.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_default_class)(
        w2c_schema_property_loader_t *schema_property_loader,
        const axutil_env_t *env);

    /**
     * retrieve default array.
     * @param  env Environment. MUST NOT be NULL
     * @return Returns axis2_char_t*.
     */
    axis2_char_t* (AXIS2_CALL* 
    get_default_array)(
        w2c_schema_property_loader_t *schema_property_loader,
        const axutil_env_t *env);


};

AXIS2_DECLARE_DATA  struct w2c_schema_property_loader
{
     struct w2c_schema_property_loader_ops *ops;
};

/**
 * create a schema_property_loader obj
 * @param env Environment. MUST NOT be NULL
 * @param  lang language to be loaded
 * @return newly created schema_property_loader obj
 */
AXIS2_EXTERN w2c_schema_property_loader_t* AXIS2_CALL
w2c_schema_property_loader_create( const axutil_env_t *env, axis2_char_t *lang);

/*************************** Function macros **********************************/

#define W2C_SCHEMA_PROPERTY_LOADER_FREE(schema_property_loader, env) \
      ((schema_property_loader)->ops->free (schema_property_loader, env)) 

#define W2C_SCHEMA_PROPERTY_LOADER_SET_FILENAME(schema_property_loader, env, filename) \
      ((schema_property_loader)->ops->set_filename (schema_property_loader, env, filename)) 

#define W2C_SCHEMA_PROPERTY_LOADER_RELOAD(schema_property_loader, env) \
      ((schema_property_loader)->ops->reload (schema_property_loader, env)) 

#define W2C_SCHEMA_PROPERTY_LOADER_GET_WRITER_INSTANCE(schema_property_loader, env) \
      ((schema_property_loader)->ops->get_writer_instance(schema_property_loader, env)) 

#define W2C_SCHEMA_PROPERTY_LOADER_GET_TEMPLATE_NAME(schema_property_loader, env) \
      ((schema_property_loader)->ops->get_template_name(schema_property_loader, env))

#define W2C_SCHEMA_PROPERTY_LOADER_GET_TYPEMAPPER_NAME(schema_property_loader, env) \
      ((schema_property_loader)->ops->get_typemapper_name(schema_property_loader, env))

#define W2C_SCHEMA_PROPERTY_LOADER_GET_DEFAULT_ARRAY(schema_property_loader, env) \
      ((schema_property_loader)->ops->get_default_array(schema_property_loader, env))

#define W2C_SCHEMA_PROPERTY_LOADER_GET_DEFAULT_CLASS(schema_property_loader, env) \
      ((schema_property_loader)->ops->get_default_class(schema_property_loader, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_SCHEMA_PROPERTY_LOADER_H */
