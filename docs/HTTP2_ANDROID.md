# Axis2/C Android Support

This document describes how Axis2/C supports Android deployment with HTTP/2 and JSON-RPC.

## Quick Reference for Claude

When working with Axis2/C Android support, remember:

1. **All builds use `-std=gnu99`** - Required for json-c header compatibility
2. **Android uses static linking** - No dlopen(), services use weak symbol registry
3. **Service code lives in applications** - Axis2/C core only has weak symbol stubs
4. **`--whole-archive` is required** - For weak/strong symbol resolution during linking
5. **IPC uses fork()/execvp()** - NEVER use system() for security (command injection)
6. **Java validates all input** - SecurityValidator class provides defense-in-depth

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

## Android IPC Security

### Native to Java Communication

Axis2/C services on Android often need to communicate with Java application components. This is done via Android Intent broadcasts using the `am` (Activity Manager) command.

### CRITICAL: Never Use system()

**INSECURE (vulnerable to command injection):**

```c
/* NEVER DO THIS - user input is parsed by shell */
char cmd[1024];
snprintf(cmd, sizeof(cmd),
    "am broadcast --es pattern '%s'", user_pattern);
system(cmd);  /* Shell injection if pattern contains: '; rm -rf / ; ' */
```

**SECURE (use fork/execvp):**

```c
/* Arguments passed directly to process, no shell parsing */
typedef struct {
    const char* type;   /* "--es" for string, "--ei" for int */
    const char* key;
    const char* value;
} intent_extra_t;

static int send_intent_broadcast_secure(
    const char* component,
    const char* action,
    const intent_extra_t* extras,
    int num_extras)
{
    /* Build argument array */
    int total_args = 8 + (num_extras * 3) + 1;  /* base args + extras + NULL */
    char** argv = (char**)malloc(total_args * sizeof(char*));

    int i = 0;
    argv[i++] = "am";
    argv[i++] = "broadcast";
    argv[i++] = "--user";
    argv[i++] = "0";
    argv[i++] = "-n";
    argv[i++] = (char*)component;
    argv[i++] = "-a";
    argv[i++] = (char*)action;

    for (int j = 0; j < num_extras; j++) {
        argv[i++] = (char*)extras[j].type;
        argv[i++] = (char*)extras[j].key;
        argv[i++] = (char*)extras[j].value;  /* User data - safe, not parsed */
    }
    argv[i] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        /* Child process */
        execvp("/system/bin/am", argv);
        _exit(127);  /* exec failed */
    }

    /* Parent waits for child */
    int status;
    waitpid(pid, &status, 0);
    free(argv);

    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : -1;
}
```

### Why fork/execvp Is Secure

| Approach | Shell Invoked | User Data Parsed | Injection Risk |
|----------|---------------|------------------|----------------|
| `system("cmd " + input)` | Yes | Yes | **HIGH** |
| `popen("cmd " + input, "r")` | Yes | Yes | **HIGH** |
| `fork()` + `execvp(argv[])` | No | No | **None** |

With `execvp()`, arguments are passed as an array directly to the new process. Shell metacharacters (`'`, `;`, `|`, `$()`, etc.) are treated as literal characters, not interpreted.

### Usage Example

```c
/* Safe: user-provided pattern passed directly to Java */
int camera_device_delete_files_impl(const char* pattern, char* op_id)
{
    intent_extra_t extras[] = {
        {"--es", "action", "delete_files"},
        {"--es", "pattern", pattern},        /* User input - safe */
        {"--es", "operation_id", op_id}
    };

    return send_intent_broadcast_secure(
        "org.kanaha.camera/org.kanaha.camera.CameraControlReceiver",
        "org.kanaha.CAMERA_CONTROL",
        extras, 3);
}
```

### Java-Side Validation

The Java `BroadcastReceiver` provides defense-in-depth validation:

```java
// In CameraControlReceiver.java
class SecurityValidator {
    // Length limits
    static final int MAX_FILENAME_LENGTH = 255;
    static final int MAX_PATH_LENGTH = 1024;

    // Blocked patterns
    private static final String[] PATH_TRAVERSAL = {"..", "%2e%2e", "%00"};
    private static final String[] INJECTION = {"<script", "${", "`", "$("};

    static String validateFilenameOrPattern(String input, String paramName) {
        if (input == null || input.isEmpty()) return "empty " + paramName;
        if (input.length() > MAX_FILENAME_LENGTH) return paramName + " too long";

        // Check for path traversal
        String lower = input.toLowerCase();
        for (String pattern : PATH_TRAVERSAL) {
            if (lower.contains(pattern)) return "path traversal in " + paramName;
        }

        // Check for injection attempts
        for (String pattern : INJECTION) {
            if (lower.contains(pattern)) return "injection attempt in " + paramName;
        }

        // Whitelist allowed characters
        if (!input.matches("^[a-zA-Z0-9_\\-\\.\\*]+$")) {
            return "invalid characters in " + paramName;
        }

        return null;  // Valid
    }
}
```

### Security Headers Reference

See the Kanaha project's `docs/SECURITY.md` for complete security documentation including:
- mTLS configuration
- Certificate revocation (CRL)
- Audit logging
- Apache httpd security headers

## Summary Table

| Aspect | Desktop/Server | Android |
|--------|----------------|---------|
| C standard | `-std=gnu99` | `-std=gnu99` |
| Library type | Shared (.so) | Static (.a) |
| Service loading | dlopen() | Weak symbol registry |
| Service location | External .so | Application provides strong symbols |
| Linker | Libtool | Direct clang with `--whole-archive` |
| Apache mode | Multi-process | Single-process (`-X`) |
| IPC method | N/A | fork()/execvp() (NOT system()) |
| Input validation | Application | Java SecurityValidator + C checks |

## Files Reference

Key files for Android support:

**Axis2/C Core:**
- `configure.ac` - Android detection, C flags
- `src/core/receivers/axis2_json_rpc_msg_recv.c` - Static service registry
- `src/core/transport/http/server/apache2/mod_axis2.c` - Android init, logging
- `src/core/transport/http/server/apache2/apache2_worker.c` - Request processing

**Application (Kanaha example):**
- `app/src/main/cpp/axis2c/camera_control_service.c` - Service impl with secure IPC
- `app/src/main/java/org/kanaha/camera/CameraControlReceiver.java` - Java receiver with SecurityValidator
- `app/src/main/assets/apache/httpd.conf` - Apache security config
- `app/src/main/assets/apache/ssl.conf` - mTLS and CRL config
- `docs/SECURITY.md` - Comprehensive security documentation

---

*Last updated: 2026-01-06*
