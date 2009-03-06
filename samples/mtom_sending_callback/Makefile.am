prglibdir=$(prefix)/samples/lib/mtom_sending_callback
prglib_LTLIBRARIES = libmtom_sending_callback.la 
libmtom_sending_callback_la_SOURCES = mtom_sending_callback.c 
libmtom_sending_callback_la_LIBADD  = $(LDFLAGS) \
						   		-L$(AXIS2C_HOME)/lib \
								-laxutil 

INCLUDES = -I ../../util/include \
			-I ../../axiom/include \
			-I /usr/include/mysql \
		@AXIS2INC@
