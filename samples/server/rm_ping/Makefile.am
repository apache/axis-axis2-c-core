prglibdir=$(prefix)/services/rm_ping
samplesdir=$(prefix)/samples/server/rm_ping
prglib_LTLIBRARIES = librm_ping.la
samples_DATA=rm_ping.c rm_ping_skeleton.c rm_ping.h services.xml Makefile.am Makefile.in
prglib_DATA= services.xml
EXTRA_DIST = services.xml
noinst_HEADERS = rm_ping.h
SUBDIRS =
librm_ping_la_SOURCES = rm_ping.c rm_ping_skeleton.c
librm_ping_la_LIBADD  =
INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
