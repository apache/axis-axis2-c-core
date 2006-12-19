TESTS = test_schema
prgbindir=$(prefix)/bin/samples/xml_schema
prgbin_PROGRAMS = test_schema
check_PROGRAMS = test_schema
test_schema_SOURCES = test_schema.c

test_schema_LDADD   =	-L$(AXIS2C_HOME)/lib \
						-laxis2_xml_schema  \
						-laxis2_axiom \
						-laxis2_parser

INCLUDES = -I$(AXIS2C_HOME)/include 
