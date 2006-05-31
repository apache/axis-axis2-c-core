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

#ifndef AXIS2_OM_ATTRIBUTE_H
#define AXIS2_OM_ATTRIBUTE_H

/**
* @file axis2_om_attribute.h
* @brief om attribute struct represents an xml attribute
*/
#include <axis2_env.h>
#include <axis2_qname.h>
#include <axis2_om_namespace.h>
#include <axis2_om_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_attribute;
    struct axis2_om_attribute_ops;

/**
 * @defgroup axis2_om_attribute OM Attribute
 * @ingroup axis2_om 
 * @{
 */

/**
 *   \brief OM attribute ops struct
 *   ops Encapsulator struct for axis2_om_attribute
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_attribute_ops
    {
      /**
        * Free an axis2_om_attribute struct
        * @param  om_attribute pointer to attribute struct to be freed
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
        free)(struct axis2_om_attribute *om_attribute,
              const axis2_env_t *env);

      /** 
        * Creates and returns a qname struct for this attribute
        * @param om_attribute pointer to attribute struct 
        *        for which the qname is to be returned
        * @param env Environment. MUST NOT be NULL
        * @return returns qname for given attribute.NULL on error 
        */

        axis2_qname_t * (AXIS2_CALL *
        get_qname)(struct axis2_om_attribute *om_attribute,
                   const axis2_env_t *env);

      /**
        * Serialize op
        * @param om_attribute pointer to attribute struct to be serialized
        * @param env Environment. MUST NOT be NULL,
        * @param om_output OM output handler to be used in serializing
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */

        int (AXIS2_CALL *
        serialize)(struct axis2_om_attribute *om_attribute,
                   const axis2_env_t *env,
                   axis2_om_output_t *om_output);
       
       /**  returns the localname of this attribute
        *@param om_attribute pointer to attribute struct 
        *@param env environment. MUST NOT not be NULL.
        *@return localname returns NULL on error.
        */
        axis2_char_t* (AXIS2_CALL *
        get_localname)(struct axis2_om_attribute *om_attribute,
                       const axis2_env_t *env);
       /**
        * returns value of this attribute 
        *@param om_attribute pointer to om_attribute struct 
        *@param env environment N not be null
        *@return value , null on error
        */
        axis2_char_t* (AXIS2_CALL *
        get_value)(struct axis2_om_attribute *om_attribute,
                   const axis2_env_t *env);
       /**
        * returns namespace of this attribute 
        *@param om_attribute
        *@param env environment MUST NOT be NULL
        *@return a pointer to om_namespace struct , returns NULL on error.
        */
        axis2_om_namespace_t* (AXIS2_CALL *
        get_namespace)(struct axis2_om_attribute *om_attribute,
                       const axis2_env_t *env);
       
       /** sets the localname of the attribute
        *@param om_attribute pointer to om attribute struct. 
        *@param env environment, MUST NOT be null.
        *@param localname localname that should be set for this attribute 
        *@return status code AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *
        set_localname)(struct axis2_om_attribute *om_attribute,
                       const axis2_env_t *env,
                       const axis2_char_t *localname);
       
       /** set the attribute value
        *@param om_attribute a pointer to om_attribute struct.
        *@param env environment, MUST NOT be NULL.
        *@param value value that should be set for this attribute 
        *@return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
        */
        
        axis2_status_t (AXIS2_CALL *
        set_value)(struct axis2_om_attribute *om_attribute,
                   const axis2_env_t *env,
                   const axis2_char_t *value);
       
       /** set namespace of the attribute
        *@param om_attribute a pointer to om_attribute struct  
        *@param env environment, MUST NOT be NULL.
        *@param om_namespace a pointer to om_namespace struct that should be set 
        *                    for this attribute 
        *@return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *
        set_namespace)(struct axis2_om_attribute *om_attribute,
                       const axis2_env_t *env,
                       axis2_om_namespace_t *om_namespace);

        /**
         * clones an om attribute
         * @param om_attibute 
         * @param env environment
         * @returns pointer to cloned om attribute struct on success
         * NULL otherwise
         */            
        struct axis2_om_attribute* (AXIS2_CALL *
        clone)(struct axis2_om_attribute *om_attribute,
               const axis2_env_t *env);


    } axis2_om_attribute_ops_t;

  /**
    * \brief OM attribute struct
    * Handles the XML attribute in OM
    */
    typedef struct axis2_om_attribute
    {
        /** ops of attribute struct */
        axis2_om_attribute_ops_t *ops;
       
    } axis2_om_attribute_t;

  /**
    * creates an om_attribute struct 
    * @param env Environment. MUST NOT be NULL
    * @param localname localname of the attribute, should not be a null value.
    * @param value normalized attribute value. cannot be NULL
    * @param ns namespace, if any, of the attribute. Optional, can be NULL
    * @return a pointer to newly created attribute struct, returns NULL on error with 
    *           error code set in environment's error. 
    */
    AXIS2_DECLARE(axis2_om_attribute_t *)
    axis2_om_attribute_create (const axis2_env_t *env,
                               const axis2_char_t *localname,
                               const axis2_char_t *value,
                               axis2_om_namespace_t *ns);
    /**
     * Free om attribute passed as void pointer. This will be
     * cast into appropriate type and then pass the cast object
     * into the om_attribute structure's free method
     */
    AXIS2_DECLARE(axis2_status_t) 
    axis2_om_attribute_free_void_arg (
            void *om_attribute,
            const axis2_env_t *env);
     
/******************** Macros **************************************************/
    
    
/** free given attribute */
#define AXIS2_OM_ATTRIBUTE_FREE(om_attribute, env) \
        ((om_attribute)->ops->free(om_attribute, env))
        
/** get qname of given attribute */
#define AXIS2_OM_ATTRIBUTE_GET_QNAME(om_attribute,env) \
        ((om_attribute)->ops->get_qname(om_attribute, env))
        
/** serialize given attribute */
#define AXIS2_OM_ATTRIBUTE_SERIALIZE(om_attribute, env, om_ouput) \
        ((om_attribute)->ops->serialize(om_attribute, env, om_output))
/** get namespace of an the attribute */        
#define AXIS2_OM_ATTRIBUTE_GET_NAMESPACE(om_attribute, env) \
        ((om_attribute)->ops->get_namespace(om_attribute, env))
/** get attribute localname */
#define AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attribute, env) \
        ((om_attribute)->ops->get_localname(om_attribute, env))
/** grt value of attribute */        
#define AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attribute, env) \
        ((om_attribute)->ops->get_value(om_attribute, env))
/** set namespace for this attribute */               
#define AXIS2_OM_ATTRIBUTE_SET_NAMESPACE(om_attribute, env,ns) \
        ((om_attribute)->ops->set_namespace(om_attribute, env,ns))
/** set localname for this attribute */        
#define AXIS2_OM_ATTRIBUTE_SET_LOCALNAME(om_attribute, env,localname) \
        ((om_attribute)->ops->set_localname(om_attribute, env,localname))
/** set attribue value */
#define AXIS2_OM_ATTRIBUTE_SET_VALUE(om_attribute, env,value) \
        ((om_attribute)->ops->set_value(om_attribute, env,value))

#define AXIS2_OM_ATTRIBUTE_CLONE(om_attribute, env) \
        ((om_attribute)->ops->clone(om_attribute, env))
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_OM_ATTRIBUTE_H */
