prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = stub_interop_test_port_type_doc_service
samplesdir=$(prefix)/samples/client/stub_interop_test_port_type_doc_service

stub_interop_test_port_type_doc_service_SOURCES = \
      test_svc.c \
      axis2_stub_interop_test_port_type_doc_service.c


stub_interop_test_port_type_doc_service_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    -lwoden \
                    -laxis2_xml_schema \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
