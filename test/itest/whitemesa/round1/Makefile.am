prgbindir=$(prefix)/bin/itest
prgbin_PROGRAMS = round1 
round1_SOURCES = round1_client.c

round1_LDADD   =    $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_unix \
                    -laxis2_soap \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser


