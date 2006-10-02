SUBDIRS = util wsdl templates typemappers schema

templatesdir=$(prefix)/conf
typemappersdir=$(prefix)/conf

prgbindir=$(prefix)/bin
prgbin_PROGRAMS = w2c

w2c_SOURCES =  wsdl2code.c
w2c_LDADD =  \
             $(top_builddir)/src/wsdl/libw2c_codegen.la \
             -L$(AXIS2C_HOME)/lib \
             -lw2c_codegen \
             -lxml2 \
             -lxslt
			../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
             ../../../util/src/libaxis2_util.la \
             ../../../axiom/src/om/libaxis2_axiom.la \
             ../../../xml_schema/src/libaxis2_xml_schema.la



INCLUDES = -I$(top_builddir)/include \
			-I ../../../util/include \
			-I ../../../axiom/include \
			-I ../../../xml_schema/include \
			-I ../../../include

#install:install-recursive
#	cp templates $(templatesdir) -rf
#	cp typemappers $(typemappersdir) -rf
