lib_LTLIBRARIES = libaxis2_om.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_om_la_SOURCES = om_attribute.c \
                        om_document.c \
                        om_node.c \
                        om_text.c \
                        om_comment.c \
                        om_element.c \
                        om_output.c \
                        om_doctype.c \
                        om_namespace.c \
                        om_processing_instruction.c \
                        om_stax_builder.c \
                        om_children_iterator.c \
                        om_children_qname_iterator.c \
                        om_child_element_iterator.c \
                        om_children_with_specific_attribute_iterator.c

libaxis2_om_la_LIBADD =  ../../util/libaxis2_util.la 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
