---
type: architecture
created: 2026-04-06
last-verified: 2026-04-09
status: Active — Java A1/A2/A3 complete, C1 complete, mcpInputSchema deployed
---

# MCP Support for Apache Axis2/C

**Summary**: Axis2/C gains MCP (Model Context Protocol) support by porting from the
Axis2/Java reference implementation — the same pattern used successfully for HTTP/2.
The C implementation targets hardware where Java cannot run: edge devices, embedded
systems, Android phones. One MCP client (Claude Desktop, Claude API) connects to
both runtimes via identical protocol. One AI protocol, the full hardware spectrum.

MCP is JSON-RPC 2.0. Three required methods: `initialize`, `tools/list`, `tools/call`.
Two transports: stdio (simple, process-per-client) and HTTP/SSE (persistent server).
Existing Axis2/C infrastructure — json-c, Apache httpd, the service dispatch table —
covers everything needed. No new dependencies.

---

## Why C Port Comes After Java

The HTTP/2 feature followed this sequence: Java reference implementation → C port.
It worked because the wire protocol is well-specified and both runtimes share the
same transport abstraction (TransportListener/TransportSender in Java;
`axis2_transport_receiver_t`/`axis2_transport_sender_t` in C).

MCP is a better candidate than HTTP/2 was:

- **Simpler wire protocol**: JSON lines over stdio or HTTP. No binary framing,
  no HPACK compression, no stream multiplexing.
- **C already has the dispatch layer**: `route_operation()` in
  `financial_benchmark_service.c` already does what `tools/call` needs — parse
  action name, dispatch to handler, return JSON. The JSON-RPC 2.0 wrapper is
  the only addition.
- **stdio in C is trivial**: `fgets(stdin)` / `fprintf(stdout)`. In Java this
  requires thread management and stream handling. In C it is a 30-line loop.
- **No JVM startup**: Claude Desktop starts MCP servers as subprocesses. JVM
  cold-start is 1–3 seconds. A C binary starts in under 100ms. For stdio MCP
  (process-per-client), this is visible every time Claude Desktop opens.
- **Port is mechanical once Java proves the protocol**: After Axis2/Java validates
  the `initialize`/`tools/list`/`tools/call` handshake against Claude Desktop,
  the C port translates proven behavior rather than discovering it.

**Estimated new C code**: ~500 lines for stdio, ~800 for HTTP/SSE. The HTTP/2 port
was ~5,000 lines. MCP is lighter because 80% of the dispatch work already exists.

---

## What the Two Runtimes Provide

| | Axis2/Java MCP | Axis2/C MCP |
|---|---|---|
| **Target** | Enterprise service bus | Edge / embedded / mobile |
| **Hardware** | WildFly, Tomcat, cloud | Raspberry Pi, Android, IoT |
| **JVM startup** | 1–3 seconds | N/A — native binary |
| **Memory** | 512MB–2GB JVM heap | ~30 MB total process |
| **Tool catalog** | `axis2-openapi` annotation reflection | Static `mcp_tools[]` struct array |
| **Demo** | Claude calling springbootdemo-tomcat11 | Claude calling financial calculations on a $20 phone |
| **Apache contribution** | First Java framework with native MCP transport | First C middleware with MCP — nothing else exists |

Same MCP client. Same protocol. Full hardware spectrum: enterprise server to $20
Android phone.

---

## Current State (2026-04-09)

### What exists in Axis2/C today

| Artifact | Status |
|----------|--------|
| json-c integration | ✅ In use throughout financial benchmark service |
| HTTP/2 transport (`modules/transport-h2`) | ✅ Working, tested |
| `financial_benchmark_service.c` | ✅ portfolioVariance, monteCarlo VaR, scenarioAnalysis — all validated |
| Service dispatch table (`route_operation()`) | ✅ Reusable for MCP `tools/call` |
| Apache httpd handler | ✅ Running, mod_axis2 loaded |
| `axis2-transport-mcp` C module | ❌ Not started |
| stdio MCP mode | ✅ Done (C1) — finbench_mcp.c + finbench_mcp_main.c |
| HTTP/SSE MCP endpoint | ❌ Not started |

### What exists in Axis2/Java (the reference)

| Artifact | Status |
|----------|--------|
| `axis2-openapi` module | ✅ Serves `/openapi.json`, `/openapi.yaml`, `/swagger-ui` |
| `springbootdemo-tomcat11` | ✅ Reference implementation, Java 25 + Tomcat 11 |
| `/openapi-mcp.json` endpoint | ✅ Done (A1) |
| `axis2-mcp-bridge` stdio JAR | ✅ Done (A2) — with mTLS (port 8443, IoT CA) |
| A3 end-to-end validation | ✅ Done — Claude Desktop → bridge → mTLS → BigDataH2Service confirmed |
| `axis2-transport-mcp` Java native | ❌ Not started (B1) |

**C1 is now unblocked.** Java A2 was the gate: the JSON-RPC 2.0 handshake
(`initialize` / `tools/list` / `tools/call`) is validated against Claude Desktop.
The C port translates proven behavior rather than discovering it.

The Java bridge uses no MCP SDK — just Jackson + Java stdlib HttpClient. The C port
similarly needs no library beyond json-c, which is already in use throughout
`financial_benchmark_service.c`.

---

## Track C — C Implementation Plan

### C1: stdio MCP Mode in `financial_benchmark_service.c`

**When**: ✅ Done.

**How it works**: The binary detects whether stdin is a pipe (MCP client attached) or
a tty (interactive/httpd mode) and switches behavior accordingly:

```c
/* In main entry point or service init */
if (!isatty(fileno(stdin))) {
    finbench_run_mcp_stdio(env);
    return AXIS2_SUCCESS;
}
```

The stdio loop is JSON-RPC 2.0 framing around the existing dispatch:

```c
static void finbench_run_mcp_stdio(const axutil_env_t *env) {
    char line_buf[65536];
    while (fgets(line_buf, sizeof(line_buf), stdin)) {
        json_object *req = json_tokener_parse(line_buf);
        if (!req) continue;

        const char *method = /* json_object_get_string(req["method"]) */;
        json_object *id_obj = /* req["id"] */;

        if (strcmp(method, "initialize") == 0)
            finbench_mcp_write_result(id_obj, finbench_mcp_initialize());
        else if (strcmp(method, "tools/list") == 0)
            finbench_mcp_write_result(id_obj, finbench_mcp_tools_list());
        else if (strcmp(method, "tools/call") == 0)
            finbench_mcp_write_result(id_obj, finbench_mcp_dispatch(env, req));
        else
            finbench_mcp_write_error(id_obj, -32601, "Method not found");

        json_object_put(req);
    }
}
```

**Tool catalog**: Static struct array — no reflection, no overhead. Derived from the
same operation names as the HTTP/2 JSON dispatch:

```c
typedef axis2_char_t* (*finbench_mcp_handler_t)(
    const axutil_env_t *env,
    json_object *arguments);

typedef struct {
    const char *name;
    const char *description;
    const char *input_schema_json;  /* JSON string, inlined or from services.xml */
    finbench_mcp_handler_t handler;
} finbench_mcp_tool_t;

static const finbench_mcp_tool_t finbench_mcp_tools[] = {
    {
        "portfolioVariance",
        "Calculate portfolio variance using O(n^2) covariance matrix "
        "multiplication. Returns variance, volatility, annualized volatility, "
        "and timing in microseconds.",
        "{\"type\":\"object\",\"properties\":{"
            "\"n_assets\":{\"type\":\"integer\"},"
            "\"weights\":{\"type\":\"array\",\"items\":{\"type\":\"number\"}},"
            "\"covariance_matrix\":{\"type\":\"array\",\"items\":{\"type\":\"number\"}}"
        "},\"required\":[\"n_assets\",\"weights\",\"covariance_matrix\"]}",
        finbench_portfolio_variance_mcp_handler
    },
    {
        "monteCarlo",
        "Monte Carlo Value at Risk simulation using Geometric Brownian Motion. "
        "Returns VaR at 95%% and 99%% confidence, max drawdown, probability of "
        "profit, and simulations-per-second throughput.",
        "{\"type\":\"object\",\"properties\":{"
            "\"n_simulations\":{\"type\":\"integer\"},"
            "\"n_periods\":{\"type\":\"integer\"},"
            "\"initial_value\":{\"type\":\"number\"},"
            "\"expected_return\":{\"type\":\"number\"},"
            "\"volatility\":{\"type\":\"number\"}"
        "},\"required\":[]}",
        finbench_monte_carlo_mcp_handler
    },
    {
        "scenarioAnalysis",
        "Scenario analysis with probability-weighted expected return, "
        "upside/downside calculation, and O(n) vs O(1) hash lookup benchmark. "
        "Validates that scenario probabilities sum to 1.0.",
        "{\"type\":\"object\",\"properties\":{"
            "\"assets\":{\"type\":\"array\"}"
        "},\"required\":[\"assets\"]}",
        finbench_scenario_mcp_handler
    },
    { NULL, NULL, NULL, NULL }  /* sentinel */
};
```

Each `_mcp_handler` is a thin wrapper: extract arguments from `json_object *`,
call the existing `finbench_*_json_only()` function, return the result string.

**Claude Desktop config** (the demo deliverable):
```json
{
  "mcpServers": {
    "axis2c-financial": {
      "command": "/usr/local/axis2c/bin/financial-benchmark-mcp",
      "args": []
    }
  }
}
```

**Effort**: ~500 lines. Two to three days after Java A2 is validated.

---

### C2: HTTP/SSE MCP Endpoint

**When**: After C1 stdio works. Enables multiple Claude sessions sharing one
persistent C server over HTTP/2.

**Transport**: Reuses the existing Apache httpd handler. New routes:
- `POST /mcp` — JSON-RPC 2.0 request (same dispatch as stdio, different framing)
- `GET /mcp/sse` — `Content-Type: text/event-stream`; server pushes
  `data: {...}\n\n` per response

SSE in C via Apache httpd:
```c
/* Handler for GET /mcp/sse */
ap_set_content_type(r, "text/event-stream");
apr_table_set(r->headers_out, "Cache-Control", "no-cache");
apr_table_set(r->headers_out, "Connection", "keep-alive");
ap_rflush(r);

/* Response loop — write MCP responses as SSE events */
while (session_active) {
    json_object *mcp_response = /* next result */;
    const char *json_str = json_object_to_json_string(mcp_response);
    ap_rprintf(r, "data: %s\n\n", json_str);
    ap_rflush(r);
    json_object_put(mcp_response);
}
```

**Effort**: ~800 lines including session management. One week after C1.

---

### C3: `axis2-transport-mcp` C Module (Long Term — Apache Graduation)

Full port of the Java `axis2-transport-mcp` module to C's transport interfaces:
- Implements `axis2_transport_receiver_t` (TransportListener equivalent)
- Implements `axis2_transport_sender_t` (TransportSender equivalent)
- Translates MCP JSON-RPC 2.0 messages ↔ `axis2_msg_ctx_t` (MessageContext)
- Deployable as a proper Axis2/C module (`.mar` file), not embedded in a service

This is the Apache-grade contribution — the same architectural position in C as
`axis2-transport-mcp` in Java. Targets the Axis2/C release after the Java transport
is accepted by the Apache community.

---

## Key Design Decisions for C Port

**Why static tool catalog, not services.xml reflection**

C has no runtime reflection. Options are:
1. Static struct array (chosen for C1/C2) — zero overhead, explicit, easily audited
2. Parse `services.xml` at startup — more Axis2-idiomatic, allows adding tools
   without recompiling, required for C3 as a proper Axis2 module

C1 and C2 use option 1 because the financial benchmark service is a self-contained
demo. C3 uses option 2 because a general-purpose transport module must discover
services dynamically.

**Why `isatty()` for mode detection**

The same binary serves both HTTP/2 JSON (via Apache httpd as a shared library) and
MCP stdio (as a subprocess launched by Claude Desktop). The `isatty()` check is the
POSIX-standard way to detect whether stdin is connected to an interactive terminal or
a pipe. When Claude Desktop launches the binary, stdin is a pipe — MCP mode. When
httpd loads the shared library, stdin is not opened at all.

In production the binary and the shared library would be separate build targets from
the same source. The `isatty()` check handles the development/demo case where one
binary does both.

**Why input_schema_json is inlined as a string**

Avoids a JSON-within-JSON parse at startup and keeps the tool definition co-located
with its handler. For C3, the schema would be loaded from `services.xml` or a
companion `.json` file in the service directory, following the pattern the Axis2/C
service registry already uses for WSDL.

---

## Relationship to Java Reference Implementation

```
Java A1 (/openapi-mcp.json)        ✅ done — defines MCP tool schema format
         ↓
Java A2 (axis2-mcp-bridge stdio)   ✅ done — validates JSON-RPC 2.0 handshake
         ↓
Java B1 (axis2-transport-mcp)      ← proves native transport pattern
         ↓
C1 (stdio in financial-benchmark)  ✅ done — finbench_mcp.c, same protocol, static tool catalog
         ↓
C2 (HTTP/SSE endpoint)             ← port: same as Java A4/B2
         ↓
C3 (axis2-transport-mcp C module)  ← port: same transport abstraction in C
```

The Java schemas defined in `axis2-openapi` (`/openapi-mcp.json`) are the canonical
`inputSchema` for each tool. The C static struct array uses the same JSON schema
strings, ensuring Claude sees identical tool definitions regardless of which runtime
is serving them.

---

## Demo Deliverable

When C1 and Java B1 are both complete, the demonstration is:

```
Claude Desktop
    ├── axis2c-financial (stdio, $20 Android phone)
    │       portfolioVariance — 500 assets in 5ms, 30MB memory
    │       monteCarlo VaR    — 10,000 GBM simulations in 100ms
    │       scenarioAnalysis  — O(n) vs O(1) hash, 2,000 assets
    │
    └── axis2-java-services (HTTP/SSE, enterprise server)
            AssetCalculations — same calculations, JVM runtime
            BigDataH2Service  — HTTP/2 streaming, large payloads
```

Same Claude session. Same MCP protocol. One C binary on a $20 phone. One Java
service on an enterprise server. The protocol unifies them.

---

## Files to Create (C Port)

| File | Purpose |
|------|---------|
| `samples/user_guide/financial-benchmark-service/src/finbench_mcp.c` | stdio loop, tools/list, initialize, dispatch |
| `samples/user_guide/financial-benchmark-service/src/finbench_mcp.h` | Tool catalog struct, handler type, public API |
| `samples/user_guide/financial-benchmark-service/src/finbench_mcp_sse.c` | HTTP/SSE transport (C2) |
| `modules/transport-mcp/` | Full Axis2/C transport module (C3) |
| `docs/userguide/mcp-userguide.md` | User-facing setup guide |
