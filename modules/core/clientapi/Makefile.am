SUBDIRS = $(DICLIENT_DIR) 
noinst_LTLIBRARIES = libaxis2_clientapi.la
libaxis2_clientapi_la_SOURCES = \
                                async_result.c \
                                callback.c \
                                mep_client.c \
                                listener_manager.c \
                                callback_recv.c \
                                msg_sender.c \
                                stub.c \
								options.c \
								op_client.c \
								svc_client.c

noinst_HEADERS = callback_recv.h \
                    listener_manager.h \
                    axis2_msg_sender.h
                    

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/xml_schema/include\
			@AXIOMINC@ \
			@UTILINC@ 

EXTRA_DIST=diclient
