SUBDIRS = enumeration xml extensions
lib_LTLIBRARIES = libwoden_wsdl10.la

libwoden_wsdl10_la_SOURCES = \
							wsdl_element.c \
							wsdl_obj.c \
							documentable.c \
							documentation.c \
							wsdl_component.c \
							nested_component.c \
							configurable_component.c \
							feature.c \
							property.c \
							types.c \
							type_def.c \
							msg_ref.c \
							part.c \
							configurable.c \
							nested_configurable.c \
							interface.c \
							interface_fault_ref.c \
							interface_msg_ref.c \
							interface_op.c \
							binding.c \
							binding_fault_ref.c \
							binding_msg_ref.c \
							binding_op.c \
							endpoint.c \
							svc.c \
							wsdl_ref.c \
							element_decl.c \
							import.c \
							include.c \
							desc.c \
							component_model_builder.c

libwoden_wsdl10_la_LIBADD =  \
                    $(top_builddir)/src/xml/libwoden_xml.la \
                    $(top_builddir)/src/util/libwoden_util.la \
                    $(top_builddir)/src/types/libwoden_types.la \
                    $(top_builddir)/src/schema/libwoden_schema.la \
                    $(top_builddir)/src/builder/wsdl10/libwoden_wsdl10_builder.la \
                    $(top_builddir)/src/wsdl10/xml/libwoden_wsdl10_xml.la \
                    $(top_builddir)/src/wsdl10/enumeration/libwoden_wsdl10_enumeration.la \
                    $(top_builddir)/src/wsdl10/extensions/libwoden_wsdl10_extensions.la \
                    $(top_builddir)/src/wsdl10/extensions/soap/libwoden_wsdl10_extensions_soap.la

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@

EXTRA_DIST = woden_constants.h

