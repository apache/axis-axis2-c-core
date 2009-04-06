lib_LTLIBRARIES = libaxis2_xpath.la
libaxis2_xpath_la_SOURCES = xpath.c \
			xpath_functions.c \
			xpath_internals.c \
			xpath_internals_engine.c \
			xpath_internals_iterators.c \
			xpath_internals_parser.c \
			xpath_streaming.c

libaxis2_xpath_la_LDFLAGS = -version-info $(VERSION_NO)

libaxis2_xpath_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
            -I$(top_builddir)/src/om \
			-I ../../../util/include \
			-I ../../../include

EXTRA_DIST =    xpath_functions.h  xpath_internals_engine.h \
		xpath_internals.h  xpath_internals_iterators.h \
		xpath_internals_parser.h  xpath_streaming.h

