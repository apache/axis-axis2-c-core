lib_LTLIBRARIES = libaxis2_deployment.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_deployment_la_SOURCES = phases_info.c \
                                    desc_builder.c \
                                    arch_reader.c \
                                    dep_engine.c \
                                    arch_file_data.c \
                                    ws_info.c \
                                    ws_info_list.c \
                                    svc_builder.c \
                                    svc_grp_builder.c \
                                    module_builder.c \
                                    conf_builder.c \
                                    repos_listener.c \
                                    conf_init.c

libaxis2_deployment_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms

noinst_HEADERS = axis2_deployment.h
