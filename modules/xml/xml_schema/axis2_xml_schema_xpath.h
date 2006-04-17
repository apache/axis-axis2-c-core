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

#ifndef AXIS2_XML_SCHEMA_XPATH_H
#define AXIS2_XML_SCHEMA_XPATH_H

/**
 * @file axis2_xml_schema_xpath.h
 * @brief Axis2 Xml Schema Xpath Interface
 *          Class for XML Path Language (XPath) expressions. Represents the
 *          World Wide Web Consortium (W3C) selector element. The World Wide
 *          Web Consortium (W3C) field element is a collection of
 *          Xml Schema XPath classes.
 */

#include <axis2_xml_schema_annotated.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_xpath Xml Schema Xpath
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_xpath axis2_xml_schema_xpath_t;
typedef struct axis2_xml_schema_xpath_ops axis2_xml_schema_xpath_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_xpath_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *xpath,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *xpath,
                    axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_xpath)(void *xpath,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_xpath)(void *xpath,
                    axis2_env_t **env,
                    axis2_char_t *x_path);

};

struct axis2_xml_schema_xpath
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_xpath_ops_t *ops;
};

/**
 * Creates new Xml Schema Xpath
 */
AXIS2_DECLARE(axis2_xml_schema_xpath_t *)
axis2_xml_schema_xpath_create(axis2_env_t **env,
                                void *value,
                                axis2_bool_t fixed);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_xpath_resolve_methods(
                                axis2_xml_schema_xpath_t *xpath,
                                axis2_env_t **env,
                                axis2_xml_schema_xpath_t *xpath_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_XPATH_FREE(xpath, env) \
		(((axis2_xml_schema_xpath_t *) xpath)->ops->\
            free(xpath, env))

#define AXIS2_XML_SCHEMA_XPATH_GET_BASE_IMPL(xpath, env) \
		(((axis2_xml_schema_xpath_t *) xpath)->ops->\
            get_base_impl(xpath, env))

#define AXIS2_XML_SCHEMA_XPATH_GET_XPATH(xpath, env) \
		(((axis2_xml_schema_xpath_t *) xpath)->ops->\
            get_xpath(xpath, env))

#define AXIS2_XML_SCHEMA_XPATH_SET_XPATH(xpath, env, x_path) \
		(((axis2_xml_schema_xpath_t *) xpath)->ops->\
            set_xpath(xpath, env, x_path))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_XPATH_H */
