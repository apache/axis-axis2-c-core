samplesdir=$(prefix)/conf
samples_DATA= schema-config.properties 

prglibdir=$(prefix)/lib
prglib_LTLIBRARIES = libw2c_schema.la

libw2c_schema_la_SOURCES = \
	schema_compiler.c  \
	schema_compiler_options.c  \
	schema_property_loader.c  \
	schema_writer_meta_info.c

libw2c_schema_la_LIBADD = \
                 $(top_builddir)/src/wsdl/libw2c_codegen.la \
				 ../../../../../util/src/libaxis2_util.la \
				 ../../../../../axiom/src/om/libaxis2_axiom.la \
				 ../../../../../xml_schema/src/libaxis2_xml_schema.la \
				 ../../../../../woden/src/wsdl20/libwoden.la


INCLUDES = -I$(top_builddir)/include \
			-I ../../../../../util/include \
			-I ../../../../../axiom/include \
			-I ../../../../../xml_schema/include 
