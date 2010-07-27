# Microsoft Developer Studio Project File - Name="axis2_engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axis2_engine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axis2_engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axis2_engine.mak" CFG="axis2_engine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axis2_engine - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axis2_engine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axis2_engine - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIS2_ENGINE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIS2_ENGINE_EXPORTS" /YX /FD /c
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

!ELSEIF  "$(CFG)" == "axis2_engine - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIS2_ENGINE_EXPORTS" /YX /FD /GZ /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axiom.lib axutil.lib axis2_parser.lib neethi.lib /nologo /dll /incremental:no /pdb:"../../../../build/deploy/lib/axis2_engine.pdb" /debug /machine:I386 /out:"../../../../build/deploy/lib/axis2_engine.dll" /pdbtype:sept /libpath:"../../../../build/deploy/lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy  .\..\..\..\..\samples\server\axis2.xml   .\..\..\..\..\build\deploy\	copy .\debug\*.lib .\..\..\..\..\build\deploy\lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "axis2_engine - Win32 Release"
# Name "axis2_engine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\addr_disp.c
DEP_CPP_ADDR_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\addr\any_content_type.c
DEP_CPP_ANY_C=\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\arch_file_data.c
DEP_CPP_ARCH_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\arch_reader.c
DEP_CPP_ARCH_R=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_reader.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_module_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_grp_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\async_result.c
DEP_CPP_ASYNC=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_async_result.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\callback.c
DEP_CPP_CALLB=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_async_result.h"\
	"..\..\..\..\include\axis2_callback.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\callback_recv.c
DEP_CPP_CALLBA=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_async_result.h"\
	"..\..\..\..\include\axis2_callback.h"\
	"..\..\..\..\include\axis2_callback_recv.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\conf.c
DEP_CPP_CONF_=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_core_utils.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_reader.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_grp_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\src\core\engine\axis2_disp_checker.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\conf_builder.c
DEP_CPP_CONF_B=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\context\conf_ctx.c
DEP_CPP_CONF_C=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\conf_init.c
DEP_CPP_CONF_I=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_conf_init.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_module.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\util\core_utils.c
DEP_CPP_CORE_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_core_utils.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\context\ctx.c
DEP_CPP_CTX_C=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\ctx_handler.c
DEP_CPP_CTX_H=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\dep_engine.c
DEP_CPP_DEP_E=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_core_utils.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_module.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_reader.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_module_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_grp_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\description\desc.c
DEP_CPP_DESC_=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\desc_builder.c
DEP_CPP_DESC_B=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
	"..\..\..\..\include\axis2_raw_xml_in_out_msg_recv.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_all.h"\
	"..\..\..\..\neethi\include\neethi_assertion.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_engine.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_generic_obj.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\disp.c
DEP_CPP_DISP_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\disp_checker.c
DEP_CPP_DISP_C=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\src\core\engine\axis2_disp_checker.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\addr\endpoint_ref.c
DEP_CPP_ENDPO=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\engine.c
DEP_CPP_ENGIN=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_engine.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\description\flow.c
DEP_CPP_FLOW_=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\flow_container.c
DEP_CPP_FLOW_C=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\handler.c
DEP_CPP_HANDL=\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\handler_desc.c
DEP_CPP_HANDLE=\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\src\http_chunked_stream.c
DEP_CPP_HTTP_=\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_http_chunked_stream.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_header.c
DEP_CPP_HTTP_H=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_out_transport_info.c
DEP_CPP_HTTP_O=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_out_transport_info.h"\
	"..\..\..\..\include\axis2_http_simple_response.h"\
	"..\..\..\..\include\axis2_http_status_line.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_request_line.c
DEP_CPP_HTTP_R=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_request_line.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_response_writer.c
DEP_CPP_HTTP_RE=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_response_writer.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_simple_request.c
DEP_CPP_HTTP_S=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_request_line.h"\
	"..\..\..\..\include\axis2_http_simple_request.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_simple_response.c
DEP_CPP_HTTP_SI=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_simple_response.h"\
	"..\..\..\..\include\axis2_http_status_line.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_status_line.c
DEP_CPP_HTTP_ST=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_status_line.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\util\http_transport_utils.c
DEP_CPP_HTTP_T=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_mime_const.h"\
	"..\..\..\..\axiom\include\axiom_mime_parser.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
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
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_engine.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\include\axis2_http_transport_utils.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_http_chunked_stream.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\http_worker.c
DEP_CPP_HTTP_W=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
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
	"..\..\..\..\include\axis2_engine.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_out_transport_info.h"\
	"..\..\..\..\include\axis2_http_request_line.h"\
	"..\..\..\..\include\axis2_http_response_writer.h"\
	"..\..\..\..\include\axis2_http_simple_request.h"\
	"..\..\..\..\include\axis2_http_simple_response.h"\
	"..\..\..\..\include\axis2_http_status_line.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\include\axis2_http_transport_utils.h"\
	"..\..\..\..\include\axis2_http_worker.h"\
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
	"..\..\..\..\include\axis2_simple_http_svr_conn.h"\
	"..\..\..\..\include\axis2_svc.h"\
	"..\..\..\..\include\axis2_svc_ctx.h"\
	"..\..\..\..\include\axis2_svc_grp.h"\
	"..\..\..\..\include\axis2_svc_grp_ctx.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\include\axis2_svc_skeleton.h"\
	"..\..\..\..\include\axis2_svr_callback.h"\
	"..\..\..\..\include\axis2_transport_in_desc.h"\
	"..\..\..\..\include\axis2_transport_out_desc.h"\
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_http_chunked_stream.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\listener_manager.c
DEP_CPP_LISTE=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_listener_manager.h"\
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
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\module_builder.c
DEP_CPP_MODUL=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_module_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\description\module_desc.c
DEP_CPP_MODULE=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_module.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\msg.c
DEP_CPP_MSG_C=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\context\msg_ctx.c
DEP_CPP_MSG_CT=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
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
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_out_transport_info.h"\
	"..\..\..\..\include\axis2_http_simple_response.h"\
	"..\..\..\..\include\axis2_http_status_line.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_msg_recv.h"\
	"..\..\..\..\include\axis2_op.h"\
	"..\..\..\..\include\axis2_op_ctx.h"\
	"..\..\..\..\include\axis2_options.h"\
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
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\addr\msg_info_headers.c
DEP_CPP_MSG_I=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\receivers\msg_recv.c
DEP_CPP_MSG_R=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_core_utils.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_engine.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\description\op.c
DEP_CPP_OP_C54=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_module.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\op_client.c
DEP_CPP_OP_CL=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_async_result.h"\
	"..\..\..\..\include\axis2_callback.h"\
	"..\..\..\..\include\axis2_callback_recv.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_core_utils.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_engine.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_listener_manager.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_msg_recv.h"\
	"..\..\..\..\include\axis2_op.h"\
	"..\..\..\..\include\axis2_op_client.h"\
	"..\..\..\..\include\axis2_op_ctx.h"\
	"..\..\..\..\include\axis2_options.h"\
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
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
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
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\context\op_ctx.c
DEP_CPP_OP_CT=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\options.c
DEP_CPP_OPTIO=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
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
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_msg_recv.h"\
	"..\..\..\..\include\axis2_op.h"\
	"..\..\..\..\include\axis2_op_ctx.h"\
	"..\..\..\..\include\axis2_options.h"\
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
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\phase.c
DEP_CPP_PHASE=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_phase.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\phaseresolver\phase_holder.c
DEP_CPP_PHASE_=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_phase.h"\
	"..\..\..\..\include\axis2_phase_holder.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\phaseresolver\phase_resolver.c
DEP_CPP_PHASE_R=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\phase_rule.c
DEP_CPP_PHASE_RU=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\phases_info.c
DEP_CPP_PHASES=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\policy_include.c
DEP_CPP_POLIC=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_policy_include.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_all.h"\
	"..\..\..\..\neethi\include\neethi_assertion.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_engine.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\receivers\raw_xml_in_out_msg_recv.c
DEP_CPP_RAW_X=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_with_specific_attribute_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_raw_xml_in_out_msg_recv.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\addr\relates_to.c
DEP_CPP_RELAT=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\repos_listener.c
DEP_CPP_REPOS=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\req_uri_disp.c
DEP_CPP_REQ_U=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\rest_disp.c
DEP_CPP_REST_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_document.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_builder.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\http\common\simple_http_svr_conn.c
DEP_CPP_SIMPL=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_http_header.h"\
	"..\..\..\..\include\axis2_http_request_line.h"\
	"..\..\..\..\include\axis2_http_response_writer.h"\
	"..\..\..\..\include\axis2_http_simple_request.h"\
	"..\..\..\..\include\axis2_http_simple_response.h"\
	"..\..\..\..\include\axis2_http_status_line.h"\
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\include\axis2_simple_http_svr_conn.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_http_chunked_stream.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\soap_action_disp.c
DEP_CPP_SOAP_=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\soap_body_disp.c
DEP_CPP_SOAP_B=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_child_element_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_iterator.h"\
	"..\..\..\..\axiom\include\axiom_children_qname_iterator.h"\
	"..\..\..\..\axiom\include\axiom_data_handler.h"\
	"..\..\..\..\axiom\include\axiom_element.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_soap_body.h"\
	"..\..\..\..\axiom\include\axiom_soap_const.h"\
	"..\..\..\..\axiom\include\axiom_soap_envelope.h"\
	"..\..\..\..\axiom\include\axiom_soap_fault.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_disp.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\stub.c
DEP_CPP_STUB_=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_async_result.h"\
	"..\..\..\..\include\axis2_callback.h"\
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
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_msg_recv.h"\
	"..\..\..\..\include\axis2_op.h"\
	"..\..\..\..\include\axis2_op_client.h"\
	"..\..\..\..\include\axis2_op_ctx.h"\
	"..\..\..\..\include\axis2_options.h"\
	"..\..\..\..\include\axis2_phase.h"\
	"..\..\..\..\include\axis2_phase_holder.h"\
	"..\..\..\..\include\axis2_phase_meta.h"\
	"..\..\..\..\include\axis2_phase_resolver.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_phases_info.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_stub.h"\
	"..\..\..\..\include\axis2_svc.h"\
	"..\..\..\..\include\axis2_svc_client.h"\
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
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\description\svc.c
DEP_CPP_SVC_C=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_module.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\svc_builder.c
DEP_CPP_SVC_B=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\clientapi\svc_client.c
DEP_CPP_SVC_CL=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_util.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_async_result.h"\
	"..\..\..\..\include\axis2_callback.h"\
	"..\..\..\..\include\axis2_callback_recv.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_conf_ctx.h"\
	"..\..\..\..\include\axis2_conf_init.h"\
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
	"..\..\..\..\include\axis2_http_transport.h"\
	"..\..\..\..\include\axis2_listener_manager.h"\
	"..\..\..\..\include\axis2_module_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_msg_recv.h"\
	"..\..\..\..\include\axis2_op.h"\
	"..\..\..\..\include\axis2_op_client.h"\
	"..\..\..\..\include\axis2_op_ctx.h"\
	"..\..\..\..\include\axis2_options.h"\
	"..\..\..\..\include\axis2_phase.h"\
	"..\..\..\..\include\axis2_phase_holder.h"\
	"..\..\..\..\include\axis2_phase_meta.h"\
	"..\..\..\..\include\axis2_phase_resolver.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_phases_info.h"\
	"..\..\..\..\include\axis2_policy_include.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc.h"\
	"..\..\..\..\include\axis2_svc_client.h"\
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
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_all.h"\
	"..\..\..\..\neethi\include\neethi_assertion.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_engine.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\neethi_util.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_generic_obj.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\context\svc_ctx.c
DEP_CPP_SVC_CT=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\svc_grp.c
DEP_CPP_SVC_G=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\svc_grp_builder.c
DEP_CPP_SVC_GR=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_addr.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_svc_grp_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\src\core\context\svc_grp_ctx.c
DEP_CPP_SVC_GRP=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\addr\svc_name.c
DEP_CPP_SVC_N=\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\receivers\svr_callback.c
DEP_CPP_SVR_C=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\include\axis2_engine.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\transport_in_desc.c
DEP_CPP_TRANS=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\description\transport_out_desc.c
DEP_CPP_TRANSP=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
	"..\..\..\..\include\axis2_msg_ctx.h"\
	"..\..\..\..\include\axis2_msg_info_headers.h"\
	"..\..\..\..\include\axis2_phase.h"\
	"..\..\..\..\include\axis2_phase_meta.h"\
	"..\..\..\..\include\axis2_phase_rule.h"\
	"..\..\..\..\include\axis2_relates_to.h"\
	"..\..\..\..\include\axis2_svc_name.h"\
	"..\..\..\..\include\axis2_transport_out_desc.h"\
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\transport\transport_receiver.c
DEP_CPP_TRANSPO=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
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
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\ws_info.c
DEP_CPP_WS_IN=\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\ws_info_list.c
DEP_CPP_WS_INF=\
	"..\..\..\..\axiom\include\axiom.h"\
	"..\..\..\..\axiom\include\axiom_attribute.h"\
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
	"..\..\..\..\axiom\include\axiom_namespace.h"\
	"..\..\..\..\axiom\include\axiom_navigator.h"\
	"..\..\..\..\axiom\include\axiom_node.h"\
	"..\..\..\..\axiom\include\axiom_output.h"\
	"..\..\..\..\axiom\include\axiom_processing_instruction.h"\
	"..\..\..\..\axiom\include\axiom_soap.h"\
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
	"..\..\..\..\axiom\include\axiom_soap_header.h"\
	"..\..\..\..\axiom\include\axiom_soap_header_block.h"\
	"..\..\..\..\axiom\include\axiom_stax_builder.h"\
	"..\..\..\..\axiom\include\axiom_text.h"\
	"..\..\..\..\axiom\include\axiom_xml_reader.h"\
	"..\..\..\..\axiom\include\axiom_xml_writer.h"\
	"..\..\..\..\include\axis2_any_content_type.h"\
	"..\..\..\..\include\axis2_conf.h"\
	"..\..\..\..\include\axis2_const.h"\
	"..\..\..\..\include\axis2_core_dll_desc.h"\
	"..\..\..\..\include\axis2_ctx.h"\
	"..\..\..\..\include\axis2_defines.h"\
	"..\..\..\..\include\axis2_desc.h"\
	"..\..\..\..\include\axis2_description.h"\
	"..\..\..\..\include\axis2_endpoint_ref.h"\
	"..\..\..\..\include\axis2_flow.h"\
	"..\..\..\..\include\axis2_flow_container.h"\
	"..\..\..\..\include\axis2_handler.h"\
	"..\..\..\..\include\axis2_handler_desc.h"\
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
	"..\..\..\..\include\axis2_policy_include.h"\
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
	"..\..\..\..\include\axis2_transport_sender.h"\
	"..\..\..\..\include\axis2_util.h"\
	"..\..\..\..\neethi\include\neethi_constants.h"\
	"..\..\..\..\neethi\include\neethi_exactlyone.h"\
	"..\..\..\..\neethi\include\neethi_includes.h"\
	"..\..\..\..\neethi\include\neethi_operator.h"\
	"..\..\..\..\neethi\include\neethi_policy.h"\
	"..\..\..\..\neethi\include\neethi_reference.h"\
	"..\..\..\..\neethi\include\neethi_registry.h"\
	"..\..\..\..\neethi\include\rp_defines.h"\
	"..\..\..\..\src\core\deployment\axis2_arch_file_data.h"\
	"..\..\..\..\src\core\deployment\axis2_conf_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_dep_engine.h"\
	"..\..\..\..\src\core\deployment\axis2_deployment.h"\
	"..\..\..\..\src\core\deployment\axis2_desc_builder.h"\
	"..\..\..\..\src\core\deployment\axis2_repos_listener.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info.h"\
	"..\..\..\..\src\core\deployment\axis2_ws_info_list.h"\
	"..\..\..\..\util\include\axutil_allocator.h"\
	"..\..\..\..\util\include\axutil_array_list.h"\
	"..\..\..\..\util\include\axutil_base64.h"\
	"..\..\..\..\util\include\axutil_base64_binary.h"\
	"..\..\..\..\util\include\axutil_class_loader.h"\
	"..\..\..\..\util\include\axutil_config.h"\
	"..\..\..\..\util\include\axutil_date_time.h"\
	"..\..\..\..\util\include\axutil_dir_handler.h"\
	"..\..\..\..\util\include\axutil_dll_desc.h"\
	"..\..\..\..\util\include\axutil_env.h"\
	"..\..\..\..\util\include\axutil_error.h"\
	"..\..\..\..\util\include\axutil_error_default.h"\
	"..\..\..\..\util\include\axutil_file.h"\
	"..\..\..\..\util\include\axutil_file_handler.h"\
	"..\..\..\..\util\include\axutil_hash.h"\
	"..\..\..\..\util\include\axutil_linked_list.h"\
	"..\..\..\..\util\include\axutil_log.h"\
	"..\..\..\..\util\include\axutil_log_default.h"\
	"..\..\..\..\util\include\axutil_network_handler.h"\
	"..\..\..\..\util\include\axutil_param.h"\
	"..\..\..\..\util\include\axutil_param_container.h"\
	"..\..\..\..\util\include\axutil_property.h"\
	"..\..\..\..\util\include\axutil_qname.h"\
	"..\..\..\..\util\include\axutil_stack.h"\
	"..\..\..\..\util\include\axutil_stream.h"\
	"..\..\..\..\util\include\axutil_string.h"\
	"..\..\..\..\util\include\axutil_string_util.h"\
	"..\..\..\..\util\include\axutil_thread.h"\
	"..\..\..\..\util\include\axutil_thread_pool.h"\
	"..\..\..\..\util\include\axutil_types.h"\
	"..\..\..\..\util\include\axutil_uri.h"\
	"..\..\..\..\util\include\axutil_url.h"\
	"..\..\..\..\util\include\axutil_utils.h"\
	"..\..\..\..\util\include\axutil_utils_defines.h"\
	"..\..\..\..\util\include\axutil_uuid_gen.h"\
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
# Begin Source File

SOURCE=..\..\..\..\include\axis2_addr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_addr_mod.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_any_content_type.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_arch_file_data.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_arch_reader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_async_result.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_callback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_callback_recv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_conf_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf_init.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_const.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_core_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_defines.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_dep_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_deployment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_desc_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_description.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_disp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\engine\axis2_disp_checker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_endpoint_ref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_flow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_flow_container.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_handler_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_chunked_stream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_header.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_out_transport_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_request_line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_response_writer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_server.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_simple_request.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_simple_response.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_status_line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_svr_thread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_transport.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_transport_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_transport_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_http_worker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_listener_manager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_module.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_module_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_module_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_msg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_msg_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_msg_info_headers.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_msg_recv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_op_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_op_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_options.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_phase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_phase_holder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_phase_meta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_phase_resolver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_phase_rule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_phases_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_policy_include.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_raw_xml_in_out_msg_recv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_relates_to.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_repos_listener.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_simple_http_svr_conn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_stub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_svc_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_grp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_svc_grp_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_grp_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_name.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_skeleton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svr_callback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_thread_mutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_in_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_out_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_receiver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_ws_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\core\deployment\axis2_ws_info_list.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
