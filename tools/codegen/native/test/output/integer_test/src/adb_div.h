

        /**
        * adb_div.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #ifndef ADB_DIV_H
        #define ADB_DIV_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        typedef struct adb_div adb_div_t;

        /**
         * Constructor for div
         */
        adb_div_t* AXIS2_CALL
        adb_div_create(const axutil_env_t *env);

        /**
         * Free div
         */
        axis2_status_t AXIS2_CALL
        adb_div_free(adb_div_t* _this, const axutil_env_t *env);

        /**
         * Getter for in0
         */
        int AXIS2_CALL
        adb_div_get_in0(adb_div_t* _this, const axutil_env_t *env);

        /**
         * Setter for in0
         */
        axis2_status_t AXIS2_CALL
        adb_div_set_in0(adb_div_t* _this, const axutil_env_t *env, const int arg_in0);

        /**
         * Getter for in1
         */
        int AXIS2_CALL
        adb_div_get_in1(adb_div_t* _this, const axutil_env_t *env);

        /**
         * Setter for in1
         */
        axis2_status_t AXIS2_CALL
        adb_div_set_in1(adb_div_t* _this, const axutil_env_t *env, const int arg_in1);

        /**
         * Serialize to axiom_node
         */
        axiom_node_t* AXIS2_CALL
        adb_div_serialize(adb_div_t* _this, const axutil_env_t *env,
                        axiom_node_t* parent, axiom_element_t *parent_element,
                        int parent_tag_closed, axutil_hash_t* namespaces,
                        int *next_ns_index);

        #ifdef __cplusplus
        }
        #endif

        #endif /* ADB_DIV_H */
