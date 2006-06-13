TESTS = test_sha
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_sha
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_sha_SOURCES = test_sha.c

test_sha_LDADD   = $(top_builddir)/src/util/librampart_util.la\
		   -laxis2_util
		   

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ 

