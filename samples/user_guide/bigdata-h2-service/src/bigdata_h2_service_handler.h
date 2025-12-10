/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 */

#ifndef BIGDATA_H2_SERVICE_HANDLER_H
#define BIGDATA_H2_SERVICE_HANDLER_H

#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Process big data set with pure JSON (called by bridge)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_process_big_data_set_json(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

/**
 * @brief Get service metadata as JSON (called by bridge)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_get_service_metadata_json(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

#ifdef __cplusplus
}
#endif

#endif /* BIGDATA_H2_SERVICE_HANDLER_H */