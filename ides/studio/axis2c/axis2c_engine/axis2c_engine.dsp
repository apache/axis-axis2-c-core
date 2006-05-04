# Microsoft Developer Studio Project File - Name="axis2c_engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axis2c_engine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axis2c_engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axis2c_engine.mak" CFG="axis2c_engine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axis2c_engine - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axis2c_engine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axis2c_engine - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIS2C_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIS2C_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "axis2c_engine - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXIS2C_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "$(AXIS2C_HOME)\modules\core\engine" /I "$(AXIS2C_HOME)\modules\core\util" /I "$(AXIS2C_HOME)\include" /I "$(AXIS2C_HOME)\modules\util" /I "$(AXIS2C_HOME)\modules\platforms" /I "$(AXIS2C_HOME)\modules\core\description" /I "$(AXIS2C_HOME)\modules\xml\parser" /I "$(AXIS2C_HOME)\modules\wsdl" /I "$(AXIS2C_HOME)\modules\core\transport" /I "$(AXIS2C_HOME)\modules\core\deployment" /I "$(AXIS2C_HOME)\modules\core\clientapi" /I "$(AXIS2C_HOME)\modules\xml\parser\guththila\impl\src" /I "$(LIBXML2_HOME)\include" /I "$(ICONV_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUTHTHILA_DECLARE_EXPORT" /D "AXIS2_DECLARE_EXPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Rpcrt4.lib Ws2_32.lib axis2c_util.lib axis2c_om.lib axis2c_soap.lib axis2c_parser.lib axis2c_wsdl.lib axis2c_windows.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\axis2c_util\Debug" /libpath:"..\axis2c_om\Debug" /libpath:"..\axis2c_soap\Debug" /libpath:"..\axis2c_parser\Debug" /libpath:"..\axis2c_wsdl\Debug" /libpath:"..\axis2c_windows\Debug"

!ENDIF 

# Begin Target

# Name "axis2c_engine - Win32 Release"
# Name "axis2c_engine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\addr_disp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\axis2_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\axis2_event.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\conf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\ctx_handler.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\disp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\disp_checker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\engine.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\event.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\handler.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\phase.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\req_uri_disp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\soap_action_disp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\engine\soap_body_disp.c
# End Source File
# End Group
# Begin Group "addr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\addr\any_content_type.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\addr\endpoint_ref.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\addr\msg_info_headers.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\addr\relates_to.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\addr\svc_name.c
# End Source File
# End Group
# Begin Group "clientapi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\async_result.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\axis2_msg_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\call.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\callback.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\callback_recv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\callback_recv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\listener_manager.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\listener_manager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\mep_client.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\msg_sender.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\op_client.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\options.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\stub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\clientapi\svc_client.c
# End Source File
# End Group
# Begin Group "context"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\context\conf_ctx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\context\ctx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\context\msg_ctx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\context\op_ctx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\context\svc_ctx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\context\svc_grp_ctx.c
# End Source File
# End Group
# Begin Group "deployment"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\arch_file_data.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\arch_reader.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_arch_file_data.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_arch_reader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_conf_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_conf_init.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_dep_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_deployment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_desc_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_module_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_repos_listener.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_svc_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_svc_grp_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_ws_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\axis2_ws_info_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\conf_builder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\conf_init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\dep_engine.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\desc_builder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\module_builder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\phases_info.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\repos_listener.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\svc_builder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\svc_grp_builder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\ws_info.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\deployment\ws_info_list.c
# End Source File
# End Group
# Begin Group "description"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\flow.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\flow_container.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\handler_desc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\module.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\module_desc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\op.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\phase_rule.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\svc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\svc_grp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\transport_in_desc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\description\transport_out_desc.c
# End Source File
# End Group
# Begin Group "phaseresolver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\phaseresolver\phase_holder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\phaseresolver\phase_resolver.c
# End Source File
# End Group
# Begin Group "receivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\receivers\msg_recv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\receivers\raw_xml_in_out_msg_recv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\receivers\svr_callback.c
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\util\axis2_core_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\util\core_utils.c
# End Source File
# End Group
# Begin Group "transport"

# PROP Default_Filter ""
# Begin Group "http"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_chunked_stream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_header.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_out_transport_info.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_request_line.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_simple_request.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_simple_response.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_status_line.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_transport_utils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\http_worker.c
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\axis2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_addr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_addr_mod.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_any_content_type.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_arch_file_data.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_arch_reader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_async_result.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_call.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_callback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_class_loader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_conf_init.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_core.h
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

SOURCE=..\..\..\..\include\axis2_dep_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_desc_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_di_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_dir_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_disp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_disp_checker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_dll_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_endpoint_ref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_env.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_error_default.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_event.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_file.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_file_diff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_file_handler.h
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

SOURCE=..\..\..\..\include\axis2_log.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_log_default.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_mep_client.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_module.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_module_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_module_desc.h
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

SOURCE=..\..\..\..\include\axis2_msg_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_network_handler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_attribute.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_child_element_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_children_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_children_qname_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_children_with_specific_attribute_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_comment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_doctype.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_document.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_namespace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_navigator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_output.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_processing_instruction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_stax_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_om_text.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_op_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_param.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_param_container.h
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

SOURCE=..\..\..\..\include\axis2_qname.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_raw_xml_in_out_msg_recv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_relates_to.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_repos_listener.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_rest_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_simple_http_svr_conn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap11_builder_helper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap12_builder_helper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_body.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_envelope.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_detail.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_reason.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_role.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_sub_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_text.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_fault_value.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_header.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_header_block.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_message.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_over_http_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_soap_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_stack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_stream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_string_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_stub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_builder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_ctx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_grp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_svc_grp_builder.h
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

SOURCE=..\..\..\..\include\axis2_thread_mutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_in_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_out_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_url.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_uuid_gen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_ws_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_ws_info_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl11_mep_finder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_binding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_constraint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_message.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_operation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_part.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_port_type.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_qname.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_schema_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_service.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl4c_soap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_binding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_binding_fault.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_binding_msg_ref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_binding_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_component.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_endpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_ext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_ext_soap_address.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_ext_soap_binding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_ext_soap_body.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_ext_soap_header.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_ext_soap_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_extensible_attribute.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_extensible_component.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_extensible_element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_fault_ref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_feature.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_import.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_include.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_msg_ref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_property.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_pump.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_soap_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_svc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_xml_schema_constants.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
