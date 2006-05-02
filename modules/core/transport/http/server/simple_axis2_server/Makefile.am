TESTS =
prgbindir=$(prefix)/bin/
prgbin_PROGRAMS = axis2_http_server
SUBDIRS =
AM_CFLAGS = -g -pthread
axis2_http_server_SOURCES = http_server_main.c

axis2_http_server_LDADD   =  $(LDFLAGS) \
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
                    
INCLUDES =  -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util\
            -I$(top_builddir)/modules/wsdl\
            -I$(top_builddir)/modules/core/description\
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/deployment\
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/xml/om\
            -I$(top_builddir)/modules/xml/parser\
            -I$(top_builddir)/modules/xml/soap\
            -I$(top_builddir)/modules/platforms
