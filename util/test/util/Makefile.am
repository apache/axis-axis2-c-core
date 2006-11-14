TESTS = test_util
bindir=$(prefix)/bin/system_test
bin_PROGRAMS = test_util test_thread
noinst_HEADERS = test_log.h \
                    test_thread.h 
check_PROGRAMS = test_util test_thread
SUBDIRS =
test_util_SOURCES = test_util.c test_log.c test_string.c
test_thread_SOURCES = test_thread.c

test_util_LDADD   =   \
                    $(top_builddir)/src/libaxis2_util.la \
                        -lpthread

test_thread_LDADD   =   \
                    $(top_builddir)/src/libaxis2_util.la \
                        -lpthread

INCLUDES = -I$(top_builddir)/include
