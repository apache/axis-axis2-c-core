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
#include "echo.h"
#include <axis2_array_list.h>

int 
echo_free(axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env);

/*
 * This method invokes the right service method 
 */
axis2_om_node_t *echo_invoke(axis2_svc_skeleton_t *svc_skeleton,
                                    axis2_env_t **env,
                                    axis2_om_node_t *node);

int echo_init(axis2_svc_skeleton_t *svc_skeleton,
                        axis2_env_t **env);


axis2_svc_skeleton_t *
axis2_echo_create(axis2_env_t **env)
{
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    svc_skeleton = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_svc_skeleton_t));

    
    svc_skeleton->ops = AXIS2_MALLOC(
        (*env)->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->ops->free = echo_free;
    svc_skeleton->ops->init = echo_init;
    svc_skeleton->ops->invoke = echo_invoke;
    /*svc_skeleton->ops->on_fault = echo_on_fault;*/

    return svc_skeleton;
}

int echo_init(axis2_svc_skeleton_t *svc_skeleton,
                        axis2_env_t **env)
{
    svc_skeleton->func_array = axis2_array_list_create(env, 0);
    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "echo");

    /* Any initialization stuff of echo goes here */
    return AXIS2_SUCCESS;
}

int 
echo_free(axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env)
{
    if(svc_skeleton->ops)
    {
        AXIS2_FREE((*env)->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }

    if(svc_skeleton->func_array)
    {
        AXIS2_ARRAY_LIST_FREE(svc_skeleton->func_array, env);
        svc_skeleton->func_array = NULL;
    }
    
    if(svc_skeleton)
    {
        AXIS2_FREE((*env)->allocator, svc_skeleton);
        svc_skeleton = NULL;
    }
    return AXIS2_SUCCESS; 
}

/*
 * This method invokes the right service method 
 */
axis2_om_node_t *echo_invoke(axis2_svc_skeleton_t *svc_skeleton,
                                    axis2_env_t **env,
                                    axis2_om_node_t *node)
{
    /* Depending on the function name invoke the
     *  corresponding echo method
     */
        
    return echo(node);
}

/**
 * Following block distinguish the exposed part of the dll.
 */

int axis2_get_instance(struct axis2_svc_skeleton **inst,
                        axis2_env_t **env)
{
	*inst = axis2_echo_create(env);
    /*if(NULL != *inst)
    {
        status = *inst->init();
    }*/
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

int axis2_remove_instance(axis2_svc_skeleton_t *inst,
                            axis2_env_t **env)
{
    axis2_status_t status = AXIS2_FAILURE;
	if (inst)
	{
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}

