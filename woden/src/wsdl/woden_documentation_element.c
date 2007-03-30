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

#include <woden_documentation_element.h>

axis2_status_t AXIS2_CALL
woden_documentation_element_resolve_methods(
    woden_documentation_element_t *documentation_element,
    const axutil_env_t *env,
    axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    documentation_element->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    documentation_element->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);
    documentation_element->ops->set_content =
        axis2_hash_get(methods, "set_content",
                AXIS2_HASH_KEY_STRING);
    documentation_element->ops->get_content =
        axis2_hash_get(methods, "get_content",
                AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

