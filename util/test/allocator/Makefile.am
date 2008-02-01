TESTS = allocator_test
check_PROGRAMS = allocator_test
noinst_PROGRAMS = allocator_test
allocator_test_SOURCES = allocator_test.c ../util/create_env.c

allocator_test_LDADD   =   \
                    $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			-I ../../../axiom/include \
			-I ../../../include


