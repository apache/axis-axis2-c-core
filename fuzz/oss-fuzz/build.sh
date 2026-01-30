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

# Configure with fuzzing flags
./configure \
    --prefix=$WORK/install \
    --enable-static \
    --disable-shared \
    CFLAGS="$CFLAGS" \
    LDFLAGS="$LDFLAGS"

make -j$(nproc)
make install

# Build fuzz targets
AXIS2_INCLUDES="-I$WORK/install/include/axis2-2.0.0 -I$WORK/install/include/axis2-2.0.0/platforms"
AXIS2_LIBS="$WORK/install/lib/libaxis2_axiom.a \
            $WORK/install/lib/libaxis2_parser.a \
            $WORK/install/lib/libaxutil.a \
            $WORK/install/lib/libaxis2_http_sender.a \
            $WORK/install/lib/libaxis2_http_receiver.a \
            $WORK/install/lib/libaxis2_http_common.a \
            $WORK/install/lib/libaxis2_engine.a"
SYS_LIBS="-lxml2 -lssl -lcrypto -ljson-c -lz -lpthread"

# Compile each fuzz target
for target in fuzz_xml_parser fuzz_json_parser fuzz_http_header fuzz_url_parser; do
    $CC $CFLAGS $AXIS2_INCLUDES \
        fuzz/${target}.c \
        -o $OUT/${target} \
        $LIB_FUZZING_ENGINE \
        $AXIS2_LIBS $SYS_LIBS $LDFLAGS
done

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
