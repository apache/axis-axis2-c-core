# Apache Axis2/C LoginService Makefile - Pure JSON HTTP/2 Service

lib_LTLIBRARIES = liblogin_service.la

# HTTP/2 JSON Login Service library
liblogin_service_la_SOURCES = \
    src/login_service.c \
    src/login_json_handler.c

liblogin_service_la_CPPFLAGS = \
    @AXIS2INC@ \
    $(JSON_CFLAGS) \
    -DAXIS2_JSON_ENABLED \
    -DWITH_NGHTTP2

liblogin_service_la_LDFLAGS = -module -avoid-version
liblogin_service_la_LIBADD = \
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