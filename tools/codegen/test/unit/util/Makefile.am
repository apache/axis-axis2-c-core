TESTS = w2c_util_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
noinst_HEADERS =
prglib_LTLIBRARIES = libtest_w2c_util.la
libtest_w2c_util_la_SOURCES = w2c_util_test.c \
                            w2c_conf_test.c

INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
		   -I ../../../../../util/include \
		   -I ../../../../../axiom/include \
		   -I ../../../../../xml_schema/include 

check_PROGRAMS = w2c_util_test
prgbin_PROGRAMS = w2c_util_test
w2c_util_test_SOURCES = main.c
w2c_util_test_LDADD   =   libtest_w2c_util.la \
                     $(top_builddir)/src/util/libw2c_util.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../../../../util/src/libaxis2_util.la \
					 ../../../../../axiom/src/om/libaxis2_axiom.la \
					 ../../../../../xml_schema/src/libaxis2_xml_schema.la \
					 ../../../../../axiom/src/parser/$(PARSER_DIR)/libaxis2_parser.la \
                     -lxslt \
                     -lxml2

