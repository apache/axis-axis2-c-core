# User Guide Overview

Apache Axis2/C is a web services framework that supports both modern HTTP/2 JSON-RPC and legacy SOAP protocols.

## Core Concepts

### Services

A **service** is a deployable unit that exposes one or more operations. Services are packaged as shared libraries (`.so` on Linux, `.dll` on Windows) with a `services.xml` descriptor.

```
services/
└── MyService/
    ├── libmyservice.so
    └── services.xml
```

### Operations

An **operation** is a callable method within a service. Each operation has:

- A name (used in the URL path or SOAP action)
- A message receiver (JSON or SOAP)
- Optional handlers for pre/post processing

### Message Context

The **message context** carries request/response data through the processing pipeline:

- HTTP headers
- Protocol information (HTTP/2 vs HTTP/1.1)
- Client certificates (for mTLS)
- Service and operation references

## Architecture

### Processing Pipeline

```
Request → Transport → Handlers → Service → Handlers → Transport → Response
```

1. **Transport Layer**: HTTP/2 (nghttp2) or HTTP/1.1
2. **In Handlers**: Authentication, logging, validation
3. **Service Invocation**: Your business logic
4. **Out Handlers**: Response transformation
5. **Transport Layer**: Send response

### Module System

Axis2/C uses a module system for extensibility:

- **Rampart** - WS-Security (legacy SOAP)
- **mod_axis2** - Apache httpd integration

## Configuration Files

### axis2.xml

Main server configuration:

- Transport settings (HTTP/2, TLS)
- Module references
- Global handlers
- Service deployment paths

See [axis2.xml Configuration](axis2-xml.md).

### services.xml

Per-service configuration:

- Service description
- Operations
- Parameters
- Handlers

See [Services Guide](services.md).

## Protocol Support

### HTTP/2 + JSON (Recommended)

Modern protocol stack for new applications:

- Binary framing, multiplexing
- JSON request/response bodies
- TLS 1.3 with mTLS support

### Legacy SOAP/HTTP

For existing SOAP integrations:

- SOAP 1.1 and 1.2
- WS-* specifications
- WSDL support

## Next Steps

- [Configuration](axis2-xml.md) - Server configuration
- [Services](services.md) - Creating services
- [Clients](clients.md) - Building clients
