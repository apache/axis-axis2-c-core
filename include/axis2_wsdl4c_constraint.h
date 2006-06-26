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
 
 #ifndef AXIS2_WSDL4C_CONSTRAINT_H
 #define AXIS2_WSDL4C_CONSTRAINT_H
 
 /**
 * @file axis2_wsdl4c_constraint.h
 * @brief defines WSDL4C_CONSTRAINT
 */
#include <axis2_array_list.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_wsdl4c axis2_wsdl4c_constraint
 * @ingroup axis2_wsdl4c
 * @{
 */
    enum axis2_wsdl4c_constraints
    {
        AXIS2_WSDL4C_KEY,
        AXIS2_WSLD4C_KEYREF,
        AXIS2_WSDL4C_UNIQUE
    };
   
    typedef enum axis2_wsdl4c_constraints axis2_wsdl4c_constraints_t;

    void *
    axis2_wsdl4c_constraint_create(axis2_wsdl4c_constraints_t constraints);

    char *
    axis2_wsdl4c_constraint_get_name(void *constraint);

    void
    axis2_wsdl4c_constraint_set_name(void *constraint, 
                                        char *name);

    /** A
     * xis2_wsdl4c_constraints_t *
     * axis2_wsdl4c_constraint_get_constraint_type(void *constraint);
     */

    void
    axis2_wsdl4c_constraint_set_selector(void *constraint, 
                                            char *xpath); 

    void
    axis2_wsdl4c_constraint_add_field(void *constraint, 
                                        char *xpath);

    char *
    axis2_wsdl4c_constraint_selector(void *constraint); 


    axis2_array_list_t *
    axis2_wsdl4c_constraint_fields(void *constraint);


    void
    axis2_wsdl4c_constraint_set_annotation(void *constraint, 
                                            char *s);
 
/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_CONSTRAINT_H */
