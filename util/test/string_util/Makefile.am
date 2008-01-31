prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = string_util_test 
string_util_test_SOURCES = string_util_test.c ../util/create_env.c

string_util_test_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../axiom/include \
			-I ../../../include


