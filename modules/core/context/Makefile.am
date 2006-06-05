noinst_LTLIBRARIES = libaxis2_context.la

libaxis2_context_la_SOURCES = \
                            ctx.c \
                            msg_ctx.c \
                            op_ctx.c \
                            svc_ctx.c \
                            svc_grp_ctx.c \
                            conf_ctx.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/engine \
			@AXIOMINC@ \
			@UTILINC@ 
