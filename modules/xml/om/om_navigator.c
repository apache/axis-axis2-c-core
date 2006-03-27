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

#include <axis2_om_navigator.h>

/*************************** function prototypes ******************************************/
axis2_status_t AXIS2_CALL
axis2_om_navigator_free(axis2_om_navigator_t *om__navigator,
                        axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_om_navigator_is_navigable(axis2_om_navigator_t *om_navigator,
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_om_navigator_is_completed(axis2_om_navigator_t *om_navigator,
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_om_navigator_visited(axis2_om_navigator_t *om_navigator,
                           axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL
axis2_om_navigator_next(axis2_om_navigator_t *om_navigator,
                        axis2_env_t **env);
                
static void 
axis2_om_navigator_update_next_node(axis2_om_navigator_t *om_navigator,
                        			axis2_env_t **env);
/************************************************************************************/

typedef struct axis2_om_navigator_impl
{
    axis2_om_navigator_t navigator;

    axis2_om_node_t *node;

    axis2_bool_t visited;

    axis2_om_node_t *next;

    axis2_om_node_t *root;

    axis2_bool_t backtracked;
    
    axis2_bool_t end;

    axis2_bool_t start;
     
}axis2_om_navigator_impl_t;


/****************************** Macro  ***************************************/

#define AXIS2_INTF_TO_IMPL(navigator) ((axis2_om_navigator_impl_t *)navigator)
                               
/*****************************************************************************/

AXIS2_DECLARE(axis2_om_navigator_t *)
axis2_om_navigator_create(axis2_env_t **env, 
                          axis2_om_node_t *om_node)
{
    axis2_om_navigator_impl_t *navigator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, om_node, NULL);

    navigator_impl = (axis2_om_navigator_impl_t *)
                     AXIS2_MALLOC((*env)->allocator, sizeof(axis2_om_navigator_impl_t));
    if(!navigator_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    navigator_impl->node = NULL;
    navigator_impl->visited = AXIS2_FALSE;
    navigator_impl->next = NULL;
    navigator_impl->root = NULL;
    navigator_impl->end = AXIS2_FALSE;
    navigator_impl->start = AXIS2_TRUE;
    navigator_impl->backtracked = AXIS2_FALSE;

    navigator_impl->next = om_node;
    navigator_impl->root = om_node;
    
    navigator_impl->navigator.ops = (axis2_om_navigator_ops_t*)AXIS2_MALLOC(
                            (*env)->allocator, sizeof(axis2_om_navigator_ops_t));

    if(!(navigator_impl->navigator.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, navigator_impl);
        navigator_impl = NULL;
        return NULL;
    };

    navigator_impl->navigator.ops->free =
        axis2_om_navigator_free;

    navigator_impl->navigator.ops->is_navigable =
        axis2_om_navigator_is_navigable;

    navigator_impl->navigator.ops->is_completed =
        axis2_om_navigator_is_completed;

    navigator_impl->navigator.ops->visited =
        axis2_om_navigator_visited;

    navigator_impl->navigator.ops->next =
        axis2_om_navigator_next;

    return &(navigator_impl->navigator);        
}                          


axis2_status_t AXIS2_CALL
axis2_om_navigator_free(axis2_om_navigator_t *om_navigator,
                       axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env , AXIS2_FAILURE);

    if(NULL != om_navigator->ops)
    {
        AXIS2_FREE((*env)->allocator, om_navigator->ops);
        om_navigator->ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(om_navigator));
    om_navigator = NULL;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_om_navigator_is_navigable(axis2_om_navigator_t *om_navigator,
                                axis2_env_t **env)
{
	axis2_om_navigator_impl_t *navigator_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FALSE);
	
	navigator_impl = AXIS2_INTF_TO_IMPL(om_navigator);
	if(AXIS2_TRUE == navigator_impl->end )
		return AXIS2_FALSE;
	else
	{
		if(NULL != navigator_impl->next)
			return AXIS2_TRUE;
	}		
	return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL
axis2_om_navigator_is_completed(axis2_om_navigator_t *om_navigator,
                                axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, AXIS2_FALSE);
	return AXIS2_INTF_TO_IMPL(om_navigator)->end;	
}


axis2_bool_t AXIS2_CALL
axis2_om_navigator_visited(axis2_om_navigator_t *om_navigator,
                           axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, AXIS2_FALSE);
	return AXIS2_INTF_TO_IMPL(om_navigator)->visited;	
}


axis2_om_node_t* AXIS2_CALL
axis2_om_navigator_next(axis2_om_navigator_t *om_navigator,
                        axis2_env_t **env)
{
	axis2_om_navigator_impl_t *navigator_impl = NULL;
	AXIS2_ENV_CHECK(env, NULL);
	
	navigator_impl = AXIS2_INTF_TO_IMPL(om_navigator);
	
	if(NULL == navigator_impl->next)
		return NULL;
	
	navigator_impl->node = navigator_impl->next;
	navigator_impl->visited = navigator_impl->backtracked;
	navigator_impl->backtracked = AXIS2_FALSE;
	
	axis2_om_navigator_update_next_node(om_navigator, env);
	
	
	/** set the starting and ending flags */
	if(navigator_impl->root == navigator_impl->next)
	{
		if(!(navigator_impl->start))
		{
			navigator_impl->end = AXIS2_TRUE;
		}
		else
		{
			navigator_impl->start = AXIS2_FALSE;
		}			
	}
	return navigator_impl->node;	
}
/** this method encapsulate searching logic */
static void 
axis2_om_navigator_update_next_node(axis2_om_navigator_t *om_navigator,
                        			axis2_env_t **env)
{
	axis2_om_navigator_impl_t *navigator_impl = NULL;
	if(!om_navigator)
		return;
	
	navigator_impl = AXIS2_INTF_TO_IMPL(om_navigator);
	
	if(!navigator_impl->next)
		return;		
	
	if((AXIS2_OM_ELEMENT == AXIS2_OM_NODE_GET_NODE_TYPE(navigator_impl->next, env)) &&
		!(navigator_impl->visited))
	{
		if(NULL != AXIS2_OM_NODE_GET_FIRST_CHILD(navigator_impl->next, env))	
		{
			navigator_impl->next = 	AXIS2_OM_NODE_GET_FIRST_CHILD(navigator_impl->next, env);
		}
		else if(AXIS2_TRUE == AXIS2_OM_NODE_GET_BUILD_STATUS(navigator_impl->next, env))
		{
			navigator_impl->backtracked = AXIS2_TRUE;				
		}			
		else
		{
			navigator_impl->next = NULL;
		}			
	}
	else
	{
		axis2_om_node_t  *parent = NULL;
		axis2_om_node_t *next_sibling = NULL;
		
		next_sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(navigator_impl->next, env);
		
		parent = AXIS2_OM_NODE_GET_PARENT(navigator_impl->next, env);	
		
		if(NULL != next_sibling)
		{
			navigator_impl->next = next_sibling;	
		}			
		else if((NULL != parent) && AXIS2_OM_NODE_GET_BUILD_STATUS(parent, env))
		{
			navigator_impl->next = parent;
			navigator_impl->backtracked = AXIS2_TRUE;	
		}			
		else
		{
			navigator_impl->next = NULL;	
		}			
	}
}
