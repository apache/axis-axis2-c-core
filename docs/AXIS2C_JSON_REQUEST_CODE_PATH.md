# Axis2/C JSON Request Processing Code Path

## 🔄 Complete JSON Request Flow Documentation

This document explains the complete code path that JSON requests take through the Axis2/C HTTP/2 system, from incoming curl request to service method invocation and response generation.

## 📋 Request Processing Pipeline

### **Phase 1: HTTP/2 Protocol Layer**
```
curl --http2 -H "Content-Type: application/json" -d '{"action":"get_status"}'
  https://localhost/services/BigDataH2Service/getStatus
        ↓
Apache HTTP Server (mod_http2)
        ↓
HTTP/2 Protocol Negotiation
        ↓
mod_axis2.c (Apache Module Handler)
```

**Key Code Locations:**
- **File**: `src/core/transport/http/server/apache2/mod_axis2.c`
- **Function**: `axis2_handler(request_rec *req)`
- **Lines**: 377-400 (HTTP/2 JSON detection)

### **Phase 2: Axis2/C Worker Processing**
```
mod_axis2.c
        ↓
apache2_worker.c (Request Processing Entry Point)
        ↓
axis2_apache2_worker_process_request()
        ↓
HTTP Header Parsing & Content-Type Detection
```

**Key Code Locations:**
- **File**: `src/core/transport/http/server/apache2/apache2_worker.c`
- **Function**: `axis2_apache2_worker_process_request()`
- **Lines**: 469-605 (HTTP header processing)
- **Lines**: 800-900 (Content-Type detection and routing)

### **Phase 3: Transport Utilities**
```
apache2_worker.c
        ↓
http_transport_utils.c (Transport Layer Processing)
        ↓
http_transport_utils_process_http_post_request()
        ↓
JSON Content-Type Detection ("application/json")
```

**Key Code Locations:**
- **File**: `src/core/transport/http/util/http_transport_utils.c`
- **Function**: `http_transport_utils_process_http_post_request()`
- **Lines**: 316 (Content-Type detection)
- **Lines**: 396-466 (HTTP/2 JSON processing)

### **Phase 4: JSON Request Processor (Current Issue Point)**
```
http_transport_utils.c
        ↓
axis2_apache2_request_processor_json_impl.c (JSON-Specific Processing)
        ↓
axis2_apache2_json_processor_parse_and_process_json()
        ↓
JSON Request Parsing & Service Path Extraction
```

**Key Code Locations:**
- **File**: `src/core/transport/http/server/apache2/axis2_apache2_request_processor_json_impl.c`
- **Function**: `axis2_apache2_json_processor_parse_and_process_json()`
- **Lines**: 419-425 (Function signature)
- **Lines**: 800-950 (Service integration - CURRENT ISSUE LOCATION)

### **Phase 5: Service Framework Integration (THE MISSING LINK)**
```
JSON Request Processor
        ↓
THIS IS WHERE THE INTEGRATION SHOULD HAPPEN
        ↓
axis2_json_rpc_msg_recv.c (JSON RPC Message Receiver)
        ↓
Service Discovery & Method Invocation
        ↓
BigDataH2Service Method Execution
```

**Key Code Locations:**
- **File**: `src/core/receivers/axis2_json_rpc_msg_recv.c`
- **Function**: `axis2_json_rpc_msg_recv_invoke_business_logic_sync()`
- **Lines**: 49-230 (Service method invocation)

### **Phase 6: Service Implementation (TARGET)**
```
JSON RPC Message Receiver
        ↓
BigDataH2Service.so (User Service Implementation)
        ↓
bigdata_h2_process_big_data_set_json()
bigdata_h2_get_service_metadata_json()
        ↓
Actual Business Logic Execution
        ↓
JSON Response Generation
```

**Key Code Locations:**
- **File**: `samples/user_guide/bigdata-h2-service/src/bigdata_h2_service_handler.c`
- **Functions**: Service-specific method implementations
- **Expected**: Real business data processing and JSON response generation

## 🚨 Current Problem: Broken Integration

### **Issue Location**
**File**: `axis2_apache2_request_processor_json_impl.c`
**Lines**: 872-932 (Service integration section)

### **Problem Description**
The JSON request processor is **generating its own transport-level responses** instead of:
1. **Routing requests to the service framework**
2. **Letting BigDataH2Service methods execute**
3. **Returning the actual service responses**

### **Current Broken Flow**
```
JSON Request Processor
        ↓
GENERATES OWN RESPONSE ❌
        ↓
{
  "transport_status": "success",
  "message": "HTTP/2 JSON request processed successfully by transport layer",
  "note": "For full service processing, configure services.xml with JsonRpcMessageReceiver"
}
```

### **Correct Expected Flow**
```
JSON Request Processor
        ↓
DELEGATES TO SERVICE FRAMEWORK ✅
        ↓
BigDataH2Service.getStatus() method executes
        ↓
{
  "camera_id": "CAM001",
  "status": "active",
  "temperature": 23.5,
  "recording": true,
  "last_motion": "2025-12-18T10:30:00Z"
}
```

## 🔧 Integration Requirements

### **Step 1: Remove Transport Response Generation**
The transport layer should **never generate business responses**. It should only handle:
- HTTP/2 protocol specifics
- Request parsing and routing
- Service framework delegation
- Response formatting and delivery

### **Step 2: Proper Service Framework Integration**
The JSON request processor needs to:
1. **Parse JSON request body**
2. **Extract service path and operation**
3. **Create Axis2 message context**
4. **Invoke axis2_json_rpc_msg_recv_invoke_business_logic_sync()**
5. **Return whatever the service framework provides**

### **Step 3: Service Configuration**
Ensure BigDataH2Service is properly configured with:
- **services.xml**: JsonRpcMessageReceiver as message receiver
- **ServiceClass parameter**: Points to actual service implementation
- **Operation mappings**: getStatus -> bigdata_h2_get_service_metadata_json()

## 📁 Code Path Summary

### **Files Involved (In Order)**
1. `mod_axis2.c` - Apache module entry point
2. `apache2_worker.c` - Request processing and header parsing
3. `http_transport_utils.c` - Transport layer utilities
4. `axis2_apache2_request_processor_json_impl.c` - JSON-specific processing ⚠️ **INTEGRATION POINT**
5. `axis2_json_rpc_msg_recv.c` - JSON service framework ⚠️ **TARGET**
6. `bigdata_h2_service_handler.c` - Actual service implementation ⚠️ **FINAL DESTINATION**

### **Critical Integration Point**
**Location**: Lines 872-932 in `axis2_apache2_request_processor_json_impl.c`

**Current State**: Transport generates own responses
**Required State**: Transport delegates to service framework
**Goal**: BigDataH2Service methods return actual business data

## 🎯 Success Criteria

When properly integrated:
1. **curl request** reaches BigDataH2Service.getStatus()
2. **Service method executes** with actual business logic
3. **JSON response** contains real service data, not transport messages
4. **HTTP 200** indicates successful service processing
5. **No hardcoded responses** from transport layer

---

**Status**: 🚧 **Integration In Progress** - Transport layer needs to delegate to service framework instead of generating own responses.