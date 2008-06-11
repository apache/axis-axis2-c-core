TESTS = test_client test_clientapi test_svc_client_handler_count
noinst_PROGRAMS = test_client test_clientapi test_svc_client_handler_count
check_PROGRAMS = test_client test_clientapi test_svc_client_handler_count
SUBDIRS =
AM_CFLAGS = -g -pthread 
test_client_SOURCES = test_client.c
test_clientapi_SOURCES = test_clientapi.c
test_svc_client_handler_count_SOURCES = test_svc_client_handler_count.c

test_clientapi_LDADD   =  \
                    ../../../util/src/libaxutil.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					$(top_builddir)/neethi/src/libneethi.la \
                    -lpthread \
                    $(top_builddir)/src/core/engine/libaxis2_engine.la \
                    $(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la

test_client_LDADD   =  \
                    ../../../util/src/libaxutil.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
			$(top_builddir)/neethi/src/libneethi.la \
                    -lpthread \
                    $(top_builddir)/src/core/engine/libaxis2_engine.la \
                    $(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la

test_svc_client_handler_count_LDADD   =  \
					../../../util/src/libaxutil.la \
					../../../axiom/src/om/libaxis2_axiom.la \
					../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					$(top_builddir)/neethi/src/libneethi.la \
					-lpthread \
					$(top_builddir)/src/core/engine/libaxis2_engine.la \
					$(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la


INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/src/core/description \
            -I$(top_builddir)/src/core/context \
            -I$(top_builddir)/src/core/phaseresolver \
            -I$(top_builddir)/src/core/deployment \
            -I$(top_builddir)/src/core/engine \
            -I$(top_builddir)/src/core/clientapi \
            -I ../../../util/include \
            -I ../../../neethi/include \
            -I ../../../axiom/include

