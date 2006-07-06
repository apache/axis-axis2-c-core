SUBDIRS = xml extensions
noinst_LTLIBRARIES = libwoden_wsdl10.la

libwoden_wsdl10_la_SOURCES = \
							msg_ref.c \
							part.c \
							interface_fault_ref.c \
							interface_msg_ref.c \
							binding_msg_ref.c \
							binding_op.c \
							endpoint.c \
							svc.c \
							desc.c \
							component_model_builder.c

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

EXTRA_DIST = woden_constants.h

