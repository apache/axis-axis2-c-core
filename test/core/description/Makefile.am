TESTS = test_description
bindir=$(prefix)/bin/system_test
bin_PROGRAMS = test_description
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_description_SOURCES = test_description.c

test_description_LDADD   =  \
					../../../util/src/libaxis2_util.la \
					../../../axiom/src/om/libaxis2_axiom.la \
					../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
                    -lpthread \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
                    -L$(top_builddir)$(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
			-I ../../../util/include \
			-I ../../../axiom/include 

