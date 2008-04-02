
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_ws_info_list.h>
#include <string.h>
#include <axutil_class_loader.h>

struct axis2_ws_info_list
{
    /**
     * This is to store all the deployment info about the deployment files 
     * for services or modules in a specified folder.
     */
    axutil_array_list_t *ws_info_list;

    /**
     * All the curently updated deployment info list.
     */
    axutil_array_list_t *current_info_list;

    /**
     * Referance to deployment engine to make update.
     */
    struct axis2_dep_engine *dep_engine;

};

AXIS2_EXTERN axis2_ws_info_list_t *AXIS2_CALL
axis2_ws_info_list_create_with_dep_engine(
    const axutil_env_t * env,
    struct axis2_dep_engine *dep_engine)
{
    axis2_ws_info_list_t *ws_info_list = NULL;

    ws_info_list = (axis2_ws_info_list_t *) AXIS2_MALLOC(env->allocator,
                                                         sizeof
                                                         (axis2_ws_info_list_t));

    if (!ws_info_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset ((void *)ws_info_list, 0, sizeof (axis2_ws_info_list_t));

    ws_info_list->dep_engine = dep_engine;

    ws_info_list->ws_info_list = axutil_array_list_create(env, 0);
    if (!(ws_info_list->ws_info_list))
    {
        axis2_ws_info_list_free(ws_info_list, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ws_info_list->current_info_list = axutil_array_list_create(env, 0);
    if (!(ws_info_list->current_info_list))
    {
        axis2_ws_info_list_free(ws_info_list, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    return ws_info_list;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_ws_info_list_free(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env)
{
    if (ws_info_list->current_info_list)
    {
        int list_size = 0;
        int i = 0;

        list_size =
            axutil_array_list_size(ws_info_list->current_info_list, env);
        for (i = 0; i < list_size; i++)
        {
            axis2_char_t *file_name = NULL;

            file_name = (axis2_char_t *)
                axutil_array_list_get(ws_info_list->current_info_list, env, i);
            AXIS2_FREE(env->allocator, file_name);
        }
        axutil_array_list_free(ws_info_list->current_info_list, env);
    }

    if (ws_info_list->ws_info_list)
    {
        int list_size = 0;
        int i = 0;

        list_size = axutil_array_list_size(ws_info_list->ws_info_list, env);
        for (i = 0; i < list_size; i++)
        {
            axis2_ws_info_t *ws_info = NULL;

            ws_info = (axis2_ws_info_t *) axutil_array_list_get(ws_info_list->
                                                                ws_info_list,
                                                                env, i);
            axis2_ws_info_free(ws_info, env);
        }
        axutil_array_list_free(ws_info_list->ws_info_list, env);
    }

    if (ws_info_list)
    {
        AXIS2_FREE(env->allocator, ws_info_list);
    }

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ws_info_list_init(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env)
{
    int size = 0;
    int i = 0;

    size = axutil_array_list_size(ws_info_list->ws_info_list, env);
    for (i = 0; i < size; i++)
    {
        axis2_ws_info_t *ws_info = NULL;

        ws_info = (axis2_ws_info_t *)
            axutil_array_list_get(ws_info_list->ws_info_list, env, i);
        axis2_ws_info_free(ws_info, env);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ws_info_list_add_ws_info_item(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env,
    axutil_file_t * file,
    int type)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *file_name = NULL;
    axis2_char_t *temp_name = NULL;

    AXIS2_PARAM_CHECK(env->error, file, AXIS2_FAILURE);

    temp_name = axutil_file_get_name(file, env);
    file_name = axutil_strdup(env, temp_name);
    if (!file_name)
    {
        return AXIS2_FAILURE;
    }

    switch (type)
    {
    case AXIS2_SVC:
        {
            axis2_ws_info_t *ws_info = NULL;
            long last_modified_date = 0;
            axis2_arch_file_data_t *file_data = NULL;

            last_modified_date = (long)axutil_file_get_timestamp(file, env);
            /* We are sure that the difference lies within the long range */
            ws_info =
                axis2_ws_info_create_with_file_name_and_last_modified_date_and_type
                (env, file_name, last_modified_date, AXIS2_SVC);
            status =
                axutil_array_list_add(ws_info_list->ws_info_list, env, ws_info);

            if (AXIS2_SUCCESS != status)
            {
                return status;
            }
            file_data = axis2_arch_file_data_create_with_type_and_file(env,
                                                                       AXIS2_SVC,
                                                                       file);
            /* To inform that new web service is to be deployed */
            status = axis2_dep_engine_add_ws_to_deploy(ws_info_list->dep_engine,
                                                       env, file_data);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }
            break;
        }
    case AXIS2_MODULE:
        {
            axis2_ws_info_t *ws_info = NULL;
            long last_modified_date = 0;
            axis2_arch_file_data_t *file_data = NULL;

            last_modified_date = (long)axutil_file_get_timestamp(file, env);
            /* We are sure that the difference lies within the long range */
            ws_info =
                axis2_ws_info_create_with_file_name_and_last_modified_date_and_type
                (env, file_name, last_modified_date, AXIS2_MODULE);
            status =
                axutil_array_list_add(ws_info_list->ws_info_list, env, ws_info);

            if (AXIS2_SUCCESS != status)
            {
                return status;
            }
            file_data = axis2_arch_file_data_create_with_type_and_file(env,
                                                                       AXIS2_MODULE,
                                                                       file);
            /* To inform that new module is to be deployed */
            status = axis2_dep_engine_add_ws_to_deploy(ws_info_list->dep_engine,
                                                       env, file_data);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }

            break;
        }
    }
    return axutil_array_list_add(ws_info_list->current_info_list, env,
                                 file_name);
}

AXIS2_EXTERN axis2_ws_info_t *AXIS2_CALL
axis2_ws_info_list_get_file_item(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env,
    axis2_char_t * file_name)
{
    int i = 0;
    int size = 0;
    AXIS2_PARAM_CHECK(env->error, file_name, NULL);

    size = axutil_array_list_size(ws_info_list->ws_info_list, env);
    for (i = 0; i < size; i++)
    {
        axis2_ws_info_t *ws_info = NULL;
        axis2_char_t *file_name_l = NULL;

        ws_info = (axis2_ws_info_t *) axutil_array_list_get(ws_info_list->
                                                            ws_info_list, env,
                                                            i);

        file_name_l = axis2_ws_info_get_file_name(ws_info, env);
        if (!axutil_strcmp(file_name_l, file_name))
        {
            return ws_info;
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_modified(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env,
    axutil_file_t * file,
    axis2_ws_info_t * ws_info)
{
    long last_modified_date = 0;

    AXIS2_PARAM_CHECK(env->error, file, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ws_info, AXIS2_FAILURE);

    last_modified_date = axis2_ws_info_get_last_modified_date(ws_info, env);
    return (last_modified_date != axutil_file_get_timestamp(file, env));
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_file_exist(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env,
    axis2_char_t * file_name)
{
    axis2_ws_info_t *ws_info = NULL;

    AXIS2_PARAM_CHECK(env->error, file_name, AXIS2_FAILURE);
    ws_info = axis2_ws_info_list_get_file_item(ws_info_list, env, file_name);
    return !(ws_info == NULL);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ws_info_list_check_for_undeploy(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env)
{
    int list_size = 0;
    axutil_array_list_t *temp_list = NULL;
    int i = 0;

    /* Create temp list */
    temp_list = axutil_array_list_create(env, 0);
    if (!temp_list)
    {
        return AXIS2_FAILURE;
    }
    list_size = axutil_array_list_size(ws_info_list->ws_info_list, env);

    for (i = 0; i < list_size; i++)
    {
        /* For each ws_info item in the ws_info_list check whether it is also
         * in the current_info_list. If not mark it to be undeployed by adding
         * it to a temporarily list. Also add that item to the undeploy list of
         * deployment engine.*/
        int current_lists_size = 0;
        axis2_ws_info_t *file_item = NULL;
        axis2_char_t *file_item_name = NULL;
        axis2_bool_t exist = AXIS2_FALSE;
        int j = 0;

        file_item = (axis2_ws_info_t *) axutil_array_list_get(ws_info_list->
                                                              ws_info_list, env,
                                                              i);

        file_item_name = axis2_ws_info_get_file_name(file_item, env);
        current_lists_size =
            axutil_array_list_size(ws_info_list->current_info_list, env);
        for (j = 0; j < current_lists_size; j++)
        {
            axis2_char_t *file_name = NULL;
            file_name = (axis2_char_t *) axutil_array_list_get(ws_info_list->
                                                               current_info_list,
                                                               env, j);
            if (!axutil_strcmp(file_name, file_item_name))
            {
                exist = AXIS2_TRUE;
                break;
            }
        }

        if (!exist)
        {
            axis2_ws_info_t *ws_info = NULL;
            long last_modified_date = 0;

            last_modified_date = axis2_ws_info_get_last_modified_date(file_item,
                                                                      env);
            axutil_array_list_add(temp_list, env, file_item);
            ws_info =
                axis2_ws_info_create_with_file_name_and_last_modified_date(env,
                                                                           file_item_name,
                                                                           last_modified_date);
            /* This ws_info is to be undeployed */
            axis2_dep_engine_add_ws_to_undeploy(ws_info_list->dep_engine, env,
                                                ws_info);
        }

    }

    list_size = axutil_array_list_size(temp_list, env);
    for (i = 0; i < list_size; i++)
    {
        /* Get each ws_info item from the temp list created and remove that
         * item from the ws_info_list */
        axis2_ws_info_t *file_item = NULL;
        int index = 0;

        file_item =
            (axis2_ws_info_t *) axutil_array_list_get(temp_list, env, i);
        index =
            axutil_array_list_index_of(ws_info_list->ws_info_list, env,
                                       file_item);
        axutil_array_list_remove(ws_info_list->ws_info_list, env, index);
    }
    axutil_array_list_free(temp_list, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ws_info_list_update(
    axis2_ws_info_list_t * ws_info_list,
    const axutil_env_t * env)
{
    axis2_status_t status = AXIS2_FAILURE;

    status = axis2_ws_info_list_check_for_undeploy(ws_info_list, env);
    if (AXIS2_SUCCESS != status)
    {
        return AXIS2_FAILURE;
    }

    return axis2_dep_engine_do_deploy(ws_info_list->dep_engine, env);
}

