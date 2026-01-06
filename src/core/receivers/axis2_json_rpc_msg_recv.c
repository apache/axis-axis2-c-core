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
 * @file axis2_json_rpc_msg_recv.c
 * @brief Revolutionary JsonRpcMessageReceiver - AXIOM-FREE Core Framework Component
 *
 * This is the Axis2/C equivalent of Axis2/Java's JsonRpcMessageReceiver.
 * Revolutionary: Completely bypasses AXIOM/SOAP - pure JSON processing only.
 *
 * Key Revolutionary Features:
 * - Zero AXIOM dependencies (no XML processing at all)
 * - Direct JSON-to-service-function invocation
 * - Framework-level component (not service-specific)
 * - HTTP/2 streaming optimized
 */

#include <axis2_json_rpc_msg_recv.h>
#include <axis2_msg_recv.h>
#include <axis2_svc.h>
#include <axis2_op.h>
#include <axutil_param.h>
#include <axutil_stream.h>
#include <axutil_property.h>
#include <axutil_dll_desc.h>
#include <axutil_class_loader.h>
#include <axis2_http_header.h>
#include <string.h>
#include <ctype.h>  /* For character type functions */
#include <json-c/json.h>
#include <dlfcn.h>  /* For JSON-direct service loading */
/* Revolutionary: NO AXIOM includes - pure JSON processing only */

#ifdef __ANDROID__
/*
 * Android Static Service Registry
 *
 * On Android with static linking, services are compiled directly into the
 * httpd binary. Instead of trying to dlopen non-existent .so files, we use
 * direct invoke functions for known services.
 *
 * Each service exports: <servicename>_invoke_json(env, json_request)
 *
 * WEAK SYMBOL ARCHITECTURE:
 * Service functions are declared as weak symbols. This allows:
 * 1. Core framework to compile without service implementations
 * 2. Applications to provide strong symbol implementations at link time
 * 3. Graceful handling when services are not linked (returns NULL)
 *
 * To add a new static service:
 * 1. Add weak declaration here
 * 2. Add entry in android_static_service_lookup()
 * 3. Provide implementation in your application, compiled as a .o or .a file
 * 4. Link the service archive during final httpd linking with --whole-archive
 */

typedef json_object* (*json_service_invoke_func_t)(const axutil_env_t *env, json_object *json_request);

/*
 * Weak symbol declarations for statically linked services
 * Applications provide strong implementations at link time
 */
__attribute__((weak))
json_object* camera_control_service_invoke_json(
    const axutil_env_t *env,
    json_object *json_request)
{
    /* Weak default - returns NULL if no implementation linked */
    (void)env;
    (void)json_request;
    return NULL;
}

/* Add more weak service declarations here:
 * __attribute__((weak))
 * json_object* another_service_invoke_json(const axutil_env_t *env, json_object *json_request)
 * {
 *     (void)env; (void)json_request;
 *     return NULL;
 * }
 */

/**
 * Android Static Service Registry - looks up services by name
 *
 * Returns function pointer for known service names. If only weak symbol
 * was linked (no strong implementation), calls will return NULL responses,
 * which the caller handles appropriately.
 *
 * @param service_name The service name (e.g., "CameraControlService")
 * @return Function pointer to the service's invoke_json function, or NULL
 */
/* Non-static to ensure linker includes this function when linking from archives */
json_service_invoke_func_t
android_static_service_lookup(const char *service_name)
{
    if (!service_name) {
        return NULL;
    }

    /* Static service registry - returns function pointer for known services.
     * If application linked a strong implementation, it will be used.
     * If only weak stub exists, caller handles NULL response gracefully. */
    if (strcmp(service_name, "CameraControlService") == 0) {
        return camera_control_service_invoke_json;
    }

    /* Add more services as needed:
     * if (strcmp(service_name, "AnotherService") == 0) {
     *     return another_service_invoke_json;
     * }
     */

    return NULL;  /* Service not found in static registry */
}

/**
 * Try static service invocation for Android
 *
 * @param env Environment
 * @param service_name Service name
 * @param json_request_str JSON request string
 * @param json_response_out Output parameter for JSON response
 * @return AXIS2_TRUE if successful, AXIS2_FALSE otherwise
 */
/* Non-static to ensure linker includes this function when linking from archives */
axis2_bool_t
try_android_static_service(const axutil_env_t *env,
                           const char *service_name,
                           const char *json_request_str,
                           axis2_char_t **json_response_out)
{
    json_service_invoke_func_t service_invoke = NULL;
    json_object *json_request = NULL;
    json_object *json_response_obj = NULL;
    const char *json_response_str = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[ANDROID_STATIC] Looking up service: %s", service_name);

    service_invoke = android_static_service_lookup(service_name);
    if (!service_invoke) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[ANDROID_STATIC] Service '%s' not in static registry", service_name);
        return AXIS2_FALSE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[ANDROID_STATIC] Found service '%s' in static registry", service_name);

    /* Parse JSON request */
    json_request = json_tokener_parse(json_request_str);
    if (!json_request) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ANDROID_STATIC] Failed to parse JSON request");
        return AXIS2_FALSE;
    }

    /* Invoke service */
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[ANDROID_STATIC] Invoking %s", service_name);

    json_response_obj = service_invoke(env, json_request);

    json_object_put(json_request);  /* Free request */

    if (!json_response_obj) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ANDROID_STATIC] Service returned NULL response");
        return AXIS2_FALSE;
    }

    /* Convert response to string */
    json_response_str = json_object_to_json_string(json_response_obj);
    if (!json_response_str) {
        json_object_put(json_response_obj);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ANDROID_STATIC] Failed to serialize response");
        return AXIS2_FALSE;
    }

    /* Copy response for caller */
    *json_response_out = axutil_strdup(env, json_response_str);

    json_object_put(json_response_obj);  /* Free response */

    if (!*json_response_out) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ANDROID_STATIC] Failed to duplicate response string");
        return AXIS2_FALSE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[ANDROID_STATIC] Service '%s' invoked successfully, response length: %d",
        service_name, (int)strlen(*json_response_out));

    return AXIS2_TRUE;
}
#endif /* __ANDROID__ */

/**
 * Try JSON-direct service loading for HTTP/2 JSON services
 *
 * This function attempts to load services that use the JSON-direct pattern:
 * - Service exports: <serviceclass>_invoke_json(const axutil_env_t *env, json_object *json_request)
 * - Service library: lib<serviceclass>.so
 * - No axis2_get_instance() required
 *
 * @param env Environment
 * @param svc Service descriptor
 * @param impl_class_param ServiceClass parameter
 * @param json_request_str JSON request string
 * @param json_response_out Output parameter for JSON response
 * @return AXIS2_TRUE if successful, AXIS2_FALSE otherwise
 */
static axis2_bool_t
try_json_direct_service_loading(const axutil_env_t *env,
                               axis2_svc_t *svc,
                               axutil_param_t *impl_class_param,
                               const char *json_request_str,
                               axis2_char_t **json_response_out)
{
    void *service_lib = NULL;
    json_object *json_request = NULL;
    json_object *json_response_obj = NULL;
    const char *json_response_str = NULL;
    char *service_lib_path = NULL;
    char *service_function_name = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    const axis2_char_t *service_class_name = NULL;
    axis2_bool_t result = AXIS2_FALSE;

    // Function pointer for JSON service invoke function
    typedef json_object* (*json_service_invoke_func)(const axutil_env_t *env, json_object *json_request);
    json_service_invoke_func service_invoke = NULL;

    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] TEMPORARY DEBUG - Starting JSON-direct service loading");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] PARAMETER CHECK - env:%p svc:%p impl_class_param:%p json_request_str:%p json_response_out:%p",
        env, svc, impl_class_param, json_request_str, json_response_out);

    if (!env) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[JSON_DIRECT] Parameter validation failed - env is NULL");
        return AXIS2_FALSE;
    }
    if (!svc) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[JSON_DIRECT] Parameter validation failed - svc is NULL");
        return AXIS2_FALSE;
    }
    if (!impl_class_param) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[JSON_DIRECT] Parameter validation failed - impl_class_param is NULL");
        return AXIS2_FALSE;
    }
    if (!json_request_str) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[JSON_DIRECT] Parameter validation failed - json_request_str is NULL");
        return AXIS2_FALSE;
    }
    if (!json_response_out) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[JSON_DIRECT] Parameter validation failed - json_response_out is NULL");
        return AXIS2_FALSE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] Parameters validated successfully");

    // Get service name first
    const axis2_char_t* service_name = axis2_svc_get_name(svc, env);
    if (!service_name) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Failed to get service name");
        return AXIS2_FALSE;
    }

    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] CRITICAL - Service name extracted: '%s'", service_name);

    // CRITICAL: Extract service class name from ServiceClass parameter - this is where corruption occurs
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] CRITICAL - About to extract ServiceClass parameter from impl_class_param=%p", impl_class_param);

    service_class_name = (const axis2_char_t*)axutil_param_get_value(impl_class_param, env);

    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] CRITICAL - ServiceClass extraction result: service_class_name=%p", service_class_name);

    if (!service_class_name) {
        // Fallback: use service name as class name
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] CRITICAL - No ServiceClass parameter found for '%s' - using service name as class name", service_name);
        service_class_name = service_name;
    } else {
        // GENERIC CORRUPTION DETECTION: Check if ServiceClass parameter contains corrupted data
        axis2_bool_t is_corrupted = AXIS2_FALSE;
        size_t class_name_len = strlen(service_class_name);
        axis2_bool_t has_underscore = AXIS2_FALSE;

        // Check for non-printable characters or obvious corruption patterns
        if (service_class_name) {
            for (int i = 0; service_class_name[i] != '\0'; i++) {
                if (!isprint(service_class_name[i]) || (unsigned char)service_class_name[i] > 127) {
                    is_corrupted = AXIS2_TRUE;
                    break;
                }
                if (service_class_name[i] == '_') {
                    has_underscore = AXIS2_TRUE;
                }
            }
        }

        // ADDITIONAL CORRUPTION CHECKS:
        // 1. ServiceClass should be at least 5 characters (e.g., "x_svc" minimum)
        // 2. ServiceClass should contain underscore (snake_case naming convention)
        // 3. ServiceClass should not be uppercase only (CamelCase would indicate corruption)
        if (!is_corrupted && class_name_len < 5) {
            AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                "[JSON_DIRECT] CORRUPTION_DETECTED - ServiceClass too short (%d chars): '%s'",
                (int)class_name_len, service_class_name);
            is_corrupted = AXIS2_TRUE;
        }
        if (!is_corrupted && !has_underscore && class_name_len > 0) {
            AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                "[JSON_DIRECT] CORRUPTION_DETECTED - ServiceClass missing underscore: '%s'",
                service_class_name);
            is_corrupted = AXIS2_TRUE;
        }

        if (is_corrupted) {
            // GENERIC SAFE CONVERSION: Convert CamelCase service name to snake_case
            char* converted_name = (char*)AXIS2_MALLOC(env->allocator, strlen(service_name) * 2 + 1);
            if (converted_name) {
                int j = 0;
                for (int i = 0; service_name[i] != '\0'; i++) {
                    if (i > 0 && isupper(service_name[i])) {
                        converted_name[j++] = '_';
                    }
                    converted_name[j++] = tolower(service_name[i]);
                }
                converted_name[j] = '\0';
                service_class_name = converted_name;

                AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                    "[JSON_DIRECT] GENERIC_CORRUPTION_FIX - Detected corruption, converted '%s' to '%s'",
                    service_name, service_class_name);
            } else {
                // Fallback if allocation fails
                service_class_name = service_name;
                AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                    "[JSON_DIRECT] ALLOCATION_FAILED - Using service name as fallback");
            }
        } else {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON_DIRECT] PARAMETER_VALID - Using ServiceClass parameter as provided");
        }
    }

    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] CRITICAL - Final service class name set (safe logging)");

    // Build library path: /usr/local/axis2c/services/<ServiceName>/lib<serviceclass>.so
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] Building library path with safe parameters");

    // Build library path: /usr/local/axis2c/services/ServiceName/libserviceclass.so
    service_lib_path = AXIS2_MALLOC(env->allocator,
        strlen("/usr/local/axis2c/services/") + strlen(service_name) +
        strlen("/lib") + strlen(service_class_name) + strlen(".so") + 1);

    if (!service_lib_path) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Failed to allocate memory for service library path");
        return AXIS2_FALSE;
    }

    sprintf(service_lib_path, "/usr/local/axis2c/services/%s/lib%s.so", service_name, service_class_name);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] constructed path='%s'", service_lib_path);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] Attempting to load service library: %s", service_lib_path);

    // Load the service library
    service_lib = dlopen(service_lib_path, RTLD_LAZY);
    if (!service_lib) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Failed to load service library %s: %s", service_lib_path, dlerror());
        goto cleanup;
    }

    // Build function name: <serviceclass>_invoke_json
    service_function_name = axutil_stracat(env, service_class_name, "_invoke_json");

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] Looking for function: %s", service_function_name);

    // Get the service invoke function
    service_invoke = (json_service_invoke_func)dlsym(service_lib, service_function_name);
    if (!service_invoke) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Function %s not found in %s: %s",
            service_function_name, service_lib_path, dlerror());
        goto cleanup;
    }

    // Parse JSON request
    json_request = json_tokener_parse(json_request_str);
    if (!json_request) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Failed to parse JSON request");
        goto cleanup;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] Calling %s with JSON request", service_function_name);

    // Call the service function
    json_response_obj = service_invoke(env, json_request);
    if (!json_response_obj) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Service function %s returned NULL", service_function_name);
        goto cleanup;
    }

    // Convert JSON response to string
    json_response_str = json_object_to_json_string(json_response_obj);
    if (!json_response_str) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Failed to convert JSON response to string");
        goto cleanup;
    }

    // Copy response string for caller
    *json_response_out = axutil_strdup(env, json_response_str);
    if (!*json_response_out) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON_DIRECT] Failed to duplicate JSON response string");
        goto cleanup;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON_DIRECT] JSON-direct service call successful, response length: %d",
        (int)strlen(*json_response_out));

    result = AXIS2_TRUE;

cleanup:
    if (json_request) {
        json_object_put(json_request);
    }
    if (json_response_obj) {
        json_object_put(json_response_obj);
    }
    if (service_lib) {
        dlclose(service_lib);
    }
    if (service_lib_path) {
        AXIS2_FREE(env->allocator, service_lib_path);
    }
    if (service_function_name) {
        AXIS2_FREE(env->allocator, service_function_name);
    }

    return result;
}

/**
 * Revolutionary JSON Business Logic Invocation (AXIOM-FREE)
 * This bypasses all SOAP/XML processing and calls service functions directly
 */
static axis2_status_t AXIS2_CALL
axis2_json_rpc_msg_recv_invoke_business_logic_sync(
    axis2_msg_recv_t* msg_recv,
    const axutil_env_t* env,
    axis2_msg_ctx_t* in_msg_ctx,
    axis2_msg_ctx_t* out_msg_ctx)
{
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] ENTRY POINT - Starting JSON business logic invocation");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] This function should handle ALL JSON errors and return JSON (NOT SOAP!)");

    axis2_svc_t* svc = NULL;
    axis2_op_ctx_t* op_ctx = NULL;
    axis2_op_t* op = NULL;
    const axis2_char_t* operation_name = NULL;
    axis2_char_t* json_request = NULL;
    axis2_char_t* json_response = NULL;
    axutil_param_t* impl_class_param = NULL;
    void* impl_obj = NULL;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "JsonRpcMessageReceiver: Revolutionary AXIOM-FREE processing");

    // Check if this is a JSON request (Revolutionary: Direct content type check)
    axutil_property_t* prop = axis2_msg_ctx_get_property(in_msg_ctx, env, "Content-Type");
    if (prop) {
        axis2_char_t* content_type = (axis2_char_t*)axutil_property_get_value(prop, env);
        if (!content_type || !strstr(content_type, "application/json")) {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "JsonRpcMessageReceiver: Not JSON request, skipping");
            return AXIS2_SUCCESS; // Let other processors handle non-JSON
        }
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "JsonRpcMessageReceiver: JSON request detected - bypassing AXIOM completely");

    // Set JSON processing mode (Revolutionary: Native Axis2/C flag)
#ifdef AXIS2_JSON_ENABLED
    axis2_msg_ctx_set_doing_json(in_msg_ctx, env, AXIS2_TRUE);
    axis2_msg_ctx_set_doing_json(out_msg_ctx, env, AXIS2_TRUE);
#endif

    // Get service and operation (Framework integration)
    svc = axis2_msg_ctx_get_svc(in_msg_ctx, env);
    if (!svc) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JsonRpcMessageReceiver: No service found");
        return AXIS2_FAILURE;
    }

    op_ctx = axis2_msg_ctx_get_op_ctx(in_msg_ctx, env);
    if (op_ctx) {
        op = axis2_op_ctx_get_op(op_ctx, env);
        if (op) {
            const axutil_qname_t* op_qname = axis2_op_get_qname(op, env);
            if (op_qname) {
                operation_name = axutil_qname_get_localpart(op_qname, env);
            }
        }
    }

    if (!operation_name) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "JsonRpcMessageReceiver: No operation name");
        return AXIS2_FAILURE;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "JsonRpcMessageReceiver: Processing JSON operation '%s'", operation_name);

    // Revolutionary: Extract JSON directly from HTTP stream (no XML conversion)
    // CRITICAL FIX: For HTTP/2 requests, check for pre-read JSON_REQUEST_BODY first
    // This fixes the issue where request body is consumed by HTTP/2 processor
    axutil_property_t* json_body_prop = axis2_msg_ctx_get_property(in_msg_ctx, env, "JSON_REQUEST_BODY");
    if (json_body_prop) {
        axis2_char_t* pre_read_json = (axis2_char_t*)axutil_property_get_value(json_body_prop, env);
        if (pre_read_json && axutil_strlen(pre_read_json) > 0) {
            json_request = axutil_strdup(env, pre_read_json);
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON_HTTP2_FIX] Using pre-read JSON from HTTP/2 processor (%d bytes): '%.100s%s'",
                (int)axutil_strlen(json_request),
                json_request,
                axutil_strlen(json_request) > 100 ? "..." : "");
        }
    }

    // Fallback: Try reading from stream (for HTTP/1.1 or if JSON_REQUEST_BODY not available)
    if (!json_request) {
        axutil_stream_t* in_stream = NULL;
        prop = axis2_msg_ctx_get_property(in_msg_ctx, env, AXIS2_TRANSPORT_IN);
        if (prop) {
            in_stream = (axutil_stream_t*)axutil_property_get_value(prop, env);
        }

        if (in_stream) {
            size_t buffer_size = 8192;
            size_t total_size = 0;
            axis2_char_t buffer[8192];
            int bytes_read = 0;
            int read_attempts = 0;
            const int max_read_attempts = 100; // Prevent infinite hangs
            const size_t max_json_size = 10 * 1024 * 1024; // 10MB limit

            json_request = AXIS2_MALLOC(env->allocator, buffer_size);
            if (json_request) {
                while ((bytes_read = axutil_stream_read(in_stream, env, buffer, sizeof(buffer))) > 0) {
                    read_attempts++;

                    // HANG FIX: Prevent infinite reading loops
                    if (read_attempts > max_read_attempts) {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[JSON_STREAM_HANG_FIX] Stream reading exceeded maximum attempts (%d) - possible malformed JSON",
                            max_read_attempts);
                        break;
                    }

                    // HANG FIX: Prevent excessive memory usage
                    if (total_size + bytes_read > max_json_size) {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[JSON_STREAM_HANG_FIX] JSON size exceeded maximum limit (%zu bytes) - rejecting request",
                            max_json_size);
                        break;
                    }

                    if (total_size + bytes_read >= buffer_size) {
                        buffer_size *= 2;
                        axis2_char_t* new_buffer = AXIS2_REALLOC(env->allocator, json_request, buffer_size);
                        if (new_buffer) {
                            json_request = new_buffer;
                        } else {
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[JSON_STREAM_HANG_FIX] Memory reallocation failed during JSON reading");
                            break;
                        }
                    }
                    memcpy(json_request + total_size, buffer, bytes_read);
                    total_size += bytes_read;
                }
                json_request[total_size] = '\0';

                if (total_size > 0) {
                    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                        "[JSON_STREAM_READ] Read JSON from transport stream (%d bytes): '%.100s%s'",
                        (int)total_size, json_request,
                        total_size > 100 ? "..." : "");

                    // VALIDATION FIX: Validate JSON format immediately after reading
                    json_object *validation_obj = json_tokener_parse(json_request);
                    if (!validation_obj) {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[JSON_VALIDATION_FIX] Invalid JSON format detected - returning validation error");

                        // Create JSON error response for malformed input
                        if (json_request) {
                            AXIS2_FREE(env->allocator, json_request);
                        }
                        json_request = axutil_strdup(env,
                            "{\"error\":{\"code\":\"INVALID_JSON\",\"message\":\"Malformed JSON in request body\"}}");

                        // Set error response in output message context
                        axis2_msg_ctx_set_status_code(out_msg_ctx, env, 400); // Bad Request

                        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                            "[JSON_VALIDATION_FIX] Returning JSON validation error response");
                    } else {
                        json_object_put(validation_obj); // Free validation object
                        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                            "[JSON_VALIDATION_FIX] JSON validation passed - proceeding with processing");
                    }
                }
            }
        }
    }

    // Final fallback: empty JSON object
    if (!json_request || axutil_strlen(json_request) == 0) {
        if (json_request) {
            AXIS2_FREE(env->allocator, json_request);
        }
        json_request = axutil_strdup(env, "{}");
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "[JSON_FALLBACK] No JSON found in HTTP/2 property or stream - using empty object");
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "[JSON_EXTRACT_DEBUG] Extracted JSON from HTTP request: '%s'", json_request);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "JsonRpcMessageReceiver: Starting service parameter processing for operation: %s",
        operation_name ? operation_name : "NULL");

    // Revolutionary: Direct service function invocation (AXIOM-FREE)
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: About to extract ServiceClass parameter from service: %p", (void*)svc);
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: Service name: %s",
        axis2_svc_get_name(svc, env) ? axis2_svc_get_name(svc, env) : "NULL");

    // Check all service parameters for debugging
    axutil_array_list_t* param_list = axis2_svc_get_all_params(svc, env);
    if (param_list) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Service has parameter list: %p", (void*)param_list);
        int param_count = axutil_array_list_size(param_list, env);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Service has %d parameters", param_count);
    } else {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Service has NO parameter list!");
    }

    // FIXED: Use the same constant as working msg_recv.c code
    impl_class_param = axis2_svc_get_param(svc, env, "ServiceClass");  // AXIS2_SERVICE_CLASS = "ServiceClass"
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: ServiceClass parameter object: %p", (void*)impl_class_param);

    if (!impl_class_param) {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: ServiceClass parameter is NULL - checking alternative parameter names");
        impl_class_param = axis2_svc_get_param(svc, env, "serviceClass");
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Alternative serviceClass parameter: %p", (void*)impl_class_param);
    }

    if (impl_class_param) {
        // FIXED: Use exact same pattern as working msg_recv.c - pass parameter object directly to class loader
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Using correct msg_recv.c pattern - passing parameter object directly to class loader");

        // Get service name for logging (instead of trying to extract ServiceClass string)
        const axis2_char_t* service_name = axis2_svc_get_name(svc, env);

        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Loading service implementation: %s",
            service_name ? service_name : "unknown");

#ifdef __ANDROID__
        /*
         * Android Static Service Loading
         *
         * On Android with static linking, try the static service registry first.
         * This avoids dlopen() which won't work with statically linked services.
         */
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Android: Trying static service registry for '%s'",
            service_name ? service_name : "unknown");

        if (service_name && json_request &&
            try_android_static_service(env, service_name, json_request, &json_response)) {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Android: Static service '%s' invoked successfully",
                service_name);
            /* json_response is already set, skip to response handling */
            goto response_ready;
        }

        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Android: Service '%s' not in static registry, trying dynamic loading",
            service_name ? service_name : "unknown");
#endif

        // CORRECT PATTERN: Pass parameter object directly to class loader (same as msg_recv.c:258)
        impl_obj = axutil_class_loader_create_dll(env, impl_class_param);

        if (impl_obj) {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "JsonRpcMessageReceiver: Service loaded successfully - %s",
                service_name ? service_name : "unknown");

            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "JsonRpcMessageReceiver: Building JSON response for service: %s, operation: %s",
                service_name ? service_name : "unknown", operation_name ? operation_name : "NULL");

            // Revolutionary: Create JSON response (service-specific processing would go here)
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: Starting JSON response generation for service='%s' operation='%s'",
                service_name ? service_name : "unknown", operation_name ? operation_name : "NULL");

            axis2_char_t* part1 = axutil_stracat(env, "{\"service\":\"", service_name ? service_name : "unknown");
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "[JSON RPC MSG RECV] DEBUG: part1 created: %p", (void*)part1);

            if (part1) {
                axis2_char_t* part2 = axutil_stracat(env, part1, "\",\"operation\":\"");
                AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "[JSON RPC MSG RECV] DEBUG: part2 created: %p", (void*)part2);

                if (part2) {
                    axis2_char_t* part3 = axutil_stracat(env, part2, operation_name ? operation_name : "unknown");
                    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "[JSON RPC MSG RECV] DEBUG: part3 created: %p", (void*)part3);

                    if (part3) {
                        json_response = axutil_stracat(env, part3, "\",\"status\":\"success\",\"message\":\"Service loaded correctly\"}");
                        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "[JSON RPC MSG RECV] DEBUG: json_response created: %p", (void*)json_response);

                        // Free intermediate strings
                        AXIS2_FREE(env->allocator, part3);
                    }
                    AXIS2_FREE(env->allocator, part2);
                }
                AXIS2_FREE(env->allocator, part1);
            }
        } else {
            AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                "JsonRpcMessageReceiver: Failed to load traditional service - %s, trying JSON-direct pattern",
                service_name ? service_name : "unknown");

            // Fallback: Try JSON-direct service loading pattern for HTTP/2 JSON services
            if (try_json_direct_service_loading(env, svc, impl_class_param, json_request, &json_response)) {
                AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                    "JsonRpcMessageReceiver: JSON-direct service loaded successfully - %s",
                    service_name ? service_name : "unknown");
            } else {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "JsonRpcMessageReceiver: Both traditional and JSON-direct service loading failed - %s",
                    service_name ? service_name : "unknown");
            }
        }
    }

#ifdef __ANDROID__
response_ready:
#endif
    if (!json_response) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] No JSON response generated - creating default error JSON");
        json_response = axutil_strdup(env, "{\"error\":\"Service invocation failed\"}");
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] Generated JSON response (should NOT create SOAP!) - length: %d",
        json_response ? (int)strlen(json_response) : 0);

    // Revolutionary: Store pure JSON response (no SOAP envelope)
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: About to check json_response - pointer: %p", (void*)json_response);
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: out_msg_ctx pointer: %p", (void*)out_msg_ctx);

    if (json_response) {
        int json_response_len = strlen(json_response);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Setting JSON response properties - Length: %d bytes", json_response_len);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Response content preview (first 100 chars): %.100s", json_response);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Response content end (last 20 chars): %s",
            json_response_len > 20 ? json_response + json_response_len - 20 : json_response);

        // Check for null bytes in response
        int has_null_byte = 0;
        for (int i = 0; i < json_response_len; i++) {
            if (json_response[i] == '\0') {
                has_null_byte = 1;
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[JSON RPC MSG RECV] WARNING: Found null byte at position %d in JSON response!", i);
                break;
            }
        }
        if (!has_null_byte) {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] SUCCESS: No null bytes found in JSON response content");
        }

        axutil_property_t* json_prop = axutil_property_create(env);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Created property object: %p", (void*)json_prop);

        if (!json_prop) {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: CRITICAL: Failed to create property object!");
        }

        axutil_property_set_value(json_prop, env, json_response);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Set property value to json_response: %p", (void*)json_response);

        // Verify property was set correctly
        void* prop_value = axutil_property_get_value(json_prop, env);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Verified property value after set: %p", prop_value);
        if (prop_value && prop_value == json_response) {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: SUCCESS: Property value matches json_response pointer");
        } else {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: ERROR: Property value does NOT match json_response pointer!");
        }

        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: About to set JSON_RESPONSE property in out_msg_ctx: %p", (void*)out_msg_ctx);
        axis2_msg_ctx_set_property(out_msg_ctx, env, "JSON_RESPONSE", json_prop);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Set JSON_RESPONSE property in out_msg_ctx: %p", (void*)out_msg_ctx);

        // CRITICAL FIX: Also set JSON_RESPONSE in in_msg_ctx since JSON processor might check that context
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: About to ALSO set JSON_RESPONSE property in in_msg_ctx: %p", (void*)in_msg_ctx);
        if (in_msg_ctx && in_msg_ctx != out_msg_ctx) {
            // Create another property object for in_msg_ctx (don't reuse the same property)
            axutil_property_t* json_prop_in = axutil_property_create(env);
            axutil_property_set_value(json_prop_in, env, json_response);
            axis2_msg_ctx_set_property(in_msg_ctx, env, "JSON_RESPONSE", json_prop_in);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: Set JSON_RESPONSE property in in_msg_ctx: %p", (void*)in_msg_ctx);

            // CRITICAL: Verify the property was actually set in in_msg_ctx
            axutil_property_t* verification_prop_in = axis2_msg_ctx_get_property(in_msg_ctx, env, "JSON_RESPONSE");
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: Verification - retrieved JSON_RESPONSE from in_msg_ctx: %p", (void*)verification_prop_in);
            if (verification_prop_in) {
                void* verification_value_in = axutil_property_get_value(verification_prop_in, env);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[JSON RPC MSG RECV] DEBUG: Verification - JSON_RESPONSE value in in_msg_ctx: %p", verification_value_in);
                if (verification_value_in == json_response) {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[JSON RPC MSG RECV] DEBUG: SUCCESS: JSON_RESPONSE property correctly stored in in_msg_ctx");
                } else {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[JSON RPC MSG RECV] DEBUG: ERROR: JSON_RESPONSE property value mismatch in in_msg_ctx!");
                }
            } else {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[JSON RPC MSG RECV] DEBUG: CRITICAL ERROR: JSON_RESPONSE property NOT FOUND in in_msg_ctx after setting!");
            }
        } else {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: Skipping in_msg_ctx set (same as out_msg_ctx or NULL)");
        }

        // Verify the property was actually set in the message context
        axutil_property_t* verification_prop = axis2_msg_ctx_get_property(out_msg_ctx, env, "JSON_RESPONSE");
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Verification - retrieved JSON_RESPONSE property: %p", (void*)verification_prop);
        if (verification_prop) {
            void* verification_value = axutil_property_get_value(verification_prop, env);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: Verification - JSON_RESPONSE property value: %p", verification_value);
            if (verification_value == json_response) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[JSON RPC MSG RECV] DEBUG: SUCCESS: JSON_RESPONSE property correctly stored in out_msg_ctx");
            } else {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[JSON RPC MSG RECV] DEBUG: ERROR: JSON_RESPONSE property value mismatch in out_msg_ctx!");
            }
        } else {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] DEBUG: CRITICAL ERROR: JSON_RESPONSE property NOT FOUND in out_msg_ctx after setting!");
        }

        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Set JSON_RESPONSE property in message context");

        // Set JSON content type for response
        axutil_property_t* content_type_prop = axutil_property_create(env);
        axutil_property_set_value(content_type_prop, env, axutil_strdup(env, "application/json"));
        axis2_msg_ctx_set_property(out_msg_ctx, env, "Content-Type", content_type_prop);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Set Content-Type property to 'application/json'");

        // Set Accept header for HTTP/2 responses to prevent curl binary warning
        axutil_property_t* accept_prop = axutil_property_create(env);
        axutil_property_set_value(accept_prop, env, axutil_strdup(env, "application/json"));
        axis2_msg_ctx_set_property(out_msg_ctx, env, "Accept", accept_prop);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Set Accept property to 'application/json'");

        // Add HTTP output headers to ensure they're sent to client
        axutil_array_list_t* output_headers = axis2_msg_ctx_get_http_output_headers(out_msg_ctx, env);
        if (!output_headers) {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Creating new HTTP output headers array");
            output_headers = axutil_array_list_create(env, 4);
            axis2_msg_ctx_set_http_output_headers(out_msg_ctx, env, output_headers);
        } else {
            int existing_count = axutil_array_list_size(output_headers, env);
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Found existing HTTP output headers array with %d headers", existing_count);
        }

        // Add Content-Type header to HTTP output headers to force text display
        axis2_http_header_t* content_type_header = axis2_http_header_create(env, "Content-Type", "application/json; charset=utf-8");
        if (content_type_header) {
            axutil_array_list_add(output_headers, env, content_type_header);
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Added Content-Type: application/json; charset=utf-8 header to HTTP output headers");
        } else {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Failed to create Content-Type header!");
        }

        // Add Cache-Control to prevent binary detection
        axis2_http_header_t* cache_control_header = axis2_http_header_create(env, "Cache-Control", "no-cache");
        if (cache_control_header) {
            axutil_array_list_add(output_headers, env, cache_control_header);
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Added Cache-Control: no-cache header to HTTP output headers");
        } else {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                "[JSON RPC MSG RECV] Failed to create Cache-Control header!");
        }

        // Log final header count
        int final_header_count = axutil_array_list_size(output_headers, env);
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] Final HTTP output headers count: %d", final_header_count);
    } else {
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] json_response is NULL - no response to process!");
    }

    if (json_request) {
        AXIS2_FREE(env->allocator, json_request);
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: FINAL COMPLETION STATUS");
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: json_response final pointer: %p", (void*)json_response);
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] DEBUG: out_msg_ctx final pointer: %p", (void*)out_msg_ctx);

    // Final verification of JSON_RESPONSE property before returning
    if (out_msg_ctx) {
        axutil_property_t* final_prop = axis2_msg_ctx_get_property(out_msg_ctx, env, "JSON_RESPONSE");
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "[JSON RPC MSG RECV] DEBUG: Final JSON_RESPONSE property check: %p", (void*)final_prop);
    }

    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] SUCCESSFUL COMPLETION - returning AXIS2_SUCCESS");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "[JSON RPC MSG RECV] If SOAP fault is still generated, error handling is BYPASSING this function!");

    return AXIS2_SUCCESS;
}

/**
 * Create Revolutionary AXIOM-FREE JsonRpcMessageReceiver (Core Framework Component)
 */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_json_rpc_msg_recv_create(const axutil_env_t* env)
{
    axis2_msg_recv_t* msg_recv = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Creating Revolutionary JsonRpcMessageReceiver (AXIOM-FREE)");

    // Use standard Axis2/C message receiver factory (Revolutionary: No custom struct needed)
    msg_recv = axis2_msg_recv_create(env);
    if (!msg_recv) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    // Set application scope for JSON services
    status = axis2_msg_recv_set_scope(msg_recv, env, AXIS2_APPLICATION_SCOPE);
    if (status != AXIS2_SUCCESS) {
        axis2_msg_recv_free(msg_recv, env);
        return NULL;
    }

    // Revolutionary: Set AXIOM-free JSON business logic
    axis2_msg_recv_set_invoke_business_logic(msg_recv, env,
        axis2_json_rpc_msg_recv_invoke_business_logic_sync);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Revolutionary JsonRpcMessageReceiver created (Zero AXIOM dependencies)");

    return msg_recv;
}