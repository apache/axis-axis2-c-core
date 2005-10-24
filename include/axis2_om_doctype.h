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

#ifndef AXIS2_OM_DOCTYPE_H
#define AXIS2_OM_DOCTYPE_H

/**
 *@file axis2_om_doctype.h	
 *@brief defines struct representing xml DTD and its manipulation functions
 */

#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_doctype;
    struct axis2_om_doctype_ops;

/**
 * @defgroup axis2_om_doctype OM Doctype
 * @ingroup axis2_om 
 * @{
 */

  /**
    * \brief OM doctype operations struct
    * Encapsulator struct for operations of axis2_om_doctype
    */
    typedef struct axis2_om_doctype_ops
    {
      /**
        * free doctype struct
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param om_doctype pointer to doctype struct to be freed
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        AXIS2_DECLARE_DATA axis2_status_t (AXIS2_CALL *axis2_om_doctype_ops_free) (axis2_environment_t *
                                                     environment,
                                                     struct axis2_om_doctype *
                                                     om_doctype);
    } axis2_om_doctype_ops_t;

  /**
    * \brief OM doctype struct
    * Handles XML document type in OM
    */
    typedef struct axis2_om_doctype
    {
        /** Doctype related operations */
        axis2_om_doctype_ops_t *ops;
        /** Doctype value */
        axis2_char_t *value;
    } axis2_om_doctype_t;

  /**
    * Creates a doctype struct
    * @param environment Environment. MUST  NOT be NULL, if NULL behaviour is undefined.
    * @param parent parent of the new node. Optinal, can be NULL. 
    * @param value doctype value 
    * @param node This is an out parameter. Mandatory, cannot be NULL.
    *                       Returns the node corresponding to the doctype created.
    *                       Node type will be set to AXIS2_OM_DOCTYPE
    * @return pointer to newly created doctype struct 
    */
    AXIS2_DECLARE(axis2_om_doctype_t *) axis2_om_doctype_create (axis2_environment_t *
                                                 environment,
                                                 axis2_om_node_t * parent,
                                                 const axis2_char_t * value,
                                                 axis2_om_node_t ** node);

/** free given doctype */    
#define axis2_om_doctype_free(environment, doctype) ((doctype)->ops->axis2_om_doctype_ops_free(environment, doctype))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_DOCTYPE_H */
