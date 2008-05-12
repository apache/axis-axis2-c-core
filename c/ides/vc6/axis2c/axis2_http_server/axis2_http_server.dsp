# Microsoft Developer Studio Project File - Name="axis2_http_server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=axis2_http_server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axis2_http_server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axis2_http_server.mak" CFG="axis2_http_server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axis2_http_server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "axis2_http_server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axis2_http_server - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0xc0c /d "NDEBUG"
# ADD RSC /l 0xc0c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "axis2_http_server - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Z7 /Od /I "..\..\..\..\util\include" /I "..\..\..\..\util\src\\" /I "..\..\..\..\util\src\minizip\\" /I "..\..\..\..\axiom\include" /I "..\..\..\..\axiom\src\om" /I "..\..\..\..\axiom\src\soap" /I "..\..\..\..\util\include\platforms" /I "..\..\..\..\neethi\include" /I "..\..\..\..\neethi\src" /I "..\..\..\..\neethi\src\secpolicy\builder" /I "..\..\..\..\neethi\src\secpolicy\model" /I "..\..\..\..\src\core\clientapi" /I "..\..\..\..\src\core\deployment" /I "..\..\..\..\src\core\description" /I "..\..\..\..\src\core\transport" /I "..\..\..\..\src\core\transport\tcp" /I "..\..\..\..\include" /I "..\..\..\..\src\core\engine" /I "..\..\..\..\src\core\context" /I "..\..\..\..\src\core\util" /I "..\..\..\..\src\core\transport\http\server\apache2" /I "..\..\..\..\axiom\src\attachments" /I "..\..\..\..\tools\tcpmon\include" /D "WIN32" /D "_MBCS" /D "_DEBUG" /D "AXIS2_DECLARE_EXPORT" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_SECURE_NO_WARNINGS" /D "AXIS2_SVR_MULTI_THREADED" /D "_CONSOLE" /FR /I..\..\..\..\axis2c_deps\libxml2-2.6.27.win32\include /I..\..\..\..\axis2c_deps\iconv-1.9.2.win32\include /I..\..\..\..\axis2c_deps\zlib-1.2.3.win32\include /c
# ADD BASE RSC /l 0xc0c /d "_DEBUG"
# ADD RSC /l 0xc0c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axutil.lib axis2_engine.lib axis2_parser.lib axis2_http_receiver.lib /nologo /subsystem:console /incremental:no /pdb:"../../../../build/deploy/lib/axis2_http_server.pdb" /debug /machine:I386 /out:"../../../../build/deploy/bin/axis2_http_server.exe" /pdbtype:sept /libpath:"../../../../build/deploy/lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=if not exist .\..\..\..\..\build\deploy\logs mkdir .\..\..\..\..\build\deploy\logs	copy .\debug\axis2_http_server.exe .\..\..\..\..\build\deploy\lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "axis2_http_server - Win32 Release"
# Name "axis2_http_server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\server\simple_axis2_server\http_server_main.c
DEP_CPP_HTTP_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_http_server.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_msg_recv.h"\
	"..\..\..\..\include\axis2_op.h"\
	"..\..\..\..\include\axis2_op_ctx.h"\
	"..\..\..\..\include\axis2_phase.h"\
	"..\..\..\..\include\axis2_phase_holder.h"\
	"..\..\..\..\include\axis2_phase_meta.h"\
	"..\..\..\..\include\axis2_phase_resolver.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_phases_info.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc.h"\
	"..\..\..\..\include\axis2_svc_ctx.h"\
	"..\..\..\..\include\axis2_svc_grp.h"\
	"..\..\..\..\include\axis2_svc_grp_ctx.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\include\axis2_svc_skeleton.h"\
	"..\..\..\..\include\axis2_svr_callback.h"\
	"..\..\..\..\include\axis2_transport_in_desc.h"\
	"..\..\..\..\include\axis2_transport_out_desc.h"\
	"..\..\..\..\include\axis2_transport_receiver.h"\
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_version.h"\
	"..\..\..\..\util\include\platforms\axutil_platform_auto_sense.h"\
	"..\..\..\..\util\include\platforms\unix\axutil_date_time_util_unix.h"\
	"..\..\..\..\util\include\platforms\unix\axutil_unix.h"\
	"..\..\..\..\util\include\platforms\unix\axutil_uuid_gen_unix.h"\
	"..\..\..\..\util\include\platforms\windows\axutil_date_time_util_windows.h"\
	"..\..\..\..\util\include\platforms\windows\axutil_dir_windows.h"\
	"..\..\..\..\util\include\platforms\windows\axutil_getopt_windows.h"\
	"..\..\..\..\util\include\platforms\windows\axutil_thread_windows.h"\
	"..\..\..\..\util\include\platforms\windows\axutil_uuid_gen_windows.h"\
	"..\..\..\..\util\include\platforms\windows\axutil_windows.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
