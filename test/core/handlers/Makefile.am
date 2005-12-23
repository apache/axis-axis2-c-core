prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_handler
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -O2 -pthread
test_handler_SOURCES = test_handler.c

test_handler_LDADD   =   -L$(top_builddir)/install/lib -laxis2_util
                    
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src
