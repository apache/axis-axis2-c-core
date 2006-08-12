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

#include "test_conf.h"
#include <stdio.h>

void Testaxis2_conf_set_default_dispatchers(CuTest *tc)
{
    axis2_conf_t *conf = NULL;
    axis2_array_list_t *in_phases = NULL;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_status_t actual = AXIS2_FAILURE;
    int i = 0;
    int size = 0;
    axis2_phase_t *phase = NULL;
    
    printf("***************************************************\n");
    printf("testing axis2_conf_set_default_dispatchers  method \n");
    printf("***************************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    conf = axis2_conf_create(env);
    AXIS2_CONF_SET_DEFAULT_DISPATCHERS(conf, env);
    in_phases = AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env);
    CuAssertPtrNotNull(tc, in_phases);
    size = AXIS2_ARRAY_LIST_SIZE(in_phases, env);
    for(i = 0; i < size; i++)
    {
        axis2_qname_t *qname = NULL;
        axis2_char_t *handler_name = NULL;
        axis2_array_list_t *handlers = NULL;
        axis2_handler_t *handler = NULL;
        int j = 0;
        int sizej = 0;
        
        phase = AXIS2_ARRAY_LIST_GET(in_phases, env, i);
        handlers = AXIS2_PHASE_GET_HANDLERS (phase, env);
        sizej = AXIS2_ARRAY_LIST_SIZE(handlers, env);
        for(j = 0; j < sizej; j++)
        {
            handler = AXIS2_ARRAY_LIST_GET(handlers, env, j);
            qname = AXIS2_HANDLER_GET_QNAME(handler, env);
            handler_name = AXIS2_QNAME_GET_LOCALPART(qname, env);
            if(0 == AXIS2_STRCMP(handler_name, "addressing_based_dispatcher"))
            {
                printf("handler_name:%s\n", handler_name);
                actual = AXIS2_SUCCESS;
                break;
            }
        }
    }

    AXIS2_CONF_FREE(conf, env);
    
    CuAssertIntEquals(tc, expected, actual);
}

