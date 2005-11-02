lib_LTLIBRARIES = libaxis2_util.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_util_la_SOURCES = axis2_hash.c \
                        axis2_allocator.c \
                        axis2_environment.c \
                        axis2_error.c \
                        axis2_stream.c \
                        axis2_log.c \
                        axis2_string.c \
                        axis2_file_diff.c \
                        axis2_qname.c

libaxis2_util_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src
