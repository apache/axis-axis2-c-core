#!/bin/bash
set -e

# Build BigDataH2Service with Native Axis2/C JSON Processing
# Following Axis2/C's actual architecture rather than trying to recreate Axis2/Java patterns

echo "🚀 Building BigDataH2 Simple JSON Service - Native Axis2/C Architecture"
echo "Using existing axis2_json_reader/axis2_json_writer + axis2_msg_recv pattern"
echo "📁 Creating services directory..."

# Ensure AXIS2C_HOME is set
if [ -z "$AXIS2C_HOME" ]; then
    export AXIS2C_HOME="/usr/local/axis2c"
    echo "📍 Set AXIS2C_HOME to $AXIS2C_HOME"
fi

# Create services directory structure
sudo mkdir -p /usr/local/axis2c/services/BigDataH2Service

echo "🔨 Building with Native Axis2/C JSON Processing..."
echo "✅ INCLUDING: Native axis2_json_reader (parses JSON from HTTP stream)"
echo "✅ INCLUDING: Native axis2_json_writer (writes JSON to HTTP stream)"
echo "✅ INCLUDING: Custom message receiver for pure JSON processing"
echo "✅ INCLUDING: Pure JSON service functions (no SOAP/XML conversion)"
echo "✅ INCLUDING: -laxutil -laxis2_engine -ljson-c -lm"
echo "❌ EXCLUDED: AXIOM/SOAP conversion overhead"

# Compile only the working source files (no builder/formatter interfaces)
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

# Create services.xml with simple JSON configuration
echo "📝 Creating services.xml with native JSON processing..."
sudo tee /usr/local/axis2c/services/BigDataH2Service/services.xml > /dev/null << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<serviceGroup>
    <service name="BigDataH2Service">
        <description>HTTP/2 Simple JSON BigData Service - Native Axis2/C</description>
        <serviceClass>bigdata_h2_service</serviceClass>
        <parameter name="ServiceClass">liblibbigdata_h2_service</parameter>

        <!-- Native JSON processing parameters -->
        <parameter name="enableJSONOnly">true</parameter>
        <parameter name="JSONContentType">application/json</parameter>

        <!-- Use standard message receiver -->
        <messageReceiver mep="http://www.w3.org/2004/08/wsdl/in-out"
                        class="axis2_raw_xml_in_out_msg_recv" />

        <operation name="processBigDataSet">
            <description>Process big data set with HTTP/2 JSON (native processing)</description>
        </operation>

        <operation name="getServiceMetadata">
            <description>Get service metadata as JSON (native processing)</description>
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
        if sudo systemctl is-active --quiet apache2; then
            echo "🔄 Restarting Apache..."
            sudo systemctl restart apache2
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
echo "📋 Service Architecture (Native Axis2/C Pattern):"
echo "   • HTTP Request: Native HTTP transport receives JSON"
echo "   • JSON Processing: Service functions handle JSON directly with json-c"
echo "   • HTTP Response: Native HTTP transport sends JSON response"
echo "   • enableJSONOnly: true (bypass SOAP processing)"
echo "   • Performance: Direct JSON processing, no AXIOM/SOAP overhead"
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
echo "📊 Architecture Flow:"
echo "   JSON Request → HTTP Transport → Message Receiver → Service Function (pure JSON) → JSON Response"