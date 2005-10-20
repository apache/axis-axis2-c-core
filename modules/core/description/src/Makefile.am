noinst_LTLIBRARIES = libaxis2_description.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_description_la_SOURCES = axis2_description_param_include.c \
                            axis2_description_operation.c \
                            axis2_description_service.c \
                            axis2_description_module.c \
                            axis2_description_parameter.c \
                            axis2_description_servicegroup.c

libaxis2_description_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src  
