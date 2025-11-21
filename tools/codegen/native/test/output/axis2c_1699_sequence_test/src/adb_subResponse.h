

        /**
        * adb_subResponse.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #ifndef ADB_SUBRESPONSE_H
        #define ADB_SUBRESPONSE_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        typedef struct adb_subResponse adb_subResponse_t;

        adb_subResponse_t* AXIS2_CALL
        adb_subResponse_create(const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_subResponse_free(adb_subResponse_t* _this, const axutil_env_t *env);

        int AXIS2_CALL
        adb_subResponse_get_addReturn(adb_subResponse_t* _this, const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_subResponse_set_addReturn(adb_subResponse_t* _this, const axutil_env_t *env, const int arg_addReturn);

        #ifdef __cplusplus
        }
        #endif

        #endif /* ADB_SUBRESPONSE_H */
