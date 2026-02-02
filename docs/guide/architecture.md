# Architecture

This document describes the internal architecture of Apache Axis2/C.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Client Applications                       │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    Transport Layer                           │
│  ┌─────────────────────┐    ┌─────────────────────┐        │
│  │  HTTP/2 Transport   │    │  HTTP/1.1 Transport │        │
│  │  (nghttp2 + TLS)    │    │  (Legacy SOAP)      │        │
│  └─────────────────────┘    └─────────────────────┘        │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    Axis2/C Engine                            │
│  ┌─────────────────────────────────────────────────────┐   │
│  │                 Handler Chain                        │   │
│  │  In Flow  → [Handler] → [Handler] → [Dispatch]      │   │
│  │  Out Flow ← [Handler] ← [Handler] ←                  │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              Message Receivers                       │   │
│  │  ┌───────────────┐  ┌───────────────┐              │   │
│  │  │ JSON Receiver │  │ SOAP Receiver │              │   │
│  │  └───────────────┘  └───────────────┘              │   │
│  └─────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    Service Layer                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │  Service A  │  │  Service B  │  │  Service C  │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### Engine (`axis2_engine`)

The engine orchestrates request processing:

1. Receives requests from transport
2. Executes handler chains
3. Dispatches to appropriate service
4. Processes responses through out-flow handlers

### Message Context (`axis2_msg_ctx`)

Carries all information about a request/response:

```c
typedef struct axis2_msg_ctx {
    /* Transport info */
    axis2_transport_in_desc_t *transport_in;
    axis2_transport_out_desc_t *transport_out;

    /* Protocol info */
    axis2_bool_t is_http2;
    axis2_char_t *content_type;

    /* Security context */
    axis2_char_t *client_cert_dn;

    /* Service/operation */
    axis2_svc_t *svc;
    axis2_op_t *op;

    /* Request/response bodies */
    axiom_soap_envelope_t *soap_envelope;  /* SOAP */
    axiom_node_t *json_node;               /* JSON */
} axis2_msg_ctx_t;
```

### Handler Chain

Handlers process messages in phases:

```
Inflow Phases:
  Transport → PreDispatch → Dispatch → PostDispatch

Outflow Phases:
  PreSend → MessageOut → Transport
```

### Service Skeleton

Services implement the skeleton interface:

```c
typedef struct axis2_svc_skeleton_ops {
    /* Main request handler */
    axiom_node_t* (AXIS2_CALL *invoke)(
        axis2_svc_skeleton_t *svc_skeleton,
        const axutil_env_t *env,
        axiom_node_t *node,
        axis2_msg_ctx_t *msg_ctx);

    /* Error handler */
    axiom_node_t* (AXIS2_CALL *on_fault)(
        axis2_svc_skeleton_t *svc_skeleton,
        const axutil_env_t *env,
        axiom_node_t *node);

    /* Lifecycle */
    int (AXIS2_CALL *init)(axis2_svc_skeleton_t *svc_skeleton,
                           const axutil_env_t *env);
    int (AXIS2_CALL *free)(axis2_svc_skeleton_t *svc_skeleton,
                           const axutil_env_t *env);
} axis2_svc_skeleton_ops_t;
```

## Module System

Modules extend Axis2/C functionality:

```
modules/
├── addressing/
│   ├── libmod_addr.so
│   └── module.xml
└── rampart/
    ├── libmod_rampart.so
    └── module.xml
```

Modules can:

- Add handlers to the processing chain
- Provide utility functions
- Implement WS-* specifications (legacy)

## Memory Management

Axis2/C uses an environment-based allocator:

```c
/* Create environment with custom allocator */
axutil_env_t *env = axutil_env_create_all("app.log", AXIS2_LOG_LEVEL_INFO);

/* All allocations through environment */
char *str = AXIS2_MALLOC(env->allocator, 256);

/* Free through environment */
AXIS2_FREE(env->allocator, str);

/* Environment cleanup frees all tracked memory */
axutil_env_free(env);
```

## Thread Safety

Axis2/C supports multi-threaded deployment:

- Message contexts are per-request (not shared)
- Service instances can be shared (thread-safe skeleton required)
- Configuration is read-only after startup

## See Also

- [Interface Patterns](../HTTP2_INTERFACE_PATTERN_IMPLEMENTATION.md)
- [SPI Pattern](../HTTP2_SERVICE_PROVIDER_INTERFACE_PATTERN.md)
- [Library Loading](../HTTP2_LIBRARY_LOADING.md)
