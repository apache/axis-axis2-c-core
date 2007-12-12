# THIS IS THE MAKE FILE FOR AXIS2/C
# This file should reside in the win32 directory of the source directory
# when executed.
# nmake [all]   - builds dlls, server and samples [ client, server ] 
# nmake clean   - cleans everything
# nmake samples - builds samples
# nmake install - installs axis2 with server all modules and services
# nmake dist    - creates the distribution, builds all distributable components

# It is possible to run individual targets to build those targets only.
#  eg: nmake axis2_apache_module, will build apache module


# The default install directory is ..\deploy with a directory structure as 
# follows.
#
#   AXIS2_BINDIR
#	|
#	|- bin         - server and other executables
#       |- samples     - samples
#       |- logs	       - log file location
#	|- lib	       - library modules
#	|- services    - deployed services will be hear
#	|- modules     - deployed modules will be hear
#       |- include - all include files of axis2 will go here
#       |- tests
#            |- system_tests
#            |- unit tests           

AUTOCONF = configure.in
!include $(AUTOCONF)

# directory structure of install directory
RELEASE_VER = 1.2.0

AXIS2_RELEASE_DIR=axis2c-bin-$(RELEASE_VER)-win32

AXIS2_BINDIR     = ..\deploy
AXIS2_SERVICES   = $(AXIS2_BINDIR)\services
AXIS2_MODULES    = $(AXIS2_BINDIR)\modules
AXIS2_LIBS       = $(AXIS2_BINDIR)\lib
AXIS2_BINS       = $(AXIS2_BINDIR)\bin
AXIS2_INCLUDE    = $(AXIS2_BINDIR)\include
AXIS2_SAMPLES    = $(AXIS2_BINS)\samples
AXIS2_TOOLS      = $(AXIS2_BINS)\tools
AXIS2_LOG        = $(AXIS2_BINDIR)\logs
AXIS2_TESTS      = $(AXIS2_BINDIR)\tests
AXIS2_TESTS_SYS  = $(AXIS2_TESTS)\system_tests
AXIS2_TESTS_UNIT = $(AXIS2_TESTS)\unit_tests
AXIS2_SOURCE_DIR = ..\..

#directories for intermediate files

AXIS2_INTDIR= .\int.msvc
AXIS2_INTDIR_SAMPLES = $(AXIS2_INTDIR)\samples

#binary distribution dll names
#core modules

AXUTIL = axutil
AXIS2_PARSER = axis2_parser
AXIOM = axiom
NEETHI = neethi				 
AXIS2_HTTP_SENDER = axis2_http_sender
AXIS2_HTTP_RECEIVER = axis2_http_receiver
AXIS2_TCP_SENDER = axis2_tcp_sender
AXIS2_TCP_RECEIVER = axis2_tcp_receiver
AXIS2_ENGINE = axis2_engine
AXIS2_APACHE_MODULE = mod_axis2
AXIS2_IIS_MODULE = axis2_mod_IIS
AXIS2_MOD_ADDR = axis2_mod_addr
AXIS2_HTTP_SERVER = axis2_http_server
AXIS2_TCP_SERVER = axis2_tcp_server
GUTHTHILA = guththila
AXIS2_MOD_LOG = axis2_mod_log

PARSER = build

!if "$(ENABLE_GUTHTHILA)" == "1"
PARSER = $(PARSER)_with_guththila
CFLAGS = $(CFLAGS) /D "AXIS2_GUTHTHILA_ENABLED"
!endif

# include path
APACHE_INCLUDE_PATH = /I$(APACHE_BIN_DIR)\include
AXIS2_INCLUDE_PATH = /I$(AXIS2_SOURCE_DIR)\util\include \
		/I$(AXIS2_SOURCE_DIR)\util\src\  \
		/I$(AXIS2_SOURCE_DIR)\util\src\minizip\  \
		/I$(AXIS2_SOURCE_DIR)\axiom\include  \
		/I$(AXIS2_SOURCE_DIR)\axiom\src\om  \
		/I$(AXIS2_SOURCE_DIR)\axiom\src\soap \
		/I$(AXIS2_SOURCE_DIR)\util\include\platforms \
		/I$(AXIS2_SOURCE_DIR)\neethi\include \
		/I$(AXIS2_SOURCE_DIR)\neethi\src \
		/I$(AXIS2_SOURCE_DIR)\neethi\src\secpolicy\builder \
		/I$(AXIS2_SOURCE_DIR)\neethi\src\secpolicy\model \
		/I$(AXIS2_SOURCE_DIR)\src\core\clientapi \
		/I$(AXIS2_SOURCE_DIR)\src\core\deployment \
		/I$(AXIS2_SOURCE_DIR)\src\core\description \
		/I$(AXIS2_SOURCE_DIR)\src\core\transport \
		/I$(AXIS2_SOURCE_DIR)\src\core\transport\tcp \
		/I$(AXIS2_SOURCE_DIR)\include \
		/I$(AXIS2_SOURCE_DIR)\src\core\engine \
		/I$(AXIS2_SOURCE_DIR)\src\core\context \
		/I$(AXIS2_SOURCE_DIR)\src\core\util \
		/I$(AXIS2_SOURCE_DIR)\src\core\transport\http\server\apache2 \
		/I$(AXIS2_SOURCE_DIR)\axiom\src\attachments \
		/I$(AXIS2_SOURCE_DIR)\tools\tcpmon\include \
		/I$(LIBXML2_BIN_DIR)\include \
		/I$(ICONV_BIN_DIR)\include
		
!if "$(WITH_ARCHIVE)" == "1"
AXIS2_INCLUDE_PATH = $(AXIS2_INCLUDE_PATH) /I$(ZLIB_BIN_DIR)\include
!endif

GUTHTHILA_INCLUDE_PATH = /I$(AXIS2_SOURCE_DIR)\guththila\include
 
!if "$(ENABLE_SSL)" == "1"
AXIS2_INCLUDE_PATH = $(AXIS2_INCLUDE_PATH) /I$(OPENSSL_BIN_DIR)\include
!endif
!if "$(ENABLE_LIBCURL)" == "1"
AXIS2_INCLUDE_PATH = $(AXIS2_INCLUDE_PATH) /I$(LIBCURL_BIN_DIR)\include
!endif

##################### compiler options

CC = @cl.exe 
CFLAGS = /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "AXIS2_DECLARE_EXPORT"  \
	 /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_SECURE_NO_WARNINGS" \
         /D "AXIS2_SVR_MULTI_THREADED" /w /nologo $(AXIS2_INCLUDE_PATH) \
	 $(APACHE_INCLUDE_PATH) 

!if "$(ENABLE_SSL)" == "1"
CFLAGS = $(CFLAGS) /D "AXIS2_SSL_ENABLED"
!endif

!if "$(ENABLE_LIBCURL)" == "1"
CFLAGS = $(CFLAGS) /D "AXIS2_LIBCURL_ENABLED"
!endif

################### linker options

LD = @link.exe
LDFLAGS = /nologo /LIBPATH:$(AXIS2_LIBS) /LIBPATH:$(LIBXML2_BIN_DIR)\lib \
       	/LIBPATH:$(APACHE_BIN_DIR)\lib /LIBPATH:$(ZLIB_BIN_DIR)\lib
		 
LIBS =  Rpcrt4.lib Ws2_32.lib

!if "$(ENABLE_SSL)" == "1"
LDFLAGS = $(LDFLAGS) /LIBPATH:$(OPENSSL_BIN_DIR)\lib\VC
LIBS = $(LIBS) libeay32MD.lib ssleay32MD.lib
!endif

!if "$(ENABLE_LIBCURL)" == "1"
LDFLAGS = $(LDFLAGS) /LIBPATH:$(LIBCURL_BIN_DIR)
LIBS = $(LIBS) libcurl_imp.lib
!endif

!if "$(APACHE_VERSION_2_0_X)" == "1"
APACHE_LIBS = apr.lib xml.lib libhttpd.lib libaprutil.lib libapr.lib
!else
APACHE_LIBS = apr-1.lib xml.lib libhttpd.lib libapr-1.lib libaprutil-1.lib
!endif

####################
MT=mt.exe
MT="$(MT)"

!if "$(EMBED_MANIFEST)" == "0"
_VC_MANIFEST_EMBED_EXE=
_VC_MANIFEST_EMBED_DLL=
!else
_VC_MANIFEST_EMBED_EXE= if exist $@.manifest $(MT) -nologo -manifest $@.manifest -outputresource:$@;1
_VC_MANIFEST_EMBED_DLL= if exist $@.manifest $(MT) -nologo -manifest $@.manifest -outputresource:$@;2
!endif

#################### debug symbols
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Z7 $(CRUNTIME)d
LDFLAGS = $(LDFLAGS) /DEBUG /INCREMENTAL
!else
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 $(CRUNTIME)
LDFLAGS = $(LDFLAGS)
!endif


################################################################

# create the directory structure
deploy:
	@if not exist $(AXIS2_BINDIR) mkdir $(AXIS2_BINDIR) 
	@if not exist $(AXIS2_BINS) mkdir $(AXIS2_BINS)
	@if not exist $(AXIS2_SERVICES) mkdir $(AXIS2_SERVICES)
	@if not exist $(AXIS2_MODULES) mkdir $(AXIS2_MODULES)
	@if not exist $(AXIS2_LIBS) mkdir $(AXIS2_LIBS)
	@if not exist $(AXIS2_INCLUDE) mkdir $(AXIS2_INCLUDE)
	@if not exist $(AXIS2_LOG) mkdir $(AXIS2_LOG)
	@if not exist $(AXIS2_MODULES)\addressing mkdir $(AXIS2_MODULES)\addressing
			
	@if not exist $(AXIS2_INTDIR) mkdir $(AXIS2_INTDIR)
	@if not exist $(AXIS2_INTDIR_SAMPLES) mkdir $(AXIS2_INTDIR_SAMPLES)
	
#clean 
clean:
	if exist $(AXIS2_BINDIR) rmdir /S /Q $(AXIS2_BINDIR)
	if exist $(AXIS2_INTDIR) rmdir /S /Q $(AXIS2_INTDIR)
	
##### axutil 

AXUTIL_SRC = $(AXIS2_SOURCE_DIR)\util\src
AXIS2_INTDIR_AXUTIL  = $(AXIS2_INTDIR)\util
AXIS2_INTDIR_AXUTIL1 = $(AXIS2_INTDIR_AXUTIL)\platform
AXIS2_INTDIR_AXUTIL2 =$(AXIS2_INTDIR_AXUTIL)\minizip

AXUTIL_OBJS = $(AXIS2_INTDIR_AXUTIL)\*.obj \
	      $(AXIS2_INTDIR_AXUTIL)\platform\*.obj


!if "$(WITH_ARCHIVE)" == "1"
AXUTIL_OBJS = $(AXUTIL_OBJS)  $(AXIS2_INTDIR_AXUTIL)\minizip\*.obj
!endif

$(AXIS2_INTDIR_AXUTIL) :
	@if not exist $(AXIS2_INTDIR_AXUTIL) mkdir $(AXIS2_INTDIR_AXUTIL)

$(AXIS2_INTDIR_AXUTIL1) :
	@if not exist $(AXIS2_INTDIR_AXUTIL1) mkdir $(AXIS2_INTDIR_AXUTIL1)

$(AXIS2_INTDIR_AXUTIL2) :
	@if not exist $(AXIS2_INTDIR_AXUTIL2) mkdir $(AXIS2_INTDIR_AXUTIL2)
	

{$(AXUTIL_SRC)}.c{$(AXIS2_INTDIR_AXUTIL)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_INTDIR_AXUTIL)\ /c $<

{$(AXUTIL_SRC)\platforms\windows}.c{$(AXIS2_INTDIR_AXUTIL1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_INTDIR_AXUTIL1)\ /c $<


!if "$(WITH_ARCHIVE)" == "1"
{$(AXUTIL_SRC)\minizip}.c{$(AXIS2_INTDIR_AXUTIL2)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_INTDIR_AXUTIL2)\ /c $<

$(AXUTIL_OBJS) :  $(AXIS2_INTDIR_AXUTIL) $(AXIS2_INTDIR_AXUTIL1) $(AXIS2_INTDIR_AXUTIL2)

$(AXIS2_LIBS)\$(AXUTIL).dll : $(AXUTIL_OBJS)
	$(LD) $(LDFLAGS) $(AXUTIL_OBJS) $(LIBS) $(ZLIB_BIN_DIR)\lib\zdll.lib \
	/DLL  /OUT:$(AXIS2_LIBS)\$(AXUTIL).dll  /IMPLIB:$(AXIS2_LIBS)\$(AXUTIL).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

!else

$(AXUTIL_OBJS) :  $(AXIS2_INTDIR_AXUTIL) $(AXIS2_INTDIR_AXUTIL1)

$(AXIS2_LIBS)\$(AXUTIL).dll : $(AXUTIL_OBJS)
	$(LD) $(LDFLAGS) $(AXUTIL_OBJS) $(LIBS) /DLL  /OUT:$(AXIS2_LIBS)\$(AXUTIL).dll  \
	/IMPLIB:$(AXIS2_LIBS)\$(AXUTIL).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

!endif

axis2_util : $(AXIS2_LIBS)\$(AXUTIL).dll
##### end axutil

##### guththila
GUTHTHILA_SRC = $(AXIS2_SOURCE_DIR)\guththila\src
AXIS2_INTDIR_GUTHTHILA = $(AXIS2_INTDIR)\guththila

GUTHTHILA_OBJS = $(AXIS2_INTDIR_GUTHTHILA)\*.obj

$(AXIS2_INTDIR_GUTHTHILA) :
	@if not exist $(AXIS2_INTDIR_GUTHTHILA) mkdir $(AXIS2_INTDIR_GUTHTHILA)	


{$(GUTHTHILA_SRC)}.c{$(AXIS2_INTDIR_GUTHTHILA)}.obj ::
	$(CC) $(CFLAGS) $(GUTHTHILA_INCLUDE_PATH) /Fo$(AXIS2_INTDIR_GUTHTHILA)\ /c $<

$(GUTHTHILA_OBJS) : $(AXIS2_INTDIR_GUTHTHILA)

$(AXIS2_LIBS)\$(GUTHTHILA).dll : $(GUTHTHILA_OBJS)
	$(LD) $(LDFLAGS) $(GUTHTHILA_OBJS) $(AXUTIL).lib $(LIBS) \
	/DLL /OUT:$(AXIS2_LIBS)\$(GUTHTHILA).dll /IMPLIB:$(AXIS2_LIBS)\$(GUTHTHILA).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

guththila: $(AXIS2_LIBS)\$(GUTHTHILA).dll

##### end guththila

##### axis2 parser

AXIS2_PARSER_SRC = $(AXIS2_SOURCE_DIR)\axiom\src\parser
AXIS2_INTDIR_PARSER = $(AXIS2_INTDIR)\parser
AXIS2_INTDIR_PARSER1 = $(AXIS2_INTDIR)\parser\libxml2

AXIS2_PARSER_OBJS = $(AXIS2_INTDIR_PARSER)\*.obj \
		    $(AXIS2_INTDIR_PARSER)\libxml2\*.obj 

$(AXIS2_INTDIR_PARSER) :
	@if not exist $(AXIS2_INTDIR_PARSER) mkdir $(AXIS2_INTDIR_PARSER)

$(AXIS2_INTDIR_PARSER1) :
	@if not exist $(AXIS2_INTDIR_PARSER1) mkdir $(AXIS2_INTDIR_PARSER1)


{$(AXIS2_PARSER_SRC)}.c{$(AXIS2_INTDIR_PARSER)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_INTDIR_PARSER)\ /c $<

{$(AXIS2_PARSER_SRC)\libxml2}.c{$(AXIS2_INTDIR_PARSER1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_INTDIR_PARSER1)\ /c $<

$(AXIS2_PARSER_OBJS) : $(AXIS2_INTDIR_PARSER) $(AXIS2_INTDIR_PARSER1)


$(AXIS2_LIBS)\$(AXIS2_PARSER).dll : $(AXIS2_PARSER_OBJS)
	$(LD) $(LDFLAGS) $(AXIS2_PARSER_OBJS) $(AXUTIL).lib $(LIBS) \
	$(LIBXML2_BIN_DIR)\lib\libxml2.lib  /DLL \
	/OUT:$(AXIS2_LIBS)\$(AXIS2_PARSER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_PARSER).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_parser : $(AXIS2_LIBS)\$(AXIS2_PARSER).dll
##### end axis2 parser

GHTHTHILA_WRAPPER_SRC = $(AXIS2_SOURCE_DIR)\axiom\src\parser
GUTHTHILA_WRAPPER_INTDIR = $(AXIS2_INTDIR)\parser
GUTHTHILA_WRAPPER_INTDIR1 = $(AXIS2_INTDIR)\parser\guththila

GUTHTHILA_WRAPPER_OBJS = $(GUTHTHILA_WRAPPER_INTDIR)\*.obj \
			 $(GUTHTHILA_WRAPPER_INTDIR1)\*.obj

$(GUTHTHILA_WRAPPER_INTDIR) :
	@if not exist $(GUTHTHILA_WRAPPER_INTDIR) mkdir $(GUTHTHILA_WRAPPER_INTDIR)

$(GUTHTHILA_WRAPPER_INTDIR1) :
	@if not exist $(GUTHTHILA_WRAPPER_INTDIR1) mkdir $(GUTHTHILA_WRAPPER_INTDIR1)


{$(GHTHTHILA_WRAPPER_SRC)}.c{$(GUTHTHILA_WRAPPER_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(GUTHTHILA_WRAPPER_INTDIR)\ /c $<

{$(GHTHTHILA_WRAPPER_SRC)\guththila}.c{$(GUTHTHILA_WRAPPER_INTDIR1)}.obj ::
	$(CC) $(CFLAGS) $(GUTHTHILA_INCLUDE_PATH) /Fo$(GUTHTHILA_WRAPPER_INTDIR1)\ /c $<

$(GUTHTHILA_WRAPPER_OBJS) : $(GUTHTHILA_WRAPPER_INTDIR) $(GUTHTHILA_WRAPPER_INTDIR1)

axis2_parser_with_guththila: $(GUTHTHILA_WRAPPER_OBJS)
	$(LD) $(LDFLAGS) $(GUTHTHILA_WRAPPER_OBJS) $(AXUTIL).lib $(LIBS) $(AXIS2_LIBS)\$(GUTHTHILA).lib  /DLL \
	/OUT:$(AXIS2_LIBS)\$(AXIS2_PARSER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_PARSER).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

##### axiom

AXIOM_SRC = $(AXIS2_SOURCE_DIR)\axiom\src
AXIOM_INTDIR = $(AXIS2_INTDIR)\axiom\om
AXIOM_INTDIR1 = $(AXIS2_INTDIR)\axiom\soap
AXIOM_INTDIR2 = $(AXIS2_INTDIR)\axiom\util
AXIOM_INTDIR3 = $(AXIS2_INTDIR)\axiom\attachments

AXIOM_OBJS = $(AXIOM_INTDIR)\*.obj \
	     $(AXIOM_INTDIR1)\*.obj \
	     $(AXIOM_INTDIR2)\*.obj \
	     $(AXIOM_INTDIR3)\*.obj 

$(AXIOM_INTDIR) :
	@if not exist $(AXIOM_INTDIR) mkdir $(AXIOM_INTDIR)
	
$(AXIOM_INTDIR1) :
	@if not exist $(AXIOM_INTDIR1) mkdir $(AXIOM_INTDIR1)

$(AXIOM_INTDIR2) :
	@if not exist $(AXIOM_INTDIR2) mkdir $(AXIOM_INTDIR2)

$(AXIOM_INTDIR3) :
	@if not exist $(AXIOM_INTDIR3) mkdir $(AXIOM_INTDIR3)


{$(AXIOM_SRC)\om}.c{$(AXIOM_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIOM_INTDIR)\ /c $<

{$(AXIOM_SRC)\soap}.c{$(AXIOM_INTDIR1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIOM_INTDIR1)\ /c $<

{$(AXIOM_SRC)\util}.c{$(AXIOM_INTDIR2)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIOM_INTDIR2)\ /c $<

{$(AXIOM_SRC)\attachments}.c{$(AXIOM_INTDIR3)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIOM_INTDIR3)\ /c $<


$(AXIOM_OBJS) : $(AXIOM_INTDIR) $(AXIOM_INTDIR1) $(AXIOM_INTDIR2) $(AXIOM_INTDIR3)


$(AXIS2_LIBS)\$(AXIOM).dll : $(AXIOM_OBJS) 
	$(LD) $(LDFLAGS) $(AXIOM_OBJS) $(AXUTIL).lib $(AXIS2_PARSER).lib $(LIBS) \
	/DLL /OUT:$(AXIS2_LIBS)\$(AXIOM).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIOM).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axiom : $(AXIS2_LIBS)\$(AXIOM).dll

##### end axiom

##### neethi
NEETHI_SRC = $(AXIS2_SOURCE_DIR)\neethi\src
NEETHI_INTDIR = $(AXIS2_INTDIR)\neethi
NEETHI_INTDIR1 = $(AXIS2_INTDIR)\neethi\model
NEETHI_INTDIR2 = $(AXIS2_INTDIR)\neethi\builder

NEETHI_OBJS = $(NEETHI_INTDIR)\*.obj \
	      $(NEETHI_INTDIR)\model\*.obj \
	      $(NEETHI_INTDIR)\builder\*.obj 

$(NEETHI_INTDIR) :
	@if not exist $(NEETHI_INTDIR) mkdir $(NEETHI_INTDIR)

$(NEETHI_INTDIR1) :
	@if not exist $(NEETHI_INTDIR1) mkdir $(NEETHI_INTDIR1)

$(NEETHI_INTDIR2) :
	@if not exist $(NEETHI_INTDIR2) mkdir $(NEETHI_INTDIR2)

{$(NEETHI_SRC)}.c{$(NEETHI_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(NEETHI_INTDIR)\ /c $<

{$(NEETHI_SRC)\secpolicy\model}.c{$(NEETHI_INTDIR1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(NEETHI_INTDIR1)\ /c $<

{$(NEETHI_SRC)\secpolicy\builder}.c{$(NEETHI_INTDIR2)}.obj ::
	$(CC) $(CFLAGS) /Fo$(NEETHI_INTDIR2)\ /c $<


$(NEETHI_OBJS) : $(NEETHI_INTDIR) $(NEETHI_INTDIR1) $(NEETHI_INTDIR2)

$(AXIS2_LIBS)\$(NEETHI).dll : $(NEETHI_OBJS)
	$(LD) $(LDFLAGS) $(NEETHI_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
    	/DLL /OUT:$(AXIS2_LIBS)\$(NEETHI).dll /IMPLIB:$(AXIS2_LIBS)\$(NEETHI).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

neethi : $(AXIS2_LIBS)\$(NEETHI).dll

##### end neethi

##### axis2 engine

ENGINE_SRC =  $(AXIS2_SOURCE_DIR)\src\core
ENGINE_INTDIR = $(AXIS2_INTDIR)\core\addr
ENGINE_INTDIR1 = $(AXIS2_INTDIR)\core\clientapi
ENGINE_INTDIR2 = $(AXIS2_INTDIR)\core\context
ENGINE_INTDIR3 = $(AXIS2_INTDIR)\core\deployment
ENGINE_INTDIR4 = $(AXIS2_INTDIR)\core\description
ENGINE_INTDIR5 = $(AXIS2_INTDIR)\core\engine
ENGINE_INTDIR6 = $(AXIS2_INTDIR)\core\phaseresolver
ENGINE_INTDIR7 = $(AXIS2_INTDIR)\core\receivers
ENGINE_INTDIR8 = $(AXIS2_INTDIR)\core\util
ENGINE_INTDIR9 = $(AXIS2_INTDIR)\core\transport
ENGINE_INTDIR10 = $(AXIS2_INTDIR)\core\transport\http\common
ENGINE_INTDIR11 = $(AXIS2_INTDIR)\core\transport\http\util

ENGINE_OBJS = $(ENGINE_INTDIR)\*.obj \
	      $(ENGINE_INTDIR1)\*.obj \
	      $(ENGINE_INTDIR2)\*.obj \
	      $(ENGINE_INTDIR3)\*.obj \
	      $(ENGINE_INTDIR4)\*.obj \
	      $(ENGINE_INTDIR5)\*.obj \
	      $(ENGINE_INTDIR6)\*.obj \
	      $(ENGINE_INTDIR7)\*.obj \
	      $(ENGINE_INTDIR8)\*.obj \
	      $(ENGINE_INTDIR9)\*.obj \
	      $(ENGINE_INTDIR10)\*.obj \
	      $(ENGINE_INTDIR11)\*.obj 

$(ENGINE_INTDIR) :
	@if not exist $(ENGINE_INTDIR) mkdir $(ENGINE_INTDIR)

$(ENGINE_INTDIR1) :
	@if not exist $(ENGINE_INTDIR1) mkdir $(ENGINE_INTDIR1)

$(ENGINE_INTDIR2) :
	@if not exist $(ENGINE_INTDIR2) mkdir $(ENGINE_INTDIR2)

$(ENGINE_INTDIR3) :
	@if not exist $(ENGINE_INTDIR3) mkdir $(ENGINE_INTDIR3)

$(ENGINE_INTDIR4) :
	@if not exist $(ENGINE_INTDIR4) mkdir $(ENGINE_INTDIR4)

$(ENGINE_INTDIR5) :
	@if not exist $(ENGINE_INTDIR5) mkdir $(ENGINE_INTDIR5)

$(ENGINE_INTDIR6) :
	@if not exist $(ENGINE_INTDIR6) mkdir $(ENGINE_INTDIR6)

$(ENGINE_INTDIR7) :
	@if not exist $(ENGINE_INTDIR7) mkdir $(ENGINE_INTDIR7)

$(ENGINE_INTDIR8) :
	@if not exist $(ENGINE_INTDIR8) mkdir $(ENGINE_INTDIR8)

$(ENGINE_INTDIR9) :
	@if not exist $(ENGINE_INTDIR9) mkdir $(ENGINE_INTDIR9)

$(ENGINE_INTDIR10) :
	@if not exist $(ENGINE_INTDIR10) mkdir $(ENGINE_INTDIR10)

$(ENGINE_INTDIR11) :
	@if not exist $(ENGINE_INTDIR11) mkdir $(ENGINE_INTDIR11)

{$(ENGINE_SRC)\addr}.c{$(ENGINE_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR)\ /c $<

{$(ENGINE_SRC)\clientapi}.c{$(ENGINE_INTDIR1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR1)\ /c $<

{$(ENGINE_SRC)\context}.c{$(ENGINE_INTDIR2)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR2)\ /c $<

{$(ENGINE_SRC)\deployment}.c{$(ENGINE_INTDIR3)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR3)\ /c $<

{$(ENGINE_SRC)\description}.c{$(ENGINE_INTDIR4)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR4)\ /c $<

{$(ENGINE_SRC)\engine}.c{$(ENGINE_INTDIR5)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR5)\ /c $<

{$(ENGINE_SRC)\phaseresolver}.c{$(ENGINE_INTDIR6)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR6)\ /c $<

{$(ENGINE_SRC)\receivers}.c{$(ENGINE_INTDIR7)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR7)\ /c $<

{$(ENGINE_SRC)\util}.c{$(ENGINE_INTDIR8)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR8)\ /c $<

{$(ENGINE_SRC)\transport}.c{$(ENGINE_INTDIR9)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR9)\ /c $<

{$(ENGINE_SRC)\transport\http\common}.c{$(ENGINE_INTDIR10)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR10)\ /c $<

{$(ENGINE_SRC)\transport\http\util}.c{$(ENGINE_INTDIR11)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ENGINE_INTDIR11)\ /c $<

$(ENGINE_OBJS) : $(ENGINE_INTDIR) $(ENGINE_INTDIR1) $(ENGINE_INTDIR2) $(ENGINE_INTDIR3) \
	$(ENGINE_INTDIR4) $(ENGINE_INTDIR5) $(ENGINE_INTDIR6) $(ENGINE_INTDIR7) \
	$(ENGINE_INTDIR8) $(ENGINE_INTDIR9) $(ENGINE_INTDIR10) $(ENGINE_INTDIR11)

$(AXIS2_LIBS)\$(AXIS2_ENGINE).dll : $(ENGINE_OBJS)
	$(LD) $(LDFLAGS) $(ENGINE_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(NEETHI).lib $(LIBS) /DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_ENGINE).dll \
	/IMPLIB:$(AXIS2_LIBS)\$(AXIS2_ENGINE).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_engine : $(AXIS2_LIBS)\$(AXIS2_ENGINE).dll


##### end axis2 engine

##### axis2 http sender

HTTP_SENDER_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\http\sender
HTTP_SENDER_INTDIR = $(AXIS2_INTDIR)\http_sender
HTTP_SENDER_INTDIR1 = $(AXIS2_INTDIR)\http_sender\ssl
HTTP_SENDER_INTDIR2 = $(AXIS2_INTDIR)\http_sender\libcurl

HTTP_SENDER_OBJS = $(HTTP_SENDER_INTDIR)\*.obj

$(HTTP_SENDER_INTDIR) :
	@if not exist $(HTTP_SENDER_INTDIR) mkdir $(HTTP_SENDER_INTDIR)

$(HTTP_SENDER_INTDIR1) :
	@if not exist $(HTTP_SENDER_INTDIR1) mkdir $(HTTP_SENDER_INTDIR1)

$(HTTP_SENDER_INTDIR2) :
	@if not exist $(HTTP_SENDER_INTDIR2) mkdir $(HTTP_SENDER_INTDIR2)

{$(HTTP_SENDER_SRC)}.c{$(HTTP_SENDER_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(HTTP_SENDER_INTDIR)\ /c $<

{$(HTTP_SENDER_SRC)\ssl}.c{$(HTTP_SENDER_INTDIR1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(HTTP_SENDER_INTDIR1)\ /c $<

{$(HTTP_SENDER_SRC)\libcurl}.c{$(HTTP_SENDER_INTDIR2)}.obj ::
	$(CC) $(CFLAGS) /Fo$(HTTP_SENDER_INTDIR2)\ /c $<

!if "$(ENABLE_SSL)" == "1"
HTTP_SENDER_OBJS = $(HTTP_SENDER_OBJS) \
	      $(HTTP_SENDER_INTDIR1)\*.obj

$(HTTP_SENDER_OBJS) : $(HTTP_SENDER_INTDIR) $(HTTP_SENDER_INTDIR1)
!elseif "$(ENABLE_LIBCURL)" == "1"
HTTP_SENDER_OBJS = $(HTTP_SENDER_OBJS) \
	      $(HTTP_SENDER_INTDIR2)\*.obj 

$(HTTP_SENDER_OBJS) : $(HTTP_SENDER_INTDIR) $(HTTP_SENDER_INTDIR2)
!else
$(HTTP_SENDER_OBJS) : $(HTTP_SENDER_INTDIR)
!endif

$(AXIS2_LIBS)\$(AXIS2_HTTP_SENDER).dll : $(HTTP_SENDER_OBJS)
	$(LD) $(LDFLAGS) $(HTTP_SENDER_OBJS) $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_HTTP_SENDER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_HTTP_SENDER).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_http_sender : $(AXIS2_LIBS)\$(AXIS2_HTTP_SENDER).dll


##### axis2 http sender

##### axis2_tcp_sender


TCP_SENDER_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\tcp\sender
TCP_SENDER_INTDIR = $(AXIS2_INTDIR)\tcp_sender

TCP_SENDER_OBJS = $(TCP_SENDER_INTDIR)\*.obj

$(TCP_SENDER_INTDIR) :
	@if not exist $(TCP_SENDER_INTDIR) mkdir $(TCP_SENDER_INTDIR)

{$(TCP_SENDER_SRC)}.c{$(TCP_SENDER_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(TCP_SENDER_INTDIR)\ /c $<


$(TCP_SENDER_OBJS) : $(TCP_SENDER_INTDIR)

$(AXIS2_LIBS)\$(AXIS2_TCP_SENDER).dll : $(TCP_SENDER_OBJS)
	$(LD) $(LDFLAGS) $(TCP_SENDER_OBJS) $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_TCP_SENDER).dll /IMPLIB:$(AXIS2_LIBS)\$(AXIS2_TCP_SENDER).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_tcp_sender : $(AXIS2_LIBS)\$(AXIS2_TCP_SENDER).dll


##### end axis2_tcp_sender


##### axis2 http receiver

HTTP_RECEIVER_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\http\receiver
HTTP_RECEIVER_INTDIR = $(AXIS2_INTDIR)\http_receiver

HTTP_RECEIVER_OBJS = $(HTTP_RECEIVER_INTDIR)\*.obj

$(HTTP_RECEIVER_INTDIR) :
	@if not exist $(HTTP_RECEIVER_INTDIR) mkdir $(HTTP_RECEIVER_INTDIR)

{$(HTTP_RECEIVER_SRC)}.c{$(HTTP_RECEIVER_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(HTTP_RECEIVER_INTDIR)\ /c $<

$(HTTP_RECEIVER_OBJS) : $(HTTP_RECEIVER_INTDIR)

$(AXIS2_LIBS)\$(AXIS2_HTTP_RECEIVER).dll : $(HTTP_RECEIVER_OBJS)
	$(LD) $(LDFLAGS) $(HTTP_RECEIVER_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_HTTP_RECEIVER).dll \
	/IMPLIB:$(AXIS2_LIBS)\$(AXIS2_HTTP_RECEIVER).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_http_receiver : $(AXIS2_LIBS)\$(AXIS2_HTTP_RECEIVER).dll

##### end axis2 http receiver

##### axis2_tcp_receiver

TCP_RECEIVER_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\tcp\receiver
TCP_RECEIVER_INTDIR = $(AXIS2_INTDIR)\tcp_receiver

TCP_RECEIVER_OBJS = $(TCP_RECEIVER_INTDIR)\*.obj

$(TCP_RECEIVER_INTDIR) :
	@if not exist $(TCP_RECEIVER_INTDIR) mkdir $(TCP_RECEIVER_INTDIR)

{$(TCP_RECEIVER_SRC)}.c{$(TCP_RECEIVER_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(TCP_RECEIVER_INTDIR)\ /c $<

$(TCP_RECEIVER_OBJS) : $(TCP_RECEIVER_INTDIR)

$(AXIS2_LIBS)\$(AXIS2_TCP_RECEIVER).dll : $(TCP_RECEIVER_OBJS)
	$(LD) $(LDFLAGS) $(TCP_RECEIVER_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_LIBS)\$(AXIS2_TCP_RECEIVER).dll \
	/IMPLIB:$(AXIS2_LIBS)\$(AXIS2_TCP_RECEIVER).lib
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_tcp_receiver : $(AXIS2_LIBS)\$(AXIS2_TCP_RECEIVER).dll


##### end axis2_tcp_receiver

##### axis2 mod addr
ADDR_SRC = $(AXIS2_SOURCE_DIR)\src\modules\mod_addr
ADDR_INTDIR = $(AXIS2_INTDIR)\addressing

ADDR_OBJS = $(ADDR_INTDIR)\*.obj

$(ADDR_INTDIR) :
	@if not exist $(ADDR_INTDIR) mkdir $(ADDR_INTDIR)

{$(ADDR_SRC)}.c{$(ADDR_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ADDR_INTDIR)\ /c $<

$(ADDR_OBJS) : $(ADDR_INTDIR)

$(AXIS2_MODULES)\addressing\$(AXIS2_MOD_ADDR).dll : $(ADDR_OBJS)
	$(LD) $(LDFLAGS) $(ADDR_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_MODULES)\addressing\$(AXIS2_MOD_ADDR).dll
	@copy $(AXIS2_SOURCE_DIR)\src\modules\mod_addr\module.xml $(AXIS2_MODULES)\addressing
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_mod_addr : $(AXIS2_MODULES)\addressing\$(AXIS2_MOD_ADDR).dll


##### end axis2 mod addr

##### simple_axis_server
SIMPLE_AXIS2_SVR_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\http\server\simple_axis2_server

AXIS2_SVR_INTDIR = $(AXIS2_INTDIR)\simple_http_server

AXIS2_SVR_OBJS = $(AXIS2_INTDIR)\simple_http_server\*.obj

$(AXIS2_SVR_INTDIR) :
	@if not exist $(AXIS2_SVR_INTDIR) mkdir $(AXIS2_SVR_INTDIR)

{$(SIMPLE_AXIS2_SVR_SRC)}.c{$(AXIS2_SVR_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_SVR_INTDIR)\ /c $<

$(AXIS2_SVR_OBJS) : $(AXIS2_SVR_INTDIR)


$(AXIS2_BINS)\$(AXIS2_HTTP_SERVER).exe : $(AXIS2_SVR_OBJS)
	$(LD) $(LDFLAGS) $(AXIS2_SVR_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_HTTP_RECEIVER).lib  /OUT:$(AXIS2_BINS)\$(AXIS2_HTTP_SERVER).exe 
        -@$(_VC_MANIFEST_EMBED_EXE)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\axis2.xml $(AXIS2_BINDIR)\

simple_axis2_http_server : $(AXIS2_BINS)\$(AXIS2_HTTP_SERVER).exe

##### end simple_axis2_server

##### simple tcp server

AXIS2_TCP_SVR_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\tcp\server\simple_tcp_server

AXIS2_TCP_SVR_INTDIR = $(AXIS2_INTDIR)\simple_tcp_server

AXIS2_TCP_SVR_OBJS = $(AXIS2_INTDIR)\simple_tcp_server\*.obj

$(AXIS2_TCP_SVR_INTDIR) :
	@if not exist $(AXIS2_TCP_SVR_INTDIR) mkdir $(AXIS2_TCP_SVR_INTDIR)

{$(AXIS2_TCP_SVR_SRC)}.c{$(AXIS2_TCP_SVR_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(AXIS2_TCP_SVR_INTDIR)\ /c $<

$(AXIS2_TCP_SVR_OBJS) : $(AXIS2_TCP_SVR_INTDIR)


$(AXIS2_BINS)\$(AXIS2_TCP_SERVER).exe : $(AXIS2_TCP_SVR_OBJS)
	$(LD) $(LDFLAGS) $(AXIS2_TCP_SVR_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_TCP_RECEIVER).lib  /OUT:$(AXIS2_BINS)\$(AXIS2_TCP_SERVER).exe 
	-@$(_VC_MANIFEST_EMBED_EXE)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\axis2.xml $(AXIS2_BINDIR)\

simple_axis2_tcp_server : $(AXIS2_BINS)\$(AXIS2_TCP_SERVER).exe


##### end simple tcp server



##########  server modules 
#apache module 

MOD_AXIS2_SRC=$(AXIS2_SOURCE_DIR)\src\core\transport\http\server\apache2
MOD_AXIS2_INTDIR = $(AXIS2_INTDIR)\apache_module

MOD_AXIS2_OBJS = $(MOD_AXIS2_INTDIR)\*.obj

$(MOD_AXIS2_INTDIR) :
	@if not exist $(MOD_AXIS2_INTDIR) mkdir $(MOD_AXIS2_INTDIR)

{$(MOD_AXIS2_SRC)}.c{$(MOD_AXIS2_INTDIR)}.obj ::
	$(CC) $(CFLAGS) $(APACHE_INCLUDE_PATH) /Fo$(MOD_AXIS2_INTDIR)\ /c $<

$(MOD_AXIS2_OBJS) : $(MOD_AXIS2_INTDIR)

$(AXIS2_LIBS)\mod_axis2.dll : $(MOD_AXIS2_OBJS)
	$(LD) $(LDFLAGS) $(MOD_AXIS2_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(APACHE_LIBS) /DLL /OUT:$(AXIS2_LIBS)\mod_axis2.dll
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_apache_module : $(AXIS2_LIBS)\mod_axis2.dll

#mod IIS

MOD_IIS_SRC = $(AXIS2_SOURCE_DIR)\src\core\transport\http\server\IIS
MOD_IIS_INTDIR = $(AXIS2_INTDIR)\mod_IIS
MOD_IIS_INTDIR1 = $(AXIS2_INTDIR)\mod_IIS\isapi51

DEF_FILE = $(AXIS2_SOURCE_DIR)\src\core\transport\http\server\IIS\mod_axis2.def

MOD_IIS_OBJS = $(MOD_IIS_INTDIR)\*.obj \
	       $(MOD_IIS_INTDIR1)\*.obj

$(MOD_IIS_INTDIR) :
	@if not exist $(MOD_IIS_INTDIR) mkdir $(MOD_IIS_INTDIR)
	
$(MOD_IIS_INTDIR1) :
	@if not exist $(MOD_IIS_INTDIR1) mkdir $(MOD_IIS_INTDIR1)


{$(MOD_IIS_SRC)\iis_iaspi_plugin_51}.c{$(MOD_IIS_INTDIR1)}.obj ::
	$(CC) $(CFLAGS) /Fo$(MOD_IIS_INTDIR1)\ /c $<

{$(MOD_IIS_SRC)}.c{$(MOD_IIS_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(MOD_IIS_INTDIR)\ /c $<

$(MOD_IIS_OBJS) : $(MOD_IIS_INTDIR) $(MOD_IIS_INTDIR1)

		
$(AXIS2_LIBS)\mod_axis2_IIS.dll : $(MOD_IIS_OBJS)
	$(LD) $(LDFLAGS) $(MOD_IIS_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) Advapi32.lib $(AXIS2_ENGINE).lib /DEF:$(DEF_FILE) /DLL /OUT:$(AXIS2_LIBS)\mod_axis2_IIS.dll
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_IIS_module : $(AXIS2_LIBS)\mod_axis2_IIS.dll

############# end modules  ###################


axis2_core: axis2_util axis2_parser axiom neethi axis2_engine axis2_http_sender \
	axis2_http_receiver axis2_mod_addr simple_axis2_http_server

axis2_core_without_server:  axis2_util axis2_parser axiom neethi axis2_engine axis2_http_sender \
	axis2_http_receiver axis2_mod_addr

axis2_core_guththila: axis2_util guththila axis2_parser_with_guththila axiom neethi axis2_engine \
	axis2_http_sender axis2_http_receiver axis2_mod_addr simple_axis2_http_server

axis2_tcp : axis2_tcp_sender axis2_tcp_receiver simple_axis2_tcp_server

##### logging module
	
MOD_LOG_SRC =  $(AXIS2_SOURCE_DIR)\src\modules\mod_log
MOD_LOG_INTDIR = $(AXIS2_INTDIR)\mod_log

MOD_LOG_OBJS = $(MOD_LOG_INTDIR)\*.obj



$(MOD_LOG_INTDIR) :
	@if not exist $(AXIS2_MDOULES)\logging mkdir $(AXIS2_MODULES)\logging
	@if not exist $(MOD_LOG_INTDIR) mkdir $(MOD_LOG_INTDIR)


{$(MOD_LOG_SRC)}.c{$(MOD_LOG_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(MOD_LOG_INTDIR)\ /c $<

$(MOD_LOG_OBJS) : $(MOD_LOG_INTDIR)

$(AXIS2_MODULES)\logging\axis2_mod_log.dll : $(MOD_LOG_OBJS)
	$(LD) $(LDFLAGS) $(MOD_LOG_OBJS) $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_MODULES)\logging\axis2_mod_log.dll 
	@copy $(AXIS2_SOURCE_DIR)\src\modules\mod_log\module.xml $(AXIS2_MODULES)\logging
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_mod_log: $(AXIS2_MODULES)\logging\axis2_mod_log.dll

##### end logging module

################ samples ####################
#sample services

### echo

ECHO_SRC =  $(AXIS2_SOURCE_DIR)\samples\server\echo
ECHO_INTDIR = $(AXIS2_INTDIR_SAMPLES)\services\echo

ECHO_OBJS = $(ECHO_INTDIR)\*.obj

$(ECHO_INTDIR) :
	@if not exist $(ECHO_INTDIR) mkdir $(ECHO_INTDIR)
	@if not exist $(AXIS2_SERVICES)\echo mkdir $(AXIS2_SERVICES)\echo

{$(ECHO_SRC)}.c{$(ECHO_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(ECHO_INTDIR)\ /c $<

$(ECHO_OBJS) : $(ECHO_INTDIR)

$(AXIS2_SERVICES)\echo\echo.dll : $(ECHO_OBJS)
	$(LD) $(LDFLAGS) $(ECHO_OBJS) $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\echo\echo.dll
	-@$(_VC_MANIFEST_EMBED_DLL)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\echo\services.xml $(AXIS2_SERVICES)\echo

axis2_services_echo: $(AXIS2_SERVICES)\echo\echo.dll

### notify

NOTIFY_SRC =  $(AXIS2_SOURCE_DIR)\samples\server\notify
NOTIFY_INTDIR = $(AXIS2_INTDIR_SAMPLES)\services\notify

NOTIFY_OBJS = $(NOTIFY_INTDIR)\*.obj

$(NOTIFY_INTDIR) :
	@if not exist $(NOTIFY_INTDIR) mkdir $(NOTIFY_INTDIR)
	@if not exist $(AXIS2_SERVICES)\notify mkdir $(AXIS2_SERVICES)\notify

{$(NOTIFY_SRC)}.c{$(NOTIFY_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(NOTIFY_INTDIR)\ /c $<

$(NOTIFY_OBJS) : $(NOTIFY_INTDIR)

$(AXIS2_SERVICES)\notify\notify.dll  : $(NOTIFY_OBJS)
	$(LD) $(LDFLAGS) $(NOTIFY_OBJS) $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\notify\notify.dll 
	-@$(_VC_MANIFEST_EMBED_DLL)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\notify\services.xml $(AXIS2_SERVICES)\notify

axis2_services_notify: $(AXIS2_SERVICES)\notify\notify.dll 

### math

MATH_SRC =  $(AXIS2_SOURCE_DIR)\samples\server\math
MATH_INTDIR = $(AXIS2_INTDIR_SAMPLES)\services\math

MATH_OBJS = $(MATH_INTDIR)\*.obj

$(MATH_INTDIR) :
	@if not exist $(MATH_INTDIR) mkdir $(MATH_INTDIR)
	@if not exist $(AXIS2_SERVICES)\math mkdir $(AXIS2_SERVICES)\math

{$(MATH_SRC)}.c{$(MATH_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(MATH_INTDIR)\ /c $<

$(MATH_OBJS) : $(MATH_INTDIR)

$(AXIS2_SERVICES)\math\math.dll  : $(MATH_OBJS)
	$(LD) $(LDFLAGS) $(MATH_OBJS) $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\math\math.dll 
	-@$(_VC_MANIFEST_EMBED_DLL)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\math\services.xml $(AXIS2_SERVICES)\math

axis2_services_math: $(AXIS2_SERVICES)\math\math.dll

### mtom

MTOM_SRC =  $(AXIS2_SOURCE_DIR)\samples\server\mtom
MTOM_INTDIR = $(AXIS2_INTDIR_SAMPLES)\services\mtom

MTOM_OBJS = $(MTOM_INTDIR)\*.obj

$(MTOM_INTDIR) :
	@if not exist $(MTOM_INTDIR) mkdir $(MTOM_INTDIR)
	@if not exist $(AXIS2_SERVICES)\mtom mkdir $(AXIS2_SERVICES)\mtom

{$(MTOM_SRC)}.c{$(MTOM_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(MTOM_INTDIR)\ /c $<

$(MTOM_OBJS) : $(MTOM_INTDIR)

$(AXIS2_SERVICES)\mtom\mtom.dll : $(MTOM_OBJS)
	$(LD) $(LDFLAGS) $(MTOM_OBJS) $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\mtom\mtom.dll  
	-@$(_VC_MANIFEST_EMBED_DLL)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\services.xml $(AXIS2_SERVICES)\mtom

axis2_services_mtom: $(AXIS2_SERVICES)\mtom\mtom.dll

#### Calculator

CALCULATOR_SRC =  $(AXIS2_SOURCE_DIR)\samples\server\\Calculator
CALCULATOR_INTDIR = $(AXIS2_INTDIR_SAMPLES)\services\\Calculator

CALCULATOR_OBJS = $(CALCULATOR_INTDIR)\*.obj

$(CALCULATOR_INTDIR) :
	@if not exist $(CALCULATOR_INTDIR) mkdir $(CALCULATOR_INTDIR)
	@if not exist $(AXIS2_SERVICES)\\Calculator mkdir $(AXIS2_SERVICES)\\Calculator

{$(CALCULATOR_SRC)}.c{$(CALCULATOR_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(CALCULATOR_INTDIR)\ /c $<

$(CALCULATOR_OBJS) : $(CALCULATOR_INTDIR)

$(AXIS2_SERVICES)\Calculator\Calculator.dll : $(CALCULATOR_OBJS)
	$(LD) $(LDFLAGS) $(CALCULATOR_OBJS) $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\Calculator\Calculator.dll  
	-@$(_VC_MANIFEST_EMBED_DLL)
	@copy $(AXIS2_SOURCE_DIR)\samples\server\Calculator\services.xml $(AXIS2_SERVICES)\Calculator

axis2_services_calculator : $(AXIS2_SERVICES)\Calculator\Calculator.dll

### sgmath

AXIS2_INTDIR_SGM=$(AXIS2_INTDIR_SAMPLES)\services\sgmath

axis2_services_sg_math_int:
	@if not exist $(AXIS2_SERVICES)\sg_math mkdir $(AXIS2_SERVICES)\sg_math
	@if not exist $(AXIS2_INTDIR_SGM)\add mkdir $(AXIS2_INTDIR_SGM)\add
	@if not exist $(AXIS2_INTDIR_SGM)\sub mkdir $(AXIS2_INTDIR_SGM)\sub
	@if not exist $(AXIS2_INTDIR_SGM)\mul mkdir $(AXIS2_INTDIR_SGM)\mul
	@if not exist $(AXIS2_INTDIR_SGM)\div mkdir $(AXIS2_INTDIR_SGM)\div

$(AXIS2_SERVICES)\sg_math\add.dll :
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math  \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\add.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\add_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\add\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\add\*.obj $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/DLL /OUT:$(AXIS2_SERVICES)\sg_math\add.dll  
	-@$(_VC_MANIFEST_EMBED_DLL)


$(AXIS2_SERVICES)\sg_math\div.dll :
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\div.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\div_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\div\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\div\*.obj $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib /DLL /OUT:$(AXIS2_SERVICES)\sg_math\div.dll  
	-@$(_VC_MANIFEST_EMBED_DLL)
	
$(AXIS2_SERVICES)\sg_math\sub.dll :
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math  \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\sub.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\sub_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\sub\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\sub\*.obj $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib  /DLL /OUT:$(AXIS2_SERVICES)\sg_math\sub.dll
	-@$(_VC_MANIFEST_EMBED_DLL)

$(AXIS2_SERVICES)\sg_math\mul.dll :
	$(CC) $(CFLAGS) /I$(AXIS2_SOURCE_DIR)\samples\server\sg_math  \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\mul.c \
	$(AXIS2_SOURCE_DIR)\samples\server\sg_math\mul_skeleton.c /Fo$(AXIS2_INTDIR_SGM)\mul\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_SGM)\mul\*.obj $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib \
	$(LIBS) $(AXIS2_ENGINE).lib $(AXIS2_HTTP_RECEIVER).lib \
	$(AXIS2_HTTP_SENDER).lib /DLL /OUT:$(AXIS2_SERVICES)\sg_math\mul.dll  
	-@$(_VC_MANIFEST_EMBED_DLL)

axis2_services_sg_math : axis2_services_sg_math_int $(AXIS2_SERVICES)\sg_math\add.dll \
	$(AXIS2_SERVICES)\sg_math\div.dll $(AXIS2_SERVICES)\sg_math\sub.dll $(AXIS2_SERVICES)\sg_math\mul.dll
	@copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\services.xml $(AXIS2_SERVICES)\sg_math

###################### clients #################################
AXIS2_INTDIR_CLI = $(AXIS2_INTDIR)\samples\clients
axis2_clients_intdir:
	@if not exist $(AXIS2_BINS)\samples mkdir $(AXIS2_BINS)\samples
	@if not exist $(AXIS2_INTDIR_CLI)\math mkdir $(AXIS2_INTDIR_CLI)\math
	@if not exist $(AXIS2_INTDIR_CLI)\echo mkdir $(AXIS2_INTDIR_CLI)\echo
	@if not exist $(AXIS2_INTDIR_CLI)\dyn_cli mkdir $(AXIS2_INTDIR_CLI)\dyn_cli
	@if not exist $(AXIS2_INTDIR_CLI)\gslc mkdir $(AXIS2_INTDIR_CLI)\gslc
	@if not exist $(AXIS2_INTDIR_CLI)\yahoo mkdir $(AXIS2_INTDIR_CLI)\yahoo
	@if not exist $(AXIS2_INTDIR_CLI)\notify mkdir $(AXIS2_INTDIR_CLI)\notify
	@if not exist $(AXIS2_INTDIR_CLI)\mtom mkdir $(AXIS2_INTDIR_CLI)\mtom

$(AXIS2_BINS)\samples\math.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\client\math\*.c /Fo$(AXIS2_INTDIR_CLI)\math\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\math\*.obj $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\math.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

axis2_client_math: $(AXIS2_BINS)\samples\math.exe

$(AXIS2_BINS)\samples\echo.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\client\echo\echo.c /Fo$(AXIS2_INTDIR_CLI)\echo\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\echo\*.obj $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\echo.exe 
	-@$(_VC_MANIFEST_EMBED_EXE)

axis2_client_echo: $(AXIS2_BINS)\samples\echo.exe 

$(AXIS2_BINS)\samples\google.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\client\google\*.c /Fo$(AXIS2_INTDIR_CLI)\gslc\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\gslc\*.obj $(AXUTIL).lib $(AXIOM).lib \
	$(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\google.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

axis2_client_google_spell : $(AXIS2_BINS)\samples\google.exe

$(AXIS2_BINS)\samples\yahoo.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\client\yahoo\*.c /Fo$(AXIS2_INTDIR_CLI)\yahoo\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\yahoo\*.obj $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\yahoo.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

axis2_client_yahoo: $(AXIS2_BINS)\samples\yahoo.exe

$(AXIS2_BINS)\samples\notify.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\client\notify\*.c /Fo$(AXIS2_INTDIR_CLI)\notify\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\notify\*.obj $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\notify.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

axis2_client_notify: $(AXIS2_BINS)\samples\notify.exe


$(AXIS2_BINS)\samples\mtom.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\client\mtom\*.c /Fo$(AXIS2_INTDIR_CLI)\mtom\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR_CLI)\mtom\*.obj $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\mtom.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

	@if not exist  $(AXIS2_BINS)\samples\resources mkdir $(AXIS2_BINS)\samples\resources 
	@xcopy /Y $(AXIS2_SOURCE_DIR)\samples\client\mtom\resources $(AXIS2_BINS)\samples\resources 

axis2_client_mtom: $(AXIS2_BINS)\samples\mtom.exe

axis2_client_userguide_int:
	@if not exist $(AXIS2_INTDIR)\userguide\echo_blocking mkdir $(AXIS2_INTDIR)\userguide\echo_blocking
	@if not exist $(AXIS2_INTDIR)\userguide\echo_non_blocking mkdir $(AXIS2_INTDIR)\userguide\echo_non_blocking
	@if not exist $(AXIS2_INTDIR)\userguide\echo_blocking_addr mkdir $(AXIS2_INTDIR)\userguide\echo_blocking_addr
	@if not exist $(AXIS2_INTDIR)\userguide\echo_rest mkdir $(AXIS2_INTDIR)\userguide\echo_rest
	@if not exist $(AXIS2_INTDIR)\userguide\echo_blocking_dual mkdir $(AXIS2_INTDIR)\userguide\echo_blocking_dual
	@if not exist $(AXIS2_INTDIR)\userguide\echo_non_blocking_dual mkdir $(AXIS2_INTDIR)\userguide\echo_non_blocking_dual
	@if not exist $(AXIS2_INTDIR)\userguide\echo_blocking_soap11 mkdir $(AXIS2_INTDIR)\userguide\echo_blocking_soap11
	@if not exist $(AXIS2_INTDIR)\userguide\echo_blocking_auth mkdir $(AXIS2_INTDIR)\userguide\echo_blocking_auth

$(AXIS2_BINS)\samples\echo_blocking.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_blocking\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_blocking\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\echo_blocking.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

$(AXIS2_BINS)\samples\echo_non_blocking.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_non_blocking.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_non_blocking\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_non_blocking\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/OUT:$(AXIS2_BINS)\samples\echo_non_blocking.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

$(AXIS2_BINS)\samples\echo_blocking_addr.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_addr.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_blocking_addr\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_blocking_addr\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/OUT:$(AXIS2_BINS)\samples\echo_blocking_addr.exe
        -@$(_VC_MANIFEST_EMBED_EXE)

$(AXIS2_BINS)\samples\echo_rest.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_rest.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_rest\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_rest\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/OUT:$(AXIS2_BINS)\samples\echo_rest.exe
        -@$(_VC_MANIFEST_EMBED_EXE)

$(AXIS2_BINS)\samples\echo_blocking_dual.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_dual.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_blocking_dual\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_blocking_dual\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\echo_blocking_dual.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

$(AXIS2_BINS)\samples\echo_non_blocking_dual.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_non_blocking_dual.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_non_blocking_dual\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_non_blocking_dual\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) $(AXIS2_ENGINE).lib \
	/OUT:$(AXIS2_BINS)\samples\echo_non_blocking_dual.exe
        -@$(_VC_MANIFEST_EMBED_EXE)

$(AXIS2_BINS)\samples\echo_blocking_soap11.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_soap11.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_blocking_soap11\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_blocking_soap11\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\echo_blocking_soap11.exe
        -@$(_VC_MANIFEST_EMBED_EXE)


$(AXIS2_BINS)\samples\echo_blocking_auth.exe :
	$(CC) $(CFLAGS) $(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_blocking_auth.c \
	$(AXIS2_SOURCE_DIR)\samples\user_guide\clients\echo_util.c /Fo$(AXIS2_INTDIR)\userguide\echo_blocking_auth\ /c
	$(LD) $(LDFLAGS) $(AXIS2_INTDIR)\userguide\echo_blocking_auth\*.obj $(AXUTIL).lib \
	$(AXIOM).lib $(AXIS2_PARSER).lib $(LIBS) \
	$(AXIS2_ENGINE).lib  /OUT:$(AXIS2_BINS)\samples\echo_blocking_auth.exe
        -@$(_VC_MANIFEST_EMBED_EXE)


axis2_client_userguide : axis2_client_userguide_int $(AXIS2_BINS)\samples\echo_blocking_soap11.exe \
	$(AXIS2_BINS)\samples\echo_non_blocking_dual.exe $(AXIS2_BINS)\samples\echo_blocking.exe \
	$(AXIS2_BINS)\samples\echo_blocking_dual.exe $(AXIS2_BINS)\samples\echo_rest.exe  \
	$(AXIS2_BINS)\samples\echo_blocking_addr.exe $(AXIS2_BINS)\samples\echo_non_blocking.exe \
	$(AXIS2_BINS)\samples\echo_blocking_auth.exe

clean_manifest: 
	@del /s $(AXIS2_BINDIR)\*.manifest

	
copy_extra:
	@copy $(AXIS2_SOURCE_DIR)\INSTALL $(AXIS2_BINDIR)
	@copy $(AXIS2_SOURCE_DIR)\LICENSE $(AXIS2_BINDIR)
	@copy $(AXIS2_SOURCE_DIR)\CREDITS $(AXIS2_BINDIR)
	@copy $(AXIS2_SOURCE_DIR)\README $(AXIS2_BINDIR)
	@copy $(AXIS2_SOURCE_DIR)\AUTHORS $(AXIS2_BINDIR)
	@copy $(AXIS2_SOURCE_DIR)\NEWS $(AXIS2_BINDIR)
	@copy $(AXIS2_SOURCE_DIR)\NOTICE $(AXIS2_BINDIR)
	@if exist $(AXIS2_SOURCE_DIR)\docs xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\docs $(AXIS2_BINDIR)\docs

copy_samples:
	@if not exist $(AXIS2_BINDIR)\samples mkdir $(AXIS2_BINDIR)\samples

	@if not exist $(AXIS2_BINDIR)\samples\client mkdir $(AXIS2_BINDIR)\samples\client
	@if not exist $(AXIS2_BINDIR)\samples\user_guide mkdir $(AXIS2_BINDIR)\samples\user_guide

	@if not exist $(AXIS2_BINDIR)\samples\client\echo mkdir $(AXIS2_BINDIR)\samples\client\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\client\echo\echo.c $(AXIS2_BINDIR)\samples\client\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\client\echo\README.txt $(AXIS2_BINDIR)\samples\client\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\client\echo\echo.mk $(AXIS2_BINDIR)\samples\client\echo

	@if not exist $(AXIS2_BINDIR)\samples\client\math mkdir $(AXIS2_BINDIR)\samples\client\math
	@copy $(AXIS2_SOURCE_DIR)\samples\client\math\*.c $(AXIS2_BINDIR)\samples\client\math
	@copy $(AXIS2_SOURCE_DIR)\samples\client\math\*.h $(AXIS2_BINDIR)\samples\client\math
	@copy $(AXIS2_SOURCE_DIR)\samples\client\math\math.mk $(AXIS2_BINDIR)\samples\client\math


	@if not exist $(AXIS2_BINDIR)\samples\client\google mkdir $(AXIS2_BINDIR)\samples\client\google
	@copy $(AXIS2_SOURCE_DIR)\samples\client\google\*.c $(AXIS2_BINDIR)\samples\client\google
	@copy $(AXIS2_SOURCE_DIR)\samples\client\google\README.txt $(AXIS2_BINDIR)\samples\client\google
        @copy $(AXIS2_SOURCE_DIR)\samples\client\google\google.mk $(AXIS2_BINDIR)\samples\client\google

	@if not exist $(AXIS2_BINDIR)\samples\client\notify mkdir $(AXIS2_BINDIR)\samples\client\notify
	@copy $(AXIS2_SOURCE_DIR)\samples\client\notify\*.c $(AXIS2_BINDIR)\samples\client\notify
	@copy $(AXIS2_SOURCE_DIR)\samples\client\notify\notify.mk $(AXIS2_BINDIR)\samples\client\notify

	@if not exist $(AXIS2_BINDIR)\samples\client\mtom\resources mkdir $(AXIS2_BINDIR)\samples\client\mtom\resources
	@copy $(AXIS2_SOURCE_DIR)\samples\client\mtom\*.c $(AXIS2_BINDIR)\samples\client\mtom
	@copy $(AXIS2_SOURCE_DIR)\samples\client\mtom\resources\*.jpg $(AXIS2_BINDIR)\samples\client\mtom\resources
	@copy $(AXIS2_SOURCE_DIR)\samples\client\mtom\mtom.mk $(AXIS2_BINDIR)\samples\client\mtom

	@if not exist $(AXIS2_BINDIR)\samples\client\yahoo mkdir $(AXIS2_BINDIR)\samples\client\yahoo
	@copy $(AXIS2_SOURCE_DIR)\samples\client\yahoo\*.c $(AXIS2_BINDIR)\samples\client\yahoo
	@copy $(AXIS2_SOURCE_DIR)\samples\client\yahoo\yahoo.mk $(AXIS2_BINDIR)\samples\client\yahoo

	@xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\samples\user_guide $(AXIS2_BINDIR)\samples\user_guide

	@if not exist $(AXIS2_BINDIR)\samples\server mkdir $(AXIS2_BINDIR)\samples\server

	@if not exist $(AXIS2_BINDIR)\samples\server\Calculator mkdir $(AXIS2_BINDIR)\samples\server\Calculator
	@copy $(AXIS2_SOURCE_DIR)\samples\server\Calculator\*.c $(AXIS2_BINDIR)\samples\server\Calculator 
	@copy $(AXIS2_SOURCE_DIR)\samples\server\Calculator\*.h $(AXIS2_BINDIR)\samples\server\Calculator 
	@copy $(AXIS2_SOURCE_DIR)\samples\server\Calculator\*.xml $(AXIS2_BINDIR)\samples\server\Calculator 
	@copy $(AXIS2_SOURCE_DIR)\samples\server\Calculator\Calculator.mk $(AXIS2_BINDIR)\samples\server\Calculator 

	@if not exist $(AXIS2_BINDIR)\samples\server\echo mkdir $(AXIS2_BINDIR)\samples\server\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.c $(AXIS2_BINDIR)\samples\server\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.h $(AXIS2_BINDIR)\samples\server\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\server\echo\*.xml $(AXIS2_BINDIR)\samples\server\echo
	@copy $(AXIS2_SOURCE_DIR)\samples\server\echo\echo.mk $(AXIS2_BINDIR)\samples\server\echo

	@if not exist $(AXIS2_BINDIR)\samples\server\math mkdir $(AXIS2_BINDIR)\samples\server\math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.c $(AXIS2_BINDIR)\samples\server\math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.h $(AXIS2_BINDIR)\samples\server\math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\math\*.xml $(AXIS2_BINDIR)\samples\server\math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\math\math.mk $(AXIS2_BINDIR)\samples\server\math

	@if not exist $(AXIS2_BINDIR)\samples\server\notify mkdir $(AXIS2_BINDIR)\samples\server\notify
	@copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.c $(AXIS2_BINDIR)\samples\server\notify
	@copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.h $(AXIS2_BINDIR)\samples\server\notify
	@copy $(AXIS2_SOURCE_DIR)\samples\server\notify\*.xml $(AXIS2_BINDIR)\samples\server\notify
        @copy $(AXIS2_SOURCE_DIR)\samples\server\notify\notify.mk $(AXIS2_BINDIR)\samples\server\notify

	@if not exist $(AXIS2_BINDIR)\samples\server\sg_math mkdir $(AXIS2_BINDIR)\samples\server\sg_math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\*.c $(AXIS2_BINDIR)\samples\server\sg_math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\*.h $(AXIS2_BINDIR)\samples\server\sg_math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\*.xml $(AXIS2_BINDIR)\samples\server\sg_math
	@copy $(AXIS2_SOURCE_DIR)\samples\server\sg_math\sg_math.mk $(AXIS2_BINDIR)\samples\server\sg_math

	@if not exist $(AXIS2_BINDIR)\samples\server\mtom mkdir $(AXIS2_BINDIR)\samples\server\mtom
	@copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\*.c $(AXIS2_BINDIR)\samples\server\mtom
	@copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\*.h $(AXIS2_BINDIR)\samples\server\mtom
	@copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\*.xml $(AXIS2_BINDIR)\samples\server\mtom
        @copy $(AXIS2_SOURCE_DIR)\samples\server\mtom\mtom.mk $(AXIS2_BINDIR)\samples\server\mtom

copy_include: 
	@xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\include $(AXIS2_BINDIR)\include\

	@if exist $(AXIS2_BINDIR)\include\Makefile.am del $(AXIS2_BINDIR)\include\Makefile.am

	@xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\axiom\include $(AXIS2_BINDIR)\include\

	@if exist $(AXIS2_BINDIR)\include\Makefile.am del $(AXIS2_BINDIR)\include\Makefile.am

	@xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\util\include $(AXIS2_BINDIR)\include\

	@if exist $(AXIS2_BINDIR)\include\Makefile.am del $(AXIS2_BINDIR)\include\Makefile.am

    	@xcopy /E /I /Y $(AXIS2_SOURCE_DIR)\neethi\include $(AXIS2_BINDIR)\include\
	

copy_vc_projects:
	@if not exist $(AXIS2_BINDIR)\ides mkdir $(AXIS2_BINDIR)\ides
	@if not exist $(AXIS2_BINDIR)\ides\vc mkdir $(AXIS2_BINDIR)\ides\vc
	@if not exist $(AXIS2_BINDIR)\ides\vc\samples mkdir $(AXIS2_BINDIR)\ides\vc\samples


	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients
	
	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo\echo.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\math mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\math
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\math\math.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\math

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\google mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\google
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\google\google.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\google

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\mtom mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\mtom
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\mtom\mtom.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\mtom

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\notify mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\notify
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\notify\notify.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\notify

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\yahoo mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\yahoo
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\yahoo\yahoo.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\yahoo

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo_blocking\echo_blocking.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking_addr mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking_addr
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo_blocking_addr\echo_blocking_addr.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking_addr

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking_dual mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking_dual
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo_blocking_dual\echo_blocking_dual.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_blocking_dual

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_non_blocking mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_non_blocking
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo_non_blocking\echo_non_blocking.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_non_blocking

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_non_blocking_dual mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_non_blocking_dual
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo_non_blocking_dual\echo_non_blocking_dual.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_non_blocking_dual

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_rest mkdir $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_rest
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\echo_rest\echo_rest.vcproj $(AXIS2_BINDIR)\ides\vc\samples\clients\echo_rest

	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\clients\clients.sln $(AXIS2_BINDIR)\ides\vc\samples\clients


 	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\services mkdir $(AXIS2_BINDIR)\ides\vc\samples\services

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\services\echo mkdir $(AXIS2_BINDIR)\ides\vc\samples\services\echo
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\services\echo\echo.vcproj $(AXIS2_BINDIR)\ides\vc\samples\services\echo

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\services\math mkdir $(AXIS2_BINDIR)\ides\vc\samples\services\math
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\services\math\math.vcproj $(AXIS2_BINDIR)\ides\vc\samples\services\math

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\services\mtom mkdir $(AXIS2_BINDIR)\ides\vc\samples\services\mtom
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\services\mtom\mtom.vcproj $(AXIS2_BINDIR)\ides\vc\samples\services\mtom

	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\services\notify mkdir $(AXIS2_BINDIR)\ides\vc\samples\services\notify
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\services\notify\notify.vcproj $(AXIS2_BINDIR)\ides\vc\samples\services\notify
	@if not exist $(AXIS2_BINDIR)\ides\vc\samples\services\Calculator mkdir $(AXIS2_BINDIR)\ides\vc\samples\services\Calculator
	@copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\services\Calculator\Calculator.vcproj $(AXIS2_BINDIR)\ides\vc\samples\services\Calculator

      @copy $(AXIS2_SOURCE_DIR)\ides\vc\samples\services\services.sln $(AXIS2_BINDIR)\ides\vc\samples\services


mv_dist:
	cd $(AXIS2_BINDIR)
	cd ..
	@if exist $(AXIS2_RELEASE_DIR) rmdir /S /Q $(AXIS2_RELEASE_DIR)
#	rename  deploy $(AXIS2_RELEASE_DIR)
	@xcopy /Y /E /I deploy $(AXIS2_RELEASE_DIR)


all_services: axis2_services_echo axis2_services_math axis2_services_notify axis2_services_sg_math axis2_services_mtom axis2_services_calculator 

all_clients: axis2_clients_intdir axis2_client_echo axis2_client_math  axis2_client_google_spell axis2_client_yahoo axis2_client_notify axis2_client_mtom axis2_client_userguide 

axis2_samples: axis2_mod_log all_services all_clients

axis2_server_modules: axis2_apache_module axis2_IIS_module

################################# tools ##################################################
# tcpmon
TCPMON_SRC = $(AXIS2_SOURCE_DIR)\tools\tcpmon\src
TCPMON_INTDIR = $(AXIS2_INTDIR)\tools\tcpmon

TCPMON_OBJS = $(TCPMON_INTDIR)\*.obj

$(TCPMON_INTDIR) : 
	@if not exist $(TCPMON_INTDIR) mkdir $(TCPMON_INTDIR)

{$(TCPMON_SRC)}.c{$(TCPMON_INTDIR)}.obj ::
	$(CC) $(CFLAGS) /Fo$(TCPMON_INTDIR)\ /c $<

$(TCPMON_OBJS) : $(TCPMON_INTDIR)

$(AXIS2_TOOLS)\tcpmon.exe : $(TCPMON_OBJS)
	@if not exist $(AXIS2_TOOLS) mkdir $(AXIS2_TOOLS)
	$(LD) $(LDFLAGS) $(TCPMON_OBJS) $(AXUTIL).lib $(AXIOM).lib $(AXIS2_PARSER).lib /OUT:$(AXIS2_TOOLS)\tcpmon.exe
	-@$(_VC_MANIFEST_EMBED_EXE)

tcpmon: $(AXIS2_TOOLS)\tcpmon.exe



##############################################################################################

build: deploy axis2_core axis2_samples copy_include
	
build_with_guththila : deploy axis2_core_guththila axis2_samples copy_include

!if "$(WITH_TCP)" == "1"
all: $(PARSER) axis2_tcp
!else
all: $(PARSER)
!endif
	
##############################################################################################

install: all copy_extra
	
dist: install axis2_apache_module axis2_IIS_module tcpmon copy_samples copy_vc_projects clean_manifest mv_dist

dist_as_lib : deploy axis2_core_without_server copy_include clean_manifest mv_dist

