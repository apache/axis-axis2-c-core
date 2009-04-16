TESTS = 
noinst_PROGRAMS = test_xpath test_xpath_streaming
check_PROGRAMS = test_xpath test_xpath_streaming
SUBDIRS =
test_xpath_SOURCES = test_xpath.c

test_xpath_streaming_SOURCES = test_xpath_streaming.c

test_xpath_LDADD   =	../../../util/src/libaxutil.la \
                    $(top_builddir)/src/om/libaxis2_axiom.la \
                    $(top_builddir)/src/xpath/libaxis2_xpath.la \
                    $(top_builddir)/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la

test_xpath_streaming_LDADD   =	../../../util/src/libaxutil.la \
                    $(top_builddir)/src/om/libaxis2_axiom.la \
                    $(top_builddir)/src/xpath/libaxis2_xpath.la \
                    $(top_builddir)/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
			-I ../../../include \
			-I ../../../util/include 
