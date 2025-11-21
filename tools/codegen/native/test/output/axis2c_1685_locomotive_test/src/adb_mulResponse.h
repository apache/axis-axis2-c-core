

        /**
        * adb_mulResponse.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #ifndef ADB_MULRESPONSE_H
        #define ADB_MULRESPONSE_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        typedef struct adb_mulResponse adb_mulResponse_t;

        adb_mulResponse_t* AXIS2_CALL
        adb_mulResponse_create(const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_mulResponse_free(adb_mulResponse_t* _this, const axutil_env_t *env);

        int AXIS2_CALL
        adb_mulResponse_get_addReturn(adb_mulResponse_t* _this, const axutil_env_t *env);

        axis2_status_t AXIS2_CALL
        adb_mulResponse_set_addReturn(adb_mulResponse_t* _this, const axutil_env_t *env, const int arg_addReturn);

        #ifdef __cplusplus
        }
        #endif

        #endif /* ADB_MULRESPONSE_H */
