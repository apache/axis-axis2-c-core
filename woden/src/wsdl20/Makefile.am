SUBDIRS = xml extensions
lib_LTLIBRARIES = libwoden.la

libwoden_la_SOURCES =	woden_interface_fault.c \
						woden_interface_fault_ref.c \
						woden_interface_msg_ref.c \
						woden_binding_fault.c \
						woden_binding_msg_ref.c \
						woden_binding_op.c \
						woden_endpoint.c \
						woden_svc.c \
						woden_desc.c \
						woden_component_model_builder.c

libwoden_la_LIBADD =$(top_builddir)/src/xml/libwoden_xml.la \
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
			-I ../../../util/include \
			-I ../../../axiom/include \
			-I ../../../xml_schema/include

EXTRA_DIST = woden_constants.h woden_component_model_builder.h

