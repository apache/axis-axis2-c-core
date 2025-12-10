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

#ifndef AXIS2_HTTP_SERVICE_PROVIDER_H
#define AXIS2_HTTP_SERVICE_PROVIDER_H

/**
 * @file axis2_http_service_provider.h
 * @brief Revolutionary HTTP Service Provider Interface
 *
 * This interface follows Java interface patterns (like axis2_http2_msg_recv.c)
 * to eliminate circular dependencies between HTTP transport and engine.
 *
 * ARCHITECTURAL SOLUTION:
 * - HTTP transport defines service interfaces (this file)
 * - Engine implements these interfaces
 * - HTTP transport uses callbacks without direct engine dependencies
 * - Clean separation of concerns without circular linking
 */

#include <axutil_utils.h>
#include <axutil_hash.h>
#include <axis2_conf_ctx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct axis2_http_service_provider axis2_http_service_provider_t;

/**
 * Revolutionary HTTP Service Provider Interface
 * Defines services that HTTP transport needs from the engine
 * Following Java interface pattern for clean architecture
 */
struct axis2_http_service_provider
{
    /**
     * Get all available services - replaces direct axis2_conf_get_all_svcs call
     * @param provider Service provider instance
     * @param env Environment
     * @param conf_ctx Configuration context
     * @return Hash map of services
     */
    axutil_hash_t* (*get_all_services)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        axis2_conf_ctx_t* conf_ctx);

    /**
     * Get all faulty services - replaces direct axis2_conf_get_all_faulty_svcs call
     * @param provider Service provider instance
     * @param env Environment
     * @param conf_ctx Configuration context
     * @return Hash map of faulty services
     */
    axutil_hash_t* (*get_faulty_services)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        axis2_conf_ctx_t* conf_ctx);

    /**
     * PHASE 2: Service Metadata Interface - SOAP Baggage Elimination
     * Get service name string - eliminates axis2_svc_get_qname + axutil_qname_get_localpart complexity
     * @param provider Service provider instance
     * @param env Environment
     * @param service Service object pointer
     * @return Service name string (caller must not free - managed by service)
     */
    const axis2_char_t* (*get_service_name)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        void* service);

    /**
     * Get service description - replaces direct axis2_svc_get_svc_desc call
     * @param provider Service provider instance
     * @param env Environment
     * @param service Service object pointer
     * @return Service description string (caller must not free - managed by service)
     */
    const axis2_char_t* (*get_service_description)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        void* service);

    /**
     * Get service operations - replaces direct axis2_svc_get_all_ops call
     * @param provider Service provider instance
     * @param env Environment
     * @param service Service object pointer
     * @return Hash map of operations (caller must not free - managed by service)
     */
    axutil_hash_t* (*get_service_operations)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        void* service);

    /**
     * Get service WSDL path - replaces direct axis2_svc_get_svc_wsdl_path call
     * @param provider Service provider instance
     * @param env Environment
     * @param service Service object pointer
     * @return WSDL file path (caller must not free - managed by service)
     */
    const axis2_char_t* (*get_service_wsdl_path)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        void* service);

    /**
     * Get service folder path - replaces direct axis2_svc_get_svc_folder_path call
     * @param provider Service provider instance
     * @param env Environment
     * @param service Service object pointer
     * @return Service folder path (caller must not free - managed by service)
     */
    const axis2_char_t* (*get_service_folder_path)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        void* service);

    /**
     * Get operation name string - eliminates axis2_op_get_qname + axutil_qname_get_localpart complexity
     * @param provider Service provider instance
     * @param env Environment
     * @param operation Operation object pointer
     * @return Operation name string (caller must not free - managed by operation)
     */
    const axis2_char_t* (*get_operation_name)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env,
        void* operation);

    /**
     * Free the service provider instance
     * @param provider Service provider instance
     * @param env Environment
     */
    void (*free)(
        axis2_http_service_provider_t* provider,
        const axutil_env_t* env);
};

/**
 * Revolutionary Service Provider Registry
 * HTTP transport registers its need for services here
 * Engine provides implementation when loaded
 */

/**
 * Set the HTTP service provider implementation
 * Called by engine during initialization
 * @param env Environment
 * @param provider Service provider implementation
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_http_service_provider_set_impl(
    const axutil_env_t* env,
    axis2_http_service_provider_t* provider);

/**
 * Get the HTTP service provider implementation
 * Used by HTTP transport utils
 * @param env Environment
 * @return Service provider implementation or NULL if not set
 */
AXIS2_EXTERN axis2_http_service_provider_t* AXIS2_CALL
axis2_http_service_provider_get_impl(
    const axutil_env_t* env);

/**
 * Revolutionary Engine Service Provider Factory
 * Creates engine implementation of HTTP service provider
 * Called by engine during initialization
 * @param env Environment
 * @return Engine service provider implementation
 */
AXIS2_EXTERN axis2_http_service_provider_t* AXIS2_CALL
axis2_engine_service_provider_create(
    const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_HTTP_SERVICE_PROVIDER_H */