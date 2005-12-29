lib_LTLIBRARIES = libaxis2_engine.la
AM_CPPFLAGS = $(CPPFLAGS)

libaxis2_engine_la_SOURCES = handler.c \
                                conf.c \
                                phase.c \
                                disp_checker.c \
                                event.c \
                                addr_disp.c \
                                req_uri_disp.c \
                                disp.c \
                                soap_action_disp.c \
                                soap_body_disp.c \
                                engine.c

libaxis2_engine_la_LIBADD = 
INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms
