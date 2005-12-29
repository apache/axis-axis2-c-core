prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_util.la

libtest_util_la_SOURCES = util_hash_test.c \
                            util_stream_test.c \
                            util_log_test.c \
                            util_error_test.c \
                            util_test.c \
                            util_array_list_test.c\
                            util_linked_list_test.c \
                            util_dir_handler_test.c

include_HEADERS=$(top_builddir)/test/unit/util/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms

prgbin_PROGRAMS = util_test
util_test_SOURCES = main.c
util_test_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     $(top_builddir)/test/unit/util/libtest_util.la \
                    -L$(top_builddir)/install/lib \
                    -laxis2_util

