prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = echo_blocking
samplesdir=$(prefix)/samples/user_guide/clients
samples_DATA=echo_blocking.c Makefile.am Makefile.in
echo_blocking_SOURCES = echo_blocking.c
echo_blocking_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_unix \
                    -lpthread \
                    -laxis2_soap \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
            -I$(AXIS2C_HOME)/platforms
