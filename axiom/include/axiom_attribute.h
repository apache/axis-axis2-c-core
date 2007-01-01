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

#ifndef AXIOM_ATTRIBUTE_H
#define AXIOM_ATTRIBUTE_H

/**
* @file axiom_attribute.h
* @brief om attribute struct represents an xml attribute
*/
#include <axis2_env.h>
#include <axis2_qname.h>
#include <axiom_namespace.h>
#include <axiom_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axiom_attribute;
    struct axiom_attribute_ops;

/**
 * @defgroup axiom_attribute attribute
 * @ingroup axiom_om
 * @{
 */

/**
 *   \brief attribute ops struct
 *   ops Encapsulator struct for axiom_attribute
 */
    typedef struct axiom_attribute_ops
    {
        int dummy; /* if this is missing, windows compiler goes crazy */
    } axiom_attribute_ops_t;

  /**
    * \brief attribute struct
    * Handles the XML attribute in AXIOM
    */
    typedef struct axiom_attribute
    {
        /** ops of attribute struct */
	  const axiom_attribute_ops_t *ops;
	  int ref;
       
    } axiom_attribute_t;

  /**
    * creates an om_attribute struct 
    * @param env Environment. MUST NOT be NULL
    * @param localname localname of the attribute, should not be a null value.
    * @param value normalized attribute value. cannot be NULL
    * @param ns namespace, if any, of the attribute. Optional, can be NULL
    * @return a pointer to newly created attribute struct, returns NULL on error with 
    *           error code set in environment's error. 
    */
    AXIS2_EXTERN axiom_attribute_t * AXIS2_CALL
    axiom_attribute_create (const axis2_env_t *env,
                               const axis2_char_t *localname,
                               const axis2_char_t *value,
                               axiom_namespace_t *ns);
    /**
     * Free om attribute passed as void pointer. This will be
     * cast into appropriate type and then pass the cast object
     * into the om_attribute structure's free method
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_attribute_free_void_arg (
            void *om_attribute,
            const axis2_env_t *env);
     
  /**
    * Free an axiom_attribute struct
    * @param  om_attribute pointer to attribute struct to be freed
    * @param  env Environment. MUST NOT be NULL
    * @return satus of the op. AXIS2_SUCCESS on success 
    *         else AXIS2_FAILURE
    */

    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_attribute_free(struct axiom_attribute *om_attribute,
          const axis2_env_t *env);

  /** 
    * Creates and returns a qname struct for this attribute
    * @param om_attribute pointer to attribute struct 
    *        for which the qname is to be returned
    * @param env Environment. MUST NOT be NULL
    * @return returns qname for given attribute.NULL on error 
    */

    AXIS2_EXTERN axis2_qname_t * AXIS2_CALL 
    axiom_attribute_get_qname(struct axiom_attribute *om_attribute,
               const axis2_env_t *env);

  /**
    * Serialize op
    * @param om_attribute pointer to attribute struct to be serialized
    * @param env Environment. MUST NOT be NULL,
    * @param om_output AXIOM output handler to be used in serializing
    * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
    */

    AXIS2_EXTERN int AXIS2_CALL 
    axiom_attribute_serialize(struct axiom_attribute *om_attribute,
               const axis2_env_t *env,
               axiom_output_t *om_output);
   
   /**  returns the localname of this attribute
    *@param om_attribute pointer to attribute struct 
    *@param env environment. MUST NOT not be NULL.
    *@return localname returns NULL on error.
    */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axiom_attribute_get_localname(struct axiom_attribute *om_attribute,
                   const axis2_env_t *env);
   /**
    * returns value of this attribute 
    *@param om_attribute pointer to om_attribute struct 
    *@param env environment N not be null
    *@return value , null on error
    */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axiom_attribute_get_value(struct axiom_attribute *om_attribute,
               const axis2_env_t *env);
   /**
    * returns namespace of this attribute 
    *@param om_attribute
    *@param env environment MUST NOT be NULL
    *@return a pointer to om_namespace struct , returns NULL on error.
    */
    AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL 
    axiom_attribute_get_namespace(struct axiom_attribute *om_attribute,
                   const axis2_env_t *env);
   
   /** sets the localname of the attribute
    *@param om_attribute pointer to om attribute struct. 
    *@param env environment, MUST NOT be null.
    *@param localname localname that should be set for this attribute 
    *@return status code AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_attribute_set_localname(struct axiom_attribute *om_attribute,
                   const axis2_env_t *env,
                   const axis2_char_t *localname);
   
   /** set the attribute value
    *@param om_attribute a pointer to om_attribute struct.
    *@param env environment, MUST NOT be NULL.
    *@param value value that should be set for this attribute 
    *@return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
    */
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_attribute_set_value(struct axiom_attribute *om_attribute,
               const axis2_env_t *env,
               const axis2_char_t *value);
   
   /** set namespace of the attribute
    *@param om_attribute a pointer to om_attribute struct  
    *@param env environment, MUST NOT be NULL.
    *@param om_namespace a pointer to om_namespace struct that should be set 
    *                    for this attribute 
    *@return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axiom_attribute_set_namespace(struct axiom_attribute *om_attribute,
                   const axis2_env_t *env,
                   axiom_namespace_t *om_namespace);

    /**
     * clones an om attribute
     * @param om_attibute 
     * @param env environment
     * @returns pointer to cloned om attribute struct on success
     * NULL otherwise
     */            
    AXIS2_EXTERN struct axiom_attribute* AXIS2_CALL 
    axiom_attribute_clone(struct axiom_attribute *om_attribute,
           const axis2_env_t *env);


     
/******************** Macros **************************************************/
    
    
/** free given attribute */
#define AXIOM_ATTRIBUTE_FREE(om_attribute, env) \
        axiom_attribute_free(om_attribute, env)
        
/** get qname of given attribute */
#define AXIOM_ATTRIBUTE_GET_QNAME(om_attribute,env) \
        axiom_attribute_get_qname(om_attribute, env)
        
/** serialize given attribute */
#define AXIOM_ATTRIBUTE_SERIALIZE(om_attribute, env, om_ouput) \
        axiom_attribute_serialize(om_attribute, env, om_output)
/** get namespace of an the attribute */        
#define AXIOM_ATTRIBUTE_GET_NAMESPACE(om_attribute, env) \
        axiom_attribute_get_namespace(om_attribute, env)
/** get attribute localname */
#define AXIOM_ATTRIBUTE_GET_LOCALNAME(om_attribute, env) \
        axiom_attribute_get_localname(om_attribute, env)
/** grt value of attribute */        
#define AXIOM_ATTRIBUTE_GET_VALUE(om_attribute, env) \
        axiom_attribute_get_value(om_attribute, env)
/** set namespace for this attribute */               
#define AXIOM_ATTRIBUTE_SET_NAMESPACE(om_attribute, env,ns) \
        axiom_attribute_set_namespace(om_attribute, env,ns)
/** set localname for this attribute */        
#define AXIOM_ATTRIBUTE_SET_LOCALNAME(om_attribute, env,localname) \
        axiom_attribute_set_localname(om_attribute, env,localname)
/** set attribue value */
#define AXIOM_ATTRIBUTE_SET_VALUE(om_attribute, env,value) \
        axiom_attribute_set_value(om_attribute, env,value)

#define AXIOM_ATTRIBUTE_CLONE(om_attribute, env) \
        axiom_attribute_clone(om_attribute, env)
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* AXIOM_ATTRIBUTE_H */
