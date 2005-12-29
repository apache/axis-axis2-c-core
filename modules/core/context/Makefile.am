lib_LTLIBRARIES = libaxis2_context.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_context_la_SOURCES = ctx.c \
                                msg_ctx.c \
                                op_ctx.c \
                                svc_ctx.c \
                                svc_grp_ctx.c \
                                conf_ctx.c

libaxis2_context_la_LIBADD = 
INCLUDES = -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms
