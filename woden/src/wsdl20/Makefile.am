SUBDIRS = xml extensions
lib_LTLIBRARIES = libwoden_wsdl20.la

libwoden_wsdl20_la_SOURCES = \
							interface_fault.c \
							interface_fault_ref.c \
							interface_msg_ref.c \
							binding_fault.c \
							binding_msg_ref.c \
							binding_op.c \
							endpoint.c \
							svc.c \
							desc.c \
							component_model_builder.c

libwoden_wsdl20_la_LIBADD =  \
                    $(top_builddir)/src/xml/libwoden_xml.la \
                    $(top_builddir)/src/util/libwoden_util.la \
                    $(top_builddir)/src/types/libwoden_types.la \
                    $(top_builddir)/src/builder/libwoden_builder.la \
                    $(top_builddir)/src/schema/libwoden_schema.la \
                    $(top_builddir)/src/wsdl20/xml/libwoden_wsdl20_xml.la \
                    $(top_builddir)/src/wsdl20/extensions/libwoden_extensions.la \
                    $(top_builddir)/src/wsdl20/extensions/soap/libwoden_extensions_soap.la \
                    $(top_builddir)/src/builder/wsdl10/libwoden_wsdl10_builder.la \
                    $(top_builddir)/src/wsdl10/xml/libwoden_wsdl10_xml.la \
                    $(top_builddir)/src/wsdl10/extensions/libwoden_wsdl10_extensions.la \
                    $(top_builddir)/src/wsdl10/extensions/soap/libwoden_wsdl10_extensions_soap.la \
                    $(top_builddir)/src/wsdl10/libwoden_wsdl10.la \
                    $(top_builddir)/src/wsdl/libwoden_wsdl.la
INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

EXTRA_DIST = woden_constants.h

