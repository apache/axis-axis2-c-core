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
#include "axis2_svc_skeleton.h"
#include "notify.h"
#include <axis2_array_list.h>

int AXIS2_CALL
notify_free(axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env);

axis2_status_t AXIS2_CALL
notify_free_void_arg(void *svc_skeleton,
                    axis2_env_t **env);

/*
 * This method invokes the right service method 
 */
axis2_om_node_t* AXIS2_CALL 
notify_invoke(axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env,
            axis2_om_node_t *node);
            

int AXIS2_CALL 
notify_init(axis2_svc_skeleton_t *svc_skeleton,
          axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL
notify_on_fault(axis2_svc_skeleton_t *svc_skeli, 
              axis2_env_t **env, axis2_om_node_t *node);

/*Create function */
axis2_svc_skeleton_t *
axis2_notify_create(axis2_env_t **env)
{
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    /* Allocate memory for the structs */
    svc_skeleton = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_svc_skeleton_t));

    svc_skeleton->ops = AXIS2_MALLOC(
        (*env)->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->func_array = NULL;

    /* Assign function pointers */
    svc_skeleton->ops->free = notify_free;
    svc_skeleton->ops->init = notify_init;
    svc_skeleton->ops->invoke = notify_invoke;
    svc_skeleton->ops->on_fault = notify_on_fault;

    return svc_skeleton;
}

/* Initialize the service */
int AXIS2_CALL
notify_init(axis2_svc_skeleton_t *svc_skeleton,
                        axis2_env_t **env)
{
    svc_skeleton->func_array = axis2_array_list_create(env, 0);
    /* Add the implemented operation names of the service to  
     * the array list of functions 
     */
    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "notify");
    /* Any initialization stuff of notify service should go here */
    return AXIS2_SUCCESS;
}

/*
 * This method invokes the right service method 
 */
axis2_om_node_t* AXIS2_CALL
notify_invoke(axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env,
            axis2_om_node_t *node)
{
    /* Invoke the business logic.
     * Depending on the function name invoke the correct impl method.
     * We have only notify in this sample, hence invoke notify method.
     * To see how to deal with multiple impl methods, have a look at the
     * math sample.
     */
    axis2_notify_notify(env, node);
    return NULL;
}

/* On fault, handle the fault */
axis2_om_node_t* AXIS2_CALL
notify_on_fault(axis2_svc_skeleton_t *svc_skeli, 
              axis2_env_t **env, axis2_om_node_t *node)
{
   /* Here we are just setting a simple error message inside an element 
    * called 'EchoServiceError' 
    */
    axis2_om_node_t *error_node = NULL;
    axis2_om_node_t* text_node = NULL;
    axis2_om_element_t *error_ele = NULL;
    error_ele = axis2_om_element_create(env, node, "EchoServiceError", NULL, 
        &error_node);
    AXIS2_OM_ELEMENT_SET_TEXT(error_ele, env, "Echo service failed ", 
        text_node);
    return error_node;
}

/* Free the resources used */
int AXIS2_CALL
notify_free(axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env)
{
    /* Free the function array */
    if(svc_skeleton->func_array)
    {
        AXIS2_ARRAY_LIST_FREE(svc_skeleton->func_array, env);
        svc_skeleton->func_array = NULL;
    }
    
    /* Free the function array */
    if(svc_skeleton->ops)
    {
        AXIS2_FREE((*env)->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }
    
    /* Free the service skeleton */
    if(svc_skeleton)
    {
        AXIS2_FREE((*env)->allocator, svc_skeleton);
        svc_skeleton = NULL;
    }

    return AXIS2_SUCCESS; 
}


/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int 
axis2_get_instance(axis2_svc_skeleton_t **inst,
                   axis2_env_t **env)
{
	*inst = axis2_notify_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int 
axis2_remove_instance(axis2_svc_skeleton_t *inst,
                      axis2_env_t **env)
{
    axis2_status_t status = AXIS2_FAILURE;
	if (inst)
	{
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}

