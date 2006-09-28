samplesdir=$(prefix)/conf
samples_DATA= codegen-config.properties resource.properties

noinst_LTLIBRARIES = libw2c_util.la
libw2c_util_la_SOURCES = \
   string.c  \
   url_processor.c  \
   xslt_utils.c \
   file_writer.c \
   xslt_template_processor.c \
   properties.c \
   cmdline_option.c \
   cmdline_option_parser.c \
   cmdline_option_validator.c \
   config_property_loader.c \
   messages.c \
   class_loader.c \
   typemapper.c \
   $(NULL)


libw2c_util_la_LIBADD =  \
                      -lxml2 -lxslt \
					  ../../../../util/src/libaxis2_util.la \
					  ../../../../woden/src/wsdl20/libwoden.la \
					  ../../../../axiom/src/om/libaxis2_axiom.la \
					  ../../../../axiom/src/parser/$(PARSER_DIR)/libaxis2_parser.la \
                      $(NULL)


INCLUDES = -I$(top_builddir)/include \
			-I ../../../../util/include \
			-I ../../../../axiom/include 
