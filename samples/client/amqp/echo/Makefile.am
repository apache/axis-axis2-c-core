prgbindir = $(prefix)/samples/bin/amqp

prgbin_PROGRAMS = echo_blocking \
				  echo_non_blocking \
				  echo_blocking_addr \
				  echo_blocking_dual \
				  echo_non_blocking_dual \
				  echo_blocking_soap11


echo_blocking_SOURCES = echo_blocking.c echo_util.c
echo_non_blocking_SOURCES = echo_non_blocking.c echo_util.c
echo_blocking_addr_SOURCES = echo_blocking_addr.c echo_util.c
echo_blocking_dual_SOURCES = echo_blocking_dual.c echo_util.c
echo_non_blocking_dual_SOURCES = echo_non_blocking_dual.c echo_util.c
echo_blocking_soap11_SOURCES = echo_blocking_soap11.c echo_util.c

LINK_FLAGS = $(LDFLAGS) \
             -L$(AXIS2C_HOME)/lib \
			 -laxutil \
             -laxis2_axiom \
             -laxis2_engine \
             -laxis2_parser \
             -lpthread \
             $(GUTHTHILA_LIBS)

echo_blocking_LDADD = $(LINK_FLAGS)
echo_non_blocking_LDADD = $(LINK_FLAGS)
echo_blocking_addr_LDADD = $(LINK_FLAGS)
echo_blocking_dual_LDADD = $(LINK_FLAGS)
echo_non_blocking_dual_LDADD = $(LINK_FLAGS)
echo_blocking_soap11_LDADD = $(LINK_FLAGS)

INCLUDES = @AXIS2INC@

EXTRA_DIST=echo_util.h
