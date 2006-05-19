TESTS = phaseresolver_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_phaseresolver.la
libtest_phaseresolver_la_SOURCES = phaseresolver_test.c \
                                    test_resolver.c \
                                    test_phase_holder.c

noinst_HEADERS=$(top_builddir)/test/unit/core/phaseresolver/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I${CUTEST_HOME}/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@UTILINC@/platforms

prgbin_PROGRAMS = phaseresolver_test
check_PROGRAMS = phaseresolver_test
phaseresolver_test_SOURCES = main.c
phaseresolver_test_LDADD   =   libtest_phaseresolver.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                  	-laxis2_util \
					-laxis2_unix \
					-laxis2_om \
					-laxis2_soap \
					-laxis2_parser \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

