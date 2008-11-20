lib_LTLIBRARIES = libaxis2_amqp_request_processor.la

libaxis2_amqp_request_processor_la_SOURCES = axis2_amqp_request_processor.c

libaxis2_amqp_request_processor_la_LIBADD = $(top_builddir)/util/src/libaxutil.la \
											$(top_builddir)/src/core/transport/amqp/util/libaxis2_amqp_util.la

libaxis2_amqp_request_processor_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/src/core/transport/amqp/receiver/qpid_receiver/request_processor \
	       -I$(top_builddir)/src/core/transport/amqp/util \
           -I$(top_builddir)/src/core/description \
           -I$(top_builddir)/src/core/context \
           -I$(top_builddir)/src/core/phaseresolver \
           -I$(top_builddir)/src/core/engine \
           -I$(top_builddir)/src/core/deployment \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include

