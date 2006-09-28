TESTS = woden_test
lib_LTLIBRARIES = libtest_woden.la
libtest_woden_la_SOURCES = \
						   woden_test.c

noinst_HEADERS= \
				$(top_builddir)/test/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
			-I ../../axiom/include \
			-I ../../util/include \
			-I ../../xml_schema/include 

noinst_PROGRAMS = woden_test
check_PROGRAMS = woden_test
woden_test_SOURCES = main.c
woden_test_LDADD   =  \
					  libtest_woden.la  \
					  ../src/wsdl20/libwoden.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../util/src/libaxis2_util.la \
					 ../../axiom/src/om/libaxis2_axiom.la \
					 ../../xml_schema/src/libaxis2_xml_schema.la \
					 ../../axiom/src/parser/$(PARSER_DIR)/libaxis2_parser.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

