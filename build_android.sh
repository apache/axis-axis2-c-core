#!/bin/bash
# Build Axis2/C for Android ARM64
set -e

ANDROID_NDK_HOME=$HOME/Android/Sdk/ndk/28.0.12916984
TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64
DEPS=$HOME/android-cross-builds/deps/arm64-v8a

echo "=== Axis2/C Android Cross-Compilation ==="
echo "NDK: $ANDROID_NDK_HOME"
echo "Toolchain: $TOOLCHAIN"
echo "Deps: $DEPS"

# Verify toolchain exists
if [ ! -f "$TOOLCHAIN/bin/aarch64-linux-android21-clang" ]; then
    echo "ERROR: NDK compiler not found"
    exit 1
fi

# Configure
CC="$TOOLCHAIN/bin/aarch64-linux-android21-clang" \
AR="$TOOLCHAIN/bin/llvm-ar" \
RANLIB="$TOOLCHAIN/bin/llvm-ranlib" \
CFLAGS="-fPIC -std=gnu99 -I$DEPS/include -I$DEPS/include/apr-1" \
LDFLAGS="-L$DEPS/lib" \
LIBS="-lapr-1 -laprutil-1 -lexpat -lssl -lcrypto -lnghttp2 -ljson-c" \
./configure \
    --host=aarch64-linux-android \
    --prefix=$DEPS \
    --with-apache2=$HOME/android-cross-builds/httpd-2.4.66 \
    --with-apr=$DEPS \
    --enable-static \
    --disable-shared \
    --enable-http2 \
    --enable-json \
    --with-openssl=$DEPS

echo "=== Configure complete, building ==="

# Build
make -j$(nproc)

echo "=== Build complete ==="
