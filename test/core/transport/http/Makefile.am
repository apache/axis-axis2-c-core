TESTS = test_http_server test_http_transport
check_PROGRAMS = test_http_transport test_http_server
noinst_PROGRAMS = test_http_transport test_http_server
SUBDIRS =
AM_CFLAGS = -g -pthread
test_http_transport_SOURCES = test_http_transport.c
test_http_server_SOURCES = test_http_server.c

test_http_transport_LDADD   =  \
							 $(LDFLAGS) \
						../../../../util/src/libaxutil.la \
							../../../../axiom/src/om/libaxis2_axiom.la \
						$(top_builddir)/neethi/src/libneethi.la \
						../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
						$(top_builddir)/src/core/engine/libaxis2_engine.la \
			 $(top_builddir)/src/core/transport/http/receiver/libaxis2_http_receiver.la \
						$(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la 

test_http_server_LDADD   =  \
							 $(LDFLAGS) \
				 $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
				 $(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
			 $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
			 $(top_builddir)/util/src/libaxutil.la \
			 $(top_builddir)/axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
						  $(top_builddir)/src/core/description/libaxis2_description.la \
			 $(top_builddir)/src/core/deployment/libaxis2_deployment.la \
			 $(top_builddir)/src/core/context/libaxis2_context.la \
			 $(top_builddir)/src/core/clientapi/libaxis2_clientapi.la \
			 $(top_builddir)/src/core/engine/libaxis2_engine.la \
			 $(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la \
			 $(top_builddir)/src/core/transport/http/receiver/libaxis2_http_receiver.la \
			 $(top_builddir)/neethi/src/libneethi.la 

INCLUDES = -I$(CUTEST_HOME)/include \
		 -I$(top_builddir)/include \
		 -I ../../../../util/include \
		 -I ../../../../axiom/include \
		 -I ../../../../cutest/include

