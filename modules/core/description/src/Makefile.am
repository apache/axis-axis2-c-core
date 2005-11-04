lib_LTLIBRARIES = libaxis2_description.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_description_la_SOURCES = param_container.c \
                            operation.c \
                            service.c \
                            module.c \
                            param.c \
                            service_group.c \
                            phase_rule.c

libaxis2_description_la_LIBADD = $(LDFLAGS)
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/modules/util/src
