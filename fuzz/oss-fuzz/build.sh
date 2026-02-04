#!/bin/bash -eu
# Copyright 2024 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Build Axis2/C with fuzzing instrumentation
cd $SRC/axis2c

# Provide defaults for potentially unset variables
: "${LDFLAGS:=}"

# Generate configure script from autotools
autoreconf -i

# Configure with fuzzing flags
# --enable-json is required for axis2_json_reader library
./configure \
    --prefix=$WORK/install \
    --enable-static \
    --disable-shared \
    --enable-json \
    CFLAGS="$CFLAGS" \
    LDFLAGS="$LDFLAGS"

make -j$(nproc)
make install

# Build fuzz targets
AXIS2_INCLUDES="-I$WORK/install/include/axis2-2.0.0 -I$WORK/install/include/axis2-2.0.0/platforms"

# Static libraries - order matters for linker (dependents before dependencies)
AXIS2_LIBS="$WORK/install/lib/libaxis2_engine.a \
            $WORK/install/lib/libaxis2_http_util.a \
            $WORK/install/lib/libaxis2_http_common.a \
            $WORK/install/lib/libaxis2_axiom.a \
            $WORK/install/lib/libaxis2_parser.a \
            $WORK/install/lib/libguththila.a \
            $WORK/install/lib/libaxutil.a"

# System libs - use static linking where possible
# Find static json-c library
JSONC_STATIC=$(find /usr -name "libjson-c.a" 2>/dev/null | head -1)
if [ -z "$JSONC_STATIC" ]; then
    JSONC_STATIC="/usr/lib/x86_64-linux-gnu/libjson-c.a"
fi

# Core system libs (no json-c for non-JSON fuzzers)
# -lm for math (ceilf), -lstdc++ for C++ runtime (libFuzzer)
SYS_LIBS_CORE="-lxml2 -lssl -lcrypto -lz -lpthread -ldl -lm -lstdc++"

# Compile fuzz targets that don't need json-c
for target in fuzz_xml_parser fuzz_http_header fuzz_url_parser; do
    $CC $CFLAGS $AXIS2_INCLUDES \
        fuzz/${target}.c \
        -o $OUT/${target} \
        $LIB_FUZZING_ENGINE \
        $AXIS2_LIBS $SYS_LIBS_CORE ${LDFLAGS}
done

# Compile JSON fuzzer with static json-c (tests json-c library directly)
$CC $CFLAGS -I/usr/include/json-c \
    fuzz/fuzz_json_parser.c \
    -o $OUT/fuzz_json_parser \
    $LIB_FUZZING_ENGINE \
    $JSONC_STATIC -lpthread -lm -lstdc++ ${LDFLAGS}

# Compile JSON Reader fuzzer (tests Axis2/C JSON-to-AXIOM conversion)
# This requires both Axis2/C libs AND json-c
# Note: WITH_NGHTTP2 enables json-c types in axis2_json_reader.h
# Note: -fsanitize=address needed for linking ASAN-instrumented libraries
$CC $CFLAGS $AXIS2_INCLUDES -I/usr/include/json-c \
    -I$WORK/install/include/axis2-2.0.0 \
    -DWITH_NGHTTP2 \
    -fsanitize=address \
    fuzz/fuzz_json_reader.c \
    -o $OUT/fuzz_json_reader \
    $LIB_FUZZING_ENGINE \
    $AXIS2_LIBS $JSONC_STATIC $SYS_LIBS_CORE ${LDFLAGS}

# Copy seed corpora
for corpus_dir in xml json http url; do
    if [ -d "fuzz/corpus/${corpus_dir}" ]; then
        zip -j $OUT/fuzz_${corpus_dir}_parser_seed_corpus.zip fuzz/corpus/${corpus_dir}/* 2>/dev/null || true
    fi
done

# HTTP corpus goes to http_header target
if [ -f "$OUT/fuzz_http_parser_seed_corpus.zip" ]; then
    mv $OUT/fuzz_http_parser_seed_corpus.zip $OUT/fuzz_http_header_seed_corpus.zip
fi

# JSON reader fuzzer can reuse JSON corpus (same input format)
if [ -f "$OUT/fuzz_json_parser_seed_corpus.zip" ]; then
    cp $OUT/fuzz_json_parser_seed_corpus.zip $OUT/fuzz_json_reader_seed_corpus.zip
fi
