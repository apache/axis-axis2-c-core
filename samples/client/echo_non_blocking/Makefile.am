prgbindir=$(prefix)/bin/samples

prgbin_PROGRAMS = echo_non_blocking

samplesdir=$(prefix)/samples/client/echo_non_blocking

samples_DATA=echo_client.c
echo_non_blocking_SOURCES = echo_client.c

echo_non_blocking_LDADD   =  \
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
            -I$(AXIS2C_HOME)/modules/platforms
