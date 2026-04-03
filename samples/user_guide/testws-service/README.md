# TestwsService — Apache Axis2/C HTTP/2 Sample

An Apache Axis2/C sample service for testing HTTP/2 transport and security features.

## Overview

TestwsService exercises HTTP/2 transport features and demonstrates input validation,
XSS protection, and SQL injection protection patterns. It is intended as a test and
reference service, not for production use.

## Operations

- **testXSSProtection** (POST /testXSS) — test XSS input sanitization
- **testSQLInjection** (POST /testSQL) — test SQL injection protection
- **performanceTest** (POST /performanceTest) — measure processing time for various payload sizes
- **getTestStatus** (GET /status) — return service capabilities and test framework info

## Building

Requires `--enable-http2 --enable-json` at configure time:

```sh
./configure --prefix=/usr/local/axis2c --enable-http2 --enable-json
make
make install
```

## Testing

```sh
curl --http2-prior-knowledge \
     -H "Content-Type: application/json" \
     -d '{"input":"<script>alert(1)</script>"}' \
     http://localhost:8080/services/TestwsService/testXSS
```

## License

Licensed to the Apache Software Foundation (ASF) under the Apache License,
Version 2.0. See the top-level LICENSE file for details.
