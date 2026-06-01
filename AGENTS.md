# AGENTS.md — Apache Axis2/C

## Security Threat Model

See [SECURITY.md](SECURITY.md) for the full threat model, including:

- Project description (C-based SOAP/REST/JSON web services engine)
- Roles and trust levels
- Security boundaries: what is and is not a security issue
- Attack surface by component (HTTP/2, JSON, XML parsers, memory safety)
- Memory safety profile (buffer overflows, use-after-free, format strings)
- Build-time hardening flags
- Fuzz testing and OSS-Fuzz integration
- Dependency version requirements

For detailed deployment hardening, see [docs/SECURITY.md](docs/SECURITY.md).

## High-Priority Scan Areas

Modern Axis2/C deployments use **HTTP/2 with pure JSON** — not HTTP/1.1
with SOAP/XML. The scan should weight accordingly: HTTP/2 transport,
JSON processing, and the mod_axis2 Apache handler are the primary
attack surface. Legacy HTTP/1.1 and XML paths are secondary.

### 1. HTTP/2 Transport (primary production path)

The HTTP/2 transport handles all modern traffic via nghttp2 stream
multiplexing. Scan for memory safety in stream lifecycle management,
header processing (HPACK), request/response buffer handling, and
connection teardown under error conditions. The Apache httpd handler
is the production entry point.

Key files:
- `src/core/transport/h2/server/apache2/axis2_http2_handler.c` (production entry point)
- `src/core/transport/h2/sender/axis2_h2_transport_sender.c` (outbound HTTP/2)
- `src/core/receivers/axis2_http2_msg_recv.c` (HTTP/2 message receiver)
- `src/core/transport/http/util/axis2_h2_transport_utils.c` (HTTP/2 utilities)

### 2. JSON Processing (both HTTP/2 and HTTP/1.1 paths)

Two JSON processing paths exist with different risk profiles:

- **HTTP/2 path (primary):** Direct `json_tokener_parse()` via json-c
  with depth limit (64) and size limit (10MB). Native JSON objects
  bypass AXIOM/XML entirely. Fuzz-tested.
- **HTTP/1.1 path (legacy):** JSON-to-AXIOM conversion in
  `axis2_json_reader.c` (25K LOC). More complex, more attack surface.
  Fuzz-tested.

Scan both paths for integer overflow in size calculations (CVE-2020-12762
pattern) and stack exhaustion from deep nesting. The HTTP/2 JSON-RPC
message receiver dispatches methods to service operations — scan for
method name injection and type confusion.

Key files:
- `src/core/transport/http/util/axis2_h2_transport_utils.c` (HTTP/2 JSON)
- `src/core/transport/http/util/axis2_json_reader.c` (HTTP/1.1 JSON)
- `samples/user_guide/bigdata-h2-service/src/axis2_json_rpc_msg_recv_native.c` (HTTP/2 JSON-RPC dispatch)
- `samples/user_guide/bigdata-h2-service/src/axis2_json_msg_builder.c` (JSON message builder)
- `samples/user_guide/bigdata-h2-service/src/axis2_json_streaming_msg_formatter.c` (streaming JSON output)

### 3. Integer Overflow in Memory Allocation

C code is vulnerable to integer wrap-around in buffer size calculations.
Scan for patterns like `malloc(count * element_size)` where `count` or
`element_size` come from untrusted input without overflow checks.
Prioritize the HTTP/2 transport and JSON processing paths.

Key locations: JSON message builders, HTTP/2 stream buffer management,
attachment handlers, HTTP chunked transfer decoding.

### 4. Use-After-Free in Connection Handling

HTTP/2 stream multiplexing and HTTP/1.1 connection lifecycle management
involve complex cleanup ordering, creating risks for use-after-free
vulnerabilities. Scan for any use of connection, stream, or
request-related objects after they have been freed, especially in error
handling paths and connection teardown. Prioritize finding exploitable
UAFs over simple memory leaks.

Key files:
- `src/core/transport/h2/server/apache2/axis2_http2_handler.c`
- `src/core/transport/h2/sender/axis2_h2_transport_sender.c`
- `src/core/transport/http/sender/http_client.c`
- `src/core/transport/http/common/simple_http_svr_conn.c`

### 5. Format String Vulnerabilities

Verify that no user-controlled input reaches `printf`-family functions
or `AXIS2_LOG_*` macros as the format argument. The pattern
`AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, user_input)` is exploitable;
it must be `AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "%s", user_input)`.

Key locations: All logging calls in transport, receiver, and dispatcher
code that handle client-supplied data (URIs, headers, JSON-RPC method
names, SOAP fault strings).

### 6. XML Parser Security (libxml2 path, secondary)

When built with `--enable-libxml2`, verify that the custom external
entity loader (`axis2_secure_external_entity_loader`) cannot be
bypassed. The Guththila parser (default) is inherently XXE-safe.
Note: modern deployments primarily use HTTP/2 with JSON, making this
a secondary concern.

Key files:
- `axiom/src/parser/libxml2/libxml2_reader_wrapper.c`
- `guththila/src/guththila_xml_parser.c`

### 7. Buffer Safety in HTTP/1.1 Parsing (secondary)

Scan for `sprintf()` usage throughout the HTTP/1.1 transport code and
verify that buffer allocations are correctly sized. The `sprintf()` in
`http_header.c` uses a heap buffer sized from measured input lengths.

Key files:
- `src/core/transport/http/common/http_header.c`
- `src/core/transport/http/common/http_status_line.c`
- `src/core/transport/http/common/http_request_line.c`

## Project Structure

```
src/core/
  transport/h2/     HTTP/2 transport: sender, Apache httpd handler, tests
  transport/http/   HTTP/1.1 transport: server, sender, utilities, JSON
  receivers/        Message receivers (HTTP/2 and SOAP)
  engine/           Message processing engine
  deployment/       Service deployment
  dispatchers/      Request routing
axiom/              AXIOM object model + XML parser wrappers (Guththila, libxml2)
guththila/          Guththila XML parser (default, XXE-safe)
neethi/             WS-Policy framework
util/               Utility library (allocator, string, hash, linked list)
fuzz/               OSS-Fuzz targets (5 fuzzers)
test/               gtest-based unit tests + security tests
samples/            Sample services including HTTP/2 BigData and financial benchmarks
```

## Testing and Fuzzing

- **OSS-Fuzz:** Active integration with 5 libFuzzer targets (XML, JSON x2,
  HTTP headers, URLs). See `docs/OSS-FUZZ.md`.
- **Security tests:** `test/security/` (XXE, buffer safety, JSON)
- **Penetration tests:** `test/security/h2_penetration_test.sh` (HTTP/2 JSON)
- **ASAN:** Build with `--enable-asan` for AddressSanitizer coverage
- **Stress tests:** Framework stability validated under high concurrency
  (100+ requests), large payloads, and long-running connections — zero
  ASAN errors

## Reporting

Security vulnerabilities: **security@apache.org**
