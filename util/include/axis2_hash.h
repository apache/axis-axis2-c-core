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

#ifndef AXIS2_HASH_H
#define AXIS2_HASH_H

/**
 * @file axis2_hash.h
 * @brief Axis2 Hash Tables
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_hash Hash Tables
 * @ingroup axis2_util 
 * @{
 */

/**
 * When passing a key to axis2_hash_set or axis2_hash_get, this value can be
 * passed to indicate a string-valued key, and have axis2_hash compute the
 * length automatically.
 *
 * @remark axis2_hash will use strlen(key) for the length. The NUL terminator
 *         is not included in the hash value (why throw a constant in?).
 *         Since the hash table merely references the provided key (rather
 *         than copying it), axis2_hash_this() will return the NUL-term'd key.
 */
#define AXIS2_HASH_KEY_STRING     (-1)

/**
 * Abstract type for hash tables.
 */
    typedef struct axis2_hash_t axis2_hash_t;

/**
 * Abstract type for scanning hash tables.
 */
    typedef struct axis2_hash_index_t axis2_hash_index_t;

/**
 * Callback functions for calculating hash values.
 * @param key The key.
 * @param klen The length of the key, or AXIS2_HASH_KEY_STRING to use the string 
 *             length. If AXIS2_HASH_KEY_STRING then returns the actual key length.
 */
    typedef unsigned int (*axis2_hashfunc_t) (const char *key, axis2_ssize_t *klen);

/**
 * The default hash function.
 */
    unsigned int axis2_hashfunc_default (const char *key, axis2_ssize_t *klen);

/**
 * Create a hash table.
 * @param environment The environment to allocate the hash table out of
 * @return The hash table just created
  */
  AXIS2_DECLARE(axis2_hash_t*) axis2_hash_make (axis2_env_t **environment);

/**
 * Create a hash table with a custom hash function
 * @param environment The environment to allocate the hash table out of
 * @param hash_func A custom hash function.
 * @return The hash table just created
  */
   AXIS2_DECLARE(axis2_hash_t*) axis2_hash_make_custom (axis2_env_t **environment
   			, axis2_hashfunc_t hash_func);

/**
 * Make a copy of a hash table
 * @param ht The hash table to clone
 * @param environment The environment from which to allocate the new hash table
 * @return The hash table just created
 * @remark Makes a shallow copy
 */
  AXIS2_DECLARE(axis2_hash_t*) axis2_hash_copy (const axis2_hash_t *ht
  			, axis2_env_t **environment);

/**
 * Associate a value with a key in a hash table.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be AXIS2_HASH_KEY_STRING to use the string length.
 * @param val Value to associate with the key
 * @remark If the value is NULL the hash entry is deleted.
 */
  AXIS2_DECLARE(void) axis2_hash_set (axis2_hash_t *ht, const void *key
  			, axis2_ssize_t klen, const void *val);

/**
 * Look up the value associated with a key in a hash table.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be AXIS2_HASH_KEY_STRING to use the string length.
 * @return Returns NULL if the key is not present.
 */
   AXIS2_DECLARE(void*) axis2_hash_get (axis2_hash_t *ht, const void *key
   			, axis2_ssize_t klen);

/**
 * Start iterating over the entries in a hash table.
 * @param ht The hash table
 * @param p The environment to allocate the axis2_hash_index_t iterator. If this
 *          environment is NULL, then an internal, non-thread-safe iterator is used.
 * @remark  There is no restriction on adding or deleting hash entries during
 * an iteration (although the results may be unpredictable unless all you do
 * is delete the current entry) and multiple iterations can be in
 * progress at the same time.

 * @example
 */
/**
 * <PRE>
 * 
 * int sum_values(axis2_env_t *environment, axis2_hash_t *ht)
 * {
 *     axis2_hash_index_t *hi;
 *     void *val;
 *     int sum = 0;
 *     for (hi = axis2_hash_first(p, ht); hi; hi = axis2_hash_next(p, hi)) {
 *         axis2_hash_this(hi, NULL, NULL, &val);
 *         sum += *(int *)val;
 *     }
 *     return sum;
 * }
 * </PRE>
 */
 AXIS2_DECLARE(axis2_hash_index_t*) axis2_hash_first (axis2_hash_t *ht
 			,axis2_env_t **environment);

/**
 * Continue iterating over the entries in a hash table.
 * @param hi The iteration state
 * @return a pointer to the updated iteration state.  NULL if there are no more  
 *         entries.
 */
 AXIS2_DECLARE(axis2_hash_index_t*) axis2_hash_next (axis2_env_t **environment
 			, axis2_hash_index_t *hi);

/**
 * Get the current entry's details from the iteration state.
 * @param hi The iteration state
 * @param key Return pointer for the pointer to the key.
 * @param klen Return pointer for the key length.
 * @param val Return pointer for the associated value.
 * @remark The return pointers should point to a variable that will be set to the
 *         corresponding data, or they may be NULL if the data isn't interesting.
 */
 AXIS2_DECLARE(void) axis2_hash_this (axis2_hash_index_t *hi, const void **key
            , axis2_ssize_t *klen, void **val);

/**
 * Get the number of key/value pairs in the hash table.
 * @param ht The hash table
 * @return The number of key/value pairs in the hash table.
 */
  AXIS2_DECLARE( unsigned int ) axis2_hash_count (axis2_hash_t *ht);

/**
 * Merge two hash tables into one new hash table. The values of the overlay
 * hash override the values of the base if both have the same key.  Both
 * hash tables must use the same hash function.
 * @param overlay The table to add to the initial table
 * @param p The environment to use for the new hash table
 * @param base The table that represents the initial values of the new table
 * @return A new hash table containing all of the data from the two passed in
 */
  AXIS2_DECLARE(axis2_hash_t*) axis2_hash_overlay (const axis2_hash_t *overlay
  			,axis2_env_t **environment, const axis2_hash_t *base);

/**
 * Merge two hash tables into one new hash table. If the same key
 * is present in both tables, call the supplied merge function to
 * produce a merged value for the key in the new table.  Both
 * hash tables must use the same hash function.
 * @param h1 The first of the tables to merge
 * @param p The environment to use for the new hash table
 * @param h2 The second of the tables to merge
 * @param merger A callback function to merge values, or NULL to
 *  make values from h1 override values from h2 (same semantics as
 *  axis2_hash_overlay())
 * @param data Client data to pass to the merger function
 * @return A new hash table containing all of the data from the two passed in
 */
  AXIS2_DECLARE(axis2_hash_t*) axis2_hash_merge (const axis2_hash_t *h1
  			, axis2_env_t **environment, const axis2_hash_t *h2
            , void *(*merger) (axis2_env_t *environment, const void *key
            , axis2_ssize_t klen, const void *h1_val, const void *h2_val
            , const void *data), const void *data);

									
/**
 * @param ht hash table to be freed
 * @param environment The environment to use for hash table
 * @return return status code 
 *
 */
									
AXIS2_DECLARE(axis2_status_t) axis2_hash_free(axis2_hash_t *ht
		, axis2_env_t **environment);
									
/**
 * Free a hash table with hash table given as void
 * @param ht hash table to be freed as a void *
 * @param environment The environment to use for hash table
 * @return return status code
 */

AXIS2_DECLARE(axis2_status_t)
axis2_hash_free_void_arg (void *ht_void, axis2_env_t** environment);

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* !AXIS2_HASH_H */
