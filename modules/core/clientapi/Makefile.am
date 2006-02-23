TESTS =
noinst_LTLIBRARIES = libaxis2_clientapi.la
libaxis2_clientapi_la_SOURCES = \
                                async_result.c \
                                callback.c \
                                mep_client.c \
                                listener_manager.c \
                                callback_recv.c \
                                call.c \
                                msg_sender.c \
                                stub.c

noinst_HEADERS = callback_recv.h \
                    listener_manager.h \
                    axis2_msg_sender.h
                    

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
