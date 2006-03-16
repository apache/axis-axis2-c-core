TESTS = test_om
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_om
check_PROGRAMS = test_om
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_om_SOURCES = test_om.c

test_om_LDADD   =   \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
                    $(top_builddir)/modules/xml/om/libaxis2_om.la \
                    $(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms 
