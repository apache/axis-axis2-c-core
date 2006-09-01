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
                      -L$(AXIS2C_HOME)/lib \
                      -lxml2 -lxslt \
                      -laxis2_util \
                      -laxis2_axiom \
                      -laxis2_parser \
                      $(NULL)


INCLUDES = -I$(top_builddir)/include \
                        @UTILINC@ \
                        @AXIOMINC@
