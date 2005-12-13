lib_LTLIBRARIES = libaxis2_util.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_util_la_SOURCES = hash.c \
                        allocator.c \
                        env.c \
                        error.c \
                        stream.c \
                        log.c \
                        string.c \
                        file_diff.c \
                        qname.c \
                        array_list.c \
                        linked_list.c \
                        utils.c

libaxis2_util_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src
