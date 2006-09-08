prglibdir=$(prefix)/services/rm_echo
samplesdir=$(prefix)/samples/server/rm/rm_echo
prglib_LTLIBRARIES = librm_echo.la
samples_DATA=rm_echo.c rm_echo_skeleton.c rm_echo.h services.xml Makefile.am Makefile.in
prglib_DATA= services.xml
EXTRA_DIST = services.xml
noinst_HEADERS = rm_echo.h
SUBDIRS =
librm_echo_la_SOURCES = rm_echo.c rm_echo_skeleton.c
librm_echo_la_LIBADD  =
INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
