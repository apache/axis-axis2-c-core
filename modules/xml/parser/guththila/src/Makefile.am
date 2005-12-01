lib_LTLIBRARIES = libaxis2_guththila.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_guththila_la_SOURCES = guththila_xml_writer_wrapper.c \
                                guththila_xml_reader_wrapper.c 

libaxis2_guththila_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/xml/parser/guththila/impl/src
