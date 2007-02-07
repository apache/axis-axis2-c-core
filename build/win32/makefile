# THIS IS THE MAKE FILE FOR AXIS2/C
# this file should reside in the win32 directory of the source directory
# when executed 
# nmake [all] build dlls , server , samples [ client, server ] 
# nmake clean clean everything
# nmake samples build samples
# nmake install to install axis2 with server all modules and services
# nmake dist    to create the distribution, builds all distributable components

#It is possible to run individual targets to build those targets only, 
# eg nmake axis2_apache_module , will build apache module


# install directory will be (default)deploy or a user specified  directory
# then the directory structure is following
#      AXIS2_BINDIR
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

AUTOCONF = configure.in
!include $(AUTOCONF)

# directory structure of install directory

AXIS2_BINDIR   = ..\deploy
AXIS2_SERVICES = $(AXIS2_BINDIR)\services
AXIS2_MODULES  = $(AXIS2_BINDIR)\modules
AXIS2_LIBS     = $(AXIS2_BINDIR)\lib
AXIS2_BINS     = $(AXIS2_BINDIR)\bin
AXIS2_INCLUDE  = $(AXIS2_BINDIR)\include
AXIS2_SAMPLES  = $(AXIS2_BINS)\samples
AXIS2_LOG      = $(AXIS2_BINDIR)\logs
AXIS2_TESTS    = $(AXIS2_BINDIR)\tests
AXIS2_TESTS_SYS = $(AXIS2_TESTS)\system_tests
AXIS2_TESTS_UNIT = $(AXIS2_TESTS)\unit_tests
AXIS2_SOURCE_DIR = ..\..

#directories for intermediate files

AXIS2_INTDIR= .\int.msvc
AXIS2_INTDIR_AXIOM = $(AXIS2_INTDIR)\axiom
AXIS2_INTDIR_UTIL  = $(AXIS2_INTDIR)\util
AXIS2_INTDIR_CORE  = $(AXIS2_INTDIR)\core
AXIS2_INTDIR_XMLSCHEMA = $(AXIS2_INTDIR)\xmlschema
AXIS2_INTDIR_WODEN  = $(AXIS2_INTDIR)\woden
AXIS2_INTDIR_TNS = $(AXIS2_INTDIR)\http_sender
AXIS2_INTDIR_TNR = $(AXIS2_INTDIR)\http_receiver
AXIS2_INTDIR_PARSER = $(AXIS2_INTDIR)\axis2_parser
AXIS2_INTDIR_ADDRESSING = $(AXIS2_INTDIR)\modules\addressing
AXIS2_INTDIR_SAMPLES = $(AXIS2_INTDIR)\samples

#  versioning information

AXIS2_MAJOR_VERSION = M
AXIS2_MINOR_VERSION = 0
AXIS2_MICRO_VERSION = 9

#binary distribution dll names
#core modules

AXIS2_UTIL   = axis2_util
AXIS2_PARSER = axis2_parser
AXIOM        = axiom
AXIS2_TRANSPORT_SENDER = axis2_http_sender
AXIS2_TRANSPORT_RECEIVER = axis2_http_receiver
AXIS2_ENGINE = axis2_engine
XML_SCHEMA   = xml_schema
WODEN        = woden
AXIS2_APACHE_MODULE = mod_axis2
AXIS2_IIS_MODULE = axis2_mod_IIS
AXIS2_MOD_ADDR = axis2_mod_addr
AXIS2_SERVER = axis2_http_server


#other modules 

AXIS2_RAMPART = mod_rampart
AXIS2_MOD_LOG = axis2_mod_log

# include path
APACHE_INCLUE_PATH = /I$(APACHE_BIN_DIR)\include
AXIS2_INCLUDE_PATH = 	/I$(AXIS2_SOURCE_DIR)\util\include \
		/I$(AXIS2_SOURCE_DIR)\util\src\  \
		/I$(AXIS2_SOURCE_DIR)\woden\include \
		/I$(AXIS2_SOURCE_DIR)\woden\src\util \
		/I$(AXIS2_SOURCE_DIR)\util\src\minizip\  \
		/I$(AXIS2_SOURCE_DIR)\axiom\include  \
		/I$(AXIS2_SOURCE_DIR)\axiom\src\om  \
		/I$(AXIS2_SOURCE_DIR)\axiom\src\soap \
		/I$(AXIS2_SOURCE_DIR)\util\include\platforms \
		/I$(AXIS2_SOURCE_DIR)\modules\core\clientapi \
		/I$(AXIS2_SOURCE_DIR)\modules\core\deployment \
		/I$(AXIS2_SOURCE_DIR)\modules\core\description \
		/I$(AXIS2_SOURCE_DIR)\modules\core\transport \
		/I$(AXIS2_SOURCE_DIR)\include \
		/I$(AXIS2_SOURCE_DIR)\modules\core\engine \
		/I$(AXIS2_SOURCE_DIR)\modules\core\context \
		/I$(AXIS2_SOURCE_DIR)\modules\core\util \
		/I$(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\apache2 \
		/I$(AXIS2_SOURCE_DIR)\axiom\src\attachments \
		/I$(AXIS2_SOURCE_DIR)\xml_schema\include \
		/I$(LIBXML2_BIN_DIR)\include \
		/I$(ICONV_BIN_DIR)\include \
		/I$(ZLIB_BIN_DIR)\include
		
WODEN_INCLUDE_PATH = /I$(AXIS2_SOURCE_DIR)\woden\include \
		     /I$(AXIS2_SOURCE_DIR)\xml_schema\include \
		     /I$(AXIS2_SOURCE_DIR)\woden\src\util 


!if "$(ENABLE_SSL)" == "1"
AXIS2_INCLUDE_PATH = $(AXIS2_INCLUDE_PATH) /I$(OPENSSL_BIN_DIR )\include
!endif

#source paths for core modules

AXIS2_UTIL_SRC = $(AXIS2_SOURCE_DIR)\util\src\platforms\windows\*.c \
		$(AXIS2_SOURCE_DIR)\util\src\minizip\*.c \
		$(AXIS2_SOURCE_DIR)\util\src\*.c 

AXIS2_PARSER_SRC = $(AXIS2_SOURCE_DIR)\axiom\src\parser\libxml2\*.c

AXIOM_SRC = $(AXIS2_SOURCE_DIR)\axiom\src\om\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\soap\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\attachments\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\util\*.c

WODEN_SRC = $(AXIS2_SOURCE_DIR)\woden\src\builder\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\builder\wsdl10\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\schema\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\types\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\util\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\xml\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\extensions\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\extensions\soap\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\extensions\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\extensions\soap\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\xml\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\xml\*.c
		
XML_SCHEMA_SRC = $(AXIS2_SOURCE_DIR)\xml_schema\src\*.c
						
AXIS2_ENGINE_SRC =  $(AXIS2_SOURCE_DIR)\modules\core\addr\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\clientapi\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\context\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\deployment\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\description\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\engine\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\phaseresolver\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\receivers\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\util\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\transport\http\*.c
		
AXIS2_HTTP_SENDER_SRC = $(AXIS2_SOURCE_DIR)\modules\core\transport\http\sender\*.c
!if "$(ENABLE_SSL)" == "1"
AXIS2_HTTP_SENDER_SRC = $(AXIS2_HTTP_SENDER_SRC) \
		$(AXIS2_SOURCE_DIR)\modules\core\transport\http\sender\ssl\*.c
!endif

AXIS2_HTTP_RECEIVER_SRC = $(AXIS2_SOURCE_DIR)\modules\core\transport\http\receiver\*.c 

AXIS2_MOD_ADDR_SRC = $(AXIS2_SOURCE_DIR)\modules\mod_addr\*.c

#compiler options

CC = cl.exe 
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT"  \
	  /D "AXIS2_SVR_MULTI_THREADED" /w /nologo $(AXIS2_INCLUDE_PATH) \
	 $(APACHE_INCLUDE_PATH)

# linker options
LD = link.exe
LDFLAGS = /nologo /LIBPATH:$(AXIS2_LIBS) /LIBPATH:$(LIBXSLT_BIN_DIR)\lib \
	     /LIBPATH:$(LIBXML2_BIN_DIR)\lib  /LIBPATH:$(APACHE_BIN_DIR)\lib \
		 /LIBPATH:$(ZLIB_BIN_DIR)\lib
		 
LIBS =  Rpcrt4.lib Ws2_32.lib

!if "$(ENABLE_SSL)" == "1"
LDFLAGS = $(LDFLAGS) /LIBPATH:$(OPENSSL_BIN_DIR )\lib\VC
LIBS = $(LIBS) libeay32MT.lib ssleay32MT.lib
!endif

!if "$(APACHE_VERSION_IS_2_0_59)" == "1"
APACHE_LIBS = apr.lib xml.lib libhttpd.lib aprutil.lib libapr.lib
!else
APACHE_LIBS = apr-1.lib xml.lib libhttpd.lib aprutil-1.lib libapr-1.lib
!endif

#debug symbols
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Z7
LDFLAGS = $(LDFLAGS) /DEBUG /NODEFAULTLIB:LIBCMTD.lib 
!else
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 /MT
LDFLAGS = $(LDFLAGS)
!endif


################################################################

# create the directory structure
deploy:
	if not exist $(AXIS2_BINDIR) mkdir $(AXIS2_BINDIR) 
	if not exist $(AXIS2_BINS) mkdir $(AXIS2_BINS)
	if not exist $(AXIS2_SERVICES) mkdir $(AXIS2_SERVICES)
	if not exist $(AXIS2_MODULES) mkdir $(AXIS2_MODULES)
	if not exist $(AXIS2_LIBS) mkdir $(AXIS2_LIBS)
	if not exist $(AXIS2_INCLUDE) mkdir $(AXIS2_INCLUDE)
	if not exist $(AXIS2_LOG) mkdir $(AXIS2_LOG)
	if not exist $(AXIS2_MODULES)\addressing mkdir $(AXIS2_MODULES)\addressing
			
#intermidiate directories
	
	if not exist $(AXIS2_INTDIR) mkdir $(AXIS2_INTDIR)
	if not exist $(AXIS2_INTDIR_AXIOM) mkdir $(AXIS2_INTDIR_AXIOM)
	if not exist $(AXIS2_INTDIR_UTIL) mkdir $(AXIS2_INTDIR_UTIL)
	if not exist $(AXIS2_INTDIR_CORE) mkdir $(AXIS2_INTDIR_CORE)
	if not exist $(AXIS2_INTDIR_XMLSCHEMA) mkdir $(AXIS2_INTDIR_XMLSCHEMA)
	if not exist $(AXIS2_INTDIR_WODEN) mkdir $(AXIS2_INTDIR_WODEN)
	if not exist $(AXIS2_INTDIR_TNS) mkdir $(AXIS2_INTDIR_TNS)
	if not exist $(AXIS2_INTDIR_TNR) mkdir $(AXIS2_INTDIR_TNR)
	if not exist $(AXIS2_INTDIR_PARSER) mkdir $(AXIS2_INTDIR_PARSER)
	if not exist $(AXIS2_INTDIR_ADDRESSING) mkdir $(AXIS2_INTDIR_ADDRESSING)
	if not exist $(AXIS2_INTDIR_SAMPLES) mkdir $(AXIS2_INTDIR_SAMPLES)
		
#clean 
clean:
	if exist $(AXIS2_BINDIR) rmdir /S /Q $(AXIS2_BINDIR)
	if exist $(AXIS2_INTDIR) rmdir /S /Q $(AXIS2_INTDIR)
	
clean_make_dir:
	del *.obj del *.exe del *.exp

axis2_util: 
	$(CC) $(CFLAGS) $(AXIS2_UTIL_SRC) /Fo$(AXIS2_INTDIR_UTIL)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_UTIL)\*.obj $(LIBS) $(ZLIB_BIN_DIR)\lib\zdll.lib \
		/DLL  /OUT:$(AXIS2_LIBS)\$(AXIS2_UTIL).dll  /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_UTIL).lib 
		
axis2_parser: 
	$(CC) $(CFLAGS) $(AXIS2_PARSER_SRC) /Fo$(AXIS2_INTDIR_PARSER)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_PARSER)\*.obj $(AXIS2_UTIL).lib $(LIBS) \
	$(LIBXML2_BIN_DIR)\lib\libxml2.lib  /DLL \
	/OUT:$(AXIS2_LIBS)\$(AXIS2_PARSER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_PARSER).lib
	
axiom: 
	$(CC) $(CFLAGS) $(AXIOM_SRC) /Fo$(AXIS2_INTDIR_AXIOM)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_AXIOM)\*.obj $(AXIS2_UTIL).lib \
	$(AXIS2_PARSER).lib $(LIBS)  /DLL /OUT:$(AXIS2_LIBS)\$(AXIOM).dll \
	/IMPLIB:$(AXIS2_LIBS)\$(AXIOM).lib
	
xml_schema:
	$(CC) $(CFLAGS) $(XML_SCHEMA_INCLUDE_PATH) $(XML_SCHEMA_SRC) /Fo$(AXIS2_INTDIR_XMLSCHEMA)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_XMLSCHEMA)\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	/DLL /OUT:$(AXIS2_LIBS)\xml_schema.dll /IMPLIB:$(AXIS2_LIBS)\xml_schema.lib

woden : 
	$(CC) $(CFLAGS) $(WODEN_INCLUDE_PATH) $(WODEN_SRC) /Fo$(AXIS2_INTDIR_WODEN)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_WODEN)\*.obj xml_schema.lib $(AXIS2_UTIL).lib $(AXIS2_PARSER).lib \
	$(AXIOM).lib /DLL /OUT:$(AXIS2_LIBS)\woden.dll /IMPLIB:$(AXIS2_LIBS)\woden.lib 
	
	
axis2_engine: 
	$(CC) $(CFLAGS) $(AXIS2_ENGINE_SRC) /Fo$(AXIS2_INTDIR_CORE)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CORE)\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib woden.lib $(LIBS)  \
	/DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_ENGINE).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_ENGINE).lib

axis2_http_sender: 
	$(CC) $(CFLAGS) $(AXIS2_HTTP_SENDER_SRC) /Fo$(AXIS2_INTDIR_TNS)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_TNS)\*.obj $(AXIS2_UTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_TRANSPORT_SENDER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_TRANSPORT_SENDER).lib

axis2_http_receiver: 
	$(CC) $(CFLAGS) $(AXIS2_HTTP_RECEIVER_SRC) /Fo$(AXIS2_INTDIR_TNR)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_TNR)\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  \
	/DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_TRANSPORT_RECEIVER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_TRANSPORT_RECEIVER).lib

axis2_mod_addr:
	
	$(CC) $(CFLAGS) $(AXIS2_MOD_ADDR_SRC) /Fo$(AXIS2_INTDIR_ADDRESSING)\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_ADDRESSING)\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_MODULES)\addressing\$(AXIS2_MOD_ADDR).dll
	copy $(AXIS2_SOURCE_DIR)\modules\mod_addr\module.xml $(AXIS2_MODULES)\addressing


##### simple_axis_server
SIMPLE_AXIS2_SVR_SRC=$(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\simple_axis2_server\http_server_main.c
axis2_server: 
	if not exist $(AXIS2_INTDIR)\simple_http_server mkdir $(AXIS2_INTDIR)\simple_http_server
	
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(SIMPLE_AXIS2_SVR_SRC) /Fo$(AXIS2_INTDIR)\simple_http_server\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\simple_http_server\http_server_main.obj $(AXIS2_UTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	$(AXIS2_TRANSPORT_RECEIVER).lib  /OUT:$(AXIS2_BINS)\$(AXIS2_SERVER).exe 
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\axis2.xml $(AXIS2_BINDIR)\

axis2_core: axis2_util axis2_parser axiom xml_schema woden axis2_engine axis2_http_sender axis2_http_receiver axis2_mod_addr axis2_server

#################################
#server modules

#apache module 

AXIS2_APACHE_MODULE_SRC=$(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\apache2\*.c

axis2_apache_module:
    if not exist $(AXIS2_INTDIR)\apache_module mkdir $(AXIS2_INTDIR)\apache_module
    $(CC) $(CFLAGS) $(APACHE_INCLUE_PATH)  $(AXIS2_APACHE_MODULE_SRC) /Fo$(AXIS2_INTDIR)\apache_module\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\apache_module\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(APACHE_LIBS) /DLL /OUT:$(AXIS2_LIBS)\mod_axis2.dll
	
#mod IIS

MOD_IIS_SRC = $(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\iis\*.c \
	      $(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\IIS\iis_iaspi_plugin_51\*.c 
DEF_FILE = $(AXIS2_SOURCE_DIR)\modules\core\transport\http\server\IIS\mod_axis2.def

	
axis2_IIS_module:
	if not exist $(AXIS2_INTDIR)\mod_IIS mkdir $(AXIS2_INTDIR)\mod_IIS
	$(CC) $(CFLAGS) $(MOD_IIS_SRC) /Fo$(AXIS2_INTDIR)\mod_IIS\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\mod_IIS\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) Advapi32.lib $(AXIS2_ENGINE).lib /DEF:$(DEF_FILE) /DLL /OUT:$(AXIS2_LIBS)\mod_axis2_IIS.dll

############# end modules  ###################


################ samples ####################
axis2_mod_log:
	if not exist $(AXIS2_INTDIR_SAMPLES)\mod_log mkdir $(AXIS2_INTDIR_SAMPLES)\mod_log
	if not exist $(AXIS2_SAMPLES)\logging mkdir $(AXIS2_SAMPLES)\logging

	$(CC) $(CFLAGS)  $(AXIS2_SOURCE_DIR)\modules\mod_log\*.c /Fo$(AXIS2_INTDIR_SAMPLES)\mod_log\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SAMPLES)\mod_log\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SAMPLES)\logging\axis2_mod_log.dll 
	
	copy $(AXIS2_SOURCE_DIR)\modules\mod_log\module.xml $(AXIS2_SAMPLES)\logging

#sample services

axis2_services_echo:
	if not exist $(AXIS2_SERVICES)\echo mkdir $(AXIS2_SERVICES)\echo
	if not exist $(AXIS2_INTDIR_SAMPLES)\services\echo mkdir $(AXIS2_INTDIR_SAMPLES)\services\echo
	
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\echo \
	$(AXIS2_SOURCE_DIR)\samples\server\echo\*.c /Fo$(AXIS2_INTDIR_SAMPLES)\services\echo\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SAMPLES)\services\echo\*.obj $(AXIS2_UTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_SERVICES)\echo\echo.dll  
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\services.xml $(AXIS2_SERVICES)\echo

axis2_services_notify:
	if not exist $(AXIS2_SERVICES)\notify mkdir $(AXIS2_SERVICES)\notify
	if not exist $(AXIS2_INTDIR_SAMPLES)\services\notify mkdir $(AXIS2_INTDIR_SAMPLES)\services\notify

	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\echo \
	$(AXIS2_SOURCE_DIR)\samples\server\notify\*.c /Fo$(AXIS2_INTDIR_SAMPLES)\services\notify\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SAMPLES)\services\notify\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	 /DLL /OUT:$(AXIS2_SERVICES)\notify\notify.dll  
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\services.xml $(AXIS2_SERVICES)\notify


axis2_services_math:
	if not exist $(AXIS2_SERVICES)\math mkdir $(AXIS2_SERVICES)\math
	if not exist $(AXIS2_INTDIR_SAMPLES)\services\math mkdir $(AXIS2_INTDIR_SAMPLES)\services\math

	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\math \
	$(AXIS2_SOURCE_DIR)\samples\server\math\*.c /Fo$(AXIS2_INTDIR_SAMPLES)\services\math\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SAMPLES)\services\math\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\math\math.dll  
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\services.xml $(AXIS2_SERVICES)\math

axis2_services_mtom:
	if not exist $(AXIS2_SERVICES)\mtom mkdir $(AXIS2_SERVICES)\mtom
	if not exist $(AXIS2_INTDIR_SAMPLES)\services\mtom mkdir $(AXIS2_INTDIR_SAMPLES)\services\mtom
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\mtom \
	$(AXIS2_SOURCE_DIR)\samples\server\mtom\*.c /Fo$(AXIS2_INTDIR_SAMPLES)\services\mtom\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SAMPLES)\services\mtom\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\mtom\mtom.dll  

	copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\services.xml $(AXIS2_SERVICES)\mtom

axis2_services_calculator:
	if not exist $(AXIS2_SERVICES)\Calculator mkdir $(AXIS2_SERVICES)\Calculator
	if not exist $(AXIS2_INTDIR_SAMPLES)\services\Calculator mkdir $(AXIS2_INTDIR_SAMPLES)\services\Calculator

	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\Calculator \
	$(AXIS2_SOURCE_DIR)\samples\server\Calculator\*.c /Fo$(AXIS2_INTDIR_SAMPLES)\services\Calculator\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SAMPLES)\services\Calculator\*.obj $(AXIS2_UTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_SERVICES)\Calculator\Calculator.dll  

	copy $(AXIS2_SOURCE_DIR)\samples\server\Calculator\services.xml $(AXIS2_SERVICES)\Calculator


AXIS2_INTDIR_SGM=$(AXIS2_INTDIR_SAMPLES)\services\sgmath

axis2_services_sg_math:
	if not exist $(AXIS2_SERVICES)\sg_math mkdir $(AXIS2_SERVICES)\sg_math
	if not exist $(AXIS2_INTDIR_SGM)\add mkdir $(AXIS2_INTDIR_SGM)\add
	if not exist $(AXIS2_INTDIR_SGM)\sub mkdir $(AXIS2_INTDIR_SGM)\sub
	if not exist $(AXIS2_INTDIR_SGM)\mul mkdir $(AXIS2_INTDIR_SGM)\mul
	if not exist $(AXIS2_INTDIR_SGM)\div mkdir $(AXIS2_INTDIR_SGM)\div

	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math  \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\add.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\add_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\add\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\add\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_SERVICES)\sg_math\add.dll  

	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\div.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\div_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\div\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\div\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\sg_math\div.dll  
	
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math  \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\sub.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\sub_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\sub\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\sub\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib  /DLL /OUT:$(AXIS2_SERVICES)\sg_math\sub.dll  
	
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math  \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\mul.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\mul_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\mul\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\mul\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TRANSPORT_RECEIVER).lib \
	$(AXIS2_TRANSPORT_SENDER).lib /DLL /OUT:$(AXIS2_SERVICES)\sg_math\mul.dll  
	
	copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\services.xml $(AXIS2_SERVICES)\sg_math

###################### clients #################################
AXIS2_INTDIR_CLI = $(AXIS2_INTDIR)\samples\clients
axis2_clients_intdir:
	if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples
	if not exist $(AXIS2_INTDIR_CLI)\math mkdir $(AXIS2_INTDIR_CLI)\math
	if not exist $(AXIS2_INTDIR_CLI)\echo mkdir $(AXIS2_INTDIR_CLI)\echo
	if not exist $(AXIS2_INTDIR_CLI)\dyn_cli mkdir $(AXIS2_INTDIR_CLI)\dyn_cli
	if not exist $(AXIS2_INTDIR_CLI)\gslc mkdir $(AXIS2_INTDIR_CLI)\gslc
	if not exist $(AXIS2_INTDIR_CLI)\yahoo mkdir $(AXIS2_INTDIR_CLI)\yahoo
	if not exist $(AXIS2_INTDIR_CLI)\notify mkdir $(AXIS2_INTDIR_CLI)\notify
	if not exist $(AXIS2_INTDIR_CLI)\mtom mkdir $(AXIS2_INTDIR_CLI)\mtom

axis2_client_math:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\samples\client\math\*.c /Fo$(AXIS2_INTDIR_CLI)\math\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\math\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\math.exe 

axis2_client_echo:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\samples\client\echo\echo.c /Fo$(AXIS2_INTDIR_CLI)\echo\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\echo\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\echo.exe 

axis2_client_dynamic_client:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\samples\client\dynamic_client\dynamic_client.c /Fo$(AXIS2_INTDIR_CLI)\dyn_cli\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\dyn_cli\*.obj $(AXIS2_UTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /OUT:$(AXIS2_BINS)\samples\dynamic_client.exe 

	if not exist $(AXIS2_BINS)\samples\resources mkdir $(AXIS2_BINS)\samples\resources
	copy $(AXIS2_SOURCE_DIR)\samples\client\dynamic_client\*.wsdl $(AXIS2_BINS)\samples\resources

axis2_client_google_spell:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\samples\client\google\*.c /Fo$(AXIS2_INTDIR_CLI)\gslc\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\gslc\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\google.exe 

axis2_client_yahoo:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\samples\client\yahoo\*.c /Fo$(AXIS2_INTDIR_CLI)\yahoo\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\yahoo\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\yahoo.exe 

axis2_client_notify:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\samples\client\notify\*.c /Fo$(AXIS2_INTDIR_CLI)\notify\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\notify\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\notify.exe 

axis2_client_mtom:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\client\mtom\*.c /Fo$(AXIS2_INTDIR_CLI)\mtom\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\mtom\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\mtom.exe 

	mkdir $(AXIS2_BINS)\samples\resources 
	xcopy $(AXIS2_SOURCE_DIR)\samples\client\mtom\resources $(AXIS2_BINS)\samples\resources 

axis2_client_userguide:
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:echo_blocking.exe 
	del *.obj
	
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_non_blocking.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:echo_non_blocking.exe 
	del *.obj

	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_addr.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:echo_blocking_addr.exe 
	del *.obj

	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_rest.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:echo_rest.exe 
	del *.obj

	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_dual.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:echo_blocking_dual.exe 
	del *.obj

	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_non_blocking_dual.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:echo_non_blocking_dual.exe 
	del *.obj

	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH)  \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_soap11.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:echo_blocking_soap11.exe 
	del *.obj
	
	copy *.exe $(AXIS2_BINS)\samples
	del *.exe
	
	
copy_extra:
	copy $(AXIS2_SOURCE_DIR)\INSTALL $(AXIS2_BINDIR)
	copy $(AXIS2_SOURCE_DIR)\LICENSE $(AXIS2_BINDIR)
	copy $(AXIS2_SOURCE_DIR)\CREDITS $(AXIS2_BINDIR)
	copy $(AXIS2_SOURCE_DIR)\README $(AXIS2_BINDIR)
	if exist $(AXIS2_SOURCE_DIR)\docs xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\docs $(AXIS2_BINDIR)\docs

copy_samples:
	if not exist $(AXIS2_BINDIR)\samples mkdir $(AXIS2_BINDIR)\samples

	if not exist $(AXIS2_BINDIR)\samples\client mkdir $(AXIS2_BINDIR)\samples\client
	if not exist $(AXIS2_BINDIR)\samples\user_guide mkdir $(AXIS2_BINDIR)\samples\user_guide

	if not exist $(AXIS2_BINDIR)\samples\client\echo mkdir $(AXIS2_BINDIR)\samples\client\echo
	copy $(AXIS2_SOURCE_DIR)\samples\client\echo\echo.c $(AXIS2_BINDIR)\samples\client\echo
	copy $(AXIS2_SOURCE_DIR)\samples\client\echo\README.txt $(AXIS2_BINDIR)\samples\client\echo
	

	if not exist $(AXIS2_BINDIR)\samples\client\math mkdir $(AXIS2_BINDIR)\samples\client\math
	copy $(AXIS2_SOURCE_DIR)\samples\client\math\*.c $(AXIS2_BINDIR)\samples\client\math
	copy $(AXIS2_SOURCE_DIR)\samples\client\math\*.h $(AXIS2_BINDIR)\samples\client\math

	if not exist $(AXIS2_BINDIR)\samples\client\google mkdir $(AXIS2_BINDIR)\samples\client\google
	copy $(AXIS2_SOURCE_DIR)\samples\client\google\*.c $(AXIS2_BINDIR)\samples\client\google
	copy $(AXIS2_SOURCE_DIR)\samples\client\google\README.txt $(AXIS2_BINDIR)\samples\client\google

	if not exist $(AXIS2_BINDIR)\samples\client\dynamic_client mkdir $(AXIS2_BINDIR)\samples\client\dynamic_client
	copy $(AXIS2_SOURCE_DIR)\samples\client\dynamic_client\dynamic_client.c $(AXIS2_BINDIR)\samples\client\dynamic_client
	copy $(AXIS2_SOURCE_DIR)\samples\client\dynamic_client\README $(AXIS2_BINDIR)\samples\client\dynamic_client

	
	if not exist $(AXIS2_BINDIR)\samples\client\notify mkdir $(AXIS2_BINDIR)\samples\client\notify
	copy $(AXIS2_SOURCE_DIR)\samples\client\notify\*.c $(AXIS2_BINDIR)\samples\client\notify

	if not exist $(AXIS2_BINDIR)\samples\client\mtom\resources mkdir $(AXIS2_BINDIR)\samples\client\mtom\resources
	copy $(AXIS2_SOURCE_DIR)\samples\client\mtom\*.c $(AXIS2_BINDIR)\samples\client\mtom
	copy $(AXIS2_SOURCE_DIR)\samples\client\mtom\resources\*.jpg $(AXIS2_BINDIR)\samples\client\mtom\resources

	xcopy /E /I $(AXIS2_SOURCE_DIR)\samples\user_guide $(AXIS2_BINDIR)\samples\user_guide

	if not exist $(AXIS2_BINDIR)\samples\server mkdir $(AXIS2_BINDIR)\samples\server

	if not exist $(AXIS2_BINDIR)\samples\server\echo mkdir $(AXIS2_BINDIR)\samples\server\echo
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.c $(AXIS2_BINDIR)\samples\server\echo
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.h $(AXIS2_BINDIR)\samples\server\echo
	copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.xml $(AXIS2_BINDIR)\samples\server\echo

	if not exist $(AXIS2_BINDIR)\samples\server\math mkdir $(AXIS2_BINDIR)\samples\server\math
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.c $(AXIS2_BINDIR)\samples\server\math
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.h $(AXIS2_BINDIR)\samples\server\math
	copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.xml $(AXIS2_BINDIR)\samples\server\math

	if not exist $(AXIS2_BINDIR)\samples\server\notify mkdir $(AXIS2_BINDIR)\samples\server\notify
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.c $(AXIS2_BINDIR)\samples\server\notify
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.h $(AXIS2_BINDIR)\samples\server\notify
	copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.xml $(AXIS2_BINDIR)\samples\server\notify

	if not exist $(AXIS2_BINDIR)\samples\server\sg_math mkdir $(AXIS2_BINDIR)\samples\server\sg_math
	copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\*.c $(AXIS2_BINDIR)\samples\server\sg_math
	copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\*.h $(AXIS2_BINDIR)\samples\server\sg_math
	copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\*.xml $(AXIS2_BINDIR)\samples\server\sg_math

	if not exist $(AXIS2_BINDIR)\samples\server\mtom mkdir $(AXIS2_BINDIR)\samples\server\mtom
	copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\*.c $(AXIS2_BINDIR)\samples\server\mtom
	copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\*.h $(AXIS2_BINDIR)\samples\server\mtom
	copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\*.xml $(AXIS2_BINDIR)\samples\server\mtom
	
copy_include: 
	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\include $(AXIS2_BINDIR)\include\

	if exist $(AXIS2_BINDIR)\include\Makefile.am del $(AXIS2_BINDIR)\include\Makefile.am

	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\axiom\include $(AXIS2_BINDIR)\include\

	if exist $(AXIS2_BINDIR)\include\Makefile.am del $(AXIS2_BINDIR)\include\Makefile.am

	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\util\include $(AXIS2_BINDIR)\include\
	
	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\xml_schema\include $(AXIS2_BINDIR)\include
	
	xcopy /Y /I /E $(AXIS2_SOURCE_DIR)\woden\include $(AXIS2_BINDIR)\include 


mv_dist:
	cd ..
	if exist axis2c-bin-$(RELEASE_VER)-win32 rmdir /S /Q axis2c-bin-$(RELEASE_VER)-win32
	rename 	deploy axis2c-bin-$(RELEASE_VER)-win32

all_services: axis2_services_echo axis2_services_math axis2_services_notify axis2_services_sg_math axis2_services_mtom axis2_services_calculator 

all_clients: axis2_clients_intdir axis2_client_echo axis2_client_math  axis2_client_google_spell axis2_client_yahoo axis2_client_notify axis2_client_mtom axis2_client_userguide axis2_client_dynamic_client 

axis2_samples: axis2_mod_log all_services all_clients

axis2_server_modules: axis2_apache_module axis2_IIS_module

#############################################################
#other samples (woden and xml_schema)

xml_schema_copy_resources:
	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\xml_schema\test-resources $(AXIS2_BINDIR)\xml_schema\test-resources
	if not exist $(AXIS2_BINDIR)\xml_schema\samples mkdir $(AXIS2_BINDIR)\xml_schema\samples
	copy $(AXIS2_SOURCE_DIR)\xml_schema\samples\test_schema.c $(AXIS2_BINDIR)\xml_schema\samples\
	
xml_schema_sample:
	if not exist $(AXIS2_BINDIR)\bin\samples\xml_schema mkdir $(AXIS2_BINDIR)\bin\samples\xml_schema
	if not exist $(AXIS2_INTDIR)\bin\xmlschema\samples mkdir $(AXIS2_INTDIR)\bin\xmlschema\samples
	
	$(CC) $(CFLAGS) $(XML_SCHEMA_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\xml_schema\samples\test_schema.c /Fo$(AXIS2_INTDIR)\bin\xmlschema\samples\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\bin\xmlschema\samples\test_schema.obj \
	$(AXIS2_UTIL).lib $(AXIS2_PARSER).lib $(AXIOM).lib $(AXIS2_BINDIR)\lib\xml_schema.lib \
	/OUT:$(AXIS2_BINDIR)\bin\samples\xml_schema\test_schema.exe
	
woden_sample:
	if not exist $(AXIS2_BINDIR)\bin\samples\woden mkdir $(AXIS2_BINDIR)\bin\samples\woden
	if not exist $(AXIS2_INTDIR)\bin\woden\samples mkdir $(AXIS2_INTDIR)\bin\woden\samples

	$(CC) $(CFLAGS) $(WODEN_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\woden\samples\test_wsdl2.c /Fo$(AXIS2_INTDIR)\bin\woden\samples\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\bin\woden\samples\test_wsdl2.obj $(AXIS2_UTIL).lib $(AXIS2_PARSER).lib $(AXIOM).lib \
	$(AXIS2_BINDIR)\lib\woden.lib $(AXIS2_BINDIR)\lib\woden.lib\
	/OUT:$(AXIS2_BINDIR)\bin\samples\woden\test_woden.exe

axis2_other_samples:  xml_schema_sample woden_sample xml_schema_copy_resources


################################# tools ##################################################

# tcpmon

tcpmon: 
	if not exist $(AXIS2_INTDIR)\tools\tcpmon mkdir $(AXIS2_INTDIR)\tools\tcpmon
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" $(AXIS2_INCLUDE_PATH) \
	/I $(AXIS2_SOURCE_DIR)\tools\tcpmon\include \
	$(AXIS2_SOURCE_DIR)\tools\tcpmon\src\*.c /Fo$(AXIS2_INTDIR)\tools\tcpmon\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\tools\tcpmon\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib /OUT:$(AXIS2_BINS)\tcpmon.exe

	
# codegen tool

W2C_INCLUDE_PATH = /I$(AXIS2_SOURCE_DIR)\tools\codegen\include \
		   /I$(LIBXSLT_BIN_DIR)\include


W2C_CODEGEN_SOURCE_PATH  = $(AXIS2_SOURCE_DIR)\tools\codegen\src\util\*.c \
		   $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\*.c \
		   $(AXIS2_SOURCE_DIR)\tools\codegen\src\schema\compiler\*.c \

W2C_WRITER_PATH = $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\writer

CODEGEN_INTDIR = $(AXIS2_INTDIR)\tools\codegen


codegen_dirs:
	if not exist $(AXIS2_BINDIR)\lib\codegen mkdir $(AXIS2_BINDIR)\lib\codegen

	if not exist $(CODEGEN_INTDIR) mkdir $(CODEGEN_INTDIR)
	if not exist $(CODEGEN_INTDIR)\w2c_codegen mkdir $(CODEGEN_INTDIR)\w2c_codegen
	if not exist $(CODEGEN_INTDIR)\w2c_databinding_adb_ext mkdir $(CODEGEN_INTDIR)\w2c_databinding_adb_ext
	if not exist $(CODEGEN_INTDIR)\w2c_c_emitter mkdir $(CODEGEN_INTDIR)\w2c_c_emitter
	if not exist $(CODEGEN_INTDIR)\w2c_schema_c_writer mkdir $(CODEGEN_INTDIR)\w2c_schema_c_writer
	if not exist $(CODEGEN_INTDIR)\w2c_cservice_xml_writer mkdir $(CODEGEN_INTDIR)\w2c_cservice_xml_writer
	if not exist $(CODEGEN_INTDIR)\w2c_cskel_header_writer mkdir $(CODEGEN_INTDIR)\w2c_cskel_header_writer
	if not exist $(CODEGEN_INTDIR)\w2c_cskel_source_writer mkdir $(CODEGEN_INTDIR)\w2c_cskel_source_writer
	if not exist $(CODEGEN_INTDIR)\w2c_cstub_source_writer mkdir $(CODEGEN_INTDIR)\w2c_cstub_source_writer
	if not exist $(CODEGEN_INTDIR)\w2c_cstub_header_writer mkdir $(CODEGEN_INTDIR)\w2c_cstub_header_writer
	if not exist $(CODEGEN_INTDIR)\w2c_csvc_skeleton_writer mkdir $(CODEGEN_INTDIR)\w2c_csvc_skeleton_writer
	if not exist $(CODEGEN_INTDIR)\w2c_c_code_indenter_ext mkdir $(CODEGEN_INTDIR)\w2c_c_code_indenter_ext
	if not exist $(CODEGEN_INTDIR)\w2c_c_qname2name_ext mkdir $(CODEGEN_INTDIR)\w2c_c_qname2name_ext
	if not exist $(CODEGEN_INTDIR)\w2c_c_namemaker_ext mkdir $(CODEGEN_INTDIR)\w2c_c_namemaker_ext
	if not exist $(CODEGEN_INTDIR)\w2c_default_qname2name_ext mkdir $(CODEGEN_INTDIR)\w2c_default_qname2name_ext
	if not exist $(CODEGEN_INTDIR)\w2c_databinding_default_ext mkdir $(CODEGEN_INTDIR)\w2c_databinding_default_ext
	if not exist $(CODEGEN_INTDIR)\wsdl2code mkdir $(CODEGEN_INTDIR)\wsdl2code
	if not exist $(CODEGEN_INTDIR)\schema2code mkdir $(CODEGEN_INTDIR)\schema2code
	
			   
w2c_codegen:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH) $(W2C_CODEGEN_SOURCE_PATH) /Fo$(CODEGEN_INTDIR)\w2c_codegen\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_codegen\*.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) xml_schema.lib woden.lib libxslt.lib \
	libxml2.lib /DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_codegen.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_codegen.lib

w2c_databinding_adb_ext:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\tools\codegen\src\schema\databinding_adb_ext.c \
	/Fo$(CODEGEN_INTDIR)\w2c_databinding_adb_ext\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_databinding_adb_ext\*.obj axis2_util.lib \
	axiom.lib axis2_parser.lib $(LIBS) xml_schema.lib woden.lib w2c_codegen.lib \
	/LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_databinding_adb_ext.dll \
	/IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_databinding_adb_ext.lib
 
w2c_c_emitter:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\emitter\*.c \
	/Fo$(CODEGEN_INTDIR)\w2c_c_emitter\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_c_emitter\*.obj axis2_util.lib \
	axiom.lib axis2_parser.lib $(LIBS) xml_schema.lib woden.lib w2c_codegen.lib \
	/LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_c_emitter.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_c_emitter.lib
	
w2c_schema_c_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\tools\codegen\src\schema\writer\*.c \
	/Fo$(CODEGEN_INTDIR)\w2c_schema_c_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_schema_c_writer\*.obj axis2_util.lib axiom.lib \
	axis2_parser.lib $(LIBS) xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_schema_c_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\2c_schema_c_writer.lib
 
w2c_cservice_xml_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH) $(W2C_WRITER_PATH)\cservice_xml_writer.c $(W2C_WRITER_PATH)\writer.c \
	/Fo$(CODEGEN_INTDIR)\w2c_cservice_xml_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_cservice_xml_writer\*.obj axis2_util.lib axiom.lib axis2_parser.lib \
	$(LIBS) xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_cservice_xml_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_cservice_xml_writer.lib

w2c_cskel_header_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(W2C_WRITER_PATH)\cskel_header_writer.c  $(W2C_WRITER_PATH)\writer.c \
	/Fo$(CODEGEN_INTDIR)\w2c_cskel_header_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_cskel_header_writer\*.obj axis2_util.lib axiom.lib \
	axis2_parser.lib $(LIBS) xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_cskel_header_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_cskel_header_writer.lib
 
w2c_cskel_source_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(W2C_WRITER_PATH)\cskel_source_writer.c  $(W2C_WRITER_PATH)\writer.c \
	/Fo$(CODEGEN_INTDIR)\w2c_cskel_source_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_cskel_source_writer\*.obj axis2_util.lib axiom.lib axis2_parser.lib \
	$(LIBS) xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_cskel_source_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_cskel_source_writer.lib
 
w2c_cstub_source_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(W2C_WRITER_PATH)\cstub_source_writer.c  $(W2C_WRITER_PATH)\writer.c \
	/Fo$(CODEGEN_INTDIR)\w2c_cstub_source_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_cstub_source_writer\*.obj axis2_util.lib axiom.lib axis2_parser.lib \
	$(LIBS) xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_cstub_source_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_cstub_source_writer.lib

w2c_cstub_header_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(W2C_WRITER_PATH)\cstub_header_writer.c $(W2C_WRITER_PATH)\writer.c \
	/Fo$(CODEGEN_INTDIR)\w2c_cstub_header_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_cstub_header_writer\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_cstub_header_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_cstub_header_writer.lib

w2c_csvc_skeleton_writer:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH) $(W2C_WRITER_PATH)\csvc_skeleton_writer.c  $(W2C_WRITER_PATH)\writer.c \
	/Fo$(CODEGEN_INTDIR)\w2c_csvc_skeleton_writer\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_csvc_skeleton_writer\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_csvc_skeleton_writer.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_csvc_skeleton_writer.lib

w2c_c_code_indenter_ext:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\extensions\c_code_indenter_ext.c \
	/Fo$(CODEGEN_INTDIR)\w2c_c_code_indenter_ext\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_c_code_indenter_ext\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_c_code_indenter_ext.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_c_code_indenter_ext.lib
 
w2c_c_qname2name_ext:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\extensions\c_qname2name_ext.c \
	/Fo$(CODEGEN_INTDIR)\w2c_c_qname2name_ext\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_c_qname2name_ext\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_c_qname2name_ext.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_c_qname2name_ext.lib
 
w2c_c_namemaker_ext:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\extensions\c_namemaker_ext.c \
	/Fo$(CODEGEN_INTDIR)\w2c_c_namemaker_ext\ /c
	$(LD) $(LDFLAGS)  $(CODEGEN_INTDIR)\w2c_c_namemaker_ext\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_c_namemaker_ext.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_c_namemaker_ext.lib
 
w2c_default_qname2name_ext:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\extensions\default_qname2name_ext.c \
	/Fo$(CODEGEN_INTDIR)\w2c_default_qname2name_ext\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_default_qname2name_ext\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_default_qname2name_ext.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_default_qname2name_ext.lib
 
w2c_databinding_default_ext:
	$(CC) $(CFLAGS) $(W2C_INCLUDE_PATH)  $(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl\extensions\databinding_default_ext.c \
	/Fo$(CODEGEN_INTDIR)\w2c_databinding_default_ext\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\w2c_databinding_default_ext\*.obj axis2_util.lib axiom.lib axis2_parser.lib $(LIBS) \
	xml_schema.lib woden.lib w2c_codegen.lib /LIBPATH:$(AXIS2_BINDIR)\lib\codegen \
	/DLL /OUT:$(AXIS2_BINDIR)\lib\codegen\w2c_databinding_default_ext.dll /IMPLIB:$(AXIS2_BINDIR)\lib\codegen\w2c_databinding_default_ext.lib
 
wsdl2code:
	$(CC) /nologo $(CFLAGS) $(W2C_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\tools\codegen\src\wsdl2code.c /Fo$(CODEGEN_INTDIR)\wsdl2code\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\wsdl2code\*.obj axis2_util.lib axiom.lib axis2_parser.lib \
	xml_schema.lib woden.lib w2c_codegen.lib $(LIBS) \
	/LIBPATH:$(AXIS2_BINDIR)\lib\codegen /OUT:$(AXIS2_BINDIR)\bin\wsdl2code.exe 

schema2code:
	$(CC) /nologo $(CFLAGS) $(W2C_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\tools\codegen\src\schema\xsd2code.c /Fo$(CODEGEN_INTDIR)\schema2code\ /c
	$(LD) $(LDFLAGS) $(CODEGEN_INTDIR)\schema2code\*.obj axis2_util.lib axiom.lib axis2_parser.lib \
	xml_schema.lib woden.lib w2c_codegen.lib $(LIBS) \
	/LIBPATH:$(AXIS2_BINDIR)\lib\codegen /OUT:$(AXIS2_BINDIR)\bin\schema2code.exe 

copy_config:
	xcopy /I /E /Y $(AXIS2_SOURCE_DIR)\tools\codegen\src\typemappers $(AXIS2_BINDIR)\conf\typemappers

	xcopy /I /E /Y $(AXIS2_SOURCE_DIR)\tools\codegen\src\templates $(AXIS2_BINDIR)\conf\templates

	copy $(AXIS2_SOURCE_DIR)\tools\codegen\src\util\codegen-config.properties $(AXIS2_BINDIR)\conf
	
	copy $(AXIS2_SOURCE_DIR)\tools\codegen\src\schema\compiler\schema-config.properties $(AXIS2_BINDIR)\conf

	copy $(AXIS2_SOURCE_DIR)\tools\codegen\src\util\resource.properties $(AXIS2_BINDIR)\conf

copy_resources:
	xcopy /I /E /Y $(AXIS2_SOURCE_DIR)\tools\codegen\test\resources\wsdl $(AXIS2_BINDIR)\bin\codegen_resources\wsdl

	xcopy /I /E /Y $(AXIS2_SOURCE_DIR)\tools\codegen\test\resources\xml $(AXIS2_BINDIR)\bin\codegen_resources\xml



codegen_writers: w2c_c_emitter w2c_schema_c_writer w2c_cservice_xml_writer w2c_cskel_header_writer w2c_cskel_source_writer w2c_cstub_source_writer w2c_cstub_header_writer w2c_csvc_skeleton_writer
codegen_ext: w2c_databinding_adb_ext w2c_c_code_indenter_ext w2c_c_namemaker_ext w2c_default_qname2name_ext w2c_c_qname2name_ext w2c_databinding_default_ext
codegen_tool: codegen_dirs w2c_codegen wsdl2code schema2code codegen_writers codegen_ext copy_config copy_resources


################################ other porjects #########################################

#rampart module

OMXMLSEC_SRC = $(AXIS2_SOURCE_DIR)\rampart\src\omxmlsec\*.c \
    	       $(AXIS2_SOURCE_DIR)\rampart\src\omxmlsec\tokens\*.c \
    	       $(AXIS2_SOURCE_DIR)\rampart\src\omxmlsec\openssl\*.c 

MOD_RAMPART_SRC = $(AXIS2_SOURCE_DIR)\rampart\src\core\*.c \
		$(AXIS2_SOURCE_DIR)\rampart\src\handlers\*.c \
		$(AXIS2_SOURCE_DIR)\rampart\src\util\*.c

MOD_RAMPART_INCLUDE_PATH = /I$(AXIS2_SOURCE_DIR)\rampart\include

!if "$(ENABLE_SSL)" == "0"
MOD_RAMPART_INCLUDE_PATH = $(MOD_RAMPART_INCLUDE_PATH) /I$(OPENSSL_BIN_DIR)\include
LDFLAGS = $(LDFLAGS) /LIBPATH:$(OPENSSL_BIN_DIR)\lib\VC
!endif


#debug symbols
!if "$(DEBUG)" == "1"
LIBS = $(LIBS) libeay32MTd.lib ssleay32MTd.lib
!else
LIBS = $(LIBS) libeay32MT.lib ssleay32MT.lib
!endif

APPLINK_FILE = $(OPENSSL_BIN_DIR)\include\openssl\applink.c

rampart_do_nothing:

RAMPART_BIN_DIR = $(AXIS2_BINDIR)\bin
RAMPART_SAMPLES_DIR = $(RAMPART_BIN_DIR)\samples\rampart
RAMPART_SAMPLES_CLIENT_DIR = $(RAMPART_BIN_DIR)\samples\rampart\client
RAMPART_SAMPLES_SERVER_DIR = $(RAMPART_BIN_DIR)\samples\rampart\server
RAMPART_SAMPLES_KEYS_DIR = $(RAMPART_BIN_DIR)\samples\rampart\keys
RAMPART_SAMPLES_SERVICE_DIR = $(AXIS2_BINDIR)\services
RAMPART_MODULES_DIR = $(AXIS2_BINDIR)\modules\

mod_rampart:
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(MOD_RAMPART_SRC) $(OMXMLSEC_SRC) $(APPLINK_FILE) /c
	$(LD) $(LDFLAGS) /LIBPATH:$(AXIS2_BINDIR)\rampart\rampart *.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib  $(LIBS) \
	$(AXIS2_ENGINE).lib /DLL /OUT:mod_rampart.dll  /IMPLIB:mod_rampart.lib
	del *.obj
	
	if not exist $(RAMPART_MODULES_DIR)\rampart mkdir $(RAMPART_MODULES_DIR)\rampart

	copy mod_rampart.dll $(RAMPART_MODULES_DIR)\rampart
	copy mod_rampart.lib $(AXIS2_BINDIR)\lib
	
	del *.dll
	del *.lib
	del *.exp
	copy $(AXIS2_SOURCE_DIR)\rampart\src\data\module.xml $(RAMPART_MODULES_DIR)\rampart
	
mod_rampart_pwcb: 
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\rampart\samples\callback\pwcb.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib /DLL /OUT:pwcb.dll /IMPLIB:pwcb.lib

	if not exist $(RAMPART_SAMPLES_DIR)\callback mkdir $(RAMPART_SAMPLES_DIR)\callback
	
	copy pwcb.dll $(RAMPART_SAMPLES_DIR)\callback\pwcb.dll
	copy $(AXIS2_SOURCE_DIR)\rampart\samples\callback\pwcb.c $(RAMPART_SAMPLES_DIR)\callback
	del pwcb.*

mod_rampart_authn: 
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\rampart\samples\authn_provider\authn_provider.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_RAMPART).lib $(AXIS2_UTIL).lib /DLL /OUT:authn.dll /IMPLIB:authn.lib

	if not exist $(RAMPART_SAMPLES_DIR)\authn_provider mkdir $(RAMPART_SAMPLES_DIR)\authn_provider
	
	copy authn.dll $(RAMPART_SAMPLES_DIR)\authn_provider\authn.dll
	copy $(AXIS2_SOURCE_DIR)\rampart\samples\authn_provider\authn_provider.c $(RAMPART_SAMPLES_DIR)\authn_provider
	del authn*.*

mod_rampart_htpwcb: 
	$(CC) $(CFLAGS) $(MOD_RAMPART_INCLUDE_PATH) $(AXIS2_SOURCE_DIR)\rampart\samples\callback\htpasswd_callback\htpwcb.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib /DLL /OUT:htpwcb.dll /IMPLIB:htpwcb.lib

	if not exist $(RAMPART_SAMPLES_DIR)\callback\htpasswd mkdir $(RAMPART_SAMPLES_DIR)\callback\htpasswd
	
	copy htpwcb.dll $(RAMPART_SAMPLES_DIR)\callback\htpasswd\htpwcb.dll
	del htpwcb.*

mod_rampart_services_sec_echo:
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\rampart\samples\server\sec_echo \
	$(AXIS2_SOURCE_DIR)\rampart\samples\server\sec_echo\*.c /c
	$(LD) $(LDFLAGS) *.obj $(AXIS2_UTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TRANSPORT_RECEIVER).lib \
	$(AXIS2_TRANSPORT_SENDER).lib /DLL /OUT:sec_echo.dll  
	del *.obj
	
	if not exist $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo mkdir $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo

	copy sec_echo.dll $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo

	del *.dll
	del *.exp
	del *.lib
	
	copy $(AXIS2_SOURCE_DIR)\rampart\samples\server\sec_echo\*.c $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo
	copy $(AXIS2_SOURCE_DIR)\rampart\samples\server\sec_echo\*.h $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo
	copy $(AXIS2_SOURCE_DIR)\rampart\samples\server\sec_echo\services.xml $(RAMPART_SAMPLES_SERVICE_DIR)\sec_echo
	
mod_rampart_samples_sec_echo:
	if not exist $(RAMPART_SAMPLES_CLIENT_DIR)\sec_echo mkdir $(RAMPART_SAMPLES_CLIENT_DIR)\sec_echo
	
	$(CC) /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" $(AXIS2_INCLUDE_PATH) $(MOD_RAMPART_INCLUDE_PATH) \
	$(AXIS2_SOURCE_DIR)\rampart\samples\client\sec_echo\echo.c /c
	$(LD) $(LDFLAGS) echo.obj $(AXIS2_UTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(AXIS2_ENGINE).lib /OUT:echo.exe

	del *.obj
	copy echo.exe $(RAMPART_SAMPLES_CLIENT_DIR)\sec_echo
	copy $(AXIS2_SOURCE_DIR)\rampart\samples\client\sec_echo\echo.c $(RAMPART_SAMPLES_CLIENT_DIR)\sec_echo
	
	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\rampart\samples\client\sec_echo\data $(RAMPART_SAMPLES_CLIENT_DIR)\sec_echo\data
	del echo.exe

mod_rampart_samples: mod_rampart_samples_sec_echo mod_rampart_services_sec_echo

mod_rampart_copy_include:
	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\rampart\include $(AXIS2_BINDIR)\rampart\include
	copy $(AXIS2_SOURCE_DIR)\rampart\README $(AXIS2_BINDIR)\rampart

mod_rampart_copy_keys:
	if not exist $(RAMPART_SAMPLES_KEYS_DIR) mkdir $(RAMPART_SAMPLES_KEYS_DIR)
	xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\rampart\samples\keys\* $(RAMPART_SAMPLES_KEYS_DIR)

mod_rampart_all: mod_rampart mod_rampart_authn mod_rampart_pwcb mod_rampart_htpwcb mod_rampart_samples mod_rampart_copy_include




##############################################################################################
#axis2.dll

SRC_DIR =       $(AXIS2_SOURCE_DIR)\util\src\platforms\windows\*.c \
		$(AXIS2_SOURCE_DIR)\util\src\minizip\*.c \
		$(AXIS2_SOURCE_DIR)\util\src\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\parser\libxml2\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\om\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\soap\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\attachments\*.c \
		$(AXIS2_SOURCE_DIR)\axiom\src\util\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\builder\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\builder\wsdl10\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\schema\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\types\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\util\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\xml\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\extensions\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl10\extensions\soap\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\extensions\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\extensions\soap\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\wsdl20\xml\*.c \
		$(AXIS2_SOURCE_DIR)\woden\src\xml\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\addr\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\clientapi\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\context\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\deployment\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\description\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\engine\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\phaseresolver\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\receivers\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\util\*.c \
		$(AXIS2_SOURCE_DIR)\modules\core\transport\http\*.c \
		$(AXIS2_SOURCE_DIR)\modules\wsdl\*.c \
		$(AXIS2_SOURCE_DIR)\xml_schema\src\*.c

axis2:
	$(CC) $(CFLAGS) /Fo $(AXIS2_INCLUDE_PATH) $(SRC_DIR) /D "_WINDLL" /Gm /Zi /c
	$(LD) $(LDFLAGS) *.obj Rpcrt4.lib Ws2_32.lib zdll.lib libxml2.lib \
	/DLL /OUT:axis2.dll /IMPLIB:axis2.lib 
	
##############################################################################################


all: clean_make_dir clean deploy axis2_core axis2_samples axis2_other_samples copy_include clean_make_dir 
 
install: all

dist: all mod_rampart_all axis2_IIS_module tcpmon codegen_tool copy_extra copy_samples mv_dist

