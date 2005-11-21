prgbindir=$(prefix)/bin/system_test            
prgbin_PROGRAMS = guththila_test
SUBDIRS = 
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread

guththila_test_SOURCES = guththila_test.c

guththila_test_LDADD   =  $(LDFLAGS)  -L$(top_builddir)/modules/xml/parser/guththila/impl/src -lguththila 

INCLUDES =  -I$(top_builddir)/modules/xml/parser/guththila/impl/src
