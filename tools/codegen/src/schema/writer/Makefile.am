
prglibdir=$(prefix)/lib/codegen
prglib_LTLIBRARIES = libw2c_schema_c_writer.la
libw2c_schema_c_writer_la_SOURCES = \
   schema_c_writer.c \
   schema_writer.c

libw2c_schema_c_writer_la_LIBADD = \
                 $(top_builddir)/src/schema/compiler/libw2c_schema.la \
                 $(top_builddir)/src/wsdl/libw2c_codegen.la \
                ../../../../../util/src/libaxis2_util.la \
                 ../../../../../axiom/src/om/libaxis2_axiom.la \
                 ../../../../../xml_schema/src/libaxis2_xml_schema.la \
                 ../../../../../woden/src/wsdl20/libwoden.la

INCLUDES = -I$(top_builddir)/include \
            -I ../../../../../util/include \
            -I ../../../../../axiom/include \
            -I ../../../../../xml_schema/include
