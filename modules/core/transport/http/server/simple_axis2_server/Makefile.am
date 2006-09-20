prgbindir=$(prefix)/bin/
prgbin_PROGRAMS = axis2_http_server
SUBDIRS =
AM_CFLAGS = -g -pthread
axis2_http_server_SOURCES = http_server_main.c

axis2_http_server_LDADD   =  $(LDFLAGS) \
                            -L$(top_builddir)/modules/wsdl \
                            -L$(top_builddir)/modules/core/engine \
                            -L$(top_builddir)/modules/core/transport/http/sender \
                            -L$(top_builddir)/modules/core/transport/http/receiver \
                            -laxis2_util \
                            -laxis2_axiom \
                            -laxis2_engine \
                            -lpthread \
                            -laxis2_parser \
                            -laxis2_wsdl \
                            -laxis2_http_sender \
                            -laxis2_http_receiver \
							-lwoden \
							-laxis2_xml_schema \
                            $(LIBXML2_LIBS) \
                            $(GUTHTHILA_LIBS)
                    
INCLUDES =  -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl\
            -I$(top_builddir)/modules/core/description\
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/deployment\
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/xml_schema/include\
			@AXIOMINC@ \
			@UTILINC@ 
