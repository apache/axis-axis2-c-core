bindir=$(prefix)/bin/system_test
TESTS = test_engine
check_PROGRAMS = test_engine
bin_PROGRAMS = test_engine
SUBDIRS =
AM_CFLAGS = -g -pthread
test_engine_SOURCES = test_engine.c


test_engine_LDADD   =   \
                    ../../../util/src/libaxis2_util.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
	                -L$(top_builddir)$(GUTHTHILA_LIBS) \
	                $(LIBXML2_LIBS)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I ../../../util/include \
            -I ../../../axiom/include

