# API Reference

## Doxygen Documentation

The complete API documentation is generated from source code comments using Doxygen.

### Building API Docs

```bash
cd axis-axis2-c-core
doxygen Doxyfile
# Output: docs/api/html/index.html
```

### Online API Docs

- [Doxygen API Reference](https://axis.apache.org/axis2/c/core/api/) (hosted)

## Core APIs

### Environment (`axutil_env.h`)

The environment provides memory allocation, logging, and error handling:

```c
/* Create environment */
axutil_env_t *env = axutil_env_create_all("app.log", AXIS2_LOG_LEVEL_INFO);

/* Memory allocation */
void *ptr = AXIS2_MALLOC(env->allocator, size);
AXIS2_FREE(env->allocator, ptr);

/* Logging */
AXIS2_LOG_INFO(env->log, "Message: %s", value);
AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occurred");

/* Error handling */
AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE, AXIS2_FAILURE);

/* Cleanup */
axutil_env_free(env);
```

### Service Client (`axis2_svc_client.h`)

Client API for consuming services:

```c
/* Create client */
axis2_svc_client_t *client = axis2_svc_client_create(env, client_home);

/* Configure options */
axis2_options_t *options = axis2_options_create(env);
axis2_options_set_to(options, env, endpoint_ref);
axis2_svc_client_set_options(client, env, options);

/* Send request */
axiom_node_t *response = axis2_svc_client_send_receive(client, env, payload);

/* Cleanup */
axis2_svc_client_free(client, env);
```

### Service Skeleton (`axis2_svc_skeleton.h`)

Server-side service implementation:

```c
/* Service skeleton operations */
typedef struct axis2_svc_skeleton_ops {
    axiom_node_t* (*invoke)(axis2_svc_skeleton_t*, const axutil_env_t*,
                            axiom_node_t*, axis2_msg_ctx_t*);
    axiom_node_t* (*on_fault)(axis2_svc_skeleton_t*, const axutil_env_t*,
                              axiom_node_t*);
    int (*init)(axis2_svc_skeleton_t*, const axutil_env_t*);
    int (*free)(axis2_svc_skeleton_t*, const axutil_env_t*);
} axis2_svc_skeleton_ops_t;
```

### Message Context (`axis2_msg_ctx.h`)

Request/response context:

```c
/* Get HTTP headers */
axutil_hash_t *headers = axis2_msg_ctx_get_http_headers(msg_ctx, env);

/* Get client certificate DN (mTLS) */
const axis2_char_t *cert_dn = axis2_msg_ctx_get_client_cert_dn(msg_ctx, env);

/* Check protocol */
axis2_bool_t is_http2 = axis2_msg_ctx_is_http2(msg_ctx, env);
```

### JSON Utilities (`axis2_json.h`)

JSON parsing and generation (when `AXIS2_JSON_ENABLED`):

```c
/* Parse JSON */
const char *value = axis2_json_get_string(env, node, "key");
int num = axis2_json_get_int(env, node, "count");

/* Create JSON */
axiom_node_t *obj = axis2_json_create_object(env);
axis2_json_set_string(env, obj, "status", "success");
axis2_json_set_int(env, obj, "count", 42);
```

## Module APIs

| Module | Header | Description |
|--------|--------|-------------|
| Addressing | `axis2_addr.h` | WS-Addressing support |
| Rampart | `rampart_context.h` | WS-Security (legacy) |

## Utility APIs

| Header | Description |
|--------|-------------|
| `axutil_string.h` | String manipulation |
| `axutil_hash.h` | Hash table |
| `axutil_array_list.h` | Dynamic array |
| `axutil_linked_list.h` | Linked list |
| `axutil_thread.h` | Threading primitives |
| `axutil_file_handler.h` | File I/O |

## See Also

- [Architecture](../guide/architecture.md)
- [Services Guide](../guide/services.md)
- [Clients Guide](../guide/clients.md)
