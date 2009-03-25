#ifndef AXUTIL_POOL_QUEUE_H
#define AXUTIL_POOL_QUEUE_H
#include <stdlib.h>
#include <axutil_thread_pool.h>
#define MAX_QUEUE_MEMORY_SIZE 65536

/**
 * This code maintains jobs using two queues namely job queue and free queue. When a job is assigned 
 * to the thread pool it get a free node from the tail of unassigned queue(free queue) and assign 
 * the job to it and push it as the head of the job queue. A thread in the pool pop this job node 
 * from the head of the job queue and do work. Then it push this free node into the head of the free 
 * queue.
 */

/**
 * Denote the work queue */
typedef struct axutil_pool_queue_node
{
  axutil_thread_start_t func_to_dispatch;
  void * func_arg;
  axutil_thread_start_t cleanup_func;
  void * cleanup_arg;
  struct axutil_pool_queue_node * next;
  struct axutil_pool_queue_node * prev;
} axutil_pool_queue_node_t;

/**
 * Denote job in the queue */
typedef struct axutil_pool_queue_head
{
	struct axutil_pool_queue_node * head;
	struct axutil_pool_queue_node * tail;
	struct axutil_pool_queue_node * freeHead;
	struct axutil_pool_queue_node * freeTail;
	int capacity;
	int max_capacity;
} axutil_pool_queue_head_t;


AXIS2_EXTERN struct axutil_pool_queue_head * AXIS2_CALL
axutil_pool_queue_make_queue(
        int initial_cap);

/**
 * Add job to the worker queue. This will effectively push the job into the job queue.
 * If there is a free node in the free queue assign the job to it and push it into the job queue.
 * Otherwise if maximum limit for nodes is not exceeded create a new free node and assign the job
 * to it and push into the job queue.
 * @param queue_head pool object
 * @param func1 job function
 * @param arg1 arguement for job function
 * @param func2 error handling function
 * @param arg2 arguement to error handling function
 * @return void
 */
AXIS2_EXTERN void AXIS2_CALL
axutil_pool_queue_add_work_order(
        axutil_pool_queue_head_t *pool_queue, 
        axutil_thread_start_t func1, 
        void *arg1, 
        axutil_thread_start_t func2, 
        void *arg2);

/**
 * This will pop jobs from the tail of the job queue. Once the job is retrieved it is considered
 * a free node and added into the free queue.
 * @param queue_head pool object
 * @param func1 job function to be assigned from pooled job
 * @param arg1 arguement for job function
 * @param func2 error handling function to be assigned from pooled job
 * @param arg2 arguement to error handling function
 * @return void
 */

AXIS2_EXTERN void AXIS2_CALL
axutil_pool_queue_get_work_order(
        axutil_pool_queue_head_t *pool_queue, 
        axutil_thread_start_t *func1, 
        void ** arg1, 
        axutil_thread_start_t *func2, 
        void **arg2);

/**
 * If free queque has free nodes to accomodate jobs then this function returns true
 */
AXIS2_EXTERN int AXIS2_CALL
axutil_pool_queue_can_accept_work(
        struct axutil_pool_queue_head *pool_queue);

/**
 * If job queue has jobs to be done then this function returns true
 */
AXIS2_EXTERN int AXIS2_CALL
axutil_pool_queue_is_job_available(
        struct axutil_pool_queue_head *pool_queue);

#endif                          /* AXUTIL_POOL_QUEUE_H */
