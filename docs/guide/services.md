# Services Guide

Services are the core building blocks of Axis2/C applications. This guide covers service creation, deployment, and configuration.

## Service Structure

A service consists of:

1. **Shared Library** - Compiled C code implementing your business logic
2. **services.xml** - Service descriptor defining operations and configuration
3. **Optional Resources** - Schema files, WSDL (for SOAP), etc.

```
services/
└── CameraControlService/
    ├── libcamera_control.so
    ├── services.xml
    └── schema/
        └── camera.json
```

## Creating a Service

### 1. Service Skeleton

Every service implements the `axis2_svc_skeleton` interface:

```c
#include <axis2_svc_skeleton.h>

/* Operation handler */
axiom_node_t* AXIS2_CALL
my_operation(axis2_svc_skeleton_t *svc_skeleton,
             const axutil_env_t *env,
             axiom_node_t *node,
             axis2_msg_ctx_t *msg_ctx)
{
    /* Process request, return response */
    return response_node;
}

/* Service skeleton with operations */
static const axis2_svc_skeleton_ops_t my_svc_ops = {
    .invoke = my_operation,
    .on_fault = my_fault_handler,
    .init = my_init,
    .free = my_free
};

/* Entry point - called by Axis2/C to load service */
AXIS2_EXPORT axis2_svc_skeleton_t* AXIS2_CALL
axis2_my_service_create(const axutil_env_t *env)
{
    axis2_svc_skeleton_t *svc = AXIS2_MALLOC(env->allocator,
                                              sizeof(axis2_svc_skeleton_t));
    svc->ops = &my_svc_ops;
    return svc;
}
```

### 2. services.xml

```xml
<service name="MyService">
    <description>Example service</description>

    <!-- Service class - matches create function suffix -->
    <parameter name="ServiceClass">my_service</parameter>

    <!-- JSON-RPC operation -->
    <operation name="doSomething">
        <messageReceiver class="axis2_json_msg_recv"/>
    </operation>

    <!-- SOAP operation (legacy) -->
    <operation name="legacyMethod">
        <messageReceiver class="axis2_raw_xml_in_out_msg_recv"/>
    </operation>
</service>
```

### 3. Build and Deploy

```bash
# Compile
gcc -shared -fPIC -o libmy_service.so my_service.c \
    $(pkg-config --cflags --libs axis2c)

# Deploy
mkdir -p $AXIS2C_HOME/services/MyService
cp libmy_service.so $AXIS2C_HOME/services/MyService/
cp services.xml $AXIS2C_HOME/services/MyService/
```

## JSON-RPC Services

### Request Handling

```c
axiom_node_t* AXIS2_CALL
handle_json_request(axis2_svc_skeleton_t *svc_skeleton,
                    const axutil_env_t *env,
                    axiom_node_t *node,
                    axis2_msg_ctx_t *msg_ctx)
{
    /* Extract JSON fields */
    const char *action = axis2_json_get_string(env, node, "action");
    int count = axis2_json_get_int(env, node, "count");

    /* Create response */
    axiom_node_t *resp = axis2_json_create_object(env);
    axis2_json_set_string(env, resp, "status", "success");
    axis2_json_set_int(env, resp, "processed", count);

    return resp;
}
```

### Response Format

```json
{
    "status": "success",
    "data": {
        "field1": "value1",
        "field2": 123
    }
}
```

## Service Lifecycle

1. **Load** - Axis2/C calls `axis2_<service>_create()`
2. **Init** - `svc_skeleton->ops->init()` called once
3. **Invoke** - `svc_skeleton->ops->invoke()` called per request
4. **Fault** - `svc_skeleton->ops->on_fault()` on errors
5. **Free** - `svc_skeleton->ops->free()` on shutdown

## See Also

- [Quick Start](../getting-started/quickstart.md)
- [services.xml Reference](../HTTP2_SERVICES_DOT_XML.md)
- [JSON Request Path](../HTTP2_JSON_REQUEST_CODE_PATH.md)
