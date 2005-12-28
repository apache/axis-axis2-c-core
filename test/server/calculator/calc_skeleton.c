#include "axis2_svc_skeleton.h"

typedef struct axis2_svc_skeleton_impl
{
    axis2_svc_skeleton_t svc_skeleton;
    axis2_array_list_t *func_array;

} axis2_svc_skeleton_impl_t;

#define AXIS2_INTF_TO_IMPL(axis2_svc_skeleton) \
    ((axis2_svc_skeleton_impl_t *) axis2_svc_skeleton)

axis2_svc_skeleton *
axis2_svc_skeleton_create(axis2_env_t **env)
{
    axis2_svc_skeleton_impl_t *skeleton_impl = NULL;
    skeleton_impl = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_svc_skeleton_impl_t));

    
    skeleton_impl->svc_skeleton.ops = AXIS2_MALLOC(
        (*env)->allocator, sizeof(axis2_skeleton_ops_t));

    skeleton_impl->svc_skeleton.ops->free = calc_free;
    skeleton_impl->svc_skeleton.ops->invoke = calc_invoke;
    skeleton_impl->svc_skeleton.ops->on_fault = calc_on_fault;
    skeleton_impl->svc_skeleton.ops->get_function_liste = calc_get_function_list;

    return (&(skeleton_impl->svc_skeleton));
}

int axis2_svc_skeleton_init(axis2_svc_skeleton_t *svc_skeleton,
                        axis2_env_t **env)
{
    axis2_svc_skeleton_impl_t *skeleton_impl = NULL;

    skeleton_impl = AXIS2_INTF_TO_IMPL(svc_skeleton);
    
    skeleton_impl->func_array = axis2_array_create(env, 0);
    AXIS2_ARRAY_ADD(skeleton_impl->func_array, env, "add");
    AXIS2_ARRAY_ADD(skeleton_impl->func_array, env, "multiply");

    /* Any initialization stuff of calculator goes here */
    return AXIS2_FAILURE;
}

int 
calc_free(axis2_svc_skeleton_t *svc_skeleton)
{
    axis2_svc_skeleton_impl_t *skeleton_impl = NULL;

    skeleton_impl = AXIS2_INTF_TO_IMPL(svc_skeleton);
    
    if(svc_skeleton->ops)
    {
        AXIS2_FREE((*env)->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }

    if(skeleton_impl->func_array)
    {
        AXIS2_ARRAY_LIST_FREE(skeleton_impl->func_array, env);
        skeleton_impl->func_array = NULL;
    }
    
    if(skeleton_impl)
    {
        AXIS2_FREE((*env)->allocator, skeleton_impl);
        skeleton_impl = NULL;
    }
    
}

/*
 * This method invokes the right service method 
 */
axis2_om_node_t *calc_invoke(axis2_svc_skeleton_t *svc_skeleton,
                                    axis2_env_t **env,
                                    axis2_om_node_t *node,
                                    axis2_char_t *function_name)
{
    /* Depending on the function name invoke the
     *  corresponding calculator method
     */
    
    return node;
}

axis2_om_node_t *calc_on_fault(axis2_svc_skeleton_t *svc_skeleton,
                       axis2_env_t **env,
                       axis2_om_node_t *node)
{
}

axis2_array_list_t *calc_get_function_list(axis2_svc_skeleton_t *svc_skeleton,
                                        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(svc_skeleton)->func_array;
}
