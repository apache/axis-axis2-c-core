TESTS = test_client test_clientapi
check_PROGRAMS = test_client test_clientapi
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread 
test_client_SOURCES = test_client.c
test_clientapi_SOURCES = test_clientapi.c

test_clientapi_LDADD   =  -L$(AXIS2C_HOME)/lib \
                    -laxis2_description \
	                -laxis2_context \
	                -laxis2_util \
                    -laxis2_unix \
	                -laxis2_core_util \
	                -laxis2_deployment \
	                -laxis2_om \
	                -laxis2_engine \
	                -laxis2_wsdl \
	                -laxis2_http_sender \
	                -laxis2_parser \
	                -laxis2_phaseresolver \
	                -laxis2_addr \
	                -laxis2_receivers \
					-laxis2_clientapi \
	                -laxis2_http_transport \
					-laxis2_soap \
                    -laxis2_unix \
	                -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
	                -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME)

test_client_LDADD   =  -L$(AXIS2C_HOME)/lib \
                    -laxis2_description \
	                -laxis2_context \
	                -laxis2_util \
	                -laxis2_core_util \
	                -laxis2_deployment \
	                -laxis2_om \
	                -laxis2_engine \
	                -laxis2_wsdl \
	                -laxis2_parser \
	                -laxis2_phaseresolver \
	                -laxis2_addr \
	                -laxis2_receivers \
					-laxis2_clientapi \
					-laxis2_soap \
                    -laxis2_unix \
	                -laxis2_http_transport \
	                -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
	                -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) 

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms

