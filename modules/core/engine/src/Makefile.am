lib_LTLIBRARIES = libaxis2_engine.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_engine_la_SOURCES = engine_configuration.c \
                                message_receiver.c

libaxis2_engine_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src
