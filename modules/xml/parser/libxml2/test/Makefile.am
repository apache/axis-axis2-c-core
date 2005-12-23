prgbindir=$(prefix)/bin/system_test            
prgbin_PROGRAMS = libxml_test
SUBDIRS = 
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread

libxml_test_SOURCES = libxml_test.c

libxml_test_LDADD   =    -L$(top_builddir)/modules/xml/parser/libxml2/src/.libs -laxis2_libxml2 \
                       -L${LIBXML2_LIB} -lxml2 \
                       -L$(top_builddir)/modules/util/src/.libs -laxis2_util
                       
INCLUDES =  -I$(top_builddir)/modules/xml/parser/ \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/include 

