noinst_LTLIBRARIES = libaxis2_receivers.la

libaxis2_receivers_la_SOURCES = \
                                msg_recv.c \
                                raw_xml_in_out_msg_recv.c \
                                svr_callback.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/util \
            -I$(top_builddir)/xml_schema/include\
			@AXIOMINC@ \
			@UTILINC@ 
