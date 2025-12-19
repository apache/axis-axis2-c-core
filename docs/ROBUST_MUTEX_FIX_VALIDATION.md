# Robust Mutex Fix Implementation and Validation Plan

**Date:** December 19, 2025
**Issue:** pthread_mutex_lock.c:450 assertion failures causing Apache child crashes
**Fix:** Implemented PTHREAD_MUTEX_ROBUST in axutil_thread_mutex_create()

## Root Cause Analysis

The pthread mutex corruption was caused by:
1. **Apache HTTP/2 worker threads terminating** while holding non-robust mutexes
2. **Subsequent cleanup code** attempting to access mutexes owned by terminated threads
3. **glibc detecting ESRCH (No such process)** and failing assertion: `e != ESRCH || !robust`

## Fix Implementation

Modified `/home/robert/w2/axis-axis2-c-core/util/src/platforms/unix/thread_unix.c`:

### Before (Non-Robust):
```c
AXIS2_EXTERN axutil_thread_mutex_t *AXIS2_CALL
axutil_thread_mutex_create(axutil_allocator_t * allocator, unsigned int flags)
{
    axutil_thread_mutex_t *new_mutex = NULL;
    new_mutex = AXIS2_MALLOC(allocator, sizeof(axutil_thread_mutex_t));
    new_mutex->allocator = allocator;

    if(pthread_mutex_init(&(new_mutex->mutex), NULL) != 0)
    {
        AXIS2_FREE(allocator, new_mutex);
        return NULL;
    }
    return new_mutex;
}
```

### After (Robust Mutex):
```c
AXIS2_EXTERN axutil_thread_mutex_t *AXIS2_CALL
axutil_thread_mutex_create(axutil_allocator_t * allocator, unsigned int flags)
{
    axutil_thread_mutex_t *new_mutex = NULL;
    pthread_mutexattr_t attr;
    int result;

    new_mutex = AXIS2_MALLOC(allocator, sizeof(axutil_thread_mutex_t));
    new_mutex->allocator = allocator;

    /* Initialize mutex attributes for robust mutex support */
    result = pthread_mutexattr_init(&attr);
    if(result != 0) {
        AXIS2_FREE(allocator, new_mutex);
        return NULL;
    }

    /* CRITICAL FIX: Set mutex to be robust - survives thread termination */
    /* This prevents pthread_mutex_lock.c:450 ESRCH assertion failures */
    result = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
    if(result != 0) {
        pthread_mutexattr_destroy(&attr);
        AXIS2_FREE(allocator, new_mutex);
        return NULL;
    }

    /* Initialize mutex with robust attributes */
    result = pthread_mutex_init(&(new_mutex->mutex), &attr);

    /* Clean up attributes */
    pthread_mutexattr_destroy(&attr);

    if(result != 0) {
        AXIS2_FREE(allocator, new_mutex);
        return NULL;
    }
    return new_mutex;
}
```

## Build Status

✅ **Fixed code successfully built:**
- Util library with robust mutex: `make -C util/src` ✅
- Core engine: `make -C src/core/engine` ✅
- Apache module: `make -C src/core/transport/http/server/apache2` ✅

## Deployment Requirements

**CRITICAL:** The following steps require sudo privileges to deploy and test:

```bash
# Step 1: Deploy updated Apache module
sudo cp /home/robert/w2/axis-axis2-c-core/src/core/transport/http/server/apache2/.libs/libmod_axis2.so /usr/local/apache2/modules/mod_axis2.so

# Step 2: Restart Apache to load new module
sudo systemctl stop apache2-custom
sudo pkill -f apache2 2>/dev/null || true
sudo systemctl start apache2-custom

# Step 3: Verify deployment
ls -la /usr/local/apache2/modules/mod_axis2.so  # Should show current timestamp
```

## Validation Test Plan

### Phase 1: Single Request Validation
```bash
curl -sk --http2 -H "Content-Type: application/json" -d '{"test":"robust_mutex_validation"}' https://localhost/services/BigDataH2Service/processBigDataSet --max-time 5
```
**Expected:** No pthread mutex errors, normal 202 response

### Phase 2: Rapid-Fire Stress Test (Previously 50% failure rate)
```bash
# Execute 20 rapid requests to trigger race condition
for i in {1..20}; do
    echo "Request $i:"
    curl -sk --http2 -H "Content-Type: application/json" -d "{\"test\":\"stress_test_$i\"}" https://localhost/services/BigDataH2Service/processBigDataSet --max-time 3
    echo "Exit code: $?"
done
```
**Expected:** 100% success rate (previously failed ~50% of requests)

### Phase 3: Log Analysis
```bash
# Check for pthread mutex errors after stress test
sudo tail -50 /usr/local/apache2/logs/error_log | grep -i pthread
sudo tail -50 /usr/local/apache2/logs/error_log | grep -i "assertion"
sudo tail -50 /usr/local/apache2/logs/error_log | grep -i "ESRCH"
```
**Expected:** Zero pthread mutex errors

## Success Criteria

1. **Zero pthread_mutex_lock.c:450 assertion failures**
2. **100% success rate with rapid-fire testing** (vs previous ~50%)
3. **Normal HTTP/2 connection cleanup** without SIGABRT crashes
4. **JsonRpcMessageReceiver completion** followed by clean thread termination

## Technical Benefits of PTHREAD_MUTEX_ROBUST

1. **Orphaned mutex recovery:** When thread dies holding mutex, new threads can detect and recover
2. **ESRCH error handling:** glibc no longer triggers assertion failures on dead thread mutex access
3. **Graceful degradation:** System continues operating instead of crashing Apache children
4. **Production stability:** Eliminates intermittent behavior unacceptable in Apache open source projects

## Current Status

- ✅ Root cause identified and documented
- ✅ Fix implemented with PTHREAD_MUTEX_ROBUST
- ✅ System rebuilt with robust mutex support
- ⏳ **PENDING:** Module deployment (requires sudo privileges)
- ⏳ **PENDING:** Validation testing with stress tests
- ⏳ **PENDING:** Confirmation of 100% success rate

## Next Steps

1. Deploy updated module with sudo privileges
2. Execute validation test plan
3. Confirm pthread mutex errors eliminated
4. Document final results

This fix addresses the user requirement: **"This is an Apache open source project. Intermittent issues are unacceptable."**