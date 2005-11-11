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
#include <axis2_string.h>

axis2_status_t AXIS2_CALL
axis2_om_comment_free (axis2_om_comment_t *om_comment,
                       axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_om_comment_get_value(axis2_om_comment_t *om_comment,
                           axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_om_comment_set_value(axis2_om_comment_t *om_comment,
                           axis2_env_t **env,
                           const axis2_char_t *value);
                                                                             
/***************************** axis2_om_comment_struct ******************/

typedef struct axis2_om_comment_impl_t
{
    axis2_om_comment_t om_comment;
    /** comment text */
    axis2_char_t *value;

}axis2_om_comment_impl_t;
/***************************** Macro **********************************/

#define AXIS2_INTF_TO_IMPL(om_comment) ((axis2_om_comment_impl_t*)om_comment)


/*************************** End Macro *******************************/

AXIS2_DECLARE(axis2_om_comment_t*)
axis2_om_comment_create(axis2_env_t **env,
                        const axis2_char_t * value,
                        axis2_om_node_t ** node)
{
    axis2_om_comment_impl_t *comment = NULL;
    *node = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, value, NULL);
    AXIS2_PARAM_CHECK((*env)->error, node, NULL);
    
    *node = axis2_om_node_create (env);
    if (!*node)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    comment = (axis2_om_comment_impl_t *) AXIS2_MALLOC((*env)->allocator,
                                             sizeof (axis2_om_comment_impl_t));
    if (!comment)
    {
        AXIS2_FREE ((*env)->allocator, (*node));
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }

    comment->value = NULL;

    if (value)
    {
        comment->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
        if (!comment->value)
        {
            AXIS2_FREE ((*env)->allocator,comment);
            AXIS2_FREE ((*env)->allocator, (*node));
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        }
    }

    AXIS2_OM_NODE_SET_DATA_ELEMENT((*node), env, comment);
    AXIS2_OM_NODE_SET_NODE_TYPE((*node), env, AXIS2_OM_COMMENT);

    /* operations */
    comment->om_comment.ops = NULL;
    comment->om_comment.ops = (axis2_om_comment_ops_t *)AXIS2_MALLOC(
                              (*env)->allocator,sizeof(axis2_om_comment_ops_t));
    if (!comment->om_comment.ops)
    {
        AXIS2_FREE ((*env)->allocator, comment);
        AXIS2_FREE ((*env)->allocator, comment->value);
        AXIS2_FREE ((*env)->allocator,*node);
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_NO_MEMORY, NULL);
    }

    comment->om_comment.ops->free = axis2_om_comment_free;
    comment->om_comment.ops->set_value = axis2_om_comment_set_value;
    comment->om_comment.ops->get_value = axis2_om_comment_get_value;
    return &(comment->om_comment);
}


axis2_status_t AXIS2_CALL
axis2_om_comment_free (axis2_om_comment_t *om_comment,
                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_comment, env, AXIS2_FAILURE);
    if (om_comment)
    {
        if (AXIS2_INTF_TO_IMPL(om_comment)->value)
        {
            AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_comment)->value);
        }
        AXIS2_FREE((*env)->allocator,AXIS2_INTF_TO_IMPL(om_comment));
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_char_t* AXIS2_CALL
axis2_om_comment_get_value(axis2_om_comment_t *om_comment,
                           axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_comment, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_comment)->value;
}   

                        
axis2_status_t AXIS2_CALL
axis2_om_comment_set_value(axis2_om_comment_t *om_comment,
                           axis2_env_t **env,
                           const axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(om_comment, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_comment)->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
    return AXIS2_SUCCESS;
}
