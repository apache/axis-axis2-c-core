SUBDIRS = xml extensions
noinst_LTLIBRARIES = libwoden_wsdl10.la

libwoden_wsdl10_la_SOURCES = \
							msg_ref.c \
							part.c \
							wsdl10_interface_fault_ref.c \
							wsdl10_interface_msg_ref.c \
							wsdl10_binding_msg_ref.c \
							wsdl10_binding_op.c \
							wsdl10_endpoint.c \
							wsdl10_svc.c \
							wsdl10_desc.c \
							wsdl10_component_model_builder.c

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

EXTRA_DIST = woden_constants.h

