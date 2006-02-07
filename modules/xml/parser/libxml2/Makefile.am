TESTS =
lib_LTLIBRARIES = libaxis2_libxml2.la libaxis2_parser.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_libxml2_la_SOURCES = libxml2_reader_wrapper.c \
                              libxml2_writer_wrapper.c

libaxis2_parser_la_SOURCES = libxml2_reader_wrapper.c \
                              libxml2_writer_wrapper.c

libaxis2_libxml2_la_LIBADD = 

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms \
            -I${LIBXML2_INCLUDE}
