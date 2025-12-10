# Apache Axis2/C TestwsService Makefile - Pure JSON HTTP/2 XSS Protection Demo

lib_LTLIBRARIES = libtestws_service.la

# HTTP/2 JSON TestWS Service library
libtestws_service_la_SOURCES = \
    src/testws_service.c \
    src/testws_service_handler.c

libtestws_service_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/src/core/transport/http/util \
    $(JSON_CFLAGS) \
    -DAXIS2_JSON_ENABLED \
    -DWITH_NGHTTP2

libtestws_service_la_LDFLAGS = -module -avoid-version
libtestws_service_la_LIBADD = \
    $(top_builddir)/util/src/libaxutil.la \
    $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
    $(JSON_LIBS)

# Note: Use curl for HTTP/2 XSS protection testing instead of custom client
# Example: curl --http2-prior-knowledge -H "Content-Type: application/json" \
#               -d '{"messagein":"<script>alert(Hello)</script>"}' \
#               http://localhost:8080/services/TestwsService

# Install service files
servicedir = $(libdir)/../services/TestwsService
service_DATA = services.xml

# Install configuration files
confdir = $(datadir)/axis2c/samples/user_guide/testws-service
conf_DATA = README.md

EXTRA_DIST = services.xml README.md

# Clean generated files
CLEANFILES = *.la *.lo *.o

distclean-local:
	rm -rf .libs