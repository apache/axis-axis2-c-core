noinst_LTLIBRARIES = libwoden_extensions.la
SUBDIRS = soap

libwoden_extensions_la_SOURCES = \
							element_extensible.c \
							attr_extensible.c \
							component_exts.c \
							ext_element.c \
							ext_deserializer.c \
							ext_registry.c

INCLUDES = -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@XMLSCHEMAINC@
