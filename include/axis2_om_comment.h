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
extern "C" {
#endif

/**
 * @defgroup axis2_om_comment OM comment
 * @ingroup axis2_om 
 * @{
 */

/** @cond */
struct axis2_om_comment;
struct axis2_om_comment_ops;
/** @endcond */
    
/** @struct axis2_om_comment_ops
 *   @brief OM comment operations struct
 *   Encapsulator struct for operations of axis2_om_comment_t
 */
  
typedef struct axis2_om_comment_ops
{
  /**
    * Free a axis2_comment struct
    * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
    * @param comment pointer to the axis2_commnet 
    * @returns status code
    */
    axis2_status_t (*axis2_om_comment_ops_free)(axis2_environment_t *environment, struct axis2_om_comment *comment);

} axis2_om_comment_ops_t;

/** \struct axis2_om_comment
    \brief OM comment struct

    Handles the XML comment in OM
*/
typedef struct axis2_om_comment
{
    /** operations struct */
    axis2_om_comment_ops_t *ops;
	/** value */
	char *value;
} axis2_om_comment_t;

/**
 * Create a comment struct and stores in in a node struct and returns a pointer
 * to the axis2_om_comment_t struct
 * the data_element field of node struct points to the acctual axis2_comment struct
 * The element type of axis2_om_node_t struct will be of type AXIS2_OM_COMMENT
 * @param value value of the comment
 * @param comment_node This is an out parameter, stores the acctual om_comment in
 *        this node struct pointer
 * @return pointer to a node_t struct containing the comment struct
 */
axis2_om_comment_t *axis2_om_comment_create(axis2_environment_t *environment, const axis2_char_t *value, axis2_om_node_t **comment_node);

#define axis2_om_comment_free(environment, comment) ((comment)->ops->axis2_om_comment_ops_free(environment, comment))
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_OM_COMMENT_H */
