# JSON Processing: HTTP/1.1 vs HTTP/2 Architecture

## Overview

Axis2/C supports JSON processing through two distinct architectures:

| Aspect | HTTP/1.1 JSON | HTTP/2 JSON |
|--------|---------------|-------------|
| **Source Files** | `axis2_json_reader.c`, `axis2_json_writer.c` | `axis2_h2_transport_utils.c` |
| **JSON Library** | json-c | json-c |
| **Internal Format** | AXIOM XML tree (`axiom_node_t*`) | Native JSON (`json_object*`) |
| **SOAP Handling** | Full SOAP envelope processing | Minimal dummy envelope (bypassed) |
| **Performance** | Slower (JSON↔AXIOM conversion) | Faster (direct JSON processing) |
| **Recommendation** | Legacy/backward compatibility | **Recommended for new development** |

## Architecture Diagrams

### HTTP/1.1 JSON Processing Path

```
┌─────────────┐     ┌──────────────────────┐     ┌─────────────────┐     ┌─────────────┐
│ JSON Input  │────▶│ axis2_json_reader.c  │────▶│ axiom_node_t*   │────▶│ SOAP/Engine │
│             │     │ (json-c parsing +    │     │ (XML tree)      │     │ Processing  │
│             │     │  AXIOM conversion)   │     │                 │     │             │
└─────────────┘     └──────────────────────┘     └─────────────────┘     └─────────────┘
                              │
                              ▼
                    ┌──────────────────────┐
                    │ axis2_json_read_node │
                    │ (recursive JSON→XML) │
                    └──────────────────────┘
```

**Key Functions:**
- `axis2_json_reader_create_for_memory()` - Creates reader from JSON string
- `axis2_json_reader_read()` - Parses JSON and converts to AXIOM tree
- `axis2_json_reader_get_root_node()` - Returns `axiom_node_t*` (NOT `json_object*`)

### HTTP/2 JSON Processing Path

```
┌─────────────┐     ┌──────────────────────┐     ┌─────────────────┐     ┌─────────────┐
│ JSON Input  │────▶│ json_tokener_parse() │────▶│ json_object*    │────▶│ Service     │
│             │     │ (direct json-c)      │     │ (native JSON)   │     │ Handler     │
└─────────────┘     └──────────────────────┘     └─────────────────┘     └─────────────┘
                                                         │
                                                         ▼
                                               ┌─────────────────────┐
                                               │ Minimal SOAP Envelope│
                                               │ (framework compat,   │
                                               │  processing bypassed)│
                                               └─────────────────────┘
```

**Key Functions:**
- `json_tokener_parse()` - Direct json-c parsing
- `axis2_h2_transport_utils_process_http_post_request()` - HTTP/2 JSON handler
- No AXIOM conversion - JSON stays as `json_object*`

## Performance Comparison

### HTTP/1.1 JSON Overhead

The HTTP/1.1 path incurs significant overhead because:

1. **Double Parsing**: JSON is parsed by json-c, then converted to AXIOM XML
2. **Memory Allocation**: AXIOM tree requires additional memory for XML nodes
3. **Recursive Conversion**: `axis2_json_read_node()` recursively builds XML tree
4. **Type Mapping**: JSON types must be mapped to AXIOM element types

```c
/* HTTP/1.1: JSON → AXIOM conversion (axis2_json_reader.c) */
axis2_status_t axis2_json_read_node(
    json_object* parent,      /* Input: JSON object */
    const char* name,
    axiom_node_t** om_node,   /* Output: AXIOM XML node */
    const axutil_env_t* env)
{
    /* Recursive conversion of every JSON element to XML */
    json_object_object_foreach(parent, key, value) {
        /* Create AXIOM element for each JSON key */
        /* Recursively process nested objects/arrays */
    }
}
```

### HTTP/2 JSON Efficiency

The HTTP/2 path eliminates conversion overhead:

1. **Single Parse**: JSON is parsed once by json-c
2. **Native Access**: Services access `json_object*` directly
3. **No Conversion**: No AXIOM tree creation
4. **Minimal Envelope**: Dummy SOAP envelope satisfies framework without processing

```c
/* HTTP/2: Direct JSON processing (axis2_h2_transport_utils.c) */
axis2_status_t axis2_h2_transport_utils_process_http_post_request(
    const axutil_env_t* env,
    axutil_stream_t* in_stream,
    const axis2_char_t* content_type)
{
    /* Parse JSON directly - no conversion */
    json_obj = json_tokener_parse(json_string);

    /* Service receives json_object* directly */
    /* No AXIOM overhead */
}
```

## The Dummy SOAP Envelope Architecture

HTTP/2 mode uses a "dummy SOAP envelope" pattern (similar to Axis2/Java):

1. **Framework Compatibility**: Axis2/C core expects SOAP message context
2. **Minimal Overhead**: Empty/minimal envelope created for framework satisfaction
3. **Processing Bypass**: `IS_JSON_STREAM=true` flag signals JSON-only mode
4. **Direct Access**: JSON Message Receiver bypasses SOAP processing entirely

This achieves framework compatibility with near-zero overhead.

## When to Use Each Path

### Use HTTP/2 JSON (Recommended)

- New service development
- High-performance JSON APIs
- RESTful services
- Microservices architecture
- Large payload processing
- HTTP/2 multiplexed streams

### Use HTTP/1.1 JSON (Legacy)

- Existing services requiring backward compatibility
- Integration with systems that only support HTTP/1.1
- When AXIOM/XML interoperability is needed
- Legacy deployment environments

## History and Rationale

### HTTP/1.1 JSON Support

- **Origin**: Community contributed feature (original Axis2/C JSON support)
- **Design**: Built on top of existing AXIOM infrastructure
- **Trade-off**: Easier integration with existing code, but performance penalty
- **Status**: Maintained for backward compatibility, not recommended for new development

### HTTP/2 JSON Support

- **Origin**: HTTP/2 Pure JSON Architecture redesign
- **Design**: Direct json-c usage with minimal SOAP envelope
- **Trade-off**: Better performance, requires HTTP/2 infrastructure
- **Status**: Recommended for all new JSON service development

## Fuzzer Coverage

Both JSON paths are tested by dedicated fuzzers:

| Fuzzer | Tests | Entry Point |
|--------|-------|-------------|
| `fuzz_json_reader` | HTTP/1.1 JSON→AXIOM | `axis2_json_reader_create_for_memory()` |
| `fuzz_json_parser` | HTTP/2 direct json-c | `json_tokener_parse()` |

See `fuzz/fuzz_json_reader.c` and `fuzz/fuzz_json_parser.c` for implementation details.

## Related Documentation

- [HTTP/2 JSON User Guide](userguide/json-httpd-h2-userguide.md) - Deployment and testing
- [HTTP/2 Migration State](AXIS2C_HTTP2_MIGRATION_STATE.md) - Technical architecture details
- [Security Guide](SECURITY.md) - JSON security considerations
