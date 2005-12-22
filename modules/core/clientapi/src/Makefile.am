lib_LTLIBRARIES = libaxis2_clientapi.la
AM_CPPFLAGS = $(CPPFLAGS)

libaxis2_clientapi_la_SOURCES = async_result.c callback.c mep_client.c listener_manager.c callback_recv.c

libaxis2_clientapi_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/core/clientapi/src
