# Testing SOAP in Axis2/C

Most recent development here targets JSON over HTTP/2, and the documentation
reflects that. SOAP is still what the majority of deployments run, and most
security work lands on the SOAP paths, so this describes how to exercise them
without walking into the traps that make SOAP *look* broken when it is not.

Everything below was learned by getting it wrong first. The failure modes are
recorded alongside the fixes because each one produces a symptom that reads as
"SOAP is broken" while actually being a setup mistake.

## SOAP works on both transports

Both HTTP front ends handle SOAP. This is worth stating because the JSON
processor is prominent enough in `apache2_worker.c` to look like it has taken
over:

| Front end | Worker | SOAP handling |
|---|---|---|
| Apache httpd (`mod_axis2`) | `apache2_worker.c` | yes |
| Standalone `axis2_http_server` | `http_worker.c` | yes |

The JSON processor in the Apache worker is selected **at runtime**, not at
compile time:

```c
request_processor = axis2_apache2_request_processor_factory_create(env, request);
if (request_processor)
{
    if (axis2_apache2_request_processor_is_json_http2_request(request) ||
        strstr(request_processor->get_protocol_id(request_processor, env), "JSON"))
```

A SOAP request matches neither branch and falls through to the SOAP path. The
absence of `#ifdef AXIS2_JSON_ENABLED` around that call means the factory is
always *compiled in*, not that it always *runs*. Do not conclude from a grep for
`AXIS2_JSON_ENABLED` that SOAP has been compiled out.

## Trap 1: the sample axis2.xml has no HTTP receiver

`samples/server/axis2.xml` ships with the transport receiver commented out,
because that file is written for the Apache module where Apache *is* the
receiver:

```xml
<!--transportReceiver name="http" class="axis2_http_receiver">
    <parameter name="port" locked="false">6060</parameter>
</transportReceiver-->
```

Hand that config to the standalone server and it looks up its receiver, gets
NULL, and dereferences it:

```
#0  axis2_transport_in_desc_get_recv (transport_in=0x0, ...) at transport_in_desc.c:197
#1  axis2_http_server_create (...) at http_receiver.c:174
```

That crash is a missing `<transportReceiver>`, not a broken server. Uncomment
the block before using this config standalone. (`http_receiver.c:174` could
usefully check for NULL and report the real problem; until it does, recognise
the backtrace.)

## Trap 2: a service directory is not a deployed service

Copying a service directory into `services/` and reloading does not necessarily
register it. **Always confirm registration before drawing any conclusion about
SOAP**, by fetching the listing:

```bash
curl -s http://<host>/services | grep -i '<service'
```

An empty "Deployed Services" list means nothing is deployed, whatever is on
disk. A request to an unregistered service is answered by the JSON error
handler:

```json
{"error":{"code":400,"message":"Invalid JSON format","type":"HTTP2_JSON_ERROR"}}
```

That response says the service was not found. It does **not** say SOAP was
rejected, and mistaking the two is the easiest way to conclude the stack is
JSON-only when it is not.

## Trap 3: `build_for_tests.sh` destroys an existing configuration

It begins with `make distclean`. Never run it in a tree configured for
something you care about — a cross-compile setup, for instance. Build from a
clean copy instead:

```bash
git archive HEAD | tar -x -C /tmp/a2c-soap && cd /tmp/a2c-soap
```

## Trap 4: the installed binary is a libtool wrapper

`axis2_http_server` in the build tree is a shell script. Debuggers reject it:

```
"axis2_http_server": not in executable format: file format not recognized
```

The real ELF binary is in the adjacent `.libs/` directory. Use that under gdb.

## A working SOAP setup

The supported route produces a correct runtime layout; prefer it over
assembling a repository by hand.

```bash
# 1. Clean copy, plain build - no --enable-json, no --enable-http2 needed for SOAP
git archive HEAD | tar -x -C /tmp/a2c-soap
cd /tmp/a2c-soap
./autogen.sh
./configure --prefix=/tmp/a2c-soap/inst
make -j$(nproc) && make install

# 2. Uncomment the HTTP transportReceiver in the installed axis2.xml (Trap 1)

# 3. Engage addressing only if testing WS-Addressing
#    <module ref="addressing"/>

# 4. Run, and confirm it is actually listening before testing anything
AXIS2C_HOME=/tmp/a2c-soap/inst LD_LIBRARY_PATH=/tmp/a2c-soap/inst/lib \
  /tmp/a2c-soap/inst/bin/axis2_http_server -r /tmp/a2c-soap/inst
ss -ltn | grep 6060
```

A SOAP request carrying WS-Addressing headers, which is the shape needed for
anything touching `wsa:ReplyTo` or `wsa:FaultTo`:

```bash
curl -s -H 'Content-Type: text/xml; charset=UTF-8' \
  -H 'SOAPAction: "http://ws.apache.org/axis2/c/samples/echoString"' \
  --data-binary @request.xml http://127.0.0.1:6060/axis2/services/echo
```

```xml
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
               xmlns:wsa="http://www.w3.org/2005/08/addressing">
  <soap:Header>
    <wsa:To>http://127.0.0.1:6060/axis2/services/echo</wsa:To>
    <wsa:Action>http://ws.apache.org/axis2/c/samples/echoString</wsa:Action>
    <wsa:MessageID>urn:uuid:test-1</wsa:MessageID>
    <wsa:ReplyTo><wsa:Address>http://127.0.0.1:9099/sink</wsa:Address></wsa:ReplyTo>
  </soap:Header>
  <soap:Body>
    <ns:echoString xmlns:ns="http://ws.apache.org/axis2/c/samples"><text>hi</text></ns:echoString>
  </soap:Body>
</soap:Envelope>
```

## Which host to test on

A deployment configured for JSON over HTTP/2 is a poor place to test SOAP: its
services are JSON, and a SOAP request to a service it does not have produces the
misleading error above. Build a throwaway standalone server for SOAP work rather
than reaching for whichever instance happens to be running.

## Unit tests

`test/core/addr/test_addr.cc` covers the addressing data structures and the
WS-Addressing response endpoint policy. Add cases there rather than building an
ad-hoc harness — `make check` then runs them for everyone.

```bash
sh build_for_tests.sh   # in a clean copy - see Trap 3
sh run_tests.sh
```

## Reading test results

The suite has several red herrings that look like defects. Each one cost real
time before being recognised.

**Some tests need a populated `/usr/local/axis2c/`.** `test/cutest/include/cut_http_server.h`
starts its in-process server from `$AXIS2C_HOME`, falling back to
`DEFAULT_REPO_PATH`, which is `/usr/local/axis2c/`. `test_http_client` then
POSTs to `/axis2/services/echo/echo` and waits for a reply. With no deployed
echo service at that path it times out after ~2000 ms. `test_deployment`
depends on deployment state the same way. Neither failure means the transport
or SOAP is broken; they mean the environment has no deployment.

**`make check` and running a test binary directly do not agree.** The harness
supplies configuration a bare invocation does not, so a target can pass under
`make check` and fail when run by hand, or the reverse. Compare like with like
before concluding anything, and say which one you ran.

**`make check` interleaves output from every test binary.** A crash in the
combined log does not necessarily belong to the target that reported FAIL.
Re-run the single binary before attributing a backtrace to it.

**ASAN is preloaded into every process the harness spawns.** Leak reports
naming `/usr/bin/sed` or `/usr/bin/ls` are from those tools, not from Axis2/C.
Only stacks mentioning `axis2_`/`axutil_` frames are yours. A separate 15-byte
leak in the connection map is known and accepted — see
[`HTTP_CONNECTION_MAP_MEMORY_LEAK.md`](../../HTTP_CONNECTION_MAP_MEMORY_LEAK.md).

**Classify a failure by baselining, not by reasoning.** Build the same target
from `origin/master` and run it identically:

```bash
git archive origin/master | tar -x -C /tmp/a2c-base
cd /tmp/a2c-base && sh build_for_tests.sh
```

A story about why a failure must be pre-existing is not evidence, and it is
easy to reproduce your own mistake against a baseline and call that
confirmation. Only run the two side by side and compare.

## Linking tests against the HTTP libraries

`http_transport_utils.c` lives in `libaxis2_http_util`, while the workers live
in `libaxis2_http_common`. A test linking `common` needs `util` on the line as
well, **after** it, and after *every* occurrence — several test Makefile.am
files declare more than one program, and fixing only the first leaves the rest
failing one target at a time with the same unresolved symbols.

This only surfaces under `build_for_tests.sh`. The Apache module links `util`
explicitly and an ordinary `make` stops before the test targets, so both stay
green while the tests will not link.

## Checklist before reporting "SOAP is broken"

1. Is the service registered? Check the `/services` listing is non-empty.
2. Is a `<transportReceiver>` configured, for the standalone server?
3. Is the server actually listening? `ss -ltn`.
4. Is the failure from the SOAP path, or from the JSON error handler answering
   for a service that was never found?
