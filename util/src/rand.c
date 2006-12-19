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

#include <stdlib.h>
#include <axis2_rand.h>
#include <axis2_string.h>
#include <platforms/axis2_platform_auto_sense.h>

#define AXIS2_RAND_MAX 32768

AXIS2_EXTERN int AXIS2_CALL
axis2_rand(
    unsigned int *seedp)
{
    *seedp = *seedp * 1103515245 + 12345;
    return((unsigned)(*seedp/(2 * AXIS2_RAND_MAX)) % AXIS2_RAND_MAX);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_rand_with_range(
    unsigned int *seedp,
    int start,
    int end)
{
    int rand = -1;
    float range = 0.0;
    if(start < 0 || end <= 0) return -1;
    if(start >= AXIS2_RAND_MAX || end > AXIS2_RAND_MAX) return -1;
    if(end <= start) return -1;
  
    range = end - start;
    rand = axis2_rand(seedp);
    rand = 1+(int) (range * rand / (AXIS2_RAND_MAX + 1.0));
    return rand;
}

AXIS2_EXTERN unsigned int AXIS2_CALL
axis2_rand_get_seed_value_based_on_time(
    const axis2_env_t *env)
{
    axis2_date_time_t *date = axis2_date_time_create(env);
    unsigned int rand_var = AXIS2_DATE_TIME_GET_YEAR(date, env);
    rand_var += AXIS2_DATE_TIME_GET_MONTH(date, env);
    rand_var += AXIS2_DATE_TIME_GET_DATE(date, env);
    rand_var += AXIS2_DATE_TIME_GET_HOUR(date, env);
    rand_var += AXIS2_DATE_TIME_GET_MINUTE(date, env);
    rand_var += AXIS2_DATE_TIME_GET_SECOND(date, env);
    return rand_var;
}

