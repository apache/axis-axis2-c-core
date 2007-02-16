SUBDIRS = tokens openssl c14n
lib_LTLIBRARIES = libomxmlsec.la

libomxmlsec_la_SOURCES = ctx.c  buffer.c key.c cipher.c error.c axis2_utils.c axiom.c \
                         iv.c xml_encryption.c encryption.c\
						utility.c asym_ctx.c x509_cert.c key_mgr.c sign_part.c sign_ctx.c \
						xml_signature.c signature.c
						

libomxmlsec_la_LIBADD  = -lssl\
			../../../util/src/libaxis2_util.la \
			../../../axiom/src/om/libaxis2_axiom.la \
			c14n/liboxsc14n.la \
			-lcrypto 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../include \
			-I ../../../util/include \
			-I ../../../axiom/include 
