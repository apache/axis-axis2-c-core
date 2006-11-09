TESTS = test_oxs
bin_PROGRAMS = test_oxs

#AM_CFLAGS = -g -O2 -pthread

test_oxs_SOURCES =  test.c

INCLUDES = -I$(top_builddir)/include \
		    -I ../../../include \
            -I ../../../util/include \
            -I ../../../axiom/include

test_oxs_LDADD   = $(top_builddir)/src/util/librampart_util.la \
                    $(top_builddir)/src/omxmlsec/libomxmlsec.la \
                    $(top_builddir)/src/omxmlsec/openssl/libomopenssl.la \
		   -laxis2_util \
		   -laxis2_axiom\
           -laxis2_libxml2\
           -lxml2
