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
 
 #ifndef AXIOM_CHILDREN_QNAME_ITERATOR_H
 #define AXIOM_CHILDREN_QNAME_ITERATOR_H
 
/**
 *@file axiom_children_qname_iterator.h   
 *@brief this is the iterator for om nodes using qname
 */
 
 #include <axiom_node.h>
 #include <axiom_namespace.h>
 #include <axis2_qname.h>
 
 
 #ifdef __cplusplus
extern "C"
{
#endif

typedef struct axiom_children_qname_iterator_ops axiom_children_qname_iterator_ops_t;
typedef struct axiom_children_qname_iterator axiom_children_qname_iterator_t;

/**
 * @defgroup axiom_children_qname_iterator children qname iterator
 * @ingroup axiom_om
 * @{
 */

   /**
    * \brief axiom_children_iterator operations
    * Encapsulator struct for operations of axiom_iterator
    */

    AXIS2_DECLARE_DATA struct axiom_children_qname_iterator_ops
    {
      /**
       * free om_children_qname_iterator struct 
       */
        axis2_status_t (AXIS2_CALL *
      free_fn)(axiom_children_qname_iterator_t *iterator,
                 const axis2_env_t *env);
                             
        /**
        * Removes from the underlying collection the last element returned by the
        * iterator (optional operation).  This method can be called only once per
        * call to <tt>next</tt>.  The behavior of an iterator is unspecified if
        * the underlying collection is modified while the iteration is in
        * progress in any way other than by calling this method.
        */                                              
        axis2_status_t (AXIS2_CALL *
      remove)(axiom_children_qname_iterator_t *iterator,
                const axis2_env_t *env);
                             
       /**
        * Returns <tt>true</tt> if the iteration has more elements. (In other
        * words, returns <tt>true</tt> if <tt>next</tt> would return an 
      * axiom_node_t struct rather than null with error code set in environment
        */                             
        
        axis2_bool_t (AXIS2_CALL *
      has_next)(axiom_children_qname_iterator_t *iterator,
                  const axis2_env_t *env);
    
       /**
        * Returns the next element in the iteration.
        */
        axiom_node_t* (AXIS2_CALL *
      next)(axiom_children_qname_iterator_t *iterator,
              const axis2_env_t *env);
                             
                        
        
    };

struct axiom_children_qname_iterator
{
    axiom_children_qname_iterator_ops_t *ops;
};

AXIS2_EXTERN axiom_children_qname_iterator_t * AXIS2_CALL
axiom_children_qname_iterator_create(const axis2_env_t *env,
                                        axiom_node_t *current_child,
                                        axis2_qname_t *given_qname);
 

#define AXIOM_CHILDREN_QNAME_ITERATOR_FREE(iterator, env) \
        ((iterator)->ops->free_fn(iterator, env))

#define AXIOM_CHILDREN_QNAME_ITERATOR_REMOVE(iterator, env) \
        ((iterator)->ops->remove(iterator, env))

#define AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(iterator, env) \
        ((iterator)->ops->has_next(iterator, env))

#define AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(iterator, env) \
        ((iterator)->ops->next(iterator, env))

/** @} */

#ifdef __cplusplus
}
#endif
 
#endif /* AXIOM_CHILDREN_QNAME_ITERATOR_H */
