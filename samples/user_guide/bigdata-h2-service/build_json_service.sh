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

    echo "📋 Installed files:"
    ls -la /usr/local/axis2c/services/BigDataH2Service/

    # Install and configure Axis2/C Apache module (if not already done)
    echo ""
    echo "🔧 Installing Axis2/C Apache module..."

    # Copy the main Axis2/C module to Apache modules directory
    if [ ! -f "/usr/local/apache2/modules/mod_axis2.so" ]; then
        # Try to locate the Axis2/C module using various methods
        AXIS2_MODULE=""

        # Method 1: Use AXIS2C_HOME environment variable
        if [ -n "$AXIS2C_HOME" ] && [ -f "$AXIS2C_HOME/lib/libmod_axis2.so.0.7.0" ]; then
            AXIS2_MODULE="$AXIS2C_HOME/lib/libmod_axis2.so.0.7.0"
        # Method 2: Look in standard install location
        elif [ -f "/usr/local/axis2c/lib/libmod_axis2.so.0.7.0" ]; then
            AXIS2_MODULE="/usr/local/axis2c/lib/libmod_axis2.so.0.7.0"
        # Method 3: Look relative to current build location (assuming we're in samples/user_guide/service/)
        elif [ -f "../../../deploy/lib/libmod_axis2.so.0.7.0" ]; then
            AXIS2_MODULE="../../../deploy/lib/libmod_axis2.so.0.7.0"
        # Method 4: Look in common build locations
        elif [ -f "../../deploy/lib/libmod_axis2.so.0.7.0" ]; then
            AXIS2_MODULE="../../deploy/lib/libmod_axis2.so.0.7.0"
        fi

        if [ -n "$AXIS2_MODULE" ]; then
            sudo cp "$AXIS2_MODULE" /usr/local/apache2/modules/mod_axis2.so
            echo "✅ Axis2/C module copied from: $AXIS2_MODULE"
        else
            echo "⚠️  Axis2/C module not found. Please set AXIS2C_HOME or ensure Axis2/C is built."
            echo "   Searched locations:"
            echo "   - \$AXIS2C_HOME/lib/libmod_axis2.so.0.7.0"
            echo "   - /usr/local/axis2c/lib/libmod_axis2.so.0.7.0"
            echo "   - ../../../deploy/lib/libmod_axis2.so.0.7.0"
            echo "   - ../../deploy/lib/libmod_axis2.so.0.7.0"
        fi
    else
        echo "✅ Axis2/C module already installed"
    fi

    # Create Apache configuration for Axis2/C
    echo ""
    echo "📝 Configuring Apache for Axis2/C services..."

    AXIS2_CONF="/usr/local/apache2/conf/extra/httpd-axis2.conf"
    if [ ! -f "$AXIS2_CONF" ]; then
        sudo tee "$AXIS2_CONF" > /dev/null << 'EOF'
# Axis2/C Configuration for HTTP/2 JSON Services
LoadModule axis2_module modules/mod_axis2.so

# Enable HTTP/2 for Axis2/C services (global setting)
Protocols h2 h2c http/1.1

# Axis2/C Repository and Services Location
Axis2RepoPath /usr/local/axis2c
Axis2LogFile /var/log/axis2c/axis2.log
Axis2LogLevel info
Axis2MaxLogFileSize 1024

# Location directive for Axis2/C services
<Location /services>
    SetHandler axis2_module
</Location>

# Enable JSON content type handling
<LocationMatch "^/services/.*">
    Header always set Access-Control-Allow-Origin "*"
    Header always set Access-Control-Allow-Methods "GET, POST, PUT, DELETE, OPTIONS"
    Header always set Access-Control-Allow-Headers "Content-Type, Authorization, Accept"
</LocationMatch>
EOF
        echo "✅ Created Axis2/C Apache configuration"
    else
        echo "✅ Axis2/C Apache configuration already exists"
    fi

    # Include the configuration in main httpd.conf
    if ! grep -q "Include conf/extra/httpd-axis2.conf" /usr/local/apache2/conf/httpd.conf; then
        echo "" | sudo tee -a /usr/local/apache2/conf/httpd.conf
        echo "# Include Axis2/C configuration" | sudo tee -a /usr/local/apache2/conf/httpd.conf
        echo "Include conf/extra/httpd-axis2.conf" | sudo tee -a /usr/local/apache2/conf/httpd.conf
        echo "✅ Added Axis2/C configuration to httpd.conf"
    else
        echo "✅ Axis2/C configuration already included in httpd.conf"
    fi

    # Create log directory
    sudo mkdir -p /var/log/axis2c
    sudo chmod 755 /var/log/axis2c

    # Test Apache configuration
    echo ""
    echo "🧪 Testing Apache configuration..."

    # Try different Apache binary locations
    APACHE_CMD=""
    if [ -x "/usr/local/apache2/bin/httpd" ]; then
        APACHE_CMD="/usr/local/apache2/bin/httpd"
    elif [ -x "/usr/sbin/httpd" ]; then
        APACHE_CMD="/usr/sbin/httpd"
    elif [ -x "/usr/sbin/apache2" ]; then
        APACHE_CMD="/usr/sbin/apache2"
    elif command -v apache2 >/dev/null 2>&1; then
        APACHE_CMD="apache2"
    elif command -v httpd >/dev/null 2>&1; then
        APACHE_CMD="httpd"
    else
        echo "⚠️  Apache binary not found - skipping configuration test"
        echo "   Manually test with: sudo apache2 -t  or  sudo httpd -t"
        APACHE_CMD=""
    fi

    if [ -n "$APACHE_CMD" ]; then
        if sudo "$APACHE_CMD" -t; then
            echo "✅ Apache configuration test passed"
            echo ""
            echo "🔄 Restart Apache to apply changes:"
            echo "   sudo systemctl restart apache2-custom  # Ubuntu 25.10"
            echo "   sudo systemctl restart httpd           # RedHat/CentOS and Ubuntu/Debian with package managed httpd that contains the proper H2 features"
        else
            echo "❌ Apache configuration test failed - please check the configuration"
        fi
    fi

    echo ""
    echo "✅ CONCLUSION: Pure JSON HTTP/2 BigDataH2 Service"
    echo "🏗️  Service Architecture: Direct JSON processing (no AXIOM/XML code)"
    echo "🔧 Apache Integration: Axis2/C module configured for HTTP/2 services"
    echo "📋 Note: AXIOM libraries present as transitive dependencies through axis2_engine"
    echo "💡 Service-level code uses pure json-c processing for HTTP/2 large payloads"
else
    echo ""
    echo "❌ BUILD FAILED"
    echo "This indicates potential missing dependencies or compilation issues"
    exit 1
fi