lib_LTLIBRARIES = libaxis2_guththila.la libaxis2_parser.la
SUBDIRS = 
libaxis2_guththila_la_SOURCES = guththila_xml_writer_wrapper.c \
                                guththila_xml_reader_wrapper.c 

libaxis2_guththila_la_LIBADD = -lguththila

libaxis2_parser_la_SOURCES = guththila_xml_writer_wrapper.c \
                                guththila_xml_reader_wrapper.c 

libaxis2_parser_la_LIBADD = -lguththila

INCLUDES = -I$(top_builddir)/include \
			@UTILINC@ \
            -I${GUTHTHILA_INCLUDES}
           
