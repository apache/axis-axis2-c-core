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

#include <axutil_linked_list.h>
#include <cut_defs.h>
#include "../util/create_env.h"

axutil_env_t *env = NULL;
axutil_linked_list_t * linked_list = NULL;
entry_t *entry = NULL;

void test_link_list(axutil_env_t *env,char * first_item,char * second_item,char * third_item,char *last_item,char *array)
{ 
    int index_of_item;
	int index_of_last_item;
	entry_t * entry;
	void *get_item;
	axis2_status_t status;
	axis2_bool_t bresult;
	void **array_from_list;
	
	linked_list = axutil_linked_list_create(env);
    CUT_ASSERT(linked_list != NULL);
    if (!linked_list) return;
    status = axutil_linked_list_add_first(linked_list,env,(void *)first_item);
	CUT_ASSERT(status = AXIS2_SUCCESS);
    bresult = axutil_linked_list_contains(linked_list,env,(void *)second_item);
	CUT_ASSERT(bresult == AXIS2_FALSE);
    status = axutil_linked_list_add(linked_list,env,(void *)third_item);
	CUT_ASSERT(status = AXIS2_SUCCESS);
    status = axutil_linked_list_add_last(linked_list,env,(void *)last_item);
	CUT_ASSERT(status = AXIS2_SUCCESS);
 	CUT_ASSERT(axutil_linked_list_size(linked_list,env) == 3);
    index_of_item = axutil_linked_list_index_of(linked_list,env,third_item);
    CUT_ASSERT(index_of_item == 1);
    index_of_last_item = axutil_linked_list_last_index_of(linked_list,env,last_item);
    CUT_ASSERT(index_of_last_item == 2);
    entry = axutil_linked_list_get_entry(linked_list,env,0);
    CUT_ASSERT(entry != NULL);
    get_item = axutil_linked_list_get(linked_list,env,1);
    CUT_ASSERT(get_item != NULL);
	CUT_ASSERT(strcmp((char*)get_item, third_item) == 0);
    get_item = axutil_linked_list_set(linked_list,env,1,(void *)array);
    CUT_ASSERT(get_item != NULL);
	CUT_ASSERT(strcmp((char*)get_item, third_item) == 0);
    array_from_list = axutil_linked_list_to_array(linked_list,env);
	CUT_ASSERT(array_from_list != NULL);
    status = axutil_linked_list_add_at_index(linked_list,env,1,(void *)second_item);
	CUT_ASSERT(status == AXIS2_SUCCESS);
    get_item = axutil_linked_list_remove_at_index(linked_list,env,1);
	CUT_ASSERT(get_item != NULL);
    bresult = axutil_linked_list_check_bounds_inclusive(linked_list,env,1);
	CUT_ASSERT(bresult == AXIS2_TRUE);
    status = axutil_linked_list_remove_entry(linked_list,env,entry);
 	CUT_ASSERT(status == AXIS2_SUCCESS);
    get_item = axutil_linked_list_remove_first(linked_list,env);
	CUT_ASSERT(get_item != NULL);
    get_item = axutil_linked_list_remove_last(linked_list,env);
	CUT_ASSERT(get_item != NULL);
	CUT_ASSERT(axutil_linked_list_size(linked_list,env) == 0);

    bresult = axutil_linked_list_remove(linked_list,env,(void *)third_item);
 	CUT_ASSERT(bresult == AXIS2_FALSE);
    
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

    axutil_linked_list_free(linked_list,env);
}

int main()
{
    axutil_env_t *env = cut_setup_env("Link list");
	CUT_ASSERT(env != NULL);
	if (env) {
        test_link_list(env,"first entry","secnd entry","third entry","last entry" ,"test");
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}







