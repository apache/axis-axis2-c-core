TESTS =

noinst_LTLIBRARIES = librampart_util.la

librampart_util_la_SOURCES = rampart_action.c rampart_crypto_engine.c rampart_crypto_util.c rampart_util.c rampart_handler_util.c username_token.c  timestamp_token.c 

librampart_util_la_LIBADD  = -lssl\
			-lcrypto 

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@
