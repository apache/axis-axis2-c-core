lib_LTLIBRARIES = libaxis2_description.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_description_la_SOURCES = param_container.c \
                            op.c \
                            svc.c \
                            module.c \
                            module_desc.c \
                            param.c \
                            svc_grp.c \
                            phase_rule.c \
                            handler_desc.c \
                            flow.c \
                            flow_container.c \
                            transport_in_desc.c \
                            transport_out_desc.c

libaxis2_description_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/wsdl/src \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/addr
