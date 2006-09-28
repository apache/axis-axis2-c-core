SUBDIRS = writer emitter extensions

prglibdir=$(prefix)/lib
prglib_LTLIBRARIES = libw2c_codegen.la
libw2c_codegen_la_SOURCES = \
   engine_config_loader.c \
   engine_configuration.c \
   engine.c

libw2c_codegen_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
                 -L$(AXIS2C_HOME)/lib \
				 ../../../../util/src/libaxis2_util.la \
				 ../../../../axiom/src/om/libaxis2_axiom.la \
				 ../../../../woden/src/wsdl20/libwoden.la \
				 ../../../../xml_schema/src/libaxis2_xml_schema.la


INCLUDES = -I$(top_builddir)/include \
			-I ../../../../util/include \
			-I ../../../../axiom/include 
