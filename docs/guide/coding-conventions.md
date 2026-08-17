# Coding Conventions

This document describes the coding standards for Apache Axis2/C development.

## Naming Conventions

### Functions

```c
/* Module prefix + component + action */
axis2_svc_client_create()
axis2_msg_ctx_get_soap_envelope()
axutil_hash_set()

/* Private functions use static */
static axis2_status_t
internal_helper_function(const axutil_env_t *env);
```

### Types

```c
/* Struct typedef with _t suffix */
typedef struct axis2_svc_client axis2_svc_client_t;

/* Enum typedef with _t suffix */
typedef enum axis2_status {
    AXIS2_SUCCESS = 0,
    AXIS2_FAILURE = -1
} axis2_status_t;
```

### Macros

```c
/* All caps with underscores */
#define AXIS2_SUCCESS 0
#define AXIS2_CALL __cdecl
#define AXIS2_EXPORT __declspec(dllexport)
```

### Variables

```c
/* Snake case for local variables */
axis2_char_t *service_name;
int message_count;

/* All caps for constants */
const int MAX_BUFFER_SIZE = 8192;
```

## Code Structure

### Header Files

```c
#ifndef AXIS2_SVC_CLIENT_H
#define AXIS2_SVC_CLIENT_H

#include <axutil_env.h>
#include <axis2_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Type declarations */
typedef struct axis2_svc_client axis2_svc_client_t;

/* Function declarations */
AXIS2_EXPORT axis2_svc_client_t* AXIS2_CALL
axis2_svc_client_create(const axutil_env_t *env,
                        const axis2_char_t *client_home);

AXIS2_EXPORT void AXIS2_CALL
axis2_svc_client_free(axis2_svc_client_t *svc_client,
                      const axutil_env_t *env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_SVC_CLIENT_H */
```

### Source Files

```c
#include "axis2_svc_client.h"
#include <axis2_const.h>
#include <axutil_string.h>

/* Private struct definition */
struct axis2_svc_client {
    axis2_conf_ctx_t *conf_ctx;
    axis2_svc_t *svc;
    /* ... */
};

/* Implementation */
AXIS2_EXPORT axis2_svc_client_t* AXIS2_CALL
axis2_svc_client_create(const axutil_env_t *env,
                        const axis2_char_t *client_home)
{
    axis2_svc_client_t *svc_client = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_client = AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_client_t));
    if (!svc_client) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* Initialize fields */
    svc_client->conf_ctx = NULL;
    svc_client->svc = NULL;

    return svc_client;
}
```

## Memory Management

### Allocation

Use the environment allocator for anything that outlives the current function
or is passed to other Axis2/C code. The one exception is growing a buffer, which
needs raw `realloc` — see [Growing a buffer](#growing-a-buffer) below.

```c
char *buffer = AXIS2_MALLOC(env->allocator, size);
if (!buffer) {
    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    return NULL;
}

/* Use AXIS2_STRDUP for strings */
char *copy = axutil_strdup(env, original);
```

### Cleanup

```c
/* Check for NULL before free */
if (buffer) {
    AXIS2_FREE(env->allocator, buffer);
    buffer = NULL;
}

/* Free in reverse allocation order */
void
axis2_svc_client_free(axis2_svc_client_t *svc_client,
                      const axutil_env_t *env)
{
    if (!svc_client)
        return;

    if (svc_client->svc) {
        axis2_svc_free(svc_client->svc, env);
    }

    AXIS2_FREE(env->allocator, svc_client);
}
```

### What the allocator actually is

`env->allocator` is an indirection, not a fixed implementation, and the two
deployments put very different things behind it. The standalone server uses the
default implementation in `util/src/allocator.c`, which is a thin wrapper over
C's `malloc`, `realloc` and `free`. Under `mod_axis2` the allocator is replaced
wholesale, in `mod_axis2.c`, by functions built on APR pools:

| Macro | Standalone | mod_axis2 |
|-------|-----------|-----------|
| `AXIS2_MALLOC` | `malloc` | `apr_palloc(current_pool)` |
| `AXIS2_FREE` | `free` | **does nothing** |
| `AXIS2_REALLOC` | `realloc` | **returns NULL, unconditionally** |

An APR pool is a region that is allocated from cheaply and released all at once.
`apr_palloc` carves a block out of the pool and there is no call to hand one
block back; the memory returns when the pool itself is destroyed, which for
request-scoped work happens at the end of the request. Two consequences follow,
and neither is visible from the call site:

**`AXIS2_FREE` is a no-op under Apache.** Not a leak in the usual sense — the
pool reclaims everything eventually — but a loop that allocates and frees
repeatedly within one request holds every allocation until that request ends.
Peak memory can be far above what the code appears to use. Size such loops by
what they will hold at once, not by what is live at any instant.

**`AXIS2_REALLOC` cannot be used under Apache.** It has no pool implementation
("can't be easily implemented", says the source) and returns NULL every time.
A buffer-growth loop written on it works in development against the standalone
server and fails on the first growth in production. Do not write one.

### Growing a buffer

Because `AXIS2_REALLOC` is unusable in the deployment that matters, grow with
raw `malloc`/`realloc`, then copy the result into allocator memory and free the
scratch buffer before returning:

```c
/* Scratch buffer: raw malloc/realloc, because AXIS2_REALLOC returns NULL
 * under mod_axis2. It must not leave this function. */
char *temp = malloc(size);
/* ... grow with realloc() as needed ... */

/* Ownership crosses to the allocator here, at exactly one point. */
buffer = AXIS2_MALLOC(env->allocator, total + 1);
if (buffer) {
    memcpy(buffer, temp, total + 1);
}
free(temp);
```

The rule this enforces is the one to remember: **release a pointer with the same
allocator that produced it.** A raw-`malloc` pointer that escapes into code
which later calls `AXIS2_FREE` is a real leak under Apache, where that call does
nothing. An `apr_palloc` pointer passed to `free()` corrupts the heap. Keeping
the scratch buffer local and copying once keeps every pointer that travels
anywhere pool-owned, so the request pool cleans up after error paths for free.

`axis2_apache2_request_processor_json_impl.c` is the worked example, with the
reasoning recorded at the handoff.

## Error Handling

```c
/* Set error and return */
if (condition_failed) {
    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE, AXIS2_FAILURE);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Detailed error message");
    return AXIS2_FAILURE;
}

/* Check return values */
status = some_function(env);
if (status != AXIS2_SUCCESS) {
    /* Error already set by callee */
    return status;
}
```

## Documentation

```c
/**
 * Creates a new service client instance.
 *
 * @param env pointer to environment struct
 * @param client_home path to Axis2/C repository
 * @return newly created service client, NULL on error
 */
AXIS2_EXPORT axis2_svc_client_t* AXIS2_CALL
axis2_svc_client_create(const axutil_env_t *env,
                        const axis2_char_t *client_home);
```

## Formatting

- **Indentation**: 4 spaces (no tabs)
- **Line length**: 100 characters max
- **Braces**: Opening brace on same line for functions and control structures
- **Blank lines**: One between functions, none inside functions except for logical grouping

```c
if (condition) {
    do_something();
} else {
    do_other();
}

for (i = 0; i < count; i++) {
    process(items[i]);
}
```
