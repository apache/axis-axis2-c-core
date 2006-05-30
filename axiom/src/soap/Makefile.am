noinst_LTLIBRARIES = libaxis2_soap.la
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
                           soap_body.c \
                           soap_envelope.c \
                           soap_builder.c \
                           soap11_builder_helper.c \
                           soap12_builder_helper.c

libaxis2_soap_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
            -I$(top_builddir)/src/om \
			@UTILINC@

EXTRA_DIST=_axis2_soap_body.h _axis2_soap_fault.h _axis2_soap_fault_code.h \
		_axis2_soap_fault_reason.h _axis2_soap_fault_value.h \
		_axis2_soap_fault_detail.h _axis2_soap_fault_role.h \
		 _axis2_soap_header_block.h _axis2_soap_body.h \
		 _axis2_soap_fault.h _axis2_soap_fault_sub_code.h \
		_axis2_soap_header.h _axis2_soap_envelope.h \
		_axis2_soap_fault_node.h _axis2_soap_fault_text.h \
		axis2_soap11_builder_helper.h axis2_soap12_builder_helper.h \
		maven.xml project.xml
