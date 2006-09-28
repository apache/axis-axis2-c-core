TESTS =
prglibdir=$(prefix)/rampart
prglib_LTLIBRARIES = libmod_rampart.la
prglib_DATA= ../data/module.xml

libmod_rampart_la_SOURCES = mod_rampart.c

libmod_rampart_la_LIBADD  = ../handlers/librampart_handlers.la \
			    ../util/librampart_util.la \
			    ../omxmlsec/libomxmlsec.la \
                ../omxmlsec/openssl/libomopenssl.la

INCLUDES = -I$(top_builddir)/include \
			-I ../../../include \
			-I ../../../util/include \
			-I ../../../axiom/include \
			-I ../../../xml_schema/include 
