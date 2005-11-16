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

#include <axis2_om_stax_builder.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_string.h>
#include <axis2_pull_parser.h>



const axis2_char_t XMLNS_URI[] = "http://www.w3.org/XML/1998/namespace";
const axis2_char_t XMLNS_PREFIX[] = "xml";

/**************************** function prototypes *****************************/

axis2_om_node_t * AXIS2_CALL 
axis2_om_stax_builder_next (axis2_om_stax_builder_t *builder,
                            axis2_env_t **env);
                                 
axis2_status_t AXIS2_CALL 
axis2_om_stax_builder_discard_current_element (axis2_om_stax_builder_t *builder,
                                               axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_om_stax_builder_free(axis2_om_stax_builder_t *builder,
                           axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL
axis2_om_stax_builder_get_lastnode (axis2_om_stax_builder_t *builder,
                                   axis2_env_t **env);
                                            
axis2_om_document_t* AXIS2_CALL
axis2_om_stax_builder_get_document (axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env);
                                            
axis2_status_t AXIS2_CALL
axis2_om_stax_builder_set_lastnode(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_node_t *om_node);
                                            
axis2_status_t  AXIS2_CALL
axis2_om_stax_builder_set_document(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_document_t *document);                           
                           											
													
/********************************* axis2_om_stax_builder_impl_t struct ********/
typedef struct axis2_om_stax_builder_impl_t
{
    axis2_om_stax_builder_t om_stax_builder;
    /** pull parser instance used by the builder */
    axis2_pull_parser_t *parser;
    /** last node the builder found */
    axis2_om_node_t *lastnode;
    /** document associated with the builder */
    axis2_om_document_t *document;
    /** done building the document? */
    axis2_bool_t done;
    /** parser was accessed? */
    axis2_bool_t parser_accessed;
    /** caching enabled? */
    axis2_bool_t cache;
    
    
    

}axis2_om_stax_builder_impl_t;

/************************************** Macro *********************************/				

#define AXIS2_INTF_TO_IMPL(builder) ((axis2_om_stax_builder_impl_t*)builder)

/******************************************************************************/													
axis2_om_stax_builder_t *AXIS2_CALL 
axis2_om_stax_builder_create (axis2_env_t **env,void* stream)
{
    axis2_om_stax_builder_impl_t *builder = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    builder = (axis2_om_stax_builder_impl_t *)AXIS2_MALLOC (
                (*env)->allocator, sizeof(axis2_om_stax_builder_impl_t));

    if (!builder)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    
   
    builder->cache = AXIS2_TRUE;
    builder->parser_accessed = AXIS2_FALSE;
    builder->done = AXIS2_FALSE;
    builder->lastnode = NULL;
    builder->document = NULL;

    builder->parser = axis2_pull_parser_create(env,stream);
    if(!builder->parser)
    {
        AXIS2_FREE((*env)->allocator, builder);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    
    /* operations */
    builder->om_stax_builder.ops = NULL;
    builder->om_stax_builder.ops = (axis2_om_stax_builder_ops_t *) AXIS2_MALLOC(
                                    (*env)->allocator, sizeof(axis2_om_stax_builder_ops_t));

    if (!builder->om_stax_builder.ops)
    {
        AXIS2_PULL_PARSER_FREE (builder->parser, env);
        AXIS2_FREE ((*env)->allocator, builder);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }

    builder->om_stax_builder.ops->next = axis2_om_stax_builder_next;
    builder->om_stax_builder.ops->discard_current_element = 
        axis2_om_stax_builder_discard_current_element;
        
	builder->om_stax_builder.ops->free = axis2_om_stax_builder_free;
	builder->om_stax_builder.ops->set_document = axis2_om_stax_builder_set_document;
	builder->om_stax_builder.ops->set_lastnode = axis2_om_stax_builder_set_lastnode;
	builder->om_stax_builder.ops->get_lastnode = axis2_om_stax_builder_get_lastnode;
	
    return &(builder->om_stax_builder);
}

axis2_status_t
axis2_om_stax_builder_process_attributes (axis2_om_stax_builder_t *om_builder,
                                          axis2_env_t **env,
                                          axis2_om_node_t *element_node)
{
    int i = 0;
    int attribute_count;
    axis2_om_attribute_t *attribute = NULL;
    axis2_om_namespace_t *ns = NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    
    AXIS2_FUNC_PARAM_CHECK(om_builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element_node, AXIS2_FAILURE);
    builder = AXIS2_INTF_TO_IMPL(om_builder);
    

    attribute_count = AXIS2_PULL_PARSER_GET_ATTRIBUTE_COUNT(
                               builder->parser,env);
    
    for (i=1; i <= attribute_count ; i++)
    {
        
        uri =  AXIS2_PULL_PARSER_GET_ATTRIBUTE_NAMESPACE_BY_NUMBER(
                       builder->parser, env, i);
              
        prefix = AXIS2_PULL_PARSER_GET_ATTRIBUTE_PREFIX_BY_NUMBER(
                        builder->parser, env, i);

        if (uri)
        {
            if (axis2_strcmp (uri, " ") != 0);
            {
                ns = AXIS2_OM_ELEMENT_FIND_NAMESPACE (
                        (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node,env),
                        env , element_node, uri, prefix);
            }
        }

        if (ns == NULL && prefix && uri
            && (axis2_strcmp (prefix, XMLNS_PREFIX) == 0)
            && (axis2_strcmp (uri, XMLNS_URI) == 0))
        {
            ns = axis2_om_namespace_create (env, XMLNS_URI,
                                            XMLNS_PREFIX);
            if (ns)
            {
                status = AXIS2_OM_ELEMENT_DECLARE_NAMESPACE (
                            (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node,env),
                            env, element_node, ns);
            }
            else
            {
                status = AXIS2_FAILURE;
            }

            ns = AXIS2_OM_ELEMENT_FIND_NAMESPACE (
                    (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node,env),
                    env, element_node, uri, prefix);
        }
        attribute = axis2_om_attribute_create (
                            env,
                            AXIS2_PULL_PARSER_GET_ATTRIBUTE_NAME_BY_NUMBER(
                                 builder->parser, env, i),
                            AXIS2_PULL_PARSER_GET_ATTRIBUTE_VALUE_BY_NUMBER(
                                 builder->parser, env, i), ns);
                            
        status = AXIS2_OM_ELEMENT_ADD_ATTRIBUTE (
                        (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node,env),
                        env, attribute);

    }

    return status;

}


axis2_om_node_t *
axis2_om_stax_builder_create_om_text (axis2_om_stax_builder_t * om_stax_builder,
                                      axis2_env_t **env)
{
    axis2_char_t *value = NULL;
    axis2_char_t *temp_value = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
   
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, NULL);
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    if (!builder->lastnode)
        AXIS2_ERROR_SET((*env)->error, 
                         AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL,NULL);

    
    
    temp_value = AXIS2_PULL_PARSER_GET_VALUE (builder->parser, env);

    if (!temp_value)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PULL_PARSER_VALUE_NULL, NULL);
    }
    else
    {
        value = (axis2_char_t*)AXIS2_STRDUP ( temp_value,env);
        AXIS2_FREE((*env)->allocator, temp_value);
    }

    if (AXIS2_OM_NODE_GET_BUILD_STATUS(builder->lastnode, env))
    {
        axis2_om_text_create (env, 
                              AXIS2_OM_NODE_GET_PARENT(builder->lastnode, env),
                              value, &node);

    }
    else
    {
        axis2_om_text_create (env, builder->lastnode, value, &node);
    }

    AXIS2_OM_NODE_SET_BUILD_STATUS(node , env, AXIS2_TRUE);
    builder->lastnode = node;

    return node;
}


axis2_status_t AXIS2_CALL
axis2_om_stax_builder_discard_current_element (axis2_om_stax_builder_t *om_stax_builder,
                                               axis2_env_t **env)
{
    axis2_om_node_t *element = NULL;
    axis2_om_node_t *prev_node = NULL;
    axis2_om_node_t *parent = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, AXIS2_FAILURE);
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    element = builder->lastnode;

    if (AXIS2_OM_NODE_GET_BUILD_STATUS(element, env) || !(builder->cache))
    {
        AXIS2_ERROR_SET((*env)->error,
                AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD, AXIS2_FAILURE);
    }

    builder->cache = AXIS2_FALSE;
    do
    {
        while (AXIS2_PULL_PARSER_NEXT (builder->parser, env)
                != AXIS2_PULL_PARSER_END_ELEMENT);
    }
    while (!(AXIS2_OM_NODE_GET_BUILD_STATUS(element, env)));

    /*All children of this element is pulled now */

    prev_node = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(element, env);
    if (prev_node)
    {
        AXIS2_OM_NODE_FREE_TREE(AXIS2_OM_NODE_GET_NEXT_SIBLING(prev_node, env), env);
        AXIS2_OM_NODE_SET_NEXT_SIBLING(prev_node, env, NULL);        
    }
    else
    {
        parent = AXIS2_OM_NODE_GET_PARENT(element, env);
        AXIS2_OM_NODE_FREE_TREE(AXIS2_OM_NODE_GET_FIRST_CHILD(parent, env), env);
        AXIS2_OM_NODE_SET_FIRST_CHILD(parent, env, NULL);
        builder->lastnode = parent;
    }
    builder->cache = AXIS2_TRUE;

    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_om_stax_builder_process_namespaces (axis2_om_stax_builder_t *om_stax_builder,
                                          axis2_env_t **env,
                                          axis2_om_node_t *node,
                                          int is_soap_element)
{
    axis2_status_t status = AXIS2_SUCCESS;
    int namespace_count = 0;
    axis2_om_namespace_t *om_ns = NULL;
    axis2_char_t *temp_prefix = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    int i=0;
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, AXIS2_FAILURE);
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    namespace_count = AXIS2_PULL_PARSER_GET_NAMESPACE_COUNT (builder->parser, env);

    
    
    for (i = 1; i <= namespace_count; i++)
    {
        om_ns = axis2_om_namespace_create (
                env,
                AXIS2_PULL_PARSER_GET_NAMESPACE_URI_BY_NUMBER(
                builder->parser, env , i),
                AXIS2_PULL_PARSER_GET_NAMESPACE_PREFIX_BY_NUMBER(
                 builder->parser, env, i));
                
        if (om_ns)
        {
            status = AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(
                        (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env),
                        env, node, om_ns);
        }
        else
        {
            /* something went wrong */
            return AXIS2_FAILURE;
        }
    }
    /* set own namespace */
    temp_prefix = AXIS2_PULL_PARSER_GET_PREFIX ( builder->parser, env);

    if (temp_prefix)
    {
        om_ns = AXIS2_OM_ELEMENT_FIND_NAMESPACE (
                    (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env),
                    env, node, NULL, temp_prefix);

        if (om_ns)
        {
            axis2_om_element_t *om_ele = NULL;
            om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
            AXIS2_OM_ELEMENT_SET_NAMESPACE (om_ele, env, om_ns, node);
        }
        else
        {
            AXIS2_ERROR_SET((*env)->error,
             AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE, AXIS2_FAILURE);
        }
    }
    return status;
}

axis2_om_node_t *
axis2_om_stax_builder_create_om_element (axis2_om_stax_builder_t *om_stax_builder,
                                         axis2_env_t **env)
{
    axis2_om_node_t *element_node;
    axis2_char_t *localname = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    axis2_char_t *temp_localname = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, NULL);
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    temp_localname  = AXIS2_PULL_PARSER_GET_NAME (builder->parser, env);

    if (!temp_localname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PULL_PARSER_ELEMENT_NULL, NULL); 
    }
    else
    {
        localname = (axis2_char_t *)AXIS2_STRDUP (temp_localname,env);
        AXIS2_FREE ((*env)->allocator, temp_localname); 
        /*we should be using guththila free here */
    }

    if (!(builder->lastnode))
    {
        axis2_om_element_create (env , NULL, localname, NULL, &element_node);
        
        /*
        if (AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(builder->document, env))
             AXIS2_OM_NODE_FREE_TREE(
                AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(builder->document, env),env);*/
                
        AXIS2_OM_DOCUMENT_SET_ROOT_ELEMENT(builder->document, env, element_node);
        
    }
    else if (AXIS2_OM_NODE_GET_BUILD_STATUS(builder->lastnode, env))
    {
        axis2_om_element_create (env, AXIS2_OM_NODE_GET_PARENT(builder->lastnode, env),
                                 localname, NULL, &element_node);
             
        AXIS2_OM_NODE_SET_NEXT_SIBLING(builder->lastnode, env, element_node);                                 
        AXIS2_OM_NODE_SET_PREVIOUS_SIBLING(builder->lastnode , env, builder->lastnode);
    }
    else
    {
        axis2_om_element_create ( env, builder->lastnode,
                                 localname, NULL, &element_node);
        
        AXIS2_OM_NODE_SET_FIRST_CHILD(builder->lastnode , env, element_node);                     
        AXIS2_OM_NODE_SET_PARENT(element_node , env, builder->lastnode);
    }
    
    axis2_om_stax_builder_process_attributes ( om_stax_builder, env, element_node);
    axis2_om_stax_builder_process_namespaces ( om_stax_builder, env, element_node, 0);
    
    builder->lastnode = element_node;
	
	/*axis2_free(environment->allocator, localname); */

    return element_node;
}


axis2_om_node_t *
axis2_om_stax_builder_create_om_comment (axis2_om_stax_builder_t *builder,
                                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    /* guththila does not support comments */
    return NULL;
}


axis2_om_node_t * 
axis2_om_stax_builder_create_om_doctype (axis2_om_stax_builder_t * builder,
                                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    /*  guththila does not support doctype */
    return NULL;
}

 
axis2_om_node_t * 
axis2_om_stax_builder_create_om_processing_instruction (axis2_om_stax_builder_t *builder,
                                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    /* guththila does not support processing instrtuctions */
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_stax_builder_end_element (axis2_om_stax_builder_t *om_stax_builder,
                                   axis2_env_t **env)
{
    axis2_om_node_t *parent;
    axis2_om_stax_builder_impl_t *builder = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, AXIS2_FAILURE );
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    if (builder->lastnode)
    {
        if (AXIS2_OM_NODE_GET_BUILD_STATUS((builder->lastnode), env))
        {
            parent = AXIS2_OM_NODE_GET_PARENT((builder->lastnode), env);
            if (parent)
            {
                AXIS2_OM_NODE_SET_BUILD_STATUS(parent, env, AXIS2_TRUE);
                builder->lastnode= parent;
            }
        }
        else
        {
            AXIS2_OM_NODE_SET_BUILD_STATUS((builder->lastnode), env, AXIS2_TRUE);
        }
    }

    return AXIS2_SUCCESS;
}


axis2_om_node_t * AXIS2_CALL
axis2_om_stax_builder_next (axis2_om_stax_builder_t *om_stax_builder,
                            axis2_env_t **env)
{
    int token = 0;
    axis2_om_stax_builder_impl_t *builder = NULL;
    axis2_om_node_t *node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, NULL);

    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    do
    {
        if (builder->done)
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, NULL);

        token = AXIS2_PULL_PARSER_NEXT (builder->parser, env);

        if (!(builder->cache))
        {
            return NULL;
        }


        switch (token)
        {
        case AXIS2_PULL_PARSER_START_DOCUMENT:
            /*Do nothing */
            break;
        
        case AXIS2_PULL_PARSER_START_ELEMENT:
           node = axis2_om_stax_builder_create_om_element (
                         om_stax_builder, env); 
            break;
        
        case AXIS2_PULL_PARSER_EMPTY_ELEMENT:
            node = axis2_om_stax_builder_create_om_element (
                        om_stax_builder, env);
        
        case AXIS2_PULL_PARSER_END_ELEMENT:
            axis2_om_stax_builder_end_element (om_stax_builder, env);
            break;
        
        
        case AXIS2_PULL_PARSER_SPACE:
            /* Do nothing */
            break;
        
        case AXIS2_PULL_PARSER_CHARACTER:
            node =
                axis2_om_stax_builder_create_om_text (om_stax_builder, env);
            break;
                
        case AXIS2_PULL_PARSER_ENTITY_REFERANCE:
            break;
        
        case AXIS2_PULL_PARSER_COMMENT:
            break;
        
        case AXIS2_PULL_PARSER_PROCESSING_INSTRUCTION:
            break;
        
        case AXIS2_PULL_PARSER_CDATA:
            break;
        
        case AXIS2_PULL_PARSER_DOCUMENT_TYPE:
            break;
        
        default:
            break;
        }
    }
    while (!node);

    return node;
}


axis2_status_t 
AXIS2_CALL axis2_om_stax_builder_free(axis2_om_stax_builder_t *builder,
                                      axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
 	if(AXIS2_INTF_TO_IMPL(builder)->parser)
    {
		AXIS2_PULL_PARSER_FREE(AXIS2_INTF_TO_IMPL(builder)->parser, env);
    }   
	if(builder->ops)
		AXIS2_FREE ((*env)->allocator,builder->ops);
	AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(builder));
	return AXIS2_SUCCESS;	
	
}

axis2_om_node_t* AXIS2_CALL
axis2_om_stax_builder_get_lastnode (axis2_om_stax_builder_t *builder,
                                   axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    return AXIS2_INTF_TO_IMPL(builder)->lastnode;
}                                   
                                            
axis2_om_document_t* AXIS2_CALL
axis2_om_stax_builder_get_document (axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(builder,env,NULL);
    return AXIS2_INTF_TO_IMPL(builder)->document;
}
                                    
                                            
axis2_status_t AXIS2_CALL
axis2_om_stax_builder_set_lastnode(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_node_t *om_node)
{
    AXIS2_FUNC_PARAM_CHECK(builder,env,AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_node, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(builder)->lastnode = om_node ;
    return AXIS2_SUCCESS;
}                                    
                                            
axis2_status_t  AXIS2_CALL
axis2_om_stax_builder_set_document(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_document_t *document)
{
    
    AXIS2_FUNC_PARAM_CHECK(builder,env,AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, document , AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(builder)->document = document ;
   
    return AXIS2_SUCCESS;
}
