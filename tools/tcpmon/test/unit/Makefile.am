TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = tcpmon_unit_test_suite

tcpmon_unit_test_suite_SOURCES = main.c tcpmon_test.c

tcpmon_unit_test_suite_LDADD   =   \
                     $(top_builddir)/src/libaxis2_tcpmon.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     -L$(AXIS2C_HOME)/lib \
                     -laxis2_util \
                     $(NULL)


INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
           @UTILINC@ \
           $(NULL)
