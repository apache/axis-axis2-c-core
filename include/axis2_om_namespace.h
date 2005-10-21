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

#ifndef AXIS2_OM_NAMESPACE_H
#define AXIS2_OM_NAMESPACE_H

/**
 * @file axis2_om_namespace.h
 * @brief defines axis2_om_namespace data structure, used for representing XML namespaces in OM
 */

#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_om_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_om_text OM Text
 * @ingroup axis2_om 
 * @{
 */

/** @cond */
    struct axis2_om_namespace;
    struct axis2_om_namespace_ops;
/** @endcond */

/** \struct axis2_om_namespace_ops_t
    \brief OM Namespace operations struct

    Encapsulator struct for operations of axis2_om_namespace_t
*/
    typedef struct axis2_om_namespace_ops
    {
  /**
    * Free an om_namespcae
    * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
    * @return Status code    
    */
        axis2_status_t (*axis2_om_namespace_ops_free) (axis2_environment_t *
                                                       environment,
                                                       struct
                                                       axis2_om_namespace *
                                                       om_namespace);

  /**
    * Compares two namepsaces
    * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
    * @param om_namespace1 first namespase
    * @param om_namespace2 second namespace to be compared
    * @return true if the two namespaces are equal, false otherwise	
    */
        axis2_bool_t (*axis2_om_namespace_ops_equals) (axis2_environment_t *
                                                       environment,
                                                       struct
                                                       axis2_om_namespace *
                                                       om_namespace1,
                                                       struct
                                                       axis2_om_namespace *
                                                       om_namespace2);

  /**
    * Serialize operation
    * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
    * @param om_output OM output handler to be used in serializing
    * @return Status code
    */
        axis2_status_t (*axis2_om_namespace_ops_serialize)
            (axis2_environment_t * environment,
             struct axis2_om_namespace * om_namespace,
             axis2_om_output_t * om_output);

    } axis2_om_namespace_ops_t;

/** \struct axis2_om_namespace_t
    \brief OM namespace struct

    Handles the XML namespace in OM
*/
    typedef struct axis2_om_namespace
    {
  /**
    * OM text related operations
    */
        axis2_om_namespace_ops_t *ops;
  /**
    * OM namespace URI
    */
        axis2_char_t *uri;
  /**
    * OM namespace prefix
    */
        axis2_char_t *prefix;
    } axis2_om_namespace_t;

/**
 *	Creates an axis2_om_namespace_t struct
 *  @param uri namespace URI
 *  @param prefix namespace prefix
 *  @return a pointer to newly created axis2_om_namespace_t struct
 */

    axis2_om_namespace_t *axis2_om_namespace_create (axis2_environment_t *
                                                     environment,
                                                     const axis2_char_t * uri,
                                                     const axis2_char_t *
                                                     prefix);

#define axis2_om_namespace_free(environment, om_namespace) ((om_namespace)->ops->axis2_om_namespace_ops_free(environment, om_namespace))
#define axis2_om_namespace_equals(environment, om_namespace1, om_namespace2) ((om_namespace1)->ops->axis2_om_namespace_ops_equals(environment, om_namespace1, om_namespace2))
#define axis2_om_namespace_serialize(environment, om_namespace,om_output) ((om_namespace)->ops->axis2_om_namespace_ops_serialize(environment, om_namespace, om_output))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_NAMESPACE */
