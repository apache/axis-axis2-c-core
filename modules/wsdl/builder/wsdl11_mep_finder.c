/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <axis2_wsdl11_mep_finder.h>
#include <axis2_wsdl4c_operation.h>
#include <axis2_wsdl.h>

axis2_char_t *AXIS2_CALL
axis2_wsdl11_mep_finder_get_mep(void *op,
                        		axis2_env_t **env) 
{
    axis2_wsdl4c_optype_t op_type = axis2_wsdl4c_operation_get_type(op);
    printf("op_type:%d\n", op_type);
    if(op_type ==  AXIS2_WSDL4C_OP_IN_OUT)
    {
        return AXIS2_MEP_URI_OUT_IN;
    }
    if (op_type ==  AXIS2_WSDL4C_OP_IN)
        return AXIS2_MEP_URI_IN_ONLY;

    if (op_type == AXIS2_WSDL4C_OP_OUT)
        return AXIS2_MEP_URI_OUT_ONLY;

    if (op_type == AXIS2_WSDL4C_OP_OUT_IN)
        return AXIS2_MEP_URI_IN_OUT;

    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MEP_CANNOT_DETERMINE_MEP,
        AXIS2_FAILURE);
    return NULL;
}
