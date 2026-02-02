# Quick Start Guide

This guide walks you through creating a simple JSON-RPC service with Axis2/C.

## Creating a Service

### 1. Service Implementation

Create a file `hello_service.c`:

```c
#include <axis2_svc_skeleton.h>
#include <axutil_env.h>

axiom_node_t* AXIS2_CALL
hello_invoke(axis2_svc_skeleton_t *svc_skeleton,
             const axutil_env_t *env,
             axiom_node_t *node,
             axis2_msg_ctx_t *msg_ctx)
{
    /* Parse JSON request */
    const char *name = axis2_json_get_string(env, node, "name");

    /* Create JSON response */
    axiom_node_t *response = axis2_json_create_object(env);
    axis2_json_set_string(env, response, "message",
        axutil_strcat(env, "Hello, ", name ? name : "World", "!", NULL));

    return response;
}

/* Service skeleton operations */
static const axis2_svc_skeleton_ops_t hello_svc_skeleton_ops = {
    .invoke = hello_invoke,
    .on_fault = NULL,
    .init = NULL,
    .free = NULL
};

/* Service entry point */
AXIS2_EXPORT axis2_svc_skeleton_t* AXIS2_CALL
axis2_hello_create(const axutil_env_t *env)
{
    axis2_svc_skeleton_t *svc_skeleton =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_skeleton_t));
    svc_skeleton->ops = &hello_svc_skeleton_ops;
    return svc_skeleton;
}
```

### 2. Service Descriptor

Create `services.xml`:

```xml
<service name="HelloService">
    <description>Simple Hello World service</description>
    <parameter name="ServiceClass">hello</parameter>
    <operation name="sayHello">
        <messageReceiver class="axis2_json_msg_recv"/>
    </operation>
</service>
```

### 3. Build the Service

```bash
gcc -shared -fPIC -o libhello.so hello_service.c \
    $(pkg-config --cflags --libs axis2c)
```

### 4. Deploy

```bash
mkdir -p $AXIS2C_HOME/services/HelloService
cp libhello.so $AXIS2C_HOME/services/HelloService/
cp services.xml $AXIS2C_HOME/services/HelloService/
```

## Testing the Service

### Start the Server

```bash
# Using Apache httpd with mod_axis2
sudo systemctl start httpd

# Or standalone simple server
axis2_http_server -p 8080
```

### Send a Request

```bash
curl -X POST http://localhost:8080/axis2/services/HelloService/sayHello \
    -H "Content-Type: application/json" \
    -d '{"name": "Axis2"}'
```

Expected response:

```json
{"message": "Hello, Axis2!"}
```

## Next Steps

- [Services Guide](../guide/services.md) - Detailed service development
- [Configuration](../guide/axis2-xml.md) - Server configuration
- [HTTP/2 Setup](../HTTP2_AXIS2_DOT_XML.md) - Enable HTTP/2 protocol
