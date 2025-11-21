/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "axis2_stub_compat.h"

int axis2_stub_set_svc(void *stub, const void *env, void *svc)
{
    /*
     * This function doesn't exist in the current Axis2/C API.
     *
     * In older versions of Axis2, this function might have been used to
     * associate a service with a stub, but in the current API this
     * association is handled differently or may not be needed.
     *
     * For now, we provide a no-op implementation to allow generated
     * code to compile successfully.
     */

    /* Suppress unused parameter warnings */
    (void)stub;
    (void)env;
    (void)svc;

    /* No operation needed - service association handled elsewhere */
    return 0; /* AXIS2_SUCCESS equivalent */
}