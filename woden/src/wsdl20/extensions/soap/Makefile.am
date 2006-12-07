noinst_LTLIBRARIES = libwoden_extensions_soap.la

libwoden_extensions_soap_la_SOURCES = \
											woden_soap_binding_exts.c \
											woden_soap_binding_fault_exts.c \
											woden_soap_binding_fault_ref_exts.c \
											woden_soap_binding_msg_ref_exts.c \
											woden_soap_binding_op_exts.c \
											woden_soap_header_block_element.c \
											woden_soap_module_element.c \
											woden_soap_header_block.c \
											woden_soap_module.c \
											woden_soap_module_deserializer.c \
											woden_soap_header_block_deserializer.c

INCLUDES = -I$(top_builddir)/include \
			-I ../../../../../util/include \
			-I ../../../../../axiom/include \
			-I ../../../../../xml_schema/include 

EXTRA_DIST = woden_soap_constants.h

