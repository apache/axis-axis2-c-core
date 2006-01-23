#include "test_conf.h"

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
    axis2_env_t *env = axis2_env_create (allocator);

    conf = axis2_conf_create(&env);
    AXIS2_CONF_SET_DEFAULT_DISPATCHERS(conf, &env);
    in_phases = AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, &env);
    CuAssertPtrNotNull(tc, in_phases);
    size = AXIS2_ARRAY_LIST_SIZE(in_phases, &env);
    for(i = 0; i < size; i++)
    {
        axis2_qname_t *qname = NULL;
        axis2_char_t *handler_name = NULL;
        axis2_array_list_t *handlers = NULL;
        axis2_handler_t *handler = NULL;
        int j = 0;
        int sizej = 0;
        
        phase = AXIS2_ARRAY_LIST_GET(in_phases, &env, i);
        handlers = axis2_phase_get_handlers(phase, &env);
        sizej = AXIS2_ARRAY_LIST_SIZE(handlers, &env);
        for(j = 0; j < sizej; j++)
        {
            handler = AXIS2_ARRAY_LIST_GET(handlers, &env, j);
            qname = AXIS2_HANDLER_GET_NAME(handler, &env);
            handler_name = AXIS2_QNAME_GET_LOCALPART(qname, &env);
            if(0 == AXIS2_STRCMP(handler_name, "addressing_based_dispatcher"))
            {
                printf("handler_name:%s\n", handler_name);
                actual = AXIS2_SUCCESS;
                break;
            }
        }
    }

    AXIS2_CONF_FREE(conf, &env);
    
    CuAssertIntEquals(tc, expected, actual);
}

