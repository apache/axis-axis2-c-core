TESTS = woden_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_woden.la
libtest_woden_la_SOURCES = \
						   woden_test.c

noinst_HEADERS= \
				$(top_builddir)/test/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

prgbin_PROGRAMS = woden_test
check_PROGRAMS = woden_test
woden_test_SOURCES = main.c
woden_test_LDADD   =  \
					  libtest_woden.la  \
					  ../src/wsdl20/libwoden_wsdl20.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                   	-laxis2_util \
					-laxis2_axiom \
					-laxis2_xml_schema \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

