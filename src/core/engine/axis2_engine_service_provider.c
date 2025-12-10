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

/**
 * @file axis2_engine_service_provider.c
 * @brief Engine Implementation of HTTP Service Provider Interface
 *
 * Following axis2_http2_msg_recv.c Java interface pattern to eliminate
 * circular dependency between HTTP transport and engine.
 *
 * ARCHITECTURAL SOLUTION:
 * - Engine implements HTTP service provider interface
 * - Engine registers itself with HTTP transport at startup
 * - HTTP transport uses interface without direct engine linking
 */

#include <axis2_http_service_provider.h>
#include <axis2_conf.h>
#include <axis2_conf_ctx.h>
#include <axutil_hash.h>

struct axis2_engine_service_provider
{
    axis2_http_service_provider_t base;
};

typedef struct axis2_engine_service_provider axis2_engine_service_provider_t;

/* Revolutionary Service Provider Implementation */

static axutil_hash_t* AXIS2_CALL
axis2_engine_service_provider_get_all_services(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    axis2_conf_ctx_t* conf_ctx)
{
    axis2_conf_t* conf = NULL;

    if (!conf_ctx) {
        return NULL;
    }

    conf = axis2_conf_ctx_get_conf(conf_ctx, env);
    if (!conf) {
        return NULL;
    }

    /* Revolutionary: Direct call to engine function, but contained within engine */
    return axis2_conf_get_all_svcs(conf, env);
}

static axutil_hash_t* AXIS2_CALL
axis2_engine_service_provider_get_faulty_services(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    axis2_conf_ctx_t* conf_ctx)
{
    axis2_conf_t* conf = NULL;

    if (!conf_ctx) {
        return NULL;
    }

    conf = axis2_conf_ctx_get_conf(conf_ctx, env);
    if (!conf) {
        return NULL;
    }

    /* Revolutionary: Direct call to engine function, but contained within engine */
    return axis2_conf_get_all_faulty_svcs(conf, env);
}

/* PHASE 2: Service Metadata Interface Implementations - SOAP Baggage Elimination */

static const axis2_char_t* AXIS2_CALL
axis2_engine_service_provider_get_service_name(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    void* service)
{
    axutil_qname_t* qname = NULL;

    if (!service) {
        return NULL;
    }

    /* Engine handles SOAP complexity: QName → simple string */
    qname = axis2_svc_get_qname((axis2_svc_t*)service, env);
    if (!qname) {
        return NULL;
    }

    /* Extract simple service name, eliminating SOAP baggage from HTTP transport */
    return axutil_qname_get_localpart(qname, env);
}

static const axis2_char_t* AXIS2_CALL
axis2_engine_service_provider_get_service_description(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    void* service)
{
    if (!service) {
        return NULL;
    }

    /* Direct service description access - no SOAP complexity */
    return axis2_svc_get_svc_desc((axis2_svc_t*)service, env);
}

static axutil_hash_t* AXIS2_CALL
axis2_engine_service_provider_get_service_operations(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    void* service)
{
    if (!service) {
        return NULL;
    }

    /* Direct operations access - engine manages complexity */
    return axis2_svc_get_all_ops((axis2_svc_t*)service, env);
}

static const axis2_char_t* AXIS2_CALL
axis2_engine_service_provider_get_service_wsdl_path(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    void* service)
{
    if (!service) {
        return NULL;
    }

    /* Direct WSDL path access */
    return axis2_svc_get_svc_wsdl_path((axis2_svc_t*)service, env);
}

static const axis2_char_t* AXIS2_CALL
axis2_engine_service_provider_get_service_folder_path(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    void* service)
{
    if (!service) {
        return NULL;
    }

    /* Direct service folder path access */
    return axis2_svc_get_svc_folder_path((axis2_svc_t*)service, env);
}

static const axis2_char_t* AXIS2_CALL
axis2_engine_service_provider_get_operation_name(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env,
    void* operation)
{
    axutil_qname_t* qname = NULL;

    if (!operation) {
        return NULL;
    }

    /* Engine handles SOAP complexity: Operation QName → simple string */
    qname = axis2_op_get_qname((axis2_op_t*)operation, env);
    if (!qname) {
        return NULL;
    }

    /* Extract simple operation name, eliminating SOAP baggage from HTTP transport */
    return axutil_qname_get_localpart(qname, env);
}

static void AXIS2_CALL
axis2_engine_service_provider_free(
    axis2_http_service_provider_t* provider,
    const axutil_env_t* env)
{
    axis2_engine_service_provider_t* engine_provider = NULL;

    if (!provider) {
        return;
    }

    engine_provider = (axis2_engine_service_provider_t*)provider;
    AXIS2_FREE(env->allocator, engine_provider);
}

/* Revolutionary Factory Function */
AXIS2_EXTERN axis2_http_service_provider_t* AXIS2_CALL
axis2_engine_service_provider_create(const axutil_env_t* env)
{
    axis2_engine_service_provider_t* engine_provider = NULL;

    engine_provider = (axis2_engine_service_provider_t*)AXIS2_MALLOC(
        env->allocator, sizeof(axis2_engine_service_provider_t));

    if (!engine_provider) {
        return NULL;
    }

    /* Set up interface implementation */
    engine_provider->base.get_all_services = axis2_engine_service_provider_get_all_services;
    engine_provider->base.get_faulty_services = axis2_engine_service_provider_get_faulty_services;

    /* PHASE 2: Service Metadata Interface - SOAP Baggage Elimination */
    engine_provider->base.get_service_name = axis2_engine_service_provider_get_service_name;
    engine_provider->base.get_service_description = axis2_engine_service_provider_get_service_description;
    engine_provider->base.get_service_operations = axis2_engine_service_provider_get_service_operations;
    engine_provider->base.get_service_wsdl_path = axis2_engine_service_provider_get_service_wsdl_path;
    engine_provider->base.get_service_folder_path = axis2_engine_service_provider_get_service_folder_path;
    engine_provider->base.get_operation_name = axis2_engine_service_provider_get_operation_name;

    engine_provider->base.free = axis2_engine_service_provider_free;

    return &(engine_provider->base);
}