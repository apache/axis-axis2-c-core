TESTS = test 
bin_PROGRAMS = test
#SUBDIRS = rsa

AM_CFLAGS = -g -O2 -pthread

#test_SOURCES = des_test.c
test_SOURCES = block_cipher_test.c

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
            @AXIOMINC@

test_LDADD   = $(top_builddir)/src/util/librampart_util.la \
                    $(top_builddir)/src/omxmlsec/libomxmlsec.la \
                    $(top_builddir)/src/omxmlsec/openssl/libomopenssl.la \
		   -laxis2_util \
		   -laxis2_axiom \
            -laxis2_libxml2 \
            -lxml2
