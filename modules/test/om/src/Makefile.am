bin_PROGRAMS = test_om
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
test_om_SOURCES = test_om.c

test_om_LDADD   =  $(LDFLAGS) ../../../xml/om/src/libom.la ../../../xml/guththila/src/libguththila.la -ldl
INCLUDES = -I${CUTEST_HOME}/include -I${APR_HOME}/include/apr-1 -I../../../xml/guththila/src -I../../../../include
