prgbindir = $(bindir)

prgbin_PROGRAMS = axis2_amqp_server

AM_CFLAGS = -g -pthread

axis2_amqp_server_SOURCES = axis2_amqp_server.c

axis2_amqp_server_LDADD = $(LDFLAGS) \
			     		  $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
						  $(top_builddir)/util/src/libaxutil.la \
						  $(top_builddir)/axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
						  $(top_builddir)/src/core/description/libaxis2_description.la \
						  $(top_builddir)/src/core/deployment/libaxis2_deployment.la \
						  $(top_builddir)/src/core/context/libaxis2_context.la \
						  $(top_builddir)/src/core/clientapi/libaxis2_clientapi.la \
						  $(top_builddir)/src/core/engine/libaxis2_engine.la \
						  $(top_builddir)/neethi/src/libneethi.la \
						  $(top_builddir)/src/core/transport/amqp/receiver/libaxis2_amqp_receiver.la \
						  $(top_builddir)/src/core/transport/amqp/util/libaxis2_amqp_util.la \
						  -lpthread
                    
INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/src/core/description \
           -I$(top_builddir)/src/core/context \
           -I$(top_builddir)/src/core/deploymenti \
           -I$(top_builddir)/src/core/phaseresolver \
           -I$(top_builddir)/src/core/engine \
           -I$(top_builddir)/src/core/transport/amqp/receiver \
           -I$(top_builddir)/src/core/transport/amqp/receiver/qpid_receiver \
           -I$(top_builddir)/src/core/transport/amqp/util \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include
