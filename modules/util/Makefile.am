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
                        utils.c \
                        dir_handler.c \
                        class_loader.c

libaxis2_util_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/platforms
