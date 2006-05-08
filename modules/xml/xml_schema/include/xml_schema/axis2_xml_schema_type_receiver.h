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

#ifndef AXIS2_XML_SCHEMA_TYPE_RECEIVER_H
#define AXIS2_XML_SCHEMA_TYPE_RECEIVER_H

/**
 * @file axis2_xml_schema_type_receiver.h
 * @brief Axis2 Xml Schema Type Receiver  interface
 *          A TypeReceiver is something that can have its type set.  
 *          This gets used to resolve forward references.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_type_receiver axis2_xml_schema_type_receiver_t;
typedef struct axis2_xml_schema_type_receiver_ops axis2_xml_schema_type_receiver_ops_t;
struct axis2_xml_schema_type;

/** @defgroup axis2_xml_schema_type_receiver Xml Schema Type Receiver
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_type_receiver_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *type_receiver,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_type) (void *type_receiver,
               axis2_env_t **env,
               struct axis2_xml_schema_type *type);

};

struct axis2_xml_schema_type_receiver
{
    axis2_xml_schema_type_receiver_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_type_receiver_t *)
axis2_xml_schema_type_receiver_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_type_receiver_resolve_methods(
                    axis2_xml_schema_type_receiver_t *schema_type_receiver,
                    axis2_env_t **env,
                    axis2_xml_schema_type_receiver_t *schema_type_receiver_impl,
                    axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_TYPE_RECEIVER_FREE(type_receiver, env) \
		(((axis2_xml_schema_type_receiver_t *) type_receiver)->ops->free (\
        type_receiver, env))

#define AXIS2_XML_SCHEMA_TYPE_RECEIVER_SET_TYPE(type_receiver, env, type) \
		(((axis2_xml_schema_type_receiver_t *) type_receiver)->ops->set_type (\
        type_receiver, env, type))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_TYPE_RECEIVER_H */
