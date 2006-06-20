SUBDIRS = xml
lib_LTLIBRARIES = libwoden_wsdl10.la

libwoden_wsdl10_la_SOURCES = \
							 msg_ref.c \
							 interface_msg_ref.c \
							 interface_fault_ref.c \
							 part.c \
							 desc.c \
							 binding_msg_ref.c \
							 binding_op.c \
							 component_model_builder.c

libwoden_wsdl10_la_LIBADD =  \
                    $(top_builddir)/src/xml/libwoden_xml.la \
                    $(top_builddir)/src/util/libwoden_util.la \
                    $(top_builddir)/src/types/libwoden_types.la \
                    $(top_builddir)/src/builder/wsdl10/libwoden_wsdl10_builder.la \
                    $(top_builddir)/src/schema/libwoden_schema.la \
                    $(top_builddir)/src/wsdl10/xml/libwoden_wsdl10_xml.la

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

EXTRA_DIST = woden_constants.h

