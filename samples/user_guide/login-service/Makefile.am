# Apache Axis2/C LoginService Makefile - Pure JSON HTTP/2 Service

lib_LTLIBRARIES = liblogin_service.la

# HTTP/2 JSON Login Service library
liblogin_service_la_SOURCES = \
    src/login_service.c \
    src/login_service_handler.c

liblogin_service_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/src/core/transport/http/util \
    $(JSON_CFLAGS) \
    -DAXIS2_JSON_ENABLED \
    -DWITH_NGHTTP2

liblogin_service_la_LDFLAGS = -module -avoid-version
liblogin_service_la_LIBADD = \
    $(top_builddir)/util/src/libaxutil.la \
    $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
    $(JSON_LIBS) \
    -lssl -lcrypto

# Note: Use curl for HTTP/2 authentication testing instead of custom client
# Example: curl --http2-prior-knowledge -H "Content-Type: application/json" \
#               -d '{"username":"admin","password":"secret"}' \
#               http://localhost:8080/services/LoginService

# Install service files
servicedir = $(libdir)/../services/LoginService
service_DATA = services.xml

# Install configuration files
confdir = $(datadir)/axis2c/samples/user_guide/login-service
conf_DATA = README.md

EXTRA_DIST = services.xml README.md

# Clean generated files
CLEANFILES = *.la *.lo *.o

distclean-local:
	rm -rf .libs