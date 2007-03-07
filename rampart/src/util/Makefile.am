TESTS =

noinst_LTLIBRARIES = librampart_util.la

librampart_util_la_SOURCES = rampart_action.c rampart_crypto_util.c \
							rampart_util.c rampart_handler_util.c username_token.c  timestamp_token.c \
							rampart_encryption.c rampart_sec_header_processor.c rampart_sec_processed_result.c \
							rampart_sec_header_builder.c rampart_context.c rampart_engine.c rampart_token_processor.c rampart_signature.c


librampart_util_la_LIBADD  = -lssl \
						-lcrypto \
						../../../util/src/libaxis2_util.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../include \
			-I ../../../util/include \
			-I ../../../xml_schema/include \
			-I ../../../axiom/include
