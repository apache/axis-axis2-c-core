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
 
 #ifndef AXIOM_CHILDREN_ITERATOR_H
 #define AXIOM_CHILDREN_ITERATOR_H
 
 /**
 *@file axiom_children_iterator.h   
 *@brief this is the iterator for om nodes 
 */
  
#include <axiom_node.h>
#include <axiom_text.h>
  

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axiom_children_iterator_ops axiom_children_iterator_ops_t;
typedef struct axiom_children_iterator axiom_children_iterator_t;

/**
 * @defgroup axiom_children_iterator children iterator
 * @ingroup axiom_om
 * @{
 */
 
   /**
    * \brief axiom_children_iterator ops
    * Encapsulator struct for ops of axiom_iterator
    */
    AXIS2_DECLARE_DATA struct axiom_children_iterator_ops
    {
      /**
       * Free the om_children_iterator struct
       */
        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_children_iterator_t *iterator,
                 const axis2_env_t *env);
                             
        /**
        * Removes from the underlying collection the last element returned by the
        * iterator (optional op).  This method can be called only once per
        * call to <tt>next</tt>.  The behavior of an iterator is unspecified if
        * the underlying collection is modified while the iteration is in
        * progress in any way other than by calling this method.
        */                                              
        axis2_status_t (AXIS2_CALL *
      remove)(axiom_children_iterator_t *iterator,
                const axis2_env_t *env);
                             
       /**
        * @returns true if the iteration has more elements. In other
        * words, returns true if next() would return an om_node_t struct 
        * rather than null with error code set in environment
        */                             
        
        axis2_bool_t (AXIS2_CALL *
      has_next)(axiom_children_iterator_t *iterator,
                  const axis2_env_t *env);
    
       /**
        * Returns the next element in the iteration. Returns null if there are
      * no more elements in the iteration
        */
        axiom_node_t* (AXIS2_CALL *
      next)(axiom_children_iterator_t *iterator,
              const axis2_env_t *env);
    };
    
    
struct axiom_children_iterator
{
    axiom_children_iterator_ops_t *ops;
};    

/** 
 * @param current child
 * @param env environment 
 * return axiom_children_iterator_t 
 */

AXIS2_EXTERN  axiom_children_iterator_t * AXIS2_CALL
axiom_children_iterator_create(const axis2_env_t *env, 
                                  axiom_node_t *current_child);
                                  
/************ Macros *********************************************/

#define AXIOM_CHILDREN_ITERATOR_FREE(iterator, env) \
        ((iterator)->ops->free_fn(iterator, env))

#define AXIOM_CHILDREN_ITERATOR_REMOVE(iterator, env) \
        ((iterator)->ops->remove(iterator, env))
        
#define AXIOM_CHILDREN_ITERATOR_HAS_NEXT(iterator, env) \
        ((iterator)->ops->has_next(iterator, env))
        
#define AXIOM_CHILDREN_ITERATOR_NEXT(iterator, env) \
        ((iterator)->ops->next(iterator, env))        
              
 /** @} */

#ifdef __cplusplus
}
#endif


#endif /* AXIOM_CHILDREN_ITERATOR_H */
