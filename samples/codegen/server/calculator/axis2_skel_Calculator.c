

    /**
     * axis2_skel_Calculator.c
     *
     * This file was auto-generated from WSDL for "Calculator|http://localhost/axis/Calculator" service
     * by the Apache Axis2/C version: #axisVersion# #today#
     * axis2_skel_Calculator Axis2/C skeleton for the axisService
     */

     #include "axis2_skel_Calculator.h"

     

		 
        /**
         * auto generated function definition signature
         * for "div|http://localhost/axis/Calculator" operation.
         
         * @param div
         */
        
        adb_divResponse_t* axis2_skel_Calculator_div (const axutil_env_t *env,
                                                          adb_div_t *_div1)
        {
            
            
            adb_divResponse_t *_divresponse2;
            int int_2;
            
            int int_0;
            int int_1;
            
            /* Extract the request */
            
            int_0 = adb_div_get_in0(_div1, env);
            printf("Returned int: %d\n", int_0);
            int_1 = adb_div_get_in1(_div1, env);
            printf("Returned int: %d\n", int_1);
        
            /* Build the response adb */
            
            _divresponse2 = adb_divResponse_create(env);
            int_2 = int_0 / int_1;
            adb_divResponse_set_divReturn(_divresponse2, env, int_2);
            
            return _divresponse2;
        }
     

		 
        /**
         * auto generated function definition signature
         * for "mul|http://localhost/axis/Calculator" operation.
         
         * @param mul
         */
        
        adb_mulResponse_t* axis2_skel_Calculator_mul (const axutil_env_t *env,
                                                          adb_mul_t *_mul1)
        {
            
            
            adb_mulResponse_t *_mulresponse2;
            int int_5;
            
            int int_3;
            int int_4;
            
            /* Extract the request */
            
            int_3 = adb_mul_get_in0(_mul1, env);
            printf("Returned int: %d\n", int_3);
            int_4 = adb_mul_get_in1(_mul1, env);
            printf("Returned int: %d\n", int_4);
        
            /* Build the response adb */
            
            _mulresponse2 = adb_mulResponse_create(env);
            int_5 = int_3 * int_4;
            adb_mulResponse_set_mulReturn(_mulresponse2, env, int_5);
            
            return _mulresponse2;
        }
     

		 
        /**
         * auto generated function definition signature
         * for "add|http://localhost/axis/Calculator" operation.
         
         * @param add
         */
        
        adb_addResponse_t* axis2_skel_Calculator_add (const axutil_env_t *env,
                                                          adb_add_t *_add1)
        {
            
            
            adb_addResponse_t *_addresponse2;
            int int_8;
            
            int int_6;
            int int_7;
            
            /* Extract the request */
            
            int_6 = adb_add_get_in0(_add1, env);
            printf("Returned int: %d\n", int_6);
            int_7 = adb_add_get_in1(_add1, env);
            printf("Returned int: %d\n", int_7);
        
            /* Build the response adb */
            
            _addresponse2 = adb_addResponse_create(env);
            int_8 = int_6 + int_7;
            adb_addResponse_set_addReturn(_addresponse2, env, int_8);
            
            return _addresponse2;
        }
     

		 
        /**
         * auto generated function definition signature
         * for "sub|http://localhost/axis/Calculator" operation.
         
         * @param sub
         */
        
        adb_subResponse_t* axis2_skel_Calculator_sub (const axutil_env_t *env,
                                                          adb_sub_t *_sub1)
        {
            
            
            adb_subResponse_t *_subresponse2;
            int int_11;
            
            int int_9;
            int int_10;
            
            /* Extract the request */
            
            int_9 = adb_sub_get_in0(_sub1, env);
            printf("Returned int: %d\n", int_9);
            int_10 = adb_sub_get_in1(_sub1, env);
            printf("Returned int: %d\n", int_10);
        
            /* Build the response adb */
            
            _subresponse2 = adb_subResponse_create(env);
            int_11 = int_9 - int_10;
            adb_subResponse_set_subReturn(_subresponse2, env, int_11);
            
            return _subresponse2;
        }
     

