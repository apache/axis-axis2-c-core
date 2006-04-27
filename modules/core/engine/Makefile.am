TESTS =
noinst_LTLIBRARIES = libaxis2_int_engine.la
noinst_HEADERS = axis2_engine.h \
                    axis2_event.h

libaxis2_int_engine_la_SOURCES = handler.c \
                                conf.c \
                                phase.c \
                                disp_checker.c \
                                event.c \
                                addr_disp.c \
                                req_uri_disp.c \
                                disp.c \
                                soap_action_disp.c \
                                soap_body_disp.c \
                                ctx_handler.c \
                                engine.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/util \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
