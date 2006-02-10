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

axis2_status_t AXIS2_CALL
axis2_om_comment_serialize(axis2_om_comment_t *om_comment,
                           axis2_env_t **env,
                           axis2_om_output_t *om_output);
                                                                             
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
                        axis2_om_node_t *parent,
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
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    comment = (axis2_om_comment_impl_t *) AXIS2_MALLOC((*env)->allocator,
                                             sizeof (axis2_om_comment_impl_t));
    if (!comment)
    {
        AXIS2_FREE ((*env)->allocator, (*node));
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    comment->value = NULL;

    if (value)
    {
        comment->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
        if (!comment->value)
        {
            AXIS2_FREE ((*env)->allocator,comment);
            AXIS2_FREE ((*env)->allocator, (*node));
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }

    AXIS2_OM_NODE_SET_DATA_ELEMENT((*node), env, comment);
    AXIS2_OM_NODE_SET_NODE_TYPE((*node), env, AXIS2_OM_COMMENT);
    
    if (parent)
    {
        AXIS2_OM_NODE_SET_PARENT((*node), env, parent);
        AXIS2_OM_NODE_ADD_CHILD(parent, env, (*node)); 
    }

    /* ops */
    comment->om_comment.ops = NULL;
    comment->om_comment.ops = (axis2_om_comment_ops_t *)AXIS2_MALLOC(
                              (*env)->allocator,sizeof(axis2_om_comment_ops_t));
    if (!comment->om_comment.ops)
    {
        AXIS2_FREE ((*env)->allocator, comment);
        AXIS2_FREE ((*env)->allocator, comment->value);
        AXIS2_FREE ((*env)->allocator,*node);
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    comment->om_comment.ops->free = axis2_om_comment_free;
    comment->om_comment.ops->set_value = axis2_om_comment_set_value;
    comment->om_comment.ops->get_value = axis2_om_comment_get_value;
    comment->om_comment.ops->serialize = axis2_om_comment_serialize;
    return &(comment->om_comment);
}


axis2_status_t AXIS2_CALL
axis2_om_comment_free (axis2_om_comment_t *om_comment,
                       axis2_env_t **env)
{
    axis2_om_comment_impl_t *comment_impl = NULL;
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    comment_impl = AXIS2_INTF_TO_IMPL(om_comment);
    
    if (comment_impl->value)
    {
        AXIS2_FREE ((*env)->allocator, comment_impl->value);
    }
    AXIS2_FREE((*env)->allocator, om_comment->ops);
    AXIS2_FREE((*env)->allocator,comment_impl);
    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
axis2_om_comment_get_value(axis2_om_comment_t *om_comment,
                           axis2_env_t **env)
{
    AXIS2_ENV_CHECK( env, NULL);
    return AXIS2_INTF_TO_IMPL(om_comment)->value;
}   

                        
axis2_status_t AXIS2_CALL
axis2_om_comment_set_value(axis2_om_comment_t *om_comment,
                           axis2_env_t **env,
                           const axis2_char_t *value)
{
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_comment)->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_comment_serialize(axis2_om_comment_t *om_comment,
                           axis2_env_t **env,
                           axis2_om_output_t *om_output)
{
    axis2_om_comment_impl_t *comment_impl;
    
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);    
    
    comment_impl = AXIS2_INTF_TO_IMPL(om_comment);
    if(comment_impl->value)
        return  axis2_om_output_write(om_output, env,
                    AXIS2_OM_COMMENT , 1 , comment_impl->value);
    return AXIS2_FAILURE;
}
