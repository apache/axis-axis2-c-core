lib_LTLIBRARIES = libaxis2_soap.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_soap_la_SOURCES = axis2_soap_fault.c \
                            axis2_soap_fault_code.c \
                            axis2_soap_fault_detail.c \
                            axis2_soap_fault_node.c \
                            axis2_soap_fault_reason.c \
                            axis2_soap_fault_role.c \
                            axis2_soap_fault_sub_code.c \
                            axis2_soap_fault_text.c \
                            axis2_soap_fault_value.c \
                            axis2_soap_header_block.c \
                            axis2_soap_header.c \
                            axis2_soap_utils.c \
                            soap_body.c \
                            soap_envelope.c

libaxis2_soap_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/platforms
