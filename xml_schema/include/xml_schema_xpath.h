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

#ifndef XML_SCHEMA_XPATH_H
#define XML_SCHEMA_XPATH_H

/**
 * @file xml_schema_xpath.h
 * @brief Axis2 Xml Schema Xpath Interface
 *          Class for XML Path Language (XPath) expressions. Represents the
 *          World Wide Web Consortium (W3C) selector element. The World Wide
 *          Web Consortium (W3C) field element is a collection of
 *          Xml Schema XPath classes.
 */

#include <xml_schema_annotated.h>
#include <axiom_element.h>
#include <axiom_node.h>
#include <axutil_hash.h>

/** @defgroup xml_schema_xpath Xml Schema Xpath
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_xpath xml_schema_xpath_t;
typedef struct xml_schema_xpath_ops xml_schema_xpath_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_xpath_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *xpath,
            const axutil_env_t *env);

    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *xpath,
                    const axutil_env_t *env);
                    
    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *xpath,
          const axutil_env_t *env);
          
    axutil_hash_t* (AXIS2_CALL *
    super_objs)(void *xpath,
                const axutil_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_xpath)(void *xpath,
                const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_xpath)(void *xpath,
                    const axutil_env_t *env,
                    axis2_char_t *x_path);

};

struct xml_schema_xpath
{
    xml_schema_annotated_t base;
    xml_schema_xpath_ops_t *ops;
};

/**
 * Creates new Xml Schema Xpath
 */
AXIS2_EXTERN xml_schema_xpath_t * AXIS2_CALL
xml_schema_xpath_create(const axutil_env_t *env);

 
#define XML_SCHEMA_XPATH_FREE(xpath, env) \
      (((xml_schema_xpath_t *) xpath)->ops->\
            free(xpath, env))

#define XML_SCHEMA_XPATH_GET_BASE_IMPL(xpath, env) \
      (((xml_schema_xpath_t *) xpath)->ops->\
            get_base_impl(xpath, env))

#define XML_SCHEMA_XPATH_GET_TYPE(xpath, env) \
      (((xml_schema_xpath_t *) xpath)->ops->\
            get_type(xpath, env))

#define XML_SCHEMA_XPATH_SUPER_OBJS(xpath, env) \
      (((xml_schema_xpath_t *) xpath)->ops->\
            super_objs(xpath, env))

#define XML_SCHEMA_XPATH_GET_XPATH(xpath, env) \
      (((xml_schema_xpath_t *) xpath)->ops->\
            get_xpath(xpath, env))

#define XML_SCHEMA_XPATH_SET_XPATH(xpath, env, x_path) \
      (((xml_schema_xpath_t *) xpath)->ops->\
            set_xpath(xpath, env, x_path))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_XPATH_H */
