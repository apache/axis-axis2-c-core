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

         axis2_status_t (AXIS2_CALL *axis2_om_attribute_ops_free) (axis2_env_t *
                                                       environment,
                                                       struct
                                                       axis2_om_attribute *
                                                       om_attribute);

      /** 
        * Creates and returns a qname struct for this attribute
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_attribute pointer to attribute struct for which the qname is to be returned
        * @return returns qname for given attribute. NULL on error 
        */

         axis2_qname_t
            *(AXIS2_CALL *axis2_om_attribute_ops_get_qname) (axis2_env_t *
                                                  environment,
                                                  struct axis2_om_attribute *
                                                  om_attribute);

      /**
        * Serialize operation
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_attribute pointer to attribute struct to be serialized
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */

         int (AXIS2_CALL *axis2_om_attribute_ops_serialize) (axis2_env_t *
                                                 environment,
                                                 struct axis2_om_attribute *
                                                 om_attribute,
                                                 axis2_om_output_t *
                                                 om_output);

    } axis2_om_attribute_ops_t;

  /**
    * \brief OM attribute struct
    * Handles the XML attribute in OM
    */
    typedef struct axis2_om_attribute
    {
        /** operations of attribute struct */
        axis2_om_attribute_ops_t *ops;
        /** localname of this attribute  */
        axis2_char_t *localname;
        /** value of this attribute */
        axis2_char_t *value;
        /** attribute namespace */
        axis2_om_namespace_t *ns;
    } axis2_om_attribute_t;

  /**
    * creates an om_attribute structure 
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param localname The local part of the attribute name. Mandatory, cannot be NULL
    * @param value normalized attribute value. Mandatory, cannot be NULL
    * @param ns namespace, if any, of the attribute. Optional, can be NULL
    * @return a pointer to newly created attribute struct 
    */
    AXIS2_DECLARE(axis2_om_attribute_t *) axis2_om_attribute_create (axis2_env_t *
                                                     environment,
                                                     const axis2_char_t *
                                                     localname,
                                                     const axis2_char_t *
                                                     value,
                                                     axis2_om_namespace_t *
                                                     ns);

/* macros */
/** free given attribute */
#define axis2_om_attribute_free(environment, om_attribute) ((om_attribute)->ops->axis2_om_attribute_ops_free(environment, om_attribute))
/** get qname of given attribute */
#define axis2_om_attribute_get_qname(environment, om_attribute) ((om_attribute)->ops->axis2_om_attribute_ops_get_qname(environment, om_attribute))
/** serialize given attribute */
#define axis2_om_attribute_serialize(environment, om_attribute,om_ouput) ((om_attribute)->ops->axis2_om_attribute_ops_serialize(environment, om_attribute,om_output))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_ATTRIBUTE_H */
