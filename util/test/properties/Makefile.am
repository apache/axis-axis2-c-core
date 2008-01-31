prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = property_test 
property_test_SOURCES = property_test.c ../util/create_env.c

property_test_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../axiom/include \
			-I ../../../include


