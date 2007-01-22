TESTS =

noinst_LTLIBRARIES = librp_model.la

librp_model_la_SOURCES = algorithmsuite.c asymmetric_binding.c \
							binding_commons.c encryption_crypto.c header.c https_token.c layout.c \
							property.c rampart_config.c secpolicy.c security_context_token.c signature_crypto.c \
							signed_encrypted_elements.c signed_encrypted_parts.c supporting_tokens.c symmetric_asymmetric_binding_commons.c\							symmetric_binding.c transport_binding.c ut.c wss10.c wss11.c x509_token.c


librp_model_la_LIBADD  = ../../../../util/src/libaxis2_util.la \
						 ../../../../axiom/src/om/libaxis2_axiom.la


INCLUDES = -I$(top_builddir)/include \
			-I ../../../../include \
			-I ../../../../util/include \
			-I ../../../../axiom/include
