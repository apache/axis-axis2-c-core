SUBDIRS = openssl
noinst_LTLIBRARIES = libomxmlsec.la

libomxmlsec_la_SOURCES = ctx.c  enc_engine.c buffer.c key.c error.c axis2_utils.c axiom.c \
                        token_encrypted_type.c token_encryption_method.c token_cipher_value.c \
                        token_cipher_data.c token_key_name.c token_key_info.c token_binary_security_token.c \
                        token_reference_list.c token_data_reference.c

libomxmlsec_la_LIBADD  = -lssl\
            -laxis2_util \
			-lcrypto \
            -laxis2_axiom

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
            @AXIOMINC@
