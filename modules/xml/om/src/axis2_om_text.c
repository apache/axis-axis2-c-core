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

#include <axis2_om_text.h>
#include <axis2_om_output.h>
#include <axis2_string.h>

/* operations */
axis2_status_t AXIS2_CALL
axis2_om_text_free (axis2_om_text_t * om_text,
                    axis2_env_t **env);
                    
                                       
axis2_status_t AXIS2_CALL 
axis2_om_text_serialize (axis2_om_text_t * om_text,
                         axis2_env_t **env,
                         axis2_om_output_t * om_output);

axis2_char_t* AXIS2_CALL
axis2_om_text_get_value(axis2_om_text_t *om_text,
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_text_set_value(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *value);                                                 
                                             
                                             
/********************* axis2_om_text_impl_struct ***************/

typedef struct axis2_om_text_impl_t
{
    axis2_om_text_t om_text;
    /** Text value */
    axis2_char_t *value;
    /** The following fields are for MTOM
        TODO: Implement MTOM support */
    axis2_char_t *mime_type;
    axis2_bool_t optimize;
    axis2_char_t *localname;
    axis2_bool_t is_binary;
    axis2_char_t *content_id;
}axis2_om_text_impl_t;

/*********************** Macro ***********************************/

#define AXIS2_INTF_TO_IMPL(text) ((axis2_om_text_impl_t*)text)

/*****************************************************************/


AXIS2_DECLARE(axis2_om_text_t*)
axis2_om_text_create (axis2_env_t **env,
                      axis2_om_node_t * parent,
                      const axis2_char_t * value,
                      axis2_om_node_t **node)
{

    axis2_om_text_impl_t *om_text = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, node, NULL);

    *node = axis2_om_node_create (env);

    if (!(*node))
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    
    om_text = (axis2_om_text_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                          sizeof (axis2_om_text_impl_t));

    if (!om_text)
    {
        AXIS2_FREE ((*env)->allocator, *node);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }

    om_text->value = NULL;
    if (value)
        om_text->value = (axis2_char_t *) AXIS2_STRDUP(value,env);

    /* om_text->attribute = NULL; */
    om_text->content_id = NULL;
    om_text->mime_type = NULL;

    AXIS2_OM_NODE_SET_DATA_ELEMENT((*node), env, om_text);
    AXIS2_OM_NODE_SET_NODE_TYPE((*node), env, AXIS2_OM_TEXT);
    AXIS2_OM_NODE_SET_BUILD_STATUS((*node), env, AXIS2_FALSE);

    if (parent && AXIS2_OM_NODE_GET_NODE_TYPE(parent, env) == AXIS2_OM_ELEMENT)
    {
        AXIS2_OM_NODE_SET_PARENT((*node), env, parent);
        AXIS2_OM_NODE_ADD_CHILD ( *node, env, parent);
    }

    /* operations */
    om_text->om_text.ops = NULL;
    om_text->om_text.ops = (axis2_om_text_ops_t *) AXIS2_MALLOC((*env)->allocator,
                                              sizeof (axis2_om_text_ops_t));

    if (!om_text->om_text.ops)
    {
        AXIS2_FREE ((*env)->allocator, om_text->value);
        AXIS2_FREE ((*env)->allocator, om_text);
        AXIS2_FREE ((*env)->allocator, *node);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }

    om_text->om_text.ops->free = axis2_om_text_free;
    om_text->om_text.ops->serialize = axis2_om_text_serialize;
    om_text->om_text.ops->set_value = axis2_om_text_set_value;
    om_text->om_text.ops->get_value = axis2_om_text_get_value;

    return &(om_text->om_text);
}


axis2_status_t AXIS2_CALL
axis2_om_text_free (axis2_om_text_t * om_text,
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_text, env, AXIS2_FAILURE);

    if (AXIS2_INTF_TO_IMPL(om_text)->value)
        AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_text)->value);

    if (om_text->ops)
        AXIS2_FREE ((*env)->allocator, om_text->ops);

    if (om_text)
        AXIS2_FREE ((*env)->allocator, om_text);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_text_serialize (axis2_om_text_t *om_text,
                         axis2_env_t **env,
                         axis2_om_output_t * om_output)
{
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(om_text, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);
    
    if (AXIS2_INTF_TO_IMPL(om_text)->value)
        status = axis2_om_output_write (om_output, env,
                                        AXIS2_OM_TEXT, 1,
                                        AXIS2_INTF_TO_IMPL(om_text)->value);
    return status;
}

axis2_char_t* AXIS2_CALL
axis2_om_text_get_value(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_text, env,NULL);
    return AXIS2_INTF_TO_IMPL(om_text)->value;
}                        

axis2_status_t AXIS2_CALL
axis2_om_text_set_value(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(om_text, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_text, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_text)->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
    return AXIS2_SUCCESS;
}                       
                        
                        
                        