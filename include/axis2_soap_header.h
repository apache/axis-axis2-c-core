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
 
 #ifndef AXIS2_SOAP_HEADER_H
 #define AXIS2_SOAP_HEADER_H
 
   /**
    * @file axis2_soap_header.h
    * @brief axis2_soap_header struct
    */
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_array_list.h>
#include <axis2_om_children_qname_iterator.h>
#include <axis2_om_children_with_specific_attribute_iterator.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_header_ops axis2_soap_header_ops_t;
    
    
/**
 * @defgroup axis2_soap_header
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_header operations struct
 *   ops Encapsulator struct of axis2_soap header
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_header_ops
    {
      /**
        * Free an axis2_soap_header
        * @param  header pointer to soap_header struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *free_fn)(axis2_soap_header_t *header,
                                             axis2_env_t **env);
                                             
       /**
        * Creates a new <CODE>SOAPHeaderBlock</CODE> object initialized with the
        * specified name and adds it to this <CODE>SOAPHeader</CODE> object.
        *
        * @param localName
        * @param ns
        * @return the new <CODE>SOAPHeaderBlock</CODE> object that was inserted
        *         into this <CODE>SOAPHeader</CODE> object
        */
        axis2_soap_header_block_t* (AXIS2_CALL *add_header_block)
                                        (axis2_soap_header_t* header,
                                         axis2_env_t **env,
                                         axis2_char_t *localname,
                                         axis2_om_namespace_t *ns); 
        /**
        * Returns a list of all the <CODE>SOAPHeaderBlock</CODE> objects in this
        * <CODE>SOAPHeader</CODE> object that have the the specified actor. An
        * actor is a global attribute that indicates the intermediate parties to
        * whom the message should be sent. An actor receives the message and then
        * sends it to the next actor. The default actor is the ultimate intended
        * recipient for the message, so if no actor attribute is included in a
        * <CODE>SOAPHeader</CODE> object, the message is sent to its ultimate
        * destination.
        */
        axis2_hash_t* (AXIS2_CALL *examine_header_blocks)
                                    (axis2_soap_header_t* header,
                                     axis2_env_t **env,
                                     axis2_char_t* param_role);
    
        axis2_om_children_qname_iterator_t* (AXIS2_CALL examine_all_header_blocks)
                                    (axis2_soap_header_t* header,
                                     axis2_env_t **env);
        
        axis2_om_children_with_specific_attribute_iterator_t *
                (AXIS2_CALL *extract_header_blocks)(axis2_soap_header_t *header,
                 axis2_env_t **env,
                 axis2_char_t *role);
                                             
                                            
                                         
       /**
        * This is only intended to be used by the builder,
        * do not use this function in other places
        */
        axis2_status_t (AXIS2_CALL *set_base)(axis2_soap_header_t *header,
                                              axis2_env_t **env,
                                              axis2_om_node_t *node);
                                              
        axis2_om_node_t* (AXIS2_CALL *get_base)(axis2_soap_header_t *header,
                                                axis2_env_t **env);                                                                                                                                                          
                                         
    };

  /**
    * \brief soap_header struct
    * represent a soap_header
    */
    struct axis2_soap_header
    {
        /** operation of axis2_soap_header struct */
        axis2_soap_header_ops_t *ops;
       
    };

  /**
    * creates a soap struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_header_t *)
axis2_soap_header_create_with_parent(axis2_env_t **env,
                                       axis2_soap_envelope_t *envelope);
                            

AXIS2_DECLARE(axis2_soap_header_t *)
axis2_soap_header_create(axis2_env_t **env);

/******************** Macros **************************************************/
    
    
/** free soap_header */
#define AXIS2_SOAP_HEADER_FREE(header , env) \
        ((header)->ops->free_fn(header, env))

#define AXIS2_SOAP_HEADER_ADD_HEADER_BLOCK(header, env, localname, ns) \
        ((header)->ops->add_header_block(header, env, localname, ns))
        
#define AXIS2_SOAP_HEADER_EXAMINE_HEADER_BLOCKS(header, env, param_role) \
        ((header)->ops->examine_header_blocks(header, env, param_role))

#define AXIS2_SOAP_HEADER_EXAMINE_ALL_HEADER_BLOCKS(header, env)
        ((header)->ops->examine_all_header_blocks(header, env))
        
#define AXIS2_SOAP_HEADER_EXTRACT_HEADER_BLOCKS(header, env) \
        ((header)->ops->extract_header_blocks(header, env))                        

#define AXIS2_SOAP_HEADER_SET_BASE(header, env, node) \
        ((header)->ops->set_base(header, env, node))
        
#define AXIS2_SOAP_HEADER_GET_BASE(header, env) \
        ((header)->ops->get_base(header, env))                       

/** @} */

#ifdef __cplusplus
}
#endif

 
 #endif /* AXIS2_SOAP_HEADER_H */