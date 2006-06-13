noinst_LTLIBRARIES = libwoden_extensions_soap.la

libwoden_extensions_soap_la_SOURCES = \
											soap_binding_exts.c \
											soap_binding_fault_exts.c \
											soap_binding_fault_ref_exts.c \
											soap_binding_msg_ref_exts.c \
											soap_binding_op_exts.c \
											soap_header_block_element.c \
											soap_module_element.c \
											soap_header_block.c \
											soap_module.c \
											soap_module_deserializer.c \
											soap_header_block_deserializer.c

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@
