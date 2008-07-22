#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "base.h"
#include "log.h"
#include "buffer.h"
#include "response.h"

#include "plugin.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <axiom_xml_reader.h>
#include <axutil_version.h>
#include <axis2_http_transport.h>
#include <axis2_conf_ctx.h>
#include <axis2_conf_init.h>
#include <axutil_class_loader.h>
#include <axis2_conf_ctx.h>

#include <axis2_http_transport.h>
#include <axis2_msg_ctx.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport_utils.h>
#include <axis2_http_accept_record.h>
#include "lighty_stream.h"
#include "axis2_lighty_out_transport_info.h"

/* plugin config for all request/connections */
typedef struct {
	buffer *axis2c_repo;
    buffer *axis2c_logfile;
    buffer *axis2c_loglevel;
} plugin_config;

typedef struct {
	PLUGIN_DATA;
	buffer *axis2c_repo_buf;

	plugin_config **config_storage;

	plugin_config conf;

    /* mod_axis2 related values */
    axutil_env_t *env;

    axis2_conf_ctx_t *conf_ctx;

} plugin_data;


static int axis2_lighty_init_server (server *srv, plugin_data *p_d);

static int axis2_lighty_process_request (server *srv, connection *con, plugin_data *p);


/* init the plugin data */
INIT_FUNC(mod_axis2_init) 
{
	plugin_data *p;
    
	p = calloc(1, sizeof(*p));

	p->axis2c_repo_buf = buffer_init();
	return p;
}

/* detroy the plugin data */
FREE_FUNC(mod_axis2_free) 
{
	plugin_data *p = p_d;

	UNUSED(srv);

	if (!p) return HANDLER_GO_ON;

	if (p->config_storage) 
    {
		size_t i;

		for (i = 0; i < srv->config_context->used; i++) 
        {
			plugin_config *s = p->config_storage[i];

			if (!s) continue;

            buffer_free(s->axis2c_repo);

			free(s);
		}
		free(p->config_storage);
	}

    if (p->conf_ctx && p->env)
    {
        axis2_conf_ctx_free(p->conf_ctx, p->env);
        axutil_env_free(p->env);
        p->conf_ctx = NULL;
        p->env = NULL;
    }

	buffer_free(p->axis2c_repo_buf);
	free(p);
	return HANDLER_GO_ON;
}

SETDEFAULTS_FUNC(mod_axis2_set_defaults) 
{
	plugin_data *p = p_d;
	size_t i = 0;

    /* values accessing from lighttpd.conf */
	config_values_t cv[] = {
		{ "axis2c.repo-path", NULL, T_CONFIG_STRING, T_CONFIG_SCOPE_CONNECTION },
		{ "axis2c.logfile", NULL, T_CONFIG_STRING, T_CONFIG_SCOPE_CONNECTION },
		{ "axis2c.loglevel", NULL, T_CONFIG_SHORT, T_CONFIG_SCOPE_CONNECTION },
		{ NULL, NULL, T_CONFIG_UNSET, T_CONFIG_SCOPE_UNSET }
	};

	if (!p) return HANDLER_ERROR;

	p->config_storage = calloc(1, srv->config_context->used * sizeof(specific_config *));

	for (i = 0; i < srv->config_context->used; i++) 
    {
		plugin_config *s;

		s = calloc(1, sizeof(plugin_config));
		s->axis2c_repo    = buffer_init();
        s->axis2c_logfile = buffer_init ();
        s->axis2c_loglevel = buffer_init ();

		cv[0].destination = s->axis2c_repo;
        cv[1].destination = s->axis2c_logfile;
        cv[2].destination = s->axis2c_loglevel;

		p->config_storage[i] = s;

		if (0 != config_insert_values_global(srv, ((data_config *)srv->config_context->data[i])->value, cv)) 
        {
			return HANDLER_ERROR;
		}
	}

    /* initializing and setting defaults values in axis2/c */
    axis2_lighty_init_server (srv, p);
	return HANDLER_GO_ON;
}


#define PATCH(x)                                \
	p->conf.x = s->x;
static int mod_axis2_patch_connection(server *srv, connection *con, plugin_data *p) 
{
	size_t i, j;
	plugin_config *s = p->config_storage[0];

	PATCH(axis2c_repo);

	/* skip the first, the global context */
	for (i = 1; i < srv->config_context->used; i++) 
    {
		data_config *dc = (data_config *)srv->config_context->data[i];
		s = p->config_storage[i];

		/* condition didn't match */
		if (!config_check_cond(srv, con, dc)) continue;

		/* merge config */
		for (j = 0; j < dc->value->used; j++) 
        {
			data_unset *du = dc->value->data[j];

			if (buffer_is_equal_string(du->key, CONST_STR_LEN("axis2c.repo-path"))) 
            {
				PATCH(axis2c_repo);
			}
            else if(buffer_is_equal_string(du->key, CONST_STR_LEN("axis2c.logfile")))
            {
                PATCH(axis2c_logfile);
            }
            else if (buffer_is_equal_string(du->key, CONST_STR_LEN("axis2c.logfile")))
            {
                PATCH(axis2c_loglevel);
            }
		}
	}

	return 0;
}
#undef PATCH


SUBREQUEST_FUNC(mode_axis2_subrequest_start)
{
	plugin_data *p = p_d;
    mod_axis2_patch_connection(srv, con, p);
    axis2_lighty_process_request (srv, con, p);
    return HANDLER_FINISHED;
}


static int axis2_lighty_init_server (server *srv, plugin_data *p)
{
    axutil_env_t *env;
    axis2_conf_t *conf;
    axutil_hash_t* svc_map = NULL;
    axutil_hash_index_t *hi = NULL;
    void* svc = NULL;
    plugin_config *plg;
    axis2_char_t *repo_path = NULL;
    axis2_char_t *log_file = NULL;
    unsigned int log_level = 0;

    if (!p)
    {
        log_error_write(srv, __FILE__, __LINE__, "s", 
                        "plugin data is null, unable to proceed"); 
        return HANDLER_ERROR;
    }

    plg = p->config_storage[0];

    if (!plg)
    {
        log_error_write(srv, __FILE__, __LINE__, "s", 
                        "unable to retrieve mod_axis2 plugin from configuration storage"); 
        return HANDLER_ERROR;
    }

    log_file = (axis2_char_t *)plg->axis2c_logfile->ptr;
    if (!log_file)
    {
        log_error_write(srv, __FILE__, __LINE__, "s", 
                        "unable to find axis2c.logfile in lighttpd.conf"); 
        return HANDLER_ERROR;
    }

    log_level = (unsigned int)plg->axis2c_loglevel->ptr;

    if (!log_level)
    {
        log_error_write(srv, __FILE__, __LINE__, "s", 
                        "unable to find axis2c.loglevel in lighttpd.conf"); 
        return HANDLER_ERROR;
    }

	
    p->env = axutil_env_create_all (log_file, log_level);

    if (!p->env)
    {    
        log_error_write(srv, __FILE__, __LINE__, "sssd", 
                        "env creation faild for mod_axis2 log_file", 
                        log_file,"and log_level", log_level );
        return HANDLER_ERROR;
    }

    env = p->env;

    /* we are using config_storage to access axis2c.repo value from
     * configuration file */

    repo_path = (axis2_char_t *)plg->axis2c_repo->ptr;

    if (!repo_path)
    {
        log_error_write(srv, __FILE__, __LINE__, "s", 
                        "unable to find axis2c.repo-path in lighttpd.conf"); 
        return HANDLER_ERROR;
    }


    p->conf_ctx = axis2_build_conf_ctx (env, axutil_strdup(env, repo_path));

    if (!p->conf_ctx)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "conf ctx creation failed for %s repo path", repo_path);
    }

    conf = axis2_conf_ctx_get_conf (p->conf_ctx, env);

    if (!conf)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "unable to get conf for %s repo path", repo_path);
    }

    svc_map = axis2_conf_get_all_svcs (conf, env);
    if (!svc_map)
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, 
                          "svcs map is NULL");
    }

    for (hi = axutil_hash_first(svc_map, env);
         hi; hi = axutil_hash_next(env, hi))
    {
        void *impl_class = NULL;
        axutil_param_t *impl_info_param = NULL;
        
        axutil_hash_this(hi, NULL, NULL, &svc);
        if (!svc)
            continue;
        impl_class = axis2_svc_get_impl_class(svc, env);
        if (impl_class)
            continue;
        impl_info_param = axis2_svc_get_param(svc, env, AXIS2_SERVICE_CLASS);
        if (!impl_info_param)
            continue;
        
        axutil_class_loader_init(env);
        impl_class = axutil_class_loader_create_dll(env, impl_info_param);
        axis2_svc_set_impl_class(svc, env, impl_class);
        if (impl_class)
        {
            AXIS2_SVC_SKELETON_INIT((axis2_svc_skeleton_t *) impl_class, env);
        }
    }

    AXIS2_LOG_INFO(env->log, "lighty mod axis2 services loaded");

    return HANDLER_GO_ON;
}


#define PTR(x) x->ptr

static int axis2_lighty_process_request (server *srv, connection *con, plugin_data *p)
{
    axutil_env_t *env;
    axis2_conf_ctx_t *conf_ctx = NULL;
    unsigned int content_length = 0;
    unsigned int http_version = -1;
    axis2_char_t *request_url = NULL;
    const axis2_char_t *content_type = NULL;
    axis2_http_transport_in_t transport_in;
    axis2_http_transport_out_t transport_out;
    axis2_transport_out_desc_t * out_desc = NULL;
    axis2_transport_in_desc_t * in_desc = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axutil_stream_t *in_stream = NULL;
    axis2_http_out_transport_info_t *lighty_out_transport_info = NULL;
    buffer *b;

    b = buffer_init ();

    if (!p->env)
    {
        log_error_write(srv, __FILE__, __LINE__, "s", 
                        "axis2c env is null, unable to continue"); 
        
        return HANDLER_ERROR;
    }

    env = p->env;

    conf_ctx = p->conf_ctx;

    if (!conf_ctx)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "conf ctx is null unable to continue");
        return HANDLER_ERROR;
    }

    /* initialzing transport_in and transport_out structures */
    memset (&transport_in, 0, sizeof(transport_in));
    memset (&transport_out, 0, sizeof(transport_out));

    content_length = (unsigned int)con->request.content_length;
    AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "request content length %d", content_length);

    http_version = con->request.http_version;

    request_url = axutil_strcat (env, PTR(con->uri.scheme), "://", PTR(con->uri.authority), 
                                 PTR(con->uri.path), NULL);

    AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "url %s", request_url);

    content_type = con->request.http_content_type;

    AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "http content type %s", content_type);

    out_desc = axis2_conf_get_transport_out(axis2_conf_ctx_get_conf(conf_ctx, env),
                                            env, AXIS2_TRANSPORT_ENUM_HTTP);
    /** transport in description */
    in_desc = axis2_conf_get_transport_in(axis2_conf_ctx_get_conf(conf_ctx, env),
                                          env, AXIS2_TRANSPORT_ENUM_HTTP);

    /* filling transport_in structure */
    transport_in.request_uri = axutil_strdup (env, request_url);
    transport_in.content_length = content_length;
    transport_in.content_type = (axis2_char_t *)content_type;

    switch (http_version) 
    {
        case HTTP_VERSION_1_1:
        {
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "Clinet http version is HTTP 1.1");
        }
        break;
        case HTTP_VERSION_1_0:
        {
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "Client http version is HTTP 1.0");
        }
        break;
    };

    msg_ctx = axis2_msg_ctx_create (env, conf_ctx, in_desc, out_desc);
    transport_in.msg_ctx = msg_ctx;

    lighty_out_transport_info = axis2_lighty_out_transport_info_create (env, srv, con);
    transport_in.out_transport_info = lighty_out_transport_info;

    in_stream = axutil_stream_create_lighty(env, con);
    transport_in.in_stream = in_stream;


    switch (con->request.http_method){
        case HTTP_METHOD_POST:
        {
            transport_in.request_method = AXIS2_HTTP_METHOD_POST;
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "processing POST request");
        }
        break;
        case HTTP_METHOD_GET:
        {
            transport_in.request_method = AXIS2_HTTP_METHOD_GET;
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "processing GET request");
        }
        break;
        case HTTP_METHOD_PUT:
        {
            transport_in.request_method = AXIS2_HTTP_METHOD_PUT;
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "processing PUT request");
        }
        break;
        case HTTP_METHOD_HEAD:
        {
            transport_in.request_method = AXIS2_HTTP_METHOD_HEAD;
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "processing HEAD request");
        }
        break;
        case HTTP_METHOD_DELETE:
        {
            transport_in.request_method = AXIS2_HTTP_METHOD_DELETE;
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "processing DELETE request");
        }
        break;
        default:
        {
            AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "http method not handled");
        }
    };

    axis2_http_transport_utils_process_request(env, conf_ctx, &transport_in, &transport_out);

    buffer_copy_off_t (b, transport_out.response_data_length);
    response_header_overwrite(srv, con, CONST_STR_LEN("Content-Length"), CONST_BUF_LEN(b));

    buffer_reset (b);
    buffer_copy_string (b, transport_out.response_data);
	chunkqueue_append_buffer(con->write_queue, b);

    AXIS2_LOG_DEBUG (env->log, AXIS2_LOG_SI, "response %s", b->ptr);
    return HANDLER_GO_ON;
}

#undef PTR

/* this function is called at dlopen() time and inits the callbacks */
int mod_axis2_plugin_init(plugin *p) 
{
	p->version     = LIGHTTPD_VERSION_ID;
	p->name        = buffer_init_string("axis2c");

	p->init        = mod_axis2_init;
	p->set_defaults  = mod_axis2_set_defaults;
	p->cleanup     = mod_axis2_free;
    p->handle_subrequest_start = mode_axis2_subrequest_start;
	p->data        = NULL;

	return 0;
}

