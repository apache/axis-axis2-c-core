TESTS =
noinst_LTLIBRARIES = libaxis2_receivers.la
AM_CPPFLAGS = $(CPPFLAGS)

libaxis2_receivers_la_SOURCES = msg_recv.c \
                                raw_xml_in_out_msg_recv.c \
                                svr_callback.c

libaxis2_receivers_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
