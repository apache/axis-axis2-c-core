prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = echo
samplesdir=$(prefix)/samples/client/echo
samples_DATA=echo.c README.txt Makefile.am Makefile.in
echo_SOURCES = echo.c

echo_LDADD   = $(LDFLAGS) \
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
