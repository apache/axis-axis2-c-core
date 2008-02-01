TESTS = utils_test
check_PROGRAMS = utils_test 
noinst_PROGRAMS = utils_test 
utils_test_SOURCES = utils_test.c ../util/create_env.c

utils_test_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../axiom/include \
			-I ../../../include


