prglibdir=$(prefix)/lib/codegen

noinst_LTLIBRARIES = libw2c_writer_if.la
libw2c_writer_if_la_SOURCES = writer.c

libw2c_writer_if_la_LIBADD = \
                 $(top_builddir)/src/util/libw2c_util.la \
				 ../../../../../util/src/libaxis2_util.la \
				 ../../../../../axiom/src/om/libaxis2_axiom.la \
				 ../../../../../woden/src/wsdl20/libwoden.la \
				 ../../../../../xml_schema/src/libaxis2_xml_schema.la


INCLUDES = -I$(top_builddir)/include \
			-I ../../../../../util/include \
			-I ../../../../../axiom/include 

prglib_LTLIBRARIES = \
             libw2c_cservice_xml_writer.la \
             libw2c_cskel_header_writer.la \
             libw2c_cskel_source_writer.la \
             libw2c_cstub_header_writer.la \
             libw2c_cstub_source_writer.la \
             libw2c_csvc_skeleton_writer.la

libw2c_cservice_xml_writer_la_SOURCES = cservice_xml_writer.c
libw2c_cservice_xml_writer_la_LIBADD = libw2c_writer_if.la

libw2c_cskel_header_writer_la_SOURCES = cskel_header_writer.c
libw2c_cskel_header_writer_la_LIBADD = libw2c_writer_if.la

libw2c_cskel_source_writer_la_SOURCES = cskel_source_writer.c
libw2c_cskel_source_writer_la_LIBADD = libw2c_writer_if.la

libw2c_cstub_header_writer_la_SOURCES = cstub_header_writer.c
libw2c_cstub_header_writer_la_LIBADD = libw2c_writer_if.la

libw2c_cstub_source_writer_la_SOURCES = cstub_source_writer.c
libw2c_cstub_source_writer_la_LIBADD = libw2c_writer_if.la

libw2c_csvc_skeleton_writer_la_SOURCES = csvc_skeleton_writer.c
libw2c_csvc_skeleton_writer_la_LIBADD = libw2c_writer_if.la
