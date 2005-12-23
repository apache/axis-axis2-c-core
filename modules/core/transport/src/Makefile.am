lib_LTLIBRARIES = libaxis2_transport.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_transport_la_SOURCES = transport_listener.c \
                            transport_sender.c

libaxis2_transport_la_LIBADD = 
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/core/transport
