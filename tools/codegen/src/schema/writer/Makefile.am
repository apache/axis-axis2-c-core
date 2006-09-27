
prglibdir=$(prefix)/lib/codegen
prglib_LTLIBRARIES = libw2c_schema_c_writer.la
libw2c_schema_c_writer_la_SOURCES = \
   schema_c_writer.c \
   schema_writer.c

libw2c_schema_c_writer_la_LIBADD = \
                 $(top_builddir)/src/schema/compiler/libw2c_schema.la \
                 $(top_builddir)/src/wsdl/libw2c_codegen.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom \
                 -laxis2_xml_schema


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @XMLSCHEMAINC@ \
                        @AXIOMINC@
