TESTS = 
noinst_PROGRAMS = test

AM_CFLAGS = -g -O2 -pthread

test_SOURCES = test.c

INCLUDES = -I$(top_builddir)/include \
			-I ../../util/include \
			-I ../../axiom/include \
		    -I ../../include

test_LDADD = $(top_builddir)/src/libneethi.la \
			../../axiom/src/om/libaxis2_axiom.la \
			../../util/src/libaxutil.la \
			../src/libneethi.la
