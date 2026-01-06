/*
 * Stub HTTP Transport Sender for Android Static Linking
 *
 * This is a minimal transport sender stub that satisfies Axis2/C's dlopen
 * requirements while actual response writing is handled by apache2_worker
 * and the message receiver for HTTP/2 JSON-RPC.
 */

#include <axis2_transport_sender.h>
#include <axis2_const.h>

typedef struct stub_http_sender_impl
{
    axis2_transport_sender_t sender;
} stub_http_sender_impl_t;

#define AXIS2_INTF_TO_IMPL(sender) ((stub_http_sender_impl_t*)(sender))

static axis2_status_t AXIS2_CALL
stub_http_sender_init(
    axis2_transport_sender_t *transport_sender,
    const axutil_env_t *env,
    struct axis2_conf_ctx *conf_ctx,
    struct axis2_transport_out_desc *transport_out)
{
    (void)transport_sender;
    (void)env;
    (void)conf_ctx;
    (void)transport_out;
    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
stub_http_sender_invoke(
    axis2_transport_sender_t *transport_sender,
    const axutil_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    (void)transport_sender;
    (void)msg_ctx;

    /* For HTTP/2 JSON-RPC, response is already written by apache2_worker
     * and the message receiver. This stub just returns success. */
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_SENDER] invoke called - response handled by apache2_worker");

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
stub_http_sender_cleanup(
    axis2_transport_sender_t *transport_sender,
    const axutil_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    (void)transport_sender;
    (void)env;
    (void)msg_ctx;
    return AXIS2_SUCCESS;
}

static void AXIS2_CALL
stub_http_sender_free(
    axis2_transport_sender_t *transport_sender,
    const axutil_env_t *env)
{
    stub_http_sender_impl_t *impl = AXIS2_INTF_TO_IMPL(transport_sender);
    if (impl)
    {
        AXIS2_FREE(env->allocator, impl);
    }
}

static const axis2_transport_sender_ops_t stub_http_sender_ops = {
    stub_http_sender_init,
    stub_http_sender_invoke,
    stub_http_sender_cleanup,
    stub_http_sender_free
};

AXIS2_EXTERN int
axis2_get_instance(
    void **inst,
    const axutil_env_t *env)
{
    stub_http_sender_impl_t *impl = NULL;

    impl = (stub_http_sender_impl_t*)AXIS2_MALLOC(env->allocator,
        sizeof(stub_http_sender_impl_t));

    if (!impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    impl->sender.ops = &stub_http_sender_ops;
    *inst = (void*)&(impl->sender);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[STUB_SENDER] Created stub HTTP transport sender instance");

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int
axis2_remove_instance(
    void *inst,
    const axutil_env_t *env)
{
    if (inst)
    {
        axis2_transport_sender_t *sender = (axis2_transport_sender_t*)inst;
        stub_http_sender_free(sender, env);
    }
    return AXIS2_SUCCESS;
}
