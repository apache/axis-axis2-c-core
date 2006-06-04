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
#include <axis2_om_output.h>

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
    * \brief OM doctype ops struct
    * Encapsulator struct for ops of axis2_om_doctype
    */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_doctype_ops
    {
      /**
        * free doctype struct
        * @param om_doctype pointer to axis2_om_doctype_t struct to be freed
        * @param env Environment. MUST NOT be NULL,
        * @return satus of the op. AXIS2_SUCCESS on success
        *         AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *
        free)(struct axis2_om_doctype *om_doctype,
              const axis2_env_t *env);
       /**
        * @param om_doctype pointer to a axis2_om_doctype_t struct
        * @param env environment must not be null       
        * @return DTD text 
        */
        axis2_char_t* (AXIS2_CALL *
        get_value)(struct axis2_om_doctype *om_doctype,
                   const axis2_env_t *env);
       /**
        * @param om_doctype pointer to axis2_om doctype_t struct
        * @param env environment , MUST NOT be NULL.
        * @param value doctype text value
        * @return status of the op,
        *         AXIS2_SUCCESS on success, AXIS2_FAILURE on error.
        */

        axis2_status_t (AXIS2_CALL *
        set_value)(struct axis2_om_doctype *om_doctype,
                   const axis2_env_t *env,
                   const axis2_char_t *value);
       /**
        * serialize op 
        * @param om_doctype pointer to axis2_om_doctype_t struct
        * @param env environment , MUST NOT be NULL
        * @param om_output pointer to axis2_om_output_t struct
        * @returns status of the op,
        *          AXIS2_SUCCESS on success, AXIS2_FAILURE on error.
        */                                   
        
        axis2_status_t (AXIS2_CALL *
        serialize)(struct axis2_om_doctype *om_doctype,
                   const axis2_env_t *env,
                   axis2_om_output_t *om_output);
                                                                                                                                      
    } axis2_om_doctype_ops_t;

  /**
    * \brief OM doctype struct
    * Handles XML document type in OM
    */
    typedef struct axis2_om_doctype
    {
        /** Doctype related ops */
        axis2_om_doctype_ops_t *ops;

    } axis2_om_doctype_t;

  /**
    * Creates a axis2_om_doctype_t struct
    * @param env Environment. MUST  NOT be NULL,
    * @param parent parent of the new node. Optinal, can be NULL. 
    * @param value doctype text
    * @param node This is an out parameter.cannot be NULL.
    *               Returns the node corresponding to the doctype created.
    *               Node type will be set to AXIS2_OM_DOCTYPE
    * @return pointer to newly created doctype struct 
    */
    AXIS2_EXTERN axis2_om_doctype_t * AXIS2_CALL
    axis2_om_doctype_create (const axis2_env_t *env,
                             axis2_om_node_t * parent,
                             const axis2_char_t * value,
                             axis2_om_node_t ** node);

/** free given doctype */    
#define AXIS2_OM_DOCTYPE_FREE(doctype, env) \
        ((doctype)->ops->free(doctype, env))
/** returns the value of doctype */
#define AXIS2_OM_DOCTYPE_GET_VALUE(doctype, env) \
        ((doctype)->ops->get_value(doctype, value))
/** set the doctype value */
#define AXIS2_OM_DOCTYPE_SET_VALUE(doctype, env, value) \
        ((doctype)->ops->set_value(doctype, env, value))
/** serialize op */       
#define AXIS2_OM_DOCTYPE_SERIALIZE(doctype, env, om_output) \
        ((doctype)->ops->serialize(doctype, env, om_output))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_DOCTYPE_H */
