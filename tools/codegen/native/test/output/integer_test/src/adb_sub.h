

        /**
        * adb_sub.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #ifndef ADB_SUB_H
        #define ADB_SUB_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        typedef struct adb_sub adb_sub_t;

        /**
         * Constructor for sub
         */
        adb_sub_t* AXIS2_CALL
        adb_sub_create(const axutil_env_t *env);

        /**
         * Free sub
         */
        axis2_status_t AXIS2_CALL
        adb_sub_free(adb_sub_t* _this, const axutil_env_t *env);

        /**
         * Getter for in0
         */
        int AXIS2_CALL
        adb_sub_get_in0(adb_sub_t* _this, const axutil_env_t *env);

        /**
         * Setter for in0
         */
        axis2_status_t AXIS2_CALL
        adb_sub_set_in0(adb_sub_t* _this, const axutil_env_t *env, const int arg_in0);

        /**
         * Getter for in1
         */
        int AXIS2_CALL
        adb_sub_get_in1(adb_sub_t* _this, const axutil_env_t *env);

        /**
         * Setter for in1
         */
        axis2_status_t AXIS2_CALL
        adb_sub_set_in1(adb_sub_t* _this, const axutil_env_t *env, const int arg_in1);

        /**
         * Serialize to axiom_node
         */
        axiom_node_t* AXIS2_CALL
        adb_sub_serialize(adb_sub_t* _this, const axutil_env_t *env,
                        axiom_node_t* parent, axiom_element_t *parent_element,
                        int parent_tag_closed, axutil_hash_t* namespaces,
                        int *next_ns_index);

        #ifdef __cplusplus
        }
        #endif

        #endif /* ADB_SUB_H */
