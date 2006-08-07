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

#ifndef AXIS2_QNAME_H
#define AXIS2_QNAME_H

/**
 * @file axis2_qname.h
 * @brief represents a qualified name
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>

#include <axis2_string.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_qname;
struct axis2_qname_ops;

static const axis2_char_t XML_NAMESPACE_URI[] =
    "http://www.w3c.org/XML/1998/namespace";

/**
 * @defgroup axis2_qname qname 
 * @ingroup axis2_util
 * @{
 */

/**
 * \brief Axis2 qname ops struct
 *
 * Encapsulator struct for ops of axis2_qname
 */
                      
AXIS2_DECLARE_DATA typedef struct axis2_qname_ops
{
    /**
     *  Free a qname struct
     *  @return Status code
     */
    axis2_status_t (AXIS2_CALL *
   free_fn) (struct axis2_qname * qname,
           const axis2_env_t *env);
                                       
     /** 
      * Compare two qnames
      * prefix is ignored when comparing
      * If ns_uri and localpart of qname1 and qname2 is equal returns true
      * @return true if qname1 equals qname2, false otherwise 
      */

    axis2_bool_t (AXIS2_CALL *
   equals) (const struct axis2_qname * qname,
             const axis2_env_t *env,
             const struct axis2_qname * qname1);
      /** 
      * clones a given qname
      * @param qname , qname struct instance to be cloned
      * @env   environment , double pointer to environment
      * @returns the newly cloned qname struct instance
      */                                       
       
    struct axis2_qname* (AXIS2_CALL *
   clone)(struct axis2_qname *qname,
           const axis2_env_t *env);
                                       
    axis2_char_t* (AXIS2_CALL *
   get_uri)(const struct axis2_qname *qname,
             const axis2_env_t *env);
                                        
    axis2_char_t* (AXIS2_CALL *
   get_prefix)(const struct axis2_qname *qname,
                const axis2_env_t *env);

    axis2_char_t* (AXIS2_CALL *
   get_localpart)(const struct axis2_qname *qname,
                   const axis2_env_t *env);
                                              
    /**
     * returns a unique string created by concatanting namespace uri 
     * and localpart .
     * The string is of the form localpart|url
     * The returned char* is freed when qname free function is called.
     */
    axis2_char_t* (AXIS2_CALL *
   to_string)(const struct axis2_qname *qname,
               const axis2_env_t *env); 
               
   
                                              
} axis2_qname_ops_t;

typedef struct axis2_qname
{
    /** ops related to qname */
    axis2_qname_ops_t *ops;
} axis2_qname_t;

/**
 *   creates a qname struct
 *  returns a pointer to a qname struct
 *  @localpart   mandatory
 *  @prefix      mandatory  
 *  @ns_uri      optional
 *  The prefix. Must not be null. Use "" (empty string) to indicate that no 
 * namespace URI is present or the namespace URI is not relevant
 *  if null is passed for prefix and uri , "'(empty string ) will be assinged to 
 * those fields
 * @return a pointer to newly created qname struct
 */

AXIS2_EXTERN axis2_qname_t * AXIS2_CALL
axis2_qname_create (const axis2_env_t *env,
                    const axis2_char_t * localpart,
                    const axis2_char_t * namespace_uri,
                    const axis2_char_t * prefix);


/**
 * returns a newly created qname using a string genarated from
 * axis2_qname_to_string method 
 * freeing the returned qname is users responsibility
 */           

AXIS2_EXTERN axis2_qname_t* AXIS2_CALL 
axis2_qname_create_from_string(const axis2_env_t *env,
                               const axis2_char_t *string);
   

#define AXIS2_QNAME_FREE(qname, env) \
        ((qname)->ops->free_fn(qname,env))
        
#define AXIS2_QNAME_EQUALS(qname, env, qname1) \
        ((qname)->ops->equals(qname, env, qname1))

#define AXIS2_QNAME_CLONE(qname, env) \
        ((qname)->ops->clone(qname, env))        
        
#define AXIS2_QNAME_GET_URI(qname, env) \
        ((qname)->ops->get_uri(qname, env))
        
#define AXIS2_QNAME_GET_PREFIX(qname, env) \
        ((qname)->ops->get_prefix(qname, env))
        
#define AXIS2_QNAME_GET_LOCALPART(qname, env) \
        ((qname)->ops->get_localpart(qname, env))
        
#define AXIS2_QNAME_TO_STRING(qname, env) \
        ((qname)->ops->to_string(qname, env))        
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_QNAME_H */
