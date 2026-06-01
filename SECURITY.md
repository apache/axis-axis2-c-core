# Security Threat Model — Apache Axis2/C

## Project Description

Apache Axis2/C is a SOAP and REST web services engine implemented in C.
It provides HTTP/1.1 and HTTP/2 transports, dual XML parsers (Guththila
and libxml2), native JSON processing (via json-c), MTOM/XOP attachment
handling, and hot-deployment of services. It runs as a standalone HTTP
server or as an Apache httpd module (`mod_axis2`).

Being a C implementation, memory safety vulnerabilities (buffer overflows,
use-after-free, format strings, integer overflows) are in scope in
addition to the logical vulnerabilities shared with Axis2/Java.

For detailed deployment hardening guidance, see
[docs/SECURITY.md](docs/SECURITY.md).

## Roles and Trust Levels

| Role | Trust Level | Description |
|------|-------------|-------------|
| **Server Administrator** | Fully trusted | Configures `axis2.xml`, compiles with chosen parser/transport options, manages TLS certificates, deploys services. |
| **Service Deployer** | Trusted | Deploys shared libraries (`.so`/`.dll`) into the services directory. Has filesystem write access. |
| **Authenticated Client** | Partially trusted | Identity verified by transport (TLS client cert), WS-Security (via Rampart/C), or application-level auth. |
| **Anonymous Client** | Untrusted | Remote caller with no credentials. Can reach any HTTP endpoint. All input is hostile. |

## Security Boundaries

### What IS a security issue

- **Remote Code Execution (RCE)** — buffer overflow, format string, or
  other memory corruption exploitable by a remote attacker through the
  Axis2/C framework.
- **Memory safety violations** — heap/stack buffer overflows,
  use-after-free, double-free, or null pointer dereferences reachable
  from untrusted input.
- **XML External Entity (XXE) injection** — the XML parser resolves
  external entities from untrusted input, enabling file read or SSRF.
  (Guththila is inherently immune; libxml2 requires hardening.)
- **Server-Side Request Forgery (SSRF)** — the server makes requests to
  arbitrary hosts due to attacker-controlled URLs in WSDL imports,
  external entities, or client-side HTTP transport.
- **Denial of Service via parser abuse** — XML bombs, deeply nested
  JSON (>64 levels), or payloads exceeding size limits (>10MB) that
  crash the process or exhaust memory.
- **Integer overflow** — size calculations in buffer allocation or
  payload processing that wrap around, leading to undersized buffers
  and subsequent overflows.
- **Format string vulnerabilities** — user-controlled data passed as
  the format argument to `printf`-family functions or logging macros.
- **Information disclosure** — stack traces, internal paths, or
  configuration details leaked in SOAP faults or HTTP error responses.
- **TLS/SSL vulnerabilities** — acceptance of deprecated protocols
  (SSLv2, SSLv3, TLS 1.0/1.1) or weak cipher suites in the HTTP
  client transport.

### What is NOT a security issue

- **Vulnerabilities in user-written services.** Input validation,
  SQL injection, and business logic flaws in deployed service
  implementations are the service author's responsibility.
- **Missing authentication.** Axis2/C does not ship built-in
  authentication. Securing endpoints is the responsibility of
  Rampart/C, the servlet container (Apache httpd), or a reverse proxy.
- **Hot-deployment with weak filesystem permissions.** If an attacker
  can write to the services directory, they can deploy arbitrary shared
  libraries. This is an OS-level access control issue.
- **Denial of service at the network level.** SYN floods, slowloris,
  and connection exhaustion are mitigated by Apache httpd or the
  network infrastructure, not by Axis2/C.
- **Vulnerabilities in optional, external modules.** Rampart/C
  (WS-Security) is a separate project.
- **The standalone HTTP server in production.** It is intended for
  development/testing. Production deployments should use `mod_axis2`
  behind a reverse proxy.

## Architecture and Attack Surface

### Message Processing Pipeline

```
Remote Client (untrusted input)
    |
    v
Apache httpd (TLS, connection limits, WAF)
  or standalone axis2_http_server (dev only)
    |
    v
mod_axis2 / HTTP request processor
    |
    v
Content-Type routing:
  JSON → json-c parser (HTTP/2) or JSON→AXIOM reader (HTTP/1.1)
  XML  → Guththila (default) or libxml2
    |
    v
AXIOM object model
    |
    v
Dispatchers → route to service/operation
    |
    v
Handler phases (Rampart/C security module executes here)
    |
    v
Message Receiver → invoke service function
    |
    v
Response serialization → transport out
```

### Attack Surface by Component

| Component | Threats | Mitigations |
|-----------|---------|-------------|
| **Guththila XML parser** | Malformed XML causing crashes; deep nesting stack exhaustion | Inherently XXE-safe (no entity processing); fuzz-tested via OSS-Fuzz |
| **libxml2 parser** (optional) | XXE, SSRF, billion laughs, entity expansion | `XML_PARSE_NONET`; custom entity loader blocks all external resolution; depth limit (256); attribute limit (256); namespace limit (64) |
| **HTTP header parsing** | Header injection (CRLF); malformed header parsing | `sprintf()` in `http_header.c` writes to a heap buffer sized from measured input lengths; fuzz-tested via OSS-Fuzz |
| **JSON parser** (json-c, HTTP/2) | Deep nesting stack exhaustion; integer overflow (CVE-2020-12762 pattern); large payload DoS | Depth limit (64 levels); payload size limit (10MB); fuzz-tested |
| **JSON-to-AXIOM reader** (HTTP/1.1) | Deep nesting; memory exhaustion during conversion | Fuzz-tested (`fuzz_json_reader`); size limits inherited from transport |
| **HTTP request/status line parsing** | Oversized lines; malformed methods/URIs | Status line limit (512 bytes); request line parsed by delimiter |
| **MTOM/XOP attachments** | Large attachment DoS; temp file exhaustion | Chunked transfer max (100MB default) |
| **WSDL/XSD parsing** | XXE in imported schemas; recursive schema DoS | Uses chosen XML parser with its hardening for XXE. **No explicit limits on schema complexity or recursion depth during validation — potential DoS risk.** |
| **HTTP client transport** (outbound) | TLS downgrade; weak ciphers; hostname bypass | TLS 1.2+ enforced; SSLv2/SSLv3/TLS 1.0-1.1 disabled; AEAD ciphers only; hostname validation enabled |
| **mod_axis2** (Apache httpd) | Shared memory corruption; configuration injection | apr_pool allocation; apr_global_mutex for thread safety |

### Transports

| Transport | Security Notes |
|-----------|----------------|
| HTTP/HTTPS (mod_axis2) | Production transport. TLS handled by Apache httpd. Recommended behind reverse proxy. |
| HTTP/HTTPS (standalone) | Development only. Single-threaded. No production-grade connection limits. |
| HTTP/2 (nghttp2) | Stream multiplexing. CVE-2023-44487 (rapid reset) mitigated by nghttp2 version. |

## Memory Safety Profile

As a C codebase, memory safety is a primary concern:

| Category | Risk Level | Notes |
|----------|-----------|-------|
| **Buffer overflow** | Low | HTTP header formatting uses `sprintf()` with correctly-sized heap allocations; `snprintf()` used elsewhere |
| **Use-after-free** | Low | Allocator-based memory management (`axutil_allocator_t`); ASAN testing |
| **Format string** | Low | Risk if user input reaches logging macros without `%s` format; code review addressed (Jan 2026) |
| **Integer overflow** | Low | Payload size limits prevent wrap-around in allocation; Gemini review findings addressed |
| **Null dereference** | Low | Defensive null checks throughout; `env` parameter validated at entry points |

### Build-Time Hardening

Production builds should use:
```
CFLAGS="-fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE -Wformat -Wformat-security"
LDFLAGS="-pie -Wl,-z,relro,-z,now"
```

AddressSanitizer is available for development via `--enable-asan`.

## Security Hardening History

### Features Removed (AXIS2C-1708, January 2026)

| Feature | Reason for Removal |
|---------|-------------------|
| NTLM authentication | Pass-the-hash vulnerabilities; Microsoft deprecating |
| TCP transport | No encryption, no authentication, incompatible with HTTP infrastructure |
| CGI transport | Obsolete deployment model |
| libcurl HTTP backend | Redundant with native client; MTOM broken; HTTP/2 uses nghttp2 |

### Hardening Applied (January-February 2026)

- TLS 1.0/1.1 completely disabled in client transport
- Strong cipher suites enforced (AEAD + forward secrecy)
- JSON payload size limits (10MB) and depth limits (64 levels) added
- libxml2 entity loader override blocks all external entity resolution
- OSS-Fuzz integration with 5 fuzz targets (XML, JSON x2, HTTP headers, URLs)
- Integer overflow and size_t consistency fixes from Gemini code review

## Fuzz Testing and OSS-Fuzz

Axis2/C has an active OSS-Fuzz integration with 5 fuzz targets:

| Target | Coverage |
|--------|----------|
| `fuzz_xml_parser` | Guththila AXIOM parsing |
| `fuzz_json_parser` | json-c direct parsing (HTTP/2 path) |
| `fuzz_json_reader` | JSON-to-AXIOM conversion (HTTP/1.1 path) |
| `fuzz_http_header` | HTTP header parsing |
| `fuzz_url_parser` | URL parsing and SSRF detection |

Additionally, `fuzz_finbench` tests the FinancialBenchmarkService at
the application level with integer overflow and boundary value inputs.

Local fuzzing validated with 8M+ iterations and zero ASAN errors.
See [docs/OSS-FUZZ.md](docs/OSS-FUZZ.md) for details.

## Dependency Security

| Dependency | Minimum Version | Key CVEs Tracked |
|-----------|----------------|------------------|
| OpenSSL | 1.1.1k+ | CVE-2021-3449, CVE-2021-3450 |
| libxml2 | 2.9.10+ | CVE-2020-24977, CVE-2019-20388 |
| json-c | 0.15+ | CVE-2020-12762 (integer overflow) |
| nghttp2 | 1.50.0+ | CVE-2023-44487 (HTTP/2 rapid reset) |
| Apache httpd | 2.4.62+ | CVE-2024-40725, CVE-2024-40898 |

Monthly CVE checks run via GitHub Actions (`.github/workflows/cve-check.yml`).

## Reporting Security Issues

Report vulnerabilities to: **security@apache.org**

Follow the [Apache Security Policy](https://www.apache.org/security/).
