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
#include <axis2_defines.h>
#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_buffer.h>
#include <oxs_axiom.h>
#include <oxs_error.h>



typedef struct oxs_buffer_impl
{
    oxs_buffer_t buffer;

    unsigned char* data;
    unsigned int size;
    unsigned int max_size;
    oxs_AllocMode alloc_mode;
}
oxs_buffer_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(oxs_buffer) ((oxs_buffer_impl_t *)oxs_buffer)

/******** function headers ***************/

/*private functions*/
static void
oxs_buffer_init_ops(
    oxs_buffer_t *buffer);

/*public functions*/

axis2_status_t AXIS2_CALL
oxs_buffer_free(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
);

axis2_status_t AXIS2_CALL
oxs_buffer_remove_head(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
);

axis2_status_t AXIS2_CALL
oxs_buffer_remove_tail(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
);

axis2_status_t AXIS2_CALL
oxs_buffer_populate(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    unsigned char *data,
    int size
);

axis2_status_t AXIS2_CALL
oxs_buffer_append(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    unsigned char *data,
    int size
);

axis2_status_t AXIS2_CALL
oxs_buffer_prepend(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    unsigned char *data,
    int size
);

axis2_status_t AXIS2_CALL
oxs_buffer_read_file(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    const axis2_char_t *filename
);

axis2_status_t AXIS2_CALL
oxs_buffer_set_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
);

axis2_status_t AXIS2_CALL
oxs_buffer_set_max_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
);

unsigned char* AXIS2_CALL
oxs_buffer_get_data(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
);

int AXIS2_CALL
oxs_buffer_get_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
);

int AXIS2_CALL
oxs_buffer_get_max_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
);


/******************** end of function headers *****************/
static void
oxs_buffer_init_ops(
    oxs_buffer_t *buffer)
{
    buffer->ops->free  = oxs_buffer_free ;
    buffer->ops->remove_head  = oxs_buffer_remove_head ;
    buffer->ops->remove_tail  = oxs_buffer_remove_tail ;
    buffer->ops->populate  = oxs_buffer_populate ;
    buffer->ops->append  = oxs_buffer_append ;
    buffer->ops->prepend  = oxs_buffer_prepend ;
    buffer->ops->read_file = oxs_buffer_read_file ;
    buffer->ops->set_size  = oxs_buffer_set_size ;
    buffer->ops->set_max_size  = oxs_buffer_set_max_size ;
    buffer->ops->get_data  = oxs_buffer_get_data ;
    buffer->ops->get_size  = oxs_buffer_get_size ;
    buffer->ops->get_max_size  = oxs_buffer_get_max_size ;
}

AXIS2_EXTERN oxs_buffer_t *AXIS2_CALL
oxs_buffer_create(const axis2_env_t *env)
{
    oxs_buffer_impl_t *buffer_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, NULL);

    buffer_impl = AXIS2_MALLOC(env->allocator, sizeof(oxs_buffer_impl_t));
    if (!buffer_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    buffer_impl->data = NULL;
    buffer_impl->size = 0;
    buffer_impl->max_size = 0;
    buffer_impl->alloc_mode = oxs_alloc_mode_double;

    status = oxs_buffer_set_max_size(&(buffer_impl->buffer), env, OXS_BUFFER_INITIAL_SIZE);
    if (status == AXIS2_FAILURE)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_DEFAULT,
                "oxs_buffer_set_max_size");
        return NULL;
    }


    buffer_impl->buffer.ops =  AXIS2_MALLOC(env->allocator, sizeof(oxs_buffer_ops_t));
    if (!buffer_impl->buffer.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        oxs_buffer_free(&(buffer_impl->buffer), env);
        return NULL;
    }

    oxs_buffer_init_ops(&(buffer_impl->buffer));

    return &(buffer_impl->buffer);

}

axis2_status_t AXIS2_CALL
oxs_buffer_free(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    if (buffer_impl->data)
    {
        AXIS2_FREE(env->allocator,  buffer_impl->data);
        buffer_impl->data = NULL;
    }

    AXIS2_FREE(env->allocator,  buffer_impl);
    buffer_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_remove_head(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    /*If the size to be removed is less than the buffer size*/
    if (size < buffer_impl->size)
    {
        if (buffer_impl->data != NULL)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "oxs_buffer_remove_head failed");
            return  AXIS2_FAILURE;
        }
        buffer_impl->size -= size;
        memmove(buffer_impl->data, buffer_impl->data + size, buffer_impl->size);
    }
    else
    {
        buffer_impl->size = 0;
    }

    /*If the buffer size is less than the max_size.*/
    if (buffer_impl->size < buffer_impl->max_size)
    {
        if (buffer_impl->data != NULL)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "oxs_buffer_remove_head failed");
            return  AXIS2_FAILURE;
        }
        memset(buffer_impl->data + buffer_impl->size, 0, buffer_impl->max_size - buffer_impl->size);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_remove_tail(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    if (size < buffer_impl->size)
    {
        buffer_impl->size -= size;
    }
    else
    {
        buffer_impl->size = 0;
    }
    if (buffer_impl->size < buffer_impl->max_size)
    {
        if (buffer_impl->data != NULL)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "");
            return  AXIS2_FAILURE;
        }
        memset(buffer_impl->data + buffer_impl->size, 0, buffer_impl->max_size - buffer_impl->size);
    }


    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_populate(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    unsigned char *data,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    if (size > 0)
    {
        oxs_buffer_set_max_size(&(buffer_impl->buffer), env, size);
        if (!data)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "data is NULL");
            return AXIS2_FAILURE;
        }

        memcpy(buffer_impl->data, data, size);
        buffer_impl->size = size;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_append(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    unsigned char *data,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    if (size > 0)
    {
        oxs_buffer_set_max_size(&(buffer_impl->buffer), env,  buffer_impl->size + size);
        if (!data)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "data is NULL");
            return AXIS2_FAILURE;
        }

        memcpy(buffer_impl->data + buffer_impl->size, data, size);
        buffer_impl->size += size;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_prepend(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    unsigned char *data,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    if (size > 0)
    {
        if (!data)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "Passed data is NULL");
            return AXIS2_FAILURE;
        }

        buffer_impl->max_size = buffer_impl->size + size;

        memmove(buffer_impl->data + size, buffer_impl->data, buffer_impl->size);
        memcpy(buffer_impl->data, data, size);
        buffer_impl->size += size;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_read_file(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    const axis2_char_t *filename
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;
    unsigned char fbuffer[1024];
    FILE* f;
    int  len;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);


    f = fopen(filename, "rb");
    if (f == NULL)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "");
        return AXIS2_FAILURE;
    }

    while (1)
    {
        len = fread(fbuffer, 1, sizeof(fbuffer), f);
        if (len == 0)
        {
            break; /*Stop reading*/
        }
        else if (len < 0)
        {
            fclose(f);
            return AXIS2_FAILURE;
        }
        status = oxs_buffer_append(&(buffer_impl->buffer), env, fbuffer, len);
        if (status == AXIS2_FAILURE)
        {
            fclose(f);
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "");
            return AXIS2_FAILURE;
        }

        /*Alright so far everything is fine. So let's close the output*/
        fclose(f);
        return AXIS2_SUCCESS;
    }/*End of while*/

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_set_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    /*First we need to make sure that the max size has a value greater or equal value*/
    status = oxs_buffer_set_max_size(buffer, env,  size);
    if (status == AXIS2_FAILURE)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "oxs_buffer_set_max_size failed");
        return AXIS2_FAILURE;
    }
    /*Now set the size*/
    buffer_impl->size = size;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_buffer_set_max_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env,
    int size
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;
    unsigned char* new_data;
    unsigned int new_size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    if (size <= buffer_impl->max_size)
    {
        return AXIS2_SUCCESS;
    }

    switch (buffer_impl->alloc_mode)
    {
        case oxs_alloc_mode_exact:
            new_size = size + 8;
            break;
        case oxs_alloc_mode_double:
            new_size = 2 * size + 32;
            break;
    }

    if (new_size < OXS_BUFFER_INITIAL_SIZE)
    {
        new_size = OXS_BUFFER_INITIAL_SIZE;
    }

    /*If there are data already then use realloc instead of malloc*/
    if (buffer_impl->data != NULL)
    {
        new_data = (unsigned char*)AXIS2_REALLOC(env->allocator, buffer_impl->data, new_size);
    }
    else
    {
        new_data = (unsigned char*)AXIS2_MALLOC(env->allocator, new_size);
    }

    if (new_data == NULL)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "");
        return AXIS2_FAILURE;
    }

    buffer_impl->data = new_data;
    buffer_impl->max_size = new_size;

    if (buffer_impl->size < buffer_impl->max_size)
    {
        if (buffer_impl->data == NULL)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "");
            return AXIS2_FAILURE;
        }
        memset(buffer_impl->data + buffer_impl->size, 0, buffer_impl->max_size - buffer_impl->size);
    }

    return AXIS2_SUCCESS;
}

unsigned char* AXIS2_CALL
oxs_buffer_get_data(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    return buffer_impl->data;
}

int AXIS2_CALL
oxs_buffer_get_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    return buffer_impl->size;
}

int AXIS2_CALL
oxs_buffer_get_max_size(
    oxs_buffer_t *buffer,
    const axis2_env_t *env
)
{
    oxs_buffer_impl_t *buffer_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    buffer_impl = AXIS2_INTF_TO_IMPL(buffer);

    return buffer_impl->max_size;
}

