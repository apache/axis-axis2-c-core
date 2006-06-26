prglibdir=$(prefix)/modules/addressing
prglib_LTLIBRARIES = libaxis2_mod_addr.la
prglib_DATA= module.xml
EXTRA_DIST = module.xml
libaxis2_mod_addr_la_SOURCES = addr_in_handler.c \
                                addr_out_handler.c \
                                mod_addr.c

libaxis2_mod_addr_la_LIBADD =  \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/xml_schema/include\
			@AXIOMINC@ \
			@UTILINC@ 
