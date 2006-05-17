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

#ifndef GUTHTHILA_HASH_H
#define GUTHTHILA_HASH_H

/**
 * @file guththila_hash.h
 * @brief Axis2 Hash Tables
 */

#include <guththila_defines.h>
#include <guththila_environment.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup guththila_hash Hash Tables
 * @ingroup guththila 
 * @{
 */

/**
 * When passing a key to guththila_hash_set or guththila_hash_get, this value can be
 * passed to indicate a string-valued key, and have guththila_hash compute the
 * length automatically.
 *
 * @remark guththila_hash will use strlen(key) for the length. The NUL terminator
 *         is not included in the hash value (why throw a constant in?).
 *         Since the hash table merely references the provided key (rather
 *         than copying it), guththila_hash_this() will return the NUL-term'd key.
 */
#define GUTHTHILA_HASH_KEY_STRING     (-1)

/**
 * Abstract type for hash tables.
 */
    typedef struct guththila_hash_t guththila_hash_t;

/**
 * Abstract type for scanning hash tables.
 */
    typedef struct guththila_hash_index_t guththila_hash_index_t;

/**
 * Callback functions for calculating hash values.
 * @param key The key.
 * @param klen The length of the key, or GUTHTHILA_HASH_KEY_STRING to use the string 
 *             length. If GUTHTHILA_HASH_KEY_STRING then returns the actual key length.
 */
    typedef unsigned int (*guththila_hashfunc_t) (const guththila_char_t *
                                                  key,
                                                  guththila_ssize_t * klen);

/**
 * The default hash function.
 */
    unsigned int guththila_hashfunc_default (const guththila_char_t * key,
                                             guththila_ssize_t * klen);

/**
 * Create a hash table.
 * @param environment The environment to allocate the hash table out of
 * @return The hash table just created
  */
      GUTHTHILA_DECLARE (guththila_hash_t *)
        guththila_hash_make (guththila_environment_t * environment);

/**
 * Create a hash table with a custom hash function
 * @param environment The environment to allocate the hash table out of
 * @param hash_func A custom hash function.
 * @return The hash table just created
  */
      GUTHTHILA_DECLARE (guththila_hash_t *)
        guththila_hash_make_custom (guththila_environment_t * environment,
                                    guththila_hashfunc_t hash_func);

/**
 * Make a copy of a hash table
 * @param environment The environment from which to allocate the new hash table
 * @param h The hash table to clone
 * @return The hash table just created
 * @remark Makes a shallow copy
 */
      GUTHTHILA_DECLARE (guththila_hash_t *)
        guththila_hash_copy (guththila_environment_t * environment,
                             const guththila_hash_t * h);

/**
 * Associate a value with a key in a hash table.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be GUTHTHILA_HASH_KEY_STRING to use the string length.
 * @param val Value to associate with the key
 * @remark If the value is NULL the hash entry is deleted.
 */
      GUTHTHILA_DECLARE (void) guththila_hash_set (guththila_hash_t * ht,
                                                   const void *key,
                                                   guththila_ssize_t klen,
                                                   const void *val);

/**
 * Look up the value associated with a key in a hash table.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be GUTHTHILA_HASH_KEY_STRING to use the string length.
 * @return Returns NULL if the key is not present.
 */
      GUTHTHILA_DECLARE (void *) guththila_hash_get (guththila_hash_t * ht,
                                                     const void *key,
                                                     guththila_ssize_t klen);

/**
 * Start iterating over the entries in a hash table.
 * @param p The environment to allocate the guththila_hash_index_t iterator. If this
 *          environment is NULL, then an internal, non-thread-safe iterator is used.
 * @param ht The hash table
 * @remark  There is no restriction on adding or deleting hash entries during
 * an iteration (although the results may be unpredictable unless all you do
 * is delete the current entry) and multiple iterations can be in
 * progress at the same time.

 * @example
 */
/**
 * <PRE>
 * 
 * int sum_values(guththila_environment_t *environment, guththila_hash_t *ht)
 * {
 *     guththila_hash_index_t *hi;
 *     void *val;
 *     int sum = 0;
 *     for (hi = guththila_hash_first(p, ht); hi; hi = guththila_hash_next(hi)) {
 *         guththila_hash_this(hi, NULL, NULL, &val);
 *         sum += *(int *)val;
 *     }
 *     return sum;
 * }
 * </PRE>
 */
      GUTHTHILA_DECLARE (guththila_hash_index_t *)
      guththila_hash_first (guththila_environment_t * environment,
                            guththila_hash_t * ht);

/**
 * Continue iterating over the entries in a hash table.
 * @param hi The iteration state
 * @return a pointer to the updated iteration state.  NULL if there are no more  
 *         entries.
 */
      GUTHTHILA_DECLARE (guththila_hash_index_t *)
      guththila_hash_next (guththila_hash_index_t * hi);

/**
 * Get the current entry's details from the iteration state.
 * @param hi The iteration state
 * @param key Return pointer for the pointer to the key.
 * @param klen Return pointer for the key length.
 * @param val Return pointer for the associated value.
 * @remark The return pointers should point to a variable that will be set to the
 *         corresponding data, or they may be NULL if the data isn't interesting.
 */
      GUTHTHILA_DECLARE (void) guththila_hash_this (guththila_hash_index_t *
                                                    hi, const void **key,
                                                    guththila_ssize_t * klen,
                                                    void **val);

/**
 * Get the number of key/value pairs in the hash table.
 * @param ht The hash table
 * @return The number of key/value pairs in the hash table.
 */
    unsigned int guththila_hash_count (guththila_hash_t * ht);

/**
 * Merge two hash tables into one new hash table. The values of the overlay
 * hash override the values of the base if both have the same key.  Both
 * hash tables must use the same hash function.
 * @param p The environment to use for the new hash table
 * @param overlay The table to add to the initial table
 * @param base The table that represents the initial values of the new table
 * @return A new hash table containing all of the data from the two passed in
 */
      GUTHTHILA_DECLARE (guththila_hash_t *)
      guththila_hash_overlay (guththila_environment_t * environment,
                                const guththila_hash_t * overlay,
                                const guththila_hash_t * base);

/**
 * Merge two hash tables into one new hash table. If the same key
 * is present in both tables, call the supplied merge function to
 * produce a merged value for the key in the new table.  Both
 * hash tables must use the same hash function.
 * @param p The environment to use for the new hash table
 * @param h1 The first of the tables to merge
 * @param h2 The second of the tables to merge
 * @param merger A callback function to merge values, or NULL to
 *  make values from h1 override values from h2 (same semantics as
 *  guththila_hash_overlay())
 * @param data Client data to pass to the merger function
 * @return A new hash table containing all of the data from the two passed in
 */
      GUTHTHILA_DECLARE (guththila_hash_t *)
      guththila_hash_merge (guththila_environment_t * environment,
                              const guththila_hash_t * h1,
                              const guththila_hash_t * h2,
                              void *(*merger) (guththila_environment_t *
                                               environment, const void *key,
                                               guththila_ssize_t klen,
                                               const void *h1_val,
                                               const void *h2_val,
                                               const void *data),
                              const void *data);

/**
 * Get a pointer to the environment which the hash table was created in
 */



GUTHTHILA_DECLARE(guththila_status_t) guththila_hash_free(
                            guththila_environment_t *environment,
                            guththila_hash_t *ht);

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* !GUTHTHILA_HASH_H */
