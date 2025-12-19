# Apache Axis2/C CameraControlService Makefile - Pure JSON HTTP/2 Service

lib_LTLIBRARIES = libcamera_control_service.la

# HTTP/2 JSON Camera Control Service library
libcamera_control_service_la_SOURCES = \
    src/camera_control_service.c

libcamera_control_service_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/src/core/transport/http/util \
    $(JSON_CFLAGS) \
    -DAXIS2_JSON_ENABLED \
    -DWITH_NGHTTP2

libcamera_control_service_la_LDFLAGS = -module -avoid-version
libcamera_control_service_la_LIBADD = \
    $(top_builddir)/util/src/libaxutil.la \
    $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
    $(JSON_LIBS)

# Note: Use curl for HTTP/2 camera control testing
# Examples:
# Start recording:
#   curl -k --http2 -H "Content-Type: application/json" \
#        -d '{"action":"start_recording","clip_name":"test_001","quality":"1080p","duration":60}' \
#        https://localhost:443/services/CameraControlService/startRecording
#
# Stop recording:
#   curl -k --http2 -H "Content-Type: application/json" \
#        -d '{"action":"stop_recording"}' \
#        https://localhost:443/services/CameraControlService/stopRecording
#
# Get status:
#   curl -k --http2 -H "Content-Type: application/json" \
#        -d '{"action":"get_status"}' \
#        https://localhost:443/services/CameraControlService/getStatus
#
# Configure settings:
#   curl -k --http2 -H "Content-Type: application/json" \
#        -d '{"action":"configure_settings","resolution":"1920x1080","fps":"30","codec":"h264"}' \
#        https://localhost:443/services/CameraControlService/configureSettings

# Install service files
servicedir = $(libdir)/../services/CameraControlService
service_DATA = services.xml

# Install documentation and implementation guide
confdir = $(datadir)/axis2c/samples/user_guide/camera-control-service
conf_DATA = README.md IMPLEMENTATION_GUIDE.md

EXTRA_DIST = services.xml README.md IMPLEMENTATION_GUIDE.md build_camera_service.sh

# Clean generated files
CLEANFILES = *.la *.lo *.o

distclean-local:
	rm -rf .libs