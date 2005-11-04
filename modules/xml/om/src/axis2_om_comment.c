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

#include <axis2_om_comment.h>
#include <string.h>

axis2_status_t AXIS2_CALL axis2_om_comment_impl_free (axis2_env_t * environment,
                                           axis2_om_comment_t * comment);

AXIS2_DECLARE(axis2_om_comment_t*)
 axis2_om_comment_create (axis2_env_t * environment,
                         const axis2_char_t * value, axis2_om_node_t ** node)
{
    axis2_om_comment_t *comment = NULL;
    *node = NULL;
    
    if (!node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAM;
        return NULL;
    }

    *node = axis2_om_node_create (environment);
    if (!*node)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    comment =
        (axis2_om_comment_t *) axis2_malloc (environment->allocator,
                                             sizeof (axis2_om_comment_t));
    if (!comment)
    {
        axis2_om_node_free (environment, *node);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    comment->value = NULL;

    if (value)
    {
        comment->value = axis2_strdup (environment->string, value);
        if (!comment->value)
        {
            axis2_om_node_free (environment, *node);
            axis2_free (environment->allocator, comment);
            environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
            return NULL;
        }
    }

    (*node)->data_element = comment;
    (*node)->node_type = AXIS2_OM_COMMENT;

    /* operations */
    comment->ops = NULL;
    comment->ops =
        (axis2_om_comment_ops_t *) axis2_malloc (environment->allocator,
                                                 sizeof
                                                 (axis2_om_comment_ops_t));
    if (!comment->ops)
    {
        axis2_om_node_free (environment, *node);
        axis2_free (environment->allocator, comment);
        axis2_free (environment->allocator, comment->value);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    comment->ops->axis2_om_comment_ops_free = axis2_om_comment_impl_free;

    return comment;
}


axis2_status_t AXIS2_CALL
axis2_om_comment_impl_free (axis2_env_t * environment,
                            axis2_om_comment_t * comment)
{
    if (comment)
    {
        if (comment->value)
        {
            axis2_free (environment->allocator, comment->value);
        }
        axis2_free (environment->allocator, comment);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
