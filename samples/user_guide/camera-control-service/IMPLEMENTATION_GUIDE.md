# Camera Control Service Implementation Guide

## Overview

This guide provides detailed examples for implementing camera-specific functionality in the Apache Axis2/C Camera Control Service. The service provides stub implementations that you must replace with code specific to your camera system.

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Stub Functions to Implement](#stub-functions-to-implement)
3. [Build System Modifications](#build-system-modifications)
4. [Testing and Validation](#testing-and-validation)
5. [Performance Optimization](#performance-optimization)
6. [License Considerations](#license-considerations)

## Architecture Overview

The Camera Control Service uses a **stub-based architecture** to maintain legal compliance while providing maximum flexibility:

```
HTTP/2 JSON Request → Service Framework (Apache 2.0) → Stub Functions → Your Implementation
```

### Key Principles

- **Service framework** remains generic and Apache 2.0 licensed
- **Stub functions** provide clear integration points for user implementation
- **Camera-specific code** as an end user i.e. not an Apache Axis committer nor contributing code, can use any license compatible with your requirements
- **Zero contamination** of Apache codebase with restrictive licenses, as an end user

## Stub Functions to Implement

The following functions in `src/camera_control_service.c` contain stub implementations that you must replace:

### 1. `camera_device_start_recording_impl()`

**Purpose**: Start camera recording with specified parameters

**Parameters**:
- `env`: Apache Axis2/C environment
- `params`: Recording parameters (clip_name, quality, duration, format)

**Returns**: `AXIS2_SUCCESS` or `AXIS2_FAILURE`

### 2. `camera_device_stop_recording_impl()`

**Purpose**: Stop active camera recording

**Parameters**:
- `env`: Apache Axis2/C environment

**Returns**: `AXIS2_SUCCESS` or `AXIS2_FAILURE`

### 3. `camera_device_get_status_impl()`

**Purpose**: Query current camera status

**Parameters**:
- `env`: Apache Axis2/C environment

**Returns**: `camera_status_t*` structure or NULL on failure

### 4. `camera_device_configure_settings_impl()`

**Purpose**: Configure camera settings

**Parameters**:
- `env`: Apache Axis2/C environment
- `settings`: Camera settings structure

**Returns**: `AXIS2_SUCCESS` or `AXIS2_FAILURE`

## Build System Modifications

### Adding Dependencies

When you implement camera-specific functionality, you'll need to modify the build system:

#### 1. Update Makefile.am

```makefile
# Add your camera-specific defines
libcamera_control_service_la_CPPFLAGS += \
    -DYOUR_CAMERA_INTEGRATION \
    -I/path/to/camera/headers

# Add camera-specific libraries
libcamera_control_service_la_LIBADD += \
    -lyour_camera_lib \
    -ladditional_deps
```

#### 2. Update build_camera_service.sh

```bash
# Add camera-specific compile flags
gcc -shared -fPIC \
    -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -DYOUR_CAMERA_INTEGRATION \
    -I/path/to/camera/headers \
    # ... existing flags ...
    src/camera_control_service.c \
    -o libcamera_control_service.so \
    # ... existing libs ...
    -lyour_camera_lib
```

#### 3. Update configure.ac (for autotools integration)

```autoconf
# Check for camera-specific libraries
AC_CHECK_LIB([your_camera_lib], [main], [], [
    AC_MSG_ERROR([Camera library not found])
])

# Check for camera headers
AC_CHECK_HEADERS([your_camera.h], [], [
    AC_MSG_ERROR([Camera headers not found])
])
```

## Testing and Validation

### Integration Testing with curl

```bash
#!/bin/bash
# test_camera_integration.sh

BASE_URL="https://localhost:443/services/CameraControlService"

echo "Testing Camera Control Service Integration..."

# Test 1: Start recording
echo "1. Testing start recording..."
curl -k --http2 -H "Content-Type: application/json" \
     -d '{"action":"start_recording","clip_name":"integration_test","quality":"1080p","duration":10}' \
     "$BASE_URL/startRecording"

echo ""
sleep 2

# Test 2: Get status
echo "2. Testing get status..."
curl -k --http2 -H "Content-Type: application/json" \
     -d '{"action":"get_status"}' \
     "$BASE_URL/getStatus"

echo ""
sleep 8

# Test 3: Stop recording
echo "3. Testing stop recording..."
curl -k --http2 -H "Content-Type: application/json" \
     -d '{"action":"stop_recording"}' \
     "$BASE_URL/stopRecording"

echo ""

# Test 4: Configure settings
echo "4. Testing configure settings..."
curl -k --http2 -H "Content-Type: application/json" \
     -d '{"action":"configure_settings","resolution":"1920x1080","fps":"30"}' \
     "$BASE_URL/configureSettings"

echo ""
echo "Integration tests complete!"
```

## Performance Optimization

### Minimizing Latency

1. **Avoid system() calls** - Use direct API calls instead of spawning processes
2. **Cache connections** - Reuse network connections for IP cameras
3. **Minimize memory allocation** - Pre-allocate buffers where possible
4. **Use efficient data structures** - Avoid unnecessary string operations

### Example Optimization

```c
// Instead of:
system("ffmpeg -i /dev/video0 output.mp4");

// Use direct API:
// Open device, configure parameters, start capture pipeline directly
// This can reduce latency from ~100ms to <1ms
```

### Memory Management

```c
// Pre-allocate response buffers
static char response_buffer[4096];

// Reuse JSON objects where possible
static json_object *status_template = NULL;

// Initialize once during service startup
if (!status_template) {
    status_template = json_object_new_object();
    // Set up template...
}
```

## License Considerations

### Pure Apache 2.0 Implementation

If you want to keep your implementation Apache 2.0 compatible:

- ✅ Use system APIs (V4L2, standard libraries)
- ✅ Use permissive libraries (libcurl, standard C library)
- ❌ Avoid GPL libraries
- ❌ Avoid proprietary SDKs with restrictive licenses

## Conclusion

This implementation guide provides comprehensive examples for integrating the Apache Axis2/C Camera Control Service with various camera systems. The key principles are:

1. **Maintain legal compliance** - Keep Apache code clean, implement camera-specific code appropriately
2. **Follow established patterns** - Use the same approaches as other Axis2/C services
3. **Optimize for performance** - Minimize latency and memory usage
4. **Test thoroughly** - Validate both HTTP/2 transport and camera integration

By following these examples and principles, you can create a robust, high-performance camera control system that leverages Apache Axis2/C's revolutionary HTTP/2 capabilities while maintaining appropriate license compliance for your specific use case.

---

**Next Steps:**
1. Choose your target camera system
2. Copy the relevant example implementation
3. Adapt the code for your specific hardware/API
4. Test with the provided curl commands
5. Optimize for your performance requirements

For questions or additional examples, refer to the Apache Axis2/C documentation or community resources.
