prgbindir=$(prefix)/bin/
prgbin_PROGRAMS = axis2_http_server
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
axis2_http_server_SOURCES = http_server_main.c

axis2_http_server_LDADD   =  $(LDFLAGS)  -laxis2_util -laxis2_http_transport -laxis2_context -laxis2_addr -laxis2_om -laxis2_engine -laxis2_description -laxis2_soap -laxis2_wsdl -laxis2_phaseresolver -laxis2_core_util -laxis2_deployment -laxis2_libxml2 -laxis2_http_sender -laxis2_http_receiver -lxml2 -laxis2_unix\
-L$(top_builddir)/modules/util/.libs\
-L$(top_builddir)/modules/wsdl/.libs\
-L$(top_builddir)/modules/core/description/.libs\
-L$(top_builddir)/modules/core/deployment/.libs\
-L$(top_builddir)/modules/core/context/.libs\
-L$(top_builddir)/modules/core/addr/.libs\
-L$(top_builddir)/modules/core/engine/.libs\
-L$(top_builddir)/modules/core/phaseresolver/.libs\
-L$(top_builddir)/modules/core/util/.libs\
-L$(top_builddir)/modules/core/transport/http/.libs\
-L$(top_builddir)/modules/core/transport/http/sender/.libs\
-L$(top_builddir)/modules/core/transport/http/receiver/.libs\
-L$(top_builddir)/modules/xml/om/.libs\
-L$(top_builddir)/modules/xml/parser/libxml2/.libs\
-L$(top_builddir)/modules/xml/soap/.libs\
-L$(top_builddir)/modules/platforms/unix/.libs\
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
