 

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
 * for "add|" operation.
 
 * @param add
 */ 
    adb_addResponse_t * axis2_skel_Calculator_add(const axutil_env_t * env,
                                                    adb_add_t * add) 
{
    adb_addResponse_t * add_res = NULL;
    int ret_val = 0;
    int val1 = 0;
    int val2 = 0;
    val1 = adb_add_get_arg_0_0(add, env);
    val2 = adb_add_get_arg_1_0(add, env);
    ret_val = val1 + val2;
    add_res = adb_addResponse_create(env);
    adb_addResponse_set_addReturn(add_res, env, ret_val);
    return add_res;
}



/**
 * auto generated function definition signature
 * for "div|" operation.
 
 * @param div
 */ 
    adb_divResponse_t * axis2_skel_Calculator_div(const axutil_env_t * env,
                                                    adb_div_t * div) 
{
    adb_divResponse_t * div_res = NULL;
    int ret_val = 0;
    int val1 = 0;
    int val2 = 0;
    val1 = adb_div_get_arg_0_3(div, env);
    val2 = adb_div_get_arg_1_3(div, env);
    ret_val = val1 / val2;
    div_res = adb_divResponse_create(env);
    adb_divResponse_set_divReturn(div_res, env, ret_val);
    return div_res;
}



/**
 * auto generated function definition signature
 * for "sub|" operation.
 
 * @param sub
 */ 
    adb_subResponse_t * axis2_skel_Calculator_sub(const axutil_env_t * env,
                                                    adb_sub_t * sub) 
{
    adb_subResponse_t * sub_res = NULL;
    int ret_val = 0;
    int val1 = 0;
    int val2 = 0;
    val1 = adb_sub_get_arg_0_1(sub, env);
    val2 = adb_sub_get_arg_1_1(sub, env);
    ret_val = val1 - val2;
    sub_res = adb_subResponse_create(env);
    adb_subResponse_set_subReturn(sub_res, env, ret_val);
    return sub_res;
}



/**
 * auto generated function definition signature
 * for "mul|" operation.
 
 * @param mul
 */ 
    adb_mulResponse_t * axis2_skel_Calculator_mul(const axutil_env_t * env,
                                                    adb_mul_t * mul) 
{
    adb_mulResponse_t * mul_res = NULL;
    int ret_val = 0;
    int val1 = 0;
    int val2 = 0;
    val1 = adb_mul_get_arg_0_2(mul, env);
    val2 = adb_mul_get_arg_1_2(mul, env);
    ret_val = val1 * val2;
    mul_res = adb_mulResponse_create(env);
    adb_mulResponse_set_mulReturn(mul_res, env, ret_val);
    return mul_res;
}


