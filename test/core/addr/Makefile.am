TESTS = test_addr
check_PROGRAMS = test_addr
noinst_PROGRAMS = test_addr
SUBDIRS =
AM_CFLAGS = -g -pthread
test_addr_SOURCES = test_addr.c

test_addr_LDADD   =  \
                    ../../../util/src/libaxis2_util.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
	                -L$(top_builddir)$(GUTHTHILA_LIBS) \
	                $(LIBXML2_LIBS)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
            -I ../../../util/include \
            -I ../../../axiom/include

