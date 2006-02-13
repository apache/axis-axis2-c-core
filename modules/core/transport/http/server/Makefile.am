TESTS =
prgbindir=$(prefix)/bin/
prgbin_PROGRAMS = axis2_http_server
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
axis2_http_server_SOURCES = http_server_main.c

axis2_http_server_LDADD   = $(LDFLAGS) \
                            -L$(AXIS2C_HOME)/lib  \
                            -laxis2_util \
                            -laxis2_http_transport \
                            -laxis2_context \
                            -laxis2_addr \
                            -laxis2_om \
                            -laxis2_engine \
                            -laxis2_receivers \
                            -laxis2_description \
                            -laxis2_soap \
                            -laxis2_wsdl \
                            -laxis2_phaseresolver \
                            -laxis2_core_util \
                            -laxis2_deployment \
                            -laxis2_libxml2 \
                            -laxis2_http_sender \
                            -laxis2_http_receiver \
                            -lxml2 \
                            -laxis2_unix \
                            -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
                            -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME)
                    
INCLUDES =  -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util\
            -I$(top_builddir)/modules/wsdl\
            -I$(top_builddir)/modules/core/description\
            -I$(top_builddir)/modules/core/deployment\
            -I$(top_builddir)/modules/core/transport\
            -I$(top_builddir)/modules/xml/om\
            -I$(top_builddir)/modules/xml/parser\
            -I$(top_builddir)/modules/xml/soap\
            -I$(top_builddir)/modules/platforms
