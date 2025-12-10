#!/bin/bash
set -e

# Build BigDataH2Service with Working Simple JSON Message Receiver
# Using existing axis2_json_rpc_msg_recv_simple.c (no complex native receiver)

echo "🚀 Building BigDataH2 Simple JSON Service - Fixed Version"
echo "Using existing simple JSON message receiver (avoiding compilation errors)"
echo "📁 Creating services directory..."

# Ensure AXIS2C_HOME is set
if [ -z "$AXIS2C_HOME" ]; then
    export AXIS2C_HOME="/usr/local/axis2c"
    echo "📍 Set AXIS2C_HOME to $AXIS2C_HOME"
fi

# Create services directory structure
sudo mkdir -p /usr/local/axis2c/services/BigDataH2Service

echo "🔨 Building with Simple JSON Message Receiver..."
echo "✅ INCLUDING: Existing axis2_json_rpc_msg_recv_simple.c (working version)"
echo "✅ INCLUDING: enableJSONOnly parameter support"
echo "✅ INCLUDING: JSON content-type detection"
echo "✅ INCLUDING: Pure JSON service functions"
echo "❌ EXCLUDED: Complex native receiver (compilation errors)"
echo "❌ EXCLUDED: Direct stream reading (API issues)"

# Compile with working Simple JSON Message Receiver (not the native one)
gcc -shared -fPIC \
    -I../../../include \
    -I../../../util/include \
    -I../../../axiom/include \
    -I/usr/include/json-c \
    -DAXIS2_GUTHTHILA_ENABLED \
    -DAXIS2_SVR_MULTI_THREADED \
    -DAXIS2_LIBCURL_ENABLED \
    -DAXIS2_JSON_ENABLED \
    -DWITH_NGHTTP2 \
    -DENABLE_JSON_ONLY \
    src/bigdata_h2_service.c \
    src/bigdata_h2_service_handler.c \
    src/axis2_json_rpc_msg_recv_simple.c \
    -L../../../deploy/lib \
    -laxutil -laxis2_engine -laxis2_axiom -ljson-c -lm \
    -o liblibbigdata_h2_service.so

if [ $? -eq 0 ]; then
    echo "✅ Simple JSON Service compilation successful!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

echo "📦 Installing Simple JSON service..."

# Copy shared library
sudo cp liblibbigdata_h2_service.so /usr/local/axis2c/services/BigDataH2Service/

# Create services.xml with Simple JSON Message Receiver
echo "📝 Creating services.xml with Simple JSON Message Receiver..."
sudo tee /usr/local/axis2c/services/BigDataH2Service/services.xml > /dev/null << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<serviceGroup>
    <service name="BigDataH2Service">
        <description>HTTP/2 Simple JSON Service - Working Version</description>
        <serviceClass>bigdata_h2_service</serviceClass>
        <parameter name="ServiceClass">liblibbigdata_h2_service</parameter>

        <!-- enableJSONOnly parameter -->
        <parameter name="enableJSONOnly">true</parameter>

        <!-- Use Simple JSON Message Receiver (working version) -->
        <messageReceiver mep="http://www.w3.org/2004/08/wsdl/in-out"
                        class="axis2_json_rpc_msg_recv_simple" />

        <operation name="processBigDataSet">
            <description>Process big data set with simple JSON processing</description>
        </operation>

        <operation name="getServiceMetadata">
            <description>Get service metadata with simple JSON processing</description>
        </operation>
    </service>
</serviceGroup>
EOF

echo "🔄 Updating Apache module..."
# Try different Apache binary locations
if [ -x "/usr/local/apache2/bin/httpd" ]; then
    APACHE_CMD="/usr/local/apache2/bin/httpd"
elif command -v apache2 >/dev/null 2>&1; then
    APACHE_CMD="apache2"
else
    echo "⚠️ Apache binary not found, skipping module update"
    APACHE_CMD=""
fi

if [ -n "$APACHE_CMD" ]; then
    # Copy Axis2/C module to Apache
    if [ -f "../../../deploy/lib/libmod_axis2.so.0.7.0" ]; then
        sudo cp ../../../deploy/lib/libmod_axis2.so.0.7.0 /usr/local/apache2/modules/mod_axis2.so
        echo "✅ Apache module updated"
    else
        echo "⚠️ Axis2/C module not found in deploy/lib"
    fi

    # Test Apache configuration
    echo "🔍 Testing Apache configuration..."
    if sudo $APACHE_CMD -t; then
        echo "✅ Apache configuration test passed"

        # Restart Apache if running
        if sudo systemctl is-active --quiet apache2-custom; then
            echo "🔄 Restarting Apache..."
            sudo systemctl restart apache2-custom
            echo "✅ Apache restarted successfully"
        else
            echo "ℹ️ Apache not running, skipping restart"
        fi
    else
        echo "❌ Apache configuration test failed"
        exit 1
    fi
else
    echo "⚠️ Skipping Apache integration - Apache binary not found"
fi

echo ""
echo "🎉 BigDataH2 Simple JSON Service Build Complete!"
echo ""
echo "📋 Service Architecture (Simple Working Pattern):"
echo "   • Message Receiver: axis2_json_rpc_msg_recv_simple (working version)"
echo "   • JSON Detection: enableJSONOnly + Content-Type checking"
echo "   • JSON Processing: Direct service function calls"
echo "   • Response: SOAP envelope with JSON content (hybrid approach)"
echo ""
echo "🔗 Service Endpoints:"
echo "   • POST https://localhost/axis2/services/BigDataH2Service/processBigDataSet"
echo "   • POST https://localhost/axis2/services/BigDataH2Service/getServiceMetadata"
echo ""
echo "🧪 Test with:"
echo "   curl -k --http2 -X POST https://localhost/axis2/services/BigDataH2Service/getServiceMetadata \\"
echo "        -H 'Content-Type: application/json' \\"
echo "        -d '{}'"
echo ""
echo "⚠️ Note: This version uses a hybrid approach - JSON processing with SOAP envelope response"
echo "   This should eliminate the 'Array list index out of bounds' error while providing JSON processing"