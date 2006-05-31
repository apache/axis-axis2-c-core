/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WODEN_NC_NAME_H
#define WODEN_NC_NAME_H

/**
 * @file woden_nc_name.h
 * @brief Axis2 NC Name Interface
 * This class represents the data type NCName use for XML non-colonized names.
 * It is based on the class of the same name in Apache Axis.
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_nc_name woden_nc_name_t;
typedef struct woden_nc_name_ops woden_nc_name_ops_t;
struct axis2_xml_nc_name;

/** @defgroup woden_nc_name NC Name
  * @ingroup axis2_wsdl
  * @{
  */

struct woden_nc_name_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *nc_name,
            const axis2_env_t *env);
 
    axis2_bool_t (AXIS2_CALL *
    is_valid) (
            void *nc_name,
            const axis2_env_t *env,
            axis2_char_t *st_value);

    /**
     * 
     * @param value String representing an NCName
     */
    axis2_status_t (AXIS2_CALL *
    set_value) (
            void *nc_name,
            const axis2_env_t *env,
            axis2_char_t *value);

    axis2_char_t *(AXIS2_CALL *
    to_string) (
            void *nc_name,
            const axis2_env_t *env);

   
};

struct woden_nc_name
{
    woden_nc_name_ops_t *ops;
};

AXIS2_DECLARE(woden_nc_name_t *)
woden_nc_name_create(
        const axis2_env_t *env,
        axis2_char_t *value);

#define WODEN_NC_NAME_FREE(nc_name, env) \
		(((woden_nc_name_t *) nc_name)->ops->free (nc_name, env))

#define WODEN_NC_NAME_IS_VALID(nc_name, env, st_value) \
		(((woden_nc_name_t *) nc_name)->ops-> \
         is_valid (nc_name, env, st_value))

#define WODEN_NC_NAME_SET_VALUE(nc_name, env, value) \
		(((woden_nc_name_t *) nc_name)->ops-> \
         set_value (nc_name, env, value))

#define WODEN_NC_NAME_TO_STRING(nc_name, env) \
		(((woden_nc_name_t *) nc_name)->ops-> \
         to_string (nc_name, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_NC_NAME_H */
