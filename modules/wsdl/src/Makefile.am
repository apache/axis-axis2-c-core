lib_LTLIBRARIES = libaxis2_wsdl.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_wsdl_la_SOURCES = wsdl_operation.c \
                        wsdl_svc.c \
                        wsdl_component.c \
                        wsdl_feature.c \
                        wsdl_property.c \
                        wsdl_extensible_component.c \
                        wsdl_interface.c

libaxis2_wsdl_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src
