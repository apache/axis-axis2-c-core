lib_LTLIBRARIES = libaxis2_axiom.la
libaxis2_axiom_la_SOURCES = om_attribute.c \
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
                        om_navigator.c

libaxis2_axiom_la_LIBADD =  \
                    $(top_builddir)/src/soap/libaxis2_soap.la \
					$(top_builddir)/src/attachments/libaxis2_attachments.la

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
            -I$(top_builddir)/src/om \
            -I$(top_builddir)/src/attachments \
			@UTILINC@

EXTRA_DIST=axis2_om_node_internal.h axis2_om_stax_builder_internal.h axis2_om_namespace_internal.h \
			maven.xml project.xml
