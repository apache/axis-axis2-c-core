# OSS-Fuzz Integration for Apache Axis2/C

## What is Fuzz Testing?

Fuzz testing (fuzzing) automatically generates random, malformed, or unexpected inputs to find bugs that manual testing misses. It's especially effective at discovering:

- Buffer overflows and memory corruption
- Crashes from malformed input
- Integer overflows
- Denial of service vulnerabilities

## What is OSS-Fuzz?

[OSS-Fuzz](https://github.com/google/oss-fuzz) is Google's free continuous fuzzing service for open source projects. Once integrated, it runs 24/7 on Google's infrastructure, automatically:

- Testing your code with millions of inputs daily
- Reporting crashes and vulnerabilities privately
- Tracking bug fixes and regressions

## Eligibility

Apache Axis2/C qualifies for OSS-Fuzz because it:

1. **Is widely used** - Deployed in enterprise SOAP/web services
2. **Processes untrusted input** - Parses XML, JSON, HTTP from network
3. **Is open source** - Apache License 2.0
4. **Has an active community** - Apache Software Foundation project

Other Apache projects using OSS-Fuzz include Apache httpd (which Axis2/C uses as a dependency), Apache Commons, and Apache Tomcat.

## Fuzz Targets

The `fuzz/` directory contains these targets:

| Target | Tests | Security Focus |
|--------|-------|----------------|
| `fuzz_xml_parser.c` | AXIOM XML/SOAP parsing | XXE, XML bombs, buffer overflows |
| `fuzz_json_parser.c` | JSON parsing (HTTP/2 mode) | DoS, integer overflows (CVE-2020-12762) |
| `fuzz_http_header.c` | HTTP header/request/status parsing | Header injection, buffer overflows |
| `fuzz_url_parser.c` | URL parsing | SSRF, malformed URLs |

## Local Testing

### Prerequisites

```bash
# Install clang with fuzzer support
sudo apt-get install clang llvm

# Build Axis2/C with AddressSanitizer
export CC=clang
export CFLAGS="-fsanitize=address,fuzzer-no-link -g"
./configure --prefix=/usr/local
make
```

### Run a Fuzz Target Locally

```bash
# Compile fuzz target
clang -fsanitize=address,fuzzer \
    -I/usr/local/include/axis2-2.0.0 \
    -I/usr/local/include/axis2-2.0.0/platforms \
    fuzz/fuzz_http_header.c \
    -L/usr/local/lib -laxis2_http_sender -laxutil \
    -o fuzz_http_header

# Run with seed corpus
./fuzz_http_header fuzz/corpus/http/

# Run for 60 seconds
./fuzz_http_header -max_total_time=60 fuzz/corpus/http/
```

## OSS-Fuzz Project Setup

To submit Axis2/C to OSS-Fuzz, create a PR to [google/oss-fuzz](https://github.com/google/oss-fuzz) with:

```
projects/axis2c/
├── Dockerfile      # Build environment
├── build.sh        # Compilation script
├── project.yaml    # Project metadata
```

See `fuzz/oss-fuzz/` for ready-to-use templates.

## Seed Corpus

The `fuzz/corpus/` directory contains seed inputs that help the fuzzer explore code paths faster:

- `corpus/xml/` - SOAP envelopes, XML with namespaces
- `corpus/json/` - Valid JSON objects, arrays, nested structures
- `corpus/http/` - HTTP request lines, headers, status lines
- `corpus/url/` - HTTP/HTTPS URLs with various components

## Handling Bug Reports

When OSS-Fuzz finds a bug:

1. You receive a private email with crash details
2. Fix the bug within 90 days (before public disclosure)
3. OSS-Fuzz automatically verifies the fix
4. After 30 days of being fixed, the bug report becomes public

## Resources

- [OSS-Fuzz Documentation](https://google.github.io/oss-fuzz/)
- [libFuzzer Tutorial](https://llvm.org/docs/LibFuzzer.html)
- [Fuzzing Best Practices](https://google.github.io/oss-fuzz/getting-started/new-project-guide/)
