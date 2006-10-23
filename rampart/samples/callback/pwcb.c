#include <stdio.h>
#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_utils.h>
#include <rampart_callback.h>
#include <axis2_string.h>
#include <axis2_svc_skeleton.h>
#include <axis2_string.h>


axis2_char_t* AXIS2_CALL
get_sample_password(rampart_callback_t *rcb,
        const axis2_env_t *env, const axis2_char_t *username)
{
    axis2_char_t * pw = NULL;
    if (0 == AXIS2_STRCMP(username, "Raigama"))
    {
        pw = "RaigamaPW" ;
    }
    else if (0 == AXIS2_STRCMP(username, "Gampola"))
    {
        pw = "GampolaPW";
    }
    else if (0 == AXIS2_STRCMP(username, "alice"))
    {
        pw = "password";
    }
    else
    {}
    return pw;
};


/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int
axis2_get_instance(rampart_callback_t **inst,
        const axis2_env_t *env)
{
    rampart_callback_t* rcb = NULL;

    rcb = AXIS2_MALLOC(env->allocator,
            sizeof(rampart_callback_t));

    rcb->ops = AXIS2_MALLOC(
                env->allocator, sizeof(rampart_callback_ops_t));

    /*assign function pointers*/

    rcb->ops->callback_password = get_sample_password;

    *inst = rcb;

    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(rampart_callback_t *inst,
        const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}

