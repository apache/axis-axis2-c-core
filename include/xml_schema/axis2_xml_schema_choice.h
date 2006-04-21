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

#ifndef AXIS2_XML_SCHEMA_CHOICE_H
#define AXIS2_XML_SCHEMA_CHOICE_H

/**
 * @file axis2_xml_schema_choice.h
 * @brief Axis2 Xml Schema Choice Interface
 *          Allows only one of its children to appear in an instance. Represents
 *          the World Wide Web Consortium (W3C) choice (compositor) element.
 */

#include <axis2_xml_schema_group_base.h>
#include <axis2_hash.h>

/** @defchoice axis2_xml_schema_choice Xml Schema Choice
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_choice 
                axis2_xml_schema_choice_t;
typedef struct axis2_xml_schema_choice_ops 
                axis2_xml_schema_choice_ops_t;
                
struct axis2_xml_schema_group_base;
struct axis2_xml_schema_obj_collection;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_choice_ops
{
	/** 
     * Dechoiceocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *choice,
            axis2_env_t **env);

    struct axis2_xml_schema_group_base *(AXIS2_CALL *
    get_base_impl) (void *choice,
                    axis2_env_t **env);

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_items)(void *choice,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_choice
{
    struct axis2_xml_schema_group_base base;
    axis2_xml_schema_choice_ops_t *ops;
};

/**
 * Creates new Xml Schema Choice
 */
AXIS2_DECLARE(axis2_xml_schema_choice_t *)
axis2_xml_schema_choice_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is cchoiceed from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_choice_resolve_methods(
                                axis2_xml_schema_choice_t *choice,
                                axis2_env_t **env,
                                axis2_xml_schema_choice_t *choice_impl,
                                axis2_hash_t *methods);
/***************** MACROS *****************************************************/

#define AXIS2_XML_SCHEMA_CHOICE_FREE(choice, env) \
		(((axis2_xml_schema_choice_t *) choice)->ops->\
            free(choice, env))

#define AXIS2_XML_SCHEMA_CHOICE_GET_BASE_IMPL(choice, env) \
		(((axis2_xml_schema_choice_t *) choice)->ops->\
            get_base_impl(choice, env))

#define AXIS2_XML_SCHEMA_CHOICE_GET_ITEMS(choice, env) \
		(((axis2_xml_schema_choice_t *) choice)->ops->\
            get_items(choice, env))

/**************** END MACROS ****************************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_CHOICE_H */
