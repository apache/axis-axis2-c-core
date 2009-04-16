
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

#include <axutil_types.h>

AXIS2_EXTERN int AXIS2_CALL
axutil_atoi(
    const char *s)
{
    int i,
     n;

    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0');
    }
    return n;
}

AXIS2_EXTERN int64_t AXIS2_CALL
axutil_atol(
    const char *s)
{
    int i;
    int64_t n;

    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0');
    }
    return n;
}

AXIS2_EXTERN uint64_t AXIS2_CALL
axutil_strtoul(
    const char *s,
    char **endptr,
    int base)
{
    int i;
    uint64_t n;

    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0');
    }
    if(endptr != NULL)
    {
        *endptr = (char *)(s + i);
    }
    return n;
}

AXIS2_EXTERN int64_t AXIS2_CALL
axutil_strtol(
    const char *s,
    char **endptr,
    int base)
{
    int i;
    int64_t n;

    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0');
    }
    if(endptr != NULL)
    {
        *endptr = (char *)(s + i);
    }
    return n;
}
