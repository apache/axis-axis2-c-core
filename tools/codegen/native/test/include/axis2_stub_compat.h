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

#ifndef AXIS2_STUB_COMPAT_H
#define AXIS2_STUB_COMPAT_H

/**
 * @file axis2_stub_compat.h
 * Compatibility header for missing Axis2/C stub functions
 *
 * This header provides stub implementations for functions that don't
 * exist in the current Axis2/C API but are referenced by generated code.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Stub implementation for axis2_stub_set_svc which doesn't exist in current Axis2/C
 * @param stub pointer to stub struct (void* for simplicity)
 * @param env pointer to environment struct (const void* to match generated code)
 * @param svc pointer to service struct (void* for simplicity)
 * @return 0 for success (AXIS2_SUCCESS equivalent)
 */
int axis2_stub_set_svc(void *stub, const void *env, void *svc);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_STUB_COMPAT_H */