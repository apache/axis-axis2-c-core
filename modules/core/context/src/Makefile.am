lib_LTLIBRARIES = libaxis2_context.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_context_la_SOURCES = ctx.c msg_ctx.c operation_ctx.c svc_ctx.c svc_grp_ctx.c conf_ctx.c

libaxis2_context_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/wsdl/src \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/core/transport
