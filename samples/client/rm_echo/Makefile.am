prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = rm_echo
samplesdir=$(prefix)/samples/client/rm_echo
samples_DATA=rm_echo_client.c Makefile.am Makefile.in
rm_echo_SOURCES = rm_echo_client.c echo_util.c

rm_echo_LDADD   =  \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
					-lwoden \
					-laxis2_xml_schema \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
