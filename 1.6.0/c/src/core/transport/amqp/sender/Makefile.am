SUBDIRS = qpid_sender

lib_LTLIBRARIES = libaxis2_amqp_sender.la

libaxis2_amqp_sender_la_SOURCES = axis2_amqp_sender.c

libaxis2_amqp_sender_la_LIBADD = $(top_builddir)/util/src/libaxutil.la \
								 $(top_builddir)/src/core/transport/amqp/util/libaxis2_amqp_util.la \
								 $(top_builddir)/src/core/transport/amqp/sender/qpid_sender/libaxis2_qpid_sender.la

libaxis2_amqp_sender_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/src/core/transport/amqp/util \
	       -I$(top_builddir)/src/core/transport/amqp/receiver \
	       -I$(top_builddir)/src/core/transport/amqp/receiver/qpid_receiver \
	       -I$(top_builddir)/src/core/transport/amqp/sender \
	       -I$(top_builddir)/src/core/transport/amqp/sender/qpid_sender \
           -I$(top_builddir)/src/core/description \
           -I$(top_builddir)/src/core/context \
           -I$(top_builddir)/src/core/phaseresolver \
           -I$(top_builddir)/src/core/engine \
           -I$(top_builddir)/src/core/deployment \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include

