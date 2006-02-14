prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = addr_echo
addr_echo_SOURCES = echo_client.c

addr_echo_LDADD   =  -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_unix \
                    -laxis2_soap \
                    -laxis2_http_transport \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include



