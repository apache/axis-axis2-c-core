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

#ifndef XML_SCHEMA_DOCUMENTATION_H
#define XML_SCHEMA_DOCUMENTATION_H

/**
 * @file xml_schema_documentation.h
 * @brief Axis2 Xml Schema App Info Interface
 *          Defines application specific information within an annotation.
 *          Represents the World Wide Web Consortium (W3C) appinfo element.
 *
 */

#include <xml_schema_defines.h>
#include <xml_schema_obj.h>
#include <axis2_hash.h>
#include <axiom.h>

/** @defgroup xml_schema_documentation Xml Schema App Info
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_documentation 
                    xml_schema_documentation_t;
typedef struct xml_schema_documentation_ops 
                    xml_schema_documentation_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_documentation_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *documentation,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *documentation,
            const axis2_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *documentation,
            const axis2_env_t *env);

    xml_schema_obj_t *(AXIS2_CALL *
    get_base_impl) (void *documentation,
                        const axis2_env_t *env);
    
    axis2_char_t *(AXIS2_CALL *
    get_source)(void *documentation,
                const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_source) (void *documentation,
                const axis2_env_t *env,
                axis2_char_t *source);

    axiom_child_element_iterator_t *(AXIS2_CALL *
    get_markup) (void *documentation,
                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_markup) (void *documentation,
                 const axis2_env_t *env,
                 axiom_child_element_iterator_t *markup);
                 
    axis2_char_t *(AXIS2_CALL *
    get_language)(
            void *documentation,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL *
    set_language)(
            void *documentation,
            const axis2_env_t *env,
            axis2_char_t *language);            
};

struct xml_schema_documentation
{
    xml_schema_obj_t base;
    xml_schema_documentation_ops_t *ops;
};

AXIS2_EXTERN xml_schema_documentation_t * AXIS2_CALL
xml_schema_documentation_create(const axis2_env_t *env);


/*********************** macros ***********************************************/

#define XML_SCHEMA_DOCUMENTATION_FREE(documentation, env) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            free(documentation, env))

#define XML_SCHEMA_DOCUMENTATION_SUPER_OBJS(documentation, env) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            super_objs(documentation, env))

#define XML_SCHEMA_DOCUMENTATION_GET_TYPE(documentation, env) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            GET_type(documentation, env))

#define XML_SCHEMA_DOCUMENTATION_GET_SOURCE(documentation, env) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            get_source(documentation, env))

#define XML_SCHEMA_DOCUMENTATION_SET_SOURCE(documentation, env, source) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            set_source(documentation, env, source))

#define XML_SCHEMA_DOCUMENTATION_GET_MARKUP(documentation, env) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            get_markup(documentation, env))

#define XML_SCHEMA_DOCUMENTATION_SET_MARKUP(documentation, env, markup) \
      (((xml_schema_documentation_t *) documentation)->ops->\
            set_markup(documentation, env, markup))


#define XML_SCHEMA_DOCUMENTATION_GET_LANGUAGE(documentation, env) \
        (((xml_schema_documentation_t*) documentation)->ops->\
            get_language(documentation, env))
            
#define XML_SCHEMA_DOCUMENTATION_SET_LANGUAGE(documentation, env, language)\
        (((xml_schema_documentation_t *) documentation)->ops->\
            set_language(documentation, env, language))            

/************************ end macros ******************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_DOCUMENTATION_H */
