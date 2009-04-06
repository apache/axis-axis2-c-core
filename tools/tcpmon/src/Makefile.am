prgbindir=$(prefix)/bin/tools/tcpmon

prgbin_PROGRAMS = tcpmon

tcpmon_SOURCES =  tcpmon.c \
            entry.c \
            session.c \
            util.c

tcpmon_LDADD =  \
			 ../../../util/src/libaxutil.la \
			 ../../../axiom/src/om/libaxis2_axiom.la \
			 ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la

INCLUDES = -I$(top_builddir)/include \
		   -I ../../../util/include \
		   -I ../../../axiom/include \
		   -I ../../../include \
		   -I ../include

EXTRA_DIST=tcpmon_entry_local.h tcpmon_session_local.h
