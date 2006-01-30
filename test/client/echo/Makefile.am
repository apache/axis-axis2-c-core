bin_PROGRAMS = echo
echo_SOURCES = axis2_echo_stub.c \
                echo_client.c

echo_LDADD   =  -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_deployment \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_addr \
                    -laxis2_unix \
                    -laxis2_receivers \
                    -laxis2_core_util \
                    -laxis2_clientapi \
                    -laxis2_soap \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include



