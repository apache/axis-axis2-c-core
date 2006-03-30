prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = notify
samplesdir=$(prefix)/samples/client/notify
samples_DATA=notify_client.c Makefile.am
notify_SOURCES = notify_client.c

notify_LDADD   =  \
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
