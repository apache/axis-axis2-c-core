# OSS-Fuzz Integration for Apache Axis2/C

## What is OSS-Fuzz?

[OSS-Fuzz](https://github.com/google/oss-fuzz) is Google's continuous fuzzing service for open source projects. It runs fuzz tests 24/7 on Google's infrastructure, automatically reporting crashes, memory leaks, and undefined behavior.

### Key Concepts

| Term | Description |
|------|-------------|
| **Fuzzing** | Automated testing technique that generates random/semi-random inputs to find bugs |
| **libFuzzer** | LLVM's coverage-guided fuzzer that Axis2/C uses |
| **Sanitizers** | Runtime checkers (AddressSanitizer, UndefinedBehaviorSanitizer) that detect bugs |
| **Corpus** | Collection of test inputs that trigger different code paths |
| **Seed Corpus** | Initial inputs to help the fuzzer start exploring |
| **Coverage-guided** | Fuzzer tracks which code paths are hit and generates inputs to explore new paths |

### Why Fuzzing Matters for Security

Traditional testing checks expected inputs. Fuzzing finds bugs with *unexpected* inputs:

- Buffer overflows from malformed XML/JSON
- Integer overflows in size calculations
- NULL pointer dereferences from missing fields
- Memory leaks from error paths
- Stack exhaustion from deeply nested structures

## Axis2/C Fuzz Targets

### Overview

The `fuzz/` directory contains 5 libFuzzer-compatible targets:

| Target | Component | Protocol Path | Attack Vectors |
|--------|-----------|---------------|----------------|
| `fuzz_xml_parser.c` | Guththila XML parser | SOAP/XML | XXE, XML bombs, buffer overflows |
| `fuzz_json_parser.c` | json-c library | HTTP/2 JSON | CVE-2020-12762, integer overflows |
| `fuzz_json_reader.c` | axis2_json_reader | HTTP/1.1 JSON | JSON→AXIOM conversion bugs |
| `fuzz_http_header.c` | HTTP header parsing | HTTP transport | Header injection, overflows |
| `fuzz_url_parser.c` | URL parsing (axutil_url) | Client URLs | SSRF, malformed URL handling |

### JSON Fuzzer Architecture

Axis2/C has two JSON processing paths with dedicated fuzzers:

```
┌─────────────────────────────────────────────────────────────────────┐
│                        JSON Fuzzing Coverage                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  HTTP/1.1 Path (fuzz_json_reader.c)                                 │
│  ┌──────────┐    ┌───────────────────┐    ┌──────────────┐         │
│  │   JSON   │───▶│ axis2_json_reader │───▶│ axiom_node_t*│         │
│  │  Input   │    │  (JSON→AXIOM)     │    │  (XML tree)  │         │
│  └──────────┘    └───────────────────┘    └──────────────┘         │
│                                                                      │
│  HTTP/2 Path (fuzz_json_parser.c)                                   │
│  ┌──────────┐    ┌───────────────────┐    ┌──────────────┐         │
│  │   JSON   │───▶│json_tokener_parse │───▶│ json_object* │         │
│  │  Input   │    │  (direct json-c)  │    │ (native JSON)│         │
│  └──────────┘    └───────────────────┘    └──────────────┘         │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

See [JSON_HTTP11_VS_HTTP2.md](JSON_HTTP11_VS_HTTP2.md) for detailed architecture comparison.

## Running Fuzz Tests Locally

### Prerequisites

```bash
# Install Docker (required for OSS-Fuzz)
sudo apt-get install docker.io
sudo usermod -aG docker $USER
# Log out and back in for group changes

# Clone OSS-Fuzz repository
git clone https://github.com/google/oss-fuzz.git
cd oss-fuzz
```

### Method 1: Using OSS-Fuzz Infrastructure (Recommended)

This method uses Docker containers identical to Google's infrastructure:

```bash
cd /path/to/oss-fuzz

# Build all fuzzers with AddressSanitizer
sudo python3 infra/helper.py build_fuzzers --sanitizer address axis2c /path/to/axis-axis2-c-core

# Verify all fuzzers built successfully
sudo python3 infra/helper.py check_build axis2c

# Run a specific fuzzer for 60 seconds
sudo python3 infra/helper.py run_fuzzer axis2c fuzz_xml_parser -- -max_total_time=60
sudo python3 infra/helper.py run_fuzzer axis2c fuzz_url_parser -- -max_total_time=60
sudo python3 infra/helper.py run_fuzzer axis2c fuzz_http_header -- -max_total_time=60
sudo python3 infra/helper.py run_fuzzer axis2c fuzz_json_parser -- -max_total_time=60
sudo python3 infra/helper.py run_fuzzer axis2c fuzz_json_reader -- -max_total_time=60
```

### Method 2: Interactive Docker Debugging

For debugging build issues or crashes:

```bash
# Start interactive container with source mounted
sudo docker run --privileged --shm-size=2g --platform linux/amd64 --rm -it \
  -e FUZZING_ENGINE=libfuzzer \
  -e SANITIZER=address \
  -e ARCHITECTURE=x86_64 \
  -e PROJECT_NAME=axis2c \
  -e SRC=/src \
  -e WORK=/work \
  -e OUT=/out \
  -e LIB_FUZZING_ENGINE=/usr/local/lib/clang/22/lib/x86_64-unknown-linux-gnu/libclang_rt.fuzzer.a \
  -v /path/to/axis-axis2-c-core:/src/axis2c \
  -v /path/to/oss-fuzz/build/out/axis2c/:/out \
  -v /path/to/oss-fuzz/build/work/axis2c:/work \
  gcr.io/oss-fuzz/axis2c bash

# Inside container:
cd /src/axis2c
bash fuzz/oss-fuzz/build.sh

# Run fuzzer directly
/out/fuzz_xml_parser -max_total_time=60
```

### Method 3: Local Build Without Docker

For quick local testing (requires clang):

```bash
cd /path/to/axis-axis2-c-core

# Build Axis2/C with fuzzing instrumentation
export CC=clang
export CFLAGS="-fsanitize=address,fuzzer-no-link -g -O1"
./configure --prefix=$HOME/axis2c-fuzz --enable-static --disable-shared --enable-json
make clean && make -j$(nproc) && make install

# Compile a single fuzzer
clang -fsanitize=address,fuzzer \
    -I$HOME/axis2c-fuzz/include/axis2-2.0.0 \
    -I$HOME/axis2c-fuzz/include/axis2-2.0.0/platforms \
    fuzz/fuzz_url_parser.c \
    $HOME/axis2c-fuzz/lib/*.a \
    -lxml2 -lssl -lcrypto -lz -lpthread -ldl -lm -lstdc++ \
    -o fuzz_url_parser

# Run with seed corpus
./fuzz_url_parser fuzz/corpus/url/ -max_total_time=300
```

## Understanding Fuzzer Output

### Successful Run

```
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 2342191498
INFO: seed corpus: files: 8 min: 42b max: 119b total: 592b rss: 33Mb
#2      INITED exec/s: 0 rss: 33Mb
#1048576        pulse  corp: 15/2Kb lim: 4096 exec/s: 524288 rss: 412Mb
#11927997       DONE   corp: 15/2Kb lim: 4096 exec/s: 384774 rss: 413Mb
Done 11927997 runs in 31 second(s)
```

Key metrics:
- `exec/s`: Executions per second (higher = faster fuzzing)
- `corp`: Corpus size (inputs that found new coverage)
- `rss`: Memory usage in MB
- `DONE`: Clean exit, no crashes found

### Crash Found

```
==14==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008
    #0 0x558b4ebcfe62 in axis2_json_reader_read axis2_json_reader.c:418:5
    #1 0x558b4eb37ffb in LLVMFuzzerTestOneInput fuzz_json_reader.c:154:33
SUMMARY: AddressSanitizer: SEGV axis2_json_reader.c:418:5 in axis2_json_reader_read
artifact_prefix='./'; Test unit written to ./crash-b6abd567fa79cbe0196d093a067271361dc6ca8b
```

The crash file contains the exact input that triggered the bug. Reproduce with:

```bash
./fuzz_json_reader ./crash-b6abd567fa79cbe0196d093a067271361dc6ca8b
```

### Memory Leak

```
==12345==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 128 byte(s) in 1 object(s) allocated from:
    #0 malloc
    #1 axutil_stream_create
    ...
```

## Seed Corpus

Initial test cases in `fuzz/corpus/` help the fuzzer start with valid inputs:

```
fuzz/corpus/
├── xml/
│   ├── simple_element.xml      # Basic XML element
│   ├── namespaces.xml          # XML with namespaces
│   ├── soap_envelope.xml       # SOAP envelope structure
│   └── deep_nesting.xml        # Deeply nested elements
├── json/
│   ├── valid_object.json       # Simple JSON object
│   ├── valid_array.json        # JSON array
│   ├── deep_nesting.json       # Deeply nested JSON
│   ├── jsonrpc_request.json    # JSON-RPC 2.0 request
│   ├── login_request.json      # Authentication payload
│   └── bigdata_request.json    # HTTP/2 service request
├── http/
│   ├── get_request.txt         # HTTP GET request line
│   ├── post_request.txt        # HTTP POST with headers
│   ├── content_type_header.txt # Content-Type header
│   └── status_200.txt          # HTTP status line
└── url/
    ├── http_url.txt            # HTTP URL with port/path
    └── https_url.txt           # HTTPS URL
```

### Adding New Corpus Files

When you find inputs that trigger new code paths, add them to the corpus:

```bash
# Good corpus files are:
# - Small (under 1KB ideally)
# - Trigger unique code paths
# - Valid or "almost valid" inputs

# Example: Add a new JSON test case
echo '{"method":"test","params":[1,2,3]}' > fuzz/corpus/json/jsonrpc_method.json
```

## Build System Files

### fuzz/oss-fuzz/build.sh

The main build script that OSS-Fuzz runs:

```bash
#!/bin/bash -eu
# Build Axis2/C with fuzzing instrumentation

cd $SRC/axis2c

# Generate configure script (required for fresh clones)
autoreconf -i

# Configure with JSON support (required for JSON fuzzers)
./configure \
    --prefix=$WORK/install \
    --enable-static \
    --disable-shared \
    --enable-json \
    CFLAGS="$CFLAGS" \
    LDFLAGS="$LDFLAGS"

make -j$(nproc)
make install

# Compile each fuzzer with appropriate libraries
# ... (see actual build.sh for full details)
```

### fuzz/oss-fuzz/Dockerfile

Defines the build environment:

```dockerfile
FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update && apt-get install -y \
    autoconf automake libtool pkg-config \
    libxml2-dev libssl-dev libjson-c-dev
COPY . $SRC/axis2c
WORKDIR $SRC/axis2c
COPY fuzz/oss-fuzz/build.sh $SRC/
```

### fuzz/oss-fuzz/project.yaml

Project metadata for OSS-Fuzz:

```yaml
homepage: "https://axis.apache.org/axis2/c/core/"
language: c
primary_contact: "dev@axis.apache.org"
sanitizers:
  - address
  - undefined
fuzzing_engines:
  - libfuzzer
```

## Fixing Bugs Found by Fuzzing

### 1. Reproduce the Crash

```bash
# Get the crash file from OSS-Fuzz or local run
./fuzz_target ./crash-xxxxx
```

### 2. Analyze with GDB

```bash
# Build without fuzzer, with debug symbols
export CFLAGS="-fsanitize=address -g -O0"
# ... rebuild ...

gdb --args ./fuzz_target ./crash-xxxxx
(gdb) run
(gdb) bt  # backtrace when it crashes
```

### 3. Fix and Verify

```bash
# After fixing, verify the crash is gone
./fuzz_target ./crash-xxxxx
# Should exit cleanly now

# Run full fuzzer to check for regressions
./fuzz_target -max_total_time=300
```

### 4. Add Regression Test

Add the crash input to the corpus so it's always tested:

```bash
cp ./crash-xxxxx fuzz/corpus/appropriate_dir/regression_issue_123.bin
```

## Common Issues and Solutions

### Build Fails: "libjson-c.a not found"

The Docker container may have an older json-c. Check version:

```bash
pkg-config --modversion json-c
```

If too old, the build.sh handles this by finding the static library manually.

### Build Fails: "undefined reference to __asan_*"

The Axis2/C libraries were built with AddressSanitizer but linking doesn't include ASAN runtime:

```bash
# Add -fsanitize=address to link command
clang -fsanitize=address ... -o fuzzer
```

### Build Fails: "undefined reference to ceilf"

Missing math library:

```bash
# Add -lm to link command
clang ... -lm -o fuzzer
```

### Fuzzer Runs But "no interesting inputs found"

This warning is normal if:
- The fuzzer is rejecting most random inputs (expected for parsers)
- The code isn't instrumented for coverage (rebuild with proper flags)

Use seed corpus to start with valid inputs:

```bash
./fuzzer fuzz/corpus/appropriate_dir/ -max_total_time=60
```

## Lessons Learned from Local Testing

### Docker Permission Issues

When running OSS-Fuzz helper scripts, you may encounter Docker permission errors:

```
permission denied while trying to connect to the Docker daemon socket
```

**Solutions:**

1. **Quick fix**: Use `sudo` with helper.py commands
   ```bash
   sudo python3 infra/helper.py build_fuzzers axis2c
   ```

2. **Permanent fix**: Add your user to the docker group
   ```bash
   sudo usermod -aG docker $USER
   newgrp docker  # or log out and back in
   ```

### Build Requires autoreconf

When OSS-Fuzz clones a fresh repository, the `configure` script doesn't exist - it must be generated from `configure.ac`. The `fuzz/oss-fuzz/build.sh` script runs `autoreconf -i` before `./configure` to handle this.

If you see this error:
```
./configure: No such file or directory
```

Ensure `autoreconf -i` runs before `./configure` in the build script.

### Validated Test Results

All 5 fuzz targets have been validated with libFuzzer + AddressSanitizer:

| Target | Runs | Exec/sec | Result |
|--------|------|----------|--------|
| fuzz_json_reader | 8M+ | ~131K | No crashes |
| fuzz_json_parser | 4M+ | ~130K | No crashes |
| fuzz_xml_parser | 4M+ | ~120K | No crashes |
| fuzz_http_header | 4M+ | ~125K | No crashes |
| fuzz_url_parser | 4M+ | ~115K | No crashes |

### Comparison with Axis2/Java

Apache Axis2/Java has been in OSS-Fuzz since January 2023. Key differences:

| Aspect | Axis2/Java | Axis2/C |
|--------|------------|---------|
| Added | January 2023 | February 2025 |
| Language | Java (Jazzer) | C (libFuzzer) |
| Fuzzer location | Inside oss-fuzz repo | Inside Axis2/C repo |
| Approach | HTTP interface fuzzing | Direct parser fuzzing |
| Maintainer | External (Code Intelligence) | Apache committers |

The Axis2/C approach of keeping fuzzer code in the main repo allows Apache committers to maintain and update fuzzers without requiring PRs to google/oss-fuzz.

## Security Vulnerability Reporting

If fuzzing finds a security vulnerability:

1. **Do NOT** open a public GitHub issue
2. Report to Apache Security Team: security@apache.org
3. Include:
   - Crash file (input that triggers the bug)
   - Stack trace from AddressSanitizer
   - Axis2/C version and commit hash
   - Brief description of impact

## Repository Structure and Maintenance

### Why Two Repositories?

OSS-Fuzz maintains project configurations in the [google/oss-fuzz](https://github.com/google/oss-fuzz) repository. However, this creates a challenge: Axis2/C committers cannot directly update fuzzing code without submitting pull requests to google/oss-fuzz and waiting for review.

To solve this, we use a **thin wrapper approach**:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    OSS-Fuzz Repository Structure                         │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│  google/oss-fuzz/projects/axis2c/                                       │
│  ├── Dockerfile      ← Minimal: just clones Axis2/C repo                │
│  ├── build.sh        ← Thin wrapper: calls Axis2/C's build.sh           │
│  └── project.yaml    ← OSS-Fuzz metadata (rarely changes)               │
│                                                                          │
│  apache/axis-axis2-c-core/fuzz/                                         │
│  ├── fuzz_*.c        ← All fuzz target source code                      │
│  ├── corpus/         ← Seed corpus files                                │
│  └── oss-fuzz/                                                          │
│      └── build.sh    ← Actual build logic (Axis2/C committers control)  │
│                                                                          │
└─────────────────────────────────────────────────────────────────────────┘
```

### What Lives Where

| Repository | Files | Who Maintains | Changes Require |
|------------|-------|---------------|-----------------|
| google/oss-fuzz | `Dockerfile`, `build.sh` (wrapper), `project.yaml` | OSS-Fuzz team | PR to google/oss-fuzz |
| axis-axis2-c-core | `fuzz/*.c`, `fuzz/corpus/`, `fuzz/oss-fuzz/build.sh` | Axis2/C committers | Commit to Axis2/C |

### How Updates Work

1. **Adding/modifying fuzz targets**: Edit files in `axis-axis2-c-core/fuzz/`. Commit directly to Axis2/C repository. OSS-Fuzz picks up changes automatically on next build.

2. **Changing build configuration**: Edit `axis-axis2-c-core/fuzz/oss-fuzz/build.sh`. No PR to google/oss-fuzz needed.

3. **Adding seed corpus**: Add files to `axis-axis2-c-core/fuzz/corpus/`. Commit to Axis2/C.

4. **Changing OSS-Fuzz metadata** (rare): Submit PR to google/oss-fuzz for `project.yaml` changes (e.g., adding new sanitizers, changing contact email).

### The Thin Wrapper Pattern

The `build.sh` in google/oss-fuzz is intentionally minimal:

```bash
#!/bin/bash -eu
# Wrapper script that calls the build.sh maintained in the Axis2/C repository.
# This allows Axis2/C committers to update fuzzing configuration without
# requiring pull requests to the google/oss-fuzz repository.

cd $SRC/axis2c
exec bash fuzz/oss-fuzz/build.sh
```

The `Dockerfile` simply clones Axis2/C and installs dependencies:

```dockerfile
FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update && apt-get install -y \
    autoconf automake libtool pkg-config \
    libxml2-dev libjson-c-dev libssl-dev zlib1g-dev
RUN git clone --depth 1 https://github.com/apache/axis-axis2-c-core.git axis2c
WORKDIR axis2c
COPY build.sh $SRC/
```

Note: `autoreconf -i` is run inside `build.sh` rather than in the Dockerfile, keeping all build logic in the Axis2/C repository.

## Licensing Considerations

### License Compatibility

Both repositories use **Apache License 2.0**:

| Repository | License | SPDX Identifier |
|------------|---------|-----------------|
| google/oss-fuzz | Apache-2.0 | Apache-2.0 |
| axis-axis2-c-core | Apache-2.0 | Apache-2.0 |

This makes the integration straightforward from a licensing perspective.

### ASF Policy Compliance

The Apache Software Foundation has [clear policies](https://www.apache.org/legal/resolved.html) on acceptable licenses. Key points for this integration:

1. **Apache-2.0 to Apache-2.0**: Fully compatible. No license conflict.

2. **No license change**: The fuzz targets in Axis2/C remain under Apache-2.0. The thin wrapper in google/oss-fuzz is also Apache-2.0.

3. **Contributor License Agreement**: Contributors to Axis2/C fuzzing code follow normal Apache CLA requirements.

4. **Third-party dependencies**: The fuzz targets link against:
   - libxml2 (MIT License) - Category A
   - json-c (MIT License) - Category A
   - OpenSSL (Apache-2.0 since 3.0) - Category A
   - zlib (zlib License) - Category A

   All are [Category A](https://www.apache.org/legal/resolved.html#category-a) (can be included).

5. **OSS-Fuzz infrastructure**: Google's fuzzing infrastructure is a service, not a code dependency. Using the service does not create license obligations.

### File Headers

All fuzz target files should include the standard Apache header:

```c
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
```

## Related Documentation

- [SECURITY.md](SECURITY.md) - Overall security guidelines
- [JSON_HTTP11_VS_HTTP2.md](JSON_HTTP11_VS_HTTP2.md) - JSON architecture details
- [OSS-Fuzz Documentation](https://google.github.io/oss-fuzz/) - Official OSS-Fuzz docs
- [libFuzzer Documentation](https://llvm.org/docs/LibFuzzer.html) - Fuzzer engine docs
