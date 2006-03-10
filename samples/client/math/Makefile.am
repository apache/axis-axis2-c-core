prgbindir=$(prefix)/bin/samples
samplesdir=$(prefix)/samples/client/math
prgbin_PROGRAMS = math
noinst_HEADERS = axis2_math_stub.h
samples_DATA=axis2_math_stub.c math_client.c axis2_math_stub.h
math_SOURCES = axis2_math_stub.c \
                math_client.c

math_LDADD   =  \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_unix \
                    -lpthread \
                    -laxis2_soap \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser


