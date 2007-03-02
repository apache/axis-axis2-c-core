TESTS = test 
bin_PROGRAMS = test
SUBDIRS =

AM_CFLAGS = -g  -pthread

#test_SOURCES = des_test.c
test_SOURCES = test.c


INCLUDES = -I$(top_builddir)/include \
            -I ../../../../include \
            -I ../../../../util/include \
            -I ../../../../axiom/include

test_LDADD   = $(top_builddir)/src/util/librampart_util.la \
                    $(top_builddir)/src/omxmlsec/libomxmlsec.la \
                    $(top_builddir)/src/omxmlsec/openssl/libomopenssl.la \
                    $(top_builddir)/src/omxmlsec/tokens/liboxstokens.la \
                    ../../../../util/src/libaxis2_util.la \
                    ../../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../../axiom/src/parser/libxml2/libaxis2_libxml2.la \
           -lxml2
