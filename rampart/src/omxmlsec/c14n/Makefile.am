noinst_LTLIBRARIES=libaxis2_c14n.la

noinst_HEADERS=c14n_sorted_list.h

libaxis2_c14n_la_SOURCES=c14n.c \
					  sorted_list.c

libaxis2_c14n_la_LIBADD=../../../../util/src/libaxis2_util.la \
					../../../../axiom/src/om/libaxis2_axiom.la 
					../../../../axiom/src/parser/libxml2/libaxis2_parser.la 

INCLUDES = 	-I ../../../../include \
            -I ../../../../util/include \
            -I ../../../../rampart/include \
            -I ../../../../axiom/include
