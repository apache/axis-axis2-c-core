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
 
 #ifndef AXIOM_SOAP_ENVELOPE_H
 #define AXIOM_SOAP_ENVELOPE_H
 
/**
 * @file axiom_soap_envelope.h
 * @brief axiom_soap_envelope struct
 *        corresponds to root element of soap message
 */
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_namespace.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_envelope        axiom_soap_envelope_t;
    typedef struct axiom_soap_envelope_ops    axiom_soap_envelope_ops_t;
    
    struct axiom_soap_body;
    struct axiom_soap_header;
    struct axiom_soap_header_block;
    struct axiom_soap_builder;
    
/**
 * @defgroup axiom_soap_envelope soap envelope
 * @ingroup axiom_soap
 * @{
 */

/**
 *   \brief soap_envelope operations struct
 *   ops Encapsulator struct of axiom_soap body
 */
    struct axiom_soap_envelope_ops
    {
        /**
         * gets the soap header of this soap envelope
         * @param envelope soap envelope
         * @param env environment must not be null
         * @return soap header null it no header is present
         */
        struct axiom_soap_header* (AXIS2_CALL *
      get_header)(axiom_soap_envelope_t *envelope,
                    const axis2_env_t *env);
        /**
         * Returns the soap body associated with this soap envelope
         * @param envelope soap_envelope
         * @param env environment
         * @return soap_body
         */
        struct axiom_soap_body* (AXIS2_CALL *
      get_body)(axiom_soap_envelope_t *envelope,
                  const axis2_env_t *env);
        /**
         * serialize function , serialize the soap envelope 
         * IF the soap version it set to soap11 the soap fault part is converted 
         * to soap11 fault even is the underlying soap fault is of soap12 type
         * @param envelope soap envelope
         * @param env environment must not be null
         * @param om_output 
         * @param cache whether caching is enabled or not
         * @return status code , AXIS2_SUCCESS if success ,
         *                 AXIS2_FAILURE otherwise
         */
        axis2_status_t (AXIS2_CALL *
      serialize)(axiom_soap_envelope_t *envelope,
                   const axis2_env_t *env,
                   axiom_output_t *om_output, 
                   axis2_bool_t cache);
      
        /**
         * Free function, This function deallocate all the resources associated 
         * with the soap_envelope
         * IT frees it's soap body and soap headers as well as the underlying
         * om node tree by calling AXIOM_NODE_FREE_TREE function
         * @param envelope soap_envelope
         * @param env environment
         * @return status code AXIS2_SUCCESS on success , AXIS2_FAILURE otherwise
         */         
        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_soap_envelope_t *envelope,
              const axis2_env_t *env);
           
        /**
         * returns the om_node associated with this soap envelope
         * @param envelope soap_envelope
         * @param env environment
         * @return axiom_node_t pointer
         */
        axiom_node_t* (AXIS2_CALL *
      get_base_node)(axiom_soap_envelope_t *envelope,
                       const axis2_env_t *env);

           
        /** returns the soap version of this soap envelope
         * @param envelope soap_envelope
         * @param env environment must not be null
         * @return soap_version AXIOM_SOAP12 or AXIOM_SOAP11
         */         
        int (AXIS2_CALL *
      get_soap_version)(axiom_soap_envelope_t *envelope,
                          const axis2_env_t *env);
                   
        /** return the soap envelope namespace 
         * @param envelope 
         * @param env 
         * @return axiom_namespace_t 
         */                                                
        axiom_namespace_t* (AXIS2_CALL *
      get_namespace)(axiom_soap_envelope_t *envelope,
                       const axis2_env_t *env);

        axis2_status_t (AXIS2_CALL *
        set_soap_version)(axiom_soap_envelope_t *envelope,
                          const axis2_env_t *env,
                          int soap_version);
     
    };

/**
 * \brief soap_envelope struct
 * represent a soap_envelope
 */
struct axiom_soap_envelope
{
  /** operation of axiom_soap_envelope struct */
  axiom_soap_envelope_ops_t *ops;
};

 /**
  * @param env Environment. MUST NOT be NULL
  * create a soap_envelope with the given namespace prefix and uri
  * as the prefix and uri, The uri of ns should be valid soap uri
  */
AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
axiom_soap_envelope_create(const axis2_env_t *env, 
                     axiom_namespace_t *ns);

 /**
  * @param env Environment. MUST NOT be NULL
  * create a soap_envelope with the given namespace prefix and uri is selected 
  * according to soap_version, soap version should be one of AXIOM_SOAP11
  * or AXIOM_SOAP12
  * @param prefix soap envelope prefix
  * if prefix is NULL default prefix is used
  * @returns a pointer to soap envelope struct
  * 
  */                     
AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
axiom_soap_envelope_create_with_soap_version_prefix(const axis2_env_t *env,
                                                    int soap_version,
                                                    const axis2_char_t *prefix);                     

AXIS2_EXTERN axiom_soap_envelope_t * AXIS2_CALL
axiom_soap_envelope_create_default_soap_envelope(const axis2_env_t *env,
                                                 int soap_version);
 
AXIS2_EXTERN axiom_soap_envelope_t * AXIS2_CALL
axiom_soap_envelope_create_default_soap_fault_envelope(const axis2_env_t *env,
    const axis2_char_t *code_value, 
    const axis2_char_t *reason_text, 
    const int soap_version,
    axis2_array_list_t *sub_codes,
    axiom_node_t *detail_node);

/******************** Macros **************************************************/

#define AXIOM_SOAP_ENVELOPE_GET_HEADER(envelope, env) \
        ((envelope)->ops->get_header(envelope, env))

#define AXIOM_SOAP_ENVELOPE_GET_BODY(envelope, env) \
        ((envelope)->ops->get_body(envelope, env))

#define AXIOM_SOAP_ENVELOPE_SERIALIZE(envelope, env, om_output, cache) \
        ((envelope)->ops->serialize(envelope, env, om_output, cache))
        
#define AXIOM_SOAP_ENVELOPE_FREE(envelope, env) \
        ((envelope)->ops->free_fn(envelope, env))
        
#define AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env) \
        ((envelope)->ops->get_base_node(envelope, env))

#define AXIOM_SOAP_ENVELOPE_GET_SOAP_VERSION(envelope, env) \
        ((envelope)->ops->get_soap_version(envelope, env))

#define AXIOM_SOAP_ENVELOPE_GET_NAMESPACE(envelope, env) \
        ((envelope)->ops->get_namespace(envelope, env))

#define AXIOM_SOAP_ENVELOPE_SET_SOAP_VERSION(envelope, env, soap_version) \
        ((envelope)->ops->set_soap_version(envelope, env, soap_version))
        

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIOM_SOAP_ENVELOPE_H */
