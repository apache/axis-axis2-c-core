SUBDIRS = openssl
noinst_LTLIBRARIES = libomxmlsec.la
libomxmlsec_la_SOURCES = oxs_ctx.c oxs_enc.c 

libomxmlsec_la_LIBADD  = -lssl\
            -laxis2_util \
			-lcrypto \
            -laxis2_axiom

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
            @AXIOMINC@
