# AGENTS.md — Apache Axis2/C

## Security Threat Model

See [SECURITY.md](SECURITY.md) for the full threat model, including:

- Project description (C-based SOAP/REST/JSON web services engine)
- Roles and trust levels
- Security boundaries: what is and is not a security issue
- Attack surface by component (XML parsers, JSON, HTTP headers, memory safety)
- Memory safety profile (buffer overflows, use-after-free, format strings)
- Build-time hardening flags
- Fuzz testing and OSS-Fuzz integration
- Dependency version requirements

For detailed deployment hardening, see [docs/SECURITY.md](docs/SECURITY.md).

## High-Priority Scan Areas

Because this is a C codebase, **memory safety vulnerabilities take
priority** alongside the logical vulnerabilities shared with Axis2/Java.

### 1. HTTP Header Buffer Overflow (known issue)

`http_header.c:150` uses unbounded `sprintf()` to format HTTP headers.
Maliciously long header names or values can overflow the stack buffer.
Scan for all `sprintf()` usage throughout the codebase and verify
adequate bounds checking.

Key files:
- `src/core/transport/http/common/http_header.c`
- `src/core/transport/http/common/http_status_line.c`
- `src/core/transport/http/common/http_request_line.c`

### 2. Format String Vulnerabilities

Verify that no user-controlled input reaches `printf`-family functions
or `AXIS2_LOG_*` macros as the format argument. The pattern
`AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, user_input)` is exploitable;
it must be `AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "%s", user_input)`.

Key locations: All logging calls in transport, receiver, and dispatcher
code that handle client-supplied data (URIs, headers, SOAP fault strings).

### 3. XML Parser Security (libxml2 path)

When built with `--enable-libxml2`, verify that the custom external
entity loader (`axis2_secure_external_entity_loader`) cannot be
bypassed. The Guththila parser (default) is inherently XXE-safe.

Key files:
- `axiom/src/parser/libxml2/libxml2_reader_wrapper.c`
- `guththila/src/guththila_xml_parser.c`

### 4. JSON Processing (both paths)

Two JSON processing paths exist with different risk profiles:

- **HTTP/2 path:** Direct `json_tokener_parse()` via json-c. Depth
  limit (64) and size limit (10MB) enforced. Fuzz-tested.
- **HTTP/1.1 path:** JSON-to-AXIOM conversion in `axis2_json_reader.c`
  (25K LOC). More complex, more attack surface. Fuzz-tested.

Scan both paths for integer overflow in size calculations (CVE-2020-12762
pattern) and stack exhaustion from deep nesting.

Key files:
- `src/core/transport/http/util/axis2_h2_transport_utils.c` (HTTP/2 JSON)
- `src/core/transport/http/util/axis2_json_reader.c` (HTTP/1.1 JSON)

### 5. Integer Overflow in Memory Allocation

C code is vulnerable to integer wrap-around in buffer size calculations.
Scan for patterns like `malloc(count * element_size)` where `count` or
`element_size` come from untrusted input without overflow checks.

Key locations: Message builders, attachment handlers, JSON/XML parsers,
HTTP chunked transfer decoding.

### 6. Use-After-Free in Connection Handling

HTTP connection lifecycle management involves complex cleanup ordering,
creating risks for use-after-free vulnerabilities. Scan for any use of
connection or request-related objects after they have been freed,
especially in error handling paths. A known memory leak exists in this
area, indicating complex memory management where UAFs may also be
present. Prioritize finding exploitable UAFs over simple memory leaks.

Key files:
- `src/core/transport/http/sender/http_client.c`
- `src/core/transport/http/common/simple_http_svr_conn.c`

## Project Structure

```
src/core/          Core engine: transport, deployment, engine, receivers,
                   dispatchers, context, util
axiom/             AXIOM object model + XML parser wrappers (Guththila, libxml2)
guththila/         Guththila XML parser (default, XXE-safe)
neethi/            WS-Policy framework
util/              Utility library (allocator, string, hash, linked list)
fuzz/              OSS-Fuzz targets (5 fuzzers)
test/              gtest-based unit tests + security tests
samples/           Sample services (camera-control, financial benchmarks)
src/modules/       Apache httpd modules (mod_addr, mod_log)
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
