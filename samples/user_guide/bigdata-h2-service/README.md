# BigDataH2Service — Apache Axis2/C HTTP/2 Sample

An Apache Axis2/C sample service demonstrating HTTP/2 with large JSON payloads.

## Overview

BigDataH2Service processes large data sets over HTTP/2 using pure JSON (no SOAP/XML
dependency). It demonstrates HTTP/2 streaming, memory optimization, and concurrent
stream handling.

## Operations

- **processBigDataSet** (POST /processBigDataSet) — process a large JSON data set
- **getServiceMetadata** (GET /metadata) — return service capabilities

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
     -d '{"data":[]}' \
     http://localhost:8080/services/BigDataH2Service/processBigDataSet
```

## License

Licensed to the Apache Software Foundation (ASF) under the Apache License,
Version 2.0. See the top-level LICENSE file for details.
