TESTS = test_schema
prgbindir=$(prefix)/bin/samples/xml_schema
prgbin_PROGRAMS = test_schema
check_PROGRAMS = test_schema
test_schema_SOURCES = test_schema.c

test_schema_LDADD   =   \
			$(top_builddir)/src/libaxis2_xml_schema.la \
			-laxis2_axiom \
			-laxis2_parser 

INCLUDES = -I$(top_builddir)/include \
		@UTILINC@ 
