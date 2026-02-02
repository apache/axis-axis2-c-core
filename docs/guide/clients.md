# Clients Guide

This guide covers building Axis2/C clients for consuming HTTP/2 JSON-RPC and legacy SOAP services.

## HTTP/2 JSON-RPC Client

### Using curl (Testing)

```bash
# Simple GET request
curl -s --http2 \
    --cert client.crt --key client.key --cacert ca.crt \
    "https://server:8443/services/MyService/getStatus"

# POST with JSON body
curl -s --http2 \
    --cert client.crt --key client.key --cacert ca.crt \
    -H "Content-Type: application/json" \
    -d '{"action":"doSomething","param":"value"}' \
    "https://server:8443/services/MyService/operation"
```

### C Client with Axis2/C

```c
#include <axis2_stub.h>
#include <axis2_http2_client.h>

int main(int argc, char *argv[])
{
    axutil_env_t *env = axutil_env_create_all("client.log", AXIS2_LOG_LEVEL_INFO);

    /* Create HTTP/2 client */
    axis2_http2_client_t *client = axis2_http2_client_create(env,
        "https://server:8443/services/MyService");

    /* Configure TLS */
    axis2_http2_client_set_cert(client, env, "client.crt");
    axis2_http2_client_set_key(client, env, "client.key");
    axis2_http2_client_set_ca(client, env, "ca.crt");

    /* Create JSON request */
    axiom_node_t *request = axis2_json_create_object(env);
    axis2_json_set_string(env, request, "action", "getStatus");

    /* Send request */
    axiom_node_t *response = axis2_http2_client_send(client, env,
        "getStatus", request);

    /* Process response */
    const char *status = axis2_json_get_string(env, response, "status");
    printf("Status: %s\n", status);

    /* Cleanup */
    axis2_http2_client_free(client, env);
    axutil_env_free(env);

    return 0;
}
```

### Build the Client

```bash
gcc -o my_client my_client.c \
    $(pkg-config --cflags --libs axis2c) \
    -lnghttp2 -lssl -lcrypto
```

## Multi-Camera Example

For IoT/camera control scenarios with multiple endpoints:

```c
const char *cameras[] = {
    "192.168.1.100",
    "192.168.1.101",
    "192.168.1.102"
};

for (int i = 0; i < 3; i++) {
    char url[256];
    snprintf(url, sizeof(url),
        "https://%s:8443/services/CameraControlService", cameras[i]);

    axis2_http2_client_t *client = axis2_http2_client_create(env, url);
    /* ... configure and send ... */
}
```

## Legacy SOAP Client

For consuming legacy SOAP services:

```c
#include <axis2_stub.h>
#include <axis2_client.h>

/* Create service client */
axis2_svc_client_t *svc_client = axis2_svc_client_create(env, client_home);

/* Set endpoint */
axis2_options_t *options = axis2_options_create(env);
axis2_options_set_to(options, env,
    axis2_endpoint_ref_create(env, "http://server:8080/axis2/services/LegacyService"));

axis2_svc_client_set_options(svc_client, env, options);

/* Build SOAP request */
axiom_node_t *payload = build_soap_request(env);

/* Send and receive */
axiom_node_t *response = axis2_svc_client_send_receive(svc_client, env, payload);
```

## Error Handling

```c
axiom_node_t *response = axis2_http2_client_send(client, env, "operation", request);

if (response == NULL) {
    /* Check for error */
    axis2_char_t *error = axis2_http2_client_get_error(client, env);
    fprintf(stderr, "Request failed: %s\n", error);
    return -1;
}

/* Check for application-level error in JSON response */
const char *error_field = axis2_json_get_string(env, response, "error");
if (error_field != NULL) {
    fprintf(stderr, "Service error: %s\n", error_field);
}
```

## See Also

- [Security Guide](../SECURITY.md) - TLS and mTLS configuration
- [HTTP/2 Overview](../HTTP2_JSON_REVOLUTION_STATE.md)
- [Migration Guide](../AXIS2C_HTTP2_MIGRATION_STATE.md)
