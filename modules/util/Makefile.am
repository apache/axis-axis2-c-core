TESTS =
lib_LTLIBRARIES = libaxis2_util.la
noinst_HEADERS = axis2_hash.h \
				axis2_string.h \
				axis2_array_list.h \
				axis2_log_default.h \
				axis2_error_default.h \
				axis2_file_diff.h \
				axis2_utils.h \
				axis2_linked_list.h

AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_util_la_SOURCES = hash.c \
                        allocator.c \
                        env.c \
                        error.c \
                        stream.c \
                        log.c \
                        string.c \
                        qname.c \
                        array_list.c \
                        linked_list.c \
                        utils.c \
                        dir_handler.c \
                        file_handler.c \
                        file_diff.c\
                        class_loader.c\
                        network_handler.c \
                        file.c\
                        uuid_gen.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms
