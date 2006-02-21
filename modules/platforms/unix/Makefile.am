TESTS =

lib_LTLIBRARIES = libaxis2_unix.la

noinst_HEADERS = axis2_unix.h \
                    axis2_uuid_gen_unix.h \
					axis2_thread_unix.h
                    
libaxis2_unix_la_SOURCES = uuid_gen_unix.c\
                           thread_unix.c

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/modules/platforms \
           -I$(top_builddir)/modules/util 
