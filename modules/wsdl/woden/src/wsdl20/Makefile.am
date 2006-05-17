SUBDIRS = extensions xml
noinst_LTLIBRARIES = libaxis2_woden_wsdl20.la

libaxis2_woden_wsdl20_la_SOURCES = \
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
							configurable.c \
							nested_configurable.c \
							interface.c \
							interface_fault.c \
							interface_fault_ref.c \
							interface_msg_ref.c \
							interface_op.c \
							binding.c \
							binding_fault.c \
							binding_fault_ref.c \
							binding_msg_ref.c \
							binding_op.c

INCLUDES = -I$(top_builddir)/include \
            -I$(AXIS2C_HOME)/include \
            -I$(XML_SCHEMA)/include
