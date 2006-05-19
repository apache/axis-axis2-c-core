TESTS = test_print
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_print
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_print_SOURCES = test_print.c

test_print_LDADD   =  \
                    -L$(WSDLPULL)/lib \
                    -lschema \
                    -lxmlpull \
                    -lwsdl \
                    -laxis2_util \
                    -laxis2_unix \
                    -lpthread \
                    -laxis2_wsdl4c

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
			@UTILINC@/platforms

