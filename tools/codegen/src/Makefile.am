SUBDIRS = util wsdl

templatesdir=$(prefix)/conf
typemappersdir=$(prefix)/conf

prgbindir=$(prefix)/bin
prgbin_PROGRAMS = w2c

w2c_SOURCES =  wsdl2code.c
w2c_LDADD =  \
             $(top_builddir)/src/wsdl/libw2c_codegen.la \
             -L$(AXIS2C_HOME)/lib \
             -lw2c_codegen \
             -laxis2_util \
             -laxis2_axiom \
             -laxis2_parser \
             -lxml2 \
             -lxslt


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @AXIOMINC@

install:install-recursive
	cp templates $(templatesdir) -rf
	cp typemappers $(typemappersdir) -rf
