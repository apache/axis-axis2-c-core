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
 
 #ifndef AXIS2_WSDL4C_QNAME_H
 #define AXIS2_WSDL4C_QNAME_H
 
 /**
 * @file axis2_wsdl4c_qname.h
 * @brief defines WSDL4C_QNAME
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
 * @defgroup axis2_wsdl4c_qname wsdl4c_qname
 * @ingroup axis2_wsdl4c
 * @{
 */
    void *
    axis2_wsdl4c_qname_create_with_name(char* name);
    
    void *
    axis2_wsdl4c_qname_create_with_qname(void* qn);
    
    void *
    axis2_wsdl4c_qname_create();

    void
    axis2_wsdl4c_qname_destroy(void *qname);

    /**
     * @name axis2_wsdl4c_get_localname
     * @return
     */
    char *
    axis2_wsdl4c_qname_get_localname(void *qname);
    
    char *
    axis2_wsdl4c_qname_get_prefix(void *qname);
    
    char *
    axis2_wsdl4c_qname_get_namespace(void *qname);

    void
    axis2_wsdl4c_qname_set_namespace(void *qname, 
                                    char *uri);

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_QNAME_H */
