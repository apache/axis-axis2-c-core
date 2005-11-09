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

/*************************** function prototypes ******************************************/

axis2_status_t AXIS2_CALL 
axis2_om_node_free (axis2_om_node_t * om_node,
                    axis2_env_t **env);


axis2_status_t AXIS2_CALL 
axis2_om_node_add_child (axis2_om_node_t *om_node,
                         axis2_env_t **env,
                         axis2_om_node_t * parent);
                              
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
axis2_om_node_iterator_get_first_child (axis2_om_node_t *om_node,
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
axis2_om_node_set_first_child(axis2_om_node_t *om_node,axis2_env_t **env,
                              axis2_om_node_t *first_child);
        
axis2_status_t AXIS2_CALL 
axis2_om_node_set_last_child(axis2_om_node_t *om_node,axis2_env_t **env,
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


/************************************************************************************/

typedef struct axis2_om_node_impl
{
     axis2_om_node_t om_node;
    
     struct axis2_om_node *parent;
     /** previous sibling */
     struct axis2_om_node *prev_sibling;
     /** next sibling */
     struct axis2_om_node *next_sibling;
     /** first child */
     struct axis2_om_node *first_child;
     /** current child */
     struct axis2_om_node *current_child;
     /** last child */
     struct axis2_om_node *last_child;
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
    node->om_node.ops = (axis2_om_node_ops_t *) AXIS2_MALLOC ((*env)->allocator,
                                                              sizeof (axis2_om_node_ops_t));
    if (!(node->om_node.ops))
    {   
        AXIS2_FREE ((*env)->allocator, node);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    /* assign fucn pointers */
    node->om_node.ops->add_child = axis2_om_node_add_child;
    node->om_node.ops->free = axis2_om_node_free;
    node->om_node.ops->detach = axis2_om_node_detach;
    node->om_node.ops->iterator_get_first_child = axis2_om_node_get_first_child;
    node->om_node.ops->insert_sibling_after = axis2_om_node_insert_sibling_after;
    node->om_node.ops->insert_sibling_before = axis2_om_node_insert_sibling_before;
    node->om_node.ops->set_parent = axis2_om_node_set_parent;
    node->om_node.ops->serialize = axis2_om_node_serialize;
    node->om_node.ops->iterator_get_next_child = axis2_om_node_iterator_get_next_child;
    node->om_node.ops->get_first_child = axis2_om_node_get_first_child;
    node->om_node.ops->get_last_child = axis2_om_node_get_last_child;
    node->om_node.ops->get_next_sibling = axis2_om_node_get_next_sibling;
    node->om_node.ops->get_previous_sibling = axis2_om_node_get_previous_sibling;
    node->om_node.ops->get_parent = axis2_om_node_get_parent ;
    node->om_node.ops->get_node_type = axis2_om_node_get_node_type;
    node->om_node.ops->get_build_status = axis2_om_node_get_build_status;
    node->om_node.ops->set_first_child = axis2_om_node_set_parent;
    node->om_node.ops->set_last_child = axis2_om_node_set_last_child;
    
    node->om_node.ops->set_previous_sibling = axis2_om_node_set_previous_sibling;
    node->om_node.ops->set_next_sibling = axis2_om_node_set_next_sibling;
    node->om_node.ops->set_first_child = axis2_om_node_set_first_child;
    node->om_node.ops->set_last_child = axis2_om_node_set_last_child;
    node->om_node.ops->set_node_type = axis2_om_node_set_node_type;
    node->om_node.ops->set_build_status = axis2_om_node_set_build_status;
    
    node->first_child = NULL;
    node->last_child = NULL;
    node->next_sibling = NULL;
    node->prev_sibling = NULL;
    node->parent = NULL;
    node->node_type = AXIS2_OM_INVALID;
    node->done = AXIS2_FALSE;
    node->data_element = NULL;
    node->current_child = NULL;
   
    return &(node->om_node);
}

/**
 *  This free fucntion will free an om_element and all the children contained in it
 *  before calling this function first free 
*/ 

axis2_status_t AXIS2_CALL axis2_om_node_free(axis2_om_node_t *om_node,axis2_env_t **env)
{

    axis2_om_node_t *child_node = NULL;
      
    AXIS2_FUNC_PARAM_CHECK(om_node,env,AXIS2_FAILURE);
    
    if ((AXIS2_INTF_TO_IMPL(om_node))->first_child)
    {   
        
        while (((axis2_om_node_impl_t*)om_node)->first_child)
        {
            
            child_node = AXIS2_OM_NODE_DETACH (((axis2_om_node_impl_t*)om_node)->first_child, env);
            AXIS2_OM_NODE_FREE ( child_node , env);
        }
    }
    switch (((axis2_om_node_impl_t*)om_node)->node_type)
    {
    case AXIS2_OM_ELEMENT:
        AXIS2_OM_ELEMENT_FREE((axis2_om_element_t*)(AXIS2_INTF_TO_IMPL(om_node)->data_element), env);
        break;

    case AXIS2_OM_COMMENT:
        /* axis2_om_comment_free(env,(axis2_om_comment_t*)node->data_element) */
        break;
    case AXIS2_OM_DOCTYPE:
        /* axis2_om_doctype_free(env,(axis2_om_doctype_t*)node->data_element) */
        break;
    case AXIS2_OM_PROCESSING_INSTRUCTION:
        /* axis2_om_prcessing_instruction_free(env,(axis2_om_processing_instruction)node->data_element)*/
        break;
    case AXIS2_OM_TEXT:
        AXIS2_OM_TEXT_FREE((axis2_om_text_t*)(AXIS2_INTF_TO_IMPL(om_node)->data_element),env);
        break;

    default:
        break;
    }

    AXIS2_FREE ((*env)->allocator, om_node->ops);
    AXIS2_FREE ((*env)->allocator, om_node);
    

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_om_node_add_child (axis2_om_node_t *om_node,
                         axis2_env_t **env,
                         axis2_om_node_t *parent)
{
    AXIS2_FUNC_PARAM_CHECK(om_node,env,AXIS2_FAILURE);

    if(!parent)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);

        return AXIS2_FAILURE;
    }

    if (AXIS2_INTF_TO_IMPL(parent)->first_child  == NULL)
    {
        AXIS2_INTF_TO_IMPL(parent)->first_child = om_node;
    }
    else
    {
       axis2_om_node_t *last_sib = NULL;
       last_sib  = AXIS2_INTF_TO_IMPL(parent)->last_child;
       if(last_sib)
           AXIS2_INTF_TO_IMPL(last_sib)->next_sibling = om_node;
        AXIS2_INTF_TO_IMPL(om_node)->prev_sibling = last_sib;
    }

    AXIS2_INTF_TO_IMPL(om_node)->parent  = parent;
    AXIS2_INTF_TO_IMPL(parent)->last_child = om_node;
    return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_node_detach (axis2_om_node_t *om_node,
                      axis2_env_t **env)
{
    axis2_om_node_t *parent = NULL;

   
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    
    parent = AXIS2_INTF_TO_IMPL(om_node)->parent;



    if ((AXIS2_INTF_TO_IMPL(om_node)->prev_sibling) == NULL)
    {
        AXIS2_INTF_TO_IMPL(om_node)->first_child = AXIS2_INTF_TO_IMPL(om_node)->next_sibling;
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
    return om_node;

}

axis2_status_t AXIS2_CALL
axis2_om_node_set_parent (axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_om_node_t * parent)
{
   
    AXIS2_FUNC_PARAM_CHECK(om_node,env,AXIS2_FAILURE);
    if(!parent)
    {

        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);

        return AXIS2_FAILURE;
    }
    
    if (parent == AXIS2_INTF_TO_IMPL(om_node)->parent)
    {   /* same parent already exist */
        return AXIS2_SUCCESS;
    }
    /* if a new parent is assigned in  place of existing 
     *  one first the node should  be detached  
     */
    if(!(AXIS2_INTF_TO_IMPL(om_node)->parent))
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
    
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(!node_to_insert)
    {

        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);

        return AXIS2_FAILURE;
    }
    
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
   
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
   
    if(!node_to_insert)
    {

        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);

        return AXIS2_FAILURE;
    }
    
    AXIS2_INTF_TO_IMPL(node_to_insert)->parent = 
        AXIS2_INTF_TO_IMPL(om_node)->parent ;
            
    AXIS2_INTF_TO_IMPL(node_to_insert)->prev_sibling = 
        AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
    AXIS2_INTF_TO_IMPL(node_to_insert)->next_sibling = om_node;
    prev_sibling = AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
    if (prev_sibling)
    {
        axis2_om_node_t *parent = AXIS2_INTF_TO_IMPL(om_node)->parent;
        AXIS2_INTF_TO_IMPL(parent)->first_child = node_to_insert;
    }
    else
    {
        axis2_om_node_t *prev_sibling = AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
        AXIS2_INTF_TO_IMPL(prev_sibling)->next_sibling = node_to_insert;
    }
    AXIS2_INTF_TO_IMPL(om_node)->prev_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}


axis2_om_node_t* AXIS2_CALL
axis2_om_node_iterator_get_first_child (axis2_om_node_t *om_node,
                               axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_FUNC_PARAM_CHECK( om_node, env, NULL);
    
    if(AXIS2_INTF_TO_IMPL(om_node)->first_child)
    {

        AXIS2_INTF_TO_IMPL(om_node)->current_child = 
            AXIS2_INTF_TO_IMPL(om_node)->first_child;
        return AXIS2_INTF_TO_IMPL(om_node)->first_child;

    }
    return NULL;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_node_iterator_get_next_child (axis2_om_node_t *om_node,
                                       axis2_env_t **env)
{
    axis2_om_node_t *node = NULL;
    axis2_om_node_t *current_child = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    AXIS2_FUNC_PARAM_CHECK(AXIS2_INTF_TO_IMPL(om_node)->first_child, env, NULL);
    if(!(AXIS2_INTF_TO_IMPL(om_node)->current_child))
    {
        (*env)->error->error_number = AXIS2_ERROR_INVALID_ITERATOR_STATE;

        return NULL;
    
    }
    current_child = AXIS2_INTF_TO_IMPL(om_node)->current_child;
    
    if(current_child && AXIS2_INTF_TO_IMPL(current_child)->next_sibling)
    {
        node = AXIS2_INTF_TO_IMPL(current_child)->next_sibling;
        AXIS2_INTF_TO_IMPL(om_node)->current_child = node;
        return node;
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_node_serialize (axis2_om_node_t *om_node,
                         axis2_env_t **env,
                         axis2_om_output_t *om_output)
{
    
    axis2_om_node_t *child_node = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);

    if(!om_output)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error ,
                                    AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error , AXIS2_FAILURE);                                    
        return AXIS2_FAILURE;
    }


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
     /*   status =
            axis2_om_text_serialize (environment,
                                     (axis2_om_text_t *) om_node->
                                     data_element, om_output);
        return status;
    */      break;
    
    default:
        break;
    }


    child_node = axis2_om_node_iterator_get_first_child ( om_node , env);

    while (child_node)
    {
        status = axis2_om_node_serialize (child_node, env, om_output);
        if (status != AXIS2_SUCCESS)
            return status;
        child_node = axis2_om_node_iterator_get_next_child (om_node, env);
    }

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

    return status;

    return AXIS2_SUCCESS;
 
}

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_parent(axis2_om_node_t *om_node,
                         axis2_env_t **env)
{
    
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->parent;
    
}
axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_first_child(axis2_om_node_t *om_node,
                              axis2_env_t **env)
{   
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->first_child;
}
        
axis2_om_node_t* AXIS2_CALL
axis2_om_node_get_last_child(axis2_om_node_t *om_node,
                             axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->last_child;

}

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_previous_sibling(axis2_om_node_t *om_node,
                                   axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->prev_sibling;
}

axis2_om_node_t* AXIS2_CALL 
axis2_om_node_get_next_sibling(axis2_om_node_t *om_node,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node)->next_sibling;
}

axis2_om_types_t AXIS2_CALL 
axis2_om_node_get_node_type(axis2_om_node_t *om_node,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_OM_INVALID);
    return AXIS2_INTF_TO_IMPL(om_node)->node_type;
}
                                 
void* AXIS2_CALL 
axis2_om_node_get_data_element(axis2_om_node_t *om_node,
                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_node);
}                                

axis2_status_t AXIS2_CALL  
axis2_om_node_set_first_child(axis2_om_node_t *om_node,axis2_env_t **env,
                              axis2_om_node_t *first_child)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(!first_child)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,AXIS2_ERROR_INVALID_NULL_PARAM);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_node)->first_child = first_child;
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL 
axis2_om_node_set_last_child(axis2_om_node_t *om_node,axis2_env_t **env,
                             axis2_om_node_t *last_child)
{
   
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(!last_child)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,AXIS2_ERROR_INVALID_NULL_PARAM);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_node)->last_child = last_child;
    return AXIS2_SUCCESS;
}
axis2_status_t AXIS2_CALL  
axis2_om_node_set_previous_sibling(axis2_om_node_t *om_node,                                  
                                   axis2_env_t **env,
                                   axis2_om_node_t *prev_sibling)
{
   
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(!prev_sibling)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,AXIS2_ERROR_INVALID_NULL_PARAM);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_node)->prev_sibling = prev_sibling;
    return AXIS2_SUCCESS;
} 
axis2_status_t AXIS2_CALL  
axis2_om_node_set_next_sibling(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_om_node_t *next_sibling)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(!next_sibling)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_node)->next_sibling = next_sibling;
    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL  
axis2_om_node_set_node_type(axis2_om_node_t *om_node,
                            axis2_env_t **env,
                            axis2_om_types_t type)
{
   
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->node_type = type;
    return AXIS2_SUCCESS;
}                            

axis2_status_t AXIS2_CALL  
axis2_om_node_set_data_element(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               void* data_element)
{                               
    
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(!data_element)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_node)->data_element = data_element;
    return AXIS2_SUCCESS;                               
}       
        
axis2_bool_t AXIS2_CALL
axis2_om_node_get_build_status(axis2_om_node_t *om_node,
                               axis2_env_t **env)
{
    /* on error what value should be returned ?...*/
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_node)->done;

}                               

axis2_status_t AXIS2_CALL
axis2_om_node_set_build_status(axis2_om_node_t *om_node,
                               axis2_env_t **env,
                               axis2_bool_t done)
{
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_node)->done = done;
    return AXIS2_SUCCESS;
}                               