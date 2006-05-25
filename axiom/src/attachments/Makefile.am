TESTS =
noinst_LTLIBRARIES = libaxis2_attachments.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_attachments_la_SOURCES = mime_output.c \
					data_handler.c \
					mime_body_part.c \
                    mime_parser.c

libaxis2_attachments_la_LIBADD =  -laxis2_util 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/parser \
			@UTILINC@ \
			@UTILINC@/platforms

EXTRA_DIST = axis2_mime_body_part.h axis2_mime_output.h
