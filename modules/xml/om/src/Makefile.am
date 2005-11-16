lib_LTLIBRARIES = libaxis2_om.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_om_la_SOURCES = axis2_om_attribute.c \
                        axis2_om_document.c \
                        axis2_om_node.c \
                        axis2_om_text.c \
                        axis2_om_comment.c \
                        axis2_om_element.c \
                        axis2_om_output.c \
                        axis2_om_doctype.c \
                        axis2_om_namespace.c \
                        axis2_om_processing_instruction.c \
                        axis2_om_stax_builder.c 
                        

libaxis2_om_la_LIBADD = $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/xml/parser
