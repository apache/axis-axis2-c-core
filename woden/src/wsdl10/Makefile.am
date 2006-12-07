SUBDIRS = xml extensions
noinst_LTLIBRARIES = libwoden_wsdl10.la

libwoden_wsdl10_la_SOURCES =woden_msg_ref.c \
							woden_part.c \
							woden_wsdl10_interface_fault_ref.c \
							woden_wsdl10_interface_msg_ref.c \
							woden_wsdl10_binding_msg_ref.c \
							woden_wsdl10_binding_op.c \
							woden_wsdl10_endpoint.c \
							woden_wsdl10_svc.c \
							woden_wsdl10_desc.c \
							woden_wsdl10_component_model_builder.c

INCLUDES = -I$(top_builddir)/include \
			-I ../../../util/include \
			-I ../../../axiom/include \
			-I ../../../xml_schema/include

EXTRA_DIST = woden_wsdl10_component_model_builder.h  woden_wsdl10_constants.h

