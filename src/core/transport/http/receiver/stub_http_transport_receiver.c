/*
 * Stub HTTP Transport Receiver for Android Static Linking
 *
 * This is a minimal transport receiver stub that satisfies Axis2/C's dlopen
 * requirements while actual HTTP receiving is handled by Apache httpd
 * via mod_axis2 for the embedded server scenario.
 */

#include <axis2_transport_receiver.h>
#include <axis2_const.h>

typedef struct stub_http_receiver_impl
{
    axis2_transport_receiver_t receiver;
    axis2_char_t *server_ip;
    axis2_conf_ctx_t *conf_ctx;
} stub_http_receiver_impl_t;

#define AXIS2_INTF_TO_IMPL(recv) ((stub_http_receiver_impl_t*)(recv))

static axis2_status_t AXIS2_CALL
stub_http_receiver_init(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env,
    struct axis2_conf_ctx *conf_ctx,
    struct axis2_transport_in_desc *transport_in)
{
    stub_http_receiver_impl_t *impl = AXIS2_INTF_TO_IMPL(transport_receiver);
    (void)transport_in;

    impl->conf_ctx = conf_ctx;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_RECEIVER] init called");

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
stub_http_receiver_start(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env)
{
    (void)transport_receiver;

    /* Apache httpd handles the actual server start */
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_RECEIVER] start called - Apache handles HTTP receiving");

    return AXIS2_SUCCESS;
}

static axis2_endpoint_ref_t* AXIS2_CALL
stub_http_receiver_get_reply_to_epr(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env,
    const axis2_char_t *svc_name)
{
    (void)transport_receiver;
    (void)env;
    (void)svc_name;
    return NULL;
}

static axis2_endpoint_ref_t* AXIS2_CALL
stub_http_receiver_get_epr_for_service(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env,
    const axis2_char_t *svc_name)
{
    (void)transport_receiver;
    (void)env;
    (void)svc_name;
    return NULL;
}

static axis2_char_t* AXIS2_CALL
stub_http_receiver_get_server_ip(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env)
{
    stub_http_receiver_impl_t *impl = AXIS2_INTF_TO_IMPL(transport_receiver);
    (void)env;
    return impl->server_ip;
}

static void AXIS2_CALL
stub_http_receiver_set_server_ip(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env,
    axis2_char_t *ip)
{
    stub_http_receiver_impl_t *impl = AXIS2_INTF_TO_IMPL(transport_receiver);

    if (impl->server_ip)
    {
        AXIS2_FREE(env->allocator, impl->server_ip);
    }
    impl->server_ip = ip ? axutil_strdup(env, ip) : NULL;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_RECEIVER] set_server_ip: %s", ip ? ip : "NULL");
}

static struct axis2_conf_ctx* AXIS2_CALL
stub_http_receiver_get_conf_ctx(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env)
{
    stub_http_receiver_impl_t *impl = AXIS2_INTF_TO_IMPL(transport_receiver);
    (void)env;
    return impl->conf_ctx;
}

static axis2_bool_t AXIS2_CALL
stub_http_receiver_is_running(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env)
{
    (void)transport_receiver;
    (void)env;
    /* Apache httpd handles the actual running state */
    return AXIS2_TRUE;
}

static void AXIS2_CALL
stub_http_receiver_set_is_application_client_side(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env,
    axis2_bool_t is_application_client_side)
{
    (void)transport_receiver;
    (void)env;
    (void)is_application_client_side;
}

static axis2_status_t AXIS2_CALL
stub_http_receiver_stop(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env)
{
    (void)transport_receiver;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_RECEIVER] stop called");

    return AXIS2_SUCCESS;
}

static void AXIS2_CALL
stub_http_receiver_free(
    axis2_transport_receiver_t *transport_receiver,
    const axutil_env_t *env)
{
    stub_http_receiver_impl_t *impl = AXIS2_INTF_TO_IMPL(transport_receiver);

    if (impl)
    {
        if (impl->server_ip)
        {
            AXIS2_FREE(env->allocator, impl->server_ip);
        }
        AXIS2_FREE(env->allocator, impl);
    }
}

static const axis2_transport_receiver_ops_t stub_http_receiver_ops = {
    stub_http_receiver_init,
    stub_http_receiver_start,
    stub_http_receiver_get_reply_to_epr,
    stub_http_receiver_get_epr_for_service,
    stub_http_receiver_get_server_ip,
    stub_http_receiver_set_server_ip,
    stub_http_receiver_get_conf_ctx,
    stub_http_receiver_is_running,
    stub_http_receiver_set_is_application_client_side,
    stub_http_receiver_stop,
    stub_http_receiver_free
};

AXIS2_EXTERN int
axis2_get_instance(
    void **inst,
    const axutil_env_t *env)
{
    stub_http_receiver_impl_t *impl = NULL;

    impl = (stub_http_receiver_impl_t*)AXIS2_MALLOC(env->allocator,
        sizeof(stub_http_receiver_impl_t));

    if (!impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    memset(impl, 0, sizeof(stub_http_receiver_impl_t));
    impl->receiver.ops = &stub_http_receiver_ops;
    *inst = (void*)&(impl->receiver);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_RECEIVER] Created stub HTTP transport receiver instance");

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int
axis2_remove_instance(
    void *inst,
    const axutil_env_t *env)
{
    if (inst)
    {
        axis2_transport_receiver_t *receiver = (axis2_transport_receiver_t*)inst;
        stub_http_receiver_free(receiver, env);
    }
    return AXIS2_SUCCESS;
}
