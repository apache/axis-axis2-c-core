prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_http_transport
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
test_http_transport_SOURCES = test_http_transport.c

test_http_transport_LDADD   =  $(LDFLAGS) -L$(top_builddir)/install/lib -laxis2_util -laxis2_http_transport -laxis2_context -laxis2_addr
                    
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util\
            -I$(top_builddir)/modules/platforms
