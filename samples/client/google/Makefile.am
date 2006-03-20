prgbindir=$(prefix)/bin/samples
samplesdir=$(prefix)/samples/client/google
prgbin_PROGRAMS = google
samples_DATA=google_client.c
google_SOURCES = google_client.c

google_LDADD   =    $(LDFLAGS) \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_unix \
                    -laxis2_soap \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
            -I$(AXIS2C_HOME)/platforms
