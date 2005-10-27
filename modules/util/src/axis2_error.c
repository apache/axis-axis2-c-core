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

#include <axis2_error.h>

axis2_char_t * AXIS2_CALL axis2_error_impl_get_message ();

AXIS2_DECLARE(axis2_error_t*)
axis2_error_create (axis2_allocator_t * allocator)
{
    axis2_error_t *error;
    if (!allocator)
        return NULL;

    error =
        (axis2_error_t *) axis2_malloc (allocator, sizeof (axis2_error_t));

    if (!error)
        return NULL;

    error->ops =
        (axis2_error_ops_t *) axis2_malloc (allocator,
                                            sizeof (axis2_error_ops_t));

    if (!error->ops)
    {
        axis2_free (allocator, error);
        return NULL;
    }

    error->ops->axis2_error_ops_get_message = axis2_error_impl_get_message;

    return error;
}

axis2_char_t * AXIS2_CALL
axis2_error_impl_get_message ()
{
    /** TODO: Need to fill in the error message list and get the error from that list */
    return "This is the default error code";
}
