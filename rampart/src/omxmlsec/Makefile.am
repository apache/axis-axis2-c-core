SUBDIRS = openssl
noinst_LTLIBRARIES = libomxmlsec.la
#libomxmlsec_la_SOURCES = ctx.c enc.c transforms.c  buffer.c errors.c
libomxmlsec_la_SOURCES = buffer.c errors.c axis2_utils.c

libomxmlsec_la_LIBADD  = -lssl\
            -laxis2_util \
			-lcrypto \
            -laxis2_axiom

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
            @AXIOMINC@
