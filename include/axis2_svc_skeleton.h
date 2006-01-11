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
  
#ifndef AXIS2_SVC_SKELETON_H
#define AXIS2_SVC_SKELETON_H

#include <axis2_om_node.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 * @file axis2_svc_skeleton.h
 * @brief Axis2c service skeleton that should be implemented by actual service
 * wrappers
 */

/** @defgroup axis2_svc_skeleton Axis2 Service Skeleton
 * @ingroup axis2_deployment
 * @{
 */    

typedef struct axis2_svc_skeleton_ops axis2_svc_skeleton_ops_t;   
typedef struct axis2_svc_skeleton axis2_svc_skeleton_t;
  
AXIS2_DECLARE_DATA struct axis2_svc_skeleton_ops
{
    int (AXIS2_CALL *
    init) (axis2_svc_skeleton_t *svc_skeleton,
            axis2_env_t **env);

    int (AXIS2_CALL * 
    free)(axis2_svc_skeleton_t *svc_skeli,
            axis2_env_t **env);
    
    axis2_om_node_t *(AXIS2_CALL* 
    invoke)(axis2_svc_skeleton_t *svc_skeli, 
            axis2_env_t **env,
            axis2_om_node_t *node);
    
    axis2_om_node_t *(AXIS2_CALL* 
    on_fault)(axis2_svc_skeleton_t *svc_skeli, 
                axis2_env_t **env,
                axis2_om_node_t *node);
    

} ;

AXIS2_DECLARE_DATA struct axis2_svc_skeleton 
{
    axis2_svc_skeleton_ops_t *ops; 
    axis2_array_list_t *func_array;
};

/**
 * Creates axis2_svc_skeleton struct
 * @return pointer to newly created axis2_svc_struct
 */
AXIS2_DECLARE(axis2_svc_skeleton_t *) 
axis2_svc_skeleton_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_SVC_SKELETON_INIT(svc_skeleton, env) \
		((svc_skeleton)->ops->init (svc_skeleton, env))

#define AXIS2_SVC_SKELETON_FREE(svc_skeleton, env) \
		((svc_skeleton)->ops->free (svc_skeleton, env))

#define AXIS2_SVC_SKELETON_INVOKE(svc_skeleton, env, node) \
		((svc_skeleton)->ops->invoke (svc_skeleton, env, node))

#define AXIS2_SVC_SKELETON_ON_FAULT(svc_skeleton, env, node) \
		((svc_skeleton)->ops->on_fault (svc_skeleton, env, node))


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SVC_SKELETON_H */
