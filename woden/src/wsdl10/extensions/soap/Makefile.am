noinst_LTLIBRARIES = libwoden_wsdl10_extensions_soap.la

libwoden_wsdl10_extensions_soap_la_SOURCES = \
											wsdl10_soap_binding_exts.c \
											wsdl10_soap_binding_fault_exts.c \
											wsdl10_soap_binding_fault_ref_exts.c \
											wsdl10_soap_binding_msg_ref_exts.c \
											wsdl10_soap_binding_op_exts.c \
											wsdl10_soap_header_block_element.c \
											wsdl10_soap_module_element.c \
											wsdl10_soap_header_block.c \
											wsdl10_soap_module.c \
											wsdl10_soap_module_deserializer.c \
											wsdl10_soap_header_block_deserializer.c \
											soap_address_exts.c

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

EXTRA_DIST = woden_soap_constants.h

