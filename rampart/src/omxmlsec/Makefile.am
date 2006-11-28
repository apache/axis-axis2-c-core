SUBDIRS = openssl 
noinst_LTLIBRARIES = libomxmlsec.la

libomxmlsec_la_SOURCES = ctx.c  buffer.c key.c cipher.c error.c axis2_utils.c axiom.c \
                        token_encrypted_data.c token_encryption_method.c token_cipher_value.c \
                        token_cipher_data.c token_key_name.c token_key_info.c token_binary_security_token.c \
                        token_reference_list.c token_data_reference.c token_encrypted_key.c iv.c xml_encryption.c encryption.c\
						utility.c asym_ctx.c x509_cert.c key_mgr.c token_key_identifier.c token_x509_data.c token_x509_issuer_serial.c\
						token_x509_issuer_name.c token_x509_serial_number.c

libomxmlsec_la_LIBADD  = -lssl\
			../../../util/src/libaxis2_util.la \
			../../../axiom/src/om/libaxis2_axiom.la \
			-lcrypto 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../include \
			-I ../../../util/include \
			-I ../../../axiom/include 
