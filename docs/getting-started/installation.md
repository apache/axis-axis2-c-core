# Installation

## Getting the Source

### Clone from GitHub

```bash
git clone https://github.com/apache/axis-axis2-c-core.git
cd axis-axis2-c-core
```

**Repository:** [https://github.com/apache/axis-axis2-c-core](https://github.com/apache/axis-axis2-c-core)

## Build Instructions

Complete build instructions are in the `INSTALL` file in the repository root. The project uses GNU autotools.

```bash
# View the full installation guide
cat INSTALL
```

### Quick Overview

The `INSTALL` file covers:

1. **Getting Axis2/C Working on Linux**
   - Prerequisites (mandatory and optional)
   - Using binary release
   - Building from source
   - Running samples

2. **Getting Axis2/C Working on Windows**
   - WSL2 (recommended)
   - Legacy native build (unsupported since 2009)

3. **Apache2 Web Server Integration (mod_axis2)**
   - Building from source
   - Deploying in Apache2
   - HTTP/2 support

4. **CGI Deployment**

Note: IIS integration was removed in 2026 due to being unmaintained since 2009.

## Prerequisites

### Required Dependencies

See the `INSTALL` file for the complete list. Key dependencies include:

- **OpenSSL** - For TLS/SSL support
- **nghttp2** - For HTTP/2 protocol support (optional)
- **libxml2** or **Guththila** - For XML parsing

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install build-essential autoconf automake libtool pkg-config \
    libssl-dev libnghttp2-dev libxml2-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc autoconf automake libtool pkgconfig \
    openssl-devel nghttp2-devel libxml2-devel
```

## Platform-Specific Notes

### Android Cross-Compilation

See [Android Support](../HTTP2_ANDROID.md) for cross-compiling Axis2/C for Android devices.

## Next Steps

- [Quick Start Guide](quickstart.md) - Create your first service
- [Configuration](../guide/axis2-xml.md) - Configure axis2.xml
