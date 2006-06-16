#################################################
# Makefile for rampart module [ msvc]
# you can do 
# nmake all       = bulid the library and tests
# nmake clean  - remove all compiler output files
# nmake install  - install the library and header files
# nmake tests   - build tests
#################################################

!include configure.in
#define folders 

RAMPARTSRC = ..\..
RAMPART_DIST_DIR = ..\rampart
RAMPART = mod_rampart


#compiler options
CC = cl.exe
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT" /w /nologo
CFLAGS = $(CFLAGS) /I$(AXIS2_INSTALL_DIR)\include /I$(OPENSSL_INSTALL_DIR)\include

#linker options 
LD = link.exe
LDFLAGS = /nologo /LIBPATH:$(AXIS2_INSTALL_DIR)\lib /LIBPATH:$(OPENSSL_INSTALL_DIR)\lib\VC
LIBS = axis2_util.lib axis2_parser.lib axiom.lib axis2_engine.lib libeay32MT.lib ssleay32MT.lib


#debug symbols
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Z7
LDFLAGS = $(LDFLAGS) /DEBUG 
!else
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 
LDFLAGS = $(LDFLAGS)
!endif



MOD_RAMPART_SRC = $(RAMPARTSRC)\src\core\*.c \
		$(RAMPARTSRC)\src\handlers\*.c \
		$(RAMPARTSRC)\src\util\*.c


MOD_RAMPART_INCLUDE_PATH = $(RAMPARTSRC)\include


mod_rampart:
	$(CC) $(CFLAGS) /I$(MOD_RAMPART_INCLUDE_PATH) $(MOD_RAMPART_SRC) /c 
	$(LD) $(LDFLAGS) *.obj $(LIBS) /DLL /OUT:mod_rampart.dll /IMPLIB:mod_rampart.lib
	del *.obj
	if not exist $(RAMPART_DIST_DIR)\rampart mkdir $(RAMPART_DIST_DIR)\rampart

	copy mod_rampart.dll $(RAMPART_DIST_DIR)\rampart
	
	del *.dll
	del *.lib
	del *.exp
	copy $(RAMPARTSRC)\src\data\module.xml $(RAMPART_DIST_DIR)\rampart
	
mod_rampart_pwcb: 
	$(CC) $(CFLAGS) /I$(MOD_RAMPART_INCLUDE_PATH) $(RAMPARTSRC)\samples\callback\pwcb.c /c
	$(LD) $(LDFLAGS) *.obj axis2_util.lib /DLL /OUT:pwcb.dll /IMPLIB:pwcb.lib

	if not exist $(RAMPART_DIST_DIR)samples\callback mkdir $(RAMPART_DIST_DIR)\samples\callback
	
	copy pwcb.dll $(RAMPART_DIST_DIR)\samples\callback\pwcb.dll
	copy $(RAMPARTSRC)\samples\callback\pwcb.c $(RAMPART_DIST_DIR)\samples\callback
	del pwcb.*

mod_rampart_samples:
	if not exist $(RAMPART_DIST_DIR)\samples\client\echo mkdir $(RAMPART_DIST_DIR)\samples\client\echo
	
	$(CC) $(CFLAGS) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" \
	$(RAMPARTSRC)\samples\client\echo\echo.c /c
	$(LD) $(LDFLAGS) echo.obj $(LIBS) /OUT:echo.exe


	del *.obj
	copy echo.exe $(RAMPART_DIST_DIR)\samples\client\echo
	copy $(RAMPARTSRC)\samples\client\echo\README.txt $(RAMPART_DIST_DIR)\samples\client\echo
	copy $(RAMPARTSRC)\samples\client\echo\echo.c $(RAMPART_DIST_DIR)\\samples\client\echo
	
	xcopy /E /I /Y $(RAMPARTSRC)\samples\client\echo\data $(RAMPART_DIST_DIR)\samples\client\echo\data
	del echo.exe

mod_rampart_copy_include:
	xcopy /E /I /Y $(RAMPARTSRC)\include $(RAMPART_DIST_DIR)\include

mod_rampart_copy_extra:
	copy $(RAMPARTSRC)\README $(RAMPART_DIST_DIR)
	copy $(RAMPARTSRC)\LICENSE $(RAMPART_DIST_DIR)
	copy $(RAMPARTSRC)\INSTALL $(RAMPART_DIST_DIR)
	
mod_rampart_clean:
	if exist $(RAMPART_DIST_DIR) rmdir /S /Q $(RAMPART_DIST_DIR)


mod_rampart_all: mod_rampart mod_rampart_pwcb mod_rampart_samples mod_rampart_copy_include mod_rampart_copy_extra


install: mod_rampart_clean mod_rampart_all




	



