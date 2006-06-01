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
 
 #ifndef AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_H
 #define AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_H
 
 /**
 *@file axis2_om_children_with_specific_attribute_iterator.h	
 *@brief this is the iterator for om nodes 
 */
  
#include <axis2_om_node.h>
#include <axis2_om_text.h>
#include <axis2_qname.h>
  

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_om_children_with_specific_attribute_iterator_ops 
                    axis2_om_children_with_specific_attribute_iterator_ops_t;

typedef struct axis2_om_children_with_specific_attribute_iterator 
                    axis2_om_children_with_specific_attribute_iterator_t;

/**
 * @defgroup axis2_xml_writer 
 * @ingroup axis2_om_parser
 * @{
 */
 
   /**
    * \brief axis2_om_children_with_specific_attribute_iterator ops
    * Encapsulator struct for ops of axis2_om_iterator
    */
    AXIS2_DECLARE_DATA struct axis2_om_children_with_specific_attribute_iterator_ops
    {
	   /**
		* Free function free the om_children_with_specific_attribute_iterator struct
		*/
        axis2_status_t (AXIS2_CALL *
		free_fn)(axis2_om_children_with_specific_attribute_iterator_t *iterator,
                 const axis2_env_t *env);
                             
        /**
        * Removes from the underlying collection the last element returned by the
        * iterator (optional op).  This method can be called only once per
        * call to <tt>next</tt>.  The behavior of an iterator is unspecified if
        * the underlying collection is modified while the iteration is in
        * progress in any way other than by calling this method.
        */                                              
        axis2_status_t (AXIS2_CALL *
		remove)(axis2_om_children_with_specific_attribute_iterator_t *iterator,
                const axis2_env_t *env);
                             
       /**
        * Returns true< if the iteration has more elements. (In other
        * words, returns true if next would return an axis2_om_node_t struct
        * rather than NULL with error code set in environment
        */                             
        
        axis2_bool_t (AXIS2_CALL *
		has_next)(axis2_om_children_with_specific_attribute_iterator_t *iterator,
                  const axis2_env_t *env);
    
       /**
        * Returns the next element in the iteration. returns null if there is no 
		* more elements in the iteration
        */
        axis2_om_node_t* (AXIS2_CALL *
		next)(axis2_om_children_with_specific_attribute_iterator_t *iterator,
              const axis2_env_t *env);
    
    
    
    };
    
    
struct axis2_om_children_with_specific_attribute_iterator
{
    axis2_om_children_with_specific_attribute_iterator_ops_t *ops;
};    

/** 
 * @param current child
 * @param env environment 
 * return axis2_om_children_with_specific_attribute_iterator_t 
 */

AXIS2_EXTERN  axis2_om_children_with_specific_attribute_iterator_t * AXIS2_CALL
axis2_om_children_with_specific_attribute_iterator_create(
                                  const axis2_env_t *env, 
                                  axis2_om_node_t *current_child,
                                  axis2_qname_t *attr_qname,
                                  axis2_char_t *attr_value,
                                  axis2_bool_t detach);
                                  
/************ Macros *********************************************/

#define AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_FREE(iterator, env) \
        ((iterator)->ops->free_fn(iterator, env))

#define AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_REMOVE(iterator, env) \
        ((iterator)->ops->remove(iterator, env))
        
#define AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_HAS_NEXT(iterator, env) \
        ((iterator)->ops->has_next(iterator, env))
        
#define AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_NEXT(iterator, env) \
        ((iterator)->ops->next(iterator, env))        
        
              
 /** @} */

#ifdef __cplusplus
}
#endif


#endif /* AXIS2_OM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_H */
