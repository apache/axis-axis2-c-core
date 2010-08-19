# Microsoft Developer Studio Project File - Name="axutil" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axutil - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axutil.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axutil.mak" CFG="axutil - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axutil - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axutil - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axutil - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXUTIL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXUTIL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "NDEBUG"
# ADD RSC /l 0xc0c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "axutil - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXUTIL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Z7 /Od /I "c:\progra~1\micros~4\include" /I "..\..\..\..\util\include" /I "..\..\..\..\util\src\\" /I "..\..\..\..\util\src\minizip\\" /I "..\..\..\..\axiom\include" /I "..\..\..\..\axiom\src\om" /I "..\..\..\..\axiom\src\soap" /I "..\..\..\..\util\include\platforms" /I "..\..\..\..\neethi\include" /I "..\..\..\..\neethi\src" /I "..\..\..\..\neethi\src\secpolicy\builder" /I "..\..\..\..\neethi\src\secpolicy\model" /I "..\..\..\..\src\core\clientapi" /I "..\..\..\..\src\core\deployment" /I "..\..\..\..\src\core\description" /I "..\..\..\..\src\core\transport" /I "..\..\..\..\src\core\transport\tcp" /I "..\..\..\..\include" /I "..\..\..\..\src\core\engine" /I "..\..\..\..\src\core\context" /I "..\..\..\..\src\core\util" /I "..\..\..\..\src\core\transport\http\server\apache2" /I "..\..\..\..\axiom\src\attachments" /I "..\..\..\..\tools\tcpmon\include" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_DEBUG" /D "AXIS2_DECLARE_EXPORT" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_SECURE_NO_WARNINGS" /D "AXIS2_SVR_MULTI_THREADED" /I..\..\..\..\axis2c_deps\libxml2-2.6.27.win32\include /I..\..\..\..\axis2c_deps\iconv-1.9.2.win32\include /I..\..\..\..\axis2c_deps\zlib-1.2.3.win32\include /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG"
# ADD RSC /l 0xc0c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Rpcrt4.lib Ws2_32.lib zdll.lib /nologo /dll /incremental:no /pdb:"../../../../build/deploy/lib/axutil.pdb" /debug /machine:I386 /out:"../../../../build/deploy/lib/axutil.dll" /pdbtype:sept /libpath:"../../../../build/deploy/lib" /libpath:"c:/progra~1/micros~4/lib" /libpath:"..\..\..\..\axis2c_deps\zlib-1.2.3.win32\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\debug\*.lib .\..\..\..\..\build\deploy\lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "axutil - Win32 Release"
# Name "axutil - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\util\src\allocator.c
DEP_CPP_ALLOC=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\archive_extract.c
DEP_CPP_ARCHI=\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\src\minizip\axis2_crypt.h"\
	"..\..\..\..\util\src\minizip\axis2_ioapi.h"\
	"..\..\..\..\util\src\minizip\axis2_iowin32.h"\
	"..\..\..\..\util\src\minizip\axis2_unzip.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
NODEP_CPP_ARCHI=\
	"..\..\..\..\util\src\minizip\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\array_list.c
DEP_CPP_ARRAY=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\axutil_windows.c
DEP_CPP_AXUTI=\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\base64.c
DEP_CPP_BASE6=\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\base64_binary.c
DEP_CPP_BASE64=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\class_loader.c
DEP_CPP_CLASS=\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_param.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\crypt.c
DEP_CPP_CRYPT=\
	"..\..\..\..\util\src\minizip\axis2_crypt.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\date_time.c
DEP_CPP_DATE_=\
	"..\..\..\..\util\include\axutil_date_time_util.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\date_time_util.c
DEP_CPP_DATE_T=\
	"..\..\..\..\util\include\axutil_date_time_util.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\date_time_util_windows.c
DEP_CPP_DATE_TI=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\digest_calc.c
DEP_CPP_DIGES=\
	"..\..\..\..\util\include\axutil_digest_calc.h"\
	"..\..\..\..\util\include\axutil_md5.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\dir_handler.c
DEP_CPP_DIR_H=\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\src\minizip\axis2_archive_extract.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\dir_windows.c
DEP_CPP_DIR_W=\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\dll_desc.c
DEP_CPP_DLL_D=\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_param.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\duration.c
DEP_CPP_DURAT=\
	"..\..\..\..\util\include\axutil_duration.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\env.c
DEP_CPP_ENV_C=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_error_default.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_log_default.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\error.c
DEP_CPP_ERROR=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_error_default.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\file.c
DEP_CPP_FILE_=\
	"..\..\..\..\util\include\axutil_file.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\file_handler.c
DEP_CPP_FILE_H=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_file_handler.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\generic_obj.c
DEP_CPP_GENER=\
	"..\..\..\..\util\include\axutil_generic_obj.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\getopt_windows.c
DEP_CPP_GETOP=\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\hash.c
DEP_CPP_HASH_=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\http_chunked_stream.c
DEP_CPP_HTTP_=\
	"..\..\..\..\util\include\axutil_http_chunked_stream.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_stream.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\ioapi.c
DEP_CPP_IOAPI=\
	"..\..\..\..\util\src\minizip\axis2_ioapi.h"\
	
NODEP_CPP_IOAPI=\
	"..\..\..\..\util\src\minizip\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\iowin32.c
DEP_CPP_IOWIN=\
	"..\..\..\..\util\src\minizip\axis2_ioapi.h"\
	"..\..\..\..\util\src\minizip\axis2_iowin32.h"\
	
NODEP_CPP_IOWIN=\
	"..\..\..\..\util\src\minizip\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\linked_list.c
DEP_CPP_LINKE=\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\log.c
DEP_CPP_LOG_C=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_file_handler.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_log_default.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\md5.c
DEP_CPP_MD5_C=\
	"..\..\..\..\util\include\axutil_md5.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_error_default.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\network_handler.c
DEP_CPP_NETWO=\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\param.c
DEP_CPP_PARAM=\
	"..\..\..\..\util\include\axutil_generic_obj.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_param.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\param_container.c
DEP_CPP_PARAM_=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_param.h"\
	{$(INCLUDE)}"axutil_param_container.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\properties.c
DEP_CPP_PROPE=\
	"..\..\..\..\util\include\axutil_properties.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\property.c
DEP_CPP_PROPER=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_property.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\qname.c
DEP_CPP_QNAME=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\rand.c
DEP_CPP_RAND_=\
	"..\..\..\..\util\include\axutil_rand.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\stack.c
DEP_CPP_STACK=\
	"..\..\..\..\util\include\axutil_stack.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\stream.c
DEP_CPP_STREA=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_stream.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\string.c
DEP_CPP_STRIN=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\string_util.c
DEP_CPP_STRING=\
	"..\..\..\..\util\include\axutil_string_util.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\thread_mutex_windows.c
DEP_CPP_THREA=\
	"..\..\..\..\util\include\platforms\windows\axutil_thread_mutex_windows.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\thread_pool.c
DEP_CPP_THREAD=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_error_default.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\thread_windows.c
DEP_CPP_THREAD_=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\types.c
DEP_CPP_TYPES=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_types.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\unzip.c
DEP_CPP_UNZIP=\
	"..\..\..\..\util\src\minizip\axis2_crypt.h"\
	"..\..\..\..\util\src\minizip\axis2_ioapi.h"\
	"..\..\..\..\util\src\minizip\axis2_unzip.h"\
	
NODEP_CPP_UNZIP=\
	"..\..\..\..\util\src\minizip\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\uri.c
DEP_CPP_URI_C=\
	"..\..\..\..\util\include\axutil_uri.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\url.c
DEP_CPP_URL_C=\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_file_handler.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_types.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\utils.c
DEP_CPP_UTILS=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\uuid_gen.c
DEP_CPP_UUID_=\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\axutil_platform_auto_sense.h"\
	{$(INCLUDE)}"platforms\windows\axutil_date_time_util_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_dir_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_getopt_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_thread_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	{$(INCLUDE)}"platforms\windows\axutil_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\platforms\windows\uuid_gen_windows.c
DEP_CPP_UUID_G=\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"platforms\windows\axutil_uuid_gen_windows.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\version.c
DEP_CPP_VERSI=\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	{$(INCLUDE)}"axutil_version.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\axis2_archive_extract.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\axis2_crypt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\axis2_ioapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\axis2_iowin32.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\minizip\axis2_unzip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_array_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_base64.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_base64_binary.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_class_loader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_date_time.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_date_time_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_date_time_util_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_dir_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_dir_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_dll_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_duration.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_env.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_error_default.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_file.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_file_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_generic_obj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_getopt_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_hash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_linked_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_log.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_log_default.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_network_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_param.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_param_container.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\axutil_platform_auto_sense.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_properties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_property.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_qname.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_rand.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_stack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_stomp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_stomp_frame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_stream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_string.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_string_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_thread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_thread_mutex_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_thread_pool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_thread_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_uri.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_url.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_utils_defines.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_uuid_gen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_uuid_gen_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\axutil_version.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\include\platforms\windows\axutil_windows.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
