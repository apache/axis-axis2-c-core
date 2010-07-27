TESTS = test_deployment
noinst_PROGRAMS = test_deployment
SUBDIRS =
AM_CFLAGS = $(CFLAGS) -g
test_deployment_SOURCES = test_deployment.c

test_deployment_LDADD   =  \
                    ../../../util/src/libaxutil.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
		            $(top_builddir)/neethi/src/libneethi.la \
					$(top_builddir)/src/core/engine/libaxis2_engine.la \
					$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la 

INCLUDES = -I ../../../include \
			-I$(CUTEST_HOME)/include \
            -I ../../../src/core/description \
            -I ../../../src/core/context \
            -I ../../../src/core/phaseresolver \
            -I ../../../src/core/deployment \
            -I ../../../src/core/engine \
            -I ../../../src/core/clientapi \
            -I ../../../src/core/util \
            -I ../../../util/include \
            -I ../../../axiom/include \
            -I ../../../neethi/include \
		 	-I ../../../cutest/include
