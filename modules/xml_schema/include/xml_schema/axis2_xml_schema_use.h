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

#ifndef AXIS2_XML_SCHEMA_USE_H
#define AXIS2_XML_SCHEMA_USE_H

/**
 * @file axis2_xml_schema_use.h
 * @brief Axis2 Xml Schema Use Interface
 *          Indicator of how the attribute is used.
 *
 */

#include <xml_schema/axis2_xml_schema_defines.h>
#include <xml_schema/axis2_xml_schema_enum.h>

/** @defgroup axis2_xml_schema_use Xml Schema Use
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_use axis2_xml_schema_use_t;
typedef struct axis2_xml_schema_use_ops axis2_xml_schema_use_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_use_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *use,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *use,
            axis2_env_t **env);

    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (
            void *use,
            axis2_env_t **env);

    axis2_xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *use,
                    axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *use,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_use
{
    axis2_xml_schema_enum_t base;
    axis2_xml_schema_use_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_use_t *)
axis2_xml_schema_use_create(axis2_env_t **env,
                                    axis2_char_t* value);

/******************* macros ***************************************/

#define AXIS2_XML_SCHEMA_USE_FREE(use, env) \
		(((axis2_xml_schema_use_t *) use)->ops->free(use, env))

#define AXIS2_XML_SCHEMA_USE_SUPER_OBJS(use, env) \
		(((axis2_xml_schema_use_t *) use)->ops->super_objs(use, env))

#define AXIS2_XML_SCHEMA_USE_TYPE(use, env) \
		(((axis2_xml_schema_use_t *) use)->ops->type(use, env))

#define AXIS2_XML_SCHEMA_USE_GET_BASE_IMPL(use, env) \
		(((axis2_xml_schema_use_t *) use)->ops->get_base_impl(use, env))

#define AXIS2_XML_SCHEMA_USE_GET_VALUES(use, env) \
		(((axis2_xml_schema_use_t *) use)->ops->values(use, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_USE_H */
