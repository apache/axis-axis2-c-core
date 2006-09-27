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
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom \
                 -laxis2_xml_schema


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @XMLSCHEMAINC@ \
                        @AXIOMINC@
