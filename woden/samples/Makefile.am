SUBDIRS = wsdl10
prgbindir=$(prefix)/bin/samples/woden
prgbin_PROGRAMS = test_wsdl2
check_PROGRAMS = test_wsdl2
test_wsdl2_SOURCES = \
					 test_wsdl2.c
					 
test_wsdl2_LDADD   =  \
                   	-laxis2_util \
					-laxis2_axiom \
					-laxis2_xml_schema \
                    -laxis2_parser \
					../src/wsdl20/libwoden_wsdl20.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@
