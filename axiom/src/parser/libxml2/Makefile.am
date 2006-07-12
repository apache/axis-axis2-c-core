lib_LTLIBRARIES = libaxis2_libxml2.la libaxis2_parser.la
libaxis2_libxml2_la_SOURCES = libxml2_reader_wrapper.c \
                              libxml2_writer_wrapper.c

libaxis2_libxml2_la_LIBADD = -lxml2

libaxis2_parser_la_SOURCES = libxml2_reader_wrapper.c \
                              libxml2_writer_wrapper.c

libaxis2_parser_la_LIBADD = -lxml2

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
			@UTILINC@
