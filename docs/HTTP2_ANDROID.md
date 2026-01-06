# Axis2/C Android Support

This document describes how Axis2/C supports Android deployment with HTTP/2 and JSON-RPC.

## Quick Reference for Claude

When working with Axis2/C Android support, remember:

1. **All builds use `-std=gnu99`** - Required for json-c header compatibility
2. **Android uses static linking** - No dlopen(), services use weak symbol registry
3. **Service code lives in applications** - Axis2/C core only has weak symbol stubs
4. **`--whole-archive` is required** - For weak/strong symbol resolution during linking

## Build System

### C Standard: Always gnu99

All Axis2/C builds use `-std=gnu99` (GNU C99 mode). This is required because:

1. **json-c headers** use C99 `inline` keyword
2. **Android NDK headers** require GNU extensions
3. **POSIX functions** are hidden by strict ANSI mode

**Implementation in all configure.ac files:**

```m4
if test "$GCC" = "yes"; then
    dnl Use gnu99 for json-c compatibility
    CFLAGS="$CFLAGS -std=gnu99 -Wall -Wno-implicit-function-declaration -D_GNU_SOURCE"
fi
```

The following configure.ac files all use this pattern:
- `configure.ac` (root)
- `util/configure.ac`
- `axiom/configure.ac`
- `neethi/configure.ac`
- `guththila/configure.ac`
- `samples/configure.ac`
- `tools/md5/configure.ac`
- `tools/tcpmon/configure.ac`

### Android Detection

Android builds are detected by the `--host` parameter:

```m4
AC_MSG_CHECKING([for Android target])
case "$host" in
    *-linux-android*|*-android*)
        android_build=yes
        AC_DEFINE([AXIS2_ANDROID_BUILD], [1], [Building for Android])
        ;;
    *)
        android_build=no
        ;;
esac
```

When `android_build=yes`:
- `AXIS2_ANDROID_BUILD` is defined
- `__ANDROID__` is typically already defined by the NDK compiler
- Static linking is expected

### Android NDK Cross-Compilation

**Environment setup:**

```bash
export ANDROID_NDK_HOME=$HOME/Android/Sdk/ndk/28.0.12916984
export TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64
export CC=$TOOLCHAIN/bin/aarch64-linux-android21-clang
export AR=$TOOLCHAIN/bin/llvm-ar
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
```

**Configure command:**

```bash
./configure \
    --host=aarch64-linux-android \
    --prefix=$DEPS \
    --enable-static \
    --disable-shared \
    --enable-http2 \
    --enable-json \
    --with-apr=$DEPS \
    --with-openssl=$DEPS
```

## Static Service Registry (Android)

### The Problem

Traditional Axis2/C loads services dynamically:

```c
void *lib = dlopen("libmyservice.so", RTLD_NOW);
func = dlsym(lib, "service_invoke_json");
```

Android with static linking has no `.so` files to load.

### Solution: Weak Symbol Architecture

Axis2/C uses weak symbols so applications can provide service implementations at link time.

**In `src/core/receivers/axis2_json_rpc_msg_recv.c`:**

```c
#ifdef __ANDROID__
typedef json_object* (*json_service_invoke_func_t)(
    const axutil_env_t *env, json_object *json_request);

/* Weak symbol - returns NULL if no implementation linked */
__attribute__((weak))
json_object* camera_control_service_invoke_json(
    const axutil_env_t *env, json_object *json_request)
{
    (void)env;
    (void)json_request;
    return NULL;
}

/* Lookup function - returns function pointer for known services */
json_service_invoke_func_t
android_static_service_lookup(const char *service_name)
{
    if (!service_name) return NULL;

    if (strcmp(service_name, "CameraControlService") == 0) {
        return camera_control_service_invoke_json;
    }
    /* Add more services here */

    return NULL;
}
#endif
```

### How It Works

```
[Axis2/C Framework]              [Application]
       |                               |
  weak symbol:                   strong symbol:
  camera_control_service_       camera_control_service_
  invoke_json() -> NULL          invoke_json() -> response
       |                               |
       +----------- Link Time ---------+
                     |
              Strong symbol wins
```

1. **Framework compiles** with weak symbol stubs (return NULL)
2. **Application provides** strong symbol implementations
3. **At link time**, strong symbols override weak symbols
4. **Result**: Application's service code handles requests

### Application Service Implementation

Applications provide two files:

**Adapter (bridges Axis2/C to app):**

```c
// axis2_static_service_adapter.c
#include <axutil_env.h>
#include <json-c/json.h>

extern int camera_control_service_impl(const char* req, char* resp, size_t size);

/* Strong symbol overrides weak in Axis2/C */
json_object* camera_control_service_invoke_json(
    const axutil_env_t *env, json_object *json_request)
{
    const char *req_str = json_object_to_json_string(json_request);
    char response[65536] = {0};

    camera_control_service_impl(req_str, response, sizeof(response));

    return json_tokener_parse(response);
}
```

**Implementation (app-specific logic):**

```c
// camera_control_service.c
int camera_control_service_impl(const char* req, char* resp, size_t size)
{
    /* Parse request, handle action, build response */
    snprintf(resp, size, "{\"result\":\"ok\"}");
    return 0;
}
```

### Critical: Functions Must Be Non-Static

Registry functions MUST NOT use `static`:

```c
/* WRONG - may be optimized away */
static json_service_invoke_func_t android_static_service_lookup(...) { }

/* CORRECT - global symbol preserved */
json_service_invoke_func_t android_static_service_lookup(...) { }
```

Static functions have internal linkage and may be excluded from static archives.

## Linking for Android

### Direct Linking Required

Libtool strips `--whole-archive` flags, so direct linking is necessary:

```bash
$CC -fPIC -o httpd modules.o buildmark.o \
    -Wl,--export-dynamic \
    -L$DEPS/lib \
    # Apache modules... \
    $DEPS/lib/libmod_axis2.a \
    -Wl,--whole-archive $DEPS/lib/libaxis2_engine.a -Wl,--no-whole-archive \
    -Wl,--whole-archive libapp_services.a -Wl,--no-whole-archive \
    # Other Axis2 libraries... \
    -ljson-c -lnghttp2 -laprutil-1 -lapr-1 \
    -lm -llog -pthread
```

**Key flags:**

| Flag | Purpose |
|------|---------|
| `-Wl,--whole-archive` | Include ALL objects from archive |
| `-Wl,--no-whole-archive` | Resume normal archive linking |
| `-Wl,--export-dynamic` | Export all symbols |
| `-llog` | Android logging |

### Verify Symbol Inclusion

```bash
nm httpd | grep -E "android_static|camera_control"

# Expected (T = global text):
# 00000000007b4998 T android_static_service_lookup
# 00000000007b3ae0 T camera_control_service_invoke_json
```

## services.xml Configuration

### REST Dispatcher Parameters

Use `RESTLocation` and `RESTMethod`, NOT `httpPath` and `httpMethod`:

```xml
<service name="CameraControlService">
    <operation name="getStatus">
        <messageReceiver class="axis2_json_rpc_msg_recv"/>
        <parameter name="RESTMethod">POST</parameter>
        <parameter name="RESTLocation">/getStatus</parameter>
        <parameter name="contentType">application/json</parameter>
    </operation>
</service>
```

## Adding New Services

### Step 1: Add Weak Symbol to Axis2/C

In `src/core/receivers/axis2_json_rpc_msg_recv.c`:

```c
#ifdef __ANDROID__
__attribute__((weak))
json_object* my_new_service_invoke_json(
    const axutil_env_t *env, json_object *json_request)
{
    (void)env; (void)json_request;
    return NULL;
}

json_service_invoke_func_t android_static_service_lookup(const char *svc)
{
    /* Existing services... */
    if (strcmp(svc, "MyNewService") == 0) {
        return my_new_service_invoke_json;
    }
    return NULL;
}
#endif
```

### Step 2: Implement in Application

Create adapter and implementation files in your application, then link with `--whole-archive`.

### Step 3: Create services.xml

Deploy to `services/MyNewService/services.xml` in the Axis2 repository.

## mod_axis2 Configuration

### Android-Specific Initialization

In `mod_axis2.c`, Android builds:
- Skip shared memory (SELinux restrictions)
- Use pool-based allocator
- Run in single-process mode (`-X` flag)

```c
#ifdef __ANDROID__
    /* Skip shared memory, use pool allocation */
    allocator = apr_palloc(pconf, sizeof(axutil_allocator_t));
    goto engine_init;
#endif
```

### Debug Logging

Android logging uses INFO level:

```c
#ifdef __ANDROID__
#include <android/log.h>
#define AXIS2_ANDROID_LOG(fmt, ...) \
    __android_log_print(ANDROID_LOG_INFO, "Axis2-mod", fmt, ##__VA_ARGS__)
#else
#define AXIS2_ANDROID_LOG(fmt, ...) ((void)0)
#endif
```

## HTTP/2 on Android

### Single-Process Mode Considerations

Apache runs with `-X` (single-process) on Android. For older devices, configure HTTP/2 conservatively:

```apache
# In http2-performance.conf
H2MaxSessionStreams 1      # Single stream for stability
H2StreamTimeout 30         # Prevent hanging streams
H2WindowSize 65535         # Reduced memory
H2Push off                 # Not needed for JSON-RPC
H2MinWorkers 1
H2MaxWorkers 2
```

## Summary Table

| Aspect | Desktop/Server | Android |
|--------|----------------|---------|
| C standard | `-std=gnu99` | `-std=gnu99` |
| Library type | Shared (.so) | Static (.a) |
| Service loading | dlopen() | Weak symbol registry |
| Service location | External .so | Application provides strong symbols |
| Linker | Libtool | Direct clang with `--whole-archive` |
| Apache mode | Multi-process | Single-process (`-X`) |

## Files Reference

Key files for Android support:

- `configure.ac` - Android detection, C flags
- `src/core/receivers/axis2_json_rpc_msg_recv.c` - Static service registry
- `src/core/transport/http/server/apache2/mod_axis2.c` - Android init, logging
- `src/core/transport/http/server/apache2/apache2_worker.c` - Request processing

---

*Last updated: 2026-01-06*
