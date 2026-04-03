# LoginService — Apache Axis2/C HTTP/2 Sample

An Apache Axis2/C sample service demonstrating HTTP/2 with JSON-based authentication.

## Overview

LoginService provides user authentication over HTTP/2 using pure JSON (no SOAP/XML
dependency). It demonstrates session management, JWT token issuance, and validation.

## Operations

- **authenticate** (POST /authenticate) — authenticate with JSON credentials, returns JWT
- **validateToken** (POST /validateToken) — validate a JWT token, returns user info
- **logout** (POST /logout) — invalidate a session token

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
     -d '{"username":"user","password":"pass"}' \
     http://localhost:8080/services/LoginService/authenticate
```

## License

Licensed to the Apache Software Foundation (ASF) under the Apache License,
Version 2.0. See the top-level LICENSE file for details.
