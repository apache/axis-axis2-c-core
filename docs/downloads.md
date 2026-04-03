# Downloads

## Current Release: 2.0.0

Apache Axis2/C 2.0.0 is the current release — the first release since 1.6.0 (2009).
It includes 17 years of bug fixes, HTTP/2 support, and JSON-RPC services.

| File | SHA-512 | Signature |
|------|---------|-----------|
| [axis2c-src-2.0.0.tar.gz](https://downloads.apache.org/axis/axis2/c/2.0.0/axis2c-src-2.0.0.tar.gz) | [SHA-512](https://downloads.apache.org/axis/axis2/c/2.0.0/axis2c-src-2.0.0.tar.gz.sha512) | [ASC](https://downloads.apache.org/axis/axis2/c/2.0.0/axis2c-src-2.0.0.tar.gz.asc) |

**KEYS file**: [https://downloads.apache.org/axis/axis2/c/KEYS](https://downloads.apache.org/axis/axis2/c/KEYS)

See [Release Notes 2.0.0](release-notes-2.0.0.md) for the full change log.

### Verifying Downloads

```bash
# Import Apache Axis2/C release signing keys
wget https://downloads.apache.org/axis/axis2/c/KEYS
gpg --import KEYS

# Verify signature
gpg --verify axis2c-src-2.0.0.tar.gz.asc axis2c-src-2.0.0.tar.gz

# Verify checksum
sha512sum -c axis2c-src-2.0.0.tar.gz.sha512
```

---

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
| `master` | Current development |

### Release Tags

Releases are tagged in the repository:

```bash
# List available tags
git tag -l

# Checkout specific version
git checkout tags/v2.0.0
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
| json-c | 0.18+ | JSON parsing | For JSON-RPC |
| libxml2 | 2.9+ | XML/SOAP parsing | For SOAP |

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install libssl-dev libnghttp2-dev libjson-c-dev libxml2-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install openssl-devel nghttp2-devel json-c-devel libxml2-devel
```

**macOS (Homebrew):**
```bash
brew install openssl nghttp2 json-c libxml2
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
