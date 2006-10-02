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
			 ../../../util/src/libaxis2_util.la \
			 ../../../woden/src/wsdl20/libwoden.la \
			 ../../../axiom/src/om/libaxis2_axiom.la \
			 ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
			 ../../../xml_schema/src/libaxis2_xml_schema.la \
            $(NULL)

prgbin_PROGRAMS = tcpmon

tcpmon_SOURCES =  tcpmon.c
tcpmon_LDADD =  \
             $(top_builddir)/src/libaxis2_tcpmon.la \
			 ../../../util/src/libaxis2_util.la \
			 ../../../woden/src/wsdl20/libwoden.la \
			 ../../../axiom/src/om/libaxis2_axiom.la \
			 ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
			 ../../../xml_schema/src/libaxis2_xml_schema.la \
             $(NULL)


INCLUDES = -I$(top_builddir)/include \
			-I ../../../util/include \
                        $(NULL)

