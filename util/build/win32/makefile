#################################################
# Makefile for axis2 utils [ msvc]
# you can do 
# nmake all       = bulid the library and tests
# nmake clean  - remove all compiler output files
# nmake install  - install the library and header files
# nmake tests   - build tests
#################################################

!include configure.in
#define folders 

UTILSRC = ..\..
UTIL_DIST_DIR= ..\axis2_util
AXIS2_UTIL = axis2_util


#versioning information
#AXIS2_UTIL_DIST_VERSION = $(AXIS2_UTIL_MAJOR_VERSION).$(AXIS2_UTIL_MINOR_VERSION).$(AXIS2_UTIL_MICRO_VERSION)

#compiler options
CC = cl.exe
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT" /D "AXIS2_SVR_MULTI_THREADED" /w /nologo
CFLAGS = $(CFLAGS) /I$(UTILSRC)\include /I$(UTILSRC)\include\platforms /I$(UTILSRC)\src /I$(UTILSRC)\src\minizip /I$(ZLIB_INSTALL_DIR)\include


#linker options 
LD = link.exe
LDFLAGS = /nologo /LIBPATH:$(ZLIB_INSTALL_DIR)\lib
LIBS = Rpcrt4.lib Ws2_32.lib zdll.lib

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
	if not exist $(UTIL_DIST_DIR) mkdir $(UTIL_DIST_DIR)
	if not exist $(UTIL_DIST_DIR)\bin mkdir $(UTIL_DIST_DIR)\bin
	if not exist $(UTIL_DIST_DIR)\lib mkdir $(UTIL_DIST_DIR)\lib
	if not exist $(UTIL_DIST_DIR)\include mkdir $(UTIL_DIST_DIR)\include
	if not exist $(UTIL_DIST_DIR)\tests mkdir $(UTIL_DIST_DIR)\tests
 	if not exist $(UTIL_DIST_DIR)\tests\unit mkdir $(UTIL_DIST_DIR)\tests\unit

clean :
	if exist $(UTIL_DIST_DIR) rmdir /S /Q $(UTIL_DIST_DIR)

copy_include:
	xcopy /E $(UTILSRC)\include $(UTIL_DIST_DIR)\include


build_util:
	$(CC) $(CFLAGS) $(UTILSRC)\src\platforms\windows\*.c $(UTILSRC)\src\minizip\*.c $(UTILSRC)\src\*.c /c
	$(LD) $(LDFLAGS) $(LIBS) *.obj /DLL /OUT:$(AXIS2_UTIL).dll /IMPLIB:$(AXIS2_UTIL).lib
	copy $(AXIS2_UTIL).dll $(UTIL_DIST_DIR)\bin
	copy $(AXIS2_UTIL).lib $(UTIL_DIST_DIR)\lib
	del *.obj
	del *.dll
	del *.obj
	del *.exp
	del *.lib

install : clean distdir build_util copy_include

all: install
		 


	






	



