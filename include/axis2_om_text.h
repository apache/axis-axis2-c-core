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

#ifndef AXIS2_OM_TEXT_H
#define AXIS2_OM_TEXT_H

/**
 * @file axis2_om_text.h
 * @brief Axis2 OM XML text
 */

#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_om_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_text;
    struct axis2_om_text_ops;

/**
 * @defgroup axis2_om_text OM Text
 * @ingroup axis2_om 
 * @{
 */

  /** 
    * @brief OM text operations struct
    * Encapsulator struct for operations of axis2_om_text
    */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_text_ops
    {
      /**
        * Free an axis2_om_text struct
        * @param environment environment.  MUST NOT be NULL, .
        * @param om_text pointer to om text struct to be freed
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *free) (struct axis2_om_text *om_text,
                                           axis2_env_t **env);

      /**
        * Serialize operation
        * @param environment environment.  MUST NOT be NULL, .
        * @param om_text pointer to om text struct to be serialized
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *serialize) (struct axis2_om_text *om_text,
                                                axis2_env_t **env,
                                                axis2_om_output_t *om_output);
                                                
        axis2_status_t (AXIS2_CALL *set_value)(struct axis2_om_text *om_text,
                                               axis2_env_t **env,
                                               const axis2_char_t *value);

        axis2_char_t* (AXIS2_CALL *get_value)(struct axis2_om_text *om_text,
                                              axis2_env_t **env);                                               
                                               
                                                                                                
    } axis2_om_text_ops_t;

  /** 
    * \brief OM Text struct
    * Handles the XML text in OM
    */
    typedef struct axis2_om_text
    {
        /** OM text related operations */
        axis2_om_text_ops_t *ops;

    } axis2_om_text_t;


  /**
    * Creates a new text struct
    * @param environment Environment. MUST  NOT be NULL, .
    * @param parent parent of the new node. Optinal, can be NULL. 
    *          The parent element must be of type AXIS2_OM_ELEMENT
    * @param value Text value. Optinal, can be NULL.
    * @param comment_node This is an out parameter.  cannot be NULL.
    *                       Returns the node corresponding to the text struct created.
    *                       Node type will be set to AXIS2_OM_TEXT    
    * @return pointer to newly created text struct 
    */
    AXIS2_DECLARE(axis2_om_text_t *) 
    axis2_om_text_create (axis2_env_t **env,
                          axis2_om_node_t *parent,
                          const axis2_char_t *value,
                          axis2_om_node_t ** node);
                          
/** frees given text */
#define AXIS2_OM_TEXT_FREE( om_text,env) \
        ((om_text)->ops->free(om_text,env))                          

/** serializes given text */
#define AXIS2_OM_TEXT_SERIALIZE(om_text, env, om_output) \
        ((om_text)->ops->serialize(om_text, env, om_output))
        
#define AXIS2_OM_TEXT_GET_VALUE(om_text, env) \
        ((om_text)->ops->get_value(om_text, env))

#define AXIS2_OM_TEXT_SET_VALUE(om_text, env, value) \
        ((om_text)->ops->set_value(om_text, env, value))
              


/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_TEXT_H */
