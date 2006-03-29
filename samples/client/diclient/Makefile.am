prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = echo_client
samplesdir=$(prefix)/samples/client/diclient
samples_DATA=echo_client.c di_client.h Makefile.am README
echo_client_SOURCES = echo_client.c
echo_client_LDADD   =  \
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
                    -laxis2_wom \
                    -laxis2_wsdl4c \
					-laxis2_diclient \
                    -L$(WSDLPULL)/lib \
                    -lschema \
                    -lxmlpull \
                    -lwsdl \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
            -I$(AXIS2C_HOME)/platforms
