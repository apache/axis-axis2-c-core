/* Copyright 2000-2005 The Apache Software Foundation or its licensors, as
 * applicable.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GUTHTHILA_TABLES_H
#define GUTHTHILA_TABLES_H

/**
 * @file guththila_tables.h
 * @brief GUTHTHILA  array and table 
 */

#include "guththila_defines.h"
#include "guththila_environment.h"
#include "guththila_defines.h"


#include <stdarg.h>     /* for va_list */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup guththila_array (stack)  and table functions
 * @ingroup GUTHTHILA 
 * Tables are used to store entirely opaque structures 
 * for applications, while Arrays are usually used to
 * deal with string lists.
 * @{
 */

/** the table abstract data type */
typedef struct guththila_table_t guththila_table_t;

/** @see guththila_array_header_t */
typedef struct guththila_array_header_t guththila_array_header_t;

/** An opaque array type */
struct guththila_array_header_t {
    /** The environment pointer of guththila */
    guththila_environment_t *environment;
    /** The amount of memory allocated for each element of the array */
    int elt_size;
    /** The number of active elements in the array */
    int nelts;
    /** The number of elements allocated in the array */
    int nalloc;
    /** The elements in the array */
    guththila_char_t *elts;
};

/**
 * The (opaque) structure for string-content tables.
 */
typedef struct guththila_table_entry_t guththila_table_entry_t;

/** The type for each entry in a string-content table */
struct guththila_table_entry_t {
    /** The key for the current table entry */
    guththila_char_t *key;          
   
   /** The value for the current table entry */
    guththila_char_t *val;

    /** A checksum for the key, for use by the guththila_table internals */
    guththila_ssize_t key_checksum;
};

/**
 * Get the elements from a table
 * @param t The table
 * @return An array containing the contents of the table
 */
const guththila_array_header_t * guththila_table_elts(const guththila_table_t *t);

/**
 * Determine if the table is empty
 * @param t The table to check
 * @return True if empty, False otherwise
 */
int guththila_is_empty_table(const guththila_table_t *t);

/**
 * Determine if the array is empty
 * @param a The array to check
 * @return True if empty, False otherwise
 */
int guththila_is_empty_array(const guththila_array_header_t *a);

/**
 * Create an array
 * @param environment guththila_environment pointer
 * @param nelts the number of elements in the initial array
 * @param elt_size The size of each element in the array.
 * @return The new array
 */
guththila_array_header_t * guththila_array_make(guththila_environment_t *environment,
                                                 int nelts, int elt_size);

/**
 * Add a new element to an array (as a first-in, last-out stack)
 * @param arr The array to add an element to.
 * @return Location for the new element in the array.
 * @remark If there are no free spots in the array, then this function will
 *         allocate new space for the new element.
 */
void * guththila_array_push(guththila_array_header_t *arr);

/**
 * Remove an element from an array (as a first-in, last-out stack)
 * @param arr The array to remove an element from.
 * @return Location of the element in the array.
 * @remark If there are no elements in the array, NULL is returned.
 */
void * guththila_array_pop(guththila_array_header_t *arr);

/**
 * Concatenate two arrays together
 * @param dst The destination array, and the one to go first in the combined 
 *            array
 * @param src The source array to add to the destination array
 */
void guththila_array_cat(guththila_array_header_t *dst,
			        const guththila_array_header_t *src);

/**
 * Copy the entire array
 * @param environment guththila_environment
 * @param arr The array to copy
 * @return An exact copy of the array passed in
 * @remark The alternate guththila_array_copy_hdr copies only the header, and arranges 
 *         for the elements to be copied if (and only if) the code subsequently
 *         does a push or arraycat.
 */
guththila_array_header_t * guththila_array_copy(guththila_environment_t *environment,
                                      const guththila_array_header_t *arr);
/**
 * Copy the headers of the array, and arrange for the elements to be copied if
 * and only if the code subsequently does a push or arraycat.
 * @param environment guththila_environment
 * @param arr The array to copy
 * @return An exact copy of the array passed in
 * @remark The alternate guththila_array_copy copies the *entire* array.
 */
guththila_array_header_t * guththila_array_copy_hdr(guththila_environment_t *environment,
                                      const guththila_array_header_t *arr);

/**
 * Append one array to the end of another, creating a new array in the process.
 * @param environment guththila_envioronment
 * @param first The array to put first in the new array.
 * @param second The array to put second in the new array.
 * @return A new array containing the data from the two arrays passed in.
*/
guththila_array_header_t * guththila_array_append(guththila_environment_t *environment,
                                      const guththila_array_header_t *first,
                                      const guththila_array_header_t *second);

/**
 * Generates a new string containing the concatenated 
 * sequence of substrings referenced as elements within the array.  The string 
 * will be empty if all substrings are empty or null, or if there are no 
 * elements in the array.  If sep is non-NUL, it will be inserted between 
 * elements as a separator.
 * @param environment guththila_environment
 * @param arr The array to generate the string from
 * @param sep The separator to use
 * @return A string containing all of the data in the array.
 */
guththila_char_t * guththila_array_pstrcat(guththila_environment_t *p,
				      const guththila_array_header_t *arr,
				      const guththila_char_t sep);

/**
 * Make a new table
 * @param environment guththila_environment
 * @param nelts The number of elements in the initial table.
 * @return The new table.
 * @warning This table can only store text data
 */
guththila_table_t * guththila_table_make(guththila_environment_t *environment, int nelts);

/**
 * Create a new table and copy another table into it
 * @param environment guththila_environment
 * @param t The table to copy
 * @return A copy of the table passed in
 */
guththila_table_t * guththila_table_copy(guththila_environment_t *environment,
                                          const guththila_table_t *t);

/**
 * Delete all of the elements from a table
 * @param t The table to clear
 */
void guththila_table_clear(guththila_table_t *t);

/**
 * Get the value associated with a given key from the table.  After this call,
 * The data is still in the table
 * @param t The table to search for the key
 * @param key The key to search for
 * @return The value associated with the key, or NULL if the key does not exist. 
 */
const guththila_char_t * guththila_table_get(const guththila_table_t *t, const guththila_char_t *key);

/**
 * Add a key/value pair to a table, if another element already exists with the
 * same key, this will over-write the old data.
 * @param t The table to add the data to.
 * @param key The key fo use
 * @param val The value to add
 * @remark When adding data, this function makes a copy of both the key and the
 *         value.
 */
void guththila_table_set(guththila_table_t *t, const guththila_char_t *key,
                                const guththila_char_t *val);

/**
 * Add a key/value pair to a table, if another element already exists with the
 * same key, this will over-write the old data.
 * @param t The table to add the data to.
 * @param key The key to use
 * @param val The value to add
 * @warning When adding data, this function does not make a copy of the key or 
 *          the value, so care should be taken to ensure that the values will 
 *          not change after they have been added..
 */
void guththila_table_setn(guththila_table_t *t, const guththila_char_t *key,
                                 const guththila_char_t *val);

/**
 * Remove data from the table
 * @param t The table to remove data from
 * @param key The key of the data being removed
 */
void guththila_table_unset(guththila_table_t *t, const guththila_char_t *key);

/**
 * Add data to a table by merging the value with data that has already been 
 * stored
 * @param t The table to search for the data
 * @param key The key to merge data for
 * @param val The data to add
 * @remark If the key is not found, then this function acts like guththila_table_add
 */
void guththila_table_merge(guththila_table_t *t, const guththila_char_t *key,
                                  const guththila_char_t *val);

/**
 * Add data to a table by merging the value with data that has already been 
 * stored
 * @param t The table to search for the data
 * @param key The key to merge data for
 * @param val The data to add
 * @remark If the key is not found, then this function acts like guththila_table_addn
 */
void guththila_table_mergen(guththila_table_t *t, const guththila_char_t *key,
                                   const guththila_char_t *val);

/**
 * Add data to a table, regardless of whether there is another element with the
 * same key.
 * @param t The table to add to
 * @param key The key to use
 * @param val The value to add.
 * @remark When adding data, this function makes a copy of both the key and the
 *         value.
 */
void guththila_table_add(guththila_table_t *t, const guththila_char_t *key,
                                const guththila_char_t *val);

/**
 * Add data to a table, regardless of whether there is another element with the
 * same key.
 * @param t The table to add to
 * @param key The key to use
 * @param val The value to add.
 * @remark When adding data, this function does not make a copy of the key or the
 *         value, so care should be taken to ensure that the values will not 
 *         change after they have been added..
 */
void guththila_table_addn(guththila_table_t *t, const guththila_char_t *key,
                                 const guththila_char_t *val);

/**
 * Merge two tables into one new table
 * @param environment guththila_environment
 * @param overlay The first table to put in the new table
 * @param base The table to add at the end of the new table
 * @return A new table containing all of the data from the two passed in
 */
guththila_table_t * guththila_table_overlay(guththila_environment_t *environment,
                                             const guththila_table_t *overlay,
                                             const guththila_table_t *base);

/**
 * Declaration prototype for the iterator callback function of guththila_table_do()
 * and guththila_table_vdo().
 * @param rec The data passed as the first argument to guththila_table_[v]do()
 * @param key The key from this iteration of the table
 * @param value The value from this iteration of the table
 * @remark Iteration continues while this callback function returns non-zero.
 * To export the callback function for guththila_table_[v]do() it must be declared 
 * in the _NONSTD convention.
 */
typedef int (guththila_table_do_callback_fn_t)(void *rec, const guththila_char_t *key, 
                                                    const guththila_char_t *value);

/** 
 * Iterate over a table running the provided function once for every
 * element in the table.  If there is data passed in as a vararg, then the 
 * function is only run on those elements whose key matches something in 
 * the vararg.  If the vararg is NULL, then every element is run through the
 * function.  Iteration continues while the function returns non-zero.
 * @param comp The function to run
 * @param rec The data to pass as the first argument to the function
 * @param t The table to iterate over
 * @param ... The vararg.  If this is NULL, then all elements in the table are
 *            run through the function, otherwise only those whose key matches
 *            are run.
 * @return FALSE if one of the comp() iterations returned zero; TRUE if all
 *            iterations returned non-zero
 * @see guththila_table_do_callback_fn_t
 */
int guththila_table_do(guththila_table_do_callback_fn_t *comp,
                                     void *rec, const guththila_table_t *t, ...);

/** 
 * Iterate over a table running the provided function once for every
 * element in the table.  If there is data passed in as a vararg, then the 
 * function is only run on those element's whose key matches something in 
 * the vararg.  If the vararg is NULL, then every element is run through the
 * function.  Iteration continues while the function returns non-zero.
 * @param comp The function to run
 * @param rec The data to pass as the first argument to the function
 * @param t The table to iterate over
 * @param vp The vararg table.  If this is NULL, then all elements in the 
 *                table are run through the function, otherwise only those 
 *                whose key matches are run.
 * @return FALSE if one of the comp() iterations returned zero; TRUE if all
 *            iterations returned non-zero
 * @see guththila_table_do_callback_fn_t
 */
int guththila_table_vdo(guththila_table_do_callback_fn_t *comp,
                               void *rec, const guththila_table_t *t, va_list vp);

/** flag for overlap to use guththila_table_setn */
#define GUTHTHILA_OVERLAP_TABLES_SET   (0)
/** flag for overlap to use guththila_table_mergen */
#define GUTHTHILA_OVERLAP_TABLES_MERGE (1)
/**
 * For each element in table b, either use setn or mergen to add the data
 * to table a.  Which method is used is determined by the flags passed in.
 * @param a The table to add the data to.
 * @param b The table to iterate over, adding its data to table a
 * @param flags How to add the table to table a.  One of:
 *          GUTHTHILA_OVERLAP_TABLES_SET        Use guththila_table_setn
 *          GUTHTHILA_OVERLAP_TABLES_MERGE      Use guththila_table_mergen
 * @remark  This function is highly optimized, and uses less memory and CPU cycles
 *          than a function that just loops through table b calling other functions.
 */
/**
 *<PRE>
 * Conceptually, guththila_table_overlap does this:
 *
 *  guththila_array_header_t *barr = guththila_table_elts(b);
 *  guththila_table_entry_t *belt = (guththila_table_entry_t *)barr->elts;
 *  int i;
 *
 *  for (i = 0; i < barr->nelts; ++i) {
 *      if (flags & GUTHTHILA_OVERLAP_TABLES_MERGE) {
 *          guththila_table_mergen(a, belt[i].key, belt[i].val);
 *      }
 *      else {
 *          guththila_table_setn(a, belt[i].key, belt[i].val);
 *      }
 *  }
 *
 *  Except that it is more efficient (less space and cpu-time) especially
 *  when b has many elements.
 *
 *  Notice the assumptions on the keys and values in b -- they must be
 *  in an ancestor of a's pool.  In practice b and a are usually from
 *  the same pool.
 * </PRE>
 */

void guththila_table_overlap(guththila_table_t *a, const guththila_table_t *b,
                                     unsigned flags);

/**
 * Eliminate redundant entries in a table by either overwriting
 * or merging duplicates
 *
 * @param t Table.
 * @param flags GUTHTHILA_OVERLAP_TABLES_MERGE to merge, or
 *              GUTHTHILA_OVERLAP_TABLES_SET to overwrite
 */
void guththila_table_compress(guththila_table_t *t, unsigned flags);

/** @} */

#ifdef __cplusplus
}
#endif

#endif	/* ! GUTHTHILA_TABLES_H */
