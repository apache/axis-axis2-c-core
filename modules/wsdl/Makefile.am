lib_LTLIBRARIES = libaxis2_wsdl.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_wsdl_la_SOURCES = wsdl_op.c \
                        wsdl_svc.c \
                        wsdl_component.c \
                        wsdl_feature.c \
                        wsdl_property.c \
                        wsdl_extensible_component.c \
                        wsdl_interface.c \
                        wsdl_binding_fault.c \
                        wsdl_binding_msg_ref.c \
                        wsdl_binding_op.c \
                        wsdl_binding.c \
                        wsdl_msg_ref.c \
                        wsdl_endpoint.c \
                        wsdl_fault_ref.c \
                        wsdl_extensible_element.c \
                        wsdl_soap_op.c

libaxis2_wsdl_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms
