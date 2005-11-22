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

#ifndef AXIS2_OM_COMMENT_H
#define AXIS2_OM_COMMENT_H

/**
 * @file axis2_om_comment.h
 * @brief defines axis2_om_comment_t struct, and manipulation functions
 */

#include <axis2_om_node.h>
#include <axis2_om_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_comment;
    struct axis2_om_comment_ops;

/**
 * @defgroup axis2_om_comment OM Comment
 * @ingroup axis2_om 
 * @{
 */


  /** 
    *   @brief OM comment operations struct
    *   Encapsulator struct for operations of axis2_om_comment_t
    */
 AXIS2_DECLARE_DATA  typedef struct axis2_om_comment_ops
    {
      /**
        * Free a axis2_comment_t struct
        * @param om_comment pointer to axis2_commnet_t struct to be freed
        * @param env Environment. MUST NOT be NULL.
        * @return satus of the operation. 
        *                AXIS2_SUCCESS on success ,AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *free) (struct axis2_om_comment *om_comment,
                                           axis2_env_t **env);
       /** get the comments data
        * @param om_comment a pointer to axis2_om_comment_t struct
        * @param env environment, MUST NOT be NULL 
        * @returns comment text
        */
               
        axis2_char_t* (AXIS2_CALL *get_value)(struct axis2_om_comment *om_comment,
                                              axis2_env_t **env);
       /**
        * set comment data
        * @param om_comment pointer to axis2_om_comment_t struct
        * @param env environment, MUST NOT be NULL.
        * @param value comment text
        * @returns AXIS2_SUCCESS on success , AXIS2_FAILURE on error
        */
        axis2_status_t (AXIS2_CALL *set_value)(struct axis2_om_comment *om_comment,
                                              axis2_env_t **env,
                                              const axis2_char_t* value);
        /**
         *  serialize function 
         *  @param om_comment pointer to axis2_om_comment_t struct
         *  @param env environment, MUST NOT be NULL.
         *  @param om_output pointer to om_output_t struct
         *  @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *serialize)(struct axis2_om_comment *om_comment,
                                               axis2_env_t **env,
                                               axis2_om_output_t *om_output);
                                                                                            
                                              
    } axis2_om_comment_ops_t;

  /** 
    * \brief OM comment struct
    * Handles the XML comment in OM
    */
    typedef struct axis2_om_comment
    {
        /** operations struct */
        axis2_om_comment_ops_t *ops;

    } axis2_om_comment_t;

  /**
    * Creates a comment struct
    * @param env Environment. MUST NOT be NULL,
    * @param parent This is the parent node of the comment is any, can be NULL.
    * @param value comment text
    * @param node This is an out parameter.cannot be NULL.
    *        Returns the node corresponding to the comment created.
    *        Node type will be set to AXIS2_OM_COMMENT
    * @return a pointer to the newly created comment struct
    */
    AXIS2_DECLARE(axis2_om_comment_t *) 
    axis2_om_comment_create (axis2_env_t **env,
                             axis2_om_node_t *parent,
                             const axis2_char_t *value,
                             axis2_om_node_t **node);


/** free given comment */
#define AXIS2_OM_COMMENT_FREE(om_comment, env) \
        ((om_comment)->ops->free(om_comment, env))
/** get comment text */
#define AXIS2_OM_COMMENT_GET_VALUE(om_comment, env) \
        ((om_comment)->ops->get_value(om_comment, env))
/** set comment text */        
#define AXIS2_OM_COMMENT_SET_VALUE(om_comment, env, value) \
        ((om_comment)->ops->set_value(om_comment, env, value))
/** serialize */
#define AXIS2_OM_COMMENT_SERIALIZE(om_comment, env, om_output) \
        ((om_comment)->ops->serialize(om_comment, env, om_output))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_COMMENT_H */
