prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = calculator
samplesdir=$(prefix)/samples/client/calculator

calculator_SOURCES = \
      test_calculator.c \
      axis2_stub_calculator.c


calculator_LDADD   = $(LDFLAGS) \
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
