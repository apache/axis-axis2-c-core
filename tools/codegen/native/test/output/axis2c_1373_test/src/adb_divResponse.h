

        /**
        * adb_divResponse.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #ifndef ADB_DIVRESPONSE_H
        #define ADB_DIVRESPONSE_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        typedef struct adb_divResponse adb_divResponse_t;

        adb_divResponse_t* AXIS2_CALL
        adb_divResponse_create(const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_divResponse_free(adb_divResponse_t* _this, const axutil_env_t *env);

        int AXIS2_CALL
        adb_divResponse_get_addReturn(adb_divResponse_t* _this, const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_divResponse_set_addReturn(adb_divResponse_t* _this, const axutil_env_t *env, const int arg_addReturn);

        #ifdef __cplusplus
        }
        #endif

        #endif /* ADB_DIVRESPONSE_H */
