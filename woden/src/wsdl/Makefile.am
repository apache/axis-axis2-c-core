lib_LTLIBRARIES = libwoden_wsdl.la

libwoden_wsdl_la_SOURCES = \
							wsdl_element.c \
							wsdl_obj.c \
							documentable.c \
							documentation.c \
							wsdl_component.c \
							nested_component.c \
							configurable_component.c \
							feature.c \
							property.c \
							woden_types.c \
							type_def.c \
							configurable.c \
							nested_configurable.c \
							wsdl_ref.c \
							element_decl.c \
							import.c \
							include.c \
							documentable_element.c \
							documentation_element.c \
							configurable_element.c \
							feature_element.c \
							import_element.c \
							include_element.c \
							nested_element.c \
							property_element.c \
							types_element.c \
							binding.c \
							binding_element.c \
							binding_fault_ref.c \
							binding_fault_ref_element.c \
							interface.c \
							interface_element.c \
							interface_op.c \
							interface_op_element.c \
							msg_label.c \
							direction.c \
							soap_fault_code.c \
							soap_fault_subcodes.c \
							component_exts.c \
							ext_element.c \
							element_extensible.c \
							attr_extensible.c
							


libwoden_wsdl_la_LIBADD =  \
                    $(top_builddir)/src/xml/libwoden_xml.la \
                    $(top_builddir)/src/util/libwoden_util.la \
                    $(top_builddir)/src/types/libwoden_types.la \
                    $(top_builddir)/src/schema/libwoden_schema.la

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@


