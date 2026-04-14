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

## Streaming JSON Message Formatter

Axis2/C 2.0.1 includes a streaming message formatter
(`axis2_json_streaming_msg_formatter`) that prevents reverse proxy
body-size rejections on large HTTP responses. Drop-in replacement for
the standard `axis2_json_msg_formatter`.

**How it works:** Instead of writing the entire JSON response in one
`axutil_stream_write()` call, the streaming formatter writes in
chunks of 64 KB and calls `axutil_stream_flush()` between chunks.
When running under Apache httpd + mod_axis2:

- `axutil_stream_write()` → `ap_rwrite()` → bucket brigade append
- `axutil_stream_flush()` → `ap_rflush()` → brigade flush → HTTP/2 DATA frame

The reverse proxy sees a stream of small chunks, never the full body.

**Problem solved (response-side):** Large JSON responses that cause
reverse proxy 502 Bad Gateway.

**NOT solved (request-side):** Large HTTP request bodies are a
client-side problem.

**Small responses (≤ 64 KB)** skip chunking entirely — written in one
call, same as the standard formatter. No overhead for typical payloads.

**Source files:**
- `src/axis2_json_streaming_msg_formatter.h`
- `src/axis2_json_streaming_msg_formatter.c`

**For the Axis2/Java equivalent, see:**
`org.apache.axis2.json.streaming.MoshiStreamingMessageFormatter` and
`org.apache.axis2.json.streaming.JSONStreamingMessageFormatter`.

## License

Licensed to the Apache Software Foundation (ASF) under the Apache License,
Version 2.0. See the top-level LICENSE file for details.
