noinst_LTLIBRARIES = libomopenssl.la
libomopenssl_la_SOURCES = cipher_ctx.c crypt.c rsa.c 

libomopenssl_la_LIBADD  = -lssl\
            -laxis2_util \
    		-lcrypto \
            -laxis2_axiom

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
            @AXIOMINC@
