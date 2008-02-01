TESTS = rand_test
check_PROGRAMS = rand_test
noinst_PROGRAMS = rand_test
rand_test_SOURCES = rand_test.c ../util/create_env.c

rand_test_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../axiom/include \
			-I ../../../include


