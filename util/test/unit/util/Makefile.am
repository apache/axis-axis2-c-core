TESTS = util_test
prglibdir=$(prefix)/lib/unit_test
prgbindir=$(prefix)/bin/unit_test
noinst_HEADERS = $(top_builddir)/test/unit/util/*.h
prglib_LTLIBRARIES = libtest_util.la
libtest_util_la_SOURCES = \
						  	util_hash_test.c \
                            util_stream_test.c \
                            util_log_test.c \
                            util_error_test.c \
                            util_test.c \
                            util_array_list_test.c\
                            util_linked_list_test.c \
                            util_dir_handler_test.c \
                            util_class_loader_test.c \
                            util_string_test.c \
                            util_url_test.c \
                            util_uri_test.c \
                            util_properties_test.c \
                            util_date_time_test.c

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/src/util \
			@UTILINC@ 

check_PROGRAMS = util_test
prgbin_PROGRAMS = util_test
util_test_SOURCES = main.c
util_test_LDADD   =   libtest_util.la  \
                    -L$(CUTEST_HOME)/lib \
                    -lcutest \
					$(top_builddir)/src/libaxis2_util.la \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)
                    
