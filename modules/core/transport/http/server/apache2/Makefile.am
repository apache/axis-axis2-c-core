lib_LTLIBRARIES = libmod_axis2.la

libmod_axis2_la_SOURCES = mod_axis2.c\
                       apache2_stream.c\
                       apache2_out_transport_info.c\
                       apache2_worker.c
AM_CFLAGS = -DAP_HAVE_DESIGNATED_INITIALIZER -DLINUX=2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -D_SVID_SOURCE -D_GNU_SOURCE
libmod_axis2_la_LIBADD   = $(LDFLAGS) \
                    -L$(top_builddir)/modules/util \
                    -L$(top_builddir)/modules/wsdl \
                    -L$(top_builddir)/modules/core/engine \
                    -L$(top_builddir)/modules/core/transport/http/sender \
                    -L$(top_builddir)/modules/core/transport/http/receiver \
                    -L$(top_builddir)/modules/xml/om \
                    -L$(top_builddir)/modules/xml/parser/libxml2 \
                    -L$(top_builddir)/modules/xml/soap \
                    -L$(top_builddir)/modules/platforms/unix \
                    -laxis2_om \
                    -laxis2_engine \
                    -laxis2_soap \
                    -lpthread \
                    -laxis2_wsdl \
                    -laxis2_libxml2 \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    -lxml2 \
                    -laxis2_unix\
					$(LIBXML2_LIBS) \
					$(GUTHTHILA_LIBS)

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/modules/util \
           -I$(top_builddir)/modules/xml/parser\
	       -I$(top_builddir)/modules/wsdl\
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/core/context \
           -I$(top_builddir)/modules/core/phaseresolver \
           -I$(top_builddir)/modules/core/engine \
           -I$(top_builddir)/modules/core/deployment \
           -I$(top_builddir)/modules/platforms\
            @APACHE2INC@ \
            @APRINC@

EXTRA_DIST=axis2_apache2_worker.h apache2_stream.h axis2_apache2_out_transport_info.h
