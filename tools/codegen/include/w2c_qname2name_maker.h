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
  
#ifndef W2C_QNAME2NAME_MAKER_H
#define W2C_QNAME2NAME_MAKER_H

/**
 * @file w2c_qname2name_maker.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <axutil_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_qname2name_maker represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_qname2name_maker w2c_qname2name_maker_t;
typedef struct w2c_qname2name_maker_ops w2c_qname2name_maker_ops_t;

AXIS2_DECLARE_DATA  struct w2c_qname2name_maker_ops
{
   /**
    * free w2c_qname2name_maker.
    * @param  qname2name_maker pointer to qname2name_maker struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_qname2name_maker_t *qname2name_maker,
         const axutil_env_t *env);

   /**
    * suggest name for a qname.
    * @param  qname2name_maker pointer to qname2name_maker struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname qname to suggest the name.
    * @return suggest name for the qname.
    */
    axis2_char_t* (AXIS2_CALL *
    suggest_name)(w2c_qname2name_maker_t *qname2name_maker,
         const axutil_env_t *env,
         axutil_qname_t *qname);


};

AXIS2_DECLARE_DATA  struct w2c_qname2name_maker
{
     struct w2c_qname2name_maker_ops *ops;
};

/** this is an interface for qname2name_maker*/

/**
 * create a qname2name_maker obj.
 * @param env Environment. MUST NOT be NULL
 * @param parser user options parser
 * @return newly created qname2name_maker obj
 */
AXIS2_EXTERN w2c_qname2name_maker_t* AXIS2_CALL
w2c_qname2name_maker_create( const axutil_env_t *env);

/*************************** Function macros **********************************/

#define W2C_QNAME2NAME_MAKER_FREE(qname2name_maker, env) \
      ((qname2name_maker)->ops->free(qname2name_maker, env)) 

#define W2C_QNAME2NAME_MAKER_SUGGEST_NAME(qname2name_maker, env, qname) \
      ((qname2name_maker)->ops->suggest_name(qname2name_maker, env, qname)) 

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_QNAME2NAME_MAKER_H */
