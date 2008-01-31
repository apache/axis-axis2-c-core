prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = date_time_test
date_time_test_SOURCES = date_time_test.c ../util/create_env.c

date_time_test_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../axiom/include \
			-I ../../../include


