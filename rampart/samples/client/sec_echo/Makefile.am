SUBDIRS = data 
prgbindir=$(prefix)/bin/samples/rampart/client/sec_echo
prgbin_PROGRAMS = echo
#samplesdir=$(prefix)/samples/client/echo
#samples_DATA=echo.c README.txt Makefile.am Makefile.in
echo_SOURCES = echo.c

echo_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -lwoden \
                    -laxis2_xml_schema \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
            -I ../../../include \
			@UTILINC@ \
			@AXIOMINC@

EXTRA_DIST = README.txt
