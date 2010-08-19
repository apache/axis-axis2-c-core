# Microsoft Developer Studio Project File - Name="axiom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axiom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axiom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axiom.mak" CFG="axiom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axiom - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axiom - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axiom - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIOM_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIOM_EXPORTS" /YX /FD /c
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

!ELSEIF  "$(CFG)" == "axiom - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIOM_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Z7 /Od /I "..\..\..\..\util\include" /I "..\..\..\..\util\src\\" /I "..\..\..\..\util\src\minizip\\" /I "..\..\..\..\axiom\include" /I "..\..\..\..\axiom\src\om" /I "..\..\..\..\axiom\src\soap" /I "..\..\..\..\util\include\platforms" /I "..\..\..\..\neethi\include" /I "..\..\..\..\neethi\src" /I "..\..\..\..\neethi\src\secpolicy\builder" /I "..\..\..\..\neethi\src\secpolicy\model" /I "..\..\..\..\src\core\clientapi" /I "..\..\..\..\src\core\deployment" /I "..\..\..\..\src\core\description" /I "..\..\..\..\src\core\transport" /I "..\..\..\..\src\core\transport\tcp" /I "..\..\..\..\include" /I "..\..\..\..\src\core\engine" /I "..\..\..\..\src\core\context" /I "..\..\..\..\src\core\util" /I "..\..\..\..\src\core\transport\http\server\apache2" /I "..\..\..\..\axiom\src\attachments" /I "..\..\..\..\tools\tcpmon\include" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_DEBUG" /D "AXIS2_DECLARE_EXPORT" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_SECURE_NO_WARNINGS" /D "AXIS2_SVR_MULTI_THREADED" /I..\..\..\..\axis2c_deps\libxml2-2.6.27.win32\include /I..\..\..\..\axis2c_deps\iconv-1.9.2.win32\include /I..\..\..\..\axis2c_deps\zlib-1.2.3.win32\include /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG"
# ADD RSC /l 0xc0c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axutil.lib axis2_parser.lib /nologo /dll /incremental:no /pdb:"../../../../build/deploy/lib/axiom.pdb" /debug /machine:I386 /out:"../../../../build/deploy/lib/axiom.dll" /pdbtype:sept /libpath:"../../../../build/deploy/lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\debug\*.lib .\..\..\..\..\build\deploy\lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "axiom - Win32 Release"
# Name "axiom - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\axiom\src\attachments\data_handler.c
DEP_CPP_DATA_=\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
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

SOURCE=..\..\..\..\axiom\src\attachments\mime_body_part.c
DEP_CPP_MIME_=\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\src\attachments\axiom_mime_body_part.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
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

SOURCE=..\..\..\..\axiom\src\attachments\mime_output.c
DEP_CPP_MIME_O=\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_mime_const.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\attachments\axiom_mime_body_part.h"\
	"..\..\..\..\axiom\src\attachments\axiom_mime_output.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\attachments\mime_parser.c
DEP_CPP_MIME_P=\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_mime_const.h"\
	"..\..\..\..\axiom\include\axiom_mime_parser.h"\
	"..\..\..\..\util\include\axutil_http_chunked_stream.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_stream.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_attribute.c
DEP_CPP_OM_AT=\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_child_element_iterator.c
DEP_CPP_OM_CH=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_children_iterator.c
DEP_CPP_OM_CHI=\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_children_qname_iterator.c
DEP_CPP_OM_CHIL=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_children_with_specific_attribute_iterator.c
DEP_CPP_OM_CHILD=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_comment.c
DEP_CPP_OM_CO=\
	"..\..\..\..\axiom\include\axiom_comment.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_data_source.c
DEP_CPP_OM_DA=\
	"..\..\..\..\axiom\include\axiom_data_source.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_stream.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_doctype.c
DEP_CPP_OM_DO=\
	"..\..\..\..\axiom\include\axiom_doctype.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_document.c
DEP_CPP_OM_DOC=\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_element.c
DEP_CPP_OM_EL=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_namespace.c
DEP_CPP_OM_NA=\
	"..\..\..\..\axiom\src\om\axiom_namespace_internal.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_navigator.c
DEP_CPP_OM_NAV=\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	{$(INCLUDE)}"axiom_node.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_node.c
DEP_CPP_OM_NO=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_comment.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_data_source.h"\
	"..\..\..\..\axiom\include\axiom_doctype.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_stream.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_output.c
DEP_CPP_OM_OU=\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\attachments\axiom_mime_output.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_processing_instruction.c
DEP_CPP_OM_PR=\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
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

SOURCE=..\..\..\..\axiom\src\om\om_stax_builder.c
DEP_CPP_OM_ST=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_comment.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_doctype.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	"..\..\..\..\axiom\src\om\axiom_stax_builder_internal.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\om_text.c
DEP_CPP_OM_TE=\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
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

SOURCE=..\..\..\..\axiom\src\util\om_util.c
DEP_CPP_OM_UT=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_comment.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_data_source.h"\
	"..\..\..\..\axiom\include\axiom_doctype.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_util.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_stream.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap11_builder_helper.c
DEP_CPP_SOAP1=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	"..\..\..\..\axiom\src\om\axiom_stax_builder_internal.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_body.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\src\soap\axiom_soap11_builder_helper.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap12_builder_helper.c
DEP_CPP_SOAP12=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_node.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_body.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_envelope.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_node.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\src\soap\axiom_soap12_builder_helper.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_body.c
DEP_CPP_SOAP_=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_body.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_envelope.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_value.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_builder.c
DEP_CPP_SOAP_B=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_stax_builder_internal.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_body.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_envelope.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_header.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_header_block.h"\
	"..\..\..\..\axiom\src\soap\axiom_soap11_builder_helper.h"\
	"..\..\..\..\axiom\src\soap\axiom_soap12_builder_helper.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_envelope.c
DEP_CPP_SOAP_E=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_namespace_internal.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_envelope.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault.c
DEP_CPP_SOAP_F=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_node.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_body.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_code.c
DEP_CPP_SOAP_FA=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_code.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_detail.c
DEP_CPP_SOAP_FAU=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_detail.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_detail.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_node.c
DEP_CPP_SOAP_FAUL=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_node.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_node.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_reason.c
DEP_CPP_SOAP_FAULT=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_role.c
DEP_CPP_SOAP_FAULT_=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_role.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_sub_code.c
DEP_CPP_SOAP_FAULT_S=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_code.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_text.c
DEP_CPP_SOAP_FAULT_T=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_text.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_reason.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_text.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_fault_value.c
DEP_CPP_SOAP_FAULT_V=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault_value.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_code.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_fault_sub_code.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_header.c
DEP_CPP_SOAP_H=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\om\axiom_node_internal.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_envelope.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_header.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\soap_header_block.c
DEP_CPP_SOAP_HE=\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_header.h"\
	"..\..\..\..\axiom\src\soap\_axiom_soap_header_block.h"\
	{$(INCLUDE)}"axiom_attribute.h"\
	{$(INCLUDE)}"axiom_namespace.h"\
	{$(INCLUDE)}"axiom_node.h"\
	{$(INCLUDE)}"axiom_output.h"\
	{$(INCLUDE)}"axiom_xml_reader.h"\
	{$(INCLUDE)}"axiom_xml_writer.h"\
	{$(INCLUDE)}"axutil_allocator.h"\
	{$(INCLUDE)}"axutil_array_list.h"\
	{$(INCLUDE)}"axutil_base64.h"\
	{$(INCLUDE)}"axutil_base64_binary.h"\
	{$(INCLUDE)}"axutil_date_time.h"\
	{$(INCLUDE)}"axutil_env.h"\
	{$(INCLUDE)}"axutil_error.h"\
	{$(INCLUDE)}"axutil_hash.h"\
	{$(INCLUDE)}"axutil_log.h"\
	{$(INCLUDE)}"axutil_qname.h"\
	{$(INCLUDE)}"axutil_string.h"\
	{$(INCLUDE)}"axutil_thread.h"\
	{$(INCLUDE)}"axutil_thread_pool.h"\
	{$(INCLUDE)}"axutil_utils.h"\
	{$(INCLUDE)}"axutil_utils_defines.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_body.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_envelope.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_detail.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_reason.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_role.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_sub_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_text.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_fault_value.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_header.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\_axiom_soap_header_block.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_attribute.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_child_element_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_children_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_children_qname_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_comment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_data_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_data_source.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_doctype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_document.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\attachments\axiom_mime_body_part.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_mime_const.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\attachments\axiom_mime_output.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_mime_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_namespace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\axiom_namespace_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_navigator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\axiom_node_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_output.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_processing_instruction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\axiom_soap11_builder_helper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\soap\axiom_soap12_builder_helper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_body.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_const.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_envelope.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_detail.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_reason.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_role.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_sub_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_text.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_fault_value.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_header.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_soap_header_block.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_stax_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\src\om\axiom_stax_builder_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_text.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_xml_reader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\axiom\include\axiom_xml_writer.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
