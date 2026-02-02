# Downloads

## Source Code

### GitHub Repository

The latest source code is available on GitHub:

- **Repository**: [https://github.com/apache/axis-axis2-c-core](https://github.com/apache/axis-axis2-c-core)

```bash
# Clone the repository
git clone https://github.com/apache/axis-axis2-c-core.git
cd axis-axis2-c-core

# Or clone with SSH (requires GitHub account)
git clone git@github.com:apache/axis-axis2-c-core.git
```

### Branches

| Branch | Description |
|--------|-------------|
| `main` | Current development (HTTP/2 + JSON focus) |
| `trunk` | Legacy branch (SOAP-era code) |

### Release Tags

Releases are tagged in the repository:

```bash
# List available tags
git tag -l

# Checkout specific version
git checkout tags/v1.7.0
```

## Building from Source

See the `INSTALL` file in the repository root for complete build instructions:

```bash
git clone https://github.com/apache/axis-axis2-c-core.git
cd axis-axis2-c-core
cat INSTALL
```

The project uses GNU autotools (autoconf, automake, libtool).

### Android Libraries

For Android deployment, see [Android Support](HTTP2_ANDROID.md) for cross-compilation instructions.

## Dependencies

Axis2/C requires these dependencies:

| Library | Version | Purpose | Required |
|---------|---------|---------|----------|
| OpenSSL | 1.1.1+ | TLS/SSL support | Yes |
| nghttp2 | 1.40+ | HTTP/2 protocol | For HTTP/2 |
| Jansson | 2.12+ | JSON parsing | For JSON-RPC |
| libxml2 | 2.9+ | XML/SOAP parsing | For SOAP |

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install libssl-dev libnghttp2-dev libjansson-dev libxml2-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install openssl-devel nghttp2-devel jansson-devel libxml2-devel
```

**macOS (Homebrew):**
```bash
brew install openssl nghttp2 jansson libxml2
```

## Verification

### GPG Signatures

Apache releases are signed with GPG. To verify:

```bash
# Import Apache keys
wget https://downloads.apache.org/axis/axis2/c/KEYS
gpg --import KEYS

# Verify signature
gpg --verify axis2c-src-*.tar.gz.asc axis2c-src-*.tar.gz
```

### SHA-512 Checksums

Verify download integrity:

```bash
sha512sum -c axis2c-src-*.tar.gz.sha512
```

## Archive

Historical releases are available in the Apache archive:

- [Apache Axis2/C Archive](https://archive.apache.org/dist/axis/axis2/c/)

## See Also

- [Installation Guide](getting-started/installation.md)
- [Building from Source](getting-started/building.md)
