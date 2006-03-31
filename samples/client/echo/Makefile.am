prgbindir=$(prefix)/bin/samples
samplesdir=$(prefix)/samples/client/echo
prgbin_PROGRAMS = echo
noinst_HEADERS = axis2_echo_stub.h
samples_DATA=axis2_echo_stub.c echo_client.c axis2_echo_stub.h Makefile.am Makefile.in

echo_SOURCES = axis2_echo_stub.c \
                echo_client.c

echo_LDADD   =  $(LDFLAGS) \
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
                $(LIBXML2_LIBS) \
                $(GUTHTHILA_LIBS)


INCLUDES = -I$(AXIS2C_HOME)/include \
            -I$(AXIS2C_HOME)/modules/platforms
