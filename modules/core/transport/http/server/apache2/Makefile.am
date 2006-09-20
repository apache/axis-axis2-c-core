lib_LTLIBRARIES = libmod_axis2.la

libmod_axis2_la_SOURCES = mod_axis2.c\
                       apache2_stream.c\
                       apache2_out_transport_info.c\
                       apache2_worker.c
AM_CFLAGS = -DLINUX=2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -D_SVID_SOURCE -D_GNU_SOURCE
libmod_axis2_la_LIBADD   = $(LDFLAGS) \
                    -L$(top_builddir)/modules/wsdl \
                    -L$(top_builddir)/modules/core/engine \
                    -L$(top_builddir)/modules/core/transport/http/sender \
                    -L$(top_builddir)/modules/core/transport/http/receiver \
                    -laxis2_axiom \
                    -laxis2_engine \
                    -lpthread \
                    -laxis2_parser \
                    -laxis2_wsdl \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
					$(LIBXML2_LIBS) \
					$(GUTHTHILA_LIBS)

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/modules/wsdl\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/core/context \
           -I$(top_builddir)/modules/core/phaseresolver \
           -I$(top_builddir)/modules/core/engine \
           -I$(top_builddir)/modules/core/deployment \
           -I$(top_builddir)/xml_schema/include\
            @APACHE2INC@ \
            @APRINC@ \
			@AXIOMINC@ \
			@UTILINC@ 

EXTRA_DIST=axis2_apache2_worker.h apache2_stream.h axis2_apache2_out_transport_info.h
