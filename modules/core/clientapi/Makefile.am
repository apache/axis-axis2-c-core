lib_LTLIBRARIES = libaxis2_clientapi.la
AM_CPPFLAGS = $(CPPFLAGS)

libaxis2_clientapi_la_SOURCES = async_result.c \
                                callback.c \
                                mep_client.c \
                                listener_manager.c \
                                callback_recv.c \
                                call.c \
                                stub.c

libaxis2_clientapi_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser
