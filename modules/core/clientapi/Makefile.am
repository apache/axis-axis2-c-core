SUBDIRS = $(DICLIENT_DIR) 
noinst_LTLIBRARIES = libaxis2_clientapi.la
libaxis2_clientapi_la_SOURCES = \
                                async_result.c \
                                callback.c \
                                listener_manager.c \
                                callback_recv.c \
								mep_client.c \
                                stub.c \
								options.c \
								op_client.c \
								svc_client.c

noinst_HEADERS = callback_recv.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/xml_schema/include\
			@AXIOMINC@ \
			@UTILINC@ 

EXTRA_DIST=diclient
