prglibdir=$(prefix)/lib/codegen

noinst_LTLIBRARIES = libw2c_emitter_if.la
libw2c_emitter_if_la_SOURCES = emitter.c

libw2c_emitter_if_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
                 -laxis2_util \
                 -laxis2_axiom \
                 -laxis2_xml_schema \
                 -lwoden_wsdl \
                 -lwoden \
                 $(NULL)


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @AXIOMINC@ \
                        @WODENINC@ 

prglib_LTLIBRARIES = \
             libw2c_c_emitter.la 

libw2c_c_emitter_la_SOURCES = c_emitter.c
libw2c_c_emitter_la_LIBADD = libw2c_emitter_if.la

