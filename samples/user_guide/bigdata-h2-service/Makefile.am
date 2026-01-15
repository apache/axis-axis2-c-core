# Apache Axis2/C BigDataH2Service Makefile - Pure JSON HTTP/2 Service

lib_LTLIBRARIES = libbigdata_h2_service.la

# HTTP/2 JSON Service library
# Note: bigdata_h2_bridge.c is excluded because it uses the SOAP/AXIOM skeleton API
# which is disabled in HTTP/2 JSON mode. Services use the JSON-direct pattern instead
# (bigdata_h2_service_invoke_json entry point).
libbigdata_h2_service_la_SOURCES = \
    src/bigdata_h2_service.c \
    src/bigdata_h2_service_handler.c

libbigdata_h2_service_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/axiom/include \
    -I$(top_srcdir)/src/core/transport/http/util \
    $(JSON_CFLAGS) \
    -DAXIS2_JSON_ENABLED \
    -DWITH_NGHTTP2

libbigdata_h2_service_la_LDFLAGS = -module -avoid-version
libbigdata_h2_service_la_LIBADD = \
    $(top_builddir)/util/src/libaxutil.la \
    $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
    $(JSON_LIBS) \
    -lm

# Note: Use curl for HTTP/2 JSON testing instead of custom client
# Example: curl --http2-prior-knowledge -H "Content-Type: application/json" \
#               -d @test_payload.json http://localhost:8080/services/BigDataH2Service

# Install service files
servicedir = $(libdir)/../services/BigDataH2Service
service_DATA = services.xml

# Install configuration files
confdir = $(datadir)/axis2c/samples/user_guide/bigdata-h2-service
conf_DATA = README.md

EXTRA_DIST = services.xml README.md

# Clean generated files
CLEANFILES = *.la *.lo *.o

distclean-local:
	rm -rf .libs