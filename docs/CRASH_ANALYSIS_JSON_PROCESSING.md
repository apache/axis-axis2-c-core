# Apache SIGABRT Crash Analysis - HTTP/2 JSON Processing

**Date**: Dec 18, 2025
**Issue**: Apache crashes with SIGABRT when processing HTTP/2 POST requests with JSON payloads

## Crash Pattern Analysis

### ✅ Working Requests (No Crash)
- **HTTP/2 GET**: `404 Not Found` - clean response
- **HTTP/2 POST (no payload)**: `500 Internal Server Error` - processes through JsonRpcMessageReceiver
- **HTTP/2 POST + text/plain**: `500 Invalid JSON format` - handles non-JSON content correctly

### ❌ Crashing Requests (SIGABRT)
- **HTTP/2 POST + Content-Type: application/json + JSON payload**: Apache crashes with SIGABRT

## Evidence from systemd logs
```
apache2-custom.service: Main process exited, code=dumped, status=6/ABRT
apache2-custom.service: Failed with result 'core-dump'
```

## Root Cause Analysis

### 1. Crash Location
The crash occurs in **JSON payload processing** in our enhanced JsonRpcMessageReceiver module:
- **File**: `/home/robert/w2/axis-axis2-c-core/src/core/receivers/axis2_json_rpc_msg_recv.c`
- **Function**: `axis2_json_rpc_msg_recv_invoke_business_logic_sync()`
- **Enhanced Code**: Service loading corruption diagnostics added at lines 193-286

### 2. Likely Memory Corruption Issues

**Problem Areas in Enhanced Code:**
```c
// Lines 193-198: Unsafe byte-by-byte analysis
AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
    "[JSON_RPC_SERVICE_LOADING] Byte analysis - first 8 bytes: 0x%02x 0x%02x...",
    (unsigned char)class_name[0], (unsigned char)class_name[1], ...);
```

**Issues:**
1. **No null-pointer check** before accessing `class_name[0]` through `class_name[7]`
2. **No length validation** before byte access
3. **Unsafe string operations** in diagnostic logging

**Problem Code Locations:**
- **Line 188-198**: Byte-by-byte analysis without bounds checking
- **Line 219-227**: Class name validation and fallback logic
- **Line 241-245**: Parameter creation and correction logic

### 3. JSON Stream Reading

**Potential Issue in JSON Reader:**
- **File**: `/home/robert/w2/axis-axis2-c-core/src/core/transport/http/util/axis2_json_reader.c`
- **Function**: `axis2_json_reader_create_for_stream()`
- **Enhanced logging**: May be causing issues during stream reading

## Fix Strategy

### Immediate Fix
1. **Add null-pointer checks** before accessing class_name bytes
2. **Add bounds checking** for all string operations
3. **Validate string length** before diagnostic operations
4. **Safe string handling** in all logging operations

### Code Fix Example
```c
// BEFORE (unsafe):
AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
    "Byte analysis - first 8 bytes: 0x%02x 0x%02x...",
    (unsigned char)class_name[0], (unsigned char)class_name[1], ...);

// AFTER (safe):
if (class_name && strlen(class_name) >= 8) {
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "Byte analysis - first 8 bytes: 0x%02x 0x%02x...",
        (unsigned char)class_name[0], (unsigned char)class_name[1], ...);
} else {
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
        "Class name too short or null for byte analysis");
}
```

## Current Status
- **Enhanced module deployed**: Contains comprehensive diagnostics but has memory corruption
- **Crash consistently reproducible**: HTTP/2 + POST + JSON = SIGABRT
- **Pipeline working for non-JSON**: Proves transport layer and basic processing work

## Next Steps
1. Fix memory corruption in JsonRpcMessageReceiver diagnostic code
2. Rebuild and redeploy enhanced module
3. Test with JSON payloads to verify corruption fix
4. Complete service loading corruption investigation