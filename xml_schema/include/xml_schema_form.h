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

#ifndef XML_SCHEMA_FORM_H
#define XML_SCHEMA_FORM_H

/**
 * @file xml_schema_form.h
 * @brief Axis2 Xml Schema Form Interface
 *          Indicates if attributes or elements need to be qualified or left 
 *          unqualified.
 *
 */

#include <xml_schema_defines.h>
#include <xml_schema_enum.h>

/** @defgroup xml_schema_form Xml Schema Form
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_form xml_schema_form_t;
typedef struct xml_schema_form_ops xml_schema_form_ops_t;

#define XML_SCHEMA_FORM_QUALIFIED "qualified"
#define XML_SCHEMA_FORM_UNQUALIFIED "unqualified"
#define XML_SCHEMA_FORM_NONE "none"

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_form_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *form,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *form,
            const axis2_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (void *form,
            const axis2_env_t *env);

    xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *form,
                    const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *form,
                const axis2_env_t *env);
    
};

struct xml_schema_form
{
    xml_schema_enum_t base;
    xml_schema_form_ops_t *ops;
};

AXIS2_EXTERN xml_schema_form_t * AXIS2_CALL
xml_schema_form_create(const axis2_env_t *env,
                                    axis2_char_t* value);

/***************** Macros ************************************************/

#define XML_SCHEMA_FORM_FREE(form, env) \
      (((xml_schema_form_t *) form)->ops->free(form, env))

#define XML_SCHEMA_FORM_SUPER_OBJS(form, env) \
      (((xml_schema_form_t *) form)->ops->super_objs(form, env))

#define XML_SCHEMA_FORM_GET_TYPE(form, env) \
      (((xml_schema_form_t *) form)->ops->type(form, env))

#define XML_SCHEMA_FORM_GET_BASE_IMPL(form, env) \
      (((xml_schema_form_t *) form)->ops->get_base_impl(form, env))

#define XML_SCHEMA_FORM_GET_VALUES(form, env) \
      (((xml_schema_form_t *) form)->ops->values(form, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_FORM_H */
