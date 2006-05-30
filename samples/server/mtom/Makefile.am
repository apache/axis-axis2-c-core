prglibdir=$(prefix)/services/mtom
samplesdir=$(prefix)/samples/server/mtom
prglib_LTLIBRARIES = libmtom.la
samples_DATA=mtom.c mtom_skeleton.c mtom.h services.xml Makefile.am Makefile.in
prglib_DATA= services.xml
EXTRA_DIST = services.xml
noinst_HEADERS = mtom.h
SUBDIRS =
libmtom_la_SOURCES = mtom.c mtom_skeleton.c
libmtom_la_LIBADD  =
INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
