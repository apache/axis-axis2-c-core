prglibdir=$(prefix)/samples/lib/mtom_caching_callback
prglib_LTLIBRARIES = libmtom_caching_callback.la 
libmtom_caching_callback_la_SOURCES = mtom_caching_callback.c 
libmtom_caching_callback_la_LIBADD  = $(LDFLAGS) \
						   		-L$(AXIS2C_HOME)/lib \
								-laxutil

INCLUDES = -I ../../util/include \
			-I ../../axiom/include \
		@AXIS2INC@
