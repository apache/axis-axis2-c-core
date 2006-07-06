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

#ifndef WODEN_WSDL10_COMPONENT_MODEL_BUILDER_H
#define WODEN_WSDL10_COMPONENT_MODEL_BUILDER_H

/**
 * @file woden_wsdl10_component_model_builder.h
 * @brief Axis2 Compoment Model Builder Interface
 * 
 */

#include <woden.h>

/** @defgroup woden_wsdl10_component_model_builder Compoment Model Builder
  * @ingroup axis2_wsdl
  * @{
  */

typedef struct woden_wsdl10_component_model_builder woden_wsdl10_component_model_builder_t;
typedef struct woden_wsdl10_component_model_builder_ops woden_wsdl10_component_model_builder_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_wsdl10_component_model_builder_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *component_model_builder,
            const axis2_env_t *env);

};

struct woden_wsdl10_component_model_builder
{
    woden_wsdl10_component_model_builder_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_component_model_builder_t *AXIS2_CALL 
woden_wsdl10_component_model_builder_create(
        const axis2_env_t *env,
        void *desc);


#define WODEN_WSDL10_COMPONENT_MODEL_BUILDER_FREE(component_model_builder, env) \
      (((woden_wsdl10_component_model_builder_t *) component_model_builder)->ops->\
       free(component_model_builder, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_COMPONENT_MODEL_BUILDER_H */
