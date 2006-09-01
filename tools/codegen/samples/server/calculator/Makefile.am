prglibdir=$(prefix)/services/calculator
samplesdir=$(prefix)/samples/server/calculator
prglib_LTLIBRARIES = libcalculator.la
prglib_DATA= services.xml
EXTRA_DIST = services.xml
SUBDIRS =
libcalculator_la_SOURCES =  \
	axis2_skel_calculator.c \
    axis2_svc_skel_calculator.c

libcalculator_la_LIBADD  =
INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
