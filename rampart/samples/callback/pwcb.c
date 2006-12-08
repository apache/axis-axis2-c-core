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
        const axis2_env_t *env, 
        const axis2_char_t *username,
        void *param)
{
    /*First set pf password are for sample usernames*/
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
    /*These are for sample keystores*/
    else if (0 == AXIS2_STRCMP(username, "a"))
    {
        pw = "a12345";
    }
    else if (0 == AXIS2_STRCMP(username, "b"))
    {
        pw = "b12345";
    }
    else if (0 == AXIS2_STRCMP(username, "x"))
    {
        pw = "x12345";
    }
    else if (0 == AXIS2_STRCMP(username, "y"))
    {
        pw = "y12345";
    }
    else
    {
        /*Append 12345 for any name not specified above*/
        /*sprintf(pw, "%s%s",  username, "12345");*/
    }
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

