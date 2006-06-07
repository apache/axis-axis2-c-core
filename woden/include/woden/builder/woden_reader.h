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

#ifndef WODEN_READER_H
#define WODEN_READER_H

/**
 * @file woden_reader.h
 * @brief Woden Wsdl Reader Interface
 * This interface declares the WSDL reader API for parsing WSDL documents.
 * <p>
 * TODO after WSDL 2.0 parsing is implemented, consider if/how to make this reader
 * API independent of the WSDL version (definition/description) or whether to make it
 * support both versions.
 * <p>
 * TODO add to the API methods to get/set features and properties of the
 * Woden framework (i.e. as distinct from features/properties of the WSDL 2.0
 * component model). A named feature will be turned on or off with a boolean. 
 * A named property will be set with some object representing the property value.
 * 
 */

#include <woden/axis2_woden.h>

/** @defgroup woden_reader Woden Wsdl Reader
  * @ingroup axis2_wsdl
  * @{
  */

typedef union woden_reader_base woden_reader_base_t;
typedef struct woden_reader woden_reader_t;
typedef struct woden_reader_ops woden_reader_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_reader_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *reader,
            const axis2_env_t *env);
    
};

struct woden_reader
{
    woden_reader_ops_t *ops;
};

AXIS2_EXTERN woden_reader_t * AXIS2_CALL
woden_reader_create(
        const axis2_env_t *env);

#define WODEN_READER_FREE(reader, env) \
      (((woden_reader_t *) reader)->ops->free(reader, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_READER_H */
