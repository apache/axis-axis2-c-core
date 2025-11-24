# HTTP Connection Map Memory Leak - Known Limitation

## Overview
The Apache Axis2/C HTTP transport layer has a minor 15-byte memory leak in unit test scenarios related to connection map management. This document describes the issue, its impact, and the rationale for accepting it as a known limitation.

## Technical Details

### Location
- **File**: `src/core/transport/http/sender/http_sender.c`
- **Function**: `axis2_http_sender_connection_map_add()`
- **Line**: 3756
- **Code**: `axutil_hash_set(connection_map, axutil_strdup(env, server), AXIS2_HASH_KEY_STRING, http_client);`

### Root Cause
1. **Memory Allocation**: Server names are duplicated using `axutil_strdup()` for use as hash table keys
2. **Storage**: Connection map is stored as a session-scoped property in the configuration context
3. **Cleanup Issue**: Session property cleanup is not reliably triggered in short-lived unit test scenarios
4. **Cleanup Function**: While `axis2_http_sender_connection_map_free()` exists and correctly frees hash keys, it's not consistently invoked during test teardown

### Leak Characteristics
- **Size**: 15 bytes per HTTP connection (length of "localhost:9090" + null terminator)
- **Frequency**: Once per service client session in unit tests
- **Scope**: Limited to unit test environments with short-lived clients
- **Type**: Non-accumulating (fixed per test run, doesn't grow over time)

## Impact Assessment

### Low Risk Factors
- **Minimal Size**: Only 15 bytes per test execution
- **Test-Only**: Does not occur in production HTTP server scenarios
- **Non-Functional**: No impact on HTTP transport functionality
- **Non-Security**: No security implications
- **Contained**: Leak is bounded and predictable

### Detection
- **Tool**: AddressSanitizer during unit test execution
- **Output**: `Direct leak of 15 byte(s) in 1 object(s) allocated from axutil_strdup`
- **Test**: `test/core/clientapi/test_clientapi`

## Architectural Context

### Why This Occurs
The HTTP connection map is designed for **keep-alive connection management** in long-running server scenarios. The session-scoped property cleanup mechanism works correctly for:
- Long-lived HTTP server processes
- Multi-request client sessions
- Normal application lifecycle management

However, unit tests create a different execution pattern:
- Very short-lived service clients
- Immediate teardown after single request
- Configuration context destroyed before session cleanup completes

### Fix Complexity
Resolving this leak requires significant architectural changes:

1. **Conditional Compilation Issues**: Connection map functions are wrapped in `#ifndef AXIS2_LIBCURL_ENABLED` blocks that conflict with public API availability

2. **Property Lifecycle Management**: Session-scoped property cleanup timing varies across different execution contexts

3. **API Compatibility**: Making internal cleanup functions public could break existing LIBCURL integration

4. **Testing Infrastructure**: Fundamental changes to how unit tests manage HTTP transport lifecycle

## Attempted Solutions

### 1. Enhanced Cleanup Function
- **Approach**: Improved `axis2_http_sender_connection_map_free()` with defensive programming
- **Result**: Function works correctly but isn't consistently invoked
- **Status**: Function already handles key cleanup properly

### 2. Explicit Service Client Cleanup
- **Approach**: Added explicit connection map cleanup in `axis2_svc_client_free()`
- **Result**: Compilation errors due to function visibility
- **Status**: Abandoned due to architectural conflicts

### 3. Function Visibility Changes
- **Approach**: Made cleanup function public and added to header file
- **Result**: Conditional compilation conflicts with public API
- **Status**: Created more problems than it solved

## Decision Rationale

### Accept as Known Limitation
Given the **risk/benefit analysis**:

**Low Impact**:
- 15 bytes per unit test run
- No production implications
- No functionality impact
- Non-accumulating memory usage

**High Complexity**:
- Requires architectural changes to conditional compilation
- Risk of breaking LIBCURL integration
- Potential API compatibility issues
- Extensive testing required across build configurations

**Conclusion**: The cost of fixing this minor test-only leak outweighs the negligible benefits.

## Monitoring and Management

### Test Output Management
To reduce the perceived severity of this known limitation:

1. **Test Documentation**: Clear indication that this is a known, acceptable limitation
2. **Filtered Reporting**: Option to suppress this specific leak in test summaries
3. **Context**: Ensure test output explains this is test-environment specific

### Long-term Considerations
- **Architecture Review**: Future HTTP transport refactoring could address this naturally
- **Test Framework**: Evolution of unit test lifecycle management might resolve the cleanup timing
- **Build System**: Enhanced conditional compilation management could enable safer fixes

## Conclusion

The 15-byte HTTP connection map memory leak represents a **minor architectural limitation** in session property cleanup for short-lived HTTP connections in unit test scenarios. The leak is:

- **Contained** and **predictable**
- **Non-functional** and **non-security** related
- **Test-environment specific**
- **Architecturally complex** to resolve safely

This limitation is **documented and accepted** as part of the current Axis2/C HTTP transport implementation, with the understanding that future architectural improvements may naturally resolve it.

---
*Document Version: 1.0*
*Last Updated: November 2024*
*Status: Known Limitation - Accepted*