prgbindir = $(prefix)/samples/bin/amqp

prgbin_PROGRAMS = notify

notify_SOURCES = notify_client.c

notify_LDADD =  -L$(AXIS2C_HOME)/lib \
				-laxutil \
                -laxis2_axiom \
                -laxis2_engine \
                -laxis2_parser \
                $(GUTHTHILA_LIBS)

INCLUDES = @AXIS2INC@
