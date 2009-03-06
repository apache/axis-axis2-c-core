prgbindir=$(prefix)/samples/bin
prgbin_PROGRAMS = version
noinst_HEADERS = axis2_version_stub.h
version_SOURCES = axis2_version_stub.c \
                version_client.c

version_LDADD   =  \
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

