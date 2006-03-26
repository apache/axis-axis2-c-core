TESTS =
lib_LTLIBRARIES = libaxis2_om.la
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
                        om_children_with_specific_attribute_iterator.c \
                        om_output_format.c

libaxis2_om_la_LIBADD =  ../../util/libaxis2_util.la ../attachments/libaxis2_attachments.la
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/om\
            -I$(top_builddir)/modules/xml/attachments

EXTRA_DIST=axis2_om_node_internal.h axis2_om_stax_builder_internal.h
