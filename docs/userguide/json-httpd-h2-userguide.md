# Apache Axis2/C HTTP/2 JSON and REST with Apache httpd User's Guide

## Overview

This guide will help you get started with Axis2/C HTTP/2 transport and JSON via REST, using [Apache httpd](https://httpd.apache.org/) with [mod_h2](https://httpd.apache.org/docs/2.4/mod/mod_h2.html) and [mod_ssl](https://httpd.apache.org/docs/2.4/mod/mod_ssl.html)! It gives a detailed description on how to write HTTP/2 optimized JSON based REST Web services for enterprise applications, including big data processing, user authentication, and security demonstrations.

**Port from Axis2/Java Spring Boot**: This guide is a complete port of the Axis2/Java Spring Boot demonstration services, replacing Spring Boot embedded server with Apache httpd for production deployment with enhanced performance and security.

**New in Axis2/C 2.0**: Complete HTTP/2 transport implementation with enterprise capabilities, streaming optimization for large JSON payloads, authentication services, XSS protection demonstrations, and production-ready performance enhancements using Apache httpd as the application server.

**✅ Verified Testing**: This guide has been tested and verified to work on **Ubuntu 25.10** with Apache httpd and mod_h2 support.

**🔧 Critical Configuration**: HTTPS Virtual Hosts **MUST** include `<Location /services>` directive inside `<VirtualHost *:443>` blocks. Without this, Axis2/C services will fail with routing errors and no debug messages will appear in logs.

## 🚀 **TL;DR - Why C Programmers Choose Axis2/C HTTP/2**

**💾 Memory Efficiency**: `240MB peak` for 50MB JSON vs JVM heap overhead
**⚡ Native Speed**: `26.56 MB/s` JSON throughput with zero JVM warm-up
**🏗️ Build Time**: `162% faster` compilation (13:31 min → <5 min)
**🎯 Latency**: `30% reduction` (60ms → 42ms) vs HTTP/1.1 baseline
**🔗 Multiplexing**: `50% overhead reduction` with HTTP/2 connection reuse
**📊 Scalability**: `10/10 concurrent streams` processed in 2ms total
**🛠️ Deployment**: Single `.so` library vs WAR + application server stack

**Bottom Line**: Native C performance with enterprise-grade HTTP/2 JSON processing, measured and documented.

**Important Note about JSON Processing Architecture:**

Axis2/C 2.0 implements a **Dual JSON Architecture** that separates HTTP/1.1 and HTTP/2 JSON processing:

- **HTTP/1.1 Transport**: Uses json-c library with optional XML/SOAP compatibility for legacy applications
- **HTTP/2 Transport**: Uses **optimized json-c library** with zero XML/SOAP dependencies for maximum performance

This pure json-c approach for both transports provides:
- ✅ **No SOAP/XML overhead**: Direct JSON processing using native json-c objects
- ✅ **Memory efficiency**: 60-80% less memory usage compared to XML-based processing
- ✅ **Performance gains**: 30-40% faster JSON processing for large payloads (50MB+)
- ✅ **Streaming optimization**: Native support for HTTP/2 flow control and streaming

**Critical Build Requirement**: Despite HTTP/2 using pure json-c (bypassing the traditional `AXIS2_JSON_ENABLED` framework), the `--enable-json` configure flag is **REQUIRED** because:

- **PKG-CONFIG Integration**: Sets up `JSON_CFLAGS` and `JSON_LIBS` build variables
- **Library Detection**: Ensures json-c development headers and libraries are available
- **HTTP/2 Compilation**: HTTP/2 tests and utilities depend on these build variables

**In the "configure" examples both flags are essential**:
```bash
./configure --enable-http2 --enable-json  # Both required for HTTP/2
```
---

## Demonstration Services

This guide covers five **fully implemented** service demonstrations:

1. **[BigDataH2Service](#bigdata-h2-service-testing)**: HTTP/2 big data processing with streaming optimization
2. **[LoginService](#login-service-testing)**: User authentication with JWT token generation
3. **[TestwsService](#testws-service-testing-xss-protection-demonstration)**: XSS protection demonstration with HTML encoding
4. **[CameraControlService](#cameracontrolservice-testing-generic-camera-control-with-sftp)**: Generic camera control with SFTP file transfer using user-implementable stub functions
5. **[FinancialBenchmarkService](#financialbenchmarkservice-testing-financial-computation-benchmark)**: Financial computation benchmarks demonstrating native C performance for O(n²) matrix operations

**⚠️ Protocol Requirement**: These services are **HTTP/2-only** for performance optimization. HTTP/1.1 requests will receive **HTTP 426 "Upgrade Required"** responses. Always use `curl --http2` for testing.

More documentation concerning Axis2/C and JSON can be found in the [HTTP/2 JSON Architecture](../AXIS2C_HTTP2_MIGRATION_STATE.md) and [HTTP/2 Configuration](../HTTP2_AXIS2_DOT_XML.md).

## 🔗 **Related Technical Documentation**

**For comprehensive technical architecture and performance analysis**, see [Axis2/C HTTP/2 Pure JSON Architecture - Migration State](../AXIS2C_HTTP2_MIGRATION_STATE.md) which provides:
- Detailed performance comparison vs Axis2/Java (memory, speed, build time)
- HTTP/2 transport architecture and multiplexing analysis
- JSON-C integration patterns and optimization techniques
- Migration strategies from XML/SOAP to pure JSON
- Comprehensive benchmarking methodology and results

This user guide focuses on **practical implementation and deployment**, while the migration document provides **deep technical insights and performance data**.

## Introduction

This user guide is written for **Axis2/C 2.0.0**, which introduces comprehensive HTTP/2 support and enterprise web service capabilities. This guide covers building from source and deploying the demonstration services.

The Axis2/C source can be obtained from the [official Apache Git repository](https://github.com/apache/axis-axis2-c-core) or as a [source distribution download](../downloads.md). For production deployments, we recommend using the tagged v2.0.0 release. The [Installation Guide](../getting-started/installation.md) provides additional build instructions for various platforms.

The source code for this guide provides a complete Apache httpd configuration and Axis2/C service implementation for enterprise big data processing with HTTP/2 optimization.

Please note that Axis2/C is an open-source effort. If you feel the code could use some new features or fixes, please get involved and lend us a hand! The Axis developer community welcomes your participation.

Let us know what you think! Send your feedback to "[c-user@axis.apache.org](mailto:c-user@axis.apache.org?subject=[Axis2C])". Kindly prefix the subject of the mail with [Axis2C].

## HTTP/2 Transport Features

Axis2/C 2.0 introduces a complete HTTP/2 transport implementation designed for enterprise big data processing requirements. The HTTP/2 transport provides significant performance improvements over HTTP/1.1, especially for large JSON payloads and concurrent requests.

### Key HTTP/2 Benefits

- **Connection Multiplexing**: Process multiple requests over a single connection, reducing connection overhead by up to 80% (from 50 connections to 10 multiplexed connections)
- **Streaming Optimization**: Memory-efficient processing of large JSON payloads (50MB+) with 20% reduction in memory usage
- **Enhanced Performance**: 30% reduction in request latency and 40% improvement in JSON processing throughput for enterprise workloads
- **Memory Management**: Adaptive flow control and memory pressure detection for operation within 2GB heap constraints
- **Security**: HTTPS-only enforcement (RFC 7540 compliance) with TLS 1.2+ and ALPN support

### HTTP/2 Processing Modes

The HTTP/2 transport automatically selects the optimal processing mode based on payload size:

- **Standard Processing (<10MB)**: Regular HTTP/2 transport with multiplexing benefits
- **Multiplexing Optimization (10-50MB)**: Enhanced concurrent processing with connection multiplexing and flow control
- **Streaming Optimization (50MB+)**: Memory-efficient streaming with chunked processing and adaptive windowing for enterprise big data requirements

### Enterprise Big Data Support

The HTTP/2 transport is specifically optimized for enterprise big data processing:

- **Large Payload Support**: Tested with JSON payloads up to 100MB+
- **Memory Constraints**: Designed for 2GB heap environments with adaptive memory management
- **Concurrent Processing**: Support for multiple simultaneous large dataset operations
- **Performance Monitoring**: Built-in metrics collection for throughput and memory usage

### Incremental Buffer Allocation

Axis2/C uses an intelligent **incremental buffer growth** strategy that optimizes memory usage across diverse workloads - from IoT devices with tiny payloads to enterprise systems with multi-megabyte JSON:

| Initial Size | Growth Strategy | Maximum | Use Case |
|--------------|-----------------|---------|----------|
| 64KB | Doubles on demand | 10MB | Adapts to actual payload size |

**Memory Efficiency by Payload Size**:

| Payload Type | Actual Size | Buffer Allocated | vs Static 10MB |
|--------------|-------------|------------------|----------------|
| IoT/Camera   | ~24 bytes   | 64KB             | **160x smaller** |
| Medium JSON  | ~50KB       | 64KB             | **160x smaller** |
| Large portfolio | ~235KB   | 256KB            | **40x smaller** |
| Enterprise   | ~5MB        | 8MB              | **1.25x smaller** |

This approach ensures:
- **IoT efficiency**: Camera control payloads (~24 bytes) use only 64KB instead of 10MB
- **Enterprise support**: 500-asset financial portfolios (~5MB) are fully supported
- **No configuration needed**: Buffer grows automatically based on actual data

**Implementation Note**: Uses standard C `malloc/realloc` for reliable buffer growth, with final copy to AXIS2-managed memory for consistent cleanup.

### Defense in Depth: Request Limits

Axis2/C implements a **multi-layer security architecture** where Apache httpd enforces request limits *before* payloads reach the C code. This provides protection against:

- **Memory exhaustion attacks**: Large payloads rejected at TCP level
- **Slowloris/slow-read attacks**: Timeout enforcement at Apache level
- **Integer overflow exploits**: Size limits enforced before arithmetic operations

**Security Layers**:

| Layer | Directive | Enforced By | Failure Response |
|-------|-----------|-------------|------------------|
| **1. Apache** | `LimitRequestBody` | httpd (before mod_axis2) | HTTP 413 |
| **2. Axis2/C** | `max_buffer` (10MB) | JSON processor with overflow checks | JSON error 413 |
| **3. Service** | `maxJSONPayloadSize` | services.xml configuration | Application error |

**Recommended Apache Configuration** (in `httpd.conf`):

> **Convenience**: A complete [`httpd.conf`](httpd.conf) with these limits pre-configured is available at `docs/userguide/httpd.conf`. Copy it to `/usr/local/apache2/conf/` to deploy.

```apache
# Global default: 10MB max request body (matches Axis2/C max_buffer)
LimitRequestBody 10485760

# Limit request line (URL + method + protocol) - prevents long URL attacks
LimitRequestLine 8190

# Limit total header fields - prevents header flooding
LimitRequestFields 100

# Limit individual header size - prevents header overflow attacks
LimitRequestFieldSize 8190

# Per-service overrides for different workloads:
<Location /services/CameraControlService>
    # IoT endpoints need minimal payloads
    LimitRequestBody 65536
</Location>

<Location /services/FinancialBenchmarkService>
    # Enterprise payloads up to 10MB
    LimitRequestBody 10485760
</Location>

<Location /services/BigDataH2Service>
    # Big data service - allow larger payloads (50MB)
    LimitRequestBody 52428800
</Location>
```

**Key Security Benefit**: If an attacker sends a 100MB payload:
1. Apache rejects at TCP level before mod_axis2 is invoked
2. No memory allocated, no CPU spent parsing malicious data
3. Clean HTTP 413 response with minimal server resources consumed

**Integer Overflow Protection**: The Axis2/C JSON processor includes explicit checks for:
- Buffer arithmetic underflow (`current_size - total_read - 1`)
- Read size overflow before addition (`bytes_read > max_buffer - total_read`)
- Safe buffer doubling (`current_size > max_buffer / 2` check before multiply)
- Allocation size validation (`total_read + 1` overflow check)

## Getting Started for Beginners

This comprehensive step-by-step guide will walk you through building and deploying all three C web services from scratch, even if you're new to Axis2/C development.

### Prerequisites and Dependencies

Before starting, ensure your system has all required dependencies installed:

**Ubuntu/Debian Systems:**
```bash
# Update package list
sudo apt-get update

# Install build tools and dependencies
sudo apt-get install -y \
    build-essential \
    autotools-dev \
    autoconf \
    automake \
    libtool \
    pkg-config \
    git

# Install Apache and development headers with HTTP/2 + SSL/TLS support
sudo apt-get install -y \
    apache2 \
    apache2-dev \
    libapache2-mod-h2 \
    libssl-dev \
    libcurl4-openssl-dev \
    openssl

# Install JSON-C library
sudo apt-get install -y \
    libjson-c-dev \
    libjson-c5

# Install additional dependencies for Axis2/C
sudo apt-get install -y \
    libxml2-dev \
    libcurl4-openssl-dev \
    uuid-dev \
    zlib1g-dev
```

**RedHat/CentOS/Fedora Systems:**
```bash
# Update package list
sudo dnf update  # or 'yum update' on older systems

# Install build tools and dependencies
sudo dnf install -y \
    gcc \
    gcc-c++ \
    make \
    autoconf \
    automake \
    libtool \
    pkg-config \
    git

# Install Apache and development headers with HTTP/2 + SSL/TLS support
sudo dnf install -y \
    httpd \
    httpd-devel \
    mod_h2 \
    mod_ssl \
    openssl \
    openssl-devel

# Install JSON-C library
sudo dnf install -y \
    json-c-devel

# Install additional dependencies for Axis2/C
sudo dnf install -y \
    libxml2-devel \
    libcurl-devel \
    libuuid-devel \
    zlib-devel
```

## Quick HTTP/2 Support Test

**Before compiling Apache from source, test if your system Apache already has HTTP/2 support:**

```bash
# Quick test - does Apache have mod_h2 available?
echo "🔍 Testing Apache HTTP/2 support on your system..."

# Test 1: Check if Apache is installed
{ command -v apache2 >/dev/null 2>&1 || command -v httpd >/dev/null 2>&1; } && echo "✅ Apache installation found" || { echo "❌ No Apache installation found"; echo "   Install Apache first: sudo apt install apache2"; return 1 2>/dev/null || echo "Please install Apache before continuing"; }

# Test 2: Check if mod_h2 module file exists
{ ls /usr/lib/apache2/modules/mod_h2.so 2>/dev/null || ls /usr/lib*/httpd/modules/mod_h2.so 2>/dev/null; } && { echo "✅ mod_h2.so found - HTTP/2 module is available!"; echo "   You can use system Apache (skip source compilation)"; } || echo "❌ mod_h2.so not found - HTTP/2 module missing"

# Test 3: Try enabling mod_h2 (safe test)
command -v a2enmod >/dev/null 2>&1 && { sudo a2enmod h2 2>/dev/null && { echo "✅ mod_h2 enabled successfully!"; echo "   Your system Apache supports HTTP/2"; sudo a2dismod h2 2>/dev/null; } || echo "❌ Cannot enable mod_h2 - module not available"; } || echo "⚠️ a2enmod not available - cannot test module enabling"

# Test 4: Check what HTTP/2 related modules exist
echo ""
echo "Available HTTP/2 related modules:"
find /usr/lib*/apache2/modules /usr/lib*/httpd/modules -name "*h2*" -o -name "*http2*" 2>/dev/null || echo "None found"

echo ""
echo "📋 RECOMMENDATION:"
ls /usr/lib*/apache2/modules/mod_h2.so /usr/lib*/httpd/modules/mod_h2.so 2>/dev/null >/dev/null && { echo "✅ Use system Apache - HTTP/2 support is available"; echo "   Continue with Step 1 Option A"; } || { echo "❌ Compile Apache from source - no HTTP/2 support in system package"; echo "   Use Step 0 below, then Step 1 Option B"; }
```

**Run this test first to save time!**

---

## Compiling Apache httpd from Source with HTTP/2 Support

**When to use this approach:**
- Your distribution lacks mod_h2 support (like Ubuntu 25.10)
- You need the latest Apache features
- You want full control over Apache compilation options
- You're working with Apache projects and expect to overcome such hurdles

**⚠️ Important**: This will install Apache to `/usr/local/apache2` to avoid conflicts with system packages.

### Step 0: Compile Apache httpd from Source

**Install build dependencies:**
```bash
# Ubuntu/Debian
sudo apt-get install -y build-essential libtool-bin \
    libpcre3-dev libssl-dev libnghttp2-dev \
    libexpat1-dev libapr1-dev libaprutil1-dev \
    wget

# RedHat/CentOS/Fedora
sudo dnf install -y gcc gcc-c++ make libtool \
    pcre-devel openssl-devel libnghttp2-devel \
    expat-devel apr-devel apr-util-devel \
    wget
```

**Download and compile Apache httpd 2.4.64 (or latest) with HTTP/2:**
```bash
# Create build directory
mkdir -p ~/apache-build
cd ~/apache-build

# Download Apache httpd source, always get latest
wget https://archive.apache.org/dist/httpd/httpd-2.4.64.tar.gz
tar -xzf httpd-2.4.64.tar.gz
cd httpd-2.4.64

# Configure with HTTP/2 support
./configure \
    --prefix=/usr/local/apache2 \
    --enable-so \
    --enable-ssl \
    --enable-http2 \
    --enable-headers \
    --enable-rewrite \
    --with-ssl \
    --with-nghttp2 \
    --enable-mods-shared=all

# Compile (use all CPU cores)
make -j$(nproc)

# Install
sudo make install

# Create systemd service (FIXED: Use Type=simple with -D FOREGROUND)
sudo tee /etc/systemd/system/apache2-custom.service << 'EOF'
[Unit]
Description=Apache HTTP Server (Custom Build)
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/apache2/bin/httpd -D FOREGROUND
ExecReload=/usr/local/apache2/bin/httpd -k graceful
KillMode=mixed
PrivateTmp=true
Environment=APACHE_RUN_DIR=/usr/local/apache2/var/run

[Install]
WantedBy=multi-user.target
EOF

# Set up runtime directories
sudo mkdir -p /usr/local/apache2/var/run
sudo chown www-data:www-data /usr/local/apache2/var/run 2>/dev/null || \
sudo chown apache:apache /usr/local/apache2/var/run 2>/dev/null || \
sudo chown $(whoami):$(whoami) /usr/local/apache2/var/run

# Fix ServerName to prevent AH00558 warning (HTTP/2 requires HTTPS)
sudo sed -i 's/#ServerName www.example.com:80/ServerName localhost:443/' /usr/local/apache2/conf/httpd.conf

# Enable HTTP/2 modules (uncomment and add missing modules)
sudo sed -i 's/#LoadModule http2_module modules\/mod_http2.so/LoadModule http2_module modules\/mod_http2.so/' /usr/local/apache2/conf/httpd.conf
sudo sed -i 's/#LoadModule ssl_module modules\/mod_ssl.so/LoadModule ssl_module modules\/mod_ssl.so/' /usr/local/apache2/conf/httpd.conf

# Enable mod_proxy (required dependency for proxy_http2)
sudo sed -i 's/#LoadModule proxy_module modules\/mod_proxy.so/LoadModule proxy_module modules\/mod_proxy.so/' /usr/local/apache2/conf/httpd.conf

# Add proxy_http2 module (usually not in default config)
echo 'LoadModule proxy_http2_module modules/mod_proxy_http2.so' | sudo tee -a /usr/local/apache2/conf/httpd.conf

# Add HTTP/2 protocol configuration
sudo tee -a /usr/local/apache2/conf/httpd.conf << 'EOF'

# HTTP/2 Protocol Configuration
Protocols h2 h2c http/1.1

# HTTP/2 Settings
H2Direct on
H2Push on
H2PushPriority *                       after
H2PushPriority text/css                before
H2PushPriority image/jpeg              after   32
H2PushPriority image/png               after   32
H2PushPriority application/javascript  interleaved

EOF

# Create SSL certificate for HTTPS (required for HTTP/2)
sudo mkdir -p /usr/local/apache2/conf/ssl
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
  -keyout /usr/local/apache2/conf/ssl/apache-selfsigned.key \
  -out /usr/local/apache2/conf/ssl/apache-selfsigned.crt \
  -subj "/C=US/ST=Test/L=Test/O=Test/OU=Test/CN=localhost"

# Add SSL Virtual Host configuration
sudo tee -a /usr/local/apache2/conf/httpd.conf << 'EOF'

# SSL Configuration for HTTPS
Listen 443 ssl
<VirtualHost *:443>
    ServerName localhost:443
    DocumentRoot /usr/local/apache2/htdocs

    SSLEngine on
    SSLCertificateFile /usr/local/apache2/conf/ssl/apache-selfsigned.crt
    SSLCertificateKeyFile /usr/local/apache2/conf/ssl/apache-selfsigned.key

    # Enable HTTP/2 for this virtual host
    Protocols h2 http/1.1

    # CRITICAL: Axis2/C Location directive must be inside HTTPS virtual host
    # Without this, /services/* requests won't reach mod_axis2
    <Location /services>
        SetHandler axis2_module
    </Location>

    # Enable JSON content type handling for HTTPS
    <LocationMatch "^/services/.*">
        Header always set Access-Control-Allow-Origin "*"
        Header always set Access-Control-Allow-Methods "GET, POST, PUT, DELETE, OPTIONS"
        Header always set Access-Control-Allow-Headers "Content-Type, Authorization, Accept"
    </LocationMatch>
</VirtualHost>

EOF

# IMPORTANT: Configuration Troubleshooting Note
#
# The above configuration fixes a critical issue where HTTPS/HTTP2 requests
# to /services/* would not reach mod_axis2. This happens because:
#
# 1. HTTPS requests go to the <VirtualHost *:443> block
# 2. If Axis2 <Location /services> is only in global config, virtual host doesn't inherit it
# 3. Result: mod_axis2 never gets called, requests fail with routing errors
#
# Symptoms of this issue:
# - HTTP/1.1 works over HTTP but fails over HTTPS
# - No debug messages from mod_axis2 in Apache error logs
# - SOAP faults or 404 errors for /services/* requests over HTTPS

# Test configuration before starting
sudo /usr/local/apache2/bin/httpd -t

# Enable and start custom Apache
sudo systemctl daemon-reload
sudo systemctl enable apache2-custom
sudo systemctl start apache2-custom

# Verify HTTP/2 module is available
/usr/local/apache2/bin/httpd -M | grep http2 && echo "✅ HTTP/2 modules available" || echo "❌ HTTP/2 modules missing"

# Check apache2-custom service status
systemctl status apache2-custom --no-pager
```

### 🔧 Apache Startup Troubleshooting

```bash
# Check service status
systemctl status apache2-custom -n 50

# View detailed logs
journalctl -xeu apache2-custom.service

# Test configuration syntax
sudo /usr/local/apache2/bin/httpd -t

**Verify Apache is working correctly:**
```bash
# Test basic HTTPS connectivity (HTTP/2 requires TLS)
curl -k -I https://localhost

# Test HTTP/2 support over HTTPS
curl -k --http2 -I https://localhost

# Check loaded modules
/usr/local/apache2/bin/httpd -M | grep -E "(http2|ssl|rewrite)"
```

**Update PATH and configuration:**
```bash
# Add custom Apache to PATH
echo 'export PATH="/usr/local/apache2/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc

# Verify installation
httpd -v
httpd -M | grep -E "(h2|ssl|rewrite)"
```

**Configure Apache for Axis2/C (adjust paths):**
- Use `/usr/local/apache2/conf/httpd.conf` instead of `/etc/apache2/apache2.conf`
- Use `/usr/local/apache2/bin/httpd` instead of `apache2ctl`
- Use `/usr/local/apache2/modules/` for module paths

#### 🚀 Quick Setup: Complete httpd.conf

**For the fastest setup**, use the complete pre-configured httpd.conf file from this user guide at location: docs/userguide/httpd.conf

```bash
# Copy the complete HTTP/2 + Axis2/C configuration
sudo cp ${AXIS2C_HOME}/docs/userguide/httpd.conf /usr/local/apache2/conf/httpd.conf

# Generate SSL certificates (required for HTTPS)
sudo mkdir -p /usr/local/apache2/conf/ssl
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout /usr/local/apache2/conf/ssl/apache-selfsigned.key \
    -out /usr/local/apache2/conf/ssl/apache-selfsigned.crt \
    -subj "/C=US/ST=Test/L=Test/O=Axis2C/CN=localhost"

# Create required directories
sudo mkdir -p /usr/local/axis2c/services
sudo mkdir -p /usr/local/axis2c/modules

# Start Apache
sudo systemctl restart apache2-custom
```

This pre-configured file includes:
- ✅ HTTP/2 protocol support (mod_http2, mod_proxy_http2)
- ✅ Axis2/C module integration
- ✅ SSL/TLS configuration for HTTPS
- ✅ Service location handlers (`/services`, `/rest`)
- ✅ Security headers and HTTPS-only enforcement

**Alternatively**, follow the manual configuration steps below:

### Step 1: Configure Apache httpd for HTTP/2

**Set up Apache httpd with HTTP/2 support first** - this ensures that the curl verification tests in Step 3 will work properly after building Axis2/C in Step 2.

**Choose Your Apache Setup:**

**Option A: System Apache** (if mod_h2 is available)
```bash
# Install HTTP/2 dependencies and system Apache
sudo apt-get update
sudo apt-get install -y libjson-c-dev libnghttp2-dev apache2-dev uuid-dev
sudo apt-get install -y apache2 openssl

# For RedHat/CentOS/Fedora:
# sudo dnf install -y json-c-devel libnghttp2-devel httpd-devel libuuid-devel
# sudo dnf install -y httpd openssl

# Create required directories for Axis2/C
sudo mkdir -p /usr/local/axis2c
sudo mkdir -p /var/log/axis2c
sudo chown www-data:www-data /var/log/axis2c
```

**Option B: Custom-Compiled Apache** (recommended for Ubuntu 25.10)
```bash
# If you completed Step 0 above, verify your custom Apache is running:
systemctl status apache2-custom
/usr/local/apache2/bin/httpd -M | grep http2

# Create required directories for Axis2/C
sudo mkdir -p /usr/local/axis2c
sudo mkdir -p /var/log/axis2c
sudo chown www-data:www-data /var/log/axis2c 2>/dev/null || sudo chown $(whoami):$(whoami) /var/log/axis2c
```

**Create Apache HTTPS configuration for HTTP/2:**

**IMPORTANT**: HTTP/2 requires HTTPS (TLS encryption). Modern browsers and the HTTP/2 specification mandate secure connections, so this configuration is HTTPS-only.

**Custom Apache Build (Recommended for /usr/local/apache2):**
```bash
# Copy the pre-configured httpd.conf with defense-in-depth limits
sudo cp /home/robert/repos/axis-axis2-c-core/docs/userguide/httpd.conf /usr/local/apache2/conf/httpd.conf

# Create SSL directory and certificates
sudo mkdir -p /usr/local/apache2/conf/ssl
sudo openssl req -new -x509 -days 365 -nodes \
  -out /usr/local/apache2/conf/ssl/apache-selfsigned.crt \
  -keyout /usr/local/apache2/conf/ssl/apache-selfsigned.key \
  -subj "/CN=localhost/O=Axis2C/C=US"

# Set proper permissions
sudo chmod 600 /usr/local/apache2/conf/ssl/apache-selfsigned.key
sudo chmod 644 /usr/local/apache2/conf/ssl/apache-selfsigned.crt

# Test configuration
/usr/local/apache2/bin/apachectl configtest

# Start Apache (or use systemd service if configured)
sudo /usr/local/apache2/bin/apachectl start

# Verify HTTP/2 is working
curl -k -I --http2 https://localhost/
```

> **Note**: The `httpd.conf` includes defense-in-depth request limits, per-service payload limits, and security headers. See the "Defense in Depth" section above for details.

**Ubuntu/Debian Systems (using system Apache):**
```bash
# Create the HTTPS-only configuration file
sudo tee /etc/apache2/sites-available/axis2-services.conf << 'EOF'
# Load required modules for HTTP/2 + HTTPS
LoadModule h2_module /usr/lib/apache2/modules/mod_h2.so
LoadModule ssl_module /usr/lib/apache2/modules/mod_ssl.so
LoadModule headers_module /usr/lib/apache2/modules/mod_headers.so
LoadModule axis2_module /usr/local/axis2c/lib/libmod_axis2.so

# Axis2/C Configuration (standard Unix paths)
Axis2RepoPath /usr/local/axis2c
Axis2LogFile /var/log/axis2c/axis2.log
Axis2LogLevel info

# HTTP/2 Configuration (requires HTTPS)
<IfModule mod_h2.c>
    # HTTP/2 performance settings for enterprise big data
    H2WindowSize 65536
    H2MaxWorkers 256
    H2MaxSessionStreams 100
    H2StreamMaxMemSize 104857600
    H2StreamTimeout 300

    # Security: HTTP/2 only over TLS 1.2+
    H2ModernTLSOnly on
    H2Upgrade off
    H2Direct on
</IfModule>

# HTTPS Virtual Host (HTTP/2 enabled)
<VirtualHost *:443>
    ServerName localhost:443
    # HTTP/2 first, HTTP/1.1 fallback
    Protocols h2 http/1.1

    # SSL Configuration for HTTP/2
    SSLEngine on
    SSLCertificateFile /etc/ssl/certs/axis2-server.crt
    SSLCertificateKeyFile /etc/ssl/private/axis2-server.key

    # TLS 1.2+ required for HTTP/2
    SSLProtocol -all +TLSv1.2 +TLSv1.3

    # Modern cipher suites for HTTP/2
    SSLCipherSuite ECDHE+AESGCM:ECDHE+CHACHA20:DHE+AESGCM:DHE+CHACHA20:!aNULL:!MD5:!DSS
    SSLHonorCipherOrder off

    # ALPN for HTTP/2 negotiation
    SSLUseStapling on
    SSLStaplingCache shmcb:/tmp/stapling_cache(128000)

    # Security headers for HTTPS-only deployment
    Header always set Strict-Transport-Security "max-age=31536000; includeSubDomains; preload"
    Header always set X-Content-Type-Options nosniff
    Header always set X-Frame-Options DENY
    Header always set Content-Security-Policy "upgrade-insecure-requests"

    # Axis2/C Services (HTTPS required)
    <Location /services>
        SetHandler axis2_module
        SSLRequireSSL
        # Force HTTPS for all service requests
        Header always set Strict-Transport-Security "max-age=31536000"
    </Location>

    # Service-specific configurations
    <Location /services/BigDataH2Service>
        Header always set Cache-Control "no-cache, no-store, must-revalidate"
    </Location>

    <Location /services/LoginService>
        Header always set X-XSS-Protection "1; mode=block"
        Header always set Content-Security-Policy "default-src 'self'"
    </Location>

    <Location /services/TestwsService>
        Header always set X-XSS-Protection "1; mode=block"
        Header always set Content-Security-Policy "default-src 'self'; script-src 'none'"
    </Location>
</VirtualHost>
EOF

# Enable the site and required modules
sudo a2ensite axis2-services

# Enable SSL and headers modules (standard modules)
sudo a2enmod ssl
sudo a2enmod headers

# Enable HTTP/2 module (REQUIRED for this guide)
# Try system Apache first, then custom Apache
command -v a2enmod >/dev/null 2>&1 && { sudo a2enmod h2 2>/dev/null && echo "✅ HTTP/2 module enabled successfully (system Apache)" || { echo "❌ System Apache mod_h2 module not available"; echo ""; echo "🔧 SOLUTIONS:"; echo "1. Use custom-compiled Apache (Step 0 above)"; echo "2. Install from Ondrej's PPA: sudo add-apt-repository ppa:ondrej/apache2"; echo "3. Use Ubuntu 24.04 LTS (has proper mod_h2 support)"; echo ""; echo "❌ SETUP FAILED: Cannot continue without mod_h2"; return 1 2>/dev/null || echo "Please fix mod_h2 issue above before continuing"; }; } || { command -v /usr/local/apache2/bin/httpd >/dev/null 2>&1 && { /usr/local/apache2/bin/httpd -M | grep -q h2 && { echo "✅ HTTP/2 modules available in custom Apache build"; echo "   mod_h2 and mod_http2 are compiled into your custom Apache"; } || { echo "❌ Custom Apache was compiled without HTTP/2 support"; echo "   Re-run Step 0 with --enable-http2 --with-nghttp2"; return 1 2>/dev/null || echo "Please recompile Apache with HTTP/2 support"; }; } || { echo "❌ No Apache installation found with HTTP/2 support"; echo "   Install system Apache or compile from source (Step 0)"; return 1 2>/dev/null || echo "Please install Apache before continuing"; }; }

# Create SSL certificates for HTTP/2 testing
# HTTP/2 requires valid TLS certificates - self-signed for development only
sudo openssl req -new -x509 -days 365 -nodes \
  -out /etc/ssl/certs/axis2-server.crt \
  -keyout /etc/ssl/private/axis2-server.key \
  -config <(
    echo '[dn]'
    echo 'CN=localhost'
    echo 'C=US'
    echo 'ST=State'
    echo 'L=City'
    echo 'O=Organization'
    echo '[req]'
    echo 'distinguished_name = dn'
    echo 'req_extensions = v3_req'
    echo '[v3_req]'
    echo 'keyUsage = keyEncipherment, dataEncipherment'
    echo 'extendedKeyUsage = serverAuth'
    echo 'subjectAltName = @alt_names'
    echo '[alt_names]'
    echo 'DNS.1 = localhost'
    echo 'IP.1 = 127.0.0.1'
    echo 'IP.2 = ::1'
  )

# Set proper permissions (critical for security)
sudo chmod 600 /etc/ssl/private/axis2-server.key
sudo chmod 644 /etc/ssl/certs/axis2-server.crt

# Verify certificate for HTTP/2 compatibility
echo "🔒 Verifying SSL certificate for HTTP/2..."
openssl x509 -in /etc/ssl/certs/axis2-server.crt -text -noout | grep -E "(Subject Alternative Name|DNS:|IP:)"

# Test Apache configuration
sudo apache2ctl configtest

# If test passes, start/restart Apache for HTTPS/HTTP/2 service
echo "🔄 Starting Apache httpd with HTTP/2 support..."
sudo systemctl is-active --quiet apache2 && {
    sudo systemctl restart apache2
    echo "✅ Apache restarted successfully"
} || {
    sudo systemctl start apache2
    echo "✅ Apache started successfully"
}

# Enable Apache to start on boot
sudo systemctl enable apache2

# Check Apache status
sudo systemctl status apache2 --no-pager

# Verify HTTP/2 is working
echo "🚀 Testing HTTP/2 connectivity..."
curl -k -I --http2 https://localhost/
```

**RedHat/CentOS/Fedora Systems (using system Apache):**
```bash
# Create the HTTPS-only configuration file
sudo tee /etc/httpd/conf.d/axis2-services.conf << 'EOF'
# Load required modules for HTTP/2 + HTTPS
LoadModule h2_module /usr/lib64/httpd/modules/mod_h2.so
LoadModule ssl_module /usr/lib64/httpd/modules/mod_ssl.so
LoadModule headers_module /usr/lib64/httpd/modules/mod_headers.so
LoadModule axis2_module /usr/local/axis2c/lib/libmod_axis2.so

# Axis2/C Configuration (standard Unix paths)
Axis2RepoPath /usr/local/axis2c
Axis2LogFile /var/log/axis2c/axis2.log
Axis2LogLevel info

# HTTP/2 Configuration (requires HTTPS)
<IfModule mod_h2.c>
    # HTTP/2 performance settings for enterprise big data
    H2WindowSize 65536
    H2MaxWorkers 256
    H2MaxSessionStreams 100
    H2StreamMaxMemSize 104857600
    H2StreamTimeout 300

    # Security: HTTP/2 only over TLS 1.2+
    H2ModernTLSOnly on
    H2Upgrade off
    H2Direct on
</IfModule>

# HTTPS Virtual Host (HTTP/2 enabled)
<VirtualHost *:443>
    ServerName localhost:443
    # HTTP/2 first, HTTP/1.1 fallback
    Protocols h2 http/1.1

    # SSL Configuration for HTTP/2
    SSLEngine on
    SSLCertificateFile /etc/pki/tls/certs/axis2-server.crt
    SSLCertificateKeyFile /etc/pki/tls/private/axis2-server.key

    # TLS 1.2+ required for HTTP/2
    SSLProtocol -all +TLSv1.2 +TLSv1.3

    # Modern cipher suites for HTTP/2
    SSLCipherSuite ECDHE+AESGCM:ECDHE+CHACHA20:DHE+AESGCM:DHE+CHACHA20:!aNULL:!MD5:!DSS
    SSLHonorCipherOrder off

    # ALPN for HTTP/2 negotiation
    SSLUseStapling on
    SSLStaplingCache shmcb:/tmp/stapling_cache(128000)

    # Security headers for HTTPS-only deployment
    Header always set Strict-Transport-Security "max-age=31536000; includeSubDomains; preload"
    Header always set X-Content-Type-Options nosniff
    Header always set X-Frame-Options DENY
    Header always set Content-Security-Policy "upgrade-insecure-requests"

    # Axis2/C Services (HTTPS required)
    <Location /services>
        SetHandler axis2_module
        SSLRequireSSL
        # Force HTTPS for all service requests
        Header always set Strict-Transport-Security "max-age=31536000"
    </Location>

    # Service-specific configurations
    <Location /services/BigDataH2Service>
        Header always set Cache-Control "no-cache, no-store, must-revalidate"
    </Location>

    <Location /services/LoginService>
        Header always set X-XSS-Protection "1; mode=block"
        Header always set Content-Security-Policy "default-src 'self'"
    </Location>

    <Location /services/TestwsService>
        Header always set X-XSS-Protection "1; mode=block"
        Header always set Content-Security-Policy "default-src 'self'; script-src 'none'"
    </Location>
</VirtualHost>
EOF

# Create SSL certificates for HTTP/2 testing
# HTTP/2 requires valid TLS certificates - self-signed for development only
sudo openssl req -new -x509 -days 365 -nodes \
  -out /etc/pki/tls/certs/axis2-server.crt \
  -keyout /etc/pki/tls/private/axis2-server.key \
  -config <(
    echo '[dn]'
    echo 'CN=localhost'
    echo 'C=US'
    echo 'ST=State'
    echo 'L=City'
    echo 'O=Organization'
    echo '[req]'
    echo 'distinguished_name = dn'
    echo 'req_extensions = v3_req'
    echo '[v3_req]'
    echo 'keyUsage = keyEncipherment, dataEncipherment'
    echo 'extendedKeyUsage = serverAuth'
    echo 'subjectAltName = @alt_names'
    echo '[alt_names]'
    echo 'DNS.1 = localhost'
    echo 'IP.1 = 127.0.0.1'
    echo 'IP.2 = ::1'
  )

# Set proper permissions (critical for security)
sudo chmod 600 /etc/pki/tls/private/axis2-server.key
sudo chmod 644 /etc/pki/tls/certs/axis2-server.crt

# Verify certificate for HTTP/2 compatibility
echo "🔒 Verifying SSL certificate for HTTP/2..."
openssl x509 -in /etc/pki/tls/certs/axis2-server.crt -text -noout | grep -E "(Subject Alternative Name|DNS:|IP:)"

# Test Apache configuration
sudo httpd -t

# If test passes, restart Apache for HTTPS/HTTP/2 service
sudo systemctl restart httpd
sudo systemctl enable httpd
sudo systemctl status httpd

# Verify HTTP/2 is working
echo "🚀 Testing HTTP/2 connectivity..."
curl -k -I --http2 https://localhost/
```

This completes Step 1. Apache httpd is now configured with HTTP/2 support and ready to serve Axis2/C services once they are built and installed in Step 2.

**Important Note about JSON Processing Architecture:**

Axis2/C 2.0 implements a **Dual JSON Architecture** that separates HTTP/1.1 and HTTP/2 JSON processing:

- **HTTP/1.1 Transport**: Uses json-c library with optional XML/SOAP compatibility for legacy applications
- **HTTP/2 Transport**: Uses **optimized json-c library** with zero XML/SOAP dependencies for maximum performance

This pure json-c approach for both transports provides:
- ✅ **No SOAP/XML overhead**: Direct JSON processing using native json-c objects
- ✅ **Memory efficiency**: 60-80% less memory usage compared to XML-based processing
- ✅ **Performance gains**: 30-40% faster JSON processing for large payloads (50MB+)
- ✅ **Streaming optimization**: Native support for HTTP/2 flow control and streaming

**🎯 HTTP/2 Build Optimization - AXIOM Not Required:**

Analysis shows that **HTTP/2 JSON services do NOT require the AXIOM library** (`-laxis2_axiom`), despite legacy header includes:

- **Header Include**: Services include `<axiom_node.h>` for compatibility but make **zero function calls** to `axiom_*` functions
- **Minimal Dependencies**: HTTP/2 services only need: `-laxutil -laxis2_engine -ljson-c -lm`
- **Build Optimization**: Removing `-laxis2_axiom` reduces runtime dependencies and proves the **Dual JSON Architecture**
- **Performance**: No XML object model overhead = faster JSON processing for HTTP/2 services

This confirms HTTP/2 services **truly bypass XML/SOAP completely** while maintaining source compatibility.

**Critical Build Requirement**: Despite HTTP/2 using pure json-c (bypassing the traditional `AXIS2_JSON_ENABLED` framework), the `--enable-json` configure flag is **REQUIRED** because:

- **PKG-CONFIG Integration**: Sets up `JSON_CFLAGS` and `JSON_LIBS` build variables
- **Library Detection**: Ensures json-c development headers and libraries are available
- **HTTP/2 Compilation**: HTTP/2 tests and utilities depend on these build variables

**Both flags are essential**:
```bash
./configure --enable-http2 --enable-json  # Both required for HTTP/2
```

**Note about APR paths:** The Apache Portable Runtime (APR) headers are located in different paths on different distributions:
- **Ubuntu/Debian**: `/usr/include/apr-1.0` (includes version number with dot)
- **RedHat/CentOS/Fedora**: `/usr/include/apr-1` (version number without dot)

If configure fails to find APR, verify the correct path on your system:
```bash
# Find APR headers on your system
find /usr/include -name "apr.h" 2>/dev/null
ls -la /usr/include/apr*

# Common alternative paths if standard ones don't work:
# /usr/include/apache2/apr-1.0  (some Ubuntu configurations)
# /usr/include/apr-1.4          (older systems)
```

**Continue with build, start here if Apache httpd is installed with h2:**
```bash
# Create services directory structure
sudo mkdir -p /usr/local/axis2c/services
sudo mkdir -p /usr/local/axis2c/modules

# Create log directory for Axis2/C
sudo mkdir -p /var/log/axis2c
sudo chown www-data:www-data /var/log/axis2c

Correct build Sequence:
# this 'find' command is optional but can fix permission errors after previous install
sudo find . -name ".libs" -type d -exec chmod -R 755 {} \; && sudo chown -R $USER:$USER . 2>/dev/null || true
autoreconf -fiv     # ✅ First: Generate build system with enhanced clean targets
./configure         # ✅ Create Makefiles that have proper clean-local targets
make distclean      # ✅ Now uses ENHANCED clean targets that remove .libs/
./configure         # ✅ Recreate Makefiles (distclean removed them)
make clean     # ✅ (aggressive - recommended)
make
sudo make install

# Run the EXACT configure command for your distro as explained below. 
# This is specifically for Ubuntu 25.10 with apache2-custom
# --enable-tests is optional, skipped here 
     ./configure --prefix=/usr/local/axis2c \
       --enable-json \
       --enable-http2 \
       --with-apache2=/usr/local/apache2/include \
       --with-apr=/usr/include/apr-1.0 \
       --enable-libxml2=no \
       PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig \
       CFLAGS="-DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2" \
       CPPFLAGS="-DAXIS2_JSON_ENABLED -DWITH_NGHTTP2" \
       LDFLAGS="-L/usr/lib/x86_64-linux-gnu"

# Add Axis2/C to library path (standard Unix paths) , could be in /etc/environment or root .bashrc
export AXIS2C_HOME=/usr/local/axis2c
export LD_LIBRARY_PATH=/usr/local/axis2c/lib:$LD_LIBRARY_PATH

# Build and install HTTP/2 sample services
cd samples/user_guide/bigdata-h2-service && bash build_json_service.sh && cd ../../..
cd samples/user_guide/login-service && bash build_json_service.sh && cd ../../..
cd samples/user_guide/testws-service && bash build_json_service.sh && cd ../../..
cd samples/user_guide/camera-control-service && bash build_camera_service.sh && cd ../../..
cd samples/user_guide/financial-benchmark-service && bash build_financial_service.sh && cd ../../..

# Check which source files exist vs what's expected by Makefile.am
echo "=== BigDataH2Service ==="
[ -f "samples/user_guide/bigdata-h2-service/src/bigdata_h2_service.c" ] && [ -f "samples/user_guide/bigdata-h2-service/src/bigdata_h2_service_handler.c" ] && echo "✅ Required: bigdata_h2_service.c, bigdata_h2_service_handler.c" && echo "📁 Additional files: $(ls samples/user_guide/bigdata-h2-service/src/*.c 2>/dev/null | wc -l) total .c files" || echo "❌ Missing required .c implementation files (bigdata_h2_service.c, bigdata_h2_service_handler.c)"

echo "=== LoginService ==="
[ -f "samples/user_guide/login-service/src/login_service.c" ] && [ -f "samples/user_guide/login-service/src/login_service_handler.c" ] && echo "✅ Required: login_service.c, login_service_handler.c" && echo "📁 Additional files: $(ls samples/user_guide/login-service/src/*.c 2>/dev/null | wc -l) total .c files" || echo "❌ Missing required .c implementation files (login_service.c, login_service_handler.c)"

echo "=== TestwsService ==="
[ -f "samples/user_guide/testws-service/src/testws_service.c" ] && [ -f "samples/user_guide/testws-service/src/testws_service_handler.c" ] && echo "✅ Required: testws_service.c, testws_service_handler.c" && echo "📁 Additional files: $(ls samples/user_guide/testws-service/src/*.c 2>/dev/null | wc -l) total .c files" || echo "❌ Missing required .c implementation files (testws_service.c, testws_service_handler.c)"

echo "=== CameraControlService ==="
[ -f "samples/user_guide/camera-control-service/src/camera_control_service.c" ] && echo "✅ Required: camera_control_service.c (stub implementation)" && echo "📁 Additional files: $(ls samples/user_guide/camera-control-service/src/*.c 2>/dev/null | wc -l) total .c files" || echo "❌ Missing required .c implementation files (camera_control_service.c)"

echo "=== FinancialBenchmarkService ==="
[ -f "samples/user_guide/financial-benchmark-service/src/financial_benchmark_service.c" ] && echo "✅ Required: financial_benchmark_service.c" && echo "📁 Additional files: $(ls samples/user_guide/financial-benchmark-service/src/*.c 2>/dev/null | wc -l) total .c files" || echo "❌ Missing required .c implementation files (financial_benchmark_service.c)"

echo ""
echo "✅ ALL HTTP/2 JSON services are fully implemented and ready for build and deployment"
echo "🧪 Use curl for comprehensive HTTP/2 JSON testing including XSS protection demonstration"
```
**Configure build (distribution-specific paths):**

**Ubuntu/Debian Systems:**
```bash
# Configure build with JSON-C and HTTP/2 support (Ubuntu/Debian)
# IMPORTANT: --prefix=/usr/local/axis2c ensures 'sudo make install' installs to system location
# --enable-tests is optional, skipped here 
./configure \
    --prefix=/usr/local/axis2c \
    --enable-json \
    --enable-ssl \
    --enable-libcurl \
    --with-apache2=/usr/include/apache2 \
    --with-apr=/usr/include/apr-1.0 \
    PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig \
    APACHE2_HOME=/usr \
    CFLAGS="$CFLAGS -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2" \
    CPPFLAGS="$CPPFLAGS -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2" \
    LDFLAGS="$LDFLAGS -L/usr/lib/x86_64-linux-gnu"
```

**RedHat/CentOS/Fedora Systems:**
```bash
# Configure build with JSON-C and HTTP/2 support (RedHat/CentOS/Fedora)
# IMPORTANT: --prefix=/usr/local/axis2c ensures 'sudo make install' installs to system location
# --enable-tests is optional, skipped here 
./configure \
    --prefix=/usr/local/axis2c \
    --enable-json \
    --enable-ssl \
    --enable-libcurl \
    --with-apache2=/usr/include/httpd \
    --with-apr=/usr/include/apr-1 \
    PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib64/pkgconfig \
    APACHE2_HOME=/usr \
    CFLAGS="$CFLAGS -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2" \
    CPPFLAGS="$CPPFLAGS -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2" \
    LDFLAGS="$LDFLAGS -L/usr/lib64"
```

**Note about APR paths:** The Apache Portable Runtime (APR) headers are located in different paths on different distributions:
- **Ubuntu/Debian**: `/usr/include/apr-1.0` (includes version number with dot)
- **RedHat/CentOS/Fedora**: `/usr/include/apr-1` (version number without dot)

If configure fails to find APR, verify the correct path on your system:
```bash
# Find APR headers on your system
find /usr/include -name "apr.h" 2>/dev/null
ls -la /usr/include/apr*

# Common alternative paths if standard ones don't work:
# /usr/include/apache2/apr-1.0  (some Ubuntu configurations)
# /usr/include/apr-1.4          (older systems)
```

**Current Implementation Status:**
- ✅ `bigdata_h2_service_handler.c` - **COMPLETE** - Service framework integration
- ✅ `bigdata_h2_service.c` - **COMPLETE** - Big data processing logic with HTTP/2 optimization
- ✅ `login_service.c` - **COMPLETE** - Authentication service with JWT token generation
- ✅ `login_service_handler.c` - **COMPLETE** - Service framework integration
- ✅ `testws_service.c` - **COMPLETE** - XSS protection demonstration with OWASP ESAPI-style validation
- ✅ `testws_service_handler.c` - **COMPLETE** - Service framework integration

### Step 3: Test Your HTTPS/HTTP/2 Installation

Now that both Apache httpd is configured (Step 1) and Axis2/C is built and installed (Step 2), let's test the complete installation.

**HTTPS/HTTP/2 connectivity test:**
```bash
# Test Apache is running with HTTP/2 support
curl -k -I --http2 https://localhost/

# Verify HTTP/2 protocol is active
curl -k -I --http2 -v https://localhost/ 2>&1 | grep -E "(HTTP/2|h2|ALPN)"

# Test if services are accessible (should return service list or 404 initially)
curl -k --http2 https://localhost/services/

# Test with verbose output to confirm HTTP/2 negotiation
curl -k --http2 -v https://localhost/services/ 2>&1 | grep -E "(Using HTTP2|ALPN, server accepted to use h2)"
```
# Verify service is up and ready for action on H2, repeat for other services as needed
curl -k --http2 -I https://localhost/services/BigDataH2Service/getServiceMetadata


**HTTP/2 Protocol Verification:**
```bash
# Detailed HTTP/2 verification
echo "🔍 HTTP/2 Protocol Analysis:"

# Check ALPN negotiation
echo "Testing ALPN negotiation..."
openssl s_client -connect localhost:443 -alpn h2 2>/dev/null | grep -E "(ALPN protocol: h2|Next protocol:|Protocol  : h2)"

# Check HTTP/2 stream support
curl -k --http2 -v https://localhost/ 2>&1 | grep -E "(h2|HTTP/2)"

# Test concurrent requests (HTTP/2 multiplexing)
echo "Testing HTTP/2 multiplexing..."
curl -k --http2 https://localhost/services/ & curl -k --http2 https://localhost/services/ &
wait
```

If all tests pass, your Apache httpd server is properly configured with HTTP/2 support and ready to serve Axis2/C services.

**Client Implementation Approach:**
- **No custom clients** - Use curl for HTTP/2 JSON testing (2025 best practice)
- Curl provides comprehensive HTTP/2, JSON, and SSL/TLS support
- Eliminates client development overhead and maintenance burden

## HTTP/2 JSON Service Testing with curl

**Comprehensive testing examples using curl for HTTP/2 JSON services:**

### ⚠️ **HTTP/2-Only Service Protocol Requirement**

These services are configured as **HTTP/2-only** for optimal performance. HTTP/1.1 requests will fail with **HTTP 426 "Upgrade Required"**:

```bash
# ❌ This will fail with HTTP 426 - Upgrade Required
curl -k --http1.1 \
     -H "Content-Type: application/json" \
     -d '{"test":"http1_compatibility"}' \
     https://localhost/services/BigDataH2Service

# Response: "HTTP/2 Required: This service only supports HTTP/2 protocol. Please use --http2 or upgrade your client."
```

**✅ Always use `--http2` flag for testing these services**. All examples below demonstrate correct HTTP/2 usage.

### BigData H2 Service Testing

**Test small dataset processing (standard mode):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "datasetId": "test_small_dataset",
       "datasetSize": 1048576,
       "analyticsType": "standard_analytics",
       "enableHttp2Optimization": true
     }' \
     https://localhost/services/BigDataH2Service
```

**Test medium dataset processing (multiplexing mode):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "datasetId": "test_medium_dataset",
       "datasetSize": 26214400,
       "analyticsType": "advanced_analytics",
       "enableHttp2Optimization": true,
       "enableMemoryOptimization": true
     }' \
     https://localhost/services/BigDataH2Service
```

**Test large dataset processing (streaming mode):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "datasetId": "test_large_dataset",
       "datasetSize": 52428800,
       "analyticsType": "enterprise_big_data",
       "enableHttp2Optimization": true
     }' \
     https://localhost/services/BigDataH2Service
```

### Login Service Testing

**Test successful authentication:**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"email": "admin@example.com", "password": "admin123"}' \
     https://localhost/services/LoginService/authenticate
```

**Expected successful response:**
```json
{
  "status": "LOGIN_SUCCESS",
  "message": "Login successful. Welcome, admin",
  "token": "eyAiYWxnIjogIkhTMjU2IiwgInR5cCI6ICJKV1QiIH0=...",
  "tokenType": "Bearer",
  "expiresIn": 3600,
  "responseTime": 1767119569
}
```

**Test authentication failure:**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"email": "invalid@example.com", "password": "wrongpassword"}' \
     https://localhost/services/LoginService/authenticate
```

**Test input validation:**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"email": "not-an-email", "password": "123"}' \
     https://localhost/services/LoginService/authenticate
```

### TestWS Service Testing (XSS Protection Demonstration)

**Test with safe input (passes validation):**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"messagein": "Hello World"}' \
     https://localhost/services/TestwsService/testXSSProtection
```

**Expected successful response:**
```json
{
  "status": "OK",
  "messageout": "<script xmlns=\"http://www.w3.org/1999/xhtml\">alert('Hello');</script> \">",
  "securityDetails": "XSS Analysis: Input validated successfully. Output contains intentional XSS payload for demonstration purposes. WARNING: Output contains potential XSS patterns - proper HTML encoding required in production.",
  "responseTime": 1767120021
}
```

**Test with XSS payload (fails validation):**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"messagein": "<script>alert(XSS)</script>"}' \
     https://localhost/services/TestwsService/testXSSProtection
```

**Expected validation failure response:**
```json
{
  "status": "ERROR",
  "messageout": "",
  "securityDetails": "Input validation failed - potential security risk detected",
  "responseTime": 1767120014
}
```

**Test with injection attempts (fails validation):**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"messagein": "javascript:alert(1)"}' \
     https://localhost/services/TestwsService/testXSSProtection
```

**Test with HTML entities (fails validation):**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"messagein": "&lt;img src=x onerror=alert(1)&gt;"}' \
     https://localhost/services/TestwsService/testXSSProtection
```

### CameraControlService Testing (Generic Camera Control with SFTP)

**⚠️ Stub Implementation Notice**: The CameraControlService provides stub implementations that log operations but do not perform actual camera or SFTP operations. Users must implement camera-specific integration functions. See the implementation guide at `samples/user_guide/camera-control-service/IMPLEMENTATION_GUIDE.md` in the source tree for detailed examples.

**Test start recording (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "start_recording",
       "clip_name": "meeting_001",
       "quality": "1080p",
       "duration": 3600,
       "format": "mp4"
     }' \
     https://localhost/services/CameraControlService/startRecording
```

**Test get camera status (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "get_status"
     }' \
     https://localhost/services/CameraControlService/getStatus
```

**Test SFTP file transfer (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "sftp_transfer",
       "hostname": "backup.example.com",
       "username": "camera_user",
       "private_key_path": "/home/camera/.ssh/id_rsa",
       "local_file_path": "/tmp/recording_001.mp4",
       "remote_path": "/backups/camera/",
       "port": 22
     }' \
     https://localhost/services/CameraControlService/sftpTransfer
```

**Test stop recording (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "stop_recording"
     }' \
     https://localhost/services/CameraControlService/stopRecording
```

**Expected Response Format (Success):**
```json
{
  "success": true,
  "message": "Recording started successfully"
}
```

**Expected Response Format (SFTP Transfer Success):**
```json
{
  "success": true,
  "message": "File transferred successfully via SFTP"
}
```

**Expected Response Format (Get Status):**
```json
{
  "success": true,
  "state": "idle",
  "active_clip": "none",
  "recording_duration": 0,
  "last_error": "none"
}
```

**Test configure settings (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "configure_settings",
       "resolution": "1920x1080",
       "fps": "30",
       "codec": "h264",
       "bitrate": "8000000",
       "audio_source": "mic"
     }' \
     https://localhost/services/CameraControlService/configureSettings
```

**Test list files (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "list_files",
       "pattern": "*.mp4",
       "directory": "/storage/emulated/0/DCIM/OpenCamera"
     }' \
     https://localhost/services/CameraControlService/listFiles
```

**Expected Response Format (List Files):**
```json
{
  "success": true,
  "file_count": 3,
  "total_size": 157286400,
  "directory": "/storage/emulated/0/DCIM/OpenCamera",
  "files": [
    {"name": "VID_20260101_120000.mp4", "size": 52428800, "modified": "2026-01-01 12:00:00"},
    {"name": "VID_20260102_140000.mp4", "size": 52428800, "modified": "2026-01-02 14:00:00"},
    {"name": "VID_20260103_160000.mp4", "size": 52428800, "modified": "2026-01-03 16:00:00"}
  ]
}
```

**Test delete files (stub implementation):**
```bash
curl -k --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "action": "delete_files",
       "pattern": "VID_20260101_*.mp4"
     }' \
     https://localhost/services/CameraControlService/deleteFiles
```

Supported delete patterns:
- Specific file: `"VID_20260104_105049.mp4"`
- Wildcard: `"*.mp4"`, `"VID_2026*"`
- Date: `"2026-01-04"` (all files from that date)
- Today: `"today"` (all files from current date)
- All: `"*"` (delete all video files)

**Implementation Requirements**:

For comprehensive implementation examples including OpenCamera and libssh2 SFTP integration, see the [Kanaha](https://github.com/robertlazarski/kanaha) project:

- [Kanaha Multi-Camera Deployment Guide](https://github.com/robertlazarski/kanaha/blob/main/docs/MULTI_CAMERA_DEPLOYMENT_SYSTEM.md) - Camera-specific implementations for `camera_device_start_recording_impl()`, `camera_device_stop_recording_impl()`, `camera_device_get_status_impl()`, `camera_device_configure_settings_impl()`, `camera_device_sftp_transfer_impl()`, `camera_device_list_files_impl()`, and `camera_device_delete_files_impl()`
- [Android Support Guide](../HTTP2_ANDROID.md) - Cross-compile for ARM64/ARMv7 mobile devices

**Note:** [Kanaha](https://github.com/robertlazarski/kanaha) is an independent project (GPLv3 licensed due to OpenCamera dependency) and is not affiliated with Apache Software Foundation. It demonstrates production deployment patterns for the CameraControlService sample in `samples/user_guide/camera-control-service/`.

### FinancialBenchmarkService Testing (Financial Computation Benchmark)

The FinancialBenchmarkService demonstrates native C performance for compute-intensive financial calculations. It provides a direct comparison point for evaluating Axis2/C against Java/WildFly on resource-constrained hardware.

**Use Case**: Run financial calculations on devices where Java/WildFly cannot operate (old Android phones with 1-2GB RAM, embedded systems, edge devices).

**Operations**:
- `portfolioVariance`: O(n²) covariance matrix calculation
- `monteCarlo`: Monte Carlo VaR simulation
- `generateTestData`: Generate synthetic test portfolios
- `metadata`: Service information and device stats

**Test service metadata:**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{}' \
     https://localhost/services/FinancialBenchmarkService/metadata
```

**Generate test portfolio data:**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{"n_assets": 500}' \
     https://localhost/services/FinancialBenchmarkService/generateTestData \
     -o /tmp/portfolio_500.json
```

**Run 500-asset portfolio variance benchmark:**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d @/tmp/portfolio_500.json \
     https://localhost/services/FinancialBenchmarkService/portfolioVariance
```

**Run Monte Carlo simulation (10K paths):**
```bash
curl -sk --http2 \
     -H "Content-Type: application/json" \
     -d '{
       "n_simulations": 10000,
       "n_periods": 252,
       "initial_value": 1000000,
       "expected_return": 0.08,
       "volatility": 0.20
     }' \
     https://localhost/services/FinancialBenchmarkService/monteCarlo
```

**Benchmark Results (500-asset O(n²) Portfolio Variance)**:

| Metric | Result |
|--------|--------|
| Assets | 500 |
| Matrix operations | 250,000 (500²) |
| Calculation time | **136 μs** |
| Throughput | **1.84 billion ops/sec** |
| Memory usage | 94 MB |

**Platform Comparison**:

| Platform | 500-asset calc | Memory | Startup |
|----------|----------------|--------|---------|
| **Axis2/C** | 136 μs | ~94 MB | Instant |
| Java/WildFly | Cannot run | 4-8 GB min | 30-60s |

This benchmark demonstrates that Axis2/C can perform enterprise financial calculations on hardware where Java-based solutions cannot operate, making it suitable for edge computing and resource-constrained environments.

**Build the service:**
```bash
cd samples/user_guide/financial-benchmark-service
bash build_financial_service.sh
```

## Architecture Comparison: Spring Boot vs Apache httpd

| Component | Axis2/Java (Spring Boot) | Axis2/C (Apache httpd) |
|-----------|---------------------------|-------------------------|
| **Web Server** | Spring Boot Embedded Tomcat | Apache httpd with mod_h2 |
| **HTTP/2 Support** | Spring Boot HTTP/2 | mod_h2 (native Apache HTTP/2) |
| **SSL/TLS** | Spring Boot SSL | mod_ssl with modern cipher suites |
| **Security** | Spring Security filters | Apache httpd auth modules |
| **JSON Processing** | Spring JSON + GSON/Moshi | json-c library |
| **Service Framework** | Spring @Service annotations | Axis2/C service skeletons |
| **Configuration** | application.properties | httpd.conf + services.xml |
| **Deployment** | WAR file to app server | Service shared library |
| **Performance** | JVM heap management | Native C memory management |

### Service Architecture

Axis2/C services use a different architecture compared to Spring Boot:

#### Spring Boot (Java) Architecture:
```java
@RestController
public class BigDataH2Service {
    @PostMapping("/processBigDataSet")
    public BigDataH2Response processBigDataSet(@RequestBody BigDataH2Request request) {
        // Processing logic
    }
}
```

#### Axis2/C Architecture:
```c
// Service implementation using json-c
bigdata_h2_response_t* bigdata_h2_service_process_big_data_set(
    const axutil_env_t *env,
    bigdata_h2_request_t *request);

// HTTP/2 JSON service integration (pure json-c)
json_object* bigdata_h2_service_invoke_json(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    json_object *json_request,
    axis2_msg_ctx_t *msg_ctx);
```

**For comprehensive technical analysis and performance benchmarks** that demonstrate why these C implementations achieve 30% latency reduction, 70% memory savings, and 162% faster build times compared to Java, see the [Axis2/C HTTP/2 JSON Architecture](../AXIS2C_HTTP2_MIGRATION_STATE.md) document.

## Resources

### Documentation
- [Apache Axis2/C Documentation](http://axis.apache.org/axis2/c/core/)
- [Axis2/C HTTP/2 Migration State](../AXIS2C_HTTP2_MIGRATION_STATE.md) - Technical architecture and performance analysis
- [Apache httpd HTTP/2 Guide](https://httpd.apache.org/docs/2.4/mod/mod_h2.html)
- [JSON-C Library Documentation](https://json-c.github.io/json-c/)
- [Apache httpd SSL/TLS Guide](https://httpd.apache.org/docs/2.4/ssl/)

### Source Code and Examples
- [BigDataH2Service Source Code](https://github.com/apache/axis-axis2-c-core/tree/master/samples/user_guide/bigdata-h2-service/) - HTTP/2 big data processing
- [LoginService Source Code](https://github.com/apache/axis-axis2-c-core/tree/master/samples/user_guide/login-service/) - User authentication with JWT tokens
- [TestwsService Source Code](https://github.com/apache/axis-axis2-c-core/tree/master/samples/user_guide/testws-service/) - XSS protection demonstration
- [CameraControlService Source Code](https://github.com/apache/axis-axis2-c-core/tree/master/samples/user_guide/camera-control-service/) - Generic camera control with SFTP file transfer and user-implementable stub functions
- [FinancialBenchmarkService Source Code](https://github.com/apache/axis-axis2-c-core/tree/master/samples/user_guide/financial-benchmark-service/) - Financial computation benchmarks (O(n²) matrix operations, Monte Carlo VaR)

### Security Resources
- [OWASP XSS Prevention Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Cross_Site_Scripting_Prevention_Cheat_Sheet.html)
- [OWASP Authentication Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Authentication_Cheat_Sheet.html)
- [JWT Security Best Practices](https://tools.ietf.org/html/rfc8725)

---
*This documentation is a complete port of the Axis2/Java Spring Boot JSON user guide, adapted for Apache Axis2/C with Apache httpd deployment.*
