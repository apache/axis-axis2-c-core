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

#include <woden/wsdl20/xml/axis2_woden_import_element.h>

axis2_status_t AXIS2_CALL
axis2_woden_import_element_resolve_methods(
        axis2_woden_import_element_t *import_element,
        axis2_env_t **env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    import_element->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    import_element->ops->to_import_element_free = axis2_hash_get(methods, 
            "to_import_element_free", AXIS2_HASH_KEY_STRING);
    import_element->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    import_element->ops->set_namespace = axis2_hash_get(methods,
            "set_namespace", AXIS2_HASH_KEY_STRING);
    import_element->ops->get_namespace = axis2_hash_get(methods,
            "get_namespace", AXIS2_HASH_KEY_STRING);
    import_element->ops->set_location = axis2_hash_get(methods,
            "set_location", AXIS2_HASH_KEY_STRING);
    import_element->ops->get_location = axis2_hash_get(methods,
            "get_location", AXIS2_HASH_KEY_STRING);
    import_element->ops->set_desc_element = axis2_hash_get(methods,
            "set_desc_element", AXIS2_HASH_KEY_STRING);
    import_element->ops->get_desc_element = axis2_hash_get(methods,
            "get_desc_element", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;    
}

