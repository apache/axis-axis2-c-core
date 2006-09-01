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
                 -laxis2_util \
                 -laxis2_axiom


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @AXIOMINC@
