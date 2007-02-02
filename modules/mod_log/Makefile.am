prglibdir=$(prefix)/modules/logging
prglib_LTLIBRARIES = libaxis2_mod_log.la
prglib_DATA= module.xml
EXTRA_DIST = module.xml mod_log.h
libaxis2_mod_log_la_SOURCES = log_in_handler.c \
                                log_out_handler.c \
                                mod_log.c

libaxis2_mod_log_la_LIBADD =  \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/util/src/libaxis2_util.la \
					$(top_builddir)/axiom/src/om/libaxis2_axiom.la

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/util/include\
            -I$(top_builddir)/axiom/include

