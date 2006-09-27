SUBDIRS= compiler writer 

prglibdir=$(prefix)/lib/codegen
prgbindir=$(prefix)/bin

prgbin_PROGRAMS = x2c

x2c_SOURCES =  xsd2code.c
x2c_LDADD =  \
             $(top_builddir)/src/wsdl/libw2c_codegen.la \
             $(top_builddir)/src/schema/compiler/libw2c_schema.la \
             -L$(AXIS2C_HOME)/lib \
             -laxis2_util \
             -laxis2_axiom \
             -laxis2_parser \
			 -laxis2_xml_schema \
             -lxml2 \
             -lxslt \
			 $(NULL)

prglib_LTLIBRARIES = \
             libw2c_databinding_adb_ext.la

libw2c_databinding_adb_ext_la_SOURCES = databinding_adb_ext.c

libw2c_databinding_adb_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 $(top_builddir)/src/schema/compiler/libw2c_schema.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom \
                 -lwoden_wsdl \
                 -lwoden \
                 -laxis2_xml_schema


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @XMLSCHEMAINC@ \
                        @AXIOMINC@
