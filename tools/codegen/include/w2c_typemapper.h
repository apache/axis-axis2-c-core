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
  
#ifndef W2C_TYPEMAPPER_H
#define W2C_TYPEMAPPER_H

/**
 * @file w2c_typemapper.h
 * @brief represent command line option
 */

#include <axutil_utils.h>
#include <axutil_array_list.h>
#include <axutil_hash.h>
#include <axutil_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define W2C_TYPEMAPPER_MAPPING_PATH "/conf/typemappers/"
/**
 * @defgroup w2c_typemapper represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_typemapper w2c_typemapper_t;
typedef struct w2c_typemapper_ops w2c_typemapper_ops_t;

AXIS2_DECLARE_DATA  struct w2c_typemapper_ops
{
   /**
    * free w2c_typemapper.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_typemapper_t *typemapper,
         const axutil_env_t *env);

   /**
    * retrieve the parameter name to the given qname in wsdl
    * parameter would hold the value of the corrosponding type.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname qname in wsdl to find the mapping
    * @return langauge type name
    */   
    axis2_char_t* (AXIS2_CALL *
    get_parameter_name)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env,
          axutil_qname_t *qname);

   /**
    * retrieve the language type name to the given qname in wsdl.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname qname in wsdl to find the mapping
    * @return langauge type name
    */
    axis2_char_t* (AXIS2_CALL *
    get_type_name)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env,
          axutil_qname_t *qname);

   /**
    * add a mapping to the list.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname qname in the wsdl 
    * @param  name langauge specific type
    * @return langauge type name
    */
    axis2_status_t (AXIS2_CALL *
    add_typemapping_name)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env,
          axutil_qname_t *qname,
          axis2_char_t *name);

   /**
    * retrieve the language default type name.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @return the default mapping name ( langauge specific)
    */
    axis2_char_t* (AXIS2_CALL *
    get_default_mapping_name)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env);

   /**
    * retrieve the default qname.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @param the dafault qname ( defined in wsdl)
    */
    axutil_qname_t* (AXIS2_CALL *
    get_default_qname)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env);

   /**
    * retrieve the qname 2 typename hash.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @param the qname 2 typename hash
    */
    axutil_hash_t* (AXIS2_CALL *
    get_all)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env);
   
   /**
    * retrieve whether the type is primitive.
    * @param  typemapper pointer to typemapper struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname qname in wsdl to find the mapping
    * @return whether the type is primitive
    */
    axis2_bool_t (AXIS2_CALL *
    is_primitive)(w2c_typemapper_t *typemapper,
          const axutil_env_t *env,
          axutil_qname_t *qname);

};

AXIS2_DECLARE_DATA  struct w2c_typemapper
{
     struct w2c_typemapper_ops *ops;
};

/**
 * create a typemapper obj
 * @param env Environment. MUST NOT be NULL
 * @param filename filename to load the typemap
 * @return newly created typemapper obj
 */
AXIS2_EXTERN w2c_typemapper_t* AXIS2_CALL
w2c_typemapper_create_from_file( const axutil_env_t *env,
                   axis2_char_t *filename);

/*************************** Function macros **********************************/

#define W2C_TYPEMAPPER_FREE(typemapper, env) \
      ((typemapper)->ops->free (typemapper, env)) 

#define W2C_TYPEMAPPER_GET_MAPPING_NAME(typemapper, env, qname)\
      ((typemapper)->ops->get_mapping_name(typemapper, env, qname))
    
#define W2C_TYPEMAPPER_GET_PARAMETER_NAME(typemapper, env, qname)\
      ((typemapper)->ops->get_parameter_name(typemapper, env, qname))
    
#define W2C_TYPEMAPPER_GET_TYPE_NAME(typemapper, env, qname)\
      ((typemapper)->ops->get_type_name(typemapper, env, qname))

#define W2C_TYPEMAPPER_ADD_TYPEMAPPING_NAME(typemapper, env, qname, name)\
      ((typemapper)->ops->add_typemapping_name(typemapper, env, qname, name))

#define W2C_TYPEMAPPER_GET_DEFAULT_MAPPING_NAME(typemapper, env)\
      ((typemapper)->ops->get_default_mapping_name(typemapper, env))

#define W2C_TYPEMAPPER_GET_DEFAULT_QNAME(typemapper, env)\
      ((typemapper)->ops->get_default_qname(typemapper, env))

#define W2C_TYPEMAPPER_GET_ALL(typemapper, env)\
      ((typemapper)->ops->get_all(typemapper, env))

#define W2C_TYPEMAPPER_IS_PRIMITIVE(typemapper, env, qname)\
      ((typemapper)->ops->is_primitive(typemapper, env, qname))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_TYPEMAPPER_H */
