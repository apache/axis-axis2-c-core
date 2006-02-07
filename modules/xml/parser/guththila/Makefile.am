TESTS =
lib_LTLIBRARIES = libaxis2_guththila.la
SUBDIRS = impl
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_guththila_la_SOURCES = guththila_xml_writer_wrapper.c \
                                guththila_xml_reader_wrapper.c 

libaxis2_guththila_la_LIBADD = 

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/xml/parser \
            -I${GUTHTHILA_INCLUDE} \
            -I$(top_builddir)/modules/platforms
           
