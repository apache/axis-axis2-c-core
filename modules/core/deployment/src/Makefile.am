lib_LTLIBRARIES = libaxis2_deployment.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_deployment_la_SOURCES = phases_info.c \
                                    desc_builder.c \
                                    dep_engine.c \
                                    arch_reader.c \
                                    arch_file_data.c

libaxis2_deployment_la_LIBADD = 
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/wsdl/src \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/core/deployment/src \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
