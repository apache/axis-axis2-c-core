prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_util.la
AM_CPPFLAGS = $(CPPFLAGS) -g
libtest_util_la_SOURCES = util_hash_test.c \
                            util_stream_test.c \
                            util_log_test.c \
                            util_error_test.c \
                            util_test.c \
                            util_array_list_test.c\
                            util_linked_list_test.c

include_HEADERS=$(top_builddir)/modules/util/test/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src

prgbin_PROGRAMS = util_test
util_test_SOURCES = main.c
util_test_LDADD   =  $(LDFLAGS) \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     $(top_builddir)/test/unit/util/libtest_util.la \
                    -L$(top_builddir)/install/lib \
                    -laxis2_util \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_guththila \
                    -lguththila

