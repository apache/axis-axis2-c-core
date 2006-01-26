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


#include <axis2_om_document.h>
#include <axis2_om_stax_builder.h>
#include <axis2_string.h>

axis2_status_t AXIS2_CALL 
axis2_om_document_free (axis2_om_document_t * document,
                        axis2_env_t **env);
                        
axis2_status_t AXIS2_CALL 
axis2_om_document_add_child (axis2_om_document_t *document,
                             axis2_env_t **env,
                             axis2_om_node_t * child);
                             
axis2_om_node_t * AXIS2_CALL
axis2_om_document_build_next (axis2_om_document_t *document,
                              axis2_env_t **env);
                              
axis2_om_node_t * AXIS2_CALL 
axis2_om_document_get_root_element (axis2_om_document_t *document,
                                    axis2_env_t **env);
                                   
axis2_status_t  AXIS2_CALL
axis2_om_document_set_root_element(axis2_om_document_t *document,
                                   axis2_env_t **env,
                                   axis2_om_node_t *node);  

axis2_om_node_t* AXIS2_CALL
axis2_om_document_build_all(axis2_om_document_t *document,
                            axis2_env_t **env);

axis2_om_stax_builder_t* AXIS2_CALL
axis2_om_document_get_builder(axis2_om_document_t *document,
                              axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_document_serialize(axis2_om_document_t *document,
                            axis2_env_t **env,
                            axis2_om_output_t *om_output);                              
                                 
axis2_status_t AXIS2_CALL
axis2_om_document_free_om_nodes(axis2_om_document_t *document,
                             axis2_env_t **env);
/********************************* end of function pointers ******************/

typedef struct axis2_om_document_impl_t
{
    axis2_om_document_t om_document;
    /** root element */
    axis2_om_node_t *root_element;
    /** last child */
    axis2_om_node_t *last_child;
    /** first child */
    axis2_om_node_t *first_child;
    /** done building the document */
    axis2_bool_t done;
    /** builder of the document */
    struct axis2_om_stax_builder *builder;
    /** char set encoding */        
    axis2_char_t *char_set_encoding;
    /** XML version */
    axis2_char_t *xml_version;
}axis2_om_document_impl_t;        

/************************************ Macro ************************************/ 

#define AXIS2_INTF_TO_IMPL(document) ((axis2_om_document_impl_t*)document)

/*******************************************************************************/

AXIS2_DECLARE(axis2_om_document_t *)
axis2_om_document_create (axis2_env_t **env,
                          axis2_om_node_t *root,
                          axis2_om_stax_builder_t *builder)
{
    axis2_om_document_impl_t *document = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    document = (axis2_om_document_impl_t *) AXIS2_MALLOC (
                (*env)->allocator, sizeof (axis2_om_document_impl_t));
    
    if (!document)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    document->builder = builder;
    document->root_element = root;
    document->first_child = root;
    document->last_child = root;
   
   
    document->char_set_encoding = NULL;
    document->char_set_encoding = (axis2_char_t *) AXIS2_STRDUP(CHAR_SET_ENCODING,env);

    if (!document->char_set_encoding)
    {
        AXIS2_FREE((*env)->allocator, document);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    document->xml_version = NULL;
    document->xml_version = (axis2_char_t *) AXIS2_STRDUP(XML_VERSION,env);
    if (!document->xml_version)
    {
        
        AXIS2_FREE((*env)->allocator, document->char_set_encoding);
        AXIS2_FREE((*env)->allocator, document);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    document->done = AXIS2_FALSE;

    

    document->om_document.ops = NULL;
    document->om_document.ops = (axis2_om_document_ops_t *) AXIS2_MALLOC ((*env)->allocator,
                                                  sizeof(axis2_om_document_ops_t));

    if (!document->om_document.ops)
    {
        
        AXIS2_FREE((*env)->allocator, document->char_set_encoding);
        AXIS2_FREE((*env)->allocator, document->xml_version);
        AXIS2_FREE((*env)->allocator, document);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
  
    document->om_document.ops->free = axis2_om_document_free;
    document->om_document.ops->add_child = axis2_om_document_add_child;
    document->om_document.ops->build_next = axis2_om_document_build_next;
    document->om_document.ops->get_root_element = axis2_om_document_get_root_element;
    document->om_document.ops->set_root_element = axis2_om_document_set_root_element;
    document->om_document.ops->build_all = axis2_om_document_build_all;
    document->om_document.ops->get_builder = axis2_om_document_get_builder;
    document->om_document.ops->serialize = axis2_om_document_serialize;
    document->om_document.ops->free_om_nodes = axis2_om_document_free_om_nodes;
    
    if (builder)
        AXIS2_OM_STAX_BUILDER_SET_DOCUMENT (builder, env, &(document->om_document) );
    
    return &(document->om_document);
}

axis2_status_t AXIS2_CALL
axis2_om_document_free (axis2_om_document_t *om_document,
                        axis2_env_t **env)
{
    axis2_om_document_impl_t *document = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_document, env, AXIS2_FAILURE);
    document = AXIS2_INTF_TO_IMPL(om_document);
    
    if (document->char_set_encoding)
        AXIS2_FREE((*env)->allocator, document->char_set_encoding);
    if (document->xml_version)
        AXIS2_FREE((*env)->allocator, document->xml_version);
    
   /* AXIS2_OM_NODE_FREE_TREE(document->root_element, env); */
    
    if(document->om_document.ops)
        AXIS2_FREE((*env)->allocator, document->om_document.ops);

    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(document));
    return AXIS2_SUCCESS;
}



axis2_status_t AXIS2_CALL
axis2_om_document_add_child (axis2_om_document_t *document,
                             axis2_env_t **env,
                             axis2_om_node_t * child)
{
    axis2_om_document_impl_t *document_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(document, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,child, AXIS2_FAILURE);

    document_impl = AXIS2_INTF_TO_IMPL(document);
    
    if (!(document_impl->root_element) && child)
    {
        document_impl->root_element = child;
        return AXIS2_SUCCESS;
    }

    if (document_impl->root_element && child)
    {
        
        return AXIS2_OM_NODE_ADD_CHILD (child, env,
                                        document_impl->last_child);
    }
    return AXIS2_FAILURE;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_document_build_next (axis2_om_document_t *om_document,
                                   axis2_env_t **env)
{
    axis2_om_document_impl_t *document = NULL;
    axis2_om_node_t  *last_child = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(om_document, env, NULL);
  
    document = AXIS2_INTF_TO_IMPL(om_document);
    
    if (!(document->root_element))
    {
        last_child = AXIS2_OM_STAX_BUILDER_NEXT (document->builder, env);
        if(!last_child)
        {
            document->last_child = last_child;
            document->root_element = last_child;
        }
        return last_child;
    }
    else if (AXIS2_OM_NODE_GET_BUILD_STATUS(document->root_element, env))
        return NULL;            /* Nothing wrong but done with pulling */
    
    last_child = AXIS2_OM_STAX_BUILDER_NEXT (document->builder, env);
    if(!last_child)
        document->last_child = last_child;
    return last_child;
}


axis2_om_node_t * AXIS2_CALL
axis2_om_document_get_root_element (axis2_om_document_t * document,
                                    axis2_env_t **env)
{
    axis2_om_node_t *node = NULL;
    axis2_om_document_impl_t *doc_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(document, env, NULL);
    doc_impl = AXIS2_INTF_TO_IMPL(document);
    
    if (doc_impl->root_element)
    {
        return doc_impl->root_element;
    }
    else
    {  
        node = axis2_om_document_build_next(document, env);
            
        if (doc_impl->root_element)
        {
            
            return doc_impl->root_element;
        }
        else
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL, AXIS2_FAILURE);
            return NULL;
        }
    }
}

axis2_status_t AXIS2_CALL
axis2_om_document_set_root_element(axis2_om_document_t *document,
                                   axis2_env_t **env,
                                   axis2_om_node_t *node)
{
    int status = AXIS2_SUCCESS;
    axis2_om_document_impl_t *document_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(document, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
    
    document_impl = AXIS2_INTF_TO_IMPL(document);
    
    if(document_impl->root_element)
    {
        status = AXIS2_OM_NODE_FREE_TREE(document_impl->root_element, env);
        if(status == AXIS2_SUCCESS)
        {
            document_impl->root_element = node;
            return AXIS2_SUCCESS;
         }
         return AXIS2_FAILURE;
    }
    else
    {
        document_impl->root_element = node;
    }
    return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_document_build_all(struct axis2_om_document *document,
                            axis2_env_t **env)
{   
    AXIS2_FUNC_PARAM_CHECK(document,env, NULL);
    do{ 
        AXIS2_OM_DOCUMENT_BUILD_NEXT(document,env);
    }while(!AXIS2_OM_NODE_GET_BUILD_STATUS(AXIS2_INTF_TO_IMPL(document)->root_element,env));
    return AXIS2_INTF_TO_IMPL(document)->root_element;    
}

axis2_om_stax_builder_t* AXIS2_CALL
axis2_om_document_get_builder(axis2_om_document_t *document,
                              axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(document, env, NULL);
    return AXIS2_INTF_TO_IMPL(document)->builder;
}

axis2_status_t AXIS2_CALL
axis2_om_document_serialize(axis2_om_document_t *document,
                            axis2_env_t **env,
                            axis2_om_output_t *om_output)
{
    axis2_om_document_impl_t *document_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(document, env, AXIS2_FAILURE);
    document_impl = AXIS2_INTF_TO_IMPL(document);
    if(!(document_impl->root_element))
    {
        axis2_om_document_get_root_element(document, env);        
    }
    return AXIS2_OM_NODE_SERIALIZE(document_impl->root_element, env, om_output);
}

axis2_status_t AXIS2_CALL
axis2_om_document_free_om_nodes(axis2_om_document_t *document,
                                axis2_env_t **env)
{
    axis2_om_document_impl_t *document_impl = NULL;
    document_impl = AXIS2_INTF_TO_IMPL(document);
    if(document_impl->root_element)
    {
        AXIS2_OM_NODE_FREE_TREE(document_impl->root_element, env);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
