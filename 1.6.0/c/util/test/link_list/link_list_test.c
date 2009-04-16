#include <axutil_linked_list.h>
#include "../util/create_env.h"

axutil_env_t *env = NULL;
axutil_linked_list_t * linked_list = NULL;
entry_t *entry = NULL;

axis2_status_t test_link_list(axutil_env_t *env,char * first_item,char * second_item,char * third_item,char *last_item,char *array)
{ 
    linked_list = axutil_linked_list_create(env);
    if(linked_list)
    {
    printf("link list is created \n");
    }
    if(!linked_list)
    {
    printf("link list is not created ");
    }
    axutil_linked_list_add_first(linked_list,env,(void *)first_item);
    axutil_linked_list_contains(linked_list,env,(void *)second_item);
    axutil_linked_list_add(linked_list,env,(void *)third_item);
    axutil_linked_list_add_last(linked_list,env,(void *)last_item);
    int index_of_item = axutil_linked_list_index_of(linked_list,env,third_item);
    printf("The index of item is %d\n",index_of_item);
    int index_of_last_item = axutil_linked_list_last_index_of(linked_list,env,last_item);
    entry_t * entry = axutil_linked_list_get_entry(linked_list,env,0);
    printf("The index of last item is %d\n",index_of_last_item);
    void *get_item = axutil_linked_list_get(linked_list,env,1);
    printf("The get item is %s\n",(char *)get_item);
    axutil_linked_list_set(linked_list,env,1,(void *)array);
    axutil_linked_list_to_array(linked_list,env);
    axutil_linked_list_add_at_index(linked_list,env,1,(void *)second_item);
    axutil_linked_list_remove_at_index(linked_list,env,1);
    axutil_linked_list_check_bounds_inclusive(linked_list,env,1);
    axutil_linked_list_remove_entry(linked_list,env,entry);
    axutil_linked_list_remove_first(linked_list,env);
    axutil_linked_list_remove_last(linked_list,env);
    axutil_linked_list_remove(linked_list,env,(void *)third_item);
    axutil_linked_list_free(linked_list,env);
    if(index_of_item && index_of_last_item &&  get_item)
    {
    printf("The test is SUCCESS\n");
    }
    if(!index_of_item || !index_of_last_item || !get_item)
    {
    printf("The test is FAIL\n");
    }
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_link_list(env,"first entry","secnd entry","third entry","last entry" ,"test");
    if(status == AXIS2_FAILURE)
    {
        printf(" build  failed");
    }
    axutil_env_free(env);
    return 0;
}







