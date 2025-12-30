# HTTP/2 JSON Threading Issues: Debugging Guide

## Overview

This document captures critical lessons learned from debugging intermittent crashes in the Axis2/C HTTP/2 JSON service. These crashes manifested as:
- `Fatal glibc error: pthread_mutex_lock.c:450 assertion failed: e != ESRCH || !robust`
- Segmentation faults occurring after several successful requests (typically 3-7 requests)

**Key Insight**: These crashes were NOT in the application logic but in **thread synchronization infrastructure** - specifically race conditions during mutex initialization.

## Request Processing Flow with Threading Context

Building on the existing "Request Processing Flow" documentation, this section adds the **threading dimension** critical for understanding intermittent crashes.

### Complete Code Execution Path (with checkpoint locations)

```
HTTP/2 Request Arrives
        |
        v
+---------------------------------------------------------------------+
| Phase 1: Apache HTTP/2 Reception                                     |
| File: apache2_worker.c                                               |
| Function: axis2_apache2_worker_process_request()                     |
| CHECKPOINT 1: Content-type check for JSON processing                 |
+---------------------------------------------------------------------+
        |
        v
+---------------------------------------------------------------------+
| Phase 2: JSON Processor Interface Processing                         |
| File: axis2_apache2_request_processor_json_impl.c                    |
| Function: process_request_body()                                     |
| - Reads JSON from HTTP/2 stream                                      |
| - Stores in JSON_REQUEST_BODY property                               |
+---------------------------------------------------------------------+
        |
        v
+---------------------------------------------------------------------+
| Phase 3: HTTP Transport Utils Processing                             |
| File: http_transport_utils.c                                         |
| Function: axis2_http_transport_utils_process_http_post_request()     |
| CHECKPOINT A: After JSON/XML processing, shows do_rest & soap_envelope|
| CHECKPOINT A2: About to set soap envelope on msg_ctx                 |
| CHECKPOINT A3: Creating engine <- CRASH ZONE BEGINS HERE             |
+---------------------------------------------------------------------+
        |
        v
+---------------------------------------------------------------------+
| Phase 4: Engine Creation <- CRITICAL THREADING ZONE                  |
| File: engine.c                                                       |
| Function: axis2_engine_create()                                      |
|                                                                      |
| [ENGINE_CREATE] Step 1 - Allocating engine                           |
| [ENGINE_CREATE] Step 2 - Engine allocated: 0x...                     |
| [ENGINE_CREATE] Step 3 - About to create service provider            |
|         |                                                            |
|         v                                                            |
| +-------------------------------------------------------------------+|
| | axis2_engine_service_provider_create(env)                         ||
| | -> Creates new service provider instance                          ||
| +-------------------------------------------------------------------+|
|         |                                                            |
|         v                                                            |
| [ENGINE_CREATE] Step 4 - Service provider: 0x...                     |
|         |                                                            |
|         v                                                            |
| +-------------------------------------------------------------------+|
| | axis2_http_service_provider_set_impl() <- RACE CONDITION WAS HERE ||
| | File: axis2_http_service_provider.c                               ||
| |                                                                   ||
| | BEFORE FIX (Dynamic initialization - DANGEROUS):                  ||
| |   if (!g_registry_initialized) {        <- Thread A checks        ||
| |       g_provider_mutex = create_mutex() <- Thread B also checks   ||
| |       g_registry_initialized = TRUE     <- Race condition!        ||
| |   }                                                               ||
| |   mutex_lock(g_provider_mutex)          <- Corrupted mutex!       ||
| |                                                                   ||
| | AFTER FIX (Static initialization - SAFE):                         ||
| |   static pthread_mutex_t g_provider_mutex = PTHREAD_MUTEX_INIT;   ||
| |   pthread_mutex_lock(&g_provider_mutex) <- Always valid           ||
| +-------------------------------------------------------------------+|
|         |                                                            |
|         v                                                            |
| [ENGINE_CREATE] Step 5 - Service provider impl set                   |
| [ENGINE_CREATE] Step 6 - Returning engine                            |
+---------------------------------------------------------------------+
        |
        v
+---------------------------------------------------------------------+
| Phase 5: Engine Receive & Service Execution                          |
| CHECKPOINT A4: Engine created: 0x...                                 |
| CHECKPOINT A5: Got soap_body: 0x...                                  |
| CHECKPOINT 6: Checking HTTP2_JSON_ERROR_HANDLING property            |
|                                                                      |
| -> axis2_engine_receive() -> JsonRpcMessageReceiver -> Service       |
+---------------------------------------------------------------------+
        |
        v
+---------------------------------------------------------------------+
| Phase 6: Response Delivery                                           |
| [JSON RPC MSG RECV] SUCCESSFUL COMPLETION                            |
| [HTTP_TRANSPORT_UTILS] Engine receive completed - status: 1          |
| [APACHE2_WORKER_SKIP] Skipping body - response already sent          |
+---------------------------------------------------------------------+
```

## The Race Condition Explained

### Why Crashes Were Intermittent

The crash only occurred when:
1. **Multiple Apache worker threads** processed requests simultaneously
2. **Both threads** reached `axis2_http_service_provider_set_impl()` before either completed initialization
3. **Thread timing** had to be "just right" for corruption to occur

This explains why:
- First few requests succeeded (single thread, no race)
- Crashes occurred after 3-7 requests (enough concurrency to trigger race)
- Crashes were non-deterministic (timing-dependent)

### The Dangerous Pattern (BEFORE FIX)

```c
/* axis2_http_service_provider.c - DANGEROUS CODE */
static axis2_http_service_provider_t* g_service_provider = NULL;
static axutil_thread_mutex_t* g_provider_mutex = NULL;  /* Dynamically initialized */
static axis2_bool_t g_registry_initialized = AXIS2_FALSE;

static void axis2_http_service_provider_init_registry(const axutil_env_t* env)
{
    if (!g_registry_initialized) {              /* Thread A: checks, sees FALSE */
                                                /* Thread B: checks, sees FALSE */
        g_provider_mutex = axutil_thread_mutex_create(env->allocator, ...);
                                                /* Thread A: creates mutex at 0x1000 */
                                                /* Thread B: creates mutex at 0x2000, overwrites! */
        g_registry_initialized = AXIS2_TRUE;    /* Thread A: sets TRUE */
                                                /* Thread B: sets TRUE again */
    }
}

void axis2_http_service_provider_set_impl(...)
{
    axis2_http_service_provider_init_registry(env);

    if (g_provider_mutex) {
        axutil_thread_mutex_lock(g_provider_mutex);  /* Thread A: locks 0x2000 */
                                                     /* But Thread A created 0x1000! */
                                                     /* Undefined behavior -> CRASH */
        g_service_provider = provider;
        axutil_thread_mutex_unlock(g_provider_mutex);
    }
}
```

### The Safe Pattern (AFTER FIX)

```c
/* axis2_http_service_provider.c - SAFE CODE */
static axis2_http_service_provider_t* g_service_provider = NULL;
static pthread_mutex_t g_provider_mutex = PTHREAD_MUTEX_INITIALIZER;  /* Static init */

/* No initialization function needed - mutex is valid from program start */

void axis2_http_service_provider_set_impl(...)
{
    pthread_mutex_lock(&g_provider_mutex);    /* Always valid, no race possible */
    g_service_provider = provider;
    pthread_mutex_unlock(&g_provider_mutex);
}
```

## Debugging Strategy: How to Find Threading Issues

### Step 1: Add Checkpoint Logging

When crashes are intermittent, add checkpoints to narrow down the crash location:

```c
AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
    "[COMPONENT] CHECKPOINT X - Description, ptr=%p", (void*)pointer);
```

**Key principle**: The last checkpoint that appears in logs before a crash tells you where to look.

### Step 2: Analyze Log Patterns

Look for these patterns in `/usr/local/apache2/logs/error_log`:

```bash
# Find the crash point
cat error_log | grep -E "(CHECKPOINT|Segmentation|Fatal)" | tail -30

# Expected output showing crash location:
# [CHECKPOINT A3] Creating engine, conf_ctx=0x7a047f800170
# Fatal glibc error: pthread_mutex_lock.c:450...
# ^ Crash is INSIDE engine creation, after A3 checkpoint
```

### Step 3: Identify Threading Patterns

Look for these red flags in code:

| Pattern | Risk | Solution |
|---------|------|----------|
| `if (!initialized) { mutex = create(); initialized = true; }` | **CRITICAL** | Use `PTHREAD_MUTEX_INITIALIZER` |
| `static` variables modified by multiple threads | **HIGH** | Add mutex protection |
| Dynamic mutex creation in frequently-called functions | **HIGH** | Use static initialization |
| `axutil_thread_mutex_create()` without `pthread_once` | **MEDIUM** | Consider static init or `pthread_once` |

### Step 4: Verify Fix Deployment

```bash
# Check library has the fix
strings /usr/local/axis2c/lib/libaxis2_http_util.so.0.0.0 | grep "PTHREAD_MUTEX_INIT"

# Check library timestamp matches build
ls -la /usr/local/axis2c/lib/libaxis2_http_util.so*
ls -la /home/robert/repos/axis-axis2-c-core/src/core/transport/http/util/.libs/libaxis2_http_util.so*

# CRITICAL: Restart Apache after deploying
sudo /usr/local/apache2/bin/apachectl -k restart
```

## Quick Diagnostic Commands

### For Intermittent Crashes

```bash
# 1. Run stress test
seq 1 100 | xargs -I{} curl -sk --http2 \
  -H "Content-Type: application/json" \
  -d '{"n_assets": 100}' \
  https://localhost/services/FinancialBenchmarkService/generateTestData \
  -w "%{http_code}\n" -o /dev/null 2>&1 | sort | uniq -c

# 2. Check for crashes during test
cat /usr/local/apache2/logs/error_log | grep -E "(Segmentation|Fatal|pthread)" | tail -10

# 3. Find last checkpoint before crash
cat /usr/local/apache2/logs/error_log | grep "CHECKPOINT" | tail -20
```

### For Mutex-Related Crashes

```bash
# Look for glibc mutex assertions
cat /usr/local/apache2/logs/error_log | grep -i "pthread_mutex\|ESRCH\|robust"

# Check if robust mutex is causing issues
nm -D /usr/local/axis2c/lib/libaxutil.so.0 | grep mutex

# Expected (safe): pthread_mutex_lock, pthread_mutex_unlock
# Problematic: pthread_mutex_consistent, pthread_mutexattr_setrobust
```

## Files Modified in This Fix

| File | Change | Purpose |
|------|--------|---------|
| `src/core/transport/http/util/axis2_http_service_provider.c` | Replaced dynamic mutex init with `PTHREAD_MUTEX_INITIALIZER` | Eliminate race condition |
| `util/src/platforms/unix/thread_unix.c` | Removed robust mutex attributes | Prevent glibc assertions |
| `src/core/transport/http/util/http_transport_utils.c` | Added checkpoint logging | Debug visibility |
| `src/core/engine/engine.c` | Added checkpoint logging | Debug visibility |

## Key Lessons for Future Debugging

1. **Intermittent crashes in multi-threaded code are almost always race conditions**
2. **Dynamic initialization of synchronization primitives is dangerous** - prefer static initialization
3. **Checkpoint logging is essential** - add it liberally during debugging, can remove later
4. **Always verify library deployment** - Apache caches modules, must restart
5. **The crash location in logs points to the symptom, not the cause** - trace back through the code path
6. **100-request stress tests are more reliable than manual testing** for finding race conditions

## Related Documentation

- `HTTP2_RESPONSE.md` - Request Processing Flow overview
- `HTTP2_JSON_REQUEST_CODE_PATH.md` - Detailed code path analysis
- `HTTP2_SERVICE_PROVIDER_INTERFACE_PATTERN.md` - Service provider architecture
- `HTTP2_APACHE2_WORKER.md` - Apache worker threading model

---

*Document based on debugging session December 30, 2025*
*Root cause: Race condition in service provider mutex initialization*
*Resolution: Static mutex initialization with PTHREAD_MUTEX_INITIALIZER*
