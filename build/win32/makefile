# THIS IS THE MAKE FILE FOR AXIS2
# this file should reside in the win32 directory of the source directory
# when executed 
# nmake [all] build dlls , server , samples [ client, server ] 
# nmake clean clean everything
# nmake samples build samples
# nmake tests
# nmake install to install axis2 with server all modules and services


# install directory will be (default)deploy or if the user a specified  directory
# then the directory structure is following
#	AXIS2_INSTALL_DIR
#	|
#	|- bin        - server and other executables
#       |   |- samples - samples
#       |- logs	
#	|- lib	      - library modules
#	|- services   - deployed services will be hear
#	|- modules    - deployed modules will be hear
#       |- include    - all include files of axis2 will go here
#       |- tests
#            |- system_tests
#            |- unit tests           


# directory structure of install directory
AUTOCONF = configure.in
!include $(AUTOCONF)

#LIBXML2_INSTALL_DIR = C:\libxml2
#ICONV_INSTALL_DIR = C:\iconv

AXIS2_INSTALL_DIR = ..\deploy
AXIS2_SERVICES = $(AXIS2_INSTALL_DIR)\services
AXIS2_MODULES = $(AXIS2_INSTALL_DIR)\modules
AXIS2_LIBS = $(AXIS2_INSTALL_DIR)\lib
AXIS2_BINS = $(AXIS2_INSTALL_DIR)\bin
AXIS2_INCLUDE = $(AXIS2_INSTALL_DIR)\include
AXIS2_SAMPLES = $(AXIS2_BINS)\samples
AXIS2_LOG     = $(AXIS2_INSTALL_DIR)\logs
AXIS2_TESTS = $(AXIS2_INSTALL_DIR)\tests
AXIS2_TESTS_SYS = $(AXIS2_TESTS)\system_tests
AXIS2_TESTS_UNIT = $(AXIS2_TESTS)\unit_tests
AXIS2_SOURCE_DIR = ..\..

#config file name

AXIS2_CONFIG_FILE = .\axis2config.msvc



#  versioning information

AXIS2_MAJOR_VERSION = M
AXIS2_MINOR_VERSION = 0
AXIS2_MICRO_VERSION = 9

#binary distribution dll names

AXIS2_WINDOWS = axis2_windows
AXIS2_UTIL   = axis2_util
AXIS2_PARSER = axis2_parser
AXIS2_OM     = axis2_om
AXIS2_SOAP   = axis2_soap
AXIS2_WSDL   = axis2_wsdl
AXIS2_TRANSPORT_SENDER = axis2_http_sender
AXIS2_TRANSPORT_RECEIVER = axis2_http_receiver
AXIS2_ENGINE = axis2_engine
AXIS2_SERVER = axis2_http_server
AXIS2_APACHE_MODULE = mod_axis2

# axis2_version

AXIS2_DIST_VER = $(AXIS2_MAJOR_VERSION).$(AXIS2_MINOR_VERSION).$(AXIS2_MICRO_VERSION)


# the preprocessor and its options
CPP = cl.exe /FP
CPPFLAGS = /nologo /I$(AXIS2_SOURCE_DIR)\include 

#The compiler and its options

# include path
GUTHTHILA_INCP = /I$(AXIS2_SOURCE_DIR)\modules\xml\parser\guththila\impl\src
APACHE_INCP = /I$(APACHE_INSTALL_DIR)\include
AXIS2_INCP = /I$(AXIS2_SOURCE_DIR)\modules\util /I$(AXIS2_SOURCE_DIR)\modules\xml\parser /I$(AXIS2_SOURCE_DIR)\modules\platforms /I$(AXIS2_SOURCE_DIR)\modules\platforms\windows /I$(AXIS2_SOURCE_DIR)\modules\wsdl /I$(AXIS2_SOURCE_DIR)\modules\wsdl /I$(AXIS2_SOURCE_DIR)\modules\core\clientapi /I$(AXIS2_SOURCE_DIR)\modules\core\deployment /I$(AXIS2_SOURCE_DIR)\modules\core\description /I$(AXIS2_SOURCE_DIR)\modules\core\transport /I$(AXIS2_SOURCE_DIR)\include /I$(LIBXML2_INSTALL_DIR)\include /I$(ICONV_INSTALL_DIR)\include /I$(AXIS2_SOURCE_DIR)\modules\core\engine /I$(AXIS2_SOURCE_DIR)\modules\core\context /I$(AXIS2_SOURCE_DIR)\modules\core\util /I$(AXIS2_SOURCE_DIR)\include\platforms /I$(AXIS2_SOURCE_DIR)\include\platforms\windows /I$(AXIS2_SOURCE_DIR)\modules\xml\om /I$(AXIS2_SOURCE_DIR)\modules\xml\soap  /I$(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\apache2

 
CC = cl.exe 
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT" /w /nologo
CFLAGS = $(CFLAGS) $(AXIS2_INCP) $(GUTHTHILA_INCP) $(APACHE_INCP)



# linker options

LD = link.exe
LDFLAGS = /nologo /LIBPATH:$(AXIS2_LIBS) /LIBPATH:$(LIBXML2_INSTALL_DIR)\lib /LIBPATH:$(APACHE_INSTALL_DIR)\lib
LIBS =  Rpcrt4.lib Ws2_32.lib
APACHE_LIBS = apr.lib xml.lib libhttpd.lib aprutil.lib libapr.lib

# The archiver and its options.
AR = lib.exe
ARFLAGS = /nologo

#debug symbols
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Z7
LDFLAGS = $(LDFLAGS) /DEBUG
!else
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 
LDFLAGS = $(LDFLAGS)
!endif


# create the directory structure

deploy:
	if not exist $(AXIS2_INSTALL_DIR) mkdir $(AXIS2_INSTALL_DIR) 
	if not exist $(AXIS2_BINS) mkdir $(AXIS2_BINS)
	if not exist $(AXIS2_SERVICES) mkdir $(AXIS2_SERVICES)
	if not exist $(AXIS2_MODULES) mkdir $(AXIS2_MODULES)
	if not exist $(AXIS2_LIBS) mkdir $(AXIS2_LIBS)
	if not exist $(AXIS2_INCLUDE) mkdir $(AXIS2_INCLUDE)
	if not exist $(AXIS2_LOG) mkdir $(AXIS2_LOG)

clean:
	if exist $(AXIS2_INSTALL_DIR) rmdir /S /Q $(AXIS2_INSTALL_DIR)

clean_make_dir:
	del *.obj del *.exe del *.exp

copy_include: deploy
	copy $(AXIS2_SOURCE_DIR)\include\*.h $(AXIS2_INSTALL_DIR)\include\
	

axis2_win: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\platforms\windows\*.c  /c
	$(LD) $(LDFLAGS) *.obj $(LIBS) /DLL /OUT:$(AXIS2_WINDOWS).dll /IMPLIB:$(AXIS2_WINDOWS).lib
	del *.obj
	copy $(AXIS2_WINDOWS).*  $(AXIS2_LIBS)\

	del axis2*.*
			
axis2_util: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\util\*.c /c
	$(LD) $(LDFLAGS) *.obj axis2_windows.lib $(LIBS) /DLL /OUT:$(AXIS2_UTIL).dll /IMPLIB:$(AXIS2_UTIL).lib
	del *.obj
	copy $(AXIS2_UTIL).* $(AXIS2_LIBS)\
	
	del axis2*.*

axis2_parser: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\xml\parser\libxml2\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(LIBS) $(LIBXML2_INSTALL_DIR)\lib\libxml2.lib /DLL /OUT:$(AXIS2_PARSER).dll /IMPLIB:$(AXIS2_PARSER).lib
	del *.obj
	copy $(AXIS2_PARSER).* $(AXIS2_LIBS)\
	
	del axis2*.*

axis2_om: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\xml\om\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_PARSER).lib $(LIBS)  /DLL /OUT:$(AXIS2_OM).dll /IMPLIB:$(AXIS2_OM).lib
	del *.obj
	copy $(AXIS2_OM).* $(AXIS2_LIBS)\
	
	del axis2*.*

axis2_soap: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\xml\soap\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(LIBS)  /DLL /OUT:$(AXIS2_SOAP).dll /IMPLIB:$(AXIS2_SOAP).lib
	del *.obj
	copy $(AXIS2_SOAP).* $(AXIS2_LIBS)\
	
	del axis2*.*

axis2_wsdl: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\wsdl\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(LIBS)  /DLL /OUT:$(AXIS2_WSDL).dll /IMPLIB:$(AXIS2_WSDL).lib
	del *.obj
	copy $(AXIS2_WSDL).* $(AXIS2_LIBS)\
	
	del axis2*.*

axis2_engine: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\core\addr\*.c $(AXIS2_SOURCE_DIR)\modules\core\clientapi\*.c $(AXIS2_SOURCE_DIR)\modules\core\context\*.c $(AXIS2_SOURCE_DIR)\modules\core\deployment\*.c $(AXIS2_SOURCE_DIR)\modules\core\description\*.c $(AXIS2_SOURCE_DIR)\modules\core\engine\*.c $(AXIS2_SOURCE_DIR)\modules\core\phaseresolver\*.c $(AXIS2_SOURCE_DIR)\modules\core\receivers\*.c $(AXIS2_SOURCE_DIR)\modules\core\util\*.c $(AXIS2_SOURCE_DIR)\modules\core\transport\http\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib $(AXIS2_WSDL).lib $(AXIS2_WINDOWS).lib $(LIBS)  /DLL /OUT:$(AXIS2_ENGINE).dll /IMPLIB:$(AXIS2_ENGINE).lib
	del *.obj
	copy $(AXIS2_ENGINE).* $(AXIS2_LIBS)\
	
	del axis2*.*

axis2_http_sender: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\core\transport\http\sender\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib $(AXIS2_WSDL).lib $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /DLL /OUT:$(AXIS2_TRANSPORT_SENDER).dll /IMPLIB:$(AXIS2_TRANSPORT_SENDER).lib
	del *.obj
	copy $(AXIS2_TRANSPORT_SENDER).* $(AXIS2_LIBS)\
	
	del axis2*.*
	
		
axis2_http_receiver: 
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\core\transport\http\receiver\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib $(AXIS2_WSDL).lib $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /DLL /OUT:$(AXIS2_TRANSPORT_RECEIVER).dll /IMPLIB:$(AXIS2_TRANSPORT_RECEIVER).lib
	del *.obj
	copy $(AXIS2_TRANSPORT_RECEIVER).* $(AXIS2_LIBS)\
	
	del axis2*.*	


axis2_server: 
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP) $(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\simple_axis2_server\http_server_main.c /c
	$(LD) $(LDFLAGS) http_server_main.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TRANSPORT_RECEIVER).lib  /OUT:$(AXIS2_SERVER).exe 
	del *.obj
	copy $(AXIS2_SERVER).exe $(AXIS2_BINS)\
	
	del axis2*.*

	copy $(AXIS2_SOURCE_DIR)\samples\server\axis2.xml $(AXIS2_INSTALL_DIR)\

axis2_services_echo:
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\echo  $(AXIS2_SOURCE_DIR)\samples\server\echo\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TRANSPORT_RECEIVER).lib $(AXIS2_TRANSPORT_SENDER).lib /DLL /OUT:echo.dll  
	del *.obj
	
	if not exist $(AXIS2_SERVICES)\echo mkdir $(AXIS2_SERVICES)\echo

	copy echo.dll $(AXIS2_SERVICES)\echo

	del *.dll

	del *.lib
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\services.xml $(AXIS2_SERVICES)\echo

axis2_services_notify:
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\echo  $(AXIS2_SOURCE_DIR)\samples\server\notify\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TRANSPORT_RECEIVER).lib $(AXIS2_TRANSPORT_SENDER).lib /DLL /OUT:notify.dll  
	del *.obj
	
	if not exist $(AXIS2_SERVICES)\notify mkdir $(AXIS2_SERVICES)\notify

	copy notify.dll $(AXIS2_SERVICES)\notify

	del *.dll

	del *.lib
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\services.xml $(AXIS2_SERVICES)\notify


axis2_services_math:
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\math  $(AXIS2_SOURCE_DIR)\samples\server\math\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TRANSPORT_RECEIVER).lib $(AXIS2_TRANSPORT_SENDER).lib /DLL /OUT:math.dll  
	del *.obj
	
	if not exist $(AXIS2_SERVICES)\math mkdir $(AXIS2_SERVICES)\math

	copy math.dll $(AXIS2_SERVICES)\math

	del *.dll

	del *.lib
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\services.xml $(AXIS2_SERVICES)\math

axis2_mod_addr:
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\mod_addr\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:axis2_mod_addr.dll  /IMPLIB:axis2_mod_addr.lib
	del *.obj
	
	if not exist $(AXIS2_MODULES)\addressing mkdir $(AXIS2_MODULES)\addressing

	copy axis2_mod_addr.dll $(AXIS2_MODULES)\addressing\

	del *.dll

	del *.lib
	
	copy $(AXIS2_SOURCE_DIR)\modules\mod_addr\module.xml $(AXIS2_MODULES)\addressing

axis2_apache_module:
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\apache2\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib $(APACHE_LIBS) /DLL /OUT:mod_axis2.dll  /IMPLIB:mod_axis2.lib 
	del *.obj
	copy mod_axis2.dll $(AXIS2_LIBS)\

	copy mod_axis2.lib $(AXIS2_LIBS)\

	del *.dll

	del *.lib
	
axis2_client_math:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP)  $(AXIS2_SOURCE_DIR)\samples\client\math\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:math.exe 
	del *.obj
	
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples

	copy math.exe $(AXIS2_BINS)\samples
	del math.exe
	del math.exp

axis2_client_echo:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP)  $(AXIS2_SOURCE_DIR)\samples\client\echo\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:echo.exe 
	del *.obj
	
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples

	copy echo.exe $(AXIS2_BINS)\samples
	del echo.exe
	del echo.exp
	

axis2_client_echo_addr:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP)  $(AXIS2_SOURCE_DIR)\samples\client\addr_echo\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:addr_echo.exe 
	del *.obj
	
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples

	copy addr_echo.exe $(AXIS2_BINS)\samples
 	del addr_echo.exe


axis2_client_google_spell:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP)  $(AXIS2_SOURCE_DIR)\samples\client\google\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:google_spell.exe 
	del *.obj
	
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples

	copy google_spell.exe $(AXIS2_BINS)\samples
	del google_spell.exe

axis2_client_notify:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP)  $(AXIS2_SOURCE_DIR)\samples\client\notify\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:notify.exe 
	del *.obj
	
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples

	copy notify.exe $(AXIS2_BINS)\samples
	del notify.exe
	del notify.exp


axis2_client_echo_non_blocking:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCP) $(AXIS2_SOURCE_DIR)\samples\client\echo_non_blocking\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIS2_OM).lib $(AXIS2_SOAP).lib $(AXIS2_PARSER).lib  $(AXIS2_WINDOWS).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:echo_non_blocking.exe 
	del *.obj
	
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples

	copy echo_non_blocking.exe $(AXIS2_BINS)\samples
 	del echo_non_blocking.exe

copy_extra:
	copy $(AXIS2_SOURCE_DIR)\INSTALL $(AXIS2_INSTALL_DIR)
	copy $(AXIS2_SOURCE_DIR)\LICENSE $(AXIS2_INSTALL_DIR)
	copy $(AXIS2_SOURCE_DIR)\CREDITS $(AXIS2_INSTALL_DIR)

copy_samples:
	if not exist $(AXIS2_INSTALL_DIR)\samples mkdir $(AXIS2_INSTALL_DIR)\samples

	if not exist $(AXIS2_INSTALL_DIR)\samples\client mkdir $(AXIS2_INSTALL_DIR)\samples\client

	if not exist $(AXIS2_INSTALL_DIR)\samples\client\echo mkdir $(AXIS2_INSTALL_DIR)\samples\client\echo
	copy $(AXIS2_SOURCE_DIR)\samples\client\echo\*.c $(AXIS2_INSTALL_DIR)\samples\client\echo
	copy $(AXIS2_SOURCE_DIR)\samples\client\echo\*.h $(AXIS2_INSTALL_DIR)\samples\client\echo

	if not exist $(AXIS2_INSTALL_DIR)\samples\client\math mkdir $(AXIS2_INSTALL_DIR)\samples\client\math
	copy $(AXIS2_SOURCE_DIR)\samples\client\math\*.c $(AXIS2_INSTALL_DIR)\samples\client\math
	copy $(AXIS2_SOURCE_DIR)\samples\client\math\*.h $(AXIS2_INSTALL_DIR)\samples\client\math

	if not exist $(AXIS2_INSTALL_DIR)\samples\client\addr_echo mkdir $(AXIS2_INSTALL_DIR)\samples\client\addr_echo
	copy $(AXIS2_SOURCE_DIR)\samples\client\addr_echo\*.c $(AXIS2_INSTALL_DIR)\samples\client\addr_echo
	copy $(AXIS2_SOURCE_DIR)\samples\client\addr_echo\README.txt $(AXIS2_INSTALL_DIR)\samples\client\addr_echo
	
	if not exist $(AXIS2_INSTALL_DIR)\samples\client\google mkdir $(AXIS2_INSTALL_DIR)\samples\client\google
	copy $(AXIS2_SOURCE_DIR)\samples\client\google\*.c $(AXIS2_INSTALL_DIR)\samples\client\google
	copy $(AXIS2_SOURCE_DIR)\samples\client\google\README.txt $(AXIS2_INSTALL_DIR)\samples\client\google

	if not exist $(AXIS2_INSTALL_DIR)\samples\client\notify mkdir $(AXIS2_INSTALL_DIR)\samples\client\notify
	copy $(AXIS2_SOURCE_DIR)\samples\client\notify\*.c $(AXIS2_INSTALL_DIR)\samples\client\notify

	if not exist $(AXIS2_INSTALL_DIR)\samples\client\echo_non_blocking mkdir $(AXIS2_INSTALL_DIR)\samples\client\echo_non_blocking
	copy $(AXIS2_SOURCE_DIR)\samples\client\echo_non_blocking\*.c $(AXIS2_INSTALL_DIR)\samples\client\echo_non_blocking
	copy $(AXIS2_SOURCE_DIR)\samples\client\echo_non_blocking\README.txt $(AXIS2_INSTALL_DIR)\samples\client\echo_non_blocking

	if not exist $(AXIS2_INSTALL_DIR)\samples\server mkdir $(AXIS2_INSTALL_DIR)\samples\server

	if not exist $(AXIS2_INSTALL_DIR)\samples\server\echo mkdir $(AXIS2_INSTALL_DIR)\samples\server\echo
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.c $(AXIS2_INSTALL_DIR)\samples\server\echo
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.h $(AXIS2_INSTALL_DIR)\samples\server\echo
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.xml $(AXIS2_INSTALL_DIR)\samples\server\echo

	if not exist $(AXIS2_INSTALL_DIR)\samples\server\math mkdir $(AXIS2_INSTALL_DIR)\samples\server\math
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.c $(AXIS2_INSTALL_DIR)\samples\server\math
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.h $(AXIS2_INSTALL_DIR)\samples\server\math
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.xml $(AXIS2_INSTALL_DIR)\samples\server\math

	
	if not exist $(AXIS2_INSTALL_DIR)\samples\server\notify mkdir $(AXIS2_INSTALL_DIR)\samples\server\notify
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.c $(AXIS2_INSTALL_DIR)\samples\server\notify
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.h $(AXIS2_INSTALL_DIR)\samples\server\notify
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.xml $(AXIS2_INSTALL_DIR)\samples\server\notify

mv_dist:
	if exist ..\axis2c-bin-$(RELEASE_VER)-win32 rmdir /S /Q ..\axis2c-bin-$(RELEASE_VER)-win32
	xcopy /E /I $(AXIS2_INSTALL_DIR) ..\axis2c-bin-$(RELEASE_VER)-win32
	rmdir /S /Q $(AXIS2_INSTALL_DIR)

all: clean_make_dir clean deploy axis2_win axis2_util axis2_parser axis2_om axis2_soap axis2_wsdl axis2_engine axis2_http_sender axis2_http_receiver axis2_mod_addr axis2_server axis2_services_echo axis2_services_math axis2_services_notify axis2_client_echo axis2_client_math axis2_client_echo_addr axis2_client_google_spell axis2_client_notify axis2_client_echo_non_blocking copy_include clean_make_dir 
 

install: all

dist: all axis2_apache_module copy_extra copy_samples mv_dist