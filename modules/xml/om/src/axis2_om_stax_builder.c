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
#include <guththila_xml_pull_parser.h>


guththila_environment_t *om_stax_builder_guththila_environment = NULL;
guththila_allocator_t *om_stax_builder_guththila_allocator = NULL;
axis2_bool_t parser_created_locally = AXIS2_FALSE;

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
    void *parser;
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
axis2_om_stax_builder_create (axis2_env_t **env, void *parser, void* parser_env)
{
    axis2_om_stax_builder_impl_t *builder = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    builder = (axis2_om_stax_builder_impl_t *)AXIS2_MALLOC (
                (*env)->allocator, sizeof(axis2_om_stax_builder_t));

    if (!builder)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);

    builder->parser = NULL;
    if (parser)
	{
        builder->parser = parser;
		if (parser_env)
			om_stax_builder_guththila_environment =  (guththila_environment_t*) parser_env;
	}
    else
    {
        /* create the default Guththila pull parser */
        guththila_reader_t *reader = NULL;
		om_stax_builder_guththila_allocator = guththila_allocator_init(NULL);
  		om_stax_builder_guththila_environment = 
  		        guththila_environment_create(om_stax_builder_guththila_allocator,
  		                                    NULL, NULL, NULL, NULL);
        reader = guththila_reader_create (om_stax_builder_guththila_environment, stdin);
		
        if (!reader){
            AXIS2_FREE((*env)->allocator, builder);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        }
        builder->parser = guththila_xml_pull_parser_create (
                            om_stax_builder_guththila_environment, reader);

        if (!builder->parser)
        {
            AXIS2_FREE((*env)->allocator, builder);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        }
		parser_created_locally = AXIS2_TRUE;
    }

    builder->cache = AXIS2_TRUE;
    builder->parser_accessed = AXIS2_FALSE;
    builder->done = AXIS2_FALSE;
    builder->lastnode = NULL;
    builder->document = NULL;

    /* operations */
    builder->om_stax_builder.ops = NULL;
    builder->om_stax_builder.ops = (axis2_om_stax_builder_ops_t *) AXIS2_MALLOC(
                                    (*env)->allocator, sizeof(axis2_om_stax_builder_t));

    if (!builder->om_stax_builder.ops)
    {
        guththila_xml_pull_parser_free (om_stax_builder_guththila_environment, builder->parser);
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
    

    attribute_count = guththila_xml_pull_parser_get_attribute_count (
                                om_stax_builder_guththila_environment,
                                builder->parser);
                               
    for (i = 0; i < attribute_count; i++)
    {

        uri =  guththila_xml_pull_parser_get_attribute_namespace_by_number
              (om_stax_builder_guththila_environment, builder->parser, i);
              
        prefix = guththila_xml_pull_parser_get_attribute_prefix_by_number
                 (om_stax_builder_guththila_environment, builder->parser, i);

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
                            guththila_xml_pull_parser_get_attribute_name_by_number
                            (om_stax_builder_guththila_environment, builder->parser, i),
                            guththila_xml_pull_parser_get_attribute_value_by_number
                            (om_stax_builder_guththila_environment, builder->parser, i), ns);
                            
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
   
    if (!builder->lastnode)
        AXIS2_ERROR_SET((*env)->error, 
                         AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL,NULL);

    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    temp_value = guththila_xml_pull_parser_get_value (
                    om_stax_builder_guththila_environment, builder->parser);

    if (!temp_value)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PULL_PARSER_VALUE_NULL, NULL);
    }
    else
    {
        value = (axis2_char_t*)AXIS2_STRDUP ( temp_value,env);
        guththila_free (om_stax_builder_guththila_environment->allocator, temp_value);
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
        while (guththila_xml_pull_parser_next (
                om_stax_builder_guththila_environment, builder->parser) != GUTHTHILA_END_ELEMENT);
    }
    while (!(AXIS2_OM_NODE_GET_BUILD_STATUS(element, env)));

    /*All children of this element is pulled now */

    prev_node = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(element, env);
    if (prev_node)
    {
        AXIS2_OM_NODE_FREE(AXIS2_OM_NODE_GET_NEXT_SIBLING(prev_node, env), env);
        AXIS2_OM_NODE_SET_NEXT_SIBLING(prev_node, env, NULL);        
    }
    else
    {
        parent = AXIS2_OM_NODE_GET_PARENT(element, env);
        AXIS2_OM_NODE_FREE(AXIS2_OM_NODE_GET_FIRST_CHILD(parent, env), env);
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
    
    AXIS2_FUNC_PARAM_CHECK(om_stax_builder, env, AXIS2_FAILURE);
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    namespace_count = guththila_xml_pull_parser_get_namespace_count (
                      om_stax_builder_guththila_environment, builder->parser);

    
    
    for (; namespace_count > 0; namespace_count--)
    {
        om_ns = axis2_om_namespace_create (
                env,
                guththila_xml_pull_parser_get_namespace_uri_by_number
                (om_stax_builder_guththila_environment, builder->parser, namespace_count),
                guththila_xml_pull_parser_get_namespace_prefix_by_number
                (om_stax_builder_guththila_environment, builder->parser, namespace_count));
                
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
    temp_prefix = guththila_xml_pull_parser_get_prefix (
                  om_stax_builder_guththila_environment, builder->parser);

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
    
    temp_localname  = guththila_xml_pull_parser_get_name (
                        om_stax_builder_guththila_environment, builder->parser);

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
        /* should have a way to access documents root_element 
            without calling build next ????????????????
        if (AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(builder->document, env))
             AXIS2_OM_NODE_FREE (
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
                AXIS2_OM_NODE_SET_PARENT((builder->lastnode), env, parent);
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

        token = guththila_xml_pull_parser_next (om_stax_builder_guththila_environment, builder->parser);

        if (!(builder->cache))
        {
            return NULL;
        }


        switch (token)
        {
        case GUTHTHILA_START_DOCUMENT:
            /*axis2_om_stax_builder_process_start_document(builder); */
            /*Do nothing */
            break;
        case GUTHTHILA_START_ELEMENT:
        /*    node = axis2_om_stax_builder_create_om_element (
                        om_stax_builder, env); */
            break;
        case GUTHTHILA_EMPTY_ELEMENT:
        /*    node = axis2_om_stax_builder_create_om_element (
                        om_stax_builder, env); */
        case GUTHTHILA_END_ELEMENT:
            axis2_om_stax_builder_end_element (om_stax_builder, env);
            break;
        case GUTHTHILA_SPACE:
            /* Do nothing */
            break;
        case GUTHTHILA_CHARACTER:
            node =
                axis2_om_stax_builder_create_om_text (om_stax_builder, env);
            break;
        case GUTHTHILA_COMMENT:
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
 	if(AXIS2_INTF_TO_IMPL(builder)->parser &&
		parser_created_locally)
    {
		guththila_xml_pull_parser_free(om_stax_builder_guththila_environment,
			                            AXIS2_INTF_TO_IMPL(builder)->parser);
			                            
        parser_created_locally = AXIS2_FALSE;
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