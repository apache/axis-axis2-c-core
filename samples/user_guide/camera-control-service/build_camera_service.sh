#!/bin/bash
set -e

echo "🚀 Building Camera Control JSON Service - SOAP-FREE HTTP/2 Architecture"

# Create services directory
echo "📁 Creating services directory..."
sudo mkdir -p /usr/local/axis2c/services/CameraControlService

echo "🔨 Building with PURE JSON dependencies (AXIOM-FREE + SOAP-FREE)..."
echo "❌ EXCLUDED: -laxis2_axiom (XML object model not needed for pure JSON)"
echo "❌ EXCLUDED: SOAP service skeleton framework patterns"
echo "✅ INCLUDING: -laxutil -laxis2_engine -ljson-c"
echo ""
echo "⚠️  STUB IMPLEMENTATION WARNING:"
echo "   This service contains STUB implementations only!"
echo "   Users must implement camera_device_*_impl() functions"
echo "   for their specific camera system (OpenCamera, V4L2, IP cameras, etc.)"
echo ""

gcc -shared -fPIC \
    -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2 \
    -I/usr/local/axis2c/include/axis2-2.0.0 \
    -I../../include \
    -I../../util/include \
    -I../../src/core/transport/http/util \
    -I/usr/include/json-c \
    src/camera_control_service.c \
    -o libcamera_control_service.so \
    -L/usr/local/axis2c/lib \
    -laxutil -laxis2_engine \
    -ljson-c

echo "Build exit code: $?"

if [ -f "libcamera_control_service.so" ]; then
    echo ""
    echo "🎉 SUCCESS: Camera Control JSON Service built with SOAP-FREE architecture!"
    echo "⚠️  REMINDER: This service contains STUB implementations only!"

    # Show file info
    ls -la libcamera_control_service.so
    file libcamera_control_service.so

    # Check dependencies
    echo ""
    echo "🔍 Checking runtime dependencies..."
    echo "📋 Service-level: No direct AXIOM usage (pure JSON processing)"
    echo "🔧 Engine-level: AXIOM present as transitive dependency (expected in Axis2/C)"
    # Show axis2 related dependencies only
    ldd libcamera_control_service.so | grep -E '(axis2|json|libax)' || echo "   (Dependencies shown above in full ldd output)"

    # Install the service
    echo ""
    echo "🚀 Installing SOAP-free JSON Camera Control service..."
    sudo cp libcamera_control_service.so /usr/local/axis2c/services/CameraControlService/libcamera_control_service.so
    sudo cp services.xml /usr/local/axis2c/services/CameraControlService/

    echo ""
    echo "✅ Installation complete! Camera Control Service ready for HTTP/2 JSON requests"
    echo ""
    echo "🧪 TEST THE SERVICE (STUB IMPLEMENTATION):"
    echo ""
    echo "1. Start Apache httpd with mod_axis2:"
    echo "   sudo systemctl start apache2  # or httpd"
    echo ""
    echo "2. Test start recording (STUB - will log but not record):"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '\''{"action":"start_recording","clip_name":"test_001","quality":"1080p","duration":60}'\'' \'
    echo '        https://localhost:443/services/CameraControlService/startRecording'
    echo ""
    echo "3. Test get status (STUB - returns demo data):"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '\''{"action":"get_status"}'\'' \'
    echo '        https://localhost:443/services/CameraControlService/getStatus'
    echo ""
    echo "4. Test stop recording (STUB - will log but not stop recording):"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '\''{"action":"stop_recording"}'\'' \'
    echo '        https://localhost:443/services/CameraControlService/stopRecording'
    echo ""
    echo "5. Test SFTP transfer (STUB - will log but not transfer):"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '\''{"action":"sftp_transfer","hostname":"backup.example.com","username":"camera_user","private_key_path":"/home/camera/.ssh/id_rsa","local_file_path":"/tmp/test.mp4","remote_path":"/backups/"}'\'' \'
    echo '        https://localhost:443/services/CameraControlService/sftpTransfer'
    echo ""
    echo "📖 IMPLEMENTATION GUIDE:"
    echo "   See IMPLEMENTATION_GUIDE.md for examples of replacing stub functions"
    echo "   with camera-specific code (OpenCamera JNI, V4L2, IP cameras, etc.)"
    echo ""
    echo "🔍 CHECK LOGS:"
    echo "   tail -f /usr/local/axis2c/logs/axis2.log"
    echo "   (Look for STUB warnings and camera operation logs)"

else
    echo ""
    echo "❌ Build failed - check compiler errors above"
    echo ""
    echo "💡 Common solutions:"
    echo "1. Install json-c development files: sudo apt install libjson-c-dev"
    echo "2. Check Apache Axis2/C installation: ls -la /usr/local/axis2c/lib"
    echo "3. Verify include paths exist: ls -la /usr/local/axis2c/include/axis2-2.0.0"
    exit 1
fi