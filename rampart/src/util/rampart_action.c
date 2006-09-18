/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

/**
* Collection class for actions
*/
#include <stdio.h>
#include <rampart_util.h>
#include <axis2_util.h>
#include <rampart_action.h>
#include <rampart_constants.h>
#include <rampart_handler_util.h>



typedef struct rampart_actions_impl
{
    rampart_actions_t actions ;

    axis2_char_t *encryption_user  ;
    axis2_char_t *encryption_sym_algorithm  ;
    axis2_char_t *encryption_key_transport_algorithm  ;
    axis2_char_t *items  ;
    axis2_char_t *user  ;
    axis2_char_t *password_type  ;
    axis2_char_t *password_callback_class  ;
    axis2_char_t *encryption_prop_file;
    axis2_char_t *decryption_prop_file;
    axis2_char_t *signature_prop_file ;
    axis2_char_t *signature_key_identifier  ;
    axis2_char_t *encryption_key_identifier  ;
    axis2_char_t *signature_parts  ;
    axis2_char_t *encryption_parts  ;
    axis2_char_t *time_to_live  ;

}
rampart_actions_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(rampart_actions) ((rampart_actions_impl_t *)rampart_actions)

/******************* function headers ******************************/
/* private functions */
static void
rampart_actions_init_ops(
    rampart_actions_t *actions);

/*public functions*/
static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_user(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_sym_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_key_transport_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_items(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_password_type(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_user(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_password_callback_class(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_decryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_signature_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_signature_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_signature_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_char_t *AXIS2_CALL
rampart_actions_get_time_to_live(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_user(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_user
);


static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_sym_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_sym_algorithm
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_key_transport_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_key_transport_algorithm
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_items(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *items
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_password_type(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *password_type
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_user(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *user
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_password_callback_class(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *password_callback_class
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_prop_file
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_decryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *decryption_prop_file
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_signature_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *signature_prop_file
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_signature_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *signature_key_identifier
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_key_identifier
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_signature_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *signature_parts
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_parts
);

static axis2_status_t AXIS2_CALL
rampart_actions_set_time_to_live(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *time_to_live
);

static axis2_status_t AXIS2_CALL
rampart_actions_reset(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_status_t AXIS2_CALL
rampart_actions_free(
    rampart_actions_t *actions,
    const axis2_env_t *env
);

static axis2_status_t AXIS2_CALL
rampart_actions_populate_from_params(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_param_t *param_action
);

static axis2_status_t AXIS2_CALL
rampart_actions_populate_from_ctx(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_ctx_t *ctx);
/******************* end of function headers ******************************/


AXIS2_EXTERN rampart_actions_t *AXIS2_CALL
rampart_actions_create(const axis2_env_t *env)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    actions_impl = AXIS2_MALLOC(env->allocator, sizeof(rampart_actions_impl_t));
    if (!actions_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    actions_impl->encryption_user   = NULL;
    actions_impl->encryption_sym_algorithm = NULL ;
    actions_impl->encryption_key_transport_algorithm = NULL ;
    actions_impl->items = NULL;
    actions_impl->password_type = NULL;
    actions_impl->user = NULL;
    actions_impl->password_callback_class = NULL;
    actions_impl->encryption_prop_file = NULL;
    actions_impl->decryption_prop_file = NULL;
    actions_impl->signature_prop_file = NULL;
    actions_impl->signature_key_identifier = NULL;
    actions_impl->encryption_key_identifier = NULL;
    actions_impl->signature_parts = NULL;
    actions_impl->encryption_parts = NULL;
    actions_impl->time_to_live = NULL;

    actions_impl->actions.ops =  AXIS2_MALLOC(env->allocator, sizeof(rampart_actions_ops_t));
    if (!actions_impl->actions.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        rampart_actions_free(&(actions_impl->actions), env);
        return NULL;
    }

    rampart_actions_init_ops(&(actions_impl->actions));

    return &(actions_impl->actions);

}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_user(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->encryption_user;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_sym_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->encryption_sym_algorithm ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_key_transport_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->encryption_key_transport_algorithm ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_items(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->items ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_password_type(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->password_type ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_user(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->user ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_password_callback_class(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->password_callback_class ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->encryption_prop_file ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_decryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->decryption_prop_file ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_signature_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->signature_prop_file ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_signature_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->signature_key_identifier ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->encryption_key_identifier ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_signature_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->signature_parts ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_encryption_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->encryption_parts ;
}

static axis2_char_t *AXIS2_CALL
rampart_actions_get_time_to_live(
    rampart_actions_t *actions,
    const axis2_env_t *env
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    return actions_impl->time_to_live ;
}


static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_user(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_user
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->encryption_user)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_user);
        actions_impl->encryption_user = NULL;
    }

    actions_impl->encryption_user = encryption_user;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_sym_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_sym_algorithm
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->encryption_sym_algorithm)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_sym_algorithm);
        actions_impl->encryption_sym_algorithm = NULL;
    }
    actions_impl->encryption_sym_algorithm = encryption_sym_algorithm;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_key_transport_algorithm(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_key_transport_algorithm
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->encryption_key_transport_algorithm)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_key_transport_algorithm);
        actions_impl->encryption_key_transport_algorithm = NULL;
    }
    actions_impl->encryption_key_transport_algorithm = encryption_key_transport_algorithm;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_items(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *items
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->items)
    {
        AXIS2_FREE(env->allocator, actions_impl->items);
        actions_impl->items = NULL;
    }
    actions_impl->items = items ;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_password_type(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *password_type
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->password_type)
    {
        AXIS2_FREE(env->allocator, actions_impl->password_type);
        actions_impl->password_type = NULL;
    }
    actions_impl->password_type = password_type ;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_user(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *user
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->user)
    {
        AXIS2_FREE(env->allocator, actions_impl->user);
        actions_impl->user = NULL;
    }
    actions_impl->user = user ;

    return AXIS2_SUCCESS;
}
static axis2_status_t AXIS2_CALL
rampart_actions_set_password_callback_class(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *password_callback_class
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->password_callback_class)
    {
        AXIS2_FREE(env->allocator, actions_impl->password_callback_class);
        actions_impl->password_callback_class = NULL;
    }
    actions_impl->password_callback_class = password_callback_class ;

    return AXIS2_SUCCESS;
}
static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_prop_file
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->encryption_prop_file)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_prop_file);
        actions_impl->encryption_prop_file = NULL;
    }
    actions_impl->encryption_prop_file = encryption_prop_file ;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_decryption_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *decryption_prop_file
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->decryption_prop_file)
    {
        AXIS2_FREE(env->allocator, actions_impl->decryption_prop_file);
        actions_impl->decryption_prop_file = NULL;
    }
    actions_impl->decryption_prop_file = decryption_prop_file ;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_signature_prop_file(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *signature_prop_file
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->signature_prop_file)
    {
        AXIS2_FREE(env->allocator, actions_impl->signature_prop_file);
        actions_impl->signature_prop_file = NULL;
    }
    actions_impl->signature_prop_file = signature_prop_file ;

    return AXIS2_SUCCESS;
}
static axis2_status_t AXIS2_CALL
rampart_actions_set_signature_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *signature_key_identifier
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->signature_key_identifier)
    {
        AXIS2_FREE(env->allocator, actions_impl->signature_key_identifier);
        actions_impl->signature_key_identifier = NULL;
    }
    actions_impl->signature_key_identifier = signature_key_identifier;

    return AXIS2_SUCCESS;
}
static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_key_identifier(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_key_identifier
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->encryption_key_identifier)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_key_identifier);
        actions_impl->encryption_key_identifier = NULL;
    }
    actions_impl-> encryption_key_identifier = encryption_key_identifier ;

    return AXIS2_SUCCESS;
}
static axis2_status_t AXIS2_CALL
rampart_actions_set_signature_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *signature_parts
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->signature_parts)
    {
        AXIS2_FREE(env->allocator, actions_impl->signature_parts);
        actions_impl->signature_parts = NULL;
    }
    actions_impl->signature_parts = signature_parts;

    return AXIS2_SUCCESS;
}
static axis2_status_t AXIS2_CALL
rampart_actions_set_encryption_parts(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *encryption_parts
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->encryption_parts)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_parts);
        actions_impl->encryption_parts = NULL;
    }
    actions_impl->encryption_parts = encryption_parts ;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_set_time_to_live(
    rampart_actions_t *actions,
    const axis2_env_t *env,
    axis2_char_t *time_to_live
)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);
    if (actions_impl->time_to_live)
    {
        AXIS2_FREE(env->allocator, actions_impl->time_to_live);
        actions_impl->time_to_live = NULL;
    }
    actions_impl->time_to_live = time_to_live ;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_reset(rampart_actions_t * actions, const axis2_env_t *env)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    actions_impl->encryption_user = NULL;
    actions_impl->encryption_sym_algorithm = NULL;
    actions_impl->encryption_key_transport_algorithm = NULL;
    actions_impl->items = NULL;
    actions_impl->password_type = NULL;
    actions_impl->user = NULL;
    actions_impl->password_callback_class = NULL;
    actions_impl->encryption_prop_file = NULL;
    actions_impl->decryption_prop_file = NULL;
    actions_impl->signature_prop_file = NULL;
    actions_impl->signature_key_identifier = NULL;
    actions_impl->encryption_key_identifier = NULL;
    actions_impl->signature_parts = NULL;
    actions_impl->encryption_parts = NULL;

    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
rampart_actions_free(rampart_actions_t * actions, const axis2_env_t *env)
{
    rampart_actions_impl_t * actions_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    actions_impl = AXIS2_INTF_TO_IMPL(actions);

    if (actions_impl->encryption_user)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_user);
        actions_impl->encryption_user = NULL;
    }

    if (actions_impl->encryption_sym_algorithm)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_sym_algorithm);
        actions_impl->encryption_sym_algorithm = NULL;
    }

    if (actions_impl->encryption_key_transport_algorithm)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_key_transport_algorithm);
        actions_impl->encryption_key_transport_algorithm = NULL;
    }

    if (actions_impl->items)
    {
        AXIS2_FREE(env->allocator, actions_impl->items);
        actions_impl->items = NULL;
    }

    if (actions_impl->password_type)
    {
        AXIS2_FREE(env->allocator, actions_impl->password_type);
        actions_impl->password_type = NULL;
    }

    if (actions_impl->user)
    {
        AXIS2_FREE(env->allocator, actions_impl->user);
        actions_impl->user = NULL;
    }

    if (actions_impl->password_callback_class)
    {
        AXIS2_FREE(env->allocator, actions_impl->password_callback_class);
        actions_impl->password_callback_class = NULL;
    }

    if (actions_impl->encryption_prop_file)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_prop_file);
        actions_impl->encryption_prop_file = NULL;
    }

    if (actions_impl->decryption_prop_file)
    {
        AXIS2_FREE(env->allocator, actions_impl->decryption_prop_file);
        actions_impl->decryption_prop_file = NULL;
    }

    if (actions_impl->signature_prop_file)
    {
        AXIS2_FREE(env->allocator, actions_impl->signature_prop_file);
        actions_impl->signature_prop_file = NULL;
    }

    if (actions_impl->signature_key_identifier)
    {
        AXIS2_FREE(env->allocator, actions_impl->signature_key_identifier);
        actions_impl->signature_key_identifier = NULL;
    }

    if (actions_impl->encryption_key_identifier)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_key_identifier);
        actions_impl->encryption_key_identifier = NULL;
    }

    if (actions_impl->signature_parts)
    {
        AXIS2_FREE(env->allocator, actions_impl->signature_parts);
        actions_impl->signature_parts = NULL;
    }

    if (actions_impl->encryption_parts)
    {
        AXIS2_FREE(env->allocator, actions_impl->encryption_parts);
        actions_impl->encryption_parts = NULL;
    }

    if (actions_impl->time_to_live)
    {
        AXIS2_FREE(env->allocator, actions_impl->time_to_live);
        actions_impl->time_to_live = NULL;
    }

    AXIS2_FREE(env->allocator, actions_impl);
    actions_impl = NULL;

    return AXIS2_SUCCESS;
}

/*Populate actions by extracting values from parameters set*/
static axis2_status_t AXIS2_CALL
rampart_actions_populate_from_params(rampart_actions_t *actions,
        const axis2_env_t *env, axis2_param_t *param_action)
{
    axis2_status_t ret = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, param_action, AXIS2_FAILURE);
    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ENCRYPTION_USER))
    {
        ret = RAMPART_ACTIONS_SET_ENC_USER(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ENCRYPTION_USER));
    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ENCRYPTION_SYM_ALGORITHM))
    {
        ret = RAMPART_ACTIONS_SET_ENC_SYM_ALGO(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ENCRYPTION_SYM_ALGORITHM));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ENCRYPTION_KEY_TRANSFORM_ALGORITHM))
    {
        ret = RAMPART_ACTIONS_SET_ENC_KT_ALGO(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ENCRYPTION_KEY_TRANSFORM_ALGORITHM));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ITEMS))
    {
        ret = RAMPART_ACTIONS_SET_ITEMS(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ITEMS));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_PASSWORD_TYPE))
    {
        ret = RAMPART_ACTIONS_SET_PASSWORD_TYPE(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_PASSWORD_TYPE));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_USER))
    {
        ret = RAMPART_ACTIONS_SET_USER(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_USER));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_PW_CALLBACK_CLASS))
    {
        ret = RAMPART_ACTIONS_SET_PW_CB_CLASS(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_PW_CALLBACK_CLASS));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ENCRYPTION_PROP_FILE))
    {
        ret = RAMPART_ACTIONS_SET_ENC_PROP_FILE(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ENCRYPTION_PROP_FILE));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_DECRYPTION_PROP_FILE))
    {
        ret = RAMPART_ACTIONS_SET_DEC_PROP_FILE(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_DECRYPTION_PROP_FILE));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_SIGNATURE_PROP_FILE))
    {
        ret = RAMPART_ACTIONS_SET_SIG_PROP_FILE(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_SIGNATURE_PROP_FILE));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_SIGNATURE_KEY_IDENTIFIER))
    {
        ret = RAMPART_ACTIONS_SET_SIG_KEY_IDENTIFIER(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_SIGNATURE_KEY_IDENTIFIER));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ENCRYPTION_KEY_IDENTIFIER))
    {
        ret = RAMPART_ACTIONS_SET_ENC_KEY_IDENTIFIER(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ENCRYPTION_KEY_IDENTIFIER));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_SIGNATURE_PARTS))
    {
        ret = RAMPART_ACTIONS_SET_SIGNATURE_PARTS(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_SIGNATURE_PARTS));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_ENCRYPTION_PARTS))
    {
        ret = RAMPART_ACTIONS_SET_ENCRYPTION_PARTS(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_ENCRYPTION_PARTS));

    }

    if (rampart_get_action_params(env, param_action, RAMPART_ACTION_TIME_TO_LIVE))
    {
        ret = RAMPART_ACTIONS_SET_TIME_TO_LIVE(actions, env,
                (axis2_char_t *)rampart_get_action_params(
                    env, param_action, RAMPART_ACTION_TIME_TO_LIVE));
    }

    return AXIS2_SUCCESS;
}

/*Populate actions by extracting values from axis2 ctx*/
static axis2_status_t AXIS2_CALL
rampart_actions_populate_from_ctx(rampart_actions_t *actions,
        const axis2_env_t *env, axis2_ctx_t *ctx)
{
    axis2_status_t ret = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, ctx, AXIS2_FAILURE);
    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ENCRYPTION_USER))
    {
        ret = RAMPART_ACTIONS_SET_ENC_USER(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ENCRYPTION_USER));
    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ENCRYPTION_SYM_ALGORITHM))
    {
        ret = RAMPART_ACTIONS_SET_ENC_SYM_ALGO(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ENCRYPTION_SYM_ALGORITHM));
    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ENCRYPTION_KEY_TRANSFORM_ALGORITHM))
    {
        ret = RAMPART_ACTIONS_SET_ENC_KT_ALGO(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ENCRYPTION_KEY_TRANSFORM_ALGORITHM));
    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ITEMS))
    {
        ret = RAMPART_ACTIONS_SET_ITEMS(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ITEMS));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_PASSWORD_TYPE))
    {
        ret = RAMPART_ACTIONS_SET_PASSWORD_TYPE(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_PASSWORD_TYPE));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_USER))
    {
        ret = RAMPART_ACTIONS_SET_USER(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_USER));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_PW_CALLBACK_CLASS))
    {
        ret = RAMPART_ACTIONS_SET_PW_CB_CLASS(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_PW_CALLBACK_CLASS));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ENCRYPTION_PROP_FILE))
    {
        ret = RAMPART_ACTIONS_SET_ENC_PROP_FILE(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ENCRYPTION_PROP_FILE));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_DECRYPTION_PROP_FILE))
    {
        ret = RAMPART_ACTIONS_SET_DEC_PROP_FILE(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_DECRYPTION_PROP_FILE));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_SIGNATURE_PROP_FILE))
    {
        ret = RAMPART_ACTIONS_SET_SIG_PROP_FILE(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_SIGNATURE_PROP_FILE));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_SIGNATURE_KEY_IDENTIFIER))
    {
        ret = RAMPART_ACTIONS_SET_SIG_KEY_IDENTIFIER(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_SIGNATURE_KEY_IDENTIFIER));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ENCRYPTION_KEY_IDENTIFIER))
    {
        ret = RAMPART_ACTIONS_SET_ENC_KEY_IDENTIFIER(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ENCRYPTION_KEY_IDENTIFIER));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_SIGNATURE_PARTS))
    {
        ret = RAMPART_ACTIONS_SET_SIGNATURE_PARTS(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_SIGNATURE_PARTS));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_ENCRYPTION_PARTS))
    {
        ret = RAMPART_ACTIONS_SET_ENCRYPTION_PARTS(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_ENCRYPTION_PARTS));

    }

    if (rampart_get_property_from_ctx(env, ctx, RAMPART_ACTION_TIME_TO_LIVE))
    {
        ret = RAMPART_ACTIONS_SET_TIME_TO_LIVE(actions, env,
                (axis2_char_t *)rampart_get_property_from_ctx(
                    env, ctx, RAMPART_ACTION_TIME_TO_LIVE));
    }

    return AXIS2_SUCCESS;
}

static void
rampart_actions_init_ops(
    rampart_actions_t * actions)
{
    actions->ops->get_encryption_user = rampart_actions_get_encryption_user;
    actions->ops->set_encryption_user = rampart_actions_set_encryption_user;
    actions->ops->get_encryption_sym_algorithm = rampart_actions_get_encryption_sym_algorithm;
    actions->ops->set_encryption_sym_algorithm = rampart_actions_set_encryption_sym_algorithm;
    actions->ops->get_encryption_key_transport_algorithm = rampart_actions_get_encryption_key_transport_algorithm;
    actions->ops->set_encryption_key_transport_algorithm = rampart_actions_set_encryption_key_transport_algorithm;

    actions->ops->get_items = rampart_actions_get_items;
    actions->ops->set_items = rampart_actions_set_items;
    actions->ops->get_password_type = rampart_actions_get_password_type;
    actions->ops->set_password_type = rampart_actions_set_password_type;
    actions->ops->get_user = rampart_actions_get_user;
    actions->ops->set_user = rampart_actions_set_user;
    actions->ops->get_password_callback_class = rampart_actions_get_password_callback_class;
    actions->ops->set_password_callback_class = rampart_actions_set_password_callback_class;
    actions->ops->get_encryption_prop_file = rampart_actions_get_encryption_prop_file;
    actions->ops->set_encryption_prop_file = rampart_actions_set_encryption_prop_file;
    actions->ops->get_decryption_prop_file = rampart_actions_get_decryption_prop_file;
    actions->ops->set_decryption_prop_file = rampart_actions_set_decryption_prop_file;
    actions->ops->get_signature_prop_file = rampart_actions_get_signature_prop_file;
    actions->ops->set_signature_prop_file = rampart_actions_set_signature_prop_file;
    actions->ops->get_signature_key_identifier = rampart_actions_get_signature_key_identifier;
    actions->ops->set_signature_key_identifier = rampart_actions_set_signature_key_identifier;
    actions->ops->get_encryption_key_identifier = rampart_actions_get_encryption_key_identifier;
    actions->ops->set_encryption_key_identifier = rampart_actions_set_encryption_key_identifier;
    actions->ops->get_signature_parts = rampart_actions_get_signature_parts;
    actions->ops->set_signature_parts = rampart_actions_set_signature_parts;
    actions->ops->get_encryption_parts = rampart_actions_get_encryption_parts;
    actions->ops->set_encryption_parts = rampart_actions_set_encryption_parts;
    actions->ops->get_time_to_live = rampart_actions_get_time_to_live;
    actions->ops->set_time_to_live = rampart_actions_set_time_to_live;

    actions->ops->reset = rampart_actions_reset;
    actions->ops->free = rampart_actions_free;
    actions->ops->populate_from_params = rampart_actions_populate_from_params;
    actions->ops->populate_from_ctx = rampart_actions_populate_from_ctx;
}
