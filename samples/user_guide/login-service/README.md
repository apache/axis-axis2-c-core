# LoginService — Apache Axis2/C HTTP/2 JSON sample

A partial port of the LoginService from the Axis2/Java guide
"Apache Axis2 JSON and REST with Spring Boot 3"
(`src/site/xdoc/docs/json-springboot-userguide.xml` in axis2-java-core). It
shows how a service receives and answers a JSON request over HTTP/2 with no
SOAP or XML anywhere in the path.

## Status: incomplete, and not a security example

Read this section before using anything here as a model.

**One operation is implemented: `doLogin`.** The deployment descriptor
(`services.xml`) declares three others — `authenticate`, `validateToken` and
`logout` — and **none of them exist in the code**. They were described before
they were written and never followed up. A request to any of the three reaches
a service that cannot answer it. Conversely, `doLogin`, the operation that does
work, is not declared in `services.xml` at all; the handler advertises it
through its own operations list instead.

**The token this service issues is not a JWT in any security sense.** It is
`base64(header) . base64(payload) . demo_signature`, where `demo_signature` is
that literal string. Nothing is signed. The header nevertheless declares
`"alg": "HS256"`, so the token *claims* an HMAC that was never computed — which
is worse than an obviously fake token, because it looks checkable.

`login_service_validate_jwt_token()` does not validate anything either. It
counts full stops and returns true for any string of ten characters or more
containing exactly two of them. No signature check, no expiry check, no issuer
check. It is exported but never called.

This matches the scope of the Java guide it was ported from, which says
plainly that the login "will return a simple token not meant for anything
beyond demos" and that JWT and JWE are **out of scope**. The intent there was
to show *where* real token handling would go. Nothing here is a starting point
for authentication; it is a starting point for JSON request handling.

For deployments that terminate authentication at the transport — mutual TLS,
for instance — none of this is needed at all.

## What works

`doLogin` takes a flat JSON object. `email` and `username` are both accepted
for the address field:

```json
{"email": "admin@example.com", "password": "admin123"}
```

and answers with:

```json
{"status": "...", "message": "...", "token": "...",
 "tokenType": "...", "expiresIn": 0, "responseTime": 0}
```

Three credential pairs are hardcoded in `src/login_service.c`:

| email | password |
|---|---|
| `admin@example.com` | `admin123` |
| `user@example.com`  | `user123`  |
| `test@example.com`  | `test123`  |

Note the request shape differs from the Java original, which nests arguments:
`{"doLogin":[{"arg0":{"email":"...","credentials":"..."}}]}`. The C port takes
the flat form above and names the second field `password`, not `credentials`.

## Building

Requires `--enable-json` and `--enable-http2`:

```sh
./configure --prefix=/usr/local/axis2c --enable-json --enable-http2
make && make install
```

`src/login_service.c` and `src/login_json_handler.c` are the whole service.
A third file, `src/login_service_handler.c`, used to sit beside them: a SOAP
skeleton left over from before the service went JSON-only, in no `Makefile.am`
and never compiled. It was removed in September 2026 — this sample is JSON
only, and the SOAP path had no remaining purpose.

`src/login_service.c` also exports `login_service_html_encode()`, which nothing
calls.

## If you want to finish the port

In rough order of what would make the sample honest:

1. Either implement `authenticate`, `validateToken` and `logout`, or remove
   them from `services.xml` and this file.
2. Declare `doLogin` in `services.xml` so the descriptor matches the code.
3. Sign the token, or stop calling it a JWT and drop the `alg` header.

## License

Licensed to the Apache Software Foundation (ASF) under the Apache License,
Version 2.0. See the top-level LICENSE file for details.
