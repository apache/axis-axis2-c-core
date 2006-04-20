TESTS =
lib_LTLIBRARIES = libaxis2_engine.la
noinst_HEADERS = axis2_engine.h \
                    axis2_event.h

libaxis2_engine_la_SOURCES = handler.c \
                                conf.c \
                                phase.c \
                                disp_checker.c \
                                event.c \
                                addr_disp.c \
                                req_uri_disp.c \
                                disp.c \
                                soap_action_disp.c \
                                soap_body_disp.c \
                                ctx_handler.c \
                                engine.c \
                                ../description/op.c \
                                ../description/svc.c \
                                ../description/module.c \
                                ../description/module_desc.c \
                                ../description/svc_grp.c \
                                ../description/phase_rule.c \
                                ../description/handler_desc.c \
                                ../description/flow.c \
                                ../description/flow_container.c \
                                ../description/transport_in_desc.c \
                                ../description/transport_out_desc.c \
                                ../receivers/msg_recv.c \
                                ../receivers/raw_xml_in_out_msg_recv.c \
                                ../receivers/svr_callback.c \
                                ../deployment/phases_info.c \
                                ../deployment/desc_builder.c \
                                ../deployment/arch_reader.c \
                                ../deployment/dep_engine.c \
                                ../deployment/arch_file_data.c \
                                ../deployment/ws_info.c \
                                ../deployment/ws_info_list.c \
                                ../deployment/svc_builder.c \
                                ../deployment/svc_grp_builder.c \
                                ../deployment/module_builder.c \
                                ../deployment/conf_builder.c \
                                ../deployment/repos_listener.c \
                                ../deployment/conf_init.c \
                                ../context/ctx.c \
                                ../context/msg_ctx.c \
                                ../context/op_ctx.c \
                                ../context/svc_ctx.c \
                                ../context/svc_grp_ctx.c \
                                ../context/conf_ctx.c \
                                ../addr/relates_to.c \
                                ../addr/svc_name.c \
                                ../addr/any_content_type.c \
                                ../addr/endpoint_ref.c \
                                ../addr/msg_info_headers.c \
                                ../clientapi/async_result.c \
                                ../clientapi/callback.c \
                                ../clientapi/mep_client.c \
                                ../clientapi/listener_manager.c \
                                ../clientapi/callback_recv.c \
                                ../clientapi/call.c \
                                ../clientapi/msg_sender.c \
                                ../clientapi/stub.c \
                                ../phaseresolver/phase_holder.c \
                                ../phaseresolver/phase_resolver.c \
                                ../util/core_utils.c \
                                ../transport/http/http_worker.c \
                                ../transport/http/http_transport_utils.c\
                				../transport/http/http_simple_request.c\
                				../transport/http/http_simple_response.c\
				                ../transport/http/http_request_line.c\
				                ../transport/http/http_status_line.c\
				                ../transport/http/http_header.c\
                                ../transport/http/http_out_transport_info.c\
                                ../transport/http/http_chunked_stream.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/util \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms
