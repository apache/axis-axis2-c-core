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

#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_om_comment.h>
#include <axis2_om_processing_instruction.h>
#include <axis2_om_doctype.h>
#include <axis2_om_document.h>
#include <axis2_om_stax_builder.h>


/*************************** function prototypes ******************************************/

axis2_status_t AXIS2_CALL 
axis2_om_node_free_tree (axis2_om_node_t * om_node,
                        axis2_env_t **env);


axis2_status_t AXIS2_CALL 
axis2_om_node_add_child (axis2_om_node_t *om_node,
                         axis2_env_t **env,
                         axis2_om_node_t * child);
                              
axis2_om_node_t * AXIS2_CALL 
axis2_om_node_detach (axis2_om_node_t *om_node,
                      axis2_env_t **env);


axis2_status_t AXIS2_CALL 
axis2_om_node_set_parent (axis2_om_node_t * om_node,
                          axis2_env_t **env,
                          axis2_om_node_t * parent);
                               
axis2_status_t AXIS2_CALL 
axis2_om_node_insert_sibling_after (axis2_om_node_t *om_node,
                                   axis2_env_t **env,
                                   axis2_om_node_t *node_to_insert);
                                         
axis2_status_t AXIS2_CALL 
axis2_om_node_insert_sibling_before (axis2_om_node_t *om_node,
                                     axis2_env_t **env, 
                                     axis2_om_node_t *node_to_insert);
                                          
                                          
axis2_om_node_t * AXIS2_CALL 
axis2_om_node_iterator_get_first_child(axis2_om_node_t *om_node,
                                       axis2_env_t **env);
                                                     
                                                     
                                                     
axis2_om_node_t * AXIS2_CALL 
axis2_om_node_iterator_get_next_child (axis2_om_node_t *om_node,
                                       axis2_env_t **env);
                                   
axis2_status_t AXIS2_CALL 
axis2_om_node_serialize (axis2_om_node_t * om_node,
                         axis2_env_t **env,
                         axis2_om_output_t * om_output);
                              
                              
/****************************** accessor mutator *********************************************/
axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_parent(axis2_om_node_t *om_node,
                         axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_first_child(axis2_om_node_t *om_node,
                              axis2_env_t **env);
        
axis2_om_node_t* AXIS2_CALL
axis2_om_node_get_last_child(axis2_om_node_t *om_node,
                             axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_previous_sibling(axis2_om_node_t *om_node,
                                   axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_next_sibling(axis2_om_node_t *om_node,
                                axis2_env_t **env);

axis2_om_types_t AXIS2_CALL 
axis2_om_node_get_node_type(axis2_om_node_t *om_node,
                            axis2_env_t **env);
                                 
void* AXIS2_CALL 
axis2_om_node_get_data_element(axis2_om_node_t *om_node,
                               axis2_env_t **env);                                

axis2_status_t AXIS2_CALL  
axis2_om_node_set_first_child(axis2_om_node_t *om_node,
                              axis2_env_t **env,
                              axis2_om_node_t *first_child);
        
axis2_status_t AXIS2_CALL 
axis2_om_node_set_last_child(axis2_om_node_t *om_node,
                             axis2_env_t **env,
                             axis2_om_node_t *last_child);

axis2_status_t AXIS2_CALL  
axis2_om_node_set_previous_sibling(axis2_om_node_t *om_node,                                  
                                   axis2_env_t **env,
                                   axis2_om_node_t *prev_sibling);

axis2_status_t AXIS2_CALL  
axis2_om_node_set_next_sibling(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_om_node_t *next_sibling);

axis2_status_t AXIS2_CALL  
axis2_om_node_set_node_type(axis2_om_node_t *om_node,
                            axis2_env_t **env,
                            axis2_om_types_t type);

axis2_status_t AXIS2_CALL  
axis2_om_node_set_data_element(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               void* data_element);
        
        
axis2_bool_t AXIS2_CALL
axis2_om_node_get_build_status(axis2_om_node_t *om_node,
                               axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_node_set_build_status(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_bool_t done);
axis2_status_t AXIS2_CALL
axis2_om_node_set_document(axis2_om_node_t *om_node,
                           axis2_env_t **env,
                           struct axis2_om_document *om_doc);

axis2_status_t AXIS2_CALL
axis2_om_node_set_builder(axis2_om_node_t *om_node,
                          axis2_env_t **env,
                          axis2_om_stax_builder_t *builder);
                           
struct axis2_om_document* AXIS2_CALL
axis2_om_node_get_document(axis2_om_node_t *om_node,
                           axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL
axis2_om_node_build_next(axis2_om_node_t *om_node,
                         axis2_env_t **env);                                                                                     


/************************************************************************************/

typedef struct axis2_om_node_impl
{
     axis2_om_node_t om_node;
    
     /** document only availble if build through builder */   
     struct axis2_om_document *om_doc;
     
     axis2_om_stax_builder_t *builder;
     /** parent node */
     axis2_om_node_t *parent;
     /** previous sibling */
     axis2_om_node_t *prev_sibling;
     /** next sibling */
     axis2_om_node_t *next_sibling;
     /** first child */
     axis2_om_node_t *first_child;
     /** last child */
     axis2_om_node_t *last_child;
     /** node type, indicates the type stored in data_element */
     axis2_om_types_t node_type;
     /** done true means that this node is completely built , false otherwise */
     int done;
     
     /** instances of an om struct, whose type is defined by node type */
     void *data_element;
     
}axis2_om_node_impl_t;


/****************************** Macro  ***************************************/

#define AXIS2_INTF_TO_IMPL(node) ((axis2_om_node_impl_t *)node)
                               


/*****************************************************************************/
AXIS2_DECLARE(axis2_om_node_t *)
axis2_om_node_create (axis2_env_t **env)
{
    axis2_om_node_impl_t *node = NULL;
    AXIS2_ENV_CHECK(env,NULL);
    
    
     node = (axis2_om_node_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                    sizeof (axis2_om_node_impl_t));
    if (!node)
    {
        (*env)->error->error_number = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    node->om_node.ops = (axis2_om_node_ops_t *) AXIS2_MALLOC
                             ((*env)->allocator,sizeof (axis2_om_node_ops_t));
                             
    if (!(node->om_node.ops))
    {   
        AXIS2_FREE ((*env)->allocator, node);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    node->first_child = NULL;
    node->last_child = NULL;
    node->next_sibling = NULL;
    node->prev_sibling = NULL;
    node->parent = NULL;
    node->node_type = AXIS2_OM_INVALID;
    node->done = AXIS2_FALSE;
    node->data_element = NULL;
    node->om_doc = NULL;
    node->builder = NULL;

    /* assign fucn pointers */
    node->om_node.ops->add_child = axis2_om_node_add_child;
    node->om_node.ops->free = axis2_om_node_free_tree;
    node->om_node.ops->detach = axis2_om_node_detach;
    
    node->om_node.ops->insert_sibling_after = axis2_om_node_insert_sibling_after;
    node->om_node.ops->insert_sibling_before = axis2_om_node_insert_sibling_before;
    node->om_node.ops->set_parent = axis2_om_node_set_parent;
    node->om_node.ops->serialize = axis2_om_node_serialize;

    node->om_node.ops->get_first_child = axis2_om_node_get_first_child;
    node->om_node.ops->get_last_child = axis2_om_node_get_last_child;
    node->om_node.ops->get_next_sibling = axis2_om_node_get_next_sibling;
    node->om_node.ops->get_previous_sibling = axis2_om_node_get_previous_sibling;
    node->om_node.ops->get_parent = axis2_om_node_get_parent ;
    node->om_node.ops->get_node_type = axis2_om_node_get_node_type;
    node->om_node.ops->get_build_status = axis2_om_node_get_build_status;
    node->om_node.ops->get_data_element = axis2_om_node_get_data_element;
    
    
    node->om_node.ops->set_data_element = axis2_om_node_set_data_element;
    node->om_node.ops->set_first_child = axis2_om_node_set_parent;
    node->om_node.ops->set_last_child = axis2_om_node_set_last_child;
    node->om_node.ops->set_previous_sibling = axis2_om_node_set_previous_sibling;
    node->om_node.ops->set_next_sibling = axis2_om_node_set_next_sibling;
    node->om_node.ops->set_first_child = axis2_om_node_set_first_child;
    node->om_node.ops->set_last_child = axis2_om_node_set_last_child;
    node->om_node.ops->set_node_type = axis2_om_node_set_node_type;
    node->om_node.ops->set_build_status = axis2_om_node_set_build_status;
    
    node->om_node.ops->get_document = axis2_om_node_get_document;
    node->om_node.ops->set_document = axis2_om_node_set_document;
    node->om_node.ops->set_builder = axis2_om_node_set_builder;
    
    node->om_node.ops->build_next = axis2_om_node_build_next;
        
    return &(node->om_node);
}

/**
 *  This free fucntion will free an om_element and all the children contained in it
 *  before calling this function  
*/ 

axis2_status_t AXIS2_CALL axis2_om_node_free_tree(axis2_om_node_t *om_node,
                                                  axis2_env_t **env)
{
    axis2_om_node_impl_t *node_impl = NULL;   
    axis2_om_node_t *child_node = NULL;
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    
    node_impl = AXIS2_INTF_TO_IMPL(om_node);
        
    if (node_impl->first_child)
    {   
        while (node_impl->first_child)
        {
            child_node = AXIS2_OM_NODE_DETACH (node_impl->first_child, env);
            if (child_node)
            {
                AXIS2_OM_NODE_FREE_TREE ( child_node , env); 
            }
        }
    }
    
    switch (node_impl->node_type)
    {
    case AXIS2_OM_ELEMENT:
        AXIS2_OM_ELEMENT_FREE((axis2_om_element_t*)(node_impl->data_element), env);
        break;

    case AXIS2_OM_COMMENT:
        AXIS2_OM_COMMENT_FREE((axis2_om_comment_t*)(node_impl->data_element), env);
        break;
    case AXIS2_OM_DOCTYPE:
        /*AXIS2_OM_DOCTYPE_FREE((axis2_om_doctype_t*)(node_impl->data_element), env);*/
        break;
    case AXIS2_OM_PROCESSING_INSTRUCTION:
         AXIS2_OM_PROCESSING_INSTRUCTION_FREE(
            (axis2_om_processing_instruction_t*)(node_impl->data_element), env);
        break;
    case AXIS2_OM_TEXT:
        AXIS2_OM_TEXT_FREE((axis2_om_text_t*)(node_impl->data_element),env);
        break;

    default:
        break;
    }
    
    AXIS2_FREE ((*env)->allocator, om_node->ops);
    AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_node));
    

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_om_node_add_child (axis2_om_node_t *om_node,
                         axis2_env_t **env,
                         axis2_om_node_t *child)
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, child, AXIS2_FAILURE);
    if(AXIS2_INTF_TO_IMPL(child)->parent)
    {
        child = AXIS2_OM_NODE_DETACH(child, env);
    }
    
    if (AXIS2_INTF_TO_IMPL(om_node)->first_child  == NULL)
    {
        AXIS2_INTF_TO_IMPL(om_node)->first_child = child;
    }
    else
    {
       axis2_om_node_t *last_sib = NULL;
       last_sib  = AXIS2_INTF_TO_IMPL(om_node)->last_child;
       if(last_sib)
           AXIS2_INTF_TO_IMPL(last_sib)->next_sibling = child;
        AXIS2_INTF_TO_IMPL(child)->prev_sibling = last_sib;
    }

    AXIS2_INTF_TO_IMPL(child)->parent  = om_node;
    AXIS2_INTF_TO_IMPL(om_node)->last_child = child;
    return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_node_detach (axis2_om_node_t *om_node,
                      axis2_env_t **env)
{
    axis2_om_node_t *parent = NULL;
    
   
    AXIS2_ENV_CHECK(env, NULL);
    
    parent = AXIS2_INTF_TO_IMPL(om_node)->parent;
    
    AXIS2_PARAM_CHECK((*env)->error,parent,NULL);
    

    if (!(AXIS2_INTF_TO_IMPL(om_node)->prev_sibling))
    {
        AXIS2_INTF_TO_IMPL(parent)->first_child = 
                AXIS2_INTF_TO_IMPL(om_node)->next_sibling;
    }
    else
    {    
        axis2_om_node_t *prev_sib = NULL;
        prev_sib  = AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
        if(prev_sib)
        {   AXIS2_INTF_TO_IMPL(prev_sib)->next_sibling = 
                AXIS2_INTF_TO_IMPL(om_node)->next_sibling;
        }
    }
    
    if (AXIS2_INTF_TO_IMPL(om_node)->next_sibling)
    {
        axis2_om_node_t *next_sibling = NULL;
        next_sibling = AXIS2_INTF_TO_IMPL(om_node)->next_sibling;
        if(next_sibling)
            AXIS2_INTF_TO_IMPL(next_sibling)->prev_sibling =
                AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
    }

    AXIS2_INTF_TO_IMPL(om_node)->parent = NULL;
    AXIS2_INTF_TO_IMPL(om_node)->prev_sibling = NULL;
    AXIS2_INTF_TO_IMPL(om_node)->next_sibling = NULL;
    return om_node;

}

axis2_status_t AXIS2_CALL
axis2_om_node_set_parent (axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_om_node_t * parent)
{
   
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, parent, AXIS2_FAILURE);
    
    if(!parent)
    {

        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    
    if (parent == AXIS2_INTF_TO_IMPL(om_node)->parent)
    {   /* same parent already exist */
        return AXIS2_SUCCESS;
    }
    /* if a new parent is assigned in  place of existing 
     *  one first the node should  be detached  
     */
    if((AXIS2_INTF_TO_IMPL(om_node)->parent))
    {
        axis2_om_node_detach(om_node,env);
    }
    AXIS2_INTF_TO_IMPL(om_node)->parent = parent;
    return AXIS2_SUCCESS;
}


/**
 * This will insert a sibling just after the current information item
 * @param node the node in consideration
 * @param nodeto_insert the node that will be inserted
 */

axis2_status_t AXIS2_CALL
axis2_om_node_insert_sibling_after (axis2_om_node_t * om_node,
                                    axis2_env_t **env,
                                    axis2_om_node_t * node_to_insert)
{
    axis2_om_node_t *next_sib = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, node_to_insert, AXIS2_FAILURE);
 
    AXIS2_INTF_TO_IMPL(node_to_insert)->parent = 
        AXIS2_INTF_TO_IMPL(om_node)->parent;
    AXIS2_INTF_TO_IMPL(node_to_insert)->prev_sibling = om_node;
        
    next_sib = AXIS2_INTF_TO_IMPL(om_node)->next_sibling;    
    
    if(next_sib)
    {
        AXIS2_INTF_TO_IMPL(next_sib)->prev_sibling = node_to_insert;
    }
    
    AXIS2_INTF_TO_IMPL(node_to_insert)->next_sibling = 
        AXIS2_INTF_TO_IMPL(om_node)->next_sibling;        
    AXIS2_INTF_TO_IMPL(om_node)->next_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_node_insert_sibling_before (axis2_om_node_t *om_node,
                                     axis2_env_t **env,
                                     axis2_om_node_t * node_to_insert)
{
    axis2_om_node_t *prev_sibling = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, node_to_insert, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(node_to_insert)->parent = 
        AXIS2_INTF_TO_IMPL(om_node)->parent ;
            
    AXIS2_INTF_TO_IMPL(node_to_insert)->prev_sibling = 
        AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
    AXIS2_INTF_TO_IMPL(node_to_insert)->next_sibling = om_node;
    prev_sibling = AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
    if (!prev_sibling)
    {
        axis2_om_node_t *parent = AXIS2_INTF_TO_IMPL(om_node)->parent;
        AXIS2_INTF_TO_IMPL(parent)->first_child = node_to_insert;
    }
    else
    {
        axis2_om_node_t *prev_sibling = AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
        if(prev_sibling)
            AXIS2_INTF_TO_IMPL(prev_sibling)->next_sibling = node_to_insert;
    }
    AXIS2_INTF_TO_IMPL(om_node)->prev_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_node_serialize (axis2_om_node_t *om_node,
                         axis2_env_t **env,
                         axis2_om_output_t *om_output)
{
    
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,om_output,AXIS2_FAILURE);
    

    switch (AXIS2_INTF_TO_IMPL(om_node)->node_type)
    {
    case AXIS2_OM_ELEMENT:
        status = AXIS2_OM_ELEMENT_SERIALIZE_START_PART(
                 (axis2_om_element_t *)(AXIS2_INTF_TO_IMPL(om_node)->data_element),
                  env,
                  om_output);
                  
        if (status != AXIS2_SUCCESS)
            return status;
        break;
    case AXIS2_OM_TEXT:
        status = AXIS2_OM_TEXT_SERIALIZE (
                (axis2_om_text_t*)(AXIS2_INTF_TO_IMPL(om_node)->data_element),
                env, om_output);
        if(status != AXIS2_SUCCESS)
            return status;
        break;
    case AXIS2_OM_COMMENT:
        status = AXIS2_OM_COMMENT_SERIALIZE(
                (axis2_om_comment_t*)(AXIS2_INTF_TO_IMPL(om_node)->data_element),
                env, om_output);
        if( status != AXIS2_SUCCESS)
            return status;
        
        break;    
    
    case AXIS2_OM_DOCTYPE:
        status = AXIS2_OM_DOCTYPE_SERIALIZE(        
                (axis2_om_doctype_t*)(AXIS2_INTF_TO_IMPL(om_node)->data_element),
                 env, om_output);
        if( status != AXIS2_SUCCESS)
            return status;
        break;
        
    case AXIS2_OM_PROCESSING_INSTRUCTION:
        status = AXIS2_OM_PROCESSING_INSTRUCTION_SERIALIZE(
                (axis2_om_processing_instruction_t*)
                    (AXIS2_INTF_TO_IMPL(om_node)->data_element), env, om_output);
        if( status != AXIS2_SUCCESS)
            return status;
        break;
        
    default:
        break;
    }
    
    /* serialize children of this node */
    if(AXIS2_INTF_TO_IMPL(om_node)->first_child)
        status =  axis2_om_node_serialize(AXIS2_INTF_TO_IMPL(om_node)->first_child, env, om_output);

    switch (AXIS2_INTF_TO_IMPL(om_node)->node_type)
    {
    case AXIS2_OM_ELEMENT:
        status = AXIS2_OM_ELEMENT_SERIALIZE_END_PART ((axis2_om_element_t *)
                                   (AXIS2_INTF_TO_IMPL(om_node)->data_element),
                                    env, om_output);
        if (status != AXIS2_SUCCESS)
            return status;
        break;
    
    default:
        break;
    }
    /* serialize next sibling */  
    if(AXIS2_INTF_TO_IMPL(om_node)->next_sibling)    
        status = axis2_om_node_serialize(AXIS2_INTF_TO_IMPL(om_node)->next_sibling, env, om_output);
    
    
    return status;
 
}

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_parent(axis2_om_node_t *om_node,
                         axis2_env_t **env)
{
    
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->parent;
    
}
axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_first_child(axis2_om_node_t *om_node,
                              axis2_env_t **env)
{   
    axis2_om_node_impl_t *om_node_impl = NULL;
    int token = 0;
    AXIS2_ENV_CHECK(env, NULL);
    om_node_impl = AXIS2_INTF_TO_IMPL(om_node);
    /**********************************************************/
    while(!(om_node_impl->first_child) && !(om_node_impl->done) 
        && om_node_impl->builder)
    {
        token = AXIS2_OM_STAX_BUILDER_NEXT_WITH_TOKEN(om_node_impl->builder, env);
        if(token == -1)
            break;
    }
    /**********************************************************/
    return om_node_impl->first_child;
}
        
axis2_om_node_t* AXIS2_CALL
axis2_om_node_get_last_child(axis2_om_node_t *om_node,
                             axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->last_child;

}

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_previous_sibling(axis2_om_node_t *om_node,
                                   axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
}

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_next_sibling(axis2_om_node_t *om_node,
                                axis2_env_t **env)
{
    axis2_om_node_impl_t *om_node_impl = NULL;
    int token = 0;
    AXIS2_ENV_CHECK(env, NULL);
    om_node_impl = AXIS2_INTF_TO_IMPL(om_node);
    /*****************************************************/
    while(!(om_node_impl->next_sibling) && om_node_impl->parent &&
    om_node_impl->builder && !(AXIS2_OM_NODE_GET_BUILD_STATUS(om_node_impl->parent, env)))
    {
        token = AXIS2_OM_STAX_BUILDER_NEXT_WITH_TOKEN(om_node_impl->builder, env);
        if(token == -1)
           break;
    }
    /*******************************************************/
    return om_node_impl->next_sibling;
}

axis2_om_types_t AXIS2_CALL 
axis2_om_node_get_node_type(axis2_om_node_t *om_node,
                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_OM_INVALID);
    return AXIS2_INTF_TO_IMPL(om_node)->node_type;
}
                                 
void* AXIS2_CALL 
axis2_om_node_get_data_element(axis2_om_node_t *om_node,
                               axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->data_element;
}                                

axis2_status_t AXIS2_CALL  
axis2_om_node_set_first_child(axis2_om_node_t *om_node,axis2_env_t **env,
                              axis2_om_node_t *first_child)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, first_child, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(om_node)->first_child = first_child;
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL 
axis2_om_node_set_last_child(axis2_om_node_t *om_node,axis2_env_t **env,
                             axis2_om_node_t *last_child)
{
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, last_child, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(om_node)->last_child = last_child;
    return AXIS2_SUCCESS;
}
axis2_status_t AXIS2_CALL  
axis2_om_node_set_previous_sibling(axis2_om_node_t *om_node,                                  
                                   axis2_env_t **env,
                                   axis2_om_node_t *prev_sibling)
{
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prev_sibling, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(om_node)->prev_sibling = prev_sibling;
    return AXIS2_SUCCESS;
} 
axis2_status_t AXIS2_CALL  
axis2_om_node_set_next_sibling(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_om_node_t *next_sibling)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, next_sibling, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->next_sibling = next_sibling;
    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL  
axis2_om_node_set_node_type(axis2_om_node_t *om_node,
                            axis2_env_t **env,
                            axis2_om_types_t type)
{
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->node_type = type;
    return AXIS2_SUCCESS;
}                            

axis2_status_t AXIS2_CALL  
axis2_om_node_set_data_element(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               void* data_element)
{                               
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data_element, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->data_element = data_element;
    return AXIS2_SUCCESS;                               
}       
        
axis2_bool_t AXIS2_CALL
axis2_om_node_get_build_status(axis2_om_node_t *om_node,
                               axis2_env_t **env)
{
    /* on error what value should be returned ?...*/
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_node)->done;

}                               

axis2_status_t AXIS2_CALL
axis2_om_node_set_build_status(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_bool_t done)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->done = done;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_node_set_document(axis2_om_node_t *om_node,
                           axis2_env_t **env,
                           struct axis2_om_document *om_doc)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_doc, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->om_doc = om_doc;
    return AXIS2_SUCCESS;
}                           

struct axis2_om_document* AXIS2_CALL
axis2_om_node_get_document(axis2_om_node_t *om_node,
                           axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->om_doc;
}                           

axis2_om_node_t* AXIS2_CALL
axis2_om_node_build_next(axis2_om_node_t *om_node,
                         axis2_env_t **env)
{
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_node_impl_t *om_node_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    om_node_impl = AXIS2_INTF_TO_IMPL(om_node);
    builder = om_node_impl->builder;
    if(!builder)
        return NULL;
    return AXIS2_OM_STAX_BUILDER_NEXT(om_node_impl->builder, env);
}

axis2_status_t AXIS2_CALL
axis2_om_node_set_builder(axis2_om_node_t *om_node,
                           axis2_env_t **env,
                           axis2_om_stax_builder_t* builder)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->builder = builder;
    return AXIS2_SUCCESS;

}

