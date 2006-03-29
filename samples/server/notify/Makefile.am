prglibdir=$(prefix)/services/notify
samplesdir=$(prefix)/samples/server/notify
prglib_LTLIBRARIES = libnotify.la
samples_DATA=notify.c notify_skeleton.c notify.h services.xml Makefile.am
prglib_DATA= services.xml
EXTRA_DIST = services.xml
noinst_HEADERS = notify.h
SUBDIRS =
libnotify_la_SOURCES = notify.c notify_skeleton.c
libnotify_la_LIBADD  =
INCLUDES = -I$(AXIS2C_HOME)/include \
            -I$(AXIS2C_HOME)/platforms
