prglibdir=$(prefix)/services/math
prglib_LTLIBRARIES = libmath.la
#prglib_DATA=services.xml
EXTRA_DIST = services.xml
noinst_HEADERS = math.h
SUBDIRS =
libmath_la_SOURCES = math.c math_skeleton.c
libmath_la_LIBADD  =
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include



