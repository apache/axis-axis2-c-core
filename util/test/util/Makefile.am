TESTS = test_thread test_util
noinst_PROGRAMS = test_util test_thread
noinst_HEADERS = test_log.h \
                 test_thread.h \
		 create_env.h\
                 test_md5.h
check_PROGRAMS = test_util test_thread
SUBDIRS =
test_util_SOURCES = test_util.c test_log.c test_string.c test_md5.c
test_thread_SOURCES = test_thread.c

test_util_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la \
                        -lpthread

test_thread_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la \
                        -lpthread

INCLUDES = -I$(top_builddir)/include
