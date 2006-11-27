noinst_LTLIBRARIES = libomopenssl.la
libomopenssl_la_SOURCES = cipher_ctx.c crypt.c rsa.c pkey.c util.c cipher_property.c digest.c x509.c pkcs12.c

libomopenssl_la_LIBADD  = -lssl\
			../../../../util/src/libaxis2_util.la \
			../../../../axiom/src/om/libaxis2_axiom.la \
    		-lcrypto

INCLUDES = -I$(top_builddir)/include \
			-I ../../../../util/include \
			-I ../../../../include \
			-I ../../../../axiom/include
