#!/bin/bash
#
# Deploy Axis2/C HTTP/2 JSON services on localhost (Ubuntu 22.04 system Apache)
#
# This script is designed for a Debian/Ubuntu system where Apache httpd is
# already installed from packages and serves other sites (e.g., Maven repo
# on port 80). It adds Axis2/C services on port 443 (HTTPS) without
# disturbing existing sites.
#
# Prerequisites:
#   - Run on localhost as root (or user with sudo access)
#   - Source tree at /usr/local/axis-axis2-c-core
#
# Usage:
#   bash /usr/local/axis-axis2-c-core/docs/userguide/deploy-localhost.sh
#

set -euo pipefail

AXIS2C_SRC=/usr/local/axis-axis2-c-core
AXIS2C_PREFIX=/usr/local/axis2c
AXIS2C_LOG=/var/log/axis2c

echo "=== Axis2/C Deployment for localhost (Ubuntu 22.04 System Apache) ==="
echo "Source tree: $AXIS2C_SRC"
echo ""

# ─────────────────────────────────────────────────────────────────────────────
# Step 1: Install build dependencies
# ─────────────────────────────────────────────────────────────────────────────
echo "── Step 1: Installing build dependencies ──"
apt-get update -qq
apt-get install -y -qq \
    build-essential autotools-dev autoconf automake libtool pkg-config \
    apache2-dev libssl-dev libcurl4-openssl-dev libjson-c-dev libjson-c5 \
    libxml2-dev uuid-dev zlib1g-dev libnghttp2-dev openssl

# ─────────────────────────────────────────────────────────────────────────────
# Step 2: Build and install Axis2/C
# ─────────────────────────────────────────────────────────────────────────────
echo ""
echo "── Step 2: Building Axis2/C ──"

mkdir -p "$AXIS2C_PREFIX/services" "$AXIS2C_PREFIX/modules"
mkdir -p "$AXIS2C_LOG"
chown www-data:www-data "$AXIS2C_LOG"

cd "$AXIS2C_SRC"

# Clean build
find . -name ".libs" -type d -exec chmod -R 755 {} \; 2>/dev/null || true
chown -R "$USER:$USER" . 2>/dev/null || true

autoreconf -fiv
./configure \
    --prefix="$AXIS2C_PREFIX" \
    --enable-json \
    --enable-http2 \
    --with-apache2=/usr/include/apache2 \
    --with-apr=/usr/include/apr-1.0 \
    PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig \
    APACHE2_HOME=/usr \
    CFLAGS="-DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2" \
    CPPFLAGS="-DAXIS2_JSON_ENABLED -DWITH_NGHTTP2" \
    LDFLAGS="-L/usr/lib/x86_64-linux-gnu"

make -j"$(nproc)"
make install

# Build sample services (skip CameraControlService — Android only)
echo ""
echo "── Building sample services ──"
cd samples/user_guide/bigdata-h2-service && bash build_json_service.sh && cd "$AXIS2C_SRC"
cd samples/user_guide/login-service && bash build_json_service.sh && cd "$AXIS2C_SRC"
cd samples/user_guide/testws-service && bash build_json_service.sh && cd "$AXIS2C_SRC"
cd samples/user_guide/financial-benchmark-service && bash build_financial_service.sh && cd "$AXIS2C_SRC"

# ─────────────────────────────────────────────────────────────────────────────
# Step 3: Enable Apache modules
# ─────────────────────────────────────────────────────────────────────────────
echo ""
echo "── Step 3: Enabling Apache modules ──"
a2enmod ssl
a2enmod headers
a2enmod http2

# ─────────────────────────────────────────────────────────────────────────────
# Step 4: Install Axis2/C site config
# ─────────────────────────────────────────────────────────────────────────────
echo ""
echo "── Step 4: Installing axis2-services site config ──"
cp "$AXIS2C_SRC/docs/userguide/axis2-services.conf" \
    /etc/apache2/sites-available/axis2-services.conf

# ─────────────────────────────────────────────────────────────────────────────
# Step 5: Generate self-signed SSL certificate
# ─────────────────────────────────────────────────────────────────────────────
echo ""
echo "── Step 5: Generating self-signed SSL certificate ──"
if [ ! -f /etc/ssl/certs/axis2-server.crt ]; then
    openssl req -new -x509 -days 365 -nodes \
        -out /etc/ssl/certs/axis2-server.crt \
        -keyout /etc/ssl/private/axis2-server.key \
        -subj "/CN=$(hostname -f)/O=Axis2C/C=US"
    chmod 600 /etc/ssl/private/axis2-server.key
    chmod 644 /etc/ssl/certs/axis2-server.crt
    echo "Certificate generated."
else
    echo "Certificate already exists, skipping."
fi

# ─────────────────────────────────────────────────────────────────────────────
# Step 6: Enable site, configure library path, restart
# ─────────────────────────────────────────────────────────────────────────────
echo ""
echo "── Step 6: Enabling site and restarting Apache ──"

# Ensure Apache can find Axis2/C shared libraries
echo "$AXIS2C_PREFIX/lib" | tee /etc/ld.so.conf.d/axis2c.conf > /dev/null
ldconfig

# Enable the site
a2ensite axis2-services

# Test configuration before restart
echo ""
echo "Testing Apache configuration..."
if apache2ctl configtest; then
    echo ""
    echo "Config OK — restarting Apache..."
    systemctl restart apache2
else
    echo ""
    echo "ERROR: Apache config test failed. Rolling back..."
    a2dissite axis2-services
    echo "Site disabled. Existing sites are unaffected."
    exit 1
fi

# ─────────────────────────────────────────────────────────────────────────────
# Step 7: Verify
# ─────────────────────────────────────────────────────────────────────────────
echo ""
echo "── Step 7: Verification ──"

# Check Maven repo (port 80) is still up
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://localhost/ 2>/dev/null || echo "000")
if [ "$HTTP_CODE" = "200" ]; then
    echo "Port 80 (Maven repo): OK (HTTP $HTTP_CODE)"
else
    echo "Port 80 (Maven repo): WARNING (HTTP $HTTP_CODE) — check default site"
fi

# Check HTTPS is responding
HTTP_CODE=$(curl -k -s -o /dev/null -w "%{http_code}" https://localhost/ 2>/dev/null || echo "000")
echo "Port 443 (HTTPS):     HTTP $HTTP_CODE"

# Check HTTP/2 negotiation
H2_CHECK=$(curl -k -s --http2 -o /dev/null -w "%{http_version}" https://localhost/ 2>/dev/null || echo "?")
echo "HTTP/2 negotiation:   version $H2_CHECK"

# Test FinancialBenchmarkService
echo ""
echo "Testing FinancialBenchmarkService..."
curl -k --http2 -s \
    -H "Content-Type: application/json" \
    -d '{"benchmark":[{"arg0":{"portfolioSize":10,"iterations":100}}]}' \
    https://localhost/services/FinancialBenchmarkService/benchmark \
    | head -c 200
echo ""

echo ""
echo "=== Deployment complete ==="
echo ""
echo "Rollback if needed:"
echo "  a2dissite axis2-services && systemctl restart apache2"
