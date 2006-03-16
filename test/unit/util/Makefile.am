TESTS =  util_test
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
                            util_dir_handler_test.c \
                            util_class_loader_test.c

noinst_HEADERS=$(top_builddir)/test/unit/util/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms

prgbin_PROGRAMS = util_test
check_PROGRAMS = util_test
util_test_SOURCES = main.c
util_test_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     $(top_builddir)/test/unit/util/libtest_util.la \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
                    -lpthread \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

