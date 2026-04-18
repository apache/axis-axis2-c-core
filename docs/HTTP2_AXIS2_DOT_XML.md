# HTTP/2 and axis2.xml Configuration: Axis2/C vs Axis2/Java

## Executive Summary

**No axis2.xml changes are required for HTTP/2 and JSON support in Axis2/C.** This document explains why Axis2/C differs from Axis2/Java in this regard and provides a comprehensive comparison of the architectural approaches.

## Key Difference: No Configuration Changes Required

Unlike Axis2/Java, **Axis2/C automatically detects and handles HTTP/2 with JSON content without requiring any axis2.xml configuration changes**. The request processing is determined at runtime through intelligent header analysis rather than static configuration.

## Axis2/C: Runtime Detection Approach

### Automatic Protocol and Content-Type Detection

Axis2/C uses the `axis2_apache2_request_processor_is_json_http2_request()` function in `src/core/transport/http/server/apache2/axis2_apache2_request_processor_factory.c` to automatically detect:

1. **HTTP/2 Protocol Detection**: Checks `request->protocol` for "HTTP/2" or "HTTP/2.0" strings
2. **JSON Content-Type Detection**: Analyzes Content-Type header for:
   - `application/json`
   - `text/json`
   - `application/hal+json`
   - `application/vnd.api+json`

```c
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_apache2_request_processor_is_json_http2_request(request_rec* request)
{
    const axis2_char_t* content_type = NULL;
    const axis2_char_t* protocol = NULL;

    if (!request) return AXIS2_FALSE;

    /* Check protocol version */
    protocol = request->protocol;
    if (!protocol || (!strstr(protocol, "HTTP/2") && !strstr(protocol, "HTTP/2.0")))
    {
        return AXIS2_FALSE;
    }

    /* Check content type */
    content_type = apr_table_get(request->headers_in, "Content-Type");
    if (!content_type)
    {
        content_type = apr_table_get(request->headers_in, "content-type");
    }

    if (!content_type) return AXIS2_FALSE;

    /* JSON content type detection */
    return (strstr(content_type, "application/json") != NULL ||
            strstr(content_type, "text/json") != NULL ||
            strstr(content_type, "application/hal+json") != NULL ||
            strstr(content_type, "application/vnd.api+json") != NULL) ?
           AXIS2_TRUE : AXIS2_FALSE;
}
```

### Intelligent Processor Selection

The factory method at `axis2_apache2_request_processor_factory.c:66-135` implements a decision matrix:

1. **HTTP/2 + JSON Content-Type** → JSON Processor (thread-safe)
2. **HTTP/2 + Any Content-Type** → JSON Processor (assume modern client)
3. **HTTP/1.1 + SOAP Content-Type** → SOAP Processor (legacy compatibility)
4. **HTTP/1.1 + JSON Content-Type** → SOAP Processor (safe fallback)
5. **Default/Unknown** → SOAP Processor (maximum compatibility)

### Usage Example

Simply send an HTTP/2 request with the appropriate Content-Type header:

```bash
curl -k --http2 -H "Content-Type: application/json" \
     -X POST -d '{"getCameraStatus":[]}' \
     https://localhost:8443/axis2/services/CameraControlService
```

No axis2.xml configuration is needed - the system automatically routes to the appropriate processor.

## Axis2/Java: Static Configuration Approach

### Why Axis2/Java Requires axis2.xml Changes

Based on the Spring Boot user guide (`src/site/xdoc/docs/json-springboot-userguide.xml` in axis-axis2-java-core), Axis2/Java requires explicit axis2.xml configuration because it uses a **static configuration model** where message processors are pre-configured rather than dynamically selected.

### Required Axis2/Java Configuration Components

#### 1. Message Formatters
```xml
<message name="requestMessage">
    <messageFormatter contentType="application/json"
                      class="org.apache.axis2.json.moshi.JsonFormatter"/>
</message>
```

#### 2. Message Receivers
Required classes mentioned in the Java documentation:
- `JsonRpcMessageReceiver`
- `JsonInOnlyRPCMessageReceiver`

#### 3. Message Builders
- `JsonBuilder`

#### 4. Dispatchers and Handlers
- `JSONBasedDefaultDispatcher`
- `JSONMessageHandler`

#### 4. HTTP/2 Transport Sender
```xml
<transportSender name="h2"
                 class="org.apache.axis2.transport.h2.impl.httpclient5.H2TransportSender">
    <parameter name="PROTOCOL">HTTP/2.0</parameter>
    <parameter name="maxConcurrentStreams">100</parameter>
    <parameter name="initialWindowSize">65536</parameter>
    <parameter name="serverPushEnabled">false</parameter>
    <parameter name="connectionTimeout">30000</parameter>
    <parameter name="responseTimeout">300000</parameter>
    <parameter name="streamingBufferSize">65536</parameter>
    <parameter name="memoryPressureThreshold">0.8</parameter>
    <!-- Enterprise Big Data Configuration -->
    <parameter name="enableStreamingOptimization">true</parameter>
    <parameter name="enableMemoryOptimization">true</parameter>
    <parameter name="largePayloadThreshold">52428800</parameter> <!-- 50MB -->
</transportSender>
```

**Note on Buffer Sizes**: The 64KB values shown above (`initialWindowSize`, `streamingBufferSize`) are Axis2/Java defaults. Axis2/C uses an **incremental buffer growth** approach:
- **Initial allocation**: 64KB (efficient for IoT/camera payloads ~24 bytes)
- **Growth strategy**: Doubles on demand (64KB → 128KB → 256KB → ... → 10MB max)
- **Maximum limit**: 10MB (supports 500+ asset financial portfolios at ~5MB)
- **Service-level limit**: Configurable via `maxJSONPayloadSize` in services.xml (default 10MB)
- **Implementation note**: Uses standard C `malloc/realloc` since `AXIS2_REALLOC` is unreliable

**Memory Efficiency Comparison**:
| Payload | Axis2/C (incremental) | Static 10MB | Savings |
|---------|----------------------|-------------|---------|
| IoT/Camera (~24B) | 64KB | 10MB | 160x |
| Medium JSON (~50KB) | 64KB | 10MB | 160x |
| Large portfolio (~235KB) | 256KB | 10MB | 40x |

## Architectural Comparison

### Axis2/C Advantages

1. **Zero Configuration**: No axis2.xml modifications required
2. **Runtime Flexibility**: Dynamic processor selection based on actual request characteristics
3. **Backward Compatibility**: Existing SOAP services continue working unchanged
4. **Intelligent Routing**: HTTP/2 requests automatically get optimized processing
5. **Thread Safety**: HTTP/2 requests automatically use thread-safe processors

### Axis2/Java Advantages

1. **Explicit Control**: Developers can precisely control message processing pipeline
2. **Performance Optimization**: Pre-configured processors avoid runtime decision overhead
3. **Enterprise Features**: Rich configuration options for big data processing (50MB+ payloads)
4. **Monitoring**: Built-in performance metrics and optimization indicators

## Implementation Details

### Axis2/C Request Flow

1. **Request Reception**: Apache HTTP server receives request
2. **Header Analysis**: Factory analyzes HTTP protocol version and Content-Type header
3. **Processor Selection**: Factory selects appropriate processor (JSON vs SOAP)
4. **Processing**: Selected processor handles the request with appropriate optimizations
5. **Response Generation**: Response format matches request expectations

### Axis2/Java Request Flow

1. **Configuration Loading**: axis2.xml defines processing pipeline at startup
2. **Request Reception**: Pre-configured message receivers handle requests
3. **Format Detection**: Content-Type header routes to appropriate formatter
4. **Processing**: Fixed pipeline processes the request
5. **Response Generation**: Pre-configured formatters generate response

## Best Practices

### For Axis2/C Development

1. **No Configuration Changes**: Simply use HTTP/2 with appropriate Content-Type headers
2. **Test Both Protocols**: Verify services work with both HTTP/1.1 and HTTP/2
3. **Monitor Logs**: Check processor selection in debug logs
4. **Content-Type Headers**: Ensure clients send `Content-Type: application/json`

### For Axis2/Java Migration to Axis2/C

1. **Remove axis2.xml HTTP/2 Configuration**: Not needed in Axis2/C
2. **Simplify Deployment**: No message formatter/receiver configuration required
3. **Test Runtime Detection**: Verify automatic processor selection works as expected
4. **Update Documentation**: Inform clients that configuration is not needed

## Conclusion

The fundamental difference between Axis2/C and Axis2/Java lies in their architectural philosophy:

- **Axis2/Java**: Static configuration with explicit control through axis2.xml
- **Axis2/C**: Dynamic runtime detection with zero configuration

This makes Axis2/C significantly easier to deploy for HTTP/2 and JSON scenarios while maintaining full backward compatibility with existing SOAP services. The intelligent request processor factory ensures that each request type gets appropriate handling without any manual configuration.

For HTTP/2 and JSON support, developers can focus on service implementation rather than configuration management, as the framework handles protocol and content-type detection transparently.

## Streaming and Field Filtering: Java vs C Architecture

### JSON Serialization Model

| | Axis2/Java | Axis2/C |
|---|---|---|
| **Serialization** | Streaming — Moshi/GSON writes fields directly to a `JsonWriter` backed by an `OutputStream`. No intermediate tree. | Tree — json-c builds a `json_object*` in memory, then `json_object_to_json_string()` serializes the whole tree at once. |
| **Memory profile** | Only the current field's value is in memory during serialization. A 50 MB response never exists as a 50 MB buffer. | The full json-c tree is in memory before serialization. For typical responses (1-10 KB) this is negligible; for large responses Java is more memory-efficient. |

### HTTP/2 Streaming

| | Axis2/Java | Axis2/C |
|---|---|---|
| **Mechanism** | `FlushingOutputStream` wraps the transport `OutputStream` and flushes every 64 KB. The flush happens DURING Moshi/GSON serialization — each field written eventually triggers a flush, converting one buffered response into a stream of HTTP/2 DATA frames. | `ap_rflush(r)` called periodically during response generation via Apache httpd `mod_h2`. Achieves similar chunked delivery but is manual — each service must call `ap_rflush()` at the right points in its serialization code. |
| **Scope** | Framework-level — configure `MoshiStreamingMessageFormatter` in `axis2.xml` and ALL services get 64 KB streaming automatically. | Service-level — each service decides when to flush. The financial benchmark service uses the same 64 KB interval as Java. |

### Field Filtering (`?fields=`)

| | Axis2/Java | Axis2/C |
|---|---|---|
| **Approach** | True streaming (Option C): reflection-based selective serialization inside `MoshiStreamingMessageFormatter.writeFilteredObject()`. Non-selected fields are NEVER serialized, never buffered, never written to the wire. | Post-build tree filtering: json-c builds the full `json_object*` tree, then `finbench_filter_json_response()` deletes unwanted keys via `json_object_object_del()`, then re-serializes. The full tree is briefly in memory. |
| **Scope** | Framework-level — `FieldFilteringMessageFormatter` wraps any `MessageFormatter`. Configure once in `axis2.xml`, all services get `?fields=` for free. | Service-level — implemented in the financial benchmark service handler. Other C services would need their own filter call. |
| **Code** | ~320 lines (`FieldFilteringMessageFormatter.java`) + ~100 lines added to `MoshiStreamingMessageFormatter.java`) | ~60 lines in `financial_benchmark_service_handler.c` (`finbench_extract_fields_param` + `finbench_filter_json_response`) |

### Why the Difference

Axis2/Java has a **pluggable message formatter pipeline** in the kernel.
`MessageFormatter` is an interface that the engine calls to serialize
every outbound message. `FlushingOutputStream`, `FieldFilteringMessageFormatter`,
and `MoshiStreamingMessageFormatter` all live in this pipeline. Any
service deployed to any Axis2/Java instance gets these features by
changing one line in `axis2.xml`.

Axis2/C has **no equivalent pluggable JSON formatter pipeline**. The C
architecture was designed for SOAP (Axiom / libxml2). JSON services
bypass the SOAP pipeline entirely by handling serialization themselves
— each service builds its own json-c tree and calls
`json_object_to_json_string()`. This is simpler per-service but means
streaming and field filtering must be implemented in each service's
handler rather than once in the framework.

### Reusability Path for C

The `finbench_filter_json_response()` function is already generic —
it operates on any JSON string using json-c and `axutil_strdup`. To
make field filtering available to all Axis2/C JSON services:

1. Move `finbench_extract_fields_param()` and
   `finbench_filter_json_response()` into an `axutil` shared utility
   (e.g., `axutil_json_field_filter.c`).
2. In the JSON message receiver (`axis2_json_rpc_msg_recv`), check for
   `?fields=` on the request URI and apply the filter to the outbound
   JSON string before sending.
3. This would give all Axis2/C JSON services field filtering without
   per-service code — matching the Axis2/Java architecture.

For now, the per-service approach in the financial benchmark handler is
correct and KISS — it is ~60 lines of self-contained C that achieves
feature parity with the Axis2/Java deployment.

### Parity Reference

| Feature | Axis2/Java | Axis2/C | Parity |
|---------|-----------|---------|--------|
| HTTP/2 transport | `modules/transport-h2` | `mod_h2` via Apache httpd | ✅ |
| 64 KB streaming flush | `FlushingOutputStream` | `ap_rflush(r)` | ✅ |
| `?fields=` response filtering | `FieldFilteringMessageFormatter` | `finbench_filter_json_response()` | ✅ |
| Pluggable for all services | Yes (axis2.xml config) | No (per-service) | ⚠️ Java ahead |
| Streaming during serialization | Yes (Moshi → Okio → flush) | No (json-c tree → serialize) | ⚠️ Java ahead |
| MCP tool catalog | `openapi-mcp.json` auto-generated | `mcp_catalog_handler.c` | ✅ |
| Financial benchmark parity | portfolioVariance, monteCarlo, scenarioAnalysis | Same three services | ✅ |

See also:
- [Axis2/Java HTTP/2 Integration Guide](https://github.com/apache/axis-axis2-java-core/blob/master/src/site/xdoc/docs/http2-integration-guide.xml)
- [Axis2/Java FieldFilteringMessageFormatter](https://github.com/apache/axis-axis2-java-core/blob/master/modules/json/src/org/apache/axis2/json/streaming/FieldFilteringMessageFormatter.java)
- [Axis2/Java Streaming JSON Formatter](https://github.com/apache/axis-axis2-java-core/blob/master/src/site/xdoc/docs/json-streaming-formatter.xml)