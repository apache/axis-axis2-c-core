# HTTP/2 JSON services.xml Configuration: Runtime Message Receiver Invocation

## Executive Summary

This document explains how the `axis2_json_rpc_msg_recv` messageReceiver defined in `services.xml` gets automatically invoked at runtime when HTTP/2 requests with `Content-Type: application/json` are received by Axis2/C services.

## Key Concept: Configuration-Driven Runtime Dispatch

Unlike the axis2.xml file (which requires no changes for HTTP/2), the **services.xml file defines the specific messageReceiver that handles JSON requests for each operation**. The runtime system automatically invokes the correct messageReceiver based on the HTTP headers and request characteristics.

## Example: Camera Control Service Configuration

From `./samples/user_guide/camera-control-service/services.xml`:

```xml
<operation name="startRecording">
    <description>
        Start camera recording with specified parameters.
        Accepts clip_name, quality, duration, and format parameters.
    </description>
    <messageReceiver class="axis2_json_rpc_msg_recv"/>

    <!-- REST-style HTTP mapping -->
    <parameter name="httpMethod">POST</parameter>
    <parameter name="httpPath">/startRecording</parameter>
    <parameter name="contentType">application/json</parameter>
    <parameter name="responseType">application/json</parameter>
</operation>
```

## Runtime Invocation Flow: From HTTP Headers to Message Receiver

### 1. HTTP Request Reception
When a client sends an HTTP/2 JSON request:

```bash
curl -k --http2 -H "Content-Type: application/json" \
     -X POST -d '{"startRecording":[{"clip_name":"meeting_001","quality":"1080p"}]}' \
     https://localhost:8443/axis2/services/CameraControlService
```

### 2. Request Processor Factory Selection
The Apache2 request processor factory (`axis2_apache2_request_processor_factory.c:66-135`) analyzes:
- **HTTP Protocol**: Detects "HTTP/2.0" in `request->protocol`
- **Content-Type Header**: Detects "application/json" in request headers
- **Decision**: Routes to JSON processor (thread-safe, optimized for HTTP/2)

### 3. Service and Operation Resolution
The HTTP transport layer:
1. **Service Identification**: Extracts service name from URL path (`/axis2/services/CameraControlService`)
2. **Operation Identification**: Extracts operation from JSON payload (`"startRecording"`)
3. **Operation Lookup**: Finds the matching `<operation name="startRecording">` in services.xml

### 4. Message Receiver Retrieval
In `engine.c:311`, the engine calls:

```c
receiver = axis2_op_get_msg_recv(op, env);
```

This retrieves the messageReceiver configured in services.xml:

```xml
<messageReceiver class="axis2_json_rpc_msg_recv"/>
```

### 5. Message Receiver Invocation
In `engine.c:318-319`, the engine invokes:

```c
status = axis2_msg_recv_receive(receiver, env, msg_ctx,
                               axis2_msg_recv_get_derived(receiver, env));
```

### 6. JSON Processing Execution
The `axis2_json_rpc_msg_recv` receiver (`src/core/receivers/axis2_json_rpc_msg_recv.c`):
1. **Bypasses AXIOM/SOAP**: Revolutionary AXIOM-free processing
2. **Direct JSON Parsing**: Uses json-c library for pure JSON processing
3. **Service Function Invocation**: Directly calls the C service function
4. **JSON Response Generation**: Returns JSON response without XML conversion

## Configuration Parameters in services.xml

### Operation-Level messageReceiver
Each operation explicitly defines its message receiver:

```xml
<operation name="startRecording">
    <messageReceiver class="axis2_json_rpc_msg_recv"/>
</operation>

<operation name="stopRecording">
    <messageReceiver class="axis2_json_rpc_msg_recv"/>
</operation>

<operation name="getStatus">
    <messageReceiver class="axis2_json_rpc_msg_recv"/>
</operation>
```

### HTTP/2 Transport Configuration
Service-level HTTP/2 optimization parameters:

```xml
<parameter name="transport.h2">
    <parameter name="enableHTTP2">true</parameter>
    <parameter name="enableStreaming">true</parameter>
    <parameter name="enableMemoryOptimization">true</parameter>
    <parameter name="maxFrameSize">16384</parameter>
    <parameter name="maxConcurrentStreams">50</parameter>
</parameter>
```

### JSON Processing Configuration
Pure JSON processing without SOAP/XML dependencies:

```xml
<parameter name="maxJSONPayloadSize">1048576</parameter>
<parameter name="jsonProcessingMode">pure-jsonc</parameter>
```

### REST-Style HTTP Mapping
Each operation can define HTTP-specific parameters:

```xml
<parameter name="httpMethod">POST</parameter>
<parameter name="httpPath">/startRecording</parameter>
<parameter name="contentType">application/json</parameter>
<parameter name="responseType">application/json</parameter>
```

## Message Receiver Types and Selection

### axis2_json_rpc_msg_recv
- **Purpose**: Revolutionary AXIOM-free JSON processing
- **Use Case**: HTTP/2 JSON requests with Content-Type: application/json
- **Features**:
  - Zero SOAP/XML dependencies
  - Direct JSON-to-service-function invocation
  - HTTP/2 streaming optimized
  - Thread-safe processing

### Traditional SOAP Message Receivers
- **axis2_raw_xml_in_out_msg_recv**: Traditional SOAP processing
- **axis2_rpc_msg_recv**: RPC-style SOAP processing
- **Use Case**: HTTP/1.1 SOAP requests with Content-Type: text/xml

## Runtime Decision Matrix

The system automatically selects the appropriate processing path:

| HTTP Protocol | Content-Type | Request Processor | Message Receiver Used |
|---------------|--------------|-------------------|---------------------|
| HTTP/2.0 | application/json | JSON Processor | axis2_json_rpc_msg_recv |
| HTTP/2.0 | text/xml | JSON Processor* | axis2_json_rpc_msg_recv |
| HTTP/1.1 | application/json | SOAP Processor | axis2_raw_xml_in_out_msg_recv** |
| HTTP/1.1 | text/xml | SOAP Processor | axis2_raw_xml_in_out_msg_recv |

\* HTTP/2 assumes modern client capabilities
\** Falls back to traditional processing for safety

## Advanced Configuration: Multiple Message Receivers

Services can define different message receivers for different operations:

```xml
<service name="HybridService">
    <!-- JSON operation for modern HTTP/2 clients -->
    <operation name="processJson">
        <messageReceiver class="axis2_json_rpc_msg_recv"/>
        <parameter name="contentType">application/json</parameter>
    </operation>

    <!-- SOAP operation for legacy HTTP/1.1 clients -->
    <operation name="processSoap">
        <messageReceiver class="axis2_raw_xml_in_out_msg_recv"/>
        <parameter name="contentType">text/xml</parameter>
    </operation>
</service>
```

## Implementation Details: axis2_json_rpc_msg_recv

### Revolutionary Architecture
From `src/core/receivers/axis2_json_rpc_msg_recv.c:19-30`:

```c
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
```

### Key Features
1. **AXIOM-Free Processing**: No SOAP/XML conversion overhead
2. **Direct Function Invocation**: Bypasses XML processing pipeline
3. **JSON-C Integration**: Pure JSON parsing and generation
4. **HTTP/2 Optimization**: Streaming and memory-efficient processing
5. **Thread Safety**: Designed for HTTP/2 concurrent request handling

## Best Practices

### Service Configuration
1. **Consistent Message Receiver**: Use `axis2_json_rpc_msg_recv` for all JSON operations
2. **HTTP/2 Parameters**: Enable HTTP/2 optimization parameters at service level
3. **REST Mapping**: Define HTTP method and path parameters for REST-style access
4. **Content Type Specification**: Explicitly specify `application/json` content types

### Operation Definition
```xml
<operation name="operationName">
    <description>Clear description of operation functionality</description>
    <messageReceiver class="axis2_json_rpc_msg_recv"/>
    <parameter name="httpMethod">POST</parameter>
    <parameter name="httpPath">/operationName</parameter>
    <parameter name="contentType">application/json</parameter>
    <parameter name="responseType">application/json</parameter>
</operation>
```

### JSON Payload Design
```json
{
    "operationName": [{
        "arg0": {
            "parameter1": "value1",
            "parameter2": "value2"
        }
    }]
}
```

## Troubleshooting

### Common Issues
1. **Wrong Message Receiver**: Using SOAP receiver for JSON requests
2. **Missing Content-Type**: Not specifying `application/json` header
3. **HTTP/2 Not Enabled**: Client not using HTTP/2 protocol
4. **Operation Mismatch**: JSON operation name not matching services.xml

### Debug Verification
Check engine logs for message receiver selection:
```
🏭 REQUEST PROCESSOR FACTORY: Creating JSON HTTP/2 processor for thread-safe processing
🚀 JSON Message Receiver: Processing HTTP/2 JSON request for operation 'startRecording'
```

## Conclusion

The services.xml messageReceiver configuration provides fine-grained control over how individual operations process requests. By configuring `axis2_json_rpc_msg_recv` for operations, services get:

1. **Automatic HTTP/2 Detection**: Runtime system automatically routes HTTP/2 + JSON requests to JSON processor
2. **Operation-Specific Processing**: Each operation can have its own message receiver and parameters
3. **Zero Configuration Overhead**: No axis2.xml changes needed - all configuration in services.xml
4. **Revolutionary Performance**: AXIOM-free processing provides significant performance benefits
5. **Backward Compatibility**: SOAP operations continue working alongside JSON operations

This design enables seamless hybrid services that support both modern HTTP/2 JSON clients and legacy SOAP clients with a single service configuration.