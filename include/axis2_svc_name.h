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

#ifndef AXIS2_SVC_NAME_H
#define AXIS2_SVC_NAME_H

/**
 * @defgroup axis2_svc_name service name
 * @ingroup axis2_addr
 * @{
 */

/**
 * @file axis2_svc_name.h
 * @brief Axis2 core addressing interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_const.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_svc_name */
    typedef struct axis2_svc_name axis2_svc_name_t;
    /** Type name for struct axis2_svc_name_ops */
    typedef struct axis2_svc_name_ops axis2_svc_name_ops_t;


    /**
     * Dispatcher ops struct
     */
    struct axis2_svc_name_ops
    {
        /**
         * @param svc_name pointer to service name
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    const axis2_svc_name_t *svc_name,
                    const axis2_env_t *env);

        /**
         * @param svc_name pointer to service name
         * @param env pointer to environment struct
         * @param qname pointer to qname
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    struct axis2_svc_name *svc_name,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
         * @param svc_name pointer to service name
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_endpoint_name)(
                    const axis2_svc_name_t *svc_name,
                    const axis2_env_t *env);

        /**
         * @param svc_name pointer to service name
         * @param env pointer to environment struct
         * @param endpoint_name pointer to endpoint name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_endpoint_name)(
                    struct axis2_svc_name *svc_name,
                    const axis2_env_t *env,
                    axis2_char_t *endpoint_name);

        /**
         * @param svc_name pointer to service name
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_svc_name *svc_name,
                    const axis2_env_t *env);

    };

    /**
     * Dispatcher struct
     */
    struct axis2_svc_name
    {
        /** Dispatcher related ops */
        axis2_svc_name_ops_t *ops;
    };


    /**
     * creates svc_name struct
     */
    AXIS2_EXTERN axis2_svc_name_t *AXIS2_CALL
    axis2_svc_name_create(
        const axis2_env_t *env,
        axis2_qname_t *qname,
        axis2_char_t *endpoint_name) ;

/** Gets the qualified name.
    @sa axis2_svc_name_ops#get_qname */
#define AXIS2_SVC_NAME_GET_QNAME(svc_name, env) \
      ((svc_name)->ops->get_qname(svc_name, env))

/** Set qname.
    @sa axis2_svc_name_ops#set_qname */
#define AXIS2_SVC_NAME_SET_QNAME(svc_name, env, qname) \
      ((svc_name)->ops->set_qname(svc_name, env, qname))

/** Gets the endpoint name.
    @sa axis2_svc_name_ops#get_endpoint_name */
#define AXIS2_SVC_NAME_GET_ENDPOINT_NAME(svc_name, env) \
      ((svc_name)->ops->get_endpoint_name(svc_name, env))

/** Sets the endpoint name.
    @sa axis2_svc_name_ops#set_endpoint_name */
#define AXIS2_SVC_NAME_SET_ENDPOINT_NAME(svc_name, env, endpoint_name) \
      ((svc_name)->ops->set_endpoint_name(svc_name, env, endpoint_name))

/** Frees the service name.
    @sa axis2_svc_name_ops#free */
#define AXIS2_SVC_NAME_FREE(svc_name, env) \
      ((svc_name)->ops->free(svc_name, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_SVC_NAME_H */
