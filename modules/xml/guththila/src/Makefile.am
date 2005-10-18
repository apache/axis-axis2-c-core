lib_LTLIBRARIES = libguththila.la
AM_CPPFLAGS = $(CPPFLAGS) -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -g -O2 -pthread
libguththila_la_SOURCES = guththila_token.c \
                            guththila_xml_pull_parser.c \
                            guththila_buffer.c \
                            guththila_xml_stream_writer.c \
                            guththila_stack.c \
                            guththila_unicode.c \
                            guththila_main.c \
                            guththila_reader.c

libguththila_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include -I${APR_HOME}/include/apr-1
