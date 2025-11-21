

        /**
        * output/axis2c_1373_unique_test/src/axis2_stub_axis2c_1373_exchange_services.c
        *
        * This file was auto-generated from WSDL for "axis2c_1373_exchange_services|http://localhost/axis/axis2c_1373_exchange_services" service
        * by the Apache Axis2/C Native version: 1.0.0  Built on : Nov 21, 2025 (11:49:30 HST)
        */

        #include "axis2_stub_axis2c_1373_exchange_services.h"

        /**
         * Default endpoint URI
         */
        static const axis2_char_t *default_endpoint_uri = "http://localhost/axis/axis2c_1373_exchange_services";

        /**
         * Create and return the stub
         */
        axis2_stub_t* AXIS2_CALL
        axis2_stub_create_axis2c_1373_exchange_services(const axutil_env_t *env,
                                        const axis2_char_t *client_home,
                                        const axis2_char_t *endpoint_uri)
        {
            axis2_stub_t *stub = NULL;
            axis2_endpoint_ref_t *endpoint_ref = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);
            
            if (NULL == endpoint_uri) {
                endpoint_uri = default_endpoint_uri;
            }
            
            endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);
            stub = axis2_stub_create_with_endpoint_ref_and_client_home(env, endpoint_ref, client_home);
            
            if (NULL == stub) {
                if (NULL != endpoint_ref) {
                    axis2_endpoint_ref_free(endpoint_ref, env);
                }
                return NULL;
            }
            
            axis2_stub_populate_services_for_axis2c_1373_exchange_services(stub, env);
            return stub;
        }

        /**
         * Populate the services for axis2c_1373_exchange_services
         */
        void AXIS2_CALL
        axis2_stub_populate_services_for_axis2c_1373_exchange_services(axis2_stub_t *stub, const axutil_env_t *env)
        {
            axis2_svc_t *svc = NULL;
            axutil_qname_t *op_qname = NULL;
            axis2_op_t *op = NULL;
            
            /* Create service */
            svc = axis2_svc_create_with_qname(env, axutil_qname_create(env, "axis2c_1373_exchange_services", "http://localhost/axis/axis2c_1373_exchange_services", NULL));
            
            /* Add add operation */
            op_qname = axutil_qname_create(env, "add", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            op = axis2_op_create_with_qname(env, op_qname);
            axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_IN_OUT);
            axis2_svc_add_op(svc, env, op);
            
            /* Add sub operation */
            op_qname = axutil_qname_create(env, "sub", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            op = axis2_op_create_with_qname(env, op_qname);
            axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_IN_OUT);
            axis2_svc_add_op(svc, env, op);
            
            /* Add mul operation */
            op_qname = axutil_qname_create(env, "mul", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            op = axis2_op_create_with_qname(env, op_qname);
            axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_IN_OUT);
            axis2_svc_add_op(svc, env, op);
            
            /* Add div operation */
            op_qname = axutil_qname_create(env, "div", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            op = axis2_op_create_with_qname(env, op_qname);
            axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_IN_OUT);
            axis2_svc_add_op(svc, env, op);
            
            /* Set service in stub */
            axis2_stub_set_svc(stub, env, svc);
        }

        /**
         * Get the default endpoint URI
         */
        axis2_char_t* AXIS2_CALL
        axis2_stub_get_endpoint_uri_of_axis2c_1373_exchange_services(const axutil_env_t *env)
        {
            return (axis2_char_t*)default_endpoint_uri;
        }

        /**
         * Auto generated function implementation for "add" operation.
         */
        adb_addResponse_t* AXIS2_CALL
        axis2_stub_add(axis2_stub_t *stub, const axutil_env_t *env, adb_add_t* _add)
        {
            axis2_svc_client_t *svc_client = NULL;
            axutil_qname_t *op_qname = NULL;
            axiom_node_t *payload = NULL;
            axiom_node_t *ret_node = NULL;
            adb_addResponse_t *ret_val = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);
            AXIS2_PARAM_CHECK(env->error, stub, NULL);
            AXIS2_PARAM_CHECK(env->error, _add, NULL);
            
            svc_client = axis2_stub_get_svc_client(stub, env);
            op_qname = axutil_qname_create(env, "add", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            
            /* TODO: Serialize ADB object to axiom_node */
            payload = adb_add_serialize(_add, env, NULL, NULL, AXIS2_TRUE, NULL, NULL);
            
            if (NULL == payload) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to serialize add request");
                return NULL;
            }
            
            ret_node = axis2_svc_client_send_receive_with_op_qname(svc_client, env, op_qname, payload);
            
            if (NULL == ret_node) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to invoke add operation");
                return NULL;
            }
            
            /* TODO: Deserialize response node to ADB object */
            ret_val = adb_addResponse_create(env);
            /* For now, return a basic response object */
            
            if (op_qname) {
                axutil_qname_free(op_qname, env);
            }
            
            return ret_val;
        }

        /**
         * Auto generated function implementation for "sub" operation.
         */
        adb_subResponse_t* AXIS2_CALL
        axis2_stub_sub(axis2_stub_t *stub, const axutil_env_t *env, adb_sub_t* _sub)
        {
            axis2_svc_client_t *svc_client = NULL;
            axutil_qname_t *op_qname = NULL;
            axiom_node_t *payload = NULL;
            axiom_node_t *ret_node = NULL;
            adb_subResponse_t *ret_val = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);
            AXIS2_PARAM_CHECK(env->error, stub, NULL);
            AXIS2_PARAM_CHECK(env->error, _sub, NULL);
            
            svc_client = axis2_stub_get_svc_client(stub, env);
            op_qname = axutil_qname_create(env, "sub", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            
            /* TODO: Serialize ADB object to axiom_node */
            payload = adb_sub_serialize(_sub, env, NULL, NULL, AXIS2_TRUE, NULL, NULL);
            
            if (NULL == payload) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to serialize sub request");
                return NULL;
            }
            
            ret_node = axis2_svc_client_send_receive_with_op_qname(svc_client, env, op_qname, payload);
            
            if (NULL == ret_node) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to invoke sub operation");
                return NULL;
            }
            
            /* TODO: Deserialize response node to ADB object */
            ret_val = adb_subResponse_create(env);
            /* For now, return a basic response object */
            
            if (op_qname) {
                axutil_qname_free(op_qname, env);
            }
            
            return ret_val;
        }

        /**
         * Auto generated function implementation for "mul" operation.
         */
        adb_mulResponse_t* AXIS2_CALL
        axis2_stub_mul(axis2_stub_t *stub, const axutil_env_t *env, adb_mul_t* _mul)
        {
            axis2_svc_client_t *svc_client = NULL;
            axutil_qname_t *op_qname = NULL;
            axiom_node_t *payload = NULL;
            axiom_node_t *ret_node = NULL;
            adb_mulResponse_t *ret_val = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);
            AXIS2_PARAM_CHECK(env->error, stub, NULL);
            AXIS2_PARAM_CHECK(env->error, _mul, NULL);
            
            svc_client = axis2_stub_get_svc_client(stub, env);
            op_qname = axutil_qname_create(env, "mul", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            
            /* TODO: Serialize ADB object to axiom_node */
            payload = adb_mul_serialize(_mul, env, NULL, NULL, AXIS2_TRUE, NULL, NULL);
            
            if (NULL == payload) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to serialize mul request");
                return NULL;
            }
            
            ret_node = axis2_svc_client_send_receive_with_op_qname(svc_client, env, op_qname, payload);
            
            if (NULL == ret_node) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to invoke mul operation");
                return NULL;
            }
            
            /* TODO: Deserialize response node to ADB object */
            ret_val = adb_mulResponse_create(env);
            /* For now, return a basic response object */
            
            if (op_qname) {
                axutil_qname_free(op_qname, env);
            }
            
            return ret_val;
        }

        /**
         * Auto generated function implementation for "div" operation.
         */
        adb_divResponse_t* AXIS2_CALL
        axis2_stub_div(axis2_stub_t *stub, const axutil_env_t *env, adb_div_t* _div)
        {
            axis2_svc_client_t *svc_client = NULL;
            axutil_qname_t *op_qname = NULL;
            axiom_node_t *payload = NULL;
            axiom_node_t *ret_node = NULL;
            adb_divResponse_t *ret_val = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);
            AXIS2_PARAM_CHECK(env->error, stub, NULL);
            AXIS2_PARAM_CHECK(env->error, _div, NULL);
            
            svc_client = axis2_stub_get_svc_client(stub, env);
            op_qname = axutil_qname_create(env, "div", "http://localhost/axis/axis2c_1373_exchange_services", NULL);
            
            /* TODO: Serialize ADB object to axiom_node */
            payload = adb_div_serialize(_div, env, NULL, NULL, AXIS2_TRUE, NULL, NULL);
            
            if (NULL == payload) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to serialize div request");
                return NULL;
            }
            
            ret_node = axis2_svc_client_send_receive_with_op_qname(svc_client, env, op_qname, payload);
            
            if (NULL == ret_node) {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to invoke div operation");
                return NULL;
            }
            
            /* TODO: Deserialize response node to ADB object */
            ret_val = adb_divResponse_create(env);
            /* For now, return a basic response object */
            
            if (op_qname) {
                axutil_qname_free(op_qname, env);
            }
            
            return ret_val;
        }

