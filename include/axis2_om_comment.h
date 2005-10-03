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

#include <axis2_node.h>

typedef struct axis2_om_comment_t
{
	char *value;
}axis2_om_comment_t;

/**
 * Create a comment struct and stores in in a node struct and returns a pointer
 * to the axis2_node_t struct
 * the data_element field of node struct points to the acctual axis2_comment struct
 * The element type of axis2_node_t struct will be of type AXIS2_OM_COMMENT
 * @return pointer to a node_t struct containing the comment struct
 */
axis2_node_t *axis2_om_comment_create(const char *value);

/**
 *	free a axis2_comment struct
 * @param comment pointer to the axis2_commnet 
 *
 */

void axis2_om_comment_free(axis2_om_comment_t *comment);

/**
 *  to get the value of a existing comment node	
 * @param comment_node pointer to comment node
 */

char *axis2_om_comment_get_value(axis2_node_t *comment_node);

/**
 *  to set the value of a existing comment node	
 * @param comment_node pointer to comment node
 */


void axis2_om_comment_set_value(axis2_node_t *comment_node,const char *value);


#endif // AXIS2_OM_COMMENT_H
