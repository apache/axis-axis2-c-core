lib_LTLIBRARIES = libaxis2_soap.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_soap_la_SOURCES = soap_fault.c \
                           soap_fault_code.c \
                           soap_fault_detail.c \
                           soap_fault_node.c \
                           soap_fault_reason.c \
                           soap_fault_role.c \
                           soap_fault_sub_code.c \
                           soap_fault_text.c \
                           soap_fault_value.c \
                           soap_header_block.c \
                           soap_header.c \
                           soap_utils.c \
                           soap_body.c \
                           soap_envelope.c \
                           soap_builder.c \
                           soap11_builder_helper.c \
                           soap12_builder_helper.c

libaxis2_soap_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms
