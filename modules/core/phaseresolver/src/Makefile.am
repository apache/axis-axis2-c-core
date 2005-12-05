lib_LTLIBRARIES = libaxis2_phaseresolver.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_phaseresolver_la_SOURCES = phase_holder.c \
                                    phase_resolver.c

libaxis2_phaseresolver_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/wsdl/src \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/core/transport
