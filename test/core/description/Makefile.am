TESTS = test_description
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_description
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_description_SOURCES = test_description.c

test_description_LDADD   =  \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_unix \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -lpthread \
                    -laxis2_soap \
                    -laxis2_parser \
                    $(GUTHTHILA_LIBNAME) \
                    $(LIBXML2_LIBNAME)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include

