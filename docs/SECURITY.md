# Apache Axis2/C Security Guide

This document provides security guidelines and best practices for deploying
and developing with Apache Axis2/C.

## XML Parser Security

### Recommended Parser Configuration

Axis2/C supports two XML parsers:

1. **Guththila (default)** - The recommended parser for production use
2. **libxml2** - Alternative parser with additional features

#### Guththila Parser (Default)

Guththila is inherently secure against XXE (XML External Entity) attacks because
it:
- Does not process external entities
- Does not load external DTDs
- Drops all unknown entity references

No additional configuration is needed for XXE protection with Guththila.

#### libxml2 Parser

When building with `--enable-libxml2`, the following security measures are
automatically applied:

- **XML_PARSE_NOENT**: Substitutes entities to prevent billion laughs attacks
- **XML_PARSE_NONET**: Forbids network access for external entities
- **XML_PARSE_NOXINCLUDE**: Prevents XInclude processing

These flags prevent:
- Local file disclosure via `file://` URLs
- Server-Side Request Forgery (SSRF) via `http://` URLs
- Denial of Service via exponential entity expansion

### XXE Attack Prevention

XXE (XML External Entity) attacks can occur when XML input containing external
entity references is processed unsafely. Axis2/C protects against these attacks
in both supported parsers.

**Attack vectors blocked:**

```xml
<!-- Local file inclusion (blocked) -->
<!DOCTYPE root [<!ENTITY xxe SYSTEM "file:///etc/passwd">]>
<root>&xxe;</root>

<!-- Remote file inclusion (blocked) -->
<!DOCTYPE root [<!ENTITY xxe SYSTEM "http://attacker.com/data">]>
<root>&xxe;</root>

<!-- Billion laughs DoS (mitigated) -->
<!DOCTYPE lolz [
  <!ENTITY lol "lol">
  <!ENTITY lol2 "&lol;&lol;&lol;&lol;&lol;">
  ...
]>
```

## SSL/TLS Security

### Protocol Configuration

Axis2/C SSL transport is configured to:

1. **Use TLS 1.2+ by default** (OpenSSL 1.1.0+)
2. **Disable SSLv2 and SSLv3** to prevent POODLE and DROWN attacks
3. **Disable compression** to prevent CRIME attacks
4. **Validate hostnames** against certificate CN/SAN fields

### Certificate Validation

SSL hostname validation is enabled by default (AXIS2C-1700). The client will
verify that the server's certificate matches the hostname being connected to.

To ensure proper certificate validation:

1. Provide a valid CA certificate file via the `SERVER_CERT` transport parameter
2. Ensure server certificates include the correct hostname in CN or SAN fields

### Recommended axis2.xml SSL Configuration

```xml
<transportSender name="https" class="axis2_http_sender">
    <parameter name="PROTOCOL">https</parameter>
    <parameter name="SERVER_CERT">/path/to/ca-bundle.crt</parameter>
</transportSender>
```

## HTTP Authentication

Axis2/C supports HTTP authentication schemes for securing web service communications.

### Supported Schemes

| Scheme | Status | Security Level | Use Case |
|--------|--------|----------------|----------|
| Basic | Supported | Low | Internal networks with TLS only |
| Digest | Supported | Medium | When Basic is insufficient |

### Security Considerations

1. **Basic Authentication**: Credentials are Base64-encoded (not encrypted).
   **Always use with TLS** to prevent credential interception.

2. **Digest Authentication**: More secure than Basic as passwords are hashed,
   but vulnerable to man-in-the-middle without TLS.

### NTLM Authentication - REMOVED

**NTLM support has been completely removed from Axis2/C.**

Microsoft is deprecating NTLM:
- [The evolution of Windows authentication](https://techcommunity.microsoft.com/blog/windows-itpro-blog/the-evolution-of-windows-authentication/4478637)

NTLM had critical security vulnerabilities including:
- Pass-the-hash attacks
- NTLM relay attacks
- Weak cryptographic algorithms (MD4/DES based)
- Session hijacking vulnerabilities

**Recommended alternatives:**
- **Kerberos** - For Windows/Active Directory environments
- **OAuth 2.0/OpenID Connect** - For modern web applications
- **Mutual TLS (mTLS)** - For service-to-service communication
- **SAML** - For enterprise SSO scenarios

The `--enable-heimdal` and `--enable-libntlm` configure options have been removed.

### Configuration Example

```xml
<transportSender name="https" class="axis2_http_sender">
    <parameter name="PROTOCOL">https</parameter>
    <parameter name="HTTP_AUTH_TYPE">Basic</parameter>
    <parameter name="HTTP_AUTH_USERNAME">user</parameter>
    <parameter name="HTTP_AUTH_PASSWORD">password</parameter>
</transportSender>
```

### Proxy Authentication

For HTTPS connections through a proxy (CONNECT tunneling):

```xml
<parameter name="PROXY_HOST">proxy.example.com</parameter>
<parameter name="PROXY_PORT">8080</parameter>
<parameter name="PROXY_AUTH_TYPE">Basic</parameter>
<parameter name="PROXY_AUTH_USERNAME">proxyuser</parameter>
<parameter name="PROXY_AUTH_PASSWORD">proxypass</parameter>
```

**Warning:** Avoid storing credentials in axis2.xml. Use environment variables
or a secrets manager where possible.

## Input Validation Guidelines

### For Service Developers

When developing Axis2/C services, follow these guidelines:

1. **Validate all input data** before processing
2. **Use parameterized queries** when interacting with databases
3. **Sanitize output** to prevent XSS in any web-facing responses
4. **Implement rate limiting** for public-facing services
5. **Log security events** (failed auth, invalid input, etc.)

### Buffer Safety

Axis2/C uses careful buffer management, but service developers should:

- Use `snprintf()` instead of `sprintf()` for formatted output
- Always validate buffer sizes before copying data
- Use `strncpy()` or `axutil_strdup()` for string operations
- Check return values from memory allocation

### Example: Safe String Handling

```c
/* Safe: Using snprintf with size limit */
char buffer[256];
snprintf(buffer, sizeof(buffer), "Value: %s", user_input);

/* Safe: Using axutil_strdup for heap allocation */
axis2_char_t *safe_copy = axutil_strdup(env, user_input);
if (!safe_copy) {
    /* Handle allocation failure */
}
```

## SOAP Fault Handling

### Information Disclosure Prevention

SOAP faults may contain sensitive information. Follow these guidelines:

1. **Production environments**: Configure minimal fault details
2. **Log detailed errors server-side** rather than returning them to clients
3. **Use generic error messages** for authentication/authorization failures
4. **Never include stack traces** in production fault responses

### Fault Configuration

In `axis2.xml`, configure fault handling:

```xml
<parameter name="sendStacktraceDetailsWithFaults">false</parameter>
```

## HTTP/2 and JSON Security (Axis2/C 2.0)

Axis2/C 2.0 introduces HTTP/2 support with pure JSON processing, bypassing
the traditional SOAP/XML pipeline. This section covers security considerations
specific to HTTP/2 JSON mode.

### JSON Parser Security

Axis2/C uses the [json-c](https://github.com/json-c/json-c) library for JSON
processing. The following security measures are implemented:

#### Nesting Depth Limits

To prevent stack exhaustion attacks (similar to CVE-2024-57699 in json-smart):

```c
/* Default: 64 levels of nesting */
#define AXIS2_JSON_MAX_DEPTH 64
```

Deeply nested JSON like `{"a":{"a":{"a":...}}}` beyond this limit is rejected.

#### Payload Size Limits

To prevent memory exhaustion and mitigate CVE-2020-12762 (integer overflow
in json-c):

```c
/* Default: 10 MB maximum payload */
#define AXIS2_JSON_MAX_PAYLOAD_SIZE (10 * 1024 * 1024)
```

Override these limits at compile time if your services require larger payloads:

```bash
CFLAGS="-DAXIS2_JSON_MAX_DEPTH=128 -DAXIS2_JSON_MAX_PAYLOAD_SIZE=52428800" ./configure ...
```

### Known json-c Vulnerabilities

| CVE | Description | Status |
|-----|-------------|--------|
| CVE-2020-12762 | Integer overflow with large JSON | Mitigated via payload size limit |

**Recommendation:** Use json-c 0.15 or later which includes security fixes.

### Apache httpd Security (mod_axis2)

When deploying Axis2/C with Apache httpd, keep httpd updated to address:

| CVE | Description | Fixed In |
|-----|-------------|----------|
| CVE-2024-40725 | Source code disclosure | 2.4.62 |
| CVE-2024-40898 | SSRF on Windows | 2.4.62 |
| CVE-2024-38476 | Information disclosure/SSRF | 2.4.60 |

**Recommendation:** Use Apache httpd 2.4.62 or later.

### Android/Kanaha Secure IPC Pattern

For Android deployments (like the Kanaha camera control example), follow
these security practices:

#### NEVER Use system() for IPC

```c
/* DANGEROUS - Command injection vulnerability */
system(intent_command);  /* DON'T DO THIS */

/* SAFE - Use fork()/execvp() */
pid_t pid = fork();
if (pid == 0) {
    char *args[] = {"am", "broadcast", "--user", "0", "-n", ...};
    execvp("am", args);
    _exit(1);
}
```

#### Input Validation

Implement defense-in-depth with both Java and C validation:

```java
// Java SecurityValidator (from Kanaha pattern)
public class SecurityValidator {
    private static final String[] PATH_TRAVERSAL = {"..", "//", "\\"};
    private static final String[] INJECTION = {"$(", "`", ";", "|", "&"};

    public static String validate(String input, String paramName) {
        if (input == null || input.isEmpty()) return null;
        String lower = input.toLowerCase();

        for (String pattern : PATH_TRAVERSAL) {
            if (lower.contains(pattern)) return "path traversal";
        }
        for (String pattern : INJECTION) {
            if (lower.contains(pattern)) return "injection attempt";
        }
        if (!input.matches("^[a-zA-Z0-9_\\-\\.\\*]+$")) {
            return "invalid characters";
        }
        return null;  // Valid
    }
}
```

```c
/* C-side validation (defense in depth) */
if (params->pattern && strstr(params->pattern, "..")) {
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "Security: path traversal detected in pattern");
    return AXIS2_FAILURE;
}
```

### HTTP/2 Specific Considerations

1. **HPACK Header Compression**: HTTP/2 uses HPACK for header compression.
   The nghttp2 library handles this securely, but ensure you're using
   nghttp2 1.50.0 or later.

2. **Stream Multiplexing**: HTTP/2 multiplexes multiple requests over a single
   TCP connection. This means multiple concurrent requests from different clients
   or contexts may share connection state. Service implementations should:
   - Avoid storing request-specific data in connection-level structures
   - Ensure thread-safe access to any shared resources
   - Not assume one-request-per-connection isolation (as in HTTP/1.1)
   - Be aware that a slow or resource-intensive request could affect other
     streams on the same connection

## Transport Limits and DoS Protection

### Built-in Size Limits

Axis2/C enforces several size limits to prevent denial-of-service attacks:

| Limit | Default | Compile-time Override |
|-------|---------|----------------------|
| HTTP header buffer | 4 KB | `AXIS2_HTTP_HEADER_LENGTH` |
| HTTP status line | 512 bytes | `AXIS2_HTTP_STATUS_LINE_LENGTH` |
| Chunked transfer max | 100 MB | `AXIS2_CHUNKED_CONTENT_LENGTH` |
| JSON payload max | 10 MB | `AXIS2_JSON_MAX_PAYLOAD_SIZE` |
| JSON nesting depth | 64 levels | `AXIS2_JSON_MAX_DEPTH` |

Override at compile time:

```bash
CFLAGS="-DAXIS2_JSON_MAX_PAYLOAD_SIZE=1048576" ./configure ...  # 1MB limit
```

### Timeout Configuration

Configure socket timeouts to prevent slowloris-style attacks:

```c
/* In code - set read timeout in milliseconds */
axis2_http_socket_read_timeout = 30000;  /* 30 seconds */
```

### Connection Limits

When deploying behind Apache httpd with mod_axis2:

```apache
# In httpd.conf
MaxRequestWorkers 150
Timeout 60
KeepAliveTimeout 5
MaxKeepAliveRequests 100
LimitRequestBody 10485760  # 10MB
LimitRequestFields 50
LimitRequestFieldSize 4094
```

## Build Hardening

### Recommended Compiler Flags

For production deployments, build Axis2/C with security hardening flags:

```bash
CFLAGS="-fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE -Wformat -Wformat-security" \
LDFLAGS="-pie -Wl,-z,relro,-z,now" \
./configure [options]
```

| Flag | Protection |
|------|------------|
| `-fstack-protector-strong` | Stack buffer overflow detection |
| `-D_FORTIFY_SOURCE=2` | Runtime buffer overflow checks for string/memory functions |
| `-fPIE -pie` | Full ASLR for executables |
| `-Wformat-security` | Format string vulnerability warnings |
| `-Wl,-z,relro,-z,now` | Full RELRO - protects GOT from overwrites |

### Development Builds with AddressSanitizer

For development and testing, enable AddressSanitizer to detect memory errors:

```bash
./configure --enable-asan [other options]
make
```

This detects:
- Use-after-free
- Buffer overflows (stack, heap, global)
- Memory leaks
- Double-free

**Note:** Do not use `--enable-asan` in production - it adds significant overhead.

### Debug Logging

The `--enable-trace` flag enables verbose debug logging. **Never enable in production**
as it may log sensitive data including message contents.

## Deployment Hardening

### Principle of Least Privilege

1. **Run as non-root**: Create a dedicated service account
   ```bash
   useradd -r -s /bin/false axis2
   chown -R axis2:axis2 /usr/local/axis2c
   ```

2. **File permissions**:
   ```bash
   chmod 640 /usr/local/axis2c/axis2.xml      # Config readable by service only
   chmod 600 /path/to/server.key               # Private keys owner-only
   chmod 644 /path/to/ca-bundle.crt           # CA certs world-readable OK
   chmod 750 /usr/local/axis2c/logs           # Logs directory
   ```

### Network Binding

By default, the standalone server binds to all interfaces (0.0.0.0).
For internal services, bind to localhost only:

```bash
./axis2_http_server -p 8080 -i 127.0.0.1
```

### Reverse Proxy Deployment

For internet-facing deployments, place Axis2/C behind a reverse proxy:

```
[Internet] → [nginx/Apache] → [Axis2/C on localhost]
```

Benefits:
- TLS termination at the proxy
- Rate limiting and WAF rules
- Request filtering
- Connection limits

Example nginx configuration:

```nginx
location /axis2/ {
    proxy_pass http://127.0.0.1:8080/;
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;

    # Rate limiting
    limit_req zone=axis2_limit burst=20 nodelay;

    # Size limits
    client_max_body_size 10m;
}
```

### Reference Implementation

For a working example of these security patterns applied to an Android camera
control system, see the Kanaha project's security documentation:

- [Kanaha SECURITY.md](https://github.com/robertlazarski/kanaha/blob/main/docs/SECURITY.md) -
  mTLS configuration, audit logging, Apache httpd hardening, user-agent filtering,
  certificate revocation procedures

**Note:** Kanaha is an independent project (GPLv3 licensed due to OpenCamera
dependency) and is not affiliated with Apache Software Foundation. It demonstrates
production deployment patterns for the CameraControlService sample in
`samples/user_guide/camera-control-service/`.

## Logging Security

### What to Log

- Authentication failures (username, source IP, timestamp)
- Authorization failures
- Invalid input attempts
- Service errors (without sensitive details)
- Connection anomalies

### What NOT to Log

**Never log these values:**
- Passwords or authentication tokens
- Credit card numbers or PII
- Full SOAP message bodies in production
- Private keys or certificates

### Log Injection Prevention

If logging user-supplied data, sanitize newlines and control characters:

```c
/* Remove newlines to prevent log injection */
char *sanitized = axutil_strdup(env, user_input);
for (char *p = sanitized; *p; p++) {
    if (*p == '\n' || *p == '\r') *p = ' ';
}
AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "User input: %s", sanitized);
```

### Trace Mode Warning

Building with `--enable-trace` enables verbose logging that may include
message contents. **This flag must never be used in production builds.**

## Dependency Security

### Minimum Secure Versions

| Dependency | Minimum Version | Critical CVEs Addressed |
|------------|-----------------|------------------------|
| OpenSSL | 1.1.1k+ | CVE-2021-3449, CVE-2021-3450 |
| libxml2 | 2.9.10+ | CVE-2020-24977, CVE-2019-20388 |
| json-c | 0.15+ | CVE-2020-12762 (integer overflow) |
| nghttp2 | 1.50.0+ | CVE-2023-44487 (rapid reset) |
| Apache httpd | 2.4.62+ | CVE-2024-40725, CVE-2024-40898 |

### Checking Installed Versions

```bash
# OpenSSL
openssl version

# libxml2
pkg-config --modversion libxml-2.0

# json-c
pkg-config --modversion json-c

# nghttp2
pkg-config --modversion libnghttp2
```

### Supply Chain Security

- Verify package checksums when downloading dependencies
- Use distribution packages where possible (apt, yum) for automatic security updates
- Subscribe to security mailing lists for critical dependencies

## WS-Security (Rampart)

Axis2/C supports WS-Security through the Apache Rampart/C module, which provides:

- XML Signature (signing SOAP messages)
- XML Encryption (encrypting SOAP messages)
- Username Token authentication
- SAML token support
- Security policy enforcement

Rampart/C is a separate project. See:
https://axis.apache.org/axis2/c/rampart/

**Note:** XML Signature is vulnerable to signature wrapping attacks if not
properly configured. Ensure you validate the signed elements match the
elements being processed.

## Security Checklist

Before deploying Axis2/C in production:

### Core Security
- [ ] Use Guththila parser (default) or verify libxml2 security flags
- [ ] Configure SSL/TLS with valid certificates
- [ ] Disable SSL compression and weak protocols
- [ ] Enable hostname validation
- [ ] Implement input validation in all services
- [ ] Configure appropriate logging
- [ ] Disable stack traces in SOAP faults
- [ ] Apply principle of least privilege to service accounts
- [ ] Keep Axis2/C and dependencies updated

### Build & Deployment
- [ ] Build with hardening flags (-fstack-protector-strong, -D_FORTIFY_SOURCE=2, etc.)
- [ ] Run as non-root user with minimal permissions
- [ ] Bind to localhost if not internet-facing
- [ ] Place behind reverse proxy for public services
- [ ] Configure appropriate timeout values
- [ ] Verify all dependencies are at minimum secure versions

### HTTP/2 JSON Mode (Axis2/C 2.0)
- [ ] Use json-c 0.15 or later
- [ ] Verify JSON depth and payload limits are appropriate for your services
- [ ] Update Apache httpd to 2.4.62 or later
- [ ] Use nghttp2 1.50.0 or later
- [ ] Implement input validation for all JSON fields
- [ ] For Android: Use fork()/execvp() instead of system() for IPC

## Reporting Security Issues

To report security vulnerabilities in Apache Axis2/C, please follow the
Apache Security Team guidelines:

https://www.apache.org/security/

Do not report security issues via public GitHub issues or mailing lists.

## References

### XML/SOAP Security
- [OWASP XML External Entity Prevention Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/XML_External_Entity_Prevention_Cheat_Sheet.html)
- [CWE-611: Improper Restriction of XML External Entity Reference](https://cwe.mitre.org/data/definitions/611.html)

### TLS/SSL Security
- [OWASP Transport Layer Protection Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Transport_Layer_Protection_Cheat_Sheet.html)
- [CWE-295: Improper Certificate Validation](https://cwe.mitre.org/data/definitions/295.html)

### JSON Security
- [json-c Security Vulnerabilities (CVE Details)](https://www.cvedetails.com/vulnerability-list/vendor_id-13247/Json-c-Project.html)
- [CVE-2020-12762: json-c Integer Overflow](https://www.cvedetails.com/cve/CVE-2020-12762/)

### Apache httpd Security
- [Apache HTTP Server 2.4 Vulnerabilities](https://httpd.apache.org/security/vulnerabilities_24.html)
- [Apache HTTP Server Security in 2025 (Stack Watch)](https://stack.watch/product/apache/http-server/)

### HTTP/2 Security
- [nghttp2 Security Advisories](https://github.com/nghttp2/nghttp2/security)
- [HTTP/2 (RFC 7540) Security Considerations](https://datatracker.ietf.org/doc/html/rfc7540#section-10)
