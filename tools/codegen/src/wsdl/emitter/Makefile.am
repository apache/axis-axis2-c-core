prglibdir=$(prefix)/lib/codegen

noinst_LTLIBRARIES = libw2c_emitter_if.la
libw2c_emitter_if_la_SOURCES = emitter.c

libw2c_emitter_if_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
				 ../../../../../util/src/libaxis2_util.la \
				 ../../../../../axiom/src/om/libaxis2_axiom.la \
				 ../../../../../xml_schema/src/libaxis2_xml_schema.la \
				 ../../../../../woden/src/wsdl20/libwoden.la \
				 ../../../../../woden/src/wsdl/libwoden_wsdl.la \
                 $(NULL)


INCLUDES = -I$(top_builddir)/include \
			-I ../../../../../util/include \
			-I ../../../../../axiom/include \
			-I ../../../../../woden/include 

prglib_LTLIBRARIES = \
             libw2c_c_emitter.la 

libw2c_c_emitter_la_SOURCES = c_emitter.c
libw2c_c_emitter_la_LIBADD = libw2c_emitter_if.la

