

        /**
        * adb_addResponse.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #ifndef ADB_ADDRESPONSE_H
        #define ADB_ADDRESPONSE_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        typedef struct adb_addResponse adb_addResponse_t;

        adb_addResponse_t* AXIS2_CALL
        adb_addResponse_create(const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_addResponse_free(adb_addResponse_t* _this, const axutil_env_t *env);

        int AXIS2_CALL
        adb_addResponse_get_addReturn(adb_addResponse_t* _this, const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_addResponse_set_addReturn(adb_addResponse_t* _this, const axutil_env_t *env, const int arg_addReturn);

        #ifdef __cplusplus
        }
        #endif

        #endif /* ADB_ADDRESPONSE_H */
