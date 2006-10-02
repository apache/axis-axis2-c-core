TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = w2c_unit_test_suite
SUBDIRS = util codegen
w2c_unit_test_suite_SOURCES = main.c


w2c_unit_test_suite_LDADD   =   \
		     util/libtest_w2c_util.la \
                     $(top_builddir)/src/util/libw2c_util.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     ../../../../util/src/libaxis2_util.la \
                     ../../../../axiom/src/om/libaxis2_axiom.la \
                     ../../../../xml_schema/src/libaxis2_xml_schema.la \
                     ../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
                     -lxslt \
                     -lxml2


INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
           -I ../../../../../util/include \
           -I ../../../../../xml_schema/include \
           -I ../../../../../axiom/include


