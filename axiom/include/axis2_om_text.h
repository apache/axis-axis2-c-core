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
#include <axis2_data_handler.h>

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
    * @brief OM text ops struct
    * Encapsulator struct for ops of axis2_om_text
    */
 AXIS2_DECLARE_DATA   typedef struct axis2_om_text_ops
    {
      /**
        * Free an axis2_om_text struct
        * @param env environment.  MUST NOT be NULL.
        * @param om_text pointer to om text struct to be freed.
        * @return satus of the op. AXIS2_SUCCESS on success
        *         AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *
        free) (struct axis2_om_text *om_text,
               const axis2_env_t *env);

      /**
        * Serialize op
        * @param env environment.  MUST NOT be NULL.
        * @param om_text pointer to om text struct to be serialized.
        * @param om_output OM output handler to be used in serializing.
        * @return satus of the op. AXIS2_SUCCESS on success,
        *            AXIS2_FAILURE on error
        */
        axis2_status_t (AXIS2_CALL *
        serialize) (struct axis2_om_text *om_text,
                    const axis2_env_t *env,
                    axis2_om_output_t *om_output);
       /**
        *  set the text value
        * @param om_text om_text struct
        * @param env environment , MUST NOT be NULL.
        * @param value text 
        * @return status of the op. AXIS2_SUCCESS on success,
        *           AXIS2_FAILURE on error.                                                
        */                                                
        axis2_status_t (AXIS2_CALL *
        set_value)(struct axis2_om_text *om_text,
                   const axis2_env_t *env,
                   const axis2_char_t *value);
       /**
        *  get text value 
        * @param om_text om_text struct
        * @param env environment, MUST NOT be NULL.
        * @return text value , NULL is returned if there is no text value.
        */                                                
        axis2_char_t* (AXIS2_CALL *
        get_value)(struct axis2_om_text *om_text,
                   const axis2_env_t *env);                                               
       /**
        * set optimized 
        * @param om_text pointer to om_text struct
        * @param env environment
        * @optimize optimize value
        * @returns AXIS2_SUCCESS
        */                                               
        axis2_status_t (AXIS2_CALL *
        set_optimize)(struct axis2_om_text *om_text,
                      const axis2_env_t *env,
                      axis2_bool_t optimize);

       /**
        * @param om_text text value
        * @param env environment
        * @param is_binary
        * @returns AXIS2_SUCCESS
        */
        axis2_status_t (AXIS2_CALL *
        set_is_binary)(struct axis2_om_text *om_text,
                       const axis2_env_t *env,
                       axis2_bool_t is_binary);
        
        axis2_data_handler_t* (AXIS2_CALL *
        get_data_handler)(struct axis2_om_text *om_text,
                        const axis2_env_t *env);
        
        axis2_char_t* (AXIS2_CALL *
        get_content_id)(struct axis2_om_text *om_text,
                        const axis2_env_t *env);
                                                                                                
    } axis2_om_text_ops_t;

  /** 
    * \brief OM Text struct
    * Handles the XML text in OM
    */
    typedef struct axis2_om_text
    {
        /** OM text related ops */
        axis2_om_text_ops_t *ops;

    } axis2_om_text_t;


  /**
    * Creates a new text struct
    * @param env Environment. MUST  NOT be NULL, .
    * @param parent parent of the new node. Optinal, can be NULL. 
    *          The parent element must be of type AXIS2_OM_ELEMENT
    * @param value Text value. Optinal, can be NULL.
    * @param comment_node This is an out parameter.  cannot be NULL.
    *                       Returns the node corresponding to the text struct created.
    *                       Node type will be set to AXIS2_OM_TEXT    
    * @return pointer to newly created text struct 
    */
    AXIS2_EXTERN axis2_om_text_t * AXIS2_CALL 
    axis2_om_text_create (const axis2_env_t *env,
                          axis2_om_node_t *parent,
                          const axis2_char_t *value,
                          axis2_om_node_t ** node);
                          
  /**
    * Creates a new text struct for binary data (MTOM)
    * @param env Environment. MUST  NOT be NULL, .
    * @param parent parent of the new node. Optinal, can be NULL. 
    *          The parent element must be of type AXIS2_OM_ELEMENT
    * @param data_handler data handler. Optinal, can be NULL.
    * @param comment_node This is an out parameter.  cannot be NULL.
    *                       Returns the node corresponding to the text struct created.
    *                       Node type will be set to AXIS2_OM_TEXT    
    * @return pointer to newly created text struct 
    */
    AXIS2_EXTERN axis2_om_text_t* AXIS2_CALL
    axis2_om_text_create_with_data_handler (const axis2_env_t *env,
                          axis2_om_node_t * parent,
                          axis2_data_handler_t* data_handler,
                          axis2_om_node_t **node);

/** frees given text */
#define AXIS2_OM_TEXT_FREE( om_text,env) \
        ((om_text)->ops->free(om_text,env))                          

/** serializes given text */
#define AXIS2_OM_TEXT_SERIALIZE(om_text, env, om_output) \
        ((om_text)->ops->serialize(om_text, env, om_output))
/** get the text value */        
#define AXIS2_OM_TEXT_GET_VALUE(om_text, env) \
        ((om_text)->ops->get_value(om_text, env))
/** set text value */
#define AXIS2_OM_TEXT_SET_VALUE(om_text, env, value) \
        ((om_text)->ops->set_value(om_text, env, value))
        
/** set whether it is to be optimized */        
#define AXIS2_OM_TEXT_SET_OPTIMIZE(om_text, env, optimize) \
        ((om_text)->ops->set_optimize(om_text, env, optimize))

/** set whether it binary */
#define AXIS2_OM_TEXT_SET_IS_BINARY(om_text, env, is_binary) \
        ((om_text)->ops->set_is_binary(om_text, env, is_binary))    

#define AXIS2_OM_TEXT_SET_GET_DATA_HANDLER(om_text, env) \
        ((om_text)->ops->get_data_handler(om_text, env))
        
#define AXIS2_OM_TEXT_SET_GET_CONTENT_ID(om_text, env) \
        ((om_text)->ops->get_content_id(om_text, env))
        
/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_TEXT_H */
