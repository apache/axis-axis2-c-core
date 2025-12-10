#!/bin/bash
set -e

# Build BigDataH2Service with Native JSON RPC Message Receiver
# Following Axis2/Java JsonRpcMessageReceiver pattern - NO SOAP PROCESSING

echo "🚀 Building BigDataH2 Native JSON RPC Service - Following Axis2/Java Pattern"
echo "Using JsonRpcMessageReceiver pattern: Pure JSON processing, SOAP fallback"
echo "📁 Creating services directory..."

# Ensure AXIS2C_HOME is set
if [ -z "$AXIS2C_HOME" ]; then
    export AXIS2C_HOME="/usr/local/axis2c"
    echo "📍 Set AXIS2C_HOME to $AXIS2C_HOME"
fi

# Create services directory structure
sudo mkdir -p /usr/local/axis2c/services/BigDataH2Service

echo "🔨 Building with Native JSON RPC Message Receiver..."
echo "✅ INCLUDING: Native JSON RPC Message Receiver (mimics Axis2/Java JsonRpcMessageReceiver)"
echo "✅ INCLUDING: IS_JSON_STREAM property checking"
echo "✅ INCLUDING: Pure JSON processing when IS_JSON_STREAM=true"
echo "✅ INCLUDING: SOAP fallback when IS_JSON_STREAM=false"
echo "✅ INCLUDING: enableJSONOnly parameter support"
echo "✅ INCLUDING: Direct JSON response writing (no SOAP envelope)"
echo "❌ EXCLUDED: axis2_raw_xml_in_out_msg_recv for JSON requests"

# Compile with Native JSON RPC Message Receiver
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
    src/axis2_json_rpc_msg_recv_native.c \
    -L../../../deploy/lib \
    -laxutil -laxis2_engine -laxis2_axiom -ljson-c -lm \
    -o liblibbigdata_h2_service.so

if [ $? -eq 0 ]; then
    echo "✅ Native JSON RPC Service compilation successful!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

echo "📦 Installing Native JSON RPC service..."

# Copy shared library
sudo cp liblibbigdata_h2_service.so /usr/local/axis2c/services/BigDataH2Service/

# Create services.xml with Native JSON RPC Message Receiver
echo "📝 Creating services.xml with Native JSON RPC Message Receiver..."
sudo tee /usr/local/axis2c/services/BigDataH2Service/services.xml > /dev/null << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<serviceGroup>
    <service name="BigDataH2Service">
        <description>HTTP/2 Native JSON RPC Service - Following Axis2/Java JsonRpcMessageReceiver Pattern</description>
        <serviceClass>bigdata_h2_service</serviceClass>
        <parameter name="ServiceClass">liblibbigdata_h2_service</parameter>

        <!-- enableJSONOnly parameter (Axis2/Java JsonRpcMessageReceiver pattern) -->
        <parameter name="enableJSONOnly">true</parameter>

        <!-- Use Native JSON RPC Message Receiver (NOT axis2_raw_xml_in_out_msg_recv) -->
        <messageReceiver mep="http://www.w3.org/2004/08/wsdl/in-out"
                        class="axis2_json_rpc_msg_recv_native" />

        <operation name="processBigDataSet">
            <description>Process big data set with Native JSON RPC (pure JSON when IS_JSON_STREAM=true)</description>
        </operation>

        <operation name="getServiceMetadata">
            <description>Get service metadata with Native JSON RPC (pure JSON when IS_JSON_STREAM=true)</description>
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
echo "🎉 BigDataH2 Native JSON RPC Service Build Complete!"
echo ""
echo "📋 Service Architecture (Following Axis2/Java JsonRpcMessageReceiver Pattern):"
echo "   • Request Processing:"
echo "     - Check IS_JSON_STREAM property"
echo "     - If true: Pure JSON processing (bypass SOAP entirely)"
echo "     - If false: Fall back to SOAP processing"
echo "   • Message Receiver: axis2_json_rpc_msg_recv_native (NOT axis2_raw_xml_in_out_msg_recv)"
echo "   • enableJSONOnly: true (for pure JSON services)"
echo "   • Response: Direct JSON writing (no SOAP envelope)"
echo ""
echo "🔗 Service Endpoints:"
echo "   • POST https://localhost/axis2/services/BigDataH2Service/processBigDataSet"
echo "   • POST https://localhost/axis2/services/BigDataH2Service/getServiceMetadata"
echo ""
echo "🧪 Test with:"
echo "   curl -k -X POST https://localhost/axis2/services/BigDataH2Service/getServiceMetadata \\"
echo "        -H 'Content-Type: application/json' \\"
echo "        -d '{}'"
echo ""
echo "📊 Architecture Flow (Pure JSON when IS_JSON_STREAM=true):"
echo "   JSON Request → Native JSON RPC Message Receiver → Check IS_JSON_STREAM"
echo "   → If true: Service Function (pure JSON) → Direct JSON Response (no SOAP envelope)"
echo "   → If false: axis2_raw_xml_in_out_msg_recv → SOAP processing"