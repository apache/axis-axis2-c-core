TESTS = xml_schema_test
noinst_PROGRAMS = xml_schema_test
check_PROGRAMS = xml_schema_test
SUBDIRS =

AM_CFLAGS = -g -O2 -pthread

xml_schema_test_SOURCES = main.c xml_schema_test.c
                 
xml_schema_test_LDADD   =   \
			-L$(CUTEST_HOME)/lib \
			-lcutest \
			-laxis2_util \
			-laxis2_axiom \
			-laxis2_parser \
			$(top_builddir)/src/libaxis2_xml_schema.la \
			$(LIBXML2_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/axiom/include \
            -I$(top_builddir)/xml_schema/include \
            -I${CUTEST_HOME}/include \
            @UTILINC@

EXTRA_DIST = xml_schema_test.h
