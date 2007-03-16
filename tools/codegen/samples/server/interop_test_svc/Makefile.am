prglibdir=$(prefix)/services/interop_test_port_type_doc_service
samplesdir=$(prefix)/samples/server/InteropTestPortTypeDocService
prglib_LTLIBRARIES = libInteropTestPortTypeDocService.la
prglib_DATA= services.xml
EXTRA_DIST = services.xml
SUBDIRS =
libInteropTestPortTypeDocService_la_SOURCES =  \
	axis2_skel_interop_test_port_type_doc_service.c \
    axis2_svc_skel_interop_test_port_type_doc_service.c

libInteropTestPortTypeDocService_la_LIBADD  =
INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
