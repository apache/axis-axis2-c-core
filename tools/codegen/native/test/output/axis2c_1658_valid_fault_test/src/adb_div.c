

        /**
        * adb_div.c
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C Native version: 1.0.0
        */

        #include "adb_div.h"

        struct adb_div
        {
            int property_in0;
            int property_in1;
            axis2_bool_t is_valid_in0;
            axis2_bool_t is_valid_in1;
        };

        adb_div_t* AXIS2_CALL
        adb_div_create(const axutil_env_t *env)
        {
            adb_div_t *_this = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);
            
            _this = (adb_div_t *) AXIS2_MALLOC(env->allocator, sizeof(adb_div_t));
            if(NULL == _this)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return NULL;
            }
            
            _this->property_in0 = 0;
            _this->property_in1 = 0;
            _this->is_valid_in0 = AXIS2_FALSE;
            _this->is_valid_in1 = AXIS2_FALSE;
            
            return _this;
        }

        axis2_status_t AXIS2_CALL
        adb_div_free(adb_div_t* _this, const axutil_env_t *env)
        {
            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
            AXIS2_PARAM_CHECK(env->error, _this, AXIS2_FAILURE);
            
            AXIS2_FREE(env->allocator, _this);
            return AXIS2_SUCCESS;
        }

        int AXIS2_CALL
        adb_div_get_in0(adb_div_t* _this, const axutil_env_t *env)
        {
            AXIS2_ENV_CHECK(env, 0);
            AXIS2_PARAM_CHECK(env->error, _this, 0);
            
            return _this->property_in0;
        }

        axis2_status_t AXIS2_CALL
        adb_div_set_in0(adb_div_t* _this, const axutil_env_t *env, const int arg_in0)
        {
            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
            AXIS2_PARAM_CHECK(env->error, _this, AXIS2_FAILURE);
            
            _this->property_in0 = arg_in0;
            _this->is_valid_in0 = AXIS2_TRUE;
            
            return AXIS2_SUCCESS;
        }

        int AXIS2_CALL
        adb_div_get_in1(adb_div_t* _this, const axutil_env_t *env)
        {
            AXIS2_ENV_CHECK(env, 0);
            AXIS2_PARAM_CHECK(env->error, _this, 0);
            
            return _this->property_in1;
        }

        axis2_status_t AXIS2_CALL
        adb_div_set_in1(adb_div_t* _this, const axutil_env_t *env, const int arg_in1)
        {
            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
            AXIS2_PARAM_CHECK(env->error, _this, AXIS2_FAILURE);
            
            _this->property_in1 = arg_in1;
            _this->is_valid_in1 = AXIS2_TRUE;
            
            return AXIS2_SUCCESS;
        }

        axiom_node_t* AXIS2_CALL
        adb_div_serialize(adb_div_t* _this, const axutil_env_t *env,
                        axiom_node_t* parent, axiom_element_t *parent_element,
                        int parent_tag_closed, axutil_hash_t* namespaces,
                        int *next_ns_index)
        {
            /* TODO: Implement XML serialization */
            return NULL;
        }

