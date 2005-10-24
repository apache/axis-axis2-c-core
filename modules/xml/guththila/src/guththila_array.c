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
/*
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
*/
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

