
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_TCP_WORKER_H
#define AXIS2_TCP_WORKER_H

/**
 * @defgroup axis2_tcp_worker tcp worker
 * @ingroup axis2_core_trans_tcp
 * @{
 */

/**
 * @file axis2_tcp_worker.h
 * @brief axis2 TCP Worker
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axis2_simple_tcp_svr_conn.h>
#include <axis2_conf_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_tcp_worker */
    typedef struct axis2_tcp_worker axis2_tcp_worker_t;

    /**
     * @param tcp_worker pointer to tcp worker
     * @param env pointer to environment struct
     * @param svr_conn pointer to svr conn
     * @param simple_request pointer to simple request
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_tcp_worker_process_request(
        axis2_tcp_worker_t * tcp_worker,
        const axutil_env_t * env,
        axis2_simple_tcp_svr_conn_t * svr_conn,
        axis2_char_t * simple_request);

    /**
     * @param tcp_worker pointer to tcp worker
     * @param env pointer to environment struct
     * @param port
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_tcp_worker_set_svr_port(
        axis2_tcp_worker_t * tcp_worker,
        const axutil_env_t * env,
        int port);

    /**
     * @param tcp_worker pointer to tcp worker
     * @param env pointer to environment strut
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_tcp_worker_free(
        axis2_tcp_worker_t * tcp_worker,
        const axutil_env_t * env);

    /**
     * @param env pointer to environment struct
     * @param conf_ctx pointer to configuration context
     */
    AXIS2_EXTERN axis2_tcp_worker_t *AXIS2_CALL
    axis2_tcp_worker_create(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_TCP_WORKER_H */
