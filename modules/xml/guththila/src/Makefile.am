lib_LTLIBRARIES = libguththila.la
AM_CPPFLAGS = $(CPPFLAGS)
libguththila_la_SOURCES = guththila_token.c \
                            guththila_xml_pull_parser.c \
                            guththila_buffer.c \
                            guththila_xml_stream_writer.c \
                            guththila_stack.c \
                            guththila_unicode.c \
                            guththila_main.c \
                            guththila_reader.c

libguththila_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include
