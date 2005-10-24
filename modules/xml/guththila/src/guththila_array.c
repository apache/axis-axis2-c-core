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




#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "guththila_array.h"

/*****************************************************************
 * This file contains guththila_array and guththila_table_t functions only.
 */

/*****************************************************************
 *
 * The 'array' functions...
 */

static void make_array_core(guththila_array_header_t *res, guththila_environment_t *environment,
			    int nelts, int elt_size, int clear)
{
    /*
     * Assure sanity if someone asks for
     * array of zero elts.
     */
    if (nelts < 1) {
        nelts = 1;
    }

    if (clear) {
        res->elts = guththila_malloc(environment->allocator, nelts * elt_size);
    }
    else {
        res->elts = guththila_malloc(environment->allocator, nelts * elt_size);
    }

    res->environment = environment;
    res->elt_size = elt_size;
    res->nelts = 0;		/* No active elements yet... */
    res->nalloc = nelts;	/* ...but this many allocated */
}

int guththila_is_empty_array(const guththila_array_header_t *a)
{
    return ((a == NULL) || (a->nelts == 0));
}

guththila_array_header_t * guththila_array_make(guththila_environment_t *environment,
						int nelts, int elt_size)
{
    guththila_array_header_t *res;

    res = (guththila_array_header_t *) guththila_malloc(environment->allocator, sizeof(guththila_array_header_t));
    make_array_core(res, environment, nelts, elt_size, 1);
    return res;
}

void * guththila_array_pop(guththila_array_header_t *arr)
{
    if (guththila_is_empty_array(arr)) {
        return NULL;
    }
   
    return arr->elts + (arr->elt_size * (--arr->nelts));
}

void * guththila_array_push(guththila_array_header_t *arr)
{
    if (arr->nelts == arr->nalloc) {
        int new_size = (arr->nalloc <= 0) ? 1 : arr->nalloc * 2;
        guththila_char_t *new_data;

        new_data = guththila_malloc(arr->environment->allocator, arr->elt_size * new_size);

        memcpy(new_data, arr->elts, arr->nalloc * arr->elt_size);
        memset(new_data + arr->nalloc * arr->elt_size, 0,
               arr->elt_size * (new_size - arr->nalloc));
        arr->elts = new_data;
        arr->nalloc = new_size;
    }

    ++arr->nelts;
    return arr->elts + (arr->elt_size * (arr->nelts - 1));
}

static void *guththila_array_push_noclear(guththila_array_header_t *arr)
{
    if (arr->nelts == arr->nalloc) {
        int new_size = (arr->nalloc <= 0) ? 1 : arr->nalloc * 2;
        guththila_char_t *new_data;

        new_data = guththila_malloc(arr->environment->allocator, arr->elt_size * new_size);

        memcpy(new_data, arr->elts, arr->nalloc * arr->elt_size);
        arr->elts = new_data;
        arr->nalloc = new_size;
    }

    ++arr->nelts;
    return arr->elts + (arr->elt_size * (arr->nelts - 1));
}

void guththila_array_cat(guththila_array_header_t *dst,
			       const guththila_array_header_t *src)
{
    int elt_size = dst->elt_size;

    if (dst->nelts + src->nelts > dst->nalloc) {
	int new_size = (dst->nalloc <= 0) ? 1 : dst->nalloc * 2;
	guththila_char_t *new_data;

	while (dst->nelts + src->nelts > new_size) {
	    new_size *= 2;
	}

	new_data = guththila_malloc(dst->environment->allocator, elt_size * new_size);
	memcpy(new_data, dst->elts, dst->nalloc * elt_size);

	dst->elts = new_data;
	dst->nalloc = new_size;
    }

    memcpy(dst->elts + dst->nelts * elt_size, src->elts,
	   elt_size * src->nelts);
    dst->nelts += src->nelts;
}

guththila_array_header_t * guththila_array_copy(guththila_environment_t *environment,
						const guththila_array_header_t *arr)
{
    guththila_array_header_t *res =
        (guththila_array_header_t *) guththila_malloc(environment->allocator, sizeof(guththila_array_header_t));
    make_array_core(res,environment, arr->nalloc, arr->elt_size, 0);

    memcpy(res->elts, arr->elts, arr->elt_size * arr->nelts);
    res->nelts = arr->nelts;
    memset(res->elts + res->elt_size * res->nelts, 0,
           res->elt_size * (res->nalloc - res->nelts));
    return res;
}

/* This cute function copies the array header *only*, but arranges
 * for the data section to be copied on the first push or arraycat.
 * It's useful when the elements of the array being copied are
 * read only, but new stuff *might* get added on the end; we have the
 * overhead of the full copy only where it is really needed.
 */

static  void copy_array_hdr_core(guththila_array_header_t *res,
					   const guththila_array_header_t *arr)
{
    res->elts = arr->elts;
    res->elt_size = arr->elt_size;
    res->nelts = arr->nelts;
    res->nalloc = arr->nelts;	/* Force overflow on push */
}

guththila_array_header_t *
    guththila_array_copy_hdr(guththila_environment_t *environment,
		       const guththila_array_header_t *arr)
{
    guththila_array_header_t *res;

    res = (guththila_array_header_t *) guththila_malloc(environment->allocator, sizeof(guththila_array_header_t));
    res->environment = environment;
    copy_array_hdr_core(res, arr);
    return res;
}

/* The above is used here to avoid consing multiple new array bodies... */

guththila_array_header_t *
    guththila_array_append(guththila_environment_t *environment,
		      const guththila_array_header_t *first,
		      const guththila_array_header_t *second)
{
    guththila_array_header_t *res = guththila_array_copy_hdr(environment, first);

    guththila_array_cat(res, second);
    return res;
}

/* guththila_array_pstrcat generates a new string containing
 * the concatenated sequence of substrings referenced as elements within
 * the array.  The string will be empty if all substrings are empty or null,
 * or if there are no elements in the array.
 * If sep is non-NUL, it will be inserted between elements as a separator.
 */
guththila_char_t * guththila_array_pstrcat(guththila_environment_t *environment,
				     const guththila_array_header_t *arr,
				     const guththila_char_t sep)
{
    guththila_char_t *cp, *res, **strpp;
    guththila_ssize_t len;
    int i;

    if (arr->nelts <= 0 || arr->elts == NULL) {    /* Empty table? */
        return (guththila_char_t *) guththila_malloc(environment->allocator, 1);
    }

    /* Pass one --- find length of required string */

    len = 0;
    for (i = 0, strpp = (guththila_char_t **) arr->elts; ; ++strpp) {
        if (strpp && *strpp != NULL) {
            len += guththila_strlen(environment->string,*strpp);
        }
        if (++i >= arr->nelts) {
            break;
	}
        if (sep) {
            ++len;
	}
    }

    /* Allocate the required string */

    res = (guththila_char_t *) guththila_malloc(environment->allocator, len + 1);
    cp = res;

    /* Pass two --- copy the argument strings into the result space */

    for (i = 0, strpp = (guththila_char_t **) arr->elts; ; ++strpp) {
        if (strpp && *strpp != NULL) {
            len = guththila_strlen(environment->string,*strpp);
            memcpy(cp, *strpp, len);
            cp += len;
        }
        if (++i >= arr->nelts) {
            break;
	}
        if (sep) {
            *cp++ = sep;
	}
    }

    *cp = '\0';

    /* Return the result string */

    return res;
}


/*****************************************************************
 *
 * The "table" functions.
 */

#if GUTHTHILA_CHARSET_EBCDIC
#define CASE_MASK 0xbfbfbfbf
#else
#define CASE_MASK 0xdfdfdfdf
#endif

#define TABLE_HASH_SIZE 32
#define TABLE_INDEX_MASK 0x1f
#define TABLE_HASH(key)  (TABLE_INDEX_MASK & *(guththila_unsigned_char_t *)(key))
#define TABLE_INDEX_IS_INITIALIZED(t, i) ((t)->index_initialized & (1 << (i)))
#define TABLE_SET_INDEX_INITIALIZED(t, i) ((t)->index_initialized |= (1 << (i)))

/* Compute the "checksum" for a key, consisting of the first
 * 4 bytes, normalized for case-insensitivity and packed into
 * an int...this checksum allows us to do a single integer
 * comparison as a fast check to determine whether we can
 * skip a strcasecmp
 */
#define COMPUTE_KEY_CHECKSUM(key, checksum)    \
{                                              \
    const guththila_char_t *k = (key);                     \
    guththila_ssize_t c = (guththila_ssize_t)*k;         \
    (checksum) = c;                            \
    (checksum) <<= 8;                          \
    if (c) {                                   \
        c = (guththila_ssize_t)*++k;                \
        checksum |= c;                         \
    }                                          \
    (checksum) <<= 8;                          \
    if (c) {                                   \
        c = (guththila_ssize_t)*++k;                \
        checksum |= c;                         \
    }                                          \
    (checksum) <<= 8;                          \
    if (c) {                                   \
        c = (guththila_ssize_t)*++k;                \
        checksum |= c;                         \
    }                                          \
    checksum &= CASE_MASK;                     \
}

/** The opaque string-content table type */
struct guththila_table_t {
    /* This has to be first to promote backwards compatibility with
     * older modules which cast a guththila_table_t * to an guththila_array_header_t *...
     * they should use the guththila_table_elts() function for most of the
     * cases they do this for.
     */
    /** The underlying array for the table */
    guththila_array_header_t a;
#ifdef MAKE_TABLE_PROFILE
    /** Who created the array. */
    void *creator;
#endif
    /* An index to speed up table lookups.  The way this works is:
     *   - Hash the key into the index:
     *     - index_first[TABLE_HASH(key)] is the offset within
     *       the table of the first entry with that key
     *     - index_last[TABLE_HASH(key)] is the offset within
     *       the table of the last entry with that key
     *   - If (and only if) there is no entry in the table whose
     *     key hashes to index element i, then the i'th bit
     *     of index_initialized will be zero.  (Check this before
     *     trying to use index_first[i] or index_last[i]!)
     */
    guththila_ssize_t index_initialized;
    int index_first[TABLE_HASH_SIZE];
    int index_last[TABLE_HASH_SIZE];
};

/*
 * NOTICE: if you tweak this you should look at is_empty_table() 
 * and table_elts() in alloc.h
 */
#ifdef MAKE_TABLE_PROFILE
static guththila_table_entry_t *table_push(guththila_table_t *t)
{
    if (t->a.nelts == t->a.nalloc) {
        return NULL;
    }
    return (guththila_table_entry_t *) guththila_array_push_noclear(&t->a);
}
#else /* MAKE_TABLE_PROFILE */
#define table_push(t)	((guththila_table_entry_t *) guththila_array_push_noclear(&(t)->a))
#endif /* MAKE_TABLE_PROFILE */

const guththila_array_header_t * guththila_table_elts(const guththila_table_t *t)
{
    return (const guththila_array_header_t *)t;
}

int guththila_is_empty_table(const guththila_table_t *t)
{
    return ((t == NULL) || (t->a.nelts == 0));
}

guththila_table_t * guththila_table_make(guththila_environment_t *environment, int nelts)
{
    guththila_table_t *t = guththila_malloc(environment->allocator, sizeof(guththila_table_t));

    make_array_core(&t->a,environment, nelts, sizeof(guththila_table_entry_t), 0);
#ifdef MAKE_TABLE_PROFILE
    t->creator = __builtin_return_address(0);
#endif
    t->index_initialized = 0;
    return t;
}

guththila_table_t * guththila_table_copy(guththila_environment_t *environment, const guththila_table_t *t)
{
    guththila_table_t *new = guththila_malloc(environment->allocator, sizeof(guththila_table_t));


    make_array_core(&new->a, environment, t->a.nalloc, sizeof(guththila_table_entry_t), 0);
    memcpy(new->a.elts, t->a.elts, t->a.nelts * sizeof(guththila_table_entry_t));
    new->a.nelts = t->a.nelts;
    memcpy(new->index_first, t->index_first, sizeof(int) * TABLE_HASH_SIZE);
    memcpy(new->index_last, t->index_last, sizeof(int) * TABLE_HASH_SIZE);
    new->index_initialized = t->index_initialized;
    return new;
}

static void table_reindex(guththila_table_t *t)
{
    int i;
    int hash;
    guththila_table_entry_t *next_elt = (guththila_table_entry_t *) t->a.elts;

    t->index_initialized = 0;
    for (i = 0; i < t->a.nelts; i++, next_elt++) {
        hash = TABLE_HASH(next_elt->key);
        t->index_last[hash] = i;
        if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
            t->index_first[hash] = i;
            TABLE_SET_INDEX_INITIALIZED(t, hash);
        }
    }
}

void guththila_table_clear(guththila_table_t *t)
{
    t->a.nelts = 0;
    t->index_initialized = 0;
}

const guththila_char_t * guththila_table_get(const guththila_table_t *t, const guththila_char_t *key)
{
    guththila_table_entry_t *next_elt;
    guththila_table_entry_t *end_elt;
    guththila_ssize_t checksum;
    int hash;

    if (key == NULL) {
	return NULL;
    }

    hash = TABLE_HASH(key);
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        return NULL;
    }
    COMPUTE_KEY_CHECKSUM(key, checksum);
    next_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_first[hash];;
    end_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_last[hash];

    for (; next_elt <= end_elt; next_elt++) {
	if ((checksum == next_elt->key_checksum) &&
            !stricmp(next_elt->key, key)) {
	    return next_elt->val;
	}
    }

    return NULL;
}

void guththila_table_set(guththila_table_t *t, const guththila_char_t *key,
                                const guththila_char_t *val)
{
    guththila_table_entry_t *next_elt;
    guththila_table_entry_t *end_elt;
    guththila_table_entry_t *table_end;
    guththila_ssize_t checksum;
    int hash;

    COMPUTE_KEY_CHECKSUM(key, checksum);
    hash = TABLE_HASH(key);
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        t->index_first[hash] = t->a.nelts;
        TABLE_SET_INDEX_INITIALIZED(t, hash);
        goto add_new_elt;
    }
    next_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_first[hash];;
    end_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_last[hash];
    table_end =((guththila_table_entry_t *) t->a.elts) + t->a.nelts;

    for (; next_elt <= end_elt; next_elt++) {
	if ((checksum == next_elt->key_checksum) &&
            !stricmp(next_elt->key, key)) {

            
            /* Found an existing entry with the same key, so overwrite it */

            int must_reindex = 0;
            guththila_table_entry_t *dst_elt = NULL;

            next_elt->val = guththila_strdup(t->a.environment->string, val);

            /* Remove any other instances of this key */
            for (next_elt++; next_elt <= end_elt; next_elt++) {
                if ((checksum == next_elt->key_checksum) &&
                    !stricmp(next_elt->key, key)) {
                    t->a.nelts--;
                    if (!dst_elt) {
                        dst_elt = next_elt;
                    }
                }
                else if (dst_elt) {
                    *dst_elt++ = *next_elt;
                    must_reindex = 1;
                }
            }

            /* If we've removed anything, shift over the remainder
             * of the table (note that the previous loop didn't
             * run to the end of the table, just to the last match
             * for the index)
             */
            if (dst_elt) {
                for (; next_elt < table_end; next_elt++) {
                    *dst_elt++ = *next_elt;
                }
                must_reindex = 1;
            }
            if (must_reindex) {
                table_reindex(t);
            }
            return;
        }
    }

add_new_elt:
    t->index_last[hash] = t->a.nelts;
    next_elt = (guththila_table_entry_t *) table_push(t);
    next_elt->key = guththila_strdup(t->a.environment->string, key);
    next_elt->val = guththila_strdup(t->a.environment->string, val);
    next_elt->key_checksum = checksum;
}

void guththila_table_setn(guththila_table_t *t, const guththila_char_t *key,
                                 const guththila_char_t *val)
{
    guththila_table_entry_t *next_elt;
    guththila_table_entry_t *end_elt;
    guththila_table_entry_t *table_end;
    guththila_ssize_t checksum;
    int hash;

    COMPUTE_KEY_CHECKSUM(key, checksum);
    hash = TABLE_HASH(key);
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        t->index_first[hash] = t->a.nelts;
        TABLE_SET_INDEX_INITIALIZED(t, hash);
        goto add_new_elt;
    }
    next_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_first[hash];;
    end_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_last[hash];
    table_end =((guththila_table_entry_t *) t->a.elts) + t->a.nelts;

    for (; next_elt <= end_elt; next_elt++) {
	if ((checksum == next_elt->key_checksum) &&
            !stricmp(next_elt->key, key)) {

            /* Found an existing entry with the same key, so overwrite it */

            int must_reindex = 0;
            guththila_table_entry_t *dst_elt = NULL;

            next_elt->val = (guththila_char_t *)val;

            /* Remove any other instances of this key */
            for (next_elt++; next_elt <= end_elt; next_elt++) {
                if ((checksum == next_elt->key_checksum) &&
                    !stricmp(next_elt->key, key)) {
                    t->a.nelts--;
                    if (!dst_elt) {
                        dst_elt = next_elt;
                    }
                }
                else if (dst_elt) {
                    *dst_elt++ = *next_elt;
                    must_reindex = 1;
                }
            }

            /* If we've removed anything, shift over the remainder
             * of the table (note that the previous loop didn't
             * run to the end of the table, just to the last match
             * for the index)
             */
            if (dst_elt) {
                for (; next_elt < table_end; next_elt++) {
                    *dst_elt++ = *next_elt;
                }
                must_reindex = 1;
            }
            if (must_reindex) {
                table_reindex(t);
            }
            return;
        }
    }

add_new_elt:
    t->index_last[hash] = t->a.nelts;
    next_elt = (guththila_table_entry_t *) table_push(t);
    next_elt->key = (guththila_char_t *)key;
    next_elt->val = (guththila_char_t *)val;
    next_elt->key_checksum = checksum;
}

void guththila_table_unset(guththila_table_t *t, const guththila_char_t *key)
{
    guththila_table_entry_t *next_elt;
    guththila_table_entry_t *end_elt;
    guththila_table_entry_t *dst_elt;
    guththila_ssize_t checksum;
    int hash;
    int must_reindex;

    hash = TABLE_HASH(key);
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        return;
    }
    COMPUTE_KEY_CHECKSUM(key, checksum);
    next_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_first[hash];
    end_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_last[hash];
    must_reindex = 0;
    for (; next_elt <= end_elt; next_elt++) {
	if ((checksum == next_elt->key_checksum) &&
            !stricmp(next_elt->key, key)) {

            /* Found a match: remove this entry, plus any additional
             * matches for the same key that might follow
             */
            guththila_table_entry_t *table_end = ((guththila_table_entry_t *) t->a.elts) +
                t->a.nelts;
            t->a.nelts--;
            dst_elt = next_elt;
            for (next_elt++; next_elt <= end_elt; next_elt++) {
                if ((checksum == next_elt->key_checksum) &&
                    !stricmp(next_elt->key, key)) {
                    t->a.nelts--;
                }
                else {
                    *dst_elt++ = *next_elt;
                }
            }

            /* Shift over the remainder of the table (note that
             * the previous loop didn't run to the end of the table,
             * just to the last match for the index)
             */
            for (; next_elt < table_end; next_elt++) {
                *dst_elt++ = *next_elt;
            }
            must_reindex = 1;
            break;
        }
    }
    if (must_reindex) {
        table_reindex(t);
    }
}
/*
void guththila_table_merge(guththila_table_t *t, const guththila_char_t *key,
				 const guththila_char_t *val)
{
    guththila_table_entry_t *next_elt;
    guththila_table_entry_t *end_elt;
    guththila_uint32_t checksum;
    int hash;

    COMPUTE_KEY_CHECKSUM(key, checksum);
    hash = TABLE_HASH(key);
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        t->index_first[hash] = t->a.nelts;
        TABLE_SET_INDEX_INITIALIZED(t, hash);
        goto add_new_elt;
    }
    next_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_first[hash];
    end_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_last[hash];

    for (; next_elt <= end_elt; next_elt++) {
	if ((checksum == next_elt->key_checksum) &&
            !strcasecmp(next_elt->key, key)) {

            /* Found an existing entry with the same key, so merge with it 
	    next_elt->val = guththila_pstrcat(t->a.pool, next_elt->val, ", ",
                                        val, NULL);
            return;
        }
        
    }

add_new_elt:
    t->index_last[hash] = t->a.nelts;
    next_elt = (guththila_table_entry_t *) table_push(t);
    next_elt->key = guththila_pstrdup(t->a.pool, key);
    next_elt->val = guththila_pstrdup(t->a.pool, val);
    next_elt->key_checksum = checksum;
}

void) guththila_table_mergen(guththila_table_t *t, const guththila_char_t *key,
				  const guththila_char_t *val)
{
    guththila_table_entry_t *next_elt;
    guththila_table_entry_t *end_elt;
    guththila_uint32_t checksum;
    int hash;

#if GUTHTHILA_POOL_DEBUG
    {
	if (!guththila_pool_is_ancestor(guththila_pool_find(key), t->a.pool)) {
	    fprintf(stderr, "guththila_table_mergen: key not in ancestor pool of t\n");
	    abort();
	}
	if (!guththila_pool_is_ancestor(guththila_pool_find(val), t->a.pool)) {
	    fprintf(stderr, "guththila_table_mergen: key not in ancestor pool of t\n");
	    abort();
	}
    }
#endif

    COMPUTE_KEY_CHECKSUM(key, checksum);
    hash = TABLE_HASH(key);
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        t->index_first[hash] = t->a.nelts;
        TABLE_SET_INDEX_INITIALIZED(t, hash);
        goto add_new_elt;
    }
    next_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_first[hash];;
    end_elt = ((guththila_table_entry_t *) t->a.elts) + t->index_last[hash];

    for (; next_elt <= end_elt; next_elt++) {
	if ((checksum == next_elt->key_checksum) &&
            !strcasecmp(next_elt->key, key)) {

            /* Found an existing entry with the same key, so merge with it 
	    next_elt->val = guththila_pstrcat(t->a.pool, next_elt->val, ", ",
                                        val, NULL);
            return;
        }
    }

add_new_elt:
    t->index_last[hash] = t->a.nelts;
    next_elt = (guththila_table_entry_t *) table_push(t);
    next_elt->key = (guththila_char_t *)key;
    next_elt->val = (guththila_char_t *)val;
    next_elt->key_checksum = checksum;
}
*/
void guththila_table_add(guththila_table_t *t, const guththila_char_t *key,
			       const guththila_char_t *val)
{
    guththila_table_entry_t *elts;
    guththila_ssize_t checksum;
    int hash;

    hash = TABLE_HASH(key);
    t->index_last[hash] = t->a.nelts;
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        t->index_first[hash] = t->a.nelts;
        TABLE_SET_INDEX_INITIALIZED(t, hash);
    }
    COMPUTE_KEY_CHECKSUM(key, checksum);
    elts = (guththila_table_entry_t *) table_push(t);
    elts->key = guththila_strdup(t->a.environment->string, key);
    elts->val = guththila_strdup(t->a.environment->string, val);
    elts->key_checksum = checksum;
}

void guththila_table_addn(guththila_table_t *t, const guththila_char_t *key,
				const guththila_char_t *val)
{
    guththila_table_entry_t *elts;
    guththila_ssize_t checksum;
    int hash;



    hash = TABLE_HASH(key);
    t->index_last[hash] = t->a.nelts;
    if (!TABLE_INDEX_IS_INITIALIZED(t, hash)) {
        t->index_first[hash] = t->a.nelts;
        TABLE_SET_INDEX_INITIALIZED(t, hash);
    }
    COMPUTE_KEY_CHECKSUM(key, checksum);
    elts = (guththila_table_entry_t *) table_push(t);
    elts->key = (guththila_char_t *)key;
    elts->val = (guththila_char_t *)val;
    elts->key_checksum = checksum;
}

guththila_table_t * guththila_table_overlay(guththila_environment_t *environment,
					     const guththila_table_t *overlay,
					     const guththila_table_t *base)
{
    guththila_table_t *res;


    res = guththila_malloc(environment->allocator, sizeof(guththila_table_t));
    /* behave like append_arrays */
    res->a.environment = environment;
    copy_array_hdr_core(&res->a, &overlay->a);
    guththila_array_cat(&res->a, &base->a);
    table_reindex(res);
    return res;
}

/* And now for something completely abstract ...

 * For each key value given as a vararg:
 *   run the function pointed to as
 *     int comp(void *r, char *key, char *value);
 *   on each valid key-value pair in the guththila_table_t t that matches the vararg key,
 *   or once for every valid key-value pair if the vararg list is empty,
 *   until the function returns false (0) or we finish the table.
 *
 * Note that we restart the traversal for each vararg, which means that
 * duplicate varargs will result in multiple executions of the function
 * for each matching key.  Note also that if the vararg list is empty,
 * only one traversal will be made and will cut short if comp returns 0.
 *
 * Note that the table_get and table_merge functions assume that each key in
 * the guththila_table_t is unique (i.e., no multiple entries with the same key).  This
 * function does not make that assumption, since it (unfortunately) isn't
 * true for some of Apache's tables.
 *
 * Note that rec is simply passed-on to the comp function, so that the
 * caller can pass additional info for the task.
 *
 * ADDENDUM for guththila_table_vdo():
 * 
 * The caching api will allow a user to walk the header values:
 *
 * guththila_status_t guththila_cache_el_header_walk(guththila_cache_el *el, 
 *    int (*comp)(void *, const char *, const char *), void *rec, ...);
 *
 * So it can be ..., however from there I use a  callback that use a va_list:
 *
 * guththila_status_t (*cache_el_header_walk)(guththila_cache_el *el, 
 *    int (*comp)(void *, const char *, const char *), void *rec, va_list);
 *
 * To pass those ...'s on down to the actual module that will handle walking
 * their headers, in the file case this is actually just an guththila_table - and
 * rather than reimplementing guththila_table_do (which IMHO would be bad) I just
 * called it with the va_list. For mod_shmem_cache I don't need it since I
 * can't use guththila_table's, but mod_file_cache should (though a good hash would
 * be better, but that's a different issue :). 
 *
 * So to make mod_file_cache easier to maintain, it's a good thing
 */
int guththila_table_do(guththila_table_do_callback_fn_t *comp,
                                     void *rec, const guththila_table_t *t, ...)
{
    int rv;

    va_list vp;
    va_start(vp, t);
    rv = guththila_table_vdo(comp, rec, t, vp);
    va_end(vp);

    return rv;
}

int guththila_table_vdo(guththila_table_do_callback_fn_t *comp,
                               void *rec, const guththila_table_t *t, va_list vp)
{
    guththila_char_t *argp;
    guththila_table_entry_t *elts = (guththila_table_entry_t *) t->a.elts;
    int vdorv = 1;

    argp = va_arg(vp, guththila_char_t *);
    do {
        int rv = 1, i;
        if (argp) {
            /* Scan for entries that match the next key */
            int hash = TABLE_HASH(argp);
            if (TABLE_INDEX_IS_INITIALIZED(t, hash)) {
                guththila_ssize_t checksum;
                COMPUTE_KEY_CHECKSUM(argp, checksum);
                for (i = t->index_first[hash];
                     rv && (i <= t->index_last[hash]); ++i) {
                    if (elts[i].key && (checksum == elts[i].key_checksum) &&
                                        !stricmp(elts[i].key, argp)) {
                        rv = (*comp) (rec, elts[i].key, elts[i].val);
                    }
                }
            }
        }
        else {
            /* Scan the entire table */
            for (i = 0; rv && (i < t->a.nelts); ++i) {
                if (elts[i].key) {
                    rv = (*comp) (rec, elts[i].key, elts[i].val);
                }
            }
        }
        if (rv == 0) {
            vdorv = 0;
        }
    } while (argp && ((argp = va_arg(vp, guththila_char_t *)) != NULL));

    return vdorv;
}

static guththila_table_entry_t **table_mergesort(guththila_environment_t *environment,
                                           guththila_table_entry_t **values, int n)
{
    /* Bottom-up mergesort, based on design in Sedgewick's "Algorithms
     * in C," chapter 8
     */
    guththila_table_entry_t **values_tmp =
        (guththila_table_entry_t **)guththila_malloc(environment->allocator, n * sizeof(guththila_table_entry_t*));
    guththila_ssize_t i;
    int blocksize;

    /* First pass: sort pairs of elements (blocksize=1) */
    for (i = 0; i + 1 < n; i += 2) {
        if (stricmp(values[i]->key, values[i + 1]->key) > 0) {
            guththila_table_entry_t *swap = values[i];
            values[i] = values[i + 1];
            values[i + 1] = swap;
        }
    }

    /* Merge successively larger blocks */
    blocksize = 2;
    while (blocksize < n) {
        guththila_table_entry_t **dst = values_tmp;
        int next_start;
        guththila_table_entry_t **swap;

        /* Merge consecutive pairs blocks of the next blocksize.
         * Within a block, elements are in sorted order due to
         * the previous iteration.
         */
        for (next_start = 0; next_start + blocksize < n;
             next_start += (blocksize + blocksize)) {

            int block1_start = next_start;
            int block2_start = block1_start + blocksize;
            int block1_end = block2_start;
            int block2_end = block2_start + blocksize;
            if (block2_end > n) {
                /* The last block may be smaller than blocksize */
                block2_end = n;
            }
            for (;;) {

                /* Merge the next two blocks:
                 * Pick the smaller of the next element from
                 * block 1 and the next element from block 2.
                 * Once either of the blocks is emptied, copy
                 * over all the remaining elements from the
                 * other block
                 */
                if (block1_start == block1_end) {
                    for (; block2_start < block2_end; block2_start++) {
                        *dst++ = values[block2_start];
                    }
                    break;
                }
                else if (block2_start == block2_end) {
                    for (; block1_start < block1_end; block1_start++) {
                        *dst++ = values[block1_start];
                    }
                    break;
                }
                if (stricmp(values[block1_start]->key,
                               values[block2_start]->key) > 0) {
                    *dst++ = values[block2_start++];
                }
                else {
                    *dst++ = values[block1_start++];
                }
            }
        }

        /* If n is not a multiple of 2*blocksize, some elements
         * will be left over at the end of the array.
         */
        for (i = dst - values_tmp; i < n; i++) {
            values_tmp[i] = values[i];
        }

        /* The output array of this pass becomes the input
         * array of the next pass, and vice versa
         */
        swap = values_tmp;
        values_tmp = values;
        values = swap;

        blocksize += blocksize;
    }

    return values;
}

void guththila_table_compress(guththila_table_t *t, unsigned flags)
{
    guththila_table_entry_t **sort_array;
    guththila_table_entry_t **sort_next;
    guththila_table_entry_t **sort_end;
    guththila_table_entry_t *table_next;
    guththila_table_entry_t **last;
    int i;
    int dups_found;

    if (t->a.nelts <= 1) {
        return;
    }

    /* Copy pointers to all the table elements into an
     * array and sort to allow for easy detection of
     * duplicate keys
     */
    sort_array = (guththila_table_entry_t **)
        guththila_malloc(t->a.environment->allocator, t->a.nelts * sizeof(guththila_table_entry_t*));
    sort_next = sort_array;
    table_next = (guththila_table_entry_t *)t->a.elts;
    i = t->a.nelts;
    do {
        *sort_next++ = table_next++;
    } while (--i);

    /* Note: the merge is done with mergesort instead of quicksort
     * because mergesort is a stable sort and runs in n*log(n)
     * time regardless of its inputs (quicksort is quadratic in
     * the worst case)
     */
    sort_array = table_mergesort(t->a.environment, sort_array, t->a.nelts);

    /* Process any duplicate keys */
    dups_found = 0;
    sort_next = sort_array;
    sort_end = sort_array + t->a.nelts;
    last = sort_next++;
    while (sort_next < sort_end) {
        if (((*sort_next)->key_checksum == (*last)->key_checksum) &&
            !stricmp((*sort_next)->key, (*last)->key)) {
            guththila_table_entry_t **dup_last = sort_next + 1;
            dups_found = 1;
            while ((dup_last < sort_end) &&
                   ((*dup_last)->key_checksum == (*last)->key_checksum) &&
                   !stricmp((*dup_last)->key, (*last)->key)) {
                dup_last++;
            }
            dup_last--; /* Elements from last through dup_last, inclusive,
                         * all have the same key
                         */
            if (flags == GUTHTHILA_OVERLAP_TABLES_MERGE) {
                guththila_ssize_t len = 0;
                guththila_table_entry_t **next = last;
                guththila_char_t *new_val;
                guththila_char_t *val_dst;
                do {
                    len += strlen((*next)->val);
                    len += 2; /* for ", " or trailing null */
                } while (++next <= dup_last);
                new_val = (guththila_char_t *)guththila_malloc(t->a.environment->allocator, len);
                val_dst = new_val;
                next = last;
                for (;;) {
                    strcpy(val_dst, (*next)->val);
                    val_dst += strlen((*next)->val);
                    next++;
                    if (next > dup_last) {
                        *val_dst = 0;
                        break;
                    }
                    else {
                        *val_dst++ = ',';
                        *val_dst++ = ' ';
                    }
                }
                (*last)->val = new_val;
            }
            else { /* overwrite */
                (*last)->val = (*dup_last)->val;
            }
            do {
                (*sort_next)->key = NULL;
            } while (++sort_next <= dup_last);
        }
        else {
            last = sort_next++;
        }
    }

    /* Shift elements to the left to fill holes left by removing duplicates */
    if (dups_found) {
        guththila_table_entry_t *src = (guththila_table_entry_t *)t->a.elts;
        guththila_table_entry_t *dst = (guththila_table_entry_t *)t->a.elts;
        guththila_table_entry_t *last_elt = src + t->a.nelts;
        do {
            if (src->key) {
                *dst++ = *src;
            }
        } while (++src < last_elt);
        t->a.nelts -= (int)(last_elt - dst);
    }

    table_reindex(t);
}

static void guththila_table_cat(guththila_table_t *t, const guththila_table_t *s)
{
    const int n = t->a.nelts;
    register int idx;

    guththila_array_cat(&t->a,&s->a);

    if (n == 0) {
        memcpy(t->index_first,s->index_first,sizeof(int) * TABLE_HASH_SIZE);
        memcpy(t->index_last, s->index_last, sizeof(int) * TABLE_HASH_SIZE);
        t->index_initialized = s->index_initialized;
        return;
    }

    for (idx = 0; idx < TABLE_HASH_SIZE; ++idx) {
        if (TABLE_INDEX_IS_INITIALIZED(s, idx)) {
            t->index_last[idx] = s->index_last[idx] + n;
            if (!TABLE_INDEX_IS_INITIALIZED(t, idx)) {
                t->index_first[idx] = s->index_first[idx] + n;
            }
        }
    }

    t->index_initialized |= s->index_initialized;
}

void guththila_table_overlap(guththila_table_t *a, const guththila_table_t *b,
				    unsigned flags)
{
    if (a->a.nelts + b->a.nelts == 0) {
        return;
    }



    guththila_table_cat(a, b);

    guththila_table_compress(a, flags);
}
