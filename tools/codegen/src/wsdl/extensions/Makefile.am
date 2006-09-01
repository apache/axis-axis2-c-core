prglibdir=$(prefix)/lib/codegen

prglib_LTLIBRARIES = \
             libw2c_databinding_default_ext.la \
             libw2c_c_namemaker_ext.la \
             libw2c_c_qname2name_ext.la \
             libw2c_default_qname2name_ext.la \
             libw2c_c_code_indenter_ext.la \
             $(NULL)

libw2c_databinding_default_ext_la_SOURCES = databinding_default_ext.c
libw2c_c_namemaker_ext_la_SOURCES = c_namemaker_ext.c
libw2c_c_qname2name_ext_la_SOURCES = c_qname2name_ext.c
libw2c_default_qname2name_ext_la_SOURCES = default_qname2name_ext.c
libw2c_c_code_indenter_ext_la_SOURCES = c_code_indenter_ext.c

libw2c_databinding_default_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom

libw2c_c_namemaker_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom

libw2c_c_qname2name_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom

libw2c_default_qname2name_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom

libw2c_c_code_indenter_ext_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom

INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @AXIOMINC@
