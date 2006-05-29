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
#include "axis2_om_node_internal.h"
#include <axis2_xml_writer.h>
#include <axis2_om_output.h>
#include <axis2_om_attribute.h>
#include <axis2_om_namespace.h>

/* ops */
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
/*Following are for MTOM*/                                             
axis2_char_t* AXIS2_CALL
axis2_om_text_get_mime_type(axis2_om_text_t *om_text,
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_text_set_mime_type(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *mime_type);


axis2_status_t AXIS2_CALL
axis2_om_text_set_optimize(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_bool_t optimize);
                        
axis2_bool_t AXIS2_CALL
axis2_om_text_get_optimize(axis2_om_text_t *om_text,
                        axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_om_text_get_is_binary(axis2_om_text_t *om_text,
                        axis2_env_t **env);
                        
axis2_status_t AXIS2_CALL
axis2_om_text_set_is_binary(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_bool_t is_binary);
                        
axis2_char_t* AXIS2_CALL
axis2_om_text_get_localname(axis2_om_text_t *om_text,
                        axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_om_text_get_content_id(axis2_om_text_t *om_text,
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_text_set_content_id(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *content_id);                        

axis2_status_t AXIS2_CALL
axis2_om_text_serialize_start_part(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        axis2_om_output_t *om_output_t);

axis2_status_t AXIS2_CALL
axis2_om_text_serialize_attribute(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        axis2_om_output_t *om_output, 
                        axis2_om_attribute_t *om_attribute);
                        
axis2_status_t AXIS2_CALL
axis2_om_text_serialize_namespace(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_om_namespace_t *om_namespace, 
                        axis2_om_output_t *om_output);

axis2_char_t* AXIS2_CALL
axis2_om_text_get_text(axis2_om_text_t *om_text,
                        axis2_env_t **env);

axis2_data_handler_t *AXIS2_CALL
axis2_om_text_get_data_handler(axis2_om_text_t *om_text,
                        axis2_env_t **env);

/********************* axis2_om_text_impl_struct ***************/

typedef struct axis2_om_text_impl_t
{
    axis2_om_text_t om_text;
    /** Text value */
    axis2_char_t *value;
    /** The following fields are for MTOM */
    axis2_char_t *mime_type;
    axis2_bool_t optimize;
    axis2_char_t *localname;
    axis2_bool_t is_binary;
    axis2_char_t *content_id;
    axis2_om_attribute_t *om_attribute;    
    axis2_om_namespace_t *ns;
    axis2_data_handler_t *data_handler;
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
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    om_text = (axis2_om_text_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                          sizeof (axis2_om_text_impl_t));
    if (!om_text)
    {
        AXIS2_FREE ((*env)->allocator, *node);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    om_text->mime_type = NULL;
    om_text->optimize = AXIS2_FALSE;
    om_text->localname = "Include";
    om_text->is_binary = AXIS2_FALSE;
    om_text->content_id = NULL;
    om_text->om_attribute = NULL;
    om_text->value = NULL;
    om_text->ns = NULL;
    om_text->data_handler = NULL;
    om_text->mime_type = NULL;

    om_text->ns = axis2_om_namespace_create(env, "http://www.w3.org/2004/08/xop/include", "xop");

    if (value)
        om_text->value = (axis2_char_t *) AXIS2_STRDUP(value,env);

    axis2_om_node_set_data_element((*node), env, om_text);
    axis2_om_node_set_node_type((*node), env, AXIS2_OM_TEXT);
    axis2_om_node_set_complete((*node), env, AXIS2_FALSE);

    if (parent && AXIS2_OM_NODE_GET_NODE_TYPE(parent, env) == AXIS2_OM_ELEMENT)
    {
        AXIS2_OM_NODE_ADD_CHILD (parent, env, (*node));
    }

    /* ops */
    om_text->om_text.ops = NULL;
    om_text->om_text.ops = (axis2_om_text_ops_t *) AXIS2_MALLOC((*env)->allocator,
                                              sizeof (axis2_om_text_ops_t));

    if (!om_text->om_text.ops)
    {
        AXIS2_FREE ((*env)->allocator, om_text->value);
        AXIS2_FREE ((*env)->allocator, om_text);
        AXIS2_FREE ((*env)->allocator, *node);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    om_text->om_text.ops->free = axis2_om_text_free;
    om_text->om_text.ops->serialize = axis2_om_text_serialize;
    om_text->om_text.ops->set_value = axis2_om_text_set_value;
    om_text->om_text.ops->get_value = axis2_om_text_get_value;
    om_text->om_text.ops->get_data_handler = axis2_om_text_get_data_handler;
    om_text->om_text.ops->get_content_id = axis2_om_text_get_content_id;
    om_text->om_text.ops->set_optimize = axis2_om_text_set_optimize;
    om_text->om_text.ops->set_is_binary = axis2_om_text_set_is_binary;
    
    return &(om_text->om_text);
}

AXIS2_DECLARE(axis2_om_text_t*)
axis2_om_text_create_with_data_handler (axis2_env_t **env,
                      axis2_om_node_t * parent,
                      axis2_data_handler_t* data_handler,
                      axis2_om_node_t **node)
{

    axis2_om_text_impl_t *om_text_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, data_handler, NULL);

    om_text_impl = (axis2_om_text_impl_t*)axis2_om_text_create(env, parent, NULL, node);
    if (!om_text_impl)
        return NULL;
    om_text_impl->optimize = AXIS2_TRUE;
    om_text_impl->is_binary = AXIS2_TRUE;
    om_text_impl->data_handler = data_handler;
    return &(om_text_impl->om_text);
}

axis2_status_t AXIS2_CALL
axis2_om_text_free (axis2_om_text_t * om_text,
                    axis2_env_t **env)
{
    axis2_om_text_impl_t *text_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    text_impl = AXIS2_INTF_TO_IMPL(om_text);
    if(NULL != text_impl->value)
    {
        AXIS2_FREE ((*env)->allocator, text_impl->value);
        text_impl->value = NULL;
    }

    if (NULL != text_impl->ns)
    {
        AXIS2_OM_NAMESPACE_FREE(text_impl->ns, env);
        text_impl->ns = NULL;
    }
    
    if(text_impl->content_id)
    {
        AXIS2_FREE((*env)->allocator, text_impl->content_id);
        text_impl->content_id = NULL;
    }
                            
    if(NULL != om_text->ops)
    {
        AXIS2_FREE ((*env)->allocator, om_text->ops);
        om_text->ops = NULL;
    }

    AXIS2_FREE ((*env)->allocator, text_impl);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_text_serialize (axis2_om_text_t *om_text,
                         axis2_env_t **env,
                         axis2_om_output_t *om_output)
{
    int status = AXIS2_SUCCESS;
    axis2_om_text_impl_t *om_text_impl = NULL;
    axis2_char_t *attribute_value = NULL;
    axis2_char_t *text = NULL;
    axis2_xml_writer_t *om_output_xml_writer = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);
    om_text_impl = AXIS2_INTF_TO_IMPL(om_text);

    if(!axis2_om_text_get_is_binary(om_text, env))
    {
        if (AXIS2_INTF_TO_IMPL(om_text)->value)
            status = axis2_om_output_write (om_output, env,
                                        AXIS2_OM_TEXT, 1,
                                        AXIS2_INTF_TO_IMPL(om_text)->value);
    }
    else
    {
        om_output_xml_writer = AXIS2_OM_OUTPUT_GET_XML_WRITER(om_output, env);
        if(om_text_impl->optimize)
        {
            if(axis2_om_text_get_content_id(om_text, env) == NULL)
            {
                axis2_char_t *content_id = AXIS2_OM_OUTPUT_GET_NEXT_CONTENT_ID(om_output, env);
                if (content_id)
                    om_text_impl->content_id = AXIS2_STRDUP(content_id, env);
            }    
            attribute_value = AXIS2_STRDUP("cid:", env);
            attribute_value = AXIS2_STRACAT(attribute_value, om_text_impl->content_id, env);
            /*send binary as MTOM optimised*/
            om_text_impl->om_attribute = axis2_om_attribute_create(env, "href", attribute_value, NULL);
            axis2_om_text_serialize_start_part(om_text, env, om_output);
            
            axis2_om_output_write_optimized(om_output, env, om_text);            
            
            axis2_om_output_write (om_output, env, AXIS2_OM_ELEMENT, 0);
        }
        else
        {
            text = axis2_om_text_get_text(om_text, env);
            AXIS2_XML_WRITER_WRITE_CHARACTERS(om_output_xml_writer, env, text);
        }
    }
    return status;    
}

axis2_char_t* AXIS2_CALL
axis2_om_text_get_value(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(om_text)->value;
}                        

axis2_status_t AXIS2_CALL
axis2_om_text_set_value(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *value)
{
    axis2_om_text_impl_t *text_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_text, AXIS2_FAILURE);
    
    text_impl = AXIS2_INTF_TO_IMPL(om_text);
    if(NULL != text_impl->value)
    {
        AXIS2_FREE((*env)->allocator, text_impl->value);
        text_impl->value = NULL;
    }
    
    text_impl->value = (axis2_char_t*)AXIS2_STRDUP(value, env);
    if(!text_impl->value)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY,
            AXIS2_FAILURE);
            
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/*Following has been implemented for the MTOM support*/

axis2_char_t* AXIS2_CALL
axis2_om_text_get_mime_type(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(om_text)->mime_type;
}                        

axis2_status_t AXIS2_CALL
axis2_om_text_set_mime_type(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *mime_type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_text, AXIS2_FAILURE);
    if(AXIS2_INTF_TO_IMPL(om_text)->mime_type)
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(om_text)->mime_type);
    AXIS2_INTF_TO_IMPL(om_text)->mime_type = (axis2_char_t*)AXIS2_STRDUP(mime_type,env);
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
axis2_om_text_get_optimize(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_text)->optimize;
}                        

axis2_status_t AXIS2_CALL
axis2_om_text_set_optimize(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_bool_t optimize)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_text, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_text)->optimize = optimize;
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
axis2_om_text_get_is_binary(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_text)->is_binary;
}                        

axis2_status_t AXIS2_CALL
axis2_om_text_set_is_binary(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_bool_t is_binary)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_text, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_text)->is_binary = is_binary;
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
axis2_om_text_get_localname(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(om_text)->localname;
}                        

axis2_char_t* AXIS2_CALL
axis2_om_text_get_content_id(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(om_text)->content_id;
}                        

axis2_status_t AXIS2_CALL
axis2_om_text_set_content_id(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_char_t *content_id)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_text, AXIS2_FAILURE);
    if(AXIS2_INTF_TO_IMPL(om_text)->content_id)
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(om_text)->content_id);
    AXIS2_INTF_TO_IMPL(om_text)->content_id = (axis2_char_t*)AXIS2_STRDUP(content_id, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_text_serialize_start_part(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        axis2_om_output_t *om_output)
{
    axis2_om_text_impl_t *om_text_impl = NULL;
    axis2_char_t *namespace_uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *local_name = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_text_impl = AXIS2_INTF_TO_IMPL(om_text);
    local_name = axis2_om_text_get_localname(om_text, env);    
    
    if (om_text_impl->ns != NULL)
    {
        namespace_uri = AXIS2_OM_NAMESPACE_GET_URI(om_text_impl->ns, env);
        if (namespace_uri != NULL) 
        {
            prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(om_text_impl->ns, env);

            if(prefix != NULL)
            {
                axis2_om_output_write (om_output, env, AXIS2_OM_ELEMENT, 3,
                            local_name, namespace_uri, prefix);
            }
            else
            {
                axis2_om_output_write (om_output, env, AXIS2_OM_ELEMENT, 2,
                            local_name, namespace_uri);
            }
        }
        else
        {
            axis2_om_output_write (om_output, env, AXIS2_OM_ELEMENT, 1,
                            local_name);
        }            
    }
    else
    {
        axis2_om_output_write (om_output, env, AXIS2_OM_TEXT, 1,
                            local_name);
    }
    if (om_text_impl->om_attribute)
        AXIS2_OM_ATTRIBUTE_SERIALIZE(om_text_impl->om_attribute, env, om_output);
    if (om_text_impl->ns)
        AXIS2_OM_NAMESPACE_SERIALIZE(om_text_impl->ns, env, om_output);
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_text_serialize_attribute(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        axis2_om_output_t *om_output, 
                        axis2_om_attribute_t *om_attribute)
{
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_namespace_t *om_namespace = NULL;
    
    axis2_om_text_impl_t *om_text_impl = NULL;
    axis2_char_t *namespace_uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *attribute_local_name = NULL;
    axis2_char_t *attribute_value = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
				AXIS2_XML_PARSER_TYPE_BUFFER);
    om_namespace = axis2_om_namespace_create(env, "" , "");
    om_text_impl = AXIS2_INTF_TO_IMPL(om_text);

    namespace_uri = AXIS2_OM_NAMESPACE_GET_URI(om_text_impl->ns, env);
    attribute_local_name = AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(om_attribute, env);
    

    if (om_namespace != NULL) 
    {
        prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(om_text_impl->ns, env);    
        attribute_value = AXIS2_OM_ATTRIBUTE_GET_VALUE(om_attribute, env);
        if (prefix != NULL) 
        {
            AXIS2_XML_WRITER_WRITE_ATTRIBUTE(xml_writer, env, attribute_local_name, attribute_value);
        } else {
            AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE(xml_writer, env, attribute_local_name, attribute_value, namespace_uri);            
        }
    } else {
        AXIS2_XML_WRITER_WRITE_ATTRIBUTE(xml_writer, env, attribute_local_name, attribute_value);
    }
    AXIS2_OM_NAMESPACE_FREE(om_namespace, env);
    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL
axis2_om_text_serialize_namespace(axis2_om_text_t *om_text,
                        axis2_env_t **env,
                        const axis2_om_namespace_t *om_namespace, 
                        axis2_om_output_t *om_output)
{
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_text_impl_t *om_text_impl = NULL;
    axis2_char_t *namespace_uri = NULL;
    axis2_char_t *namespace_prefix = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
					AXIS2_XML_PARSER_TYPE_BUFFER);
    om_namespace = axis2_om_namespace_create(env, "" , "");
    om_text_impl = AXIS2_INTF_TO_IMPL(om_text);

    if (om_namespace != NULL) 
    {
        namespace_uri = AXIS2_OM_NAMESPACE_GET_URI(om_text_impl->ns, env);
        namespace_prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(om_text_impl->ns, env);    
        AXIS2_XML_WRITER_WRITE_NAMESPACE(xml_writer, env, namespace_prefix, namespace_uri);
        AXIS2_XML_WRITER_SET_PREFIX(xml_writer, env, namespace_prefix, namespace_uri);  
    }
    return AXIS2_SUCCESS;    
}

axis2_char_t* AXIS2_CALL
axis2_om_text_get_text(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    axis2_om_text_impl_t *om_text_impl = NULL;
    AXIS2_ENV_CHECK(env,NULL);

    om_text_impl = AXIS2_INTF_TO_IMPL(om_text);
    if(om_text_impl->value != NULL)
    {    
        return om_text_impl->value;
    }
    else
    {
        axis2_char_t *data_handler_stream = NULL;
        int data_handler_stream_size = 0;
        if (om_text_impl->data_handler)
        {
            int encoded_len = 0;
            axis2_char_t *encoded_str = NULL;
            AXIS2_DATA_HANDLER_READ_FROM(om_text_impl->data_handler, env,
                        &data_handler_stream, &data_handler_stream_size);
            if (data_handler_stream)
            {
                encoded_len = axis2_base64_encode_len(data_handler_stream_size);
                encoded_str = AXIS2_MALLOC((*env)->allocator, encoded_len);
                if (encoded_str)
                {
                    encoded_len = axis2_base64_encode(encoded_str, 
                        data_handler_stream, data_handler_stream_size);
                    encoded_str[encoded_len] = '\0';
                    return encoded_str;
                }                
            }
        }
    }
    return om_text_impl->value;    
}

axis2_data_handler_t *AXIS2_CALL
axis2_om_text_get_data_handler(axis2_om_text_t *om_text,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(om_text)->data_handler;
}
