prgbindir=$(prefix)/bin/samples/woden
prgbin_PROGRAMS = test_wsdl1
check_PROGRAMS = test_wsdl1
test_wsdl1_SOURCES = \
					 test_wsdl1.c
					 
test_wsdl1_LDADD   = -laxis2_util \
					-laxis2_axiom \
					-laxis2_xml_schema \
					-laxis2_parser \
					-lwoden \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include

