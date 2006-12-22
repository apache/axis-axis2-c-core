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

RAMPART = mod_rampart
RAMPART_SOURCE_DIR = ..\..

APPLINK_FILE = $(OPENSSL_BIN_DIR)\include\openssl\applink.c

#compiler options
CC = cl.exe
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT" /w /nologo
CFLAGS = $(CFLAGS) /I$(AXIS2_INSTALL_DIR)\include /I$(OPENSSL_BIN_DIR)\include

#linker options 
LD = link.exe
LDFLAGS = /nologo /LIBPATH:$(AXIS2_INSTALL_DIR)\lib /LIBPATH:$(OPENSSL_BIN_DIR)\lib
LIBS = axis2_util.lib axis2_parser.lib axiom.lib axis2_engine.lib


#debug symbols
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Z7
LDFLAGS = $(LDFLAGS) /DEBUG 
LIBS= $(LIBS) libeay32MTd.lib ssleay32MTd.lib
!else
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 
LDFLAGS = $(LDFLAGS)
LIBS= $(LIBS) libeay32MT.lib ssleay32MT.lib
!endif


APPLINK_FILE = $(OPENSSL_BIN_DIR)\include\openssl\applink.c

RAMPART_BIN_DIR = $(AXIS2_INSTALL_DIR)\bin
RAMPART_SAMPLES_CLIENT_DIR = $(RAMPART_BIN_DIR)\samples\rampart\client
RAMPART_SAMPLES_SERVER_DIR = $(RAMPART_BIN_DIR)\samples\rampart\server
RAMPART_SAMPLES_KEYS_DIR = $(RAMPART_BIN_DIR)\samples\rampart\keys
RAMPART_SAMPLES_SERVICE_DIR = $(AXIS2_INSTALL_DIR)\services
RAMPART_MODULES_DIR = $(AXIS2_INSTALL_DIR)\modules\rampart
MOD_RAMPART_INCLUDE_PATH = /I$(RAMPART_SOURCE_DIR)\include

#rampart module

OMXMLSEC_SRC = $(RAMPART_SOURCE_DIR)\src\omxmlsec\*.c \
    	       $(RAMPART_SOURCE_DIR)\src\omxmlsec\tokens\*.c \
    	       $(RAMPART_SOURCE_DIR)\src\omxmlsec\openssl\*.c 

MOD_RAMPART_SRC = $(RAMPART_SOURCE_DIR)\src\core\*.c \
		$(RAMPART_SOURCE_DIR)\src\handlers\*.c \
		$(RAMPART_SOURCE_DIR)\src\util\*.c

mod_rampart:
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(MOD_RAMPART_SRC) $(OMXMLSEC_SRC) $(APPLINK_FILE) /c
	$(LD) $(LDFLAGS) /LIBPATH:$(AXIS2_INSTALL_DIR)\rampart\rampart *.obj $(LIBS) \
	/DLL /OUT:mod_rampart.dll  /IMPLIB:mod_rampart.lib
	del *.obj
	
	if not exist $(RAMPART_MODULES_DIR) mkdir $(RAMPART_MODULES_DIR)

	copy mod_rampart.dll $(RAMPART_MODULES_DIR)
	
	del *.dll
	del *.lib
	del *.exp
	copy $(RAMPART_SOURCE_DIR)\src\data\module.xml $(RAMPART_MODULES_DIR)
	
mod_rampart_pwcb: 
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(RAMPART_SOURCE_DIR)\samples\callback\pwcb.c /c
	$(LD) $(LDFLAGS) *.obj $(LIBS) /DLL /OUT:pwcb.dll /IMPLIB:pwcb.lib

	if not exist $(RAMPART_SAMPLES_CLIENT_DIR)\callback mkdir $(RAMPART_SAMPLES_CLIENT_DIR)\callback
	
	copy pwcb.dll $(RAMPART_SAMPLES_CLIENT_DIR)\callback\pwcb.dll
	copy $(RAMPART_SOURCE_DIR)\samples\callback\pwcb.c $(RAMPART_SAMPLES_CLIENT_DIR)\callback
	del pwcb.*

mod_rampart_htpwcb: 
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(RAMPART_SOURCE_DIR)\samples\callback\htpasswd_callback\htpwcb.c /c
	$(LD) $(LDFLAGS) *.obj $(LIBS) /DLL /OUT:htpwcb.dll /IMPLIB:htpwcb.lib

	if not exist $(RAMPART_SAMPLES_CLIENT_DIR)\callback\htpasswd mkdir $(RAMPART_SAMPLES_CLIENT_DIR)\callback\htpasswd
	
	copy htpwcb.dll $(RAMPART_SAMPLES_CLIENT_DIR)\callback\htpasswd\htpwcb.dll
	del htpwcb.*

mod_rampart_services_sec_echo:
	$(CC) $(CFLAGS) /I$(RAMPART_SOURCE_DIR)\samples\server\sec_echo \
	$(RAMPART_SOURCE_DIR)\samples\server\sec_echo\*.c /c
	$(LD) $(LDFLAGS) *.obj $(LIBS) axis2_http_receiver.lib \
	axis2_http_sender.lib /DLL /OUT:sec_echo.dll  
	del *.obj
	
	if not exist $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo mkdir $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo

	copy sec_echo.dll $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo

	del *.dll
	del *.exp
	del *.lib
	
	copy $(RAMPART_SOURCE_DIR)\samples\server\sec_echo\*.c $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo
	copy $(RAMPART_SOURCE_DIR)\samples\server\sec_echo\*.h $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo
	copy $(RAMPART_SOURCE_DIR)\samples\server\sec_echo\services.xml $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo
	
mod_rampart_samples_enc_echo:
	if not exist $(RAMPART_SAMPLES_CLIENT_DIR)\enc_echo mkdir $(RAMPART_SAMPLES_CLIENT_DIR)\enc_echo
	
	$(CC) $(CFLAGS) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) $(MOD_RAMPART_INCLUDE_PATH) \
	$(RAMPART_SOURCE_DIR)\samples\client\enc_echo\echo.c /c
	$(LD) $(LDFLAGS) $(LIBS) echo.obj /OUT:enc_echo.exe

	del *.obj
	copy enc_echo.exe $(RAMPART_SAMPLES_CLIENT_DIR)\enc_echo
	copy $(RAMPART_SOURCE_DIR)\samples\client\enc_echo\echo.c $(RAMPART_SAMPLES_CLIENT_DIR)\enc_echo
	
	xcopy /E /I /Y $(RAMPART_SOURCE_DIR)\samples\client\enc_echo\data $(RAMPART_SAMPLES_CLIENT_DIR)\enc_echo\data
	del enc_echo.exe

mod_rampart_samples: mod_rampart_samples_enc_echo mod_rampart_services_sec_echo

mod_rampart_copy_include:
	xcopy /E /I /Y $(RAMPART_SOURCE_DIR)\include $(AXIS2_INSTALL_DIR)\rampart\include
	copy $(RAMPART_SOURCE_DIR)\README $(AXIS2_INSTALL_DIR)\rampart

mod_rampart_copy_keys:
	if not exist $(RAMPART_SAMPLES_KEYS_DIR) mkdir $(RAMPART_SAMPLES_KEYS_DIR)
	xcopy /E /I /Y $(RAMPART_SOURCE_DIR)\samples\keys\* $(RAMPART_SAMPLES_KEYS_DIR)

mod_rampart_all: mod_rampart mod_rampart_pwcb mod_rampart_htpwcb mod_rampart_samples mod_rampart_copy_include

mod_rampart_clean: 
	if exist $(RAMPART_BIN_DIR)\samples\rampart rmdir /S /Q $(RAMPART_BIN_DIR)\samples\rampart
	if exist $(RAMPART_MODULES_DIR) rmdir /S /Q $(RAMPART_MODULES_DIR)
	if exist $(AXIS2_INSTALL_DIR)\rampart rmdir /S /Q $(AXIS2_INSTALL_DIR)\rampart
	if exist $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo rmdir /S /Q $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo

