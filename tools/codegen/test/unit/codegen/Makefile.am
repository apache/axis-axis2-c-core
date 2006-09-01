TESTS = w2c_codegen_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
noinst_HEADERS =
prglib_LTLIBRARIES = libtest_w2c_codegen.la
libtest_w2c_codegen_la_SOURCES = w2c_engine_test.c w2c_writer_test.c

INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
           @UTILINC@ \
           @AXIOMINC@

check_PROGRAMS = w2c_codegen_test
prgbin_PROGRAMS = w2c_codegen_test
w2c_codegen_test_SOURCES = main.c
w2c_codegen_test_LDADD   =   libtest_w2c_codegen.la \
                     $(top_builddir)/src/wsdl/libw2c_codegen.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     -L$(AXIS2C_HOME)/lib \
                     -laxis2_util \
                     -laxis2_axiom \
                     -laxis2_parser \
                     -lxslt \
                     -lxml2

