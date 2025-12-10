/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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

/*
 * Surgical Conditional Compilation - Service Skeleton Support
 *
 * HTTP/2 JSON mode: Minimal definitions for compatibility
 * HTTP/1.1 SOAP mode: Full skeleton support with axiom
 */

#ifndef WITH_NGHTTP2
/* HTTP/1.1 SOAP mode - Full service skeleton support */
#include <axiom_node.h>
#include <axutil_array_list.h>
#include <axis2_msg_ctx.h>
#else
/* HTTP/2 JSON mode - Minimal compatibility definitions */
#include <axis2_const.h>
#include <axis2_defines.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WITH_NGHTTP2
/* HTTP/1.1 SOAP mode - Full service skeleton definitions */

    /** Type name for struct axis2_svc_skeleton_ops */
    typedef struct axis2_svc_skeleton_ops axis2_svc_skeleton_ops_t;

    /** Type name for struct axis2_svc_skeleton */
    typedef struct axis2_svc_skeleton axis2_svc_skeleton_t;

    /**
     * Service skeleton operations struct
     */
    struct axis2_svc_skeleton_ops
    {
        /**
         * Invoke method
         */
        axiom_node_t* (AXIS2_CALL *invoke)(
            axis2_svc_skeleton_t *svc_skeleton,
            const axutil_env_t *env,
            axiom_node_t *node,
            axis2_msg_ctx_t *msg_ctx);

        /**
         * On fault method
         */
        axiom_node_t* (AXIS2_CALL *on_fault)(
            axis2_svc_skeleton_t *svc_skeleton,
            const axutil_env_t *env,
            axiom_node_t *node);

        /**
         * Free method
         */
        int (AXIS2_CALL *free)(
            axis2_svc_skeleton_t *svc_skeleton,
            const axutil_env_t *env);

        /**
         * Init method
         */
        int (AXIS2_CALL *init)(
            axis2_svc_skeleton_t *svc_skeleton,
            const axutil_env_t *env);
    };

    /**
     * Service skeleton struct
     */
    struct axis2_svc_skeleton
    {
        /** operations of service skeleton */
        axis2_svc_skeleton_ops_t *ops;

        /** function array for operations */
        axutil_array_list_t *func_array;
    };

    /** Macro to invoke service skeleton */
    #define AXIS2_SVC_SKELETON_INVOKE(svc_skeleton, env, node, msg_ctx) \
        ((svc_skeleton)->ops->invoke(svc_skeleton, env, node, msg_ctx))

    /** Macro to call on_fault on service skeleton */
    #define AXIS2_SVC_SKELETON_ON_FAULT(svc_skeleton, env, node) \
        ((svc_skeleton)->ops->on_fault(svc_skeleton, env, node))

    /** Macro to free service skeleton */
    #define AXIS2_SVC_SKELETON_FREE(svc_skeleton, env) \
        ((svc_skeleton)->ops->free(svc_skeleton, env))

#else
/* HTTP/2 JSON mode - Minimal compatibility definitions */

    /** Stub type definition for HTTP/2 JSON compatibility */
    typedef void axis2_svc_skeleton_t;

    /** Stub macros for HTTP/2 JSON compatibility */
    #define AXIS2_SVC_SKELETON_INVOKE(svc_skeleton, env, node, msg_ctx) NULL
    #define AXIS2_SVC_SKELETON_ON_FAULT(svc_skeleton, env, node) NULL
    #define AXIS2_SVC_SKELETON_FREE(svc_skeleton, env) AXIS2_SUCCESS

#endif /* WITH_NGHTTP2 */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SVC_SKELETON_H */