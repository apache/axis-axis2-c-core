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

#include <axis2c_om_comment.h>
#include <string.h>
#include <axis2c_errno.h>

axis2c_node_t *axis2c_create_om_comment(const char *value)
{
    axis2c_node_t *node = NULL;
    axis2c_om_comment_t *comment = NULL;

    node = axis2c_create_node();
    if (!node)
    {
	fprintf(stderr, "%d Error", AXIS2C_ERROR_OM_MEMORY_ALLOCATION);
	return NULL;
    }
    comment = (axis2c_om_comment_t *) malloc(sizeof(axis2c_om_comment_t));
    if (!comment)
    {
	free(node);
	fprintf(stderr, "%d Error", AXIS2C_ERROR_OM_MEMORY_ALLOCATION);
	return NULL;

    }
    comment->value = strdup(value);
    if (!comment->value)
    {
	fprintf(stderr, "%d Error", AXIS2C_ERROR_OM_MEMORY_ALLOCATION);
    }

    node->data_element = comment;
    node->element_type = OM_COMMENT;
    return node;
}


void axis2c_om_comment_free(axis2c_om_comment_t * comment)
{
    if (comment)
    {
	if (comment->value)
	    free(comment->value);
	free(comment);
    }
}

/*
*	returns a duplicated string as value
*
*/

char *axis2c_om_comment_get_value(axis2c_node_t * comment_node)
{
    if (!comment_node || comment_node->element_type != OM_COMMENT)
    {
	return NULL;
    }
    return strdup(((axis2c_om_comment_t *) (comment_node->data_element))->value);
}

void axis2c_om_comment_set_value(axis2c_node_t * comment_node, const char *value)
{
    axis2c_om_comment_t *comm = NULL;

    if (!comment_node || comment_node->element_type != OM_COMMENT)
    {
	return;
    }
    comm = ((axis2c_om_comment_t *) (comment_node->data_element));

    if (comm->value)
    {
	free(comm->value);
    }
    comm->value = strdup(value);
}

