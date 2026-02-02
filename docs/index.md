# Apache Axis2/C

**HTTP/2 JSON-RPC web services framework for C with legacy SOAP support**

Apache Axis2/C is a high-performance web services framework written in C. Originally designed for SOAP web services, the modern Axis2/C focuses on **HTTP/2** and **JSON-RPC** while maintaining backward compatibility with legacy SOAP deployments.

## Key Features

### Modern HTTP/2 + JSON Stack

- **HTTP/2 Protocol** - Multiplexed connections, header compression
- **JSON-RPC** - Lightweight request/response format for modern APIs
- **Native C Performance** - Minimal memory footprint, no garbage collection
- **Android Support** - Cross-compile for ARM64/ARMv7 mobile devices

### Security

- **TLS 1.2/1.3** - Modern cipher suites via OpenSSL
- **OSS-Fuzz Integration** - Continuous fuzzing for vulnerability detection

### Legacy SOAP Support

- [WS-Security via Rampart/C](https://axis.apache.org/axis2/c/rampart/) - XML Signature, Encryption, SAML
- WS-Addressing, WS-Policy
- WSDL 1.1/2.0 support
- MTOM/XOP binary attachments

**Note:** A Rampart/C 2.0.0 release is planned to accompany Axis2/C 2.0.0, but this may be the final release. The [WS-Security 1.0 specification](http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0.pdf) (OASIS 2004) is mature and considered legacy technology in 2026, with most new applications using TLS, OAuth 2.0, or JWT for security.

## Quick Start

```bash
# Clone from GitHub
git clone https://github.com/apache/axis-axis2-c-core.git
cd axis-axis2-c-core

# See INSTALL file for build instructions
cat INSTALL
```

**GitHub Repository:** [https://github.com/apache/axis-axis2-c-core](https://github.com/apache/axis-axis2-c-core)

For complete build instructions including HTTP/2 support, dependencies, and platform-specific options, see the `INSTALL` file in the repository root.

See [Installation Guide](getting-started/installation.md) for detailed instructions.

## Architecture

Axis2/C uses a modular architecture:

```
┌─────────────────────────────────────────────────┐
│              Client Application                 │
├─────────────────────────────────────────────────┤
│  HTTP/2 Transport    │    Legacy HTTP/1.1       │
│  (nghttp2 + OpenSSL) │    (SOAP Transport)      │
├─────────────────────────────────────────────────┤
│            Axis2/C Engine Core                  │
│  ┌─────────────┐  ┌─────────────┐              │
│  │ JSON-RPC    │  │ SOAP        │              │
│  │ Module      │  │ Module      │              │
│  └─────────────┘  └─────────────┘              │
├─────────────────────────────────────────────────┤
│              Service Handlers                   │
└─────────────────────────────────────────────────┘
```

## Documentation

- **[Getting Started](getting-started/installation.md)** - Installation and quick start
- **[User Guide](guide/overview.md)** - Configuration and usage
- **[HTTP/2 & JSON](HTTP2_JSON_REVOLUTION_STATE.md)** - Modern protocol support
- **[Android Support](HTTP2_ANDROID.md)** - Cross-compilation and mobile deployment
- **[Security Guide](SECURITY.md)** - TLS and security hardening
- **[Developer Guide](guide/architecture.md)** - Architecture and internals

## Community

- [GitHub Repository](https://github.com/apache/axis-axis2-c-core)
- [Mailing Lists](community/mailing-lists.md)
- [Issue Tracker](https://issues.apache.org/jira/projects/AXIS2C/summary)
- [Contributing Guide](community/contributing.md)

## License

Apache Axis2/C is licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

---

*Copyright © 2005-2026 The Apache Software Foundation*
