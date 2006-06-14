#################################################
# Makefile for xml_schema [ msvc]
# you can do 
# nmake all       = bulid the library and tests
# nmake clean  - remove all compiler output files
# nmake install  - install the library and header files
# nmake tests   - build tests
#################################################

!include configure.in
#define folders 

XMLSCHEMASRC = ..\..
XML_SCHEMA_DIST_DIR = ..\xml_schema
XML_SCHEMA = xml_schema


#versioning information
#XML_SCHEMA_DIST_VERSION = $(XML_SCHEMA_MAJOR_VERSION).$(XML_SCHEMA_MINOR_VERSION).$(XML_SCHEMA_MICRO_VERSION)


#compiler options
CC = cl.exe
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT" /D "AXIS2_SVR_MULTI_THREADED" /w /nologo
CFLAGS = $(CFLAGS) /I$(AXIOM_INSTALL_DIR)\include /I$(AXIS2_UTIL_INSTALL_DIR)\include /I$(AXIS2_UTIL_INSTALL_DIR)\include\platforms /I$(XMLSCHEMASRC)\include

#linker options 
LD = link.exe
LDFLAGS = /nologo /LIBPATH:$(AXIOM_INSTALL_DIR)\lib /LIBPATH:$(AXIS2_UTIL_INSTALL_DIR)\lib
LIBS = axis2_util.lib axiom.lib axis2_parser.lib


#debug symbols
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Z7
LDFLAGS = $(LDFLAGS) /DEBUG /NODEFAULTLIB:LIBCMTD.lib
!else
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 
LDFLAGS = $(LDFLAGS)
!endif

#create the director structure

distdir:
	if not exist $(XML_SCHEMA_DIST_DIR) mkdir $(XML_SCHEMA_DIST_DIR)
	if not exist $(XML_SCHEMA_DIST_DIR)\bin mkdir $(XML_SCHEMA_DIST_DIR)\bin
	if not exist $(XML_SCHEMA_DIST_DIR)\lib mkdir $(XML_SCHEMA_DIST_DIR)\lib
	if not exist $(XML_SCHEMA_DIST_DIR)\include mkdir $(XML_SCHEMA_DIST_DIR)\include

clean :
	if exist $(XML_SCHEMA_DIST_DIR) rmdir /S /Q $(XML_SCHEMA_DIST_DIR)

copy_include:
	xcopy /E $(XMLSCHEMASRC)\include $(XML_SCHEMA_DIST_DIR)\include

build_xml_schema:
	$(CC) $(CFLAGS) $(XMLSCHEMASRC)\src\*.c /c
	$(LD) $(LDFLAGS) $(LIBS) *.obj /DLL /OUT:$(XML_SCHEMA).dll /IMPLIB:$(XML_SCHEMA).lib
	copy $(XML_SCHEMA).dll $(XML_SCHEMA_DIST_DIR)\bin
	copy $(XML_SCHEMA).lib $(XML_SCHEMA_DIST_DIR)\lib
	del *.obj
	del *.dll
	del *.exp
	del *.lib

install: clean distdir build_xml_schema copy_include
	 


	






	



