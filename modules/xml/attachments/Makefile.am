TESTS =
lib_LTLIBRARIES = libaxis2_attachments.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_attachments_la_SOURCES = mime_output.c \
					data_handler.c \
					mime_body_part.c	

libaxis2_attachments_la_LIBADD =  ../../util/libaxis2_util.la 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
