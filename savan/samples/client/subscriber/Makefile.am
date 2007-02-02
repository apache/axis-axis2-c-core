prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = subscriber 
samplesdir=$(prefix)/samples/client/subscriber
subscriber_SOURCES = subscriber.c

subscriber_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-L../../../src/core \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)\
					-lmod_savan

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@ \
			-I../../../include
