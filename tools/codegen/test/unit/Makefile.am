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
                     -L$(AXIS2C_HOME)/lib \
                     -laxis2_util \
                     -laxis2_axiom \
                     -laxis2_parser \
                     -lxslt \
                     -lxml2


INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
                        @UTILINC@ \
                        @AXIOMINC@

