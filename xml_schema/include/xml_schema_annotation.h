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

#ifndef XML_SCHEMA_ANNOTATION_H
#define XML_SCHEMA_ANNOTATION_H

/** @defgroup xml_schema_annotation Represents W3C annotation element
  * @ingroup xml_schema
  * @{
  */

/**
 * @file xml_schema_annotation.h
 * @brief annotation element representation
 */

#include <xml_schema_defines.h>
#include <xml_schema_obj.h>
#include <xml_schema_obj_collection.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** type name for struct xml_schema_annotation */
    typedef struct xml_schema_annotation xml_schema_annotation_t;
    /** type name for struct xml_schema_annotation_ops */
    typedef struct xml_schema_annotation_ops xml_schema_annotation_ops_t;

    struct xml_schema_annotation_ops
    {
        /**
          * Free xml_schema_annotation struct instance .
          * @param annotation xml_schema_annotation struct instance 
        * @param env environment
        * @returns AXIS2_SUCCESS
          */
        axis2_status_t (AXIS2_CALL *
                free) (
                        void *annotation,
                        const axis2_env_t *env);

        /**
         * returns a hashtable containing pointers to base struct instances
         * @param annotation xml_schema_annotation struct 
         * @param env environment struct 
         * @returns pointer to hashtable if available ,NULL otherwise
         */

        axis2_hash_t *(AXIS2_CALL *
                super_objs) (
                    void *annotation,
                    const axis2_env_t *env);

        /**
         * get the type of the struct instance
         * @param annotation xml_schema_annotation struct instance
         * @param env pointer to environment struct
         * @returns xml_schema_types_t 
         */
        xml_schema_types_t (AXIS2_CALL *
                get_type) (
                    void *annotation,
                    const axis2_env_t *env);
        /**
         * get base type ( xml_schema_obj ) 
         * @param annotation xml_schema_annotation
         * @param env environment 
         * @returns pointer to xml_schema_obj
         */
        xml_schema_obj_t *(AXIS2_CALL *
                get_base_impl) (
                    void *annotation,
                    const axis2_env_t *env);

        /**
         * get the collection of items 
         * @param annotation xml_schema_annotation struct instance
         * @param env environment
         * @returns pointer to xml_schema_obj_collection 
         */
        xml_schema_obj_collection_t* (AXIS2_CALL *
                get_items)(
                        void *annotation,
                        const axis2_env_t *env);

    };

    /**
     * xml_schema_annotation struct instance
     */
    struct xml_schema_annotation
    {
        xml_schema_obj_t base;
        xml_schema_annotation_ops_t *ops;
    };

    /**
     * create an xml_schema_annotation struct instance
     * @param env environment
     * @returns pointer to newly created xml_schema_annotation struct
     */
    AXIS2_EXTERN xml_schema_annotation_t * AXIS2_CALL
    xml_schema_annotation_create(const axis2_env_t *env);

    /** Free an xml_schema_annotation struct
        @sa xml_schema_annotation_ops#free */
#define XML_SCHEMA_ANNOTATION_FREE(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            free(annotation, env))

    /** get super structs hashtable
        @sa xml_schema_annotation_ops#super_objs */
#define XML_SCHEMA_ANNOTATION_SUPER_OBJS(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            super_objs(annotation, env))

    /** get type
       @sa xml_schema_annotation_ops#get_type */
#define XML_SCHEMA_ANNOTATION_GET_TYPE(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            get_type(annotation, env))

    /** get items
       @sa xml_schema_annotation_ops#get_items */
#define XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            get_items(annotation, env))

    /** get_base_impl struct
       @sa xml_schema_annotation_ops#get_base_impl */
#define XML_SCHEMA_ANNOTATION_GET_BASE_IMPL(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            get_base_impl(annotation, env))

    /** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_ANNOTATION_H */
