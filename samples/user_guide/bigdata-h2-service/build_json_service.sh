#!/bin/bash
set -e

echo "🚀 Building BigDataH2 JSON Service - AXIOM-FREE HTTP/2 Architecture"

# Create services directory
echo "📁 Creating services directory..."
sudo mkdir -p /usr/local/axis2c/services/BigDataH2Service

echo "🔨 Building with PURE JSON dependencies (AXIOM-FREE)..."
echo "❌ EXCLUDED: -laxis2_axiom (XML object model not needed for pure JSON)"
echo "✅ INCLUDING: -laxutil -laxis2_engine -ljson-c -lm"

gcc -shared -fPIC \
    -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2 \
    -I/usr/local/axis2c/include/axis2-2.0.0 \
    -I../../include \
    -I../../util/include \
    -I../../src/core/transport/http/util \
    -I/usr/include/json-c \
    src/bigdata_h2_service.c \
    src/bigdata_h2_service_handler.c \
    -o libbigdata_h2_json_service.so \
    -L/usr/local/axis2c/lib \
    -laxutil -laxis2_engine \
    -ljson-c -lm

echo "Build exit code: $?"

if [ -f "libbigdata_h2_json_service.so" ]; then
    echo ""
    echo "🎉 SUCCESS: BigDataH2 JSON Service built with AXIOM-FREE architecture!"

    # Show file info
    ls -la libbigdata_h2_json_service.so
    file libbigdata_h2_json_service.so

    # Check dependencies
    echo ""
    echo "🔍 Checking runtime dependencies..."
    echo "📋 Service-level: No direct AXIOM usage (pure JSON processing)"
    echo "🔧 Engine-level: AXIOM present as transitive dependency (expected in Axis2/C)"
    # Show axis2 related dependencies only
    ldd libbigdata_h2_json_service.so | grep -E '(axis2|json|libax)' || echo "   (Dependencies shown above in full ldd output)"

    # Install the service
    echo ""
    echo "🚀 Installing AXIOM-free JSON service..."
    sudo cp libbigdata_h2_json_service.so /usr/local/axis2c/services/BigDataH2Service/libbigdata_h2_service.so
    sudo cp services.xml /usr/local/axis2c/services/BigDataH2Service/

    # Fix ownership and permissions for Apache (runs as www-data)
    sudo chown -R www-data:www-data /usr/local/axis2c/services/BigDataH2Service
    sudo chmod 755 /usr/local/axis2c/services/BigDataH2Service
    sudo chmod 644 /usr/local/axis2c/services/BigDataH2Service/services.xml
    sudo chmod 755 /usr/local/axis2c/services/BigDataH2Service/libbigdata_h2_service.so

    echo "📋 Installed files:"
    ls -la /usr/local/axis2c/services/BigDataH2Service/

    # Apache module + config: only needed for source-built Apache (/usr/local/apache2).
    # Packaged Apache (Debian/Ubuntu) uses deploy-localhost.sh + axis2-services.conf instead.
    if [ -d "/usr/local/apache2" ]; then
        echo ""
        echo "🔧 Source-built Apache detected at /usr/local/apache2 — configuring module..."

        # Copy Axis2/C module to Apache modules directory
        if [ ! -f "/usr/local/apache2/modules/mod_axis2.so" ]; then
            AXIS2_MODULE=""
            if [ -n "${AXIS2C_HOME:-}" ] && [ -f "$AXIS2C_HOME/lib/libmod_axis2.so.0.7.0" ]; then
                AXIS2_MODULE="$AXIS2C_HOME/lib/libmod_axis2.so.0.7.0"
            elif [ -f "/usr/local/axis2c/lib/libmod_axis2.so.0.7.0" ]; then
                AXIS2_MODULE="/usr/local/axis2c/lib/libmod_axis2.so.0.7.0"
            elif [ -f "../../../deploy/lib/libmod_axis2.so.0.7.0" ]; then
                AXIS2_MODULE="../../../deploy/lib/libmod_axis2.so.0.7.0"
            elif [ -f "../../deploy/lib/libmod_axis2.so.0.7.0" ]; then
                AXIS2_MODULE="../../deploy/lib/libmod_axis2.so.0.7.0"
            fi

            if [ -n "$AXIS2_MODULE" ]; then
                sudo cp "$AXIS2_MODULE" /usr/local/apache2/modules/mod_axis2.so
                echo "✅ Axis2/C module copied from: $AXIS2_MODULE"
            else
                echo "⚠️  Axis2/C module not found. Please set AXIS2C_HOME or ensure Axis2/C is built."
                echo "   Searched: \$AXIS2C_HOME/lib, /usr/local/axis2c/lib, ../../../deploy/lib"
            fi
        else
            echo "✅ Axis2/C module already installed"
        fi

        # Create Apache configuration for Axis2/C
        AXIS2_CONF="/usr/local/apache2/conf/extra/httpd-axis2.conf"
        if [ ! -f "$AXIS2_CONF" ]; then
            echo ""
            echo "📝 Creating Axis2/C Apache configuration..."
            sudo tee "$AXIS2_CONF" > /dev/null << 'CONFEOF'
# Axis2/C Configuration for HTTP/2 JSON Services
LoadModule axis2_module modules/mod_axis2.so
Protocols h2 h2c http/1.1
Axis2RepoPath /usr/local/axis2c
Axis2LogFile /var/log/axis2c/axis2.log
Axis2LogLevel info
Axis2MaxLogFileSize 1024
<Location /services>
    SetHandler axis2_module
</Location>
<LocationMatch "^/services/.*">
    Header always set Access-Control-Allow-Origin "*"
    Header always set Access-Control-Allow-Methods "GET, POST, PUT, DELETE, OPTIONS"
    Header always set Access-Control-Allow-Headers "Content-Type, Authorization, Accept"
</LocationMatch>
CONFEOF
            echo "✅ Created $AXIS2_CONF"
        fi

        # Include in main httpd.conf if not already present
        if [ -f /usr/local/apache2/conf/httpd.conf ] && \
           ! grep -q "Include conf/extra/httpd-axis2.conf" /usr/local/apache2/conf/httpd.conf; then
            { echo ""; echo "# Include Axis2/C configuration"; echo "Include conf/extra/httpd-axis2.conf"; } \
                | sudo tee -a /usr/local/apache2/conf/httpd.conf > /dev/null
            echo "✅ Added Include directive to httpd.conf"
        fi

        sudo mkdir -p /var/log/axis2c && sudo chmod 755 /var/log/axis2c

        echo ""
        echo "🔄 Restart Apache to apply changes:"
        echo "   sudo /usr/local/apache2/bin/httpd -k restart"
    else
        echo ""
        echo "ℹ️  Packaged Apache detected — skipping module/config install."
        echo "   Use deploy-localhost.sh + axis2-services.conf for Debian/Ubuntu."
    fi

    echo ""
    echo "✅ BigDataH2 JSON Service ready"
else
    echo ""
    echo "❌ BUILD FAILED"
    echo "This indicates potential missing dependencies or compilation issues"
    exit 1
fi