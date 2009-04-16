prgbindir=$(prefix)/samples/bin
prgbin_PROGRAMS = mtom_callback

mtom_callback_SOURCES = mtom_callback_client.c

mtom_callback_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-laxutil \
                    -laxis2_axiom \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS)

INCLUDES = @AXIS2INC@

EXTRA_DIST=mtom_callback.mk
