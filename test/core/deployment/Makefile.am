TESTS = test_deployment
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_deployment
SUBDIRS =
AM_CFLAGS = $(CFLAGS) -g -pthread
test_deployment_SOURCES = test_deployment.c

test_deployment_LDADD   =  \
                    -laxis2_util \
					-laxis2_axiom \
					-laxis2_parser \
					-lwoden \
					-laxis2_xml_schema \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/core/util \
            -I$(top_builddir)/xml_schema/include \
			@AXIOMINC@ \
			@UTILINC@ 

