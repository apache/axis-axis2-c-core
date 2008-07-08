prgbindir = $(prefix)/bin/samples/amqp

prgbin_PROGRAMS = echo_blocking echo_non_blocking

echo_blocking_SOURCES = echo_blocking.c echo_util.c
echo_non_blocking_SOURCES = echo_non_blocking.c echo_util.c

LINK_FLAGS = $(LDFLAGS) \
             -L$(AXIS2C_HOME)/lib \
			 -laxutil \
             -laxis2_axiom \
             -laxis2_engine \
             -laxis2_parser \
             -lpthread \
             $(GUTHTHILA_LIBS) \
             $(LIBXML2_LIBS)

echo_blocking_LDADD = $(LINK_FLAGS)
echo_non_blocking_LDADD = $(LINK_FLAGS)

INCLUDES = @AXIS2INC@
prgbindir = $(prefix)/bin/samples/amqp

prgbin_PROGRAMS = echo_blocking echo_non_blocking

echo_blocking_SOURCES = echo_blocking.c echo_util.c
echo_non_blocking_SOURCES = echo_non_blocking.c echo_util.c

LINK_FLAGS = $(LDFLAGS) \
             -L$(AXIS2C_HOME)/lib \
			 -laxutil \
             -laxis2_axiom \
             -laxis2_engine \
             -laxis2_parser \
             -lpthread \
             $(GUTHTHILA_LIBS) \
             $(LIBXML2_LIBS)

echo_blocking_LDADD = $(LINK_FLAGS)
echo_non_blocking_LDADD = $(LINK_FLAGS)

INCLUDES = @AXIS2INC@
EXTRA_DIST=echo_util.h

