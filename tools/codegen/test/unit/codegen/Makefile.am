TESTS = w2c_codegen_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
noinst_HEADERS =
prglib_LTLIBRARIES = libtest_w2c_codegen.la
libtest_w2c_codegen_la_SOURCES = w2c_engine_test.c w2c_writer_test.c

INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
		   -I ../../../../../util/include \
		   -I ../../../../../axiom/include \
		   -I ../../../../../include 

check_PROGRAMS = w2c_codegen_test
prgbin_PROGRAMS = w2c_codegen_test
w2c_codegen_test_SOURCES = main.c
w2c_codegen_test_LDADD   =   libtest_w2c_codegen.la \
                     $(top_builddir)/src/wsdl/libw2c_codegen.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../../../../util/src/libaxis2_util.la \
					 ../../../../../axiom/src/om/libaxis2_axiom.la \
					 ../../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
                     -lxslt \
                     -lxml2

