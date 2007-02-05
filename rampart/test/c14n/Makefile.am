TESTS = test_c14n
noinst_PROGRAMS = test_c14n

#AM_CFLAGS = -g -O2 -pthread

test_c14n_SOURCES =  test.c

INCLUDES = -I$(top_builddir)/include \
		    -I ../../../include \
            -I ../../../util/include \
            -I ../../../axiom/include

test_c14n_LDADD   =	../../../util/src/libaxis2_util.la \
					../../../axiom/src/om/libaxis2_axiom.la \
					../../../axiom/src/parser/libxml2/libaxis2_libxml2.la \
					../../../rampart/src/omxmlsec/c14n/libaxis2_c14n.la 
