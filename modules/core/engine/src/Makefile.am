lib_LTLIBRARIES = libaxis2_engine.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_engine_la_SOURCES = handler.c msg_recv.c engine_config.c phase.c disp_checker.c event.c \
                             addr_disp.c req_uri_disp.c soap_action_disp.c soap_body_disp.c

libaxis2_engine_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/wsdl/src \
            -I$(top_builddir)/modules/core/transport
