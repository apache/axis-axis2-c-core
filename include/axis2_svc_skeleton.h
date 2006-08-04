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

/**
 * @defgroup axis2_svc_api service API
 * @ingroup axis2
 * @{
 * @} 
 */

/** @defgroup axis2_svc_skeleton service skeleton
 * @ingroup axis2_svc_api
 * Description
 * @{
 */

/**
 * @file axis2_svc_skeleton.h
 * @brief Axis2c service skeleton that should be implemented by actual service
 * wrappers
 */

#include <axiom_node.h>
#include <axis2_array_list.h>
#include <axis2_msg_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif


    /** Type name for struct axis2_svc_skeleton_ops */
    typedef struct axis2_svc_skeleton_ops axis2_svc_skeleton_ops_t;
    /** Type name for struct axis2_svc_skeleton */
    typedef struct axis2_svc_skeleton axis2_svc_skeleton_t;
   
    /**
     *service skeleton ops struct
     * struct for 
     * Encapsulator struct for operations of axis2_svc_skeleton
     */
    AXIS2_DECLARE_DATA struct axis2_svc_skeleton_ops
    {

        /*
    * @param svc_skeleton pointer to svc_skeleton struct
    * @param env pointer to environment struct
    */
        int (AXIS2_CALL *
                init)(
                    axis2_svc_skeleton_t *svc_skeleton,
                    const axis2_env_t *env);

        /*
    * @param svc_skeli pointer to svc_skeli struct
    * @param env pointer to environment struct
    */
        int (AXIS2_CALL *
                free)(
                    axis2_svc_skeleton_t *svc_skeli,
                    const axis2_env_t *env);

        /*
    * @param svc_skeli pointer to svc_skeli struct
    * @param env pointer to environment struct
    */
        int (AXIS2_CALL *
                free_void_arg)(
                    void *svc_skeli,
                    const axis2_env_t *env);

        /*
    * @param svc_skeli pointer to svc_skeli struct
    * @param env pointer to environment struct
    * @param node pointer to node struct
    * @param msg_ctx pointer to message context struct
    */
        axiom_node_t *(AXIS2_CALL*
                invoke)(
                    axis2_svc_skeleton_t *svc_skeli,
                    const axis2_env_t *env,
                    axiom_node_t *node,
                    axis2_msg_ctx_t *msg_ctx);

        /*
    * @param svc_skeli pointer to svc_skeli struct
    * @param env pointer to environment struct
    * @param node pointer to node struct
    */
        axiom_node_t *(AXIS2_CALL*
                on_fault)(
                    axis2_svc_skeleton_t *svc_skeli,
                    const axis2_env_t *env,
                    axiom_node_t *node);


    } ;

    /**
     *service skeleton struct
     * struct for 
     * @param ops pointer to ops struct
     * @param func_array pointer to func_array struct
     */
    AXIS2_DECLARE_DATA struct axis2_svc_skeleton
    {
        axis2_svc_skeleton_ops_t *ops;
        axis2_array_list_t *func_array;
    };

    /**
     * Creates axis2_svc_skeleton struct
     * @param env pointer to environment struct
     * @return pointer to newly created axis2_svc_struct
     */
    AXIS2_EXTERN axis2_svc_skeleton_t * AXIS2_CALL
    axis2_svc_skeleton_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

/** Initialize the svc skeleton.
    @sa axis2_svc_skeleton_ops#init */
#define AXIS2_SVC_SKELETON_INIT(svc_skeleton, env) \
      ((svc_skeleton)->ops->init (svc_skeleton, env))

/** Frees the svc skeleton.
    @sa axis2_svc_skeleton_ops#free */
#define AXIS2_SVC_SKELETON_FREE(svc_skeleton, env) \
      ((svc_skeleton)->ops->free (svc_skeleton, env))

/** Invokes axis2 svc skeleton.
    @sa axis2_svc_skeleton_ops#invoke */
#define AXIS2_SVC_SKELETON_INVOKE(svc_skeleton, env, node, msg_ctx) \
      ((svc_skeleton)->ops->invoke (svc_skeleton, env, node, msg_ctx))

/** axis2_svc_skeleton on fault.
    @sa axis2_svc_skeleton_ops#on_fault */
#define AXIS2_SVC_SKELETON_ON_FAULT(svc_skeleton, env, node) \
      ((svc_skeleton)->ops->on_fault (svc_skeleton, env, node))

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SVC_SKELETON_H */
