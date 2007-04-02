#include <axutil_string.h>
#include <axis2_http_transport.h>
#include <string.h>
#include <axiom_output.h>
#include <axis2_op_ctx.h>
#include <axis2_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_http_client.h>
#include <axiom_xml_writer.h>
#include <axutil_property.h>
#include <axutil_param.h>
#include <axutil_types.h>
#include <axutil_generic_obj.h>

void
axis2_http_sender_util_add_header (const axutil_env_t *env,
				   axis2_http_simple_request_t *request,
				   axis2_char_t *header_name,
				   const axis2_char_t *header_value)
{
	axis2_http_header_t *http_header;
    http_header = axis2_http_header_create(env, header_name, header_value);
    axis2_http_simple_request_add_header(request, env, http_header);
}

