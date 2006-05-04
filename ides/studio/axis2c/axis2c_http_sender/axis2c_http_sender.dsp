# Microsoft Developer Studio Project File - Name="axis2c_http_sender" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axis2c_http_sender - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axis2c_http_sender.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axis2c_http_sender.mak" CFG="axis2c_http_sender - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axis2c_http_sender - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axis2c_http_sender - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axis2c_http_sender - Win32 Release"

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

!ELSEIF  "$(CFG)" == "axis2c_http_sender - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "$(AXIS2C_HOME)\include" /I "$(AXIS2C_HOME)\modules\util" /I "$(AXIS2C_HOME)\modules\platforms" /I "$(AXIS2C_HOME)\modules\core\description" /I "$(AXIS2C_HOME)\modules\xml\parser" /I "$(AXIS2C_HOME)\modules\wsdl" /I "$(AXIS2C_HOME)\modules\core\transport" /I "$(AXIS2C_HOME)\modules\core\deployment" /I "$(AXIS2C_HOME)\modules\core\clientapi" /I "$(AXIS2C_HOME)\modules\xml\parser\guththila\impl\src" /I "$(LIBXML2_HOME)\include" /I "$(ICONV_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUTHTHILA_DECLARE_EXPORT" /D "AXIS2_DECLARE_EXPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Rpcrt4.lib Ws2_32.lib axis2c_util.lib axis2c_om.lib axis2c_soap.lib axis2c_parser.lib axis2c_wsdl.lib axis2c_windows.lib axis2c_engine.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\axis2c_util\Debug" /libpath:"..\axis2c_om\Debug" /libpath:"..\axis2c_soap\Debug" /libpath:"..\axis2c_parser\Debug" /libpath:"..\axis2c_wsdl\Debug" /libpath:"..\axis2c_windows\Debug" /libpath:"..\axis2c_engine\Debug"

!ENDIF 

# Begin Target

# Name "axis2c_http_sender - Win32 Release"
# Name "axis2c_http_sender - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "transport"

# PROP Default_Filter ""
# Begin Group "http"

# PROP Default_Filter ""
# Begin Group "sender"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\sender\http_client.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\sender\http_transport_sender.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\sender\rest_sender.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\http\sender\soap_over_http_sender.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\axis2_transport_receiver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\modules\core\transport\axis2_transport_sender.h
# End Source File
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

SOURCE=..\..\..\..\include\axis2_dep_http_sender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_desc_builder.h
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

SOURCE=..\..\..\..\include\axis2_event.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_file.h
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

SOURCE=..\..\..\..\include\axis2_log.h
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

SOURCE=..\..\..\..\include\axis2_stream.h
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

SOURCE=..\..\..\..\include\axis2_transport_in_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_transport_out_desc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_url.h
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

SOURCE=..\..\..\..\include\axis2_wsdl_endpoint.h
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

SOURCE=..\..\..\..\include\axis2_wsdl_soap_op.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\axis2_wsdl_svc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
