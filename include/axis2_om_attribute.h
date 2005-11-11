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
 *   \brief OM attribute operations struct
 *   Encapsulator struct for axis2_om_attribute
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_attribute_ops
    {
      /**
        * Free an axis2_om_attribute struct
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_attribute pointer to attribute struct to be freed
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */

         axis2_status_t 
         (AXIS2_CALL *free) (struct axis2_om_attribute *om_attribute,
                                                    axis2_env_t **env);

      /** 
        * Creates and returns a qname struct for this attribute
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_attribute pointer to attribute struct for which the qname is to be returned
        * @return returns qname for given attribute. NULL on error 
        */

         axis2_qname_t *
         (AXIS2_CALL *get_qname) (struct axis2_om_attribute *om_attribute,
                                                         axis2_env_t **env);

      /**
        * Serialize operation
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_attribute pointer to attribute struct to be serialized
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */

         int 
         (AXIS2_CALL *serialize) (struct axis2_om_attribute *om_attribute,
                                                 axis2_env_t **env,
                                                 axis2_om_output_t *om_output);
        axis2_char_t* 
        (AXIS2_CALL *get_localname)(struct axis2_om_attribute *om_attribute,
                                    axis2_env_t **env);

        axis2_char_t* 
        (AXIS2_CALL *get_value)(struct axis2_om_attribute *om_attribute,
                                axis2_env_t **env);

        axis2_om_namespace_t* 
        (AXIS2_CALL *get_namespace)(struct axis2_om_attribute *om_attribute,
                                    axis2_env_t **env);

        axis2_status_t 
        (AXIS2_CALL *set_localname)(struct axis2_om_attribute *om_attribute,
                                    axis2_env_t **env,
                                    const axis2_char_t *localname);

        axis2_status_t 
        (AXIS2_CALL *set_value)(struct axis2_om_attribute *om_attribute,
                                axis2_env_t **env,
                                const axis2_char_t *value);

        axis2_status_t 
        (AXIS2_CALL *set_namespace)(struct axis2_om_attribute *om_attribute,
                                    axis2_env_t **env,
                                    axis2_om_namespace_t *om_namespace);
                                                                                                                                                                                             


    } axis2_om_attribute_ops_t;

  /**
    * \brief OM attribute struct
    * Handles the XML attribute in OM
    */
    typedef struct axis2_om_attribute
    {
        /** operations of attribute struct */
        axis2_om_attribute_ops_t *ops;
       
    } axis2_om_attribute_t;

  /**
    * creates an om_attribute structure 
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param localname The local part of the attribute name. Mandatory, cannot be NULL
    * @param value normalized attribute value. Mandatory, cannot be NULL
    * @param ns namespace, if any, of the attribute. Optional, can be NULL
    * @return a pointer to newly created attribute struct 
    */
    AXIS2_DECLARE(axis2_om_attribute_t *)
     axis2_om_attribute_create (axis2_env_t **env,
                                const axis2_char_t *localname,
                                const axis2_char_t *value,
                                axis2_om_namespace_t *ns);

/******************** Macros *********************************************/

/** free given attribute */
#define AXIS2_OM_ATTRIBUTE_FREE(om_attribute, env) \
        ((om_attribute)->ops->free(om_attribute, env))
        
/** get qname of given attribute */
#define AXIS2_OM_ATTRIBUTE_GET_QNAME(om_attribute,env) \
        ((om_attribute)->ops->get_qname(om_attribute, env))
        
/** serialize given attribute */
#define AXIS2_OM_ATTRIBUTE_SERIALIZE(om_attribute, env, om_ouput) \
        ((om_attribute)->ops->serialize(om_attribute, env, om_output))
        
#define AXIS2_OM_ATTRIBUTE_GET_NAMESPACE(om_attribute, env) \
        ((om_attribute)->ops->get_namespace(om_attribute, env))

#define AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attribute, env) \
        ((om_attribute)->ops->get_localname(om_attribute, env))
        
#define AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attribute, env) \
        ((om_attribute)->ops->get_value(om_attribute, env))
               
#define AXIS2_OM_ATTRIBUTE_SET_NAMESPACE(om_attribute, env,ns) \
        ((om_attribute)->ops->set_namespace(om_attribute, env,ns))
        
#define AXIS2_OM_ATTRIBUTE_SET_LOCALNAME(om_attribute, env,localname) \
        ((om_attribute)->ops->set_localname(om_attribute, env,localname))

#define AXIS2_OM_ATTRIBUTE_SET_VALUE(om_attribute, env,value) \
        ((om_attribute)->ops->set_value(om_attribute, env,value))
                
                

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_ATTRIBUTE_H */
