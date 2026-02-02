# Apache Rampart/C - WS-Security for Axis2/C

## Overview

[Apache Rampart/C](https://axis.apache.org/axis2/c/rampart/) is the security module for Apache Axis2/C, providing WS-Security implementation for SOAP web services.

## Features

Rampart/C implements the [OASIS WS-Security 1.0 specification](http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0.pdf):

- **XML Signature** - Sign SOAP messages for integrity and authentication
- **XML Encryption** - Encrypt SOAP messages for confidentiality
- **Username Token** - Password-based authentication
- **SAML Tokens** - Security Assertion Markup Language support
- **X.509 Certificates** - Certificate-based authentication
- **Security Policy** - WS-SecurityPolicy enforcement

## Release Status

### Rampart/C 2.0.0 (Planned)

A Rampart/C 2.0.0 release is planned to accompany Axis2/C 2.0.0 for compatibility with the updated core library.

**Note:** This may be the **final release** of Rampart/C. The WS-Security specification dates from 2004 and is considered mature legacy technology in 2026.

### Why Legacy?

The WS-Security standard was designed for SOAP/XML web services in an era before:

- **TLS 1.2/1.3** became ubiquitous (message-level security was needed when transport security was unreliable)
- **OAuth 2.0** (2012) and **OpenID Connect** (2014) emerged as modern authentication standards
- **JWT** (JSON Web Tokens, 2015) provided lightweight token-based security
- **REST APIs** largely replaced SOAP for new development

### Modern Alternatives

For new applications, consider these alternatives to WS-Security:

| Use Case | WS-Security Approach | Modern Alternative |
|----------|---------------------|-------------------|
| Transport security | WS-Security + HTTP | **TLS 1.3** (mandatory) |
| Authentication | Username Token / SAML | **OAuth 2.0 / OIDC** |
| API tokens | SAML assertions | **JWT** |
| Message signing | XML Signature | **JWS** (JSON Web Signature) |
| Message encryption | XML Encryption | **JWE** (JSON Web Encryption) |
| Service-to-service | WS-Trust / SAML | **mTLS** (mutual TLS) |

## When to Use Rampart/C

Rampart/C remains valuable for:

- **Legacy system integration** - Connecting to existing WS-Security enabled services
- **Compliance requirements** - Some industries mandate WS-Security for SOAP services
- **Government systems** - Many government web services still use SOAP/WS-Security
- **Enterprise SOA** - Existing Service-Oriented Architecture deployments

## Installation

Rampart/C is a separate Apache project. See the official documentation:

- **Website**: [https://axis.apache.org/axis2/c/rampart/](https://axis.apache.org/axis2/c/rampart/)
- **Download**: Available from Apache mirrors when released

## Configuration Example

```xml
<!-- axis2.xml - Enable Rampart module -->
<module ref="rampart"/>

<!-- Service-level security policy -->
<service name="SecureService">
    <module ref="rampart"/>
    <parameter name="InflowSecurity">
        <action>
            <items>UsernameToken</items>
            <passwordCallbackClass>my_password_callback</passwordCallbackClass>
        </action>
    </parameter>
</service>
```

## References

- [OASIS WS-Security 1.0](http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0.pdf) - Core specification
- [Apache Rampart/C Documentation](https://axis.apache.org/axis2/c/rampart/) - Official docs
- [WS-SecurityPolicy](http://docs.oasis-open.org/ws-sx/ws-securitypolicy/200702) - Policy language

---

*For new HTTP/2 JSON applications, see the [Security Guide](../SECURITY.md) for modern TLS configuration and best practices.*
