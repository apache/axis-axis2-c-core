SUBDIRS= compiler writer 

prglibdir=$(prefix)/lib/codegen
prgbindir=$(prefix)/bin/tools

prgbin_PROGRAMS = x2c

x2c_SOURCES =  xsd2code.c
x2c_LDADD =  \
             $(top_builddir)/src/wsdl/libw2c_codegen.la \
             $(top_builddir)/src/schema/compiler/libw2c_schema.la \
             ../../../../util/src/libaxis2_util.la \
             ../../../../axiom/src/om/libaxis2_axiom.la \
             ../../../../xml_schema/src/libaxis2_xml_schema.la \
             ../../../../woden/src/wsdl20/libwoden.la \
			../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
             -lxml2 \
             -lxslt \
			 $(NULL)

prglib_LTLIBRARIES = \
             libw2c_databinding_adb_ext.la

libw2c_databinding_adb_ext_la_SOURCES = databinding_adb_ext.c

libw2c_databinding_adb_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 $(top_builddir)/src/schema/compiler/libw2c_schema.la \
               	../../../../util/src/libaxis2_util.la \
                 ../../../../axiom/src/om/libaxis2_axiom.la \
                 ../../../../xml_schema/src/libaxis2_xml_schema.la \
                 ../../../../woden/src/wsdl20/libwoden.la \
                 ../../../../woden/src/wsdl/libwoden_wsdl.la


INCLUDES = -I$(top_builddir)/include \
            -I ../../../../util/include \
            -I ../../../../axiom/include \
            -I ../../../../xml_schema/include \
            -I ../../../../woden/include \
            -I ../../../../include
