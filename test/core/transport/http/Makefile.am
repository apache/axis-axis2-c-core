TESTS = test_http_transport
check_PROGRAMS = test_http_transport
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
test_http_transport_SOURCES = test_http_transport.c

test_http_transport_LDADD   =  $(LDFLAGS) \
								-L$(AXIS2C_HOME)/lib \
								-laxis2_util \
								-laxis2_om \
								-laxis2_unix \
								-laxis2_engine \
								-laxis2_soap \
								-laxis2_wsdl \
								-laxis2_parser \
								-laxis2_http_sender \
								-L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
                    			-L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME)
                    
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util\
            -I$(top_builddir)/modules/platforms
