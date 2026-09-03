# Building from Source

## Build System

Apache Axis2/C uses GNU autotools (autoconf, automake, libtool) for building.

## Build Instructions

Complete build instructions are in the `INSTALL` file in the repository root:

```bash
git clone https://github.com/apache/axis-axis2-c-core.git
cd axis-axis2-c-core
cat INSTALL
```

## Quick Reference

### Linux

```bash
# Clone from GitHub
git clone https://github.com/apache/axis-axis2-c-core.git
cd axis-axis2-c-core

# Follow instructions in INSTALL file
# Typically:
./configure [options]
make
sudo make install
```

### Configure Options

The `INSTALL` file documents all configure options. Common options include:

| Option | Description |
|--------|-------------|
| `--enable-guththila` | Use Guththila XML parser |
| `--enable-libxml2` | Use libxml2 XML parser |
| `--enable-openssl` | Enable SSL/TLS support |
| `--with-nghttp2` | Enable HTTP/2 support |

### Windows

Windows is supported through WSL2; see Section 2 of the `INSTALL` file. The
Visual Studio project files and the win32 nmake build were removed in
September 2026 -- they targeted Visual Studio 6/2005/2008, which current
Visual Studio cannot open, and had not been maintained since 2011.

### Android Cross-Compilation

See [Android Support](../HTTP2_ANDROID.md) for cross-compiling Axis2/C for Android devices.

## Build System Details

For detailed information about the build system internals, see:

- [Build System Changes](../BUILD_SYSTEM_HTTP2_CHANGES.md)
- [Conditional Compilation](../HTTP2_CONDITIONAL_COMPILATION.md)
- [Library Loading](../HTTP2_LIBRARY_LOADING.md)
