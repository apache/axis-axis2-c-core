lib_LTLIBRARIES = libaxis2_wsdl.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_wsdl_la_SOURCES = wsdl_operation.c \
                        wsdl_svc.c \
                        wsdl_component.c \
                        wsdl_feature.c \
                        wsdl_property.c \
                        wsdl_extensible_component.c \
                        wsdl_interface.c \
                        wsdl_binding_fault.c \
                        wsdl_binding_msg_ref.c \
                        wsdl_binding_operation.c \
                        wsdl_binding.c \
                        wsdl_msg_ref.c \
                        wsdl_endpoint.c \
                        wsdl_fault_ref.c \
                        wsdl_extensible_element.c \
                        wsdl_soap_operation.c

libaxis2_wsdl_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/core/transport
