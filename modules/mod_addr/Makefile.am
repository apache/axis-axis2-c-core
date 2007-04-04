prglibdir=$(prefix)/modules/addressing
prglib_LTLIBRARIES = libaxis2_mod_addr.la
prglib_DATA= module.xml
EXTRA_DIST = module.xml
libaxis2_mod_addr_la_SOURCES = addr_in_handler.c \
                                addr_out_handler.c \
                                mod_addr.c

libaxis2_mod_addr_la_LIBADD =	$(top_builddir)/modules/core/engine/libaxis2_engine.la \
							$(top_builddir)/axiom/src/om/libaxis2_axiom.la \
							$(top_builddir)/util/src/libaxutil.la 

lib_LTLIBRARIES = libaxis2.la
libaxis2_la_SOURCES = addr_in_handler.c \
                                addr_out_handler.c \
                                mod_addr.c

libaxis2_la_LIBADD =	$(top_builddir)/modules/core/engine/libaxis2_engine.la \
			$(top_builddir)/axiom/src/om/libaxis2_axiom.la \
			$(top_builddir)/axiom/src/parser/${WRAPPER_DIR}/libaxis2_parser.la



INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/util/include \
            -I$(top_builddir)/axiom/include
