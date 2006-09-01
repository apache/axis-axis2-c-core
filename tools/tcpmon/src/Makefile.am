prglibdir=$(prefix)/lib
prgbindir=$(prefix)/bin

prglib_LTLIBRARIES = \
             libaxis2_tcpmon.la

libaxis2_tcpmon_la_SOURCES = \
            entry.c \
            session.c \
            util.c \
            $(NULL)

libaxis2_tcpmon_la_LIBADD = \
            -L$(AXIS2C_HOME)/lib \
            -laxis2_util \
            $(NULL)

prgbin_PROGRAMS = tcpmon

tcpmon_SOURCES =  tcpmon.c
tcpmon_LDADD =  \
             $(top_builddir)/src/libaxis2_tcpmon.la \
             -L$(AXIS2C_HOME)/lib \
             -laxis2_util \
             $(NULL)


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@  \
                        $(NULL)

