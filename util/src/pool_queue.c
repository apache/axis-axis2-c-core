#include <axutil_pool_queue.h>
#include <stdio.h>
#define MAX_QUEUE_MEMORY_SIZE 65536


AXIS2_EXTERN struct axutil_pool_queue_head * AXIS2_CALL
axutil_pool_queue_make_queue(
        int initial_cap)
{
	axutil_pool_queue_head_t * pool_queue = 
        (axutil_pool_queue_head_t *) malloc (sizeof(axutil_pool_queue_head_t));
	int max_cap = MAX_QUEUE_MEMORY_SIZE / (sizeof(axutil_pool_queue_node_t)),i;
	axutil_pool_queue_node_t * temp;
	if(pool_queue == NULL)
	{
		perror("Out of memory on malloc\n");
		exit(2);
	}
	if(initial_cap > max_cap)
	{
		initial_cap = max_cap;
	}
	if(initial_cap == 0)
	{
		perror("Attempting to create a queue that holds no work orders\n");
		exit(2);
	}
	pool_queue->capacity =initial_cap;
	pool_queue->max_capacity = max_cap;
	pool_queue->head = NULL;
	pool_queue->tail = NULL;
	pool_queue->freeHead = (axutil_pool_queue_node_t *) malloc (sizeof(axutil_pool_queue_node_t));
	if(pool_queue->freeHead == NULL)
	{
		perror("Out of memory on malloc\n");
		exit(2);
	}
	pool_queue->freeTail = pool_queue->freeHead;

	/* populate the free queue */
	for(i = 1;i<=initial_cap;i++)
	{
		temp = (axutil_pool_queue_node_t *) malloc (sizeof(axutil_pool_queue_node_t));
		if(temp == NULL)
		{
			perror("Out of memory on malloc\n");
			exit(2);
		}
		temp->next = pool_queue->freeHead;
		temp->prev = NULL;
		pool_queue->freeHead->prev = temp;
		pool_queue->freeHead=temp;
	}

	return pool_queue;
}


AXIS2_EXTERN void AXIS2_CALL
axutil_pool_queue_add_work_order(
        axutil_pool_queue_head_t *pool_queue, 
        axutil_thread_start_t func1, 
        void *arg1, 
        axutil_thread_start_t func2, 
        void *arg2)
{
	axutil_pool_queue_node_t * temp;
		
	if(pool_queue->freeTail == NULL)
	{
	    temp = (axutil_pool_queue_node_t *) malloc (sizeof(axutil_pool_queue_node_t));
		if(temp == NULL)
		{
			perror("Out of memory on malloc\n");
			exit(2);
		}
		temp->next = NULL;
		temp->prev = NULL;
		pool_queue->freeHead = temp;
		pool_queue->freeTail = temp;
		pool_queue->capacity++;
	}
	
	temp = pool_queue->freeTail;
	if(pool_queue->freeTail->prev == NULL)
	{
		pool_queue->freeTail = NULL;
		pool_queue->freeHead = NULL;
	}
	else
	{
		pool_queue->freeTail->prev->next= NULL;
		pool_queue->freeTail = pool_queue->freeTail->prev;
		pool_queue->freeTail->next=NULL;
	}
	
	temp->func_to_dispatch = func1;
	temp->func_arg = arg1;
	temp->cleanup_func = func2;
	temp->cleanup_arg = arg2;

	temp->prev=NULL;
	if(pool_queue->head == NULL)
	{
		pool_queue->tail = temp;
		pool_queue->head = temp;
	}
	else
	{
		temp->next=pool_queue->head;
		pool_queue->head->prev = temp;
		pool_queue->head=temp;
	}
}


AXIS2_EXTERN void AXIS2_CALL
axutil_pool_queue_get_work_order(
        axutil_pool_queue_head_t *pool_queue, 
        axutil_thread_start_t *func1, 
        void **arg1, 
        axutil_thread_start_t *func2, 
        void **arg2)
{
	axutil_pool_queue_node_t * temp;
		
	temp = pool_queue->tail;
	if(temp == NULL)
	{
		perror("Attempting to axutil_pool_queue_get_work_order from an empty queue.\n");
		exit(2);
	}

	if(pool_queue->tail->prev == NULL)
	{
		pool_queue->tail = NULL;
		pool_queue->head = NULL;
	}
	else
	{
		pool_queue->tail->prev->next = NULL;
		pool_queue->tail = pool_queue->tail->prev;
		pool_queue->tail->next=NULL;
	}
	
	*func1 = temp->func_to_dispatch;
	*arg1  = temp->func_arg;
	*func2 = temp->cleanup_func;
	*arg2  = temp->cleanup_arg;

	temp->prev=NULL;
	if(pool_queue->freeHead == NULL)
	{
		pool_queue->freeTail = temp;
		pool_queue->freeHead = temp;
		temp->prev=NULL;
		
	}
	else
	{
		temp->next=pool_queue->freeHead;
		pool_queue->freeHead->prev = temp;
		pool_queue->freeHead=temp;
	}
}

AXIS2_EXTERN int AXIS2_CALL
axutil_pool_queue_can_accept_work(
        struct axutil_pool_queue_head *pool_queue)
{
	return(pool_queue->freeTail != NULL
	       || pool_queue->capacity <= pool_queue->max_capacity);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_pool_queue_is_job_available(
        struct axutil_pool_queue_head *pool_queue)
{
  return(pool_queue->tail != NULL);
}





