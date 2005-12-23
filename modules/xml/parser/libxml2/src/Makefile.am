lib_LTLIBRARIES = libaxis2_libxml2.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_libxml2_la_SOURCES = axis2_libxml2_reader_wrapper.c \
                              axis2_libxml2_writer_wrapper.c

libaxis2_libxml2_la_LIBADD = 

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/xml/parser \
            -I${LIBXML2_INCLUDE}
