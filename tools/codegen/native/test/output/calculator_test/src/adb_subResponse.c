

        /**
        * adb_subResponse.c
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #include "adb_subResponse.h"

        struct adb_subResponse
        {
            int property_addReturn;
            axis2_bool_t is_valid_addReturn;
        };

        adb_subResponse_t* AXIS2_CALL
        adb_subResponse_create(const axutil_env_t *env)
        {
            adb_subResponse_t *_this = NULL;
            _this = (adb_subResponse_t *) AXIS2_MALLOC(env->allocator, sizeof(adb_subResponse_t));
            if(NULL == _this) return NULL;
            _this->property_addReturn = 0;
            _this->is_valid_addReturn = AXIS2_FALSE;
            return _this;
        }

        axis2_status_t AXIS2_CALL
        adb_subResponse_free(adb_subResponse_t* _this, const axutil_env_t *env)
        {
            if(_this) AXIS2_FREE(env->allocator, _this);
            return AXIS2_SUCCESS;
        }

        int AXIS2_CALL
        adb_subResponse_get_addReturn(adb_subResponse_t* _this, const axutil_env_t *env)
        {
            return _this ? _this->property_addReturn : 0;
        }

        axis2_status_t AXIS2_CALL
        adb_subResponse_set_addReturn(adb_subResponse_t* _this, const axutil_env_t *env, const int arg_addReturn)
        {
            if(_this) {
                _this->property_addReturn = arg_addReturn;
                _this->is_valid_addReturn = AXIS2_TRUE;
            }
            return AXIS2_SUCCESS;
        }

