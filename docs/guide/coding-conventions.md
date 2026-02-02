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

```c
/* Always use environment allocator */
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
