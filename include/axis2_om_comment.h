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
        * Free a axis2_comment struct
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param comment pointer to axis2_commnet struct to be freed
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
         axis2_status_t (AXIS2_CALL *axis2_om_comment_ops_free) (axis2_env_t *
                                                     environment,
                                                     struct axis2_om_comment *
                                                     comment);

    } axis2_om_comment_ops_t;

  /** 
    * \brief OM comment struct
    * Handles the XML comment in OM
    */
    typedef struct axis2_om_comment
    {
        /** operations struct */
        axis2_om_comment_ops_t *ops;
        /** comment text */
        axis2_char_t *value;
    } axis2_om_comment_t;

  /**
    * Creates a comment struct
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param value comment text
    * @param comment_node This is an out parameter. Mandatory, cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIS2_OM_COMMENT
    * @return a pointer to the newly created comment struct
    */
    AXIS2_DECLARE(axis2_om_comment_t *) axis2_om_comment_create (axis2_env_t *
                                                 environment,
                                                 const axis2_char_t * value,
                                                 axis2_om_node_t **
                                                 comment_node);

/** free given comment */
#define axis2_om_comment_free(environment, comment) ((comment)->ops->axis2_om_comment_ops_free(environment, comment))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_COMMENT_H */
